//
//  types.h
//  wadutil
//
//  Created by Vraiment on 01/04/14.
//  Copyright (c) 2014 Vraiment. All rights reserved.
//

#ifndef WADUTIL_WAD
#define WADUTIL_WAD

#include <stdio.h>

#define NOWAD 0
#define IWAD 1
#define PWAD 2

typedef struct {
	FILE *handle;
	char *name;
	long int size;
	
	//header
	char type;
	unsigned int lumpCount;
	unsigned int dirPos;
} wad_t;

char readWad(wad_t *wad, const char *name);
void closeWad(wad_t *wad);

typedef struct {
	wad_t *wad;
	char name[9];
	unsigned int pos;
	unsigned int size;
} lumpInfo_t;

void readLumpInfo(lumpInfo_t *lumpInfo, wad_t *wad, unsigned int lumpNumber);

#endif //WADUTIL_WAD
