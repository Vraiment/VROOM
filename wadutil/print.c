//
//  print.c
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

#include "print.h"

#include <stdlib.h>

void printManual() {
	printf("NAME\n");
	printf("\twadutil - WAD file dump utility\n\n");
	
	printf("SYNOPSIS\n");
	printf("\twadutil <filename> <option>\n\n");
	
	printf("DESCRIPTION\n");
	printf("\twadutil is a command line utility to dump data of a WAD file.\n");
	printf("\tIt requires a WAD file and only one option.\n\n");
	
	printf("OPTIONS\n");
	printf("\t--header\n\t\tPrints the header of the WAD file.\n\n");
	printf("\t-h\n\t\tSame as --header.\n\n");
	printf("\t--directory\n\t\tPrints the directory of the WAD file.\n\n");
	printf("\t-d\n\t\tSame as --directory.\n\n");
	printf("\t--PNAMES\n\t\tPrints the list of patch names of the WAD file.\n\n");
	printf("\t--TEXTURE1\n\t\tPrints the info about the TEXTURE1 lump.\n\n");
	printf("\t--TEXTURE2\n\t\tPrints the info about the TEXTURE2 lump.\n\n");
}

void printHelp() {
	printf("wadutil requires a WAD file and one or more valid options to work.\n");
	printf("to see a list of available options launch wadutil without any parameter.\n");
}

void printSeparator() {
	int n;
	
	for (n = 0; n < 50; ++n) {
		printf("-");
	}
	
	printf("\n");
}

void printHeader(wad_t *wad) {
	printf("=>Header\n");
	printf("\twad type:\t%s\n", (wad->type == IWAD) ? "IWAD" : "PWAD");
	printf("\tlump count:\t%d\n", wad->lumpCount);
	printf("\tdirectory pos:\t%d\n", wad->dirPos);
}


void printDirectory(wad_t *wad) {
	unsigned int n;
	lumpInfo_t lumpInfo;
	
	printf("=>Directory\n");
	
	for (n = 0; n < wad->lumpCount; ++n) {
		readLumpInfo(&lumpInfo, wad, n);
		
		printf("\t%d) %s => size: %d bytes, position: %d\n", n, lumpInfo.name, lumpInfo.size, lumpInfo.pos);
	}
	
	printf("\n\tTotal lumps:\t%d\n", wad->lumpCount);
}

void printPNames(wad_t *wad) {
	unsigned int n;
	lumpInfo_t lumpInfo;
	pNames_t pNames = { NULL, 0, NULL };
	
	if (!findLumpInfo(&lumpInfo, wad, "PNAMES")) {
		printf("\tno \"PNAMES\" lump found.\n");
		return;
	}
	
	printf("=>PNAMES\n");
	
	readPNames(&pNames, &lumpInfo);
	
	for (n = 0; n < pNames.count; ++n) {
		printf("\t%d) %s\n", n, pNames.names[n]);
	}
	
	printf("\n\tTotal patches:\t%d\n", pNames.count);
	
	free(pNames.names);
}

void printTextures(wad_t *wad, const char *name) {
	lumpInfo_t lumpInfo;
	unsigned int textureCount;
	texture_t *textures, *texture;
	patch_t *patch;
	unsigned int n, m;
	
	if (!findLumpInfo(&lumpInfo, wad, name)) {
		printf("\tno \"%s\" lump found.\n", name);
		return;
	}
	
	printf("=>%s\n", name);
	
	textures = readTextures(&lumpInfo, &textureCount);
	if (textureCount && !textures) {
		printf("\terror reading \"%s\" lump.\n", name);
	}
	
	for (n = 0; n < textureCount; ++n) {
		texture = &textures[n];
		
		printf("\t%d) %s ", n, texture->name);
		printf("(MASKED: %s, ", texture->masked ? "YES" : "NO");
		printf("%dx%d, ", texture->width, texture->height);
		printf("PATCHES: %d)\n", texture->patchCount);
		
		for (m = 0; m < texture->patchCount; ++m) {
			patch = &texture->patches[m];
			
			printf("\t\t%d -> %s: ", m, patch->name);
			printf("(%d, %d), ", patch->origin.x, patch->origin.y);
			printf("%d, %d\n", patch->stepdir, patch->colormap);
		}
		
		printf("\n");
	}
	
	printf("\n\tTotal textures in \"%s\":\t%d\n", name, textureCount);
	
	freeTextures(textures, textureCount);
}
