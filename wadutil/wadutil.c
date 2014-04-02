//
//  wadutil.c
//  wadutil
//
//  Created by Vraiment on 01/04/14.
//  Copyright (c) 2014 Vraiment. All rights reserved.
//

#include "types.h"
#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cleanUp();

wad_t wad;

int main(int argc, const char * argv[]) {
	const char *mode;
	
#ifdef DEBUG
	char *d_argv[] = {
		"", //irrelevant
		"DOOM1.WAD", //file
		"--TEXTURE1" //mode
	};
	
	if (argc != 3) {
		*((char***)&argv) = d_argv; //HOW UGLY!
		argc = 3;
	}
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
		printHeader(&wad);
	} else if (!strcmp(mode, "-d") || !strcmp(mode, "--directory")) {
		printDirectory(&wad);
	} else if (!strcmp(mode, "--PNAMES")) {
		printPNames(&wad);
	} else if (!strcmp(mode, "--TEXTURE1") || !strcmp(mode, "--TEXTURE2")) {
		printTextures(&wad, &mode[2]);
	} else {
		printf("unknown option %s\n", mode);
	}
	
    return 0;
}

void cleanUp() {
	closeWad(&wad);
}
