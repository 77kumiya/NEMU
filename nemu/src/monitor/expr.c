#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum
{
	NOTYPE = 256,
	PLUS,
	MINUS,
	MUL,
	DIV,
	LEFT_P,
	RIGHT_P,
	DEC,
	HEX,
	REG,
    SYMBLE,
	EQ,
	NEQ,
	AND,
    OR,
    NOT,
	DEREF,
    NEG
};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {
	{" +", NOTYPE}, 	// white space
	{"\\+", PLUS},		// plus
	{"-", MINUS},		// minus
	{"\\*", MUL},		// multiply
	{"/", DIV},		// divide
	{"\\(", LEFT_P},	// (
	{"\\)", RIGHT_P},	// )
	{"[0-9]+u?", DEC},	// decimal
	{"0x[0-9a-fA-F]+", HEX},// hex
	{"\\$[a-z]{2,3}", REG},	// register
    {"[_a-zA-Z]+[_0-9a-zA-Z]*", SYMBLE},        // symble
	{"==", EQ},		// equal
	{"!=", NEQ},		// not equal
	{"&&", AND},		// and
    {"\\|\\|", OR},     // or
    {"!", NOT}          // not
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))
#define NUM_TOKEN 32
#define TOKEN_LEN 32

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[TOKEN_LEN];
} Token;

Token tokens[NUM_TOKEN];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				int cur_type = rules[i].token_type;
				/* if the token is '*' (or '-'), it will be recognized as multiply (minus), need to distinguish it from dereference (negation)*/
				if(cur_type == MUL || cur_type == MINUS)
				{
					if(!nr_token)
					{
						cur_type = cur_type == MUL ? DEREF : NEG;
					}else
					{
						int pre_type = tokens[nr_token - 1].type;
						if(pre_type != DEC && pre_type != HEX && pre_type != REG && pre_type != RIGHT_P)
						{
							cur_type = cur_type == MUL ? DEREF : NEG;
						}
					}
				}
				/* do not record white spaces */
				if(cur_type == NOTYPE)
				{
					break;
				}
				/* handle cases when expressions are too long */
				if(nr_token == NUM_TOKEN)
				{
					printf("Too many tokens to handle!!!\n");
					return false;
				}
				if(substr_len >= TOKEN_LEN)
				{
					printf("There must be a token too long to hold!!!\n");
					return false;
				}
				tokens[nr_token].type = cur_type;
				switch (cur_type)
				{
				case DEC : 
					{
						int j;
						char* des = tokens[nr_token].str;
						if(*(substr_start + substr_len - 1) == 'u'){
							--substr_len;
						}
						for(j=0; j < substr_len; ++j){
							*(des + j) = *(substr_start + j);
						}
						*(des + j) = '\0';
					}
					break;
                case SYMBLE :
                    {
                        int j;
						char* des = tokens[nr_token].str;
						for(j=0; j < substr_len; ++j){
							*(des + j) = *(substr_start + j);
						}
						*(des + j) = '\0';
                    }
                    break;
				case HEX :
					{
						int j;
						char* des = tokens[nr_token].str;
						for(j = 2; j < substr_len; ++j){
							*(des + j - 2) = *(substr_start + j);
						}
						*(des + j) = '\0';
					}
					break;
				case REG :
					{
						int j;
						char* des = tokens[nr_token].str;
						for(j = 1; j < substr_len; ++j){
							*(des + j - 1) = *(substr_start + j);
						}
						*(des + j) = '\0';
					}
					break;
				default:
					break;
				}
				++nr_token;
				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}
uint32_t eval(int, int, bool*);
bool check_p_valid(int, int);

uint32_t expr(char *e, bool *success)
{
	*success = true;
	if (!make_token(e))
    {
		*success = false;
		printf("Token making failed!!!\n");
		return 0;
	}

    /* evaluate the expression */
	if(!check_p_valid(0, nr_token - 1))
    {
		*success = false;
		printf("Invalid parentheses!!!\n");
		return 0;
	}
	return eval(0, nr_token - 1, success);
}

bool check_p_valid(int p, int q)
{
	int num_p = 0;
	int i;
	bool valid = true;
	for(i = p; i <= q; ++i)
    {
		int type = tokens[i].type;
		if(type == LEFT_P)
        {
            ++num_p;
		}else if(type == RIGHT_P)
        {
			--num_p;
			if(num_p < 0)
            {
				valid = false;
				break;
			}
		}
	}
	valid = valid ? num_p == 0 : false;
	return valid;
}

bool check_parentheses(int p, int q)
{
	if(tokens[p].type != LEFT_P || tokens[q].type != RIGHT_P) return false;
	int num_p = 0;
	int i;
	bool ret = true;
	for(i = p; i <= q; ++i)
    {
		char type = tokens[i].type;
		if(type == LEFT_P)
        {
            ++num_p;
		}else if(type == RIGHT_P)
        {
			--num_p;
			if(num_p == 0)
            {
				ret = i == q;
				break;
			}
		}
	}
	return ret;
}

/* find main binary operator */
int find_main_op(int p, int q)
{
	int i;
	int in_p = 0;
	int ret[2] = {0};
	for(i = p; i <= q; ++i)
    {
		int type = tokens[i].type;
		if(type == LEFT_P)
        {
            ++in_p;
		}else if(type == RIGHT_P)
        {
            --in_p;
		}else if(in_p == 0)
        {
			int t_type;
			switch(type)
            {
                case OR:
                    ret[0] = type;
                    ret[1] = i;
                    break;
				case AND:
                    if(ret[0] != OR)
                    {
                        ret[0] = type;
					    ret[1] = i;
                    }
					break;
				case EQ:
				case NEQ:
					if(ret[0] != AND && ret[0] != OR)
                    {
						ret[0] = type;
						ret[1] = i;
					}
					break;
				case PLUS:
				case MINUS:
					t_type = ret[0];
					if(t_type != AND && t_type != OR && t_type != EQ && t_type != NEQ)
                    {
						ret[0] = type;
						ret[1] = i;
					}
					break;
				case MUL:
				case DIV:
					t_type = ret[0];
					if(t_type == 0 || t_type == MUL || t_type == DIV){
						ret[0] = type;
						ret[1] = i;
					}
					break;
			}
		}
	}
	return ret[1];
}

extern int get_reg_val(const char*, bool*);
extern uint32_t look_up_symtab(char *sym, bool *success);

uint32_t eval(int p, int q, bool *success){
	if(!success) return 0;
	if(p > q)
    {
		*success = false;
		printf("error: p > q\n");
		return 0;
	}
	else if(p == q)
    {
		int t_type = tokens[p].type;
		switch(t_type)
        {
			case DEC: return strtoul(tokens[p].str, NULL, 10);
			case HEX: return strtoul(tokens[p].str, NULL, 16);
			case REG: return get_reg_val(tokens[p].str, success);
            case SYMBLE: return look_up_symtab(tokens[p].str, success);
			default:
				*success = false;
				printf("Not a decimal or a hex or a register\n");
				return 0;
		}
	}
	else if(check_parentheses(p, q))
    {
		return eval(p + 1, q - 1, success);
	}
	else if(tokens[p].type == DEREF)
    {
		vaddr_t vaddr = eval(p + 1, q, success);
		return vaddr_read(vaddr, SREG_DS, 4); // segment doesn't matter since we are in flat mode
	}else if(tokens[p].type == NEG)
    {
        return -eval(p + 1, q, success);
    }else if(tokens[p].type == NOT)
    {
        return !eval(p + 1, q, success);	
    }else{
		int op = find_main_op(p, q);
		uint32_t val1 = eval(p, op - 1, success);
		uint32_t val2 = eval(op + 1, q, success);
		if(!success) return 0;
		switch(tokens[op].type)
        {
			case PLUS: return val1 + val2;
			case MINUS: return val1 - val2;
			case MUL: return val1 * val2;
			case DIV:
			{
			    if(val2 == 0)
                {
					*success = false;
					printf("I cannot divide zero!!!\n");
					return 0;
				}
                return val1 / val2;
			}
			case EQ: return val1 == val2;
			case NEQ: return val1 != val2;
			case AND: return val1 && val2;
            case OR: return val1 || val2;
			default: 
			{
			    *success = false;
				printf("Operator's missing!!!\n");
			}
		}
	}
	return 0;
}
