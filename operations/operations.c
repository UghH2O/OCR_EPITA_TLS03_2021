#include "operations.h"


////////////Pixels Operations////////////
static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}


//get the value of the pixel at x, y
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}


//change the value of the pixel at x, y
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}



////////////SDL Operations//////////// 

//update the display of the image om the screen
void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

// Initializing the SDL
void init_sdl()
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}


//Loading an image from a file
SDL_Surface* load_image(char *path)
{
	SDL_Surface *img;
	img = IMG_Load(path);
	if (!img)
		errx(3, "can't load %s: %s", path, IMG_GetError());

	return img;
}


//Displaying an image
SDL_Surface* display_image(SDL_Surface *img)
{
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
	if (screen == NULL)
	{
		errx(1, 
        "Couldn't set %dx%d video mode: %s\n",img->w, img->h, SDL_GetError());
	}

	if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
		warnx("BlitSurface error: %s\n", SDL_GetError());

	SDL_UpdateRect(screen, 0, 0, img->w, img->h);

	return screen;
}




//Waiting for a key to be pressed for the testing
void wait_for_keypressed()
{
	SDL_Event event;
	do
	{
		SDL_PollEvent(&event);
	} while(event.type != SDL_KEYDOWN);

	do
	{
		SDL_PollEvent(&event);
	} while(event.type != SDL_KEYUP);
}



//To free the allocated memory, just use the following SDL function.
void SDL_FreeSurface(SDL_Surface *surface);



////////////Arrays Operations//////////// 

//return the pivot of an array
int __pivot(int arr[], int low, int high) 
{ 
    int pivot = arr[high];
    int i = (low - 1);
    int tmp;
    for (int j = low; j < high; j++) 
    { 
        if (arr[j] < pivot) 
        { 
            i++;
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        } 
    } 
    tmp = arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = tmp;
    return (i + 1); 
} 
  
//Sort an array with the quicksort method
void array_sort(int arr[], int low, int high) 
{ 
    if (low < high) 
    { 
        int pivot = __pivot(arr, low, high); 
        array_sort(arr, low, pivot - 1); 
        array_sort(arr, pivot + 1, high); 
    } 
}