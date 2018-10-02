#######################################################
# CS:APP Bomb Lab
# Directions to Instructors
#
# Copyright (c) 2003-2016, R. Bryant and D. O'Hallaron
#
#######################################################

This directory contains the files that you will use to build and run
the CS:APP Bomb Lab. The Bomb Lab teaches students principles of
machine-level programs, as well as general debugger and reverse
engineering skills.

***********
1. Overview
***********

----
1.1. Binary Bombs
----
A "binary bomb" is a Linux executable C program that consists of six
"phases." Each phase expects the student to enter a particular string
on stdin.  If the student enters the expected string, then that phase
is "defused."  Otherwise the bomb "explodes" by printing "BOOM!!!".
The goal for the students is to defuse as many phases as possible.

----
1.2. Solving Binary Bombs
----
In order to defuse the bomb, students must use a debugger, typically
gdb or ddd, to disassemble the binary and single-step through the
machine code in each phase. The idea is to understand what each
assembly statement does, and then use this knowledge to infer the
defusing string.  Students earn points for defusing phases, and they
lose points (configurable by the instructor, but typically 1/2 point)
for each explosion. Thus, they quickly learn to set breakpoints before
each phase and the function that explodes the bomb. It's a great
lesson and forces them to learn to use a debugger.

***********
2. Defuse Your Bomb
***********

Your job for this lab is to defuse your bomb.
You must do the assignment on one of the class machines. In fact, there is a rumor that Dr. Evil really is evil, and the bomb will always blow up if run elsewhere. There are several other tamper-proofing devices built into the bomb as well, or so we hear.
You can use many tools to help you defuse your bomb. Please look at the hints section for some tips and ideas. The best way is to use your favorite debugger to step through the disassembled binary.
Although phases get progressively harder to defuse, the expertise you gain as you move from phase to phase should offset this difficulty. However, the last phase will challenge even the best students, so please don¡¯t wait until the last minute to start.
The bomb ignores blank input lines. If you run your bomb with a command line argument, for example,
linux> ./bomb psol.txt
then it will read the input lines from psol.txt until it reaches EOF (end of file), and then switch over to stdin. In a moment of weakness, Dr. Evil added this feature so you don¡¯t have to keep retyping the solutions to phases you have already defused.
To avoid accidentally detonating the bomb, you will need to learn how to single-step through the assembly code and how to set breakpoints. You will also need to learn how to inspect both the registers and the memory states. One of the nice side-effects of doing the lab is that you will get very good at using a debugger. This is a crucial skill that will pay big dividends the rest of your career.


***********
3. Hints
***********

There are many tools which are designed to help you figure out both how programs work, and what is wrong when they don¡¯t work. Here is a list of some of the tools you may find useful in analyzing your bomb, and hints on how to use them.

3.1 gdb
The GNU debugger, this is a command line debugger tool available on virtually every platform. You can trace through a program line by line, examine memory and registers, look at both the source code and assembly code (we are not giving you the source code for most of your bomb), set breakpoints, set memory watch points, and write scripts.
The CS:APP web site
http://csapp.cs.cmu.edu/public/students.html
has a very handy single-page gdb summary that you can print out and use as a reference. Here are some other tips for using gdb.
¨C To keep the bomb from blowing up every time you type in a wrong input, you¡¯ll want to learn how to set breakpoints.
¨C For online documentation, type ¡°help¡± at the gdb command prompt, or type ¡°man gdb¡±,or ¡°info gdb¡± at a Unix prompt. Some people also like to run gdb under gdb-mode in emacs.

3.2 objdump -t
This will print out the bomb¡¯s symbol table. The symbol table includes the names of all functions and global variables in the bomb, the names of all the functions the bomb calls, and their addresses. You may learn something by looking at the function names!
3.3 objdump -d
Use this to disassemble all of the code in the bomb. You can also just look at individual functions.
Reading the assembler code can tell you how the bomb works.
Although objdump -d gives you a lot of information, it doesn¡¯t tell you the whole story. Calls to system-level functions are displayed in a cryptic form. For example, a call to sscanf might appear as:
8048c36: e8 99 fc ff ff call 80488d4 <_init+0x1a0>
To determine that the call was to sscanf, you would need to disassemble within gdb.
