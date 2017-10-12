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
seta20.64: #send 0xd1 to port 0x64
in $0x64, %al
test $0x2, %al
jnz seta20.60 #wait for empty input buffer
mov $0xd1, %al
out %al, $0x64

seta20.60: #send 0xdf to port 0x60
in $0x64, %al
test $0x2, %al
jnz seta20.60 #wait for empty input buffer
mov $0xdf, %al
out %al, $0x60

#clear bss
cbss:
mov bss_start, %ax
mov $0, (%ax)
mov bss_end, %bx
test %bx, %ax
jz cbssok
add $1, %ax
jmp cbss
cbssok:



.text

bootasm:
