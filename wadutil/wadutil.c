//
//  wadutil.c
//  wadutil
//
//  Created by Vraiment on 01/04/14.
//  Copyright (c) 2014 Vraiment. All rights reserved.
//

#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printHelp();

void cleanUp();

wad_t wad;

int main(int argc, const char * argv[]) {
	const char *mode;
	int n;
	lumpInfo_t lumpInfo;
	pNames_t pNames = { NULL, 0, NULL };
	
#ifdef DEBUG
	char *d_argv[] = {
		"", //irrelevant
		"DOOM1.WAD", //file
		"--pnames" //mode
	};
	
	*((char***)&argv) = d_argv; //HOW UGLY!
	argc = 3;
#endif
	
	atexit(cleanUp);
	
	if (argc < 3 || argc > 3) {
		printHelp();
		return 0;
	}
	
	if (!readWad(&wad, argv[1])) {
		printf("could not open %s.\n", argv[1]);
		return -1;
	}
	
	if (wad.type != NOWAD) {
		printf("reading file %s.\n", wad.name);
	} else {
		printf("%s is not a valid wad file.\n", wad.name);
		return -1;
	}
	
	mode = argv[2];
	
	printf("-------------------------\n");
	
	if (!strcmp(mode, "-h") || !strcmp(mode, "--header")) {
		printf("wad type:\t\t%s\n", (IWAD) ? "IWAD" : "PWAD");
		printf("lump count:\t\t%d\n", wad.lumpCount);
		printf("directory pos:\t%d\n", wad.dirPos);
	} else if (!strcmp(mode, "-d") || !strcmp(mode, "--directory")) {
		for (n = 0; n < wad.lumpCount; ++n) {
			readLumpInfo(&lumpInfo, &wad, n);
			
			printf("%d) %s => size: %d bytes, position: %d\n", n, lumpInfo.name, lumpInfo.size, lumpInfo.pos);
		}
		
		printf("\nTotal:\t%d\n", wad.lumpCount);
	} else if (!strcmp(mode, "--pnames")) {
		if (!findLumpInfo(&lumpInfo, &wad, "PNAMES")) {
			printf("no \"PNAMES\" lump found.\n");
			return 0;
		}
		
		readPNames(&pNames, &lumpInfo);
		
		for (n = 0; n < pNames.count; ++n) {
			printf("%d) %s\n", n, pNames.names[n]);
		}
		
		printf("\nTotal:\t%d\n", pNames.count);
		
		free(pNames.names);
	} else {
		printf("unknown option %s\n", mode);
	}
	
    return 0;
}

void printHelp() {
	printf("oh noes!, help!");
}

void cleanUp() {
	closeWad(&wad);
}
