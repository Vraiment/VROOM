//
//  wad.c
//  wadutil
//
//  Created by Vraiment on 01/04/14.
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

#include "wad.h"

#include <stdlib.h>
#include <string.h>

char readWad(wad_t *wad, const char *name) {
	char type[5];
	FILE *file;
	
	file = fopen(name, "r");
	
	if (!file) {
		return 0;
	}
	
	wad->handle = file;
	
	wad->name = calloc(1, strlen(name) + 1);
	strcpy(wad->name, name);
	
	fseek(file, 0, SEEK_END);
	wad->size = ftell(file);
	
	if (wad->size < 12) {
		wad->type = NOWAD;
		return 1;
	}
	
	//Read WAD type
	fseek(file, 0, SEEK_SET);
	fread(type, 4, 1, file);
	type[4] = 0;
	
	if (!strcmp(type, "IWAD")) {
		wad->type = IWAD;
	} else if (!strcmp(type, "PWAD")) {
		wad->type = PWAD;
	} else {
		wad->type = NOWAD;
		return 1;
	}
	
	fread(&wad->lumpCount, 4, 2, file); //lump count and directory pos
	
	return 1;
}

void closeWad(wad_t *wad) {
	if (wad->handle) { fclose(wad->handle); }
	if (wad->name) { free(wad->name); }
}

void readLumpInfo(lumpInfo_t *lumpInfo, wad_t *wad, unsigned int lumpNumber) {
	lumpInfo->wad = wad;
	
	fseek(wad->handle, wad->dirPos + (lumpNumber * 16), SEEK_SET);
	
	fread(&lumpInfo->pos, 4, 2, wad->handle); //read lump position and size
	fread(&lumpInfo->name, 8, 1, wad->handle); //read lump name
	lumpInfo->name[8] = 0;
}

char findLumpInfo(lumpInfo_t *lumpInfo, wad_t *wad, const char *name) {
	char fname[9];
	unsigned int n;
	lumpInfo_t fLumpInfo;
	
	fseek(wad->handle, wad->dirPos + 8, SEEK_SET);
	fname[8] = 0;
	
	for (n = 0; n < wad->lumpCount; ++n) {
		readLumpInfo(&fLumpInfo, wad, n);
		
		if (!strcmp(fLumpInfo.name, name)) {
			memcpy(lumpInfo, &fLumpInfo, sizeof(lumpInfo_t));
			return 1;
		}
	}
	
	return 0;
}

void readPNames(pNames_t *pNames, lumpInfo_t *lumpInfo) {
	unsigned int n;
	
	pNames->wad = lumpInfo->wad;
	
	fseek(pNames->wad->handle, lumpInfo->pos, SEEK_SET);
	fread(&pNames->count, 4, 1, pNames->wad->handle);
	
	pNames->names = malloc(pNames->count * 9);
	for (n = 0; n < pNames->count; ++n) {
		fread(&pNames->names[n], 8, 1, pNames->wad->handle);
		pNames->names[n][8] = 0;
	}
}

texture_t *readTextures(lumpInfo_t *lumpInfo, unsigned int *count) {
	lumpInfo_t pNamesInfo;
	pNames_t pNames;
	texture_t *textures, *texture;
	patch_t *patch;
	unsigned int n, m, offset;
	unsigned short patchId;
	wad_t *wad;
	
	wad = lumpInfo->wad;
	
	if (!findLumpInfo(&pNamesInfo, wad, "PNAMES")) {
		*count = 1;
		return NULL;
	}
	
	readPNames(&pNames, &pNamesInfo);
	
	//Get textures count
	fseek(wad->handle, lumpInfo->pos, SEEK_SET);
	fread(count, 4, 1, wad->handle);
	
	textures = malloc(sizeof(texture_t) * *count);
	
	for (n = 0; n < *count; ++n) {
		texture = &textures[n];
		texture->name[8] = 0;
		
		//read offset of the texture
		fseek(wad->handle, lumpInfo->pos + 4 + (n * 4), SEEK_SET);
		fread(&offset, 4, 1, wad->handle);
		
		//read data of the texture
		fseek(wad->handle, lumpInfo->pos + offset, SEEK_SET);
		fread(texture->name, 8, 1, wad->handle);
		fread(&texture->masked, 4, 2, wad->handle); //masked, width and height
		
		fseek(wad->handle, 4, SEEK_CUR);
		fread(&texture->patchCount, 2, 1, wad->handle);
		
		if (!texture->patchCount) {
			continue;
		}
		
		texture->patches = malloc(sizeof(patch_t) * texture->patchCount);
		
		for (m = 0; m < texture->patchCount; ++m) {
			patch = &texture->patches[m];
			
			fread(&patch->origin, 2, 2, wad->handle);
			fread(&patchId, 2, 1, wad->handle);
			fread(&patch->stepdir, 2, 2, wad->handle);
			
			strcpy(patch->name, pNames.names[patchId]);
		}
	}
	
	free(pNames.names);
	
	return textures;
}

void freeTextures(texture_t *textures, unsigned int count) {
	unsigned int n;
	
	for (n = 0; n < count; ++n) {
		free(textures[n].patches);
	}
	
	free(textures);
}

void readColormaps(lumpInfo_t *lumpInfo, colormaps_t maps) {
	fseek(lumpInfo->wad->handle, lumpInfo->pos, SEEK_SET);
	fread(maps, COLORMAPCOUNT, COLORMAPSIZE, lumpInfo->wad->handle);
}
