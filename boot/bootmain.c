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

#include <sys/types.h>
#include <aim/boot.h>
#include <elf.h>

#define SECTSIZE 512

const uint8_t magic[] = {
		0x7f, 0x45, 0x4c, 0x46
	};

void readseg(void*, uint32_t, uint32_t);

__noreturn
void bootmain(void)
{
	struct elf_hdr *elf;
	struct elf_phdr *ph, *eph;
	void (*entry)(void);
	void *pa;

	elf = (struct elf_hdr*)0x10000;

	readseg((void*)elf, 4096, 0);

	for (int i = 0; i < sizeof(magic); i++) {
		if (elf->e_ != magic[i]) goto bad;
	} // check elf magic number

//load segment
	ph = (struct elf_phdr*)((void*)elf + elf->phoff);
	eph = ph + elf->phnum;
	for (; ph < eph; ph++)
	{
		pa = (void*)ph->paddr;
		readseg(pa, ph->filesz, ph->offset);
	}

	entry = (void(*)(void))(elf->entry);
	entry();

bad:
	while (1);
}

void waitdisk(void)
{
	while ((inb(0x1F7) & 0xC0) != 0x40) ;
}

void readsect(void *dst, uint32_t offset)
{
	waitdisk();
	outb(0x1F2, 1);
    outb(0x1F3, offset);
    outb(0x1F4, offset >> 8);
    outb(0x1F5, offset >> 16);
    outb(0x1F6, (offset >> 24) | 0xE0);
    outb(0x1F7, 0x20);

    waitdisk();
    insl(0x1F0, dst, SECTSIZE/4);
}

void readseg(void* pa, uint32_t count, uint32_t offset)
{
	void *epa;
	epa = pa + count;
	pa −= offset % SECTSIZE;

    offset = (offset/SECTSIZE) + 1;

    for(; pa < epa; pa += SECTSIZE, offset++)
        readsect(pa, offset);
}
