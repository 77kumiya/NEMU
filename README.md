# NEMU

NEMU is a simplified but complete computer system emulator, including a virtual machine, an operating system and a monitor with common debugging features to support development on NEMU.

## Virtual Machine

Emulated logic of common computing hardware, including:

1. Intel 80386 CPU
    - Registers: general purpose registers, eflag register, segment registers, program counter, control registers, GDTR, IDTR.
    - ALU, FPU
    - Control Unit: instruction fetcher and decoder
    - MMU
    - Cache
    - i8259a interrupt controller
2. Main memory
3. Interrupt and exception handling mechanism
    - Exception: system calls
    - Interrupt: I/O interrupts
4. Common I/O devices
    - Disk
    - Keyboard
    - Serial
    - VGA
    - Timer

## Operating System

Provided following simplified and necessary support for applications programs:
1. Program loader
2. Virtual memory management
3. System calls
4. Interrupt handling mechanism
5. Device drivers

## Monitor

Provided commonly used debugging features:
1. Break points
2. Watch points
3. Single step execution
4. Register/memory examination
5. Expression evaluation with the support of symbols
