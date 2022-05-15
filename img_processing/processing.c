#include "processing.h"


//transform the image in greyscale image
void greyscale(SDL_Surface* image_surface)
{
	int width = image_surface->w;
	int height = image_surface->h;
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			Uint32 pixel = get_pixel(image_surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			Uint8 average = 0.3*r + 0.59*g + 0.11*b;
			pixel = SDL_MapRGB(image_surface->format,average,average,average);
			put_pixel(image_surface, x, y, pixel);
		}
	}
}


//reduct the noise in the image with a median filtre of 3x3 matrix
void noise(SDL_Surface* image_surface)
{
	int width = image_surface->w;
	int height = image_surface->h;
	int neighbors[9];

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			neighbors[4] = get_pixel(image_surface, x, y);
			//left border
			if(x == 0)
			{
				//upper border
				if(y == 0)
				{
					neighbors[0] = 255;
					neighbors[1] = 255;
					neighbors[2] = 255;
					neighbors[3] = 255;
					neighbors[5] = get_pixel(image_surface, x+1, y);
					neighbors[6] = 255;
					neighbors[7] = get_pixel(image_surface, x, y+1);
					neighbors[8] = get_pixel(image_surface, x+1, y+1);
				}
				//lower border
				else if(y == height - 1)
				{
					neighbors[0] = 255;
					neighbors[1] = get_pixel(image_surface, x, y-1);
					neighbors[2] = get_pixel(image_surface, x+1, y-1);
					neighbors[3] = 255;
					neighbors[5] = get_pixel(image_surface, x+1, y);
					neighbors[6] = 255;
					neighbors[7] = 255;
					neighbors[8] = 255;
				}
				else
				{
					neighbors[0] = 255;
					neighbors[1] = get_pixel(image_surface, x, y-1);
					neighbors[2] = get_pixel(image_surface, x+1, y-1);
					neighbors[3] = 255;
					neighbors[5] = get_pixel(image_surface, x+1, y);
					neighbors[6] = 255;
					neighbors[7] = get_pixel(image_surface, x, y+1);
					neighbors[8] = get_pixel(image_surface, x+1, y+1);
				}				
			}
			//right border
			else if(x == width - 1)
			{
				//upper border
				if(y == 0)
				{
					neighbors[0] = 255;
					neighbors[1] = 255;
					neighbors[2] = 255;
					neighbors[3] = get_pixel(image_surface, x-1, y);
					neighbors[5] = 255;
					neighbors[6] = get_pixel(image_surface, x-1, y+1);
					neighbors[7] = get_pixel(image_surface, x, y+1);
					neighbors[8] = 255;
				}
				//lower border
				else if(y == height - 1)
				{
					neighbors[0] = get_pixel(image_surface, x-1, y-1);
					neighbors[1] = get_pixel(image_surface, x, y-1);
					neighbors[2] = 255;
					neighbors[3] = get_pixel(image_surface, x-1, y);
					neighbors[5] = 255;
					neighbors[6] = 255;
					neighbors[7] = 255;
					neighbors[8] = 255;
				}
				else
				{
					neighbors[0] = get_pixel(image_surface, x-1, y-1);
					neighbors[1] = get_pixel(image_surface, x, y-1);
					neighbors[2] = 255;
					neighbors[3] = get_pixel(image_surface, x-1, y);
					neighbors[5] = 255;
					neighbors[6] = get_pixel(image_surface, x-1, y+1);
					neighbors[7] = get_pixel(image_surface, x, y+1);
					neighbors[8] = 255;
				}		
			}
			//normal case
			else if(y != 0 && y != height - 1 && x != 0 && x != width - 1)
			{
				neighbors[0] = get_pixel(image_surface, x-1, y-1);
				neighbors[1] = get_pixel(image_surface, x, y-1);
				neighbors[2] = get_pixel(image_surface, x+1, y-1);
				neighbors[3] = get_pixel(image_surface, x-1, y);
				neighbors[5] = get_pixel(image_surface, x+1, y);
				neighbors[6] = get_pixel(image_surface, x-1, y+1);
				neighbors[7] = get_pixel(image_surface, x, y+1);
				neighbors[8] = get_pixel(image_surface, x+1, y+1);
			}

			array_sort(neighbors, 1, 9);
			put_pixel(image_surface, x, y, neighbors[4]);
			
		}
	}
}



//return an array of probability of greyscale
double* histogram(SDL_Surface* image_surface)
{
	int width = image_surface->w;
	int height = image_surface->h;
	double *histo = malloc(256 * sizeof(double));
	for(int i = 0; i < 256; i++)
		histo[i] = 0;
	//number of pixels
	double total = 0;

	//for each pixel increment the histogram
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			Uint32 pixel = get_pixel(image_surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			histo[r]++;
			total++;
		}
	}

	//calculate the probability of each scale
	double *proba = malloc(256 * sizeof(double));
	for(int i = 0; i < 256; i++)
		proba[i] = histo[i] / total;
	return proba;
}


//apply the Otsu's method and return threshold of grey
int otsu(double* proba)
{
	int threshold = 0;
	double *omega = malloc(256 * sizeof(double));
	double *average = malloc(256 * sizeof(double));

	//probability of the class 1
	omega[0] = proba[0];
	//average of the class 1
	average[0] = 0.0;
	for(int j = 1; j < 256; j++)
	{	
		omega[j] = omega[j - 1] + proba[j];
		average[j] = average[j - 1] + j * proba[j];
	}

	//find the higher variance (maximum)
	double maximum = 0.0;
	for(int i = 0; i < 256; i++)
	{
		//extract extreme cases
		if(omega[i] != 0.0 && omega[i] < 1.0)
		{
			double tmp = (pow(average[255] * omega[i] - average[i], 2))
			/ (omega[i] * (1 - omega[i]));
			if(tmp >= maximum)
			{
				maximum = tmp;
				threshold = i;
			}
		}		
	}

	//return the index of the higher variance
	return threshold;
}



//transform the image to a binary image in black and white
void binary(SDL_Surface* image_surface)
{
	int width = image_surface->w;
	int height = image_surface->h;
	int threshold = otsu(histogram(image_surface));
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			Uint32 pixel = get_pixel(image_surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			//transform the pixel in black
			if(r < threshold)
			{
				pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
				put_pixel(image_surface, x, y, pixel);
			}
			//transform the pixel in white
			else
			{
				pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
				put_pixel(image_surface, x, y, pixel);
			}		
		}
	}
}




//make all the transformations to the image
void treatment(SDL_Surface* image_surface)
{

	greyscale(image_surface);

	noise(image_surface);

	binary(image_surface);
}

