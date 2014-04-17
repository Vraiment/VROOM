// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for SDL.
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <errno.h>
#include <signal.h>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"

#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Surface *surface;

SDL_Event sdl_event;
int lastMouse;

static int multiply = 1;

int realWidth, centerX;
int realHeight, centerY;

boolean grabMouse;

int GetKey(SDL_Keycode key) {
	switch (key) {
		case SDLK_LEFT: return KEY_LEFTARROW;
		case SDLK_RIGHT: return KEY_RIGHTARROW;
		case SDLK_DOWN: return KEY_DOWNARROW;
		case SDLK_UP: return KEY_UPARROW;
		case SDLK_ESCAPE: return KEY_ESCAPE;
		case SDLK_RETURN: return KEY_ENTER;
		case SDLK_TAB: return KEY_TAB;
		case SDLK_F1: return KEY_F1;
		case SDLK_F2: return KEY_F2;
		case SDLK_F3: return KEY_F3;
		case SDLK_F4: return KEY_F4;
		case SDLK_F5: return KEY_F5;
		case SDLK_F6: return KEY_F6;
		case SDLK_F7: return KEY_F7;
		case SDLK_F8: return KEY_F8;
		case SDLK_F9: return KEY_F9;
		case SDLK_F10: return KEY_F10;
		case SDLK_F11: return KEY_F11;
		case SDLK_F12: return KEY_F12;
		
		case SDLK_BACKSPACE:
		case SDLK_DELETE: return KEY_BACKSPACE;
		
		case SDLK_PAUSE: return KEY_PAUSE;
		
		case SDLK_KP_EQUALS:
		case SDLK_EQUALS: return KEY_EQUALS;
		
		case SDLK_KP_MINUS:
		case SDLK_MINUS: return KEY_MINUS;
		
		case SDLK_LSHIFT:
		case SDLK_RSHIFT: return KEY_RSHIFT;
		
		case SDLK_LCTRL:
		case SDLK_RCTRL: return KEY_RCTRL;
		
		case SDLK_LALT:
		case SDLK_RALT: return KEY_RALT;
		default: return key; //letters are their ascii value
	}
}

void RetrieveEvents() {
	event_t event;
	Uint8 button;
	
	while (SDL_PollEvent(&sdl_event)) {
		switch (sdl_event.type) {
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				event.type = (sdl_event.type == SDL_KEYDOWN) ? ev_keydown : ev_keyup;
				event.data1 = GetKey(sdl_event.key.keysym.sym);
				D_PostEvent(&event);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				event.type = ev_mouse;
				event.data1 = lastMouse;
				
				button = sdl_event.button.button;
				
				if (sdl_event.type == SDL_MOUSEBUTTONDOWN) {
					event.data1 |= button == SDL_BUTTON_LEFT;
					event.data1 |= button == SDL_BUTTON_MIDDLE ? 2 : 0;
					event.data1 |= button == SDL_BUTTON_RIGHT ? 4 : 0;
				} else {
					event.data1 ^= button == SDL_BUTTON_LEFT;
					event.data1 ^= button == SDL_BUTTON_MIDDLE ? 2 : 0;
					event.data1 ^= button == SDL_BUTTON_RIGHT ? 4 : 0;
				}
				
				event.data2 = event.data3 = 0;
				
				D_PostEvent(&event);
				
				lastMouse = event.data1;
				break;
			case SDL_MOUSEMOTION:
				if (!grabMouse) {
					break; //X11 acts this way
				}
				
				event.type = ev_mouse;
				
				event.data1 = 0;
				event.data2 = sdl_event.motion.xrel << 2;
				event.data3 = -(sdl_event.motion.yrel << 2);
				
				D_PostEvent(&event);
				break;
			case SDL_QUIT:
				I_Quit();
				break;
		}
	}
}

//
// I_ShutdownGraphics
//
void I_ShutdownGraphics()
{
	if (surface) {
		SDL_FreeSurface(surface);
	}
	
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	
	if (window) {
		SDL_DestroyWindow(window);
	}
}

//
// I_StartFrame
//
void I_StartFrame ()
{
	
}

//
// I_StartTic
//
void I_StartTic ()
{
	RetrieveEvents();
}

//
// I_UpdateNoBlit
//
void I_UpdateNoBlit ()
{
	
}

//
// I_FinishUpdate
//
void I_FinishUpdate ()
{
	static int lasttic;
	int tics;
	int i;
	SDL_Texture *texture;
	
	if (devparm) {
		i = I_GetTime();
		tics = i - lasttic;
		lasttic = i;
		
		if (tics > 20) {
			tics = 20;
		}
		
		for (i = 0; i < (tics * 2); i += 2) {
			screens[0][(SCREENHEIGHT - 1) * SCREENWIDTH + i] = 0xFF;
		}
		
		for (; i < (20 * 2); i += 2) {
			screens[0][(SCREENHEIGHT - 1) * SCREENWIDTH + i] = 0x00;
		}
	}
	
	if (multiply == 2) {
		I_Error("-2 switch not implemented");
	} else if (multiply == 3) {
		I_Error("-3 switch not implemented");
	} else if (multiply == 4) {
		I_Error("-4 switch not implemented");
	}
	
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

//
// I_ReadScreen
//
void I_ReadScreen (byte* scr)
{
	memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

//
// I_SetPalette
//
void I_SetPalette (byte* palette)
{
	int n;
	int c;
	SDL_Palette *sdl_palette;
	
	sdl_palette = surface->format->palette;
	
	for (n = 0; n < sdl_palette->ncolors; ++n) {
		c = gammatable[usegamma][*palette++];
		sdl_palette->colors[n].r = (c << 8) + c;
		
		c = gammatable[usegamma][*palette++];
		sdl_palette->colors[n].g = (c << 8) + c;

		c = gammatable[usegamma][*palette++];
		sdl_palette->colors[n].b = (c << 8) + c;
	}
}

void I_InitGraphics()
{
	int n;
	int pnum;
	int x = 0;
	int y = 0;
	
	char xsign = ' ';
	char ysign = ' ';
	
	Uint32 windowFlags;
	
	if (window != NULL) {
		return;
	}
	
	signal(SIGINT, (void (*)(int))I_Quit); //this should be somewhere else... I think
	
    if (M_CheckParm("-2")) {
		multiply = 2;
	}
	
    if (M_CheckParm("-3")) {
		multiply = 3;
	}
	
    if (M_CheckParm("-4")) {
		multiply = 4;
	}
	
	realWidth = SCREENWIDTH * multiply;
	realHeight = SCREENHEIGHT * multiply;
	
	centerX = realWidth / 2;
	centerY = realHeight / 2;
	
	// check if the user wants to grab the mouse (quite unnice)
	grabMouse = !!M_CheckParm("-grabmouse");
	
	// check for command-line geometry
    if ( (pnum=M_CheckParm("-geom")) ) {
		printf("-geom parameter ignored\n"); //because I don't fuckin' understand how is supposed to work
    }
	
	windowFlags = SDL_WINDOW_SHOWN;
	if (grabMouse) {
		SDL_SetRelativeMouseMode(true);
	} else {
		SDL_ShowCursor(false);
	}
	
	// open window, should set a name string in d_main and then set the title
	window = SDL_CreateWindow("VROOM", x, y, realWidth, realHeight, windowFlags);
	if (window == NULL) {
		I_Error(SDL_GetError());
	}
	
	//Drawing stuff
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL) {
		I_Error(SDL_GetError());
	}
	surface = SDL_CreateRGBSurface(0, realWidth, realHeight, 8, 0, 0, 0, 0);
	if (surface == NULL) {
		I_Error(SDL_GetError());
	}
	
	if (multiply == 1) {
		screens[0] = (unsigned char*) surface->pixels;
	} else {
		screens[0] = malloc(SCREENWIDTH * SCREENHEIGHT);
	}
}
