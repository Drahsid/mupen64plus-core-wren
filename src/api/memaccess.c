/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   Mupen64plus-core - api/frontend.c                                     *
*   Mupen64Plus homepage: https://mupen64plus.org/                        *
*   Copyright (C) 2012 CasualJames                                        *
*   Copyright (C) 2009 Richard Goedeken                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "device/memory/memory.h"
#include "main/main.h"
#include "main/rom.h"
#include "main/savestates.h"
#include "m64p_memaccess.h"
#include <string.h>
#include <stdlib.h>

static u32 ram_addr_align(u32 address) { return (address & 0xffffff) >> 2; }
static u32 rom_addr_align(u32 address) { return address/4; }

// #########################################################
// ## RDRam Memory
// #########################################################

EXPORT u8 CALL read_rdram_8(u32 addr);
EXPORT const u8* CALL read_rdram_buffer(u32 addr, u32 length)
{
	const u8* value = malloc(length + 1);
	u8 val;

	for (size_t i = 0; i < length; i++) {
		val = read_rdram_8(addr + i);
		memcpy(value + i, &val, 1);
	}

	return value;
}

EXPORT u32 CALL read_rdram_32(u32 addr)
{
	struct device* dev = &g_dev;
	return (u32)dev->rdram.dram[ram_addr_align(addr)];
}

EXPORT u16 CALL read_rdram_16(u32 addr)
{
	return ((u16)read_rdram_8(addr) << 8) |
			(u16)read_rdram_8(addr+1);
}

EXPORT u8 CALL read_rdram_8(u32 addr)
{
	struct device* dev = &g_dev;
	size_t offset = (ram_addr_align(addr) * 4) + (3 - addr & 3);
	return (u8)(((u8*)dev->rdram.dram)[offset]);
}

// -------------------------------------------

EXPORT void CALL write_rdram_32(u32 addr, u32 value)
{
	struct device* dev = &g_dev;
	size_t offset = ram_addr_align(addr) * 4;
	memcpy((u8*)dev->rdram.dram + offset, &value, 4);
}

EXPORT void CALL write_rdram_8(u32 addr, u8 value);
EXPORT void CALL write_rdram_16(u32 addr, u16 value)
{
	write_rdram_8(addr, (u8)(value >> 8));
    write_rdram_8(addr + 1, (u8)(value & 0xFF));
}

EXPORT void CALL write_rdram_8(u32 addr, u8 value)
{
	struct device* dev = &g_dev;
	size_t offset = (ram_addr_align(addr) * 4) + (3 - addr & 3);
	((u8*)dev->rdram.dram)[offset] = value;
}

// #########################################################
// ## Rom Memory
// #########################################################

#ifdef WIN32
#define __bswap_32(x) ((u32)_byteswap_ulong(x))
#else
#define __bswap_32(x) ((u32)__builtin_bswap32(x))
#endif

EXPORT u8 CALL read_rom_8(u32 addr);
EXPORT const u8* CALL read_rom_buffer(u32 addr, u32 length)
{
	if (!g_EmulatorRunning) {
		u8* rom = (u8*)mem_base_u32(g_mem_base, MM_CART_ROM);
		const u8* value = malloc(length + 1);
		memcpy(value, rom + addr, length);
		return value;
	} else {
		const u8* value = malloc(length + 1);
		u8 val;

		for (size_t i = 0; i < length; i++) {
			val = read_rom_8(addr + i);
			memcpy(value + i, &val, 1);
		}

		return value;
	}
}

EXPORT u32 CALL read_rom_32(u32 addr)
{
	size_t offset = rom_addr_align(addr) * 4;
	u32 value = mem_base_u32(g_mem_base, MM_CART_ROM)[offset];

	if (!g_EmulatorRunning)
		value = __bswap_32(value);
		
	return value;
}

EXPORT u16 CALL read_rom_16(u32 addr)
{
	return ((u16)read_rom_8(addr) << 8) |
			(u16)read_rom_8(addr+1);
}

EXPORT u8 CALL read_rom_8(u32 addr)
{
	if (!g_EmulatorRunning) {
		return (u8)((u8*)mem_base_u32(g_mem_base, MM_CART_ROM)[addr]);
	} else {
		size_t offset = (rom_addr_align(addr) * 4) + (3 - addr & 3);
		return (u8)(((u8*)mem_base_u32(g_mem_base, MM_CART_ROM))[offset]);
	}
}

// -------------------------------------------

EXPORT void CALL write_rom_32(u32 addr, u32 value)
{
	u32* rom = mem_base_u32(g_mem_base, MM_CART_ROM);
	size_t offset = rom_addr_align(addr);

	if (!g_EmulatorRunning)
		value = __bswap_32(value);

	rom[offset] = value;
}

EXPORT void CALL write_rom_8(u32 addr, u8 value);
EXPORT void CALL write_rom_16(u32 addr, u16 value)
{
	write_rom_8(addr, value >> 8);
	write_rom_8(addr + 1, value & 0xFF);
}

EXPORT void CALL write_rom_8(u32 addr, u8 value)
{
	if (!g_EmulatorRunning) {
		((u8*)mem_base_u32(g_mem_base, MM_CART_ROM))[addr] = value;
	} else {
		size_t offset = (rom_addr_align(addr) * 4) + (3 - addr & 3);
		((u8*)mem_base_u32(g_mem_base, MM_CART_ROM))[offset] = value;
	}
}

// #########################################################
// ## Cart Info
// #########################################################

EXPORT u32 CALL get_cart_clock_rate(void) { return ROM_HEADER.ClockRate; }
EXPORT u32 CALL get_cart_pc(void) { return ROM_HEADER.PC; }
EXPORT u32 CALL get_cart_release(void) { return ROM_HEADER.Release; }

EXPORT u8 CALL get_cart_name(void) { return ROM_HEADER.Name; }
EXPORT u32 CALL get_cart_manufacturer_id(void) { return ROM_HEADER.Manufacturer_ID; }
EXPORT u16 CALL get_cart_serial(void) { return ROM_HEADER.Cartridge_ID; }
EXPORT u16 CALL get_cart_country_code(void) { return ROM_HEADER.Country_code; }

