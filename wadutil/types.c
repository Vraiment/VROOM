//
//  types.c
//  wadutil
//
//  Created by Vraiment on 01/04/14.
//  Copyright (c) 2014 Vraiment. All rights reserved.
//

#include "types.h"

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
	
	fread(&wad->lumpCount, 4, 2, file);
	
	return 1;
}

void closeWad(wad_t *wad) {
	if (wad->handle) { fclose(wad->handle); }
	if (wad->name) { free(wad->name); }
}

void readLumpInfo(lumpInfo_t *lumpInfo, wad_t *wad, unsigned int lumpNumber) {
	lumpInfo->wad = wad;
	
	fseek(wad->handle, wad->dirPos + (lumpNumber * 16), SEEK_SET);
	
	fread(&lumpInfo->pos, 4, 2, wad->handle);
	fread(&lumpInfo->name, 8, 1, wad->handle);
	lumpInfo->name[8] = 0;
}

char findLumpInfo(lumpInfo_t *lumpInfo, wad_t *wad, const char *name) {
	char fname[9];
	int n;
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
	int n;
	
	pNames->wad = lumpInfo->wad;
	
	fseek(pNames->wad->handle, lumpInfo->pos, SEEK_SET);
	fread(&pNames->count, 4, 1, pNames->wad->handle);
	
	pNames->names = malloc(pNames->count * 9);
	for (n = 0; n < pNames->count; ++n) {
		fread(&pNames->names[n], 8, 1, pNames->wad->handle);
		pNames->names[n][8] = 0;
	}
}
