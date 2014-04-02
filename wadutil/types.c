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
}
