//
//  wadutil.c
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
#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_NONE			0x00
#define ARG_HEADER			0x01
#define ARG_DIRECTORY		0x02
#define ARG_PNAMES			0x04
#define ARG_TEXTURE1		0x08
#define ARG_TEXTURE2		0x10
#define ARG_COLORMAP		0x20

typedef struct {
	const char *fileName;
	char flags;
	const char *badArgument;
} arguments_t;

void cleanUp();

void parseArguments(int argc, const char *argv[], arguments_t *arguments);

wad_t wad;

int main(int argc, const char * argv[]) {
	arguments_t arguments;
	
	atexit(cleanUp);
	parseArguments(argc - 1, &argv[1], &arguments);
	
	if (!arguments.fileName) {
		printManual();
		return 0;
	}
	
	if (!readWad(&wad, arguments.fileName)) {
		printf("could not open %s.\n", argv[1]);
		return -1;
	}
	
	if (wad.type != NOWAD) {
		printf("reading file %s.\n", wad.name);
	} else {
		printf("%s is not a valid wad file.", wad.name);
		return -1;
	}
	
	if (arguments.badArgument) {
		printf("\n\"%s\" is not a valid option.\n\n", arguments.badArgument);
		printHelp();
		return 0;
	}
	
	if (arguments.flags == ARG_NONE) {
		printf("\nnothing to do.\n\n");
		printHelp();
		return 0;
	}
	
	if (arguments.flags & ARG_HEADER) {
		printSeparator();
		printHeader(&wad);
	}
	
	if (arguments.flags & ARG_DIRECTORY) {
		printSeparator();
		printDirectory(&wad);
	}
	
	if (arguments.flags & ARG_PNAMES) {
		printSeparator();
		printPNames(&wad);
	}
	
	if (arguments.flags & ARG_TEXTURE1) {
		printSeparator();
		printTextures(&wad, "TEXTURE1");
	}
	
	if (arguments.flags & ARG_TEXTURE2) {
		printSeparator();
		printTextures(&wad, "TEXTURE2");
	}
	
	if (arguments.flags & ARG_COLORMAP) {
		printSeparator();
		printColormaps(&wad);
	}
	
    return 0;
}

void cleanUp() {
	closeWad(&wad);
}

void parseArguments(int argc, const char *argv[], arguments_t *arguments) {
	int n;
	const char *arg;
	
	arguments->fileName = NULL;
	arguments->flags = ARG_NONE;
	arguments->badArgument = NULL;
	
	if (argc == 0) {
		return;
	}
	
	arguments->fileName = argv[0];
	
	for (n = 1; n < argc; ++n) {
		arg = argv[n];
		
		if (!strcmp(arg, "--header") || !strcmp(arg, "-h")) {
			arguments->flags |= ARG_HEADER;
		} else if (!strcmp(arg, "--directory") || !strcmp(arg, "-d")) {
			arguments->flags |= ARG_DIRECTORY;
		} else if (!strcmp(arg, "--PNAMES")) {
			arguments->flags |= ARG_PNAMES;
		} else if (!strcmp(arg, "--TEXTURE1")) {
			arguments->flags |= ARG_TEXTURE1;
		}  else if (!strcmp(arg, "--TEXTURE2")) {
			arguments->flags |= ARG_TEXTURE2;
		} else if (!strcmp(arg, "--COLORMAP")) {
			arguments->flags |= ARG_COLORMAP;
		} else {
			arguments->badArgument = arg;
			return;
		}
	}
}
