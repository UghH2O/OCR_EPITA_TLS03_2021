#ifndef rotation_h
#define rotation_h

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <math.h>
#include <err.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "../operations/operations.h"

SDL_Surface* rotate(SDL_Surface* image_surface, double angle);

#endif