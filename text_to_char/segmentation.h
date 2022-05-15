#ifndef segmentation_h
#define segmentation_h

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "../operations/operations.h"
#include "../neural_network/OCR/execute.h"
#include "../neural_network/OCR/tools.h"

void linesegm(SDL_Surface* image_surface);
void charsegm(SDL_Surface* image_surface);
void segmentation(SDL_Surface* image_surface);


#endif
