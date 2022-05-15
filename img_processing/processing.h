#ifndef processing_h
#define processing_h

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <math.h>
#include <err.h>
#include "../operations/operations.h"



void greyscale(SDL_Surface* image_surface);
void binary(SDL_Surface* image_surface);
void treatment(SDL_Surface* image_surface);
void noise(SDL_Surface* image_surface);


#endif
