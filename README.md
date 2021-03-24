# NEMU
1. 用软件实现了对下列硬件的模拟：
  1) 计算器件(x86 CPU)
    a) 寄存器：通用寄存器、标志寄存器、段寄存器、程序计数器、控制寄存器、GDTR、IDTR；
    b) ALU、FPU运算器；
    c) 控制器：取指译码器；
    d) MMU内存管理单元；
    e) Cache；
	  f) i8259中断控制器。
  2) 内存。
  3) 异常和中断响应机制
    a) 异常：只实现了系统调用(int 0x80)；
    b) 中断：实现了外部设备IO中断。
  4) 一些常见的IO设备
    a) 键盘；
    b) 串口；
    c) 硬盘；
    d) 显示器；
    e) 时钟。

2. 一个简单的操作系统内核
  1) 程序加载；
  2) 虚存管理；
  3) 系统调用；
  4) 中断处理；
  5) 设备驱动。
  
3. 一个Debugger
  1) 单步执行；
  2) 扫描寄存器、内存；
  3) 表达式计算：支持+-*/=!=&&||!、解引用、括号、10进制整数、16进制整数、寄存器名称、符号；
  4) 设置和删除断点和监视点。
