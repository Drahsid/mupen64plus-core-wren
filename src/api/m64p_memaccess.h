#if !defined(M64P_MEMACCESS_H)
#define M64P_MEMACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "m64p_types.h"

	// Shortened Types
	typedef  int8_t  s8;
	typedef uint8_t  u8;
	typedef  int16_t s16;
	typedef uint16_t u16;
	typedef  int32_t s32;
	typedef uint32_t u32;
	typedef  int64_t s64;
	typedef uint64_t u64;

	// RDRam Memory
	typedef const u8*(*ptr_read_rdram_buffer)(u32 addr, u32 length);
	typedef u32(*ptr_read_rdram_32)(u32 addr);
	typedef u16(*ptr_read_rdram_16)(u32 addr);
	typedef u8(*ptr_read_rdram_8)(u32 addr);

	typedef void(*ptr_write_rdram_32)(u32 addr, u32 value);
	typedef void(*ptr_write_rdram_16)(u32 addr, u16 value);
	typedef void(*ptr_write_rdram_8)(u32 addr, u8 value);

	// Rom Memory
	typedef const u8*(*ptr_read_rom_buffer)(u32 addr, u32 length);
	typedef u32(*ptr_read_rom_32)(u32 addr);
	typedef u16(*ptr_read_rom_16)(u32 addr);
	typedef u8(*ptr_read_rom_8)(u32 addr);

	typedef void(*ptr_write_rom_32)(u32 addr, u32 value);
	typedef void(*ptr_write_rom_16)(u32 addr, u16 value);
	typedef void(*ptr_write_rom_8)(u32 addr, u8 value);

	// Cart Info
	typedef u32(*ptr_get_cart_clock_rate)(void);
	typedef u32(*ptr_get_cart_pc)(void);
	typedef u32(*ptr_get_cart_release)(void);

	typedef u8(*ptr_get_cart_name)(void);
	typedef u32(*ptr_get_cart_manufacturer_id)(void);
	typedef u16(*ptr_get_cart_serial)(void);
	typedef u16(*ptr_get_cart_country_code)(void);

	// Hacking Function
	typedef void(*ptr_refresh_memory_cache)(void);

#if defined(M64P_CORE_PROTOTYPES)
	// RDRam Memory
	EXPORT const u8* CALL read_rdram_buffer(u32 addr, u32 length);
	EXPORT u32 CALL read_rdram_32(u32 addr);
	EXPORT u16 CALL read_rdram_16(u32 addr);
	EXPORT u8 CALL read_rdram_8(u32 addr);
	
	EXPORT void CALL write_rdram_32(u32 addr, u32 value);
	EXPORT void CALL write_rdram_16(u32 addr, u16 value);
	EXPORT void CALL write_rdram_8(u32 addr, u8 value);

	// Rom Memory
	EXPORT const unsigned char* CALL read_rom_buffer(u32 addr, u32 length);
	EXPORT u32 CALL read_rom_32(u32 addr);
	EXPORT u16 CALL read_rom_16(u32 addr);
	EXPORT u8 CALL read_rom_8(u32 addr);
	
	EXPORT void CALL write_rom_32(u32 addr, u32 value);
	EXPORT void CALL write_rom_16(u32 addr, u16 value);
	EXPORT void CALL write_rom_8(u32 addr, u8 value);

	// Cart Info

	EXPORT u32 CALL get_cart_clock_rate(void);
	EXPORT u32 CALL get_cart_pc(void);
	EXPORT u32 CALL get_cart_release(void);

	EXPORT u8 CALL get_cart_name(void);
	EXPORT u32 CALL get_cart_manufacturer_id(void);
	EXPORT u16 CALL get_cart_serial(void);
	EXPORT u16 CALL get_cart_country_code(void);

	
	// Hacking Function
	EXPORT void CALL refresh_memory_cache(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* #define M64P_MEMACCESS_H */