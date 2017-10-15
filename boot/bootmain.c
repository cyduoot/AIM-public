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

const uint8_t magic[] = {
		0x7f, 0x45, 0x4c, 0x46
	};

__noreturn
void bootmain(void)
{
	struct elf_hdr *elf;
	struct elf_phdr *ph, *eph;
	void (*entry)(void);
	uchar *pa;

	elf = (struct elf_hdr*)0x10000;

	readseg((uchar*)elf, 4096, 0);

	for (int i = 0; i < sizeof(magic); i++) {
		if (elf->e_ != magic[i]) goto bad;
	} // check elf magic number

//load segment
	ph = (struct elf_phdr*)((uchar*)elf + elf->phoff);
	eph = ph + elf->phnum;
	for (; ph < eph; ph++)
	{
		pa = (uchar*)ph->paddr;
		readseg(pa, ph->filesz, ph->offset);
	}

	entry = (void(*)(void))(elf->entry);
	entry();

bad:
	while (1);
}
