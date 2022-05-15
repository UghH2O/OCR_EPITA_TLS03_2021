#include "segmentation.h"


//detect lines of text, replace white lines per red lines
void linesegm(SDL_Surface* image_surface)
{
	int width = image_surface->w;
	int height = image_surface->h;
	
	//for each line
	for(int y = 0; y < height; y++)
	{

		int x = 0;
		//if the line is white, 0 for True and 1 for False
		int iswhite = 0;
		while(iswhite == 0)
		{
			//it is the end of a line, so color it in red
			if(x == width)
			{
				iswhite++;
				Uint32 red = SDL_MapRGB(image_surface->format, 255, 0, 0);
				for(int tmp = 0; tmp < width; tmp ++)
					put_pixel(image_surface, tmp, y, red);	
			}
			else
			{
				Uint32 pixel = get_pixel(image_surface, x, y);
				Uint8 r, g, b;
				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
				//if there is a black pixel in the line
				if(r == 0)
					iswhite++;
				x++;
			}
		}
	}
}


//detect characters in each lines
//replace white spaces between characters per green spaces
void charsegm(SDL_Surface* image_surface)
{
	int width = image_surface->w;
	int height = image_surface->h;
	//the beginning of a line
	int beginline = 0;
	//the end of a line
	int endline = 0;
	//for each line
	int y = 0;
	while(y < height)
	{
		Uint32 pixel = get_pixel(image_surface, 0, y);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		//if the first pixel is red, go to next line
		if(g != r)
			y++;
		else
		{

			beginline = y;
			//find the bottom pixel line of current char line
			while(y < height && g == r )
			{
				y++;
				pixel = get_pixel(image_surface, 0, y);
				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			}
			endline = y;

			//same as the linesegm, for each column in this line
			for(int x = 0; x < width; x++)
			{
				int z = beginline;
				//if the column is white, 0 for True and 1 for False
				int iswhite = 0;
				while(iswhite == 0)
				{
					//it is the end of the column, so color it in green
					if(z == endline)
					{
						iswhite++;
						Uint32 green=SDL_MapRGB(image_surface->format,0,255,0);
						for(int tmp = beginline; tmp < endline; tmp++)
							put_pixel(image_surface, x, tmp, green);
					}
					else
					{
						pixel = get_pixel(image_surface, x, z);
						SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
						//if there is a black pixel in the column
						if(r == 0)
							iswhite++;
						z++;
					}
				}
			}
		}
	}
}


//Reframe the image around the letter for the neural network
SDL_Surface* frame(SDL_Surface* image_surface)
{
	linesegm(image_surface);
	charsegm(image_surface);
	int y = 0;
	Uint32 pixel = get_pixel(image_surface, 0, y);
	Uint8 r, g, b;
	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	//Find the begining of the char
	while (b != r)
	{
		y++;
		pixel = get_pixel(image_surface, 0, y);
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	}
	int x = 0;
	while (g != b)
	{
		x++;
		pixel = get_pixel(image_surface, x, y);
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	}

	//Find the end of the char
	int w = image_surface->w - 1;
	int h = image_surface->h - 1;
	pixel = get_pixel(image_surface, 0, h);
	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	while (b != r)
	{
		h--;
		pixel = get_pixel(image_surface, 0, h);
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	}
	while (g != b)
	{
		w--;
		pixel = get_pixel(image_surface, w, h);
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	}
	w += 1;
	h += 1;
	
	//Create the new surface 
	SDL_Surface *surface = SDL_CreateRGBSurface(0, w - x, h - y, 
			image_surface->format->BitsPerPixel, image_surface->format->Rmask,
			image_surface->format->Gmask, image_surface->format->Bmask, 
			image_surface->format->Amask);
			
	//Fill the surface with the char
	for(int i = x; i < w; i++)
	{
		for(int j = y; j < h; j++)
		{
			pixel = get_pixel(image_surface, i, j);
			put_pixel(surface, i - x, j - y, pixel);
		}
	}
	return surface;
}


//Creat a new SDL_Surface with only one character
SDL_Surface* extractchar(SDL_Surface* image_surface, int beginw, int beginh)
{
	int width = 0;
	int height = 0;

	Uint32 pixel = get_pixel(image_surface, beginw + width, beginh);
	Uint8 r, g, b;
	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	//Find the width of the char
	while(width < image_surface->w && g == b && b == r)
	{
		width++;
		pixel = get_pixel(image_surface, beginw + width, beginh);
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	}

	pixel = get_pixel(image_surface, beginw, beginh + height);
	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	//Find the height of the char
	while(height < image_surface->h && g == b && b == r)
	{
		height++;
		pixel = get_pixel(image_surface, beginw, beginh + height);
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	}

	//Create the new surface 
	SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 
			image_surface->format->BitsPerPixel, image_surface->format->Rmask,
			image_surface->format->Gmask, image_surface->format->Bmask, 
			image_surface->format->Amask);

	//Fill the surface with the char
	for(int x = beginw; x < beginw + width; x++)
	{
		for(int y = beginh; y < beginh + height; y++)
		{
			pixel = get_pixel(image_surface, x, y);
			put_pixel(surface, x - beginw, y - beginh, pixel);
		}
	}
	return surface;
}


//Resize the surface to less than 30 * 30
SDL_Surface* resizechar(SDL_Surface* image_surface)
{
	SDL_Surface *surface = SDL_malloc(88);
	double width = image_surface->w;
	double height = image_surface->h;
	double zoom = 0;
	if(width > height)
		zoom = 30 / width;
	else
		zoom = 30 / height;
	surface = rotozoomSurface(image_surface, 0, zoom, 1);
	return surface;
}


//Print the matrix of on char on the terminal
void printmatrix(double* matrix)
{
	for (int x = 0; x < 30; x++)
	{
    	for(int y = 0; y < 30; y++)
        	printf("%.0f", matrix[y *30 + x]);
    	printf("\n");
	}
}


//Write the matrix in a txt file to creat the training set
void writematrix(double matrix[], char filename[])
{
	char c[5] = {filename[13], '.', 't', 'x', 't'};
	FILE *fichier = fopen(c,"w" );
    if (fichier == NULL)
		printf("Error when file is loaded" );
    for (int x = 0; x < 30; x++)
    { 
        for (int y = 0; y < 30; y++)
        {
			fprintf(fichier, "%.0f", matrix[x * 30 + y]);
			fprintf(fichier, ",");
        } 
    }
}



//Put a surface into a matrix
void putmatrix(SDL_Surface* image_surface, double* matrix)
{
	int width = image_surface->w;
	int height = image_surface->h;
	SDL_LockSurface(image_surface);
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			Uint32 pixel = get_pixel(image_surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			if(r == 0)
				matrix[x * 30 + y] = 1;
			else if(r == 255)
				matrix[x * 30 + y] = 0;
		}
	}
}


//Calculate the average size of a space between 2 characters
void findmediumspace(SDL_Surface* image_surface, int medium[])
{
	int width = image_surface->w;
	int height = image_surface->h;

	//if first char is found, 0 for True and 1 for False
	int firstchar = 1;
	//if first lign is found, 0 for True and 1 for False
	int firstlign = 1;
	int charspace = 0;
	int lignspace = 0;

	int redpixel = 0;
	int greenpixel = 0;
	int nbred = 0;
	int nbgreen = 0;

	int y = 0;
	while(y < height)
	{
		Uint32 pixel = get_pixel(image_surface, 0, y);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		//if the first pixel is red, go to next line
		if(b != r)
		{
			y++;
			//avoid counting margins
			if(firstlign == 0)
				lignspace++;
		}
		//else it is a character line
		else
		{
			if (firstlign == 0)
			{
				redpixel += lignspace;
				nbred += 1;
			}

			int x = 0;
			while(x < width)
			{
				
				pixel = get_pixel(image_surface, x, y);
				Uint8 r, g, b;
				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
				//if the pixel is green, go to next column
				if(g != b)
				{
					x++;
					//avoid counting margins
					if(firstchar == 0)
						charspace++;

				}
				//else there is a character
				else
				{
					if (firstchar == 0)
					{
						greenpixel += charspace;
						nbgreen += 1;
					}
					
					charspace = 0;
					firstchar = 0;
					while(x < width && g == b && b == r)
					{
						x++;
						pixel = get_pixel(image_surface, x, y);
						SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
					}
				}
			}

			while(y < height && b == r)
			{
				y++;
				pixel = get_pixel(image_surface, 0, y);
				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			}

			charspace = 0;
			lignspace = 0;
			firstchar = 1;
			firstlign = 0;
		}
	}

	//if there is only one or two ligns
	if (nbred <= 1)
		medium[0] = redpixel + 10;
	else
	{
		//if there only three lignes, can't make a medium value
		if (nbred == 2)
			medium[0] = redpixel/nbred + 10;
		else
			medium[0] = redpixel/nbred + redpixel/(nbred * 4);
	}
	//if there is only one or two char
	if (nbgreen<= 1)
		medium[1] = greenpixel + 10;
	else
		medium[1] = greenpixel/nbgreen + greenpixel/(nbgreen * 4) + 1;
}



//Search all characters and detect spaces and new paragraphs
//Each time: put it in a matrix, call neural network, write it in a file
void segmentation(SDL_Surface* image_surface)
{
	FILE *file = fopen("text.txt", "w");

	int width = image_surface->w;
	int height = image_surface->h;
	
	//if first char is found, 0 for True and 1 for False
	int firstchar = 1;
	//if first lign is found, 0 for True and 1 for False
	int firstlign = 1;

	int medium[2];
	findmediumspace(image_surface, medium);
	//the medium size of a space between two char to find word spaces
	int mediumcharspace = medium[1];
	//the medium size of a space between two ligns to find new paragraphs
	int mediumlignspace = medium[0];
	int charspace = 0;
	int lignspace = 0;

	int y = 0;

	//init network before looping so it's loaded only once
	network net = 
    {
        .input = {},
        .hidden = {},
        .output = {},

        .weights01 = {},
        .weights12 = {},
        .bias01 = {},
        .bias12 = {}
    };

	while(y < height)
	{
		Uint32 pixel = get_pixel(image_surface, 0, y);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		//if the first pixel is red, go to next line
		if(b != r)
		{
			y++;
			//avoid counting margins
			if(firstlign == 0)
				lignspace++;
		}
		//else it is a character line
		else
		{
			//if there is a new paragraph
			if(lignspace > mediumlignspace)
				fprintf(file, "\n");

			int x = 0;
			while(x < width)
			{
				pixel = get_pixel(image_surface, x, y);
				Uint8 r, g, b;
				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
				//if the pixel is green, go to next column
				if(g != b)
				{
					x++;
					//avoid counting margins
					if(firstchar == 0)
						charspace++;
				}
				//else there is a character
				else
				{
					//if it is a new word
					if(charspace > mediumcharspace)
						fprintf(file, " ");
					
					SDL_Surface* character = SDL_malloc(88);
					character = extractchar(image_surface, x, y);

					SDL_Surface* charframe = SDL_malloc(88);
					charframe = frame(character);

					SDL_Surface* charresized = SDL_malloc(88);
					charresized = resizechar(charframe);


					double matchar[30 * 30] = {0};
					putmatrix(charresized, matchar);
					//printmatrix(matchar);
					//printf("\n");
					//writematrix(matchar, image);

					SDL_free(character);
					SDL_free(charresized);

					//Call the neural network
					char c = getChar(matchar, &net);
					//char c = 'a';
					fprintf(file, "%c", c);
					
					charspace = 0;
					firstchar = 0;

					//Move to the next green pixel
					while(x < width && g == b)
					{
						x++;
						pixel = get_pixel(image_surface, x, y);
						SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
					}
				}
			}
			fprintf(file, " ");

			//Move to the next red pixel
			while(y < height && b == r)
			{
				y++;
				pixel = get_pixel(image_surface, 0, y);
				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			}
			charspace = 0;
			lignspace = 0;
			firstchar = 1;
			firstlign = 0;
		}
	}
	fclose(file);
}