#include "rotation.h"

SDL_Surface* rotate(SDL_Surface* image_surface, double angle)
{
    //Create a temporary surface of the rotated image
    SDL_Surface *tmp = rotozoomSurface(image_surface, angle, 1.0, 1);
    
    //Calculate the new withd and height of the image
    double col = image_surface->w;
    double row = image_surface->h;
    if(angle == 90)
    {
        col = image_surface->h;
        row = image_surface->w;
    }
    if(angle == 1)
    {
        col = image_surface->w + image_surface->w * 0.1;
        row = image_surface->h + image_surface->h * 0.1;
    }
    if(angle == 10 || angle == -10)
    {
        col = image_surface->w + image_surface->w * 0.3;
        row = image_surface->h + image_surface->h * 0.3;
    }

    //Create an new surface of the new size filled with white pixel
    SDL_Surface *rotate = SDL_CreateRGBSurface(0, (int) col, (int) row,
            image_surface->format->BitsPerPixel, image_surface->format->Rmask,
            image_surface->format->Gmask, image_surface->format->Bmask,
            image_surface->format->Amask);   
    SDL_FillRect(rotate, NULL, SDL_MapRGB(rotate->format, 255, 255, 255));

    //Copy the temporary surface in the new surface
    SDL_BlitSurface(tmp, NULL, rotate, NULL);

    //Free the memory
    SDL_FreeSurface(tmp);
    SDL_FreeSurface(image_surface);

    //Return the new surface
    return rotate;
}