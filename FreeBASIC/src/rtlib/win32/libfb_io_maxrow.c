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
 * io_maxrow.c -- get max row (console, no gfx) for Windows
 *
 * chng: nov/2004 written [v1ctor]
 *       jul/2005 mod: return 25 when function fails [mjs]
 *
 */

#include "fb.h"

/*:::::*/
int fb_ConsoleGetMaxRow( void )
{
    COORD max = GetLargestConsoleWindowSize( fb_out_handle );
    if( max.Y==0 )
        return FB_SCRN_DEFAULT_HEIGHT;
	return max.Y + 1;
}
