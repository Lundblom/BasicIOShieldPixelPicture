/*
	Author: Me and You <3
*/
#include <stdio.h>
#include "lodepng.h"
#include <stdlib.h>
#include <math.h>
#include <limits.h>

unsigned char* parse_image(const char* filename, unsigned *width, unsigned *height);
char* image_to_bitarray(const unsigned char *image, const unsigned w, const unsigned h);
char* resize_bitarray(char *bits, const int sourceW, const int sourceH, const int targetW, const int targetH);

int main( int argc, char** argv  )
{
	if(argc == 1)
	{
		printf("Append --help for correct usage.\n");
		exit(0);
	}
	
	char* filename = argv[1];
	int target_width = -1;
	int target_height = -1;
	
	int argument_location = 2;
	//Handler for arguments
	printf("Argc is: %d\n", argc);
	while(argument_location < argc)
	{
		printf("Comparing %s\n", argv[argument_location]);
		if(strcmp(argv[argument_location],"-w") == 0)
		{
			argument_location++;
			if(argument_location >= argc)
			{
				printf("No argument for -w supplied\n");
				exit(0);
			}
			
			char* end;
			long result = strtol(argv[argument_location], &end, 10);
			if(end == argv[argument_location] || result == LONG_MAX || result == LONG_MIN)
			{
				printf("Incorrect integer argument to -w\n");
				exit(0);
			}	
			target_width = (int)result;	
		}
		else if(strcmp(argv[argument_location],"-h") == 0)
                {       
                        argument_location++; 
                        if(argument_location >= argc)
                        {       
                                printf("No argument for -h supplied\n");
                                exit(0);
                        }
                        
                        char* end;
                        long result = strtol(argv[argument_location], &end, 10);
                        if(end == argv[argument_location] || result == LONG_MAX || result == LONG_MIN)
                        {       
                                printf("Incorrect integer argument to -w\n");
                                exit(0);
                        }       
                        target_height = (int)result;
                }
		else
		{
			argument_location++;
		}
	}

	//all pixels
	int in[1024];

	//8 columns in 16 squares
	//each column is represented by a (?)
	int out[128];

	int i = 0;
	int j = 0;

	//Example
	//Set in as a striped "picture"
	//Should put out a list of "170"
	for(i = 0; i < 1024; i++)
	{
		if(i%2 == 0)
		{
			in[i] = 1;
		}
		else 
		{
			in[i] = 0;
		}
	}

	//out elements used in inner loop.
	int outElement = 0;

	//Set output array
	for(i = 0; i < 128; i++)
	{
		for(j = 0; j < 8; j++)
		{
			outElement += in[(i*8) + j] << (j+1);
		}

		out[i] = outElement;

		outElement = 0;
	}

	//Print output array
	for(i = 0; i < 128; i++)
	{
		if((i+1)%8 == 0)
		{
			printf("%d, \n", out[i]);
		}
		else
		{
			printf("%d, ", out[i]);
		}
	}
	unsigned w;
	unsigned h;
	unsigned char* image = parse_image(filename, &w, &h);
	char* arr = image_to_bitarray(image, w, h);

	char* resizedArr = NULL;
	if(target_width != -1 || target_height != -1)
	{
		if(target_width == -1)
		{
			target_width = w;
		}
		if(target_height == -1)
		{
			target_height = h;
		}
		resizedArr = resize_bitarray(arr, w, h, target_width, target_height);
	}

	if(resizedArr != NULL)
	{
		for(i = 0; i < target_height ; i++)
		{
			for(j = 0; j < target_width; j++)
			{
				printf("%d,", resizedArr[i*(target_width) + j] );
			}
			printf("\n");
		}
	}
	else
	{
		for(i = 0; i < h ; i++)
                {
                        for(j = 0; j < w; j++)
                        {
                                printf("%d,", image[i*(w) + j] );
                        }
                        printf("\n");
                }
	}
	free(image);
	free(resizedArr);
	return 1;
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
                printf("\n");
        }
	
	return result;		
}
