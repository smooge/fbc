/*
 *  libfb - FreeBASIC's runtime library
 *	Copyright (C) 2004-2005 Andre V. T. Vicentini (av1ctor@yahoo.com.br) and others.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * io_keys.c -- console input functions for Windows console mode apps
 *
 * chng: jun/2005 written [lillo]
 *       aug/2005 modified to read keyboard events [mjs]
 *
 */

#include <assert.h>
#include <ctype.h>
#include "fb.h"
#include "fb_rterr.h"

typedef struct _FB_KEY_CODES {
    unsigned short value_normal;
    unsigned short value_shift;
    unsigned short value_ctrl;
    unsigned short value_alt;
} FB_KEY_CODES;

typedef struct _FB_KEY_LIST_ENTRY {
    unsigned short            scan_code;
    const FB_KEY_CODES        codes;
} FB_KEY_LIST_ENTRY;

static const FB_KEY_LIST_ENTRY fb_ext_key_entries[] = {
    { 0x001C, { 0x000D, 0x000D, 0x000A, 0xA600 } },
    { 0x0035, { 0x002F, 0x003F, 0x9500, 0xA400 } },
    { 0x0047, { 0x4700, 0x4700, 0x7700, 0x9700 } },
    { 0x0048, { 0x4800, 0x4800, 0x8D00, 0x9800 } },
    { 0x0049, { 0x4900, 0x4900, 0x8400, 0x9900 } },
    { 0x004B, { 0x4B00, 0x4B00, 0x7300, 0x9B00 } },
    { 0x004D, { 0x4D00, 0x4D00, 0x7400, 0x9D00 } },
    { 0x004F, { 0x4F00, 0x4F00, 0x7500, 0x9F00 } },
    { 0x0050, { 0x5000, 0x5000, 0x9100, 0xA000 } },
    { 0x0051, { 0x5100, 0x5100, 0x7600, 0xA100 } },
    { 0x0052, { 0x5200, 0x5200, 0x9200, 0xA200 } },
    { 0x0053, { 0x5300, 0x5300, 0x9300, 0xA300 } }
};

#define FB_KEY_LIST_SIZE (sizeof(fb_ext_key_entries)/sizeof(FB_KEY_LIST_ENTRY))

static const FB_KEY_CODES fb_asc_key_codes[] = {
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x001B, 0x001B, 0x001B, 0x0100 },
    { 0x0031, 0x0021, 0x0000, 0x7800 },
    { 0x0032, 0x0040, 0x0300, 0x7900 },

    { 0x0033, 0x0023, 0x0000, 0x7A00 },
    { 0x0034, 0x0024, 0x0000, 0x7B00 },
    { 0x0035, 0x0025, 0x0000, 0x7C00 },
    { 0x0036, 0x005E, 0x001E, 0x7D00 },

    { 0x0037, 0x0026, 0x001F, 0x8200 },
    { 0x0038, 0x002B, 0x0000, 0x8300 },
    { 0x0039, 0x0028, 0x0000, 0x8000 },
    { 0x0030, 0x0029, 0x0000, 0x8100 },

    { 0x002D, 0x005F, 0x001F, 0x8200 },
    { 0x003D, 0x002B, 0x0000, 0x8300 },
    { 0x0008, 0x0008, 0x007F, 0xE000 },
    { 0x0009, 0x0F00, 0x9400, 0x0F00 },

    { 0x0071, 0x0051, 0x0011, 0x1000 }, /* 16 */
    { 0x0077, 0x0057, 0x0017, 0x1100 },
    { 0x0065, 0x0045, 0x0005, 0x1200 },
    { 0x0072, 0x0052, 0x0012, 0x1300 },

    { 0x0074, 0x0054, 0x0014, 0x1400 },
    { 0x0079, 0x0059, 0x0019, 0x1500 },
    { 0x0075, 0x0055, 0x0015, 0x1600 },
    { 0x0069, 0x0049, 0x0009, 0x1700 },

    { 0x006F, 0x004F, 0x000F, 0x1800 },
	{ 0x0070, 0x0050, 0x0010, 0x1900 },
	{ 0x005B, 0x007B, 0x001B, 0x1A00 },
	{ 0x005D, 0x007D, 0x001D, 0x1B00 },

    { 0x000D, 0x000D, 0x000A, 0x1C00 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0061, 0x0041, 0x0001, 0x1E00 },
	{ 0x0073, 0x0053, 0x0013, 0x1F00 },

    { 0x0064, 0x0044, 0x0004, 0x2000 }, /* 32 */
	{ 0x0066, 0x0046, 0x0006, 0x2100 },
	{ 0x0067, 0x0047, 0x0007, 0x2200 },
	{ 0x0068, 0x0048, 0x0008, 0x2300 },

    { 0x006A, 0x004A, 0x000A, 0x2400 },
	{ 0x006B, 0x004B, 0x000B, 0x2500 },
	{ 0x006C, 0x004C, 0x000C, 0x2600 },
	{ 0x003B, 0x003A, 0x0000, 0x2700 },

    { 0x0027, 0x0022, 0x0000, 0x2800 },
	{ 0x0060, 0x007E, 0x0000, 0x2900 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x005C, 0x007C, 0x001C, 0x0000 },

    { 0x007A, 0x005A, 0x001A, 0x2C00 },
	{ 0x0078, 0x0058, 0x0018, 0x2D00 },
	{ 0x0063, 0x0043, 0x0003, 0x2E00 },
	{ 0x0076, 0x0056, 0x0016, 0x2F00 },

    { 0x0062, 0x0042, 0x0002, 0x3000 }, /* 48 */
	{ 0x006E, 0x004E, 0x000E, 0x3100 },
	{ 0x006D, 0x004D, 0x000D, 0x3200 },
	{ 0x002C, 0x003C, 0x0000, 0x3300 },

    { 0x002E, 0x003E, 0x0000, 0x3400 },
	{ 0x002F, 0x003F, 0x0000, 0x3500 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x002A, 0x0000, 0x0072, 0x0000 },

    { 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0020, 0x0020, 0x0020, 0x0020 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x3B00, 0x5400, 0x5E00, 0x6800 },

    { 0x3C00, 0x5500, 0x5F00, 0x6900 },
	{ 0x3D00, 0x5600, 0x6000, 0x6A00 },
	{ 0x3E00, 0x5700, 0x6100, 0x6B00 },
	{ 0x3F00, 0x5800, 0x6200, 0x6C00 },

    { 0x4000, 0x5900, 0x6300, 0x6D00 }, /* 64 */
	{ 0x4100, 0x5A00, 0x6400, 0x6E00 },
	{ 0x4200, 0x5B00, 0x6500, 0x6F00 },
	{ 0x4300, 0x5C00, 0x6600, 0x7000 },

    { 0x4400, 0x5D00, 0x6700, 0x7100 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x4700, 0x0037, 0x7700, 0x0000 },

    { 0x4800, 0x0038, 0x8D00, 0x0000 },
	{ 0x4900, 0x0039, 0x8400, 0x0000 },
	{ 0x0000, 0x002D, 0x0000, 0x0000 },
	{ 0x4B00, 0x0034, 0x7300, 0x0000 },

    { 0x4C00, 0x0035, 0x8F00, 0x4C00 },
	{ 0x4D00, 0x0036, 0x7400, 0x0000 },
	{ 0x0000, 0x002B, 0x0000, 0x0000 },
	{ 0x4F00, 0x0031, 0x7500, 0x0000 },

    { 0x5000, 0x0032, 0x9100, 0x0000 }, /* 80 */
	{ 0x5100, 0x0033, 0x7600, 0x0000 },
	{ 0x5200, 0x0030, 0x9200, 0x0000 },
	{ 0x5300, 0x002E, 0x9300, 0x0000 },

    { 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x8500, 0x8700, 0x8900, 0x8B00 },

    { 0x8600, 0x8800, 0x8A00, 0x8C00 }
};

#define FB_KEY_CODES_SIZE (sizeof(fb_asc_key_codes)/sizeof(FB_KEY_CODES))

/** Translates an ASCII character, Virtual scan code and Virtual key code to
 *  a single QB-compatible keyboard code.
 *
 * @returns -1 if key not translatable
 */
int fb_hConsoleTranslateKey( char AsciiChar,
                             WORD wVsCode,
                             WORD wVkCode,
                             DWORD dwControlKeyState,
                             int bEnhancedKeysOnly )
{
    int KeyCode = 0, AddKeyCode = FALSE;
    int is_ext_code = AsciiChar==0;

    /* Process ENHANCED_KEY's in a different way */
    if( (dwControlKeyState & ENHANCED_KEY)!=0 && is_ext_code) {
        size_t i;
        for( i=0; i!=FB_KEY_LIST_SIZE; ++i ) {
            const FB_KEY_LIST_ENTRY *entry =
                fb_ext_key_entries + i;
            if(entry->scan_code==wVsCode) {
                const FB_KEY_CODES *codes = &entry->codes;
                if( dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED) ) {
                    KeyCode = codes->value_alt;
                    AddKeyCode = KeyCode!=0;
                } else if( dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED) ) {
                    KeyCode = codes->value_ctrl;
                    AddKeyCode = KeyCode!=0;
                } else if( dwControlKeyState & SHIFT_PRESSED ) {
                    KeyCode = codes->value_shift;
                    AddKeyCode = KeyCode!=0;
                } else {
                    KeyCode = codes->value_normal;
                    AddKeyCode = TRUE;
                }
                break;
            }
        }
    } else {
        unsigned uiAsciiChar = (unsigned) (unsigned char) AsciiChar;
        unsigned uiNormalKey, uiNormalKeyOtherCase;
        /* Test if we must translate a "normal" key into an enhanced key */
        if( wVsCode < FB_KEY_CODES_SIZE ) {
            const FB_KEY_CODES *codes = fb_asc_key_codes + wVsCode;

            uiNormalKey = MapVirtualKey( wVkCode, 2 ) & 0xFFFF;
            if( isupper( (int) uiNormalKey ) ) {
                uiNormalKeyOtherCase = tolower( (int) uiNormalKey );
            } else if( islower( (int) uiNormalKey ) ) {
                uiNormalKeyOtherCase = toupper( (int) uiNormalKey );
            } else {
                uiNormalKeyOtherCase = uiNormalKey;
            }

            if( dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED) ) {
                KeyCode = codes->value_alt;
            } else if( dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED) ) {
                KeyCode = codes->value_ctrl;
            } else if( dwControlKeyState & SHIFT_PRESSED ) {
                KeyCode = codes->value_shift;
            } else {
                if( uiAsciiChar==0 ) {
                    KeyCode = codes->value_normal;
                } else {
                    KeyCode = uiNormalKey;
                }
            }
            /* Add the found key code only when the following conditions are
             * met:
             * 1. KeyCode must be > 255 (enhanced)
             * 2. The ASCII character provided must be different from the
             *    "normal" character - this test is required to allow
             *    AltGr+character combinations that are language-specific
             *    and therefore quite hard to detect ... */
            AddKeyCode = (KeyCode > 255)
                && ((uiAsciiChar==uiNormalKey) || (uiAsciiChar==uiNormalKeyOtherCase));
        }

        if( !AddKeyCode && !bEnhancedKeysOnly) {
            if( !is_ext_code ) {
                /* The key code is simply the returned ASCII character */
                KeyCode = uiAsciiChar;
                AddKeyCode = TRUE;
            }
        }
    }

    if( AddKeyCode ) {
        if( KeyCode > 255 )
            KeyCode = FB_MAKE_EXT_KEY((char) (KeyCode >> 8));
        return KeyCode;
    }
    return -1;
}
