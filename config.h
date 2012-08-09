/*
 *  Copyright (C) 2002-2009  The DOSBox Team
 *  Copyright (C) 2009-2010  Michał "MasterM" Siejak
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef WIN32
// TODO: Add windows macros here.
#else
#define GCC_ATTRIBUTE __attribute__
#define packed        (__packed__)
#endif

#include <inttypes.h>

typedef double Real64;
typedef  unsigned char Bit8u;
typedef    signed char Bit8s;
typedef unsigned short Bit16u;
typedef   signed short Bit16s;
typedef  unsigned long Bit32u;
typedef    signed long Bit32s;
typedef uint64_t Bit64u;
typedef int64_t Bit64s;
typedef unsigned int Bitu;
typedef signed int Bits;

typedef Bit32u PhysPt;
typedef Bit8u * HostPt;
typedef Bit32u RealPt;

// Helper log functions
int log_stdout(const char*, ...);
int log_file(const char*, ...);

// Application config
#define PROGNAME "ipxnet"
#define LOGNAME  "/home/intangir/log/ipxnet.log"
#define TICK_TIME 10000
#define LOG_TO_FILE

#ifdef LOG_TO_FILE
#define LOG_MSG log_file
#else
#define LOG_MSG log_stdout
#endif

#endif
