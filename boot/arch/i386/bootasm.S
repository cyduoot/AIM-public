/* Copyright (C) 2016 David Gao <davidgao1001@gmail.com>
 *
 * This file is part of AIM.
 *
 * AIM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AIM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

.set PROT_MODE_CSEG, 0x8
.set PROT_MODE_DSEG, 0x10
.set CR0_PE_ON,      0x1

.section .entry

.globl _start
_start:

.code16

cli #close interrupt
cld #string direction

# zero the segment register
xor %ax, %ax
mov %ax, %ds
mov %ax, %ss
mov %ax, %es


#Close A20s compatibility
seta20.1: #send 0xd1 to port 0x64
in $0x64, %al
test $0x2, %al
jnz seta20.1 #wait for empty input buffer
mov $0xd1, %al
out %al, $0x64

seta20.2: #send 0xdf to port 0x60
in $0x64, %al
test $0x2, %al
jnz seta20.2 #wait for empty input buffer
mov $0xdf, %al
out %al, $0x60

#switch to protected mode
lgdt gdtdesc
mov %cr0, %eax
or $CR0_PE_ON, %eax
mov %eax, %cr0

ljmp $PROT_MODE_CSEG, $bootasm

.text
.code32
bootasm:
#set the segment registers
mov $PROT_MODE_DSEG, %ax
mov %ax, %ds
mov %ax, %es
mov %ax, %fs
mov %ax, %gs
mov %ax, %ss

#set the stack
mov $start, %esp
call bootmain

#endless loop in case the kernal returns
loop:
    jmp loop

.data
.p2align 2
gdt:
    .byte 0,0,0,0,0,0,0,0                  #NULL
    .byte 0xFF,0xFF,0,0,0,0x9A,0xCF,0      #code segment(exec&read)
    .byte 0xFF,0xFF,0,0,0,0x92,0xCF,0      #data segment(write)

gdtdesc:
    .word 0x17
    .long gdt


#clear bss
#cbss:
#mov %bss_start, %ax
#mov $0, (%ax)
#mov %bss_end, %bx
#test %bx, %ax
#jz cbssok
#add $1, %ax
#jmp cbss
#cbssok:
