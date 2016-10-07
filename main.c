/*
	Author: Me and You <3
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "imagetopixels.h" 
#include <string.h>

int main( int argc, char** argv  )
{
	if(argc == 1)
	{
		printf("Append --help for correct usage.\n");
		exit(0);
	}

	if(strcmp(argv[1],"--help") == 0)
	{
		printf("This program converts PNG images to a format that can be read by the Basic IO Shield\n");
		printf("-w sets what to scale the image width to (in pixels)\n");
		printf("-h sets what to scale the image height to (in pixels)\n");
	}
	
	char* filename = argv[1];
	int target_width = -1;
	int target_height = -1;

	
	int argument_location = 2;
	//Handler for arguments
	while(argument_location < argc)
	{

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

	
	unsigned w;
	unsigned h;
	unsigned char* image = parse_image(filename, &w, &h);
	char* arr = image_to_bitarray(image, w, h);

	char* resizedArr = NULL;
	if(target_width > -1 || target_height > -1)
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

	int i = 0;
	int j = 0;

	unsigned char* richards_array = bitarray_to_pixelimage(resizedArr, target_width*target_height);

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

	if(richards_array != NULL)
	{
		for(i = 0; i < 16 ; i++)
		{
			for(j = 0; j < 8; j++)
			{
				printf("%u,", (unsigned int)richards_array[i*(8) + j] );
			}
			printf("\n");
		}
	}
	/*
	else
	{
		for(i = 0; i < h ; i++)
                {
                        for(j = 0; j < w; j++)
                        {
                                printf("%d,", arr[i*(w) + j] );
                        }
                        printf("\n");
                }
	}*/
	free(image);
	free(resizedArr);
	return 1;
}