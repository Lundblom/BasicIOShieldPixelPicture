/*
	Author: Me and You <3
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "imagetopixels.h"
#include <string.h>
#include <unistd.h>

#define PROGRAM_NAME "pixelstoicon"

int main( int argc, char** argv  )
{
	if(argc == 1 || (strcmp(argv[1],"--help") == 0) || (strcmp(argv[1],"-h") == 0))
	{
		printf("-- %s --\n", PROGRAM_NAME);
		printf("This program converts PNG images to a format that can be read by the Basic IO Shield\n");
		printf("Usage: %s [image_name] {flags}\n", argv[0]);
		printf("-w sets what to scale the image width to (in pixels)\n");
		printf("-h sets what to scale the image height to (in pixels)\n");
		printf("-i enables bit inversion\n");
		printf("-d displays the target bit array as an image\n");
		return 0;
	}

	char* filename = argv[1];
	int target_width = -1;
	int target_height = -1;

	int bit_invert = 0;
	int display_image = 0;


	int argument_location = 2;
	//Handler for arguments
	while(argument_location < argc)
	{

		if(strcmp(argv[argument_location],"-w") == 0)
		{
			argument_location++;
			if(argument_location >= argc)
			{
				printf("%s, No argument for -w supplied\n", PROGRAM_NAME);
				exit(0);
			}

			char* end;
			long result = strtol(argv[argument_location], &end, 10);
			if(end == argv[argument_location] || result == LONG_MAX || result == LONG_MIN)
			{
				printf("%s: Incorrect integer '%s' argument to -w\n", PROGRAM_NAME, argv[argument_location]);
				exit(0);
			}
			target_width = (int)result;
		}
		else if(strcmp(argv[argument_location],"-h") == 0)
    {
            argument_location++;
            if(argument_location >= argc)
            {
                    printf("%s: No argument for -h supplied\n", PROGRAM_NAME);
                    exit(0);
            }

            char* end;
            long result = strtol(argv[argument_location], &end, 10);
            if(end == argv[argument_location] || result == LONG_MAX || result == LONG_MIN)
            {
                    printf("%s: Incorrect integer argument '%s' to -h\n", PROGRAM_NAME, argv[argument_location]);
                    exit(0);
            }
            target_height = (int)result;
    }
    else if(strcmp(argv[argument_location],"-i") == 0)
    {
      argument_location++;
      bit_invert = 1;
    }
    else if(strcmp(argv[argument_location],"-d") == 0)
    {
      argument_location++;
      display_image = 1;
    }
		else
		{
			argument_location++;

		}
	}

	if( !(access( filename, F_OK ) != -1))
	{
		printf("%s: The file specified does not exist.\n", PROGRAM_NAME);
		exit(0);
	}


	unsigned w;
	unsigned h;
	unsigned char* image = parse_image(filename, &w, &h);
	char* arr = image_to_bitarray(image, w, h);

	char* resized_arr = NULL;
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
		resized_arr = resize_bitarray(arr, w, h, target_width, target_height);
	}

	int i = 0;
	int j = 0;

	unsigned char* pixel_array;
	if(resized_arr == NULL)
	{
	  pixel_array = bitarray_to_pixelimage(arr, w*h, bit_invert);
	}
	else
	{
	  pixel_array = bitarray_to_pixelimage_any(resized_arr, target_height, target_width, bit_invert);
	}

  if(display_image)
  {
	  if(resized_arr != NULL)
	  {
		  for(i = 0; i < target_height ; i++)
		  {
			  for(j = 0; j < target_width; j++)
			  {
				  printf("%d,", resized_arr[i*(target_width) + j] );
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
                      printf("%d,", arr[i*(w) + j] );
              }
              printf("\n");
      	  }
	  }
	}

	if(pixel_array != NULL)
	{
		for(i = 0; i < target_height / 8 ; i++)
		{
			for(j = 0; j < target_width ; j++)
			{
				printf("%u,", (unsigned int)pixel_array[i*(8) + j] );
			}
			printf("\n");
		}
	}

	free(image);
	free(resized_arr);
	return 1;
}
