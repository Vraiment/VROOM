//
//  print.h
//  wadutil
//
//  Created by Vraiment on 02/04/14.
//  Copyright (c) 2014 Vraiment. All rights reserved.
//

#ifndef WADUTIL_PRINT
#define WADUTIL_PRINT

#include "types.h"

void printHelp();

void printHeader(wad_t *wad);

void printDirectory(wad_t *wad);

void printPNames(wad_t *wad);

void printTextures(wad_t *wad, const char *name);

#endif //WADUTIL_PRINT
