//
//  print.c
//  wadutil
//
//  Created by Vraiment on 02/04/14.
//  Copyright (c) 2014 Vraiment. All rights reserved.
//

#include "print.h"

#include <stdlib.h>

void printHelp() {
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

void printHeader(wad_t *wad) {
	printf("wad type:\t\t%s\n", (wad->type == IWAD) ? "IWAD" : "PWAD");
	printf("lump count:\t\t%d\n", wad->lumpCount);
	printf("directory pos:\t%d\n", wad->dirPos);
}


void printDirectory(wad_t *wad) {
	unsigned int n;
	lumpInfo_t lumpInfo;
	
	for (n = 0; n < wad->lumpCount; ++n) {
		readLumpInfo(&lumpInfo, wad, n);
		
		printf("%d) %s => size: %d bytes, position: %d\n", n, lumpInfo.name, lumpInfo.size, lumpInfo.pos);
	}
	
	printf("\nTotal:\t%d\n", wad->lumpCount);
}

void printPNames(wad_t *wad) {
	unsigned int n;
	lumpInfo_t lumpInfo;
	pNames_t pNames = { NULL, 0, NULL };
	
	if (!findLumpInfo(&lumpInfo, wad, "PNAMES")) {
		printf("no \"PNAMES\" lump found.\n");
		return;
	}
	
	readPNames(&pNames, &lumpInfo);
	
	for (n = 0; n < pNames.count; ++n) {
		printf("%d) %s\n", n, pNames.names[n]);
	}
	
	printf("\nTotal:\t%d\n", pNames.count);
	
	free(pNames.names);
}

void printTextures(wad_t *wad, const char *name) {
	lumpInfo_t lumpInfo;
	unsigned int textureCount;
	texture_t *textures, *texture;
	patch_t *patch;
	unsigned int n, m;
	
	if (!findLumpInfo(&lumpInfo, wad, name)) {
		printf("no \"%s\" lump found.\n", name);
		return;
	}
	
	textures = readTextures(&lumpInfo, &textureCount);
	if (textureCount && !textures) {
		printf("error reading \"%s\" lump.\n", name);
	}
	
	for (n = 0; n < textureCount; ++n) {
		texture = &textures[n];
		
		printf("%d) %s ", n, texture->name);
		printf("(MASKED: %s, ", texture->masked ? "YES" : "NO");
		printf("%dx%d, ", texture->width, texture->height);
		printf("PATCHES: %d)\n", texture->patchCount);
		
		for (m = 0; m < texture->patchCount; ++m) {
			patch = &texture->patches[m];
			
			printf("\t%d -> %s: ", m, patch->name);
			printf("(%d, %d), ", patch->origin.x, patch->origin.y);
			printf("%d, %d\n", patch->stepdir, patch->colormap);
		}
		
		printf("\n");
	}
	
	printf("\nTotal:\t%d\n", textureCount);
	
	freeTextures(textures, textureCount);
}
