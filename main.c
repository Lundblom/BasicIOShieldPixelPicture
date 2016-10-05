/*
	Author: Me and You <3
*/
#include <stdio.h>

int main(  void )
{
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
			outElement += in[(i*16) + j] << (j+1);
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


	return 1;
}
