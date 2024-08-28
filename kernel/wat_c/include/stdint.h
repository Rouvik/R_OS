/*  Provides basic definitions for sized types in C
    Copyright (C) 2024  Rouvik Maji

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Contact: majirouvik@gmail.com
*/

#ifndef __STDINT__
#define __STDINT__

// Base types
#define uint8_t unsigned char
#define int8_t char
#define uint16_t unsigned short
#define int16_t short
#define uint32_t unsigned long int
#define int32_t long int
#define uint64_t unsigned long long
#define int64_t long long

// Boolean
#define bool unsigned char
#define true 1
#define false 0

#endif // __STDINT__
