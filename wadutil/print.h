//
//  print.h
//  wadutil
//
//  Created by Vraiment on 02/04/14.
//  Copyright (c) 2014 Vraiment. All rights reserved.
//
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this software; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef WADUTIL_PRINT
#define WADUTIL_PRINT

#include "types.h"

void printManual();

void printHelp();

void printSeparator();

void printHeader(wad_t *wad);

void printDirectory(wad_t *wad);

void printPNames(wad_t *wad);

void printTextures(wad_t *wad, const char *name);

#endif //WADUTIL_PRINT
