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
char findLumpInfo(lumpInfo_t *lumpInfo, wad_t *wad, const char *name);

typedef struct {
	wad_t *wad;
	unsigned int count;
	char (*names)[9];
} pNames_t;

void readPNames(pNames_t *pNames, lumpInfo_t *lumpInfo);

typedef struct {
	struct {
		unsigned short x;
		unsigned short y;
	} origin;
	char name[9];
	unsigned short stepdir;
	unsigned short colormap;
} patch_t;

typedef struct {
	char name[9];
	unsigned int masked;
	unsigned short width;
	unsigned short height;
	unsigned short patchCount;
	patch_t *patches;
} texture_t;

texture_t *readTextures(lumpInfo_t *lumpInfo, unsigned int *count);
void freeTextures(texture_t *textures, unsigned int count);

#endif //WADUTIL_WAD
