/*
	Author: Me and You <3
*/
#include <stdio.h>
#include "lodepng.h"
#include <stdlib.h>
#include <math.h>
#include <limits.h>

unsigned char* bitarray_to_pixelimage(char* in, int size, int invert)
{
	//all pixels
	if(size != 1024)
	{
		return NULL;
	}

	//8 columns in 16 squares
	//each column is represented by a (?)
	unsigned char* out = malloc(sizeof(char) * 128);

	int i = 0;
	int j = 0;
	int k = 0;

	//out elements used in inner loop.
	int outElement = 0;
	
	int whichRow = 0;
	int counter = 0;

	//Set output array
	for(i = 0; i < 4; i++)
	{
	  for(j = 0; j < 32; j++)
	  {
		  for(k = 0; k < 8; k++)
		  {
			  outElement += in[(k*32) + (i*256) + j] << (k);
		  }
      if(invert)
      {
		    out[(i*32) + (j)] = outElement;
		  }
		  else
      {
		    out[(i*32) + (j)] = ~outElement;
		  }

		  outElement = 0;
		 }
	}

	return out;
}
               
char* resize_bitarray(char *bits, const int sourceW, const int sourceH, const int targetW, const int targetH)
{
	if(sourceW == targetW && sourceH == targetH)
	{
		return bits;
	}
	if(targetW == 0 || targetH == 0)
	{
		return NULL;
	}

	int i = 0;
	int j = 0;

	char* result = malloc(sizeof(char) * targetW * targetH);

	double ratioH = sourceH / (double)targetH;
	double ratioW = sourceW / (double)targetW;  

	for(i = 0; i < targetH; i++)
	{
		for(j = 0; j < targetW; j++)
		{
			result[i*targetW + j] = bits[ (int)(floor(i * ratioH) * sourceW + floor(ratioW * j)) ] ;
		}
	}
	
	return result;	
}

unsigned char* parse_image(const char* filename, unsigned *width, unsigned *height)
{
	unsigned char *image;
	unsigned error;
	
	unsigned tWidth;
	unsigned tHeight;
	error = lodepng_decode32_file(&image, &tWidth, &tHeight, filename);
        if(error)
	{ 
		printf("error %u: %s\n", error, lodepng_error_text(error));
		exit(0);	
	}

	*width = tWidth;
	*height = tHeight;

	return image;
}

char* image_to_bitarray(const unsigned char *image, const unsigned w, const unsigned h)
{
	char *result = malloc(sizeof(char) * w * h);
	int i;
	int j;
	for(i = 0; i < h; i++)
        {
                for(j = 0; j < w; j++)
                {
			//If Alpha is 0 or all colors are 0, we output 0
			if((image[4*i*w + 4*j + 3]) == 0 || ( (image[4*i*w + 4*j + 0]) == 0 && (image[4*i*w + 4*j + 1]) == 0  &&
				(image[4*i*w + 4*j + 2]) == 0 ))
			{
				result[i*w + j] = 0;
			}
			else
			{
				result[i*w + j] = 1;
			}	             
                }
        }
	
	return result;		
}
