#include <stdlib.h>
#include <stdio.h>
#include "PropertyParser.h"
#include "raycaster.h"
#include "3dMathLib/3dmath.h"

int render(int height, int width, OBECT_LIST_STR *list, char* output)
{
	double focal_length = -1;

	//m and n are m x n pixels of view plane 
	int m, n, i5, j5;
	m = 1024;
	n = 1024;

	//ratios for pixel by width or height
	double height_r, width_r;
	double sub_h, sub_w;
	
	//create the origin R0 and c, center of viewplane
	V3 r0 = v3_assign(0,0,0);
	V3 c = v3_assign(0,0,focal_length);
	//Pij
	V3 Pij, sub_v, Rd;
	Pij = malloc(sizeof(double) * 3);
	Rd = malloc(sizeof(double) * 3);

	// buffers for vector mathematics
	double buff_height, buff_width;

	//allocate area for pixmap to be written onto
	Pixel* pixmap = malloc(sizeof(Pixel) * width * height);

	//Here we have the i and j from P ij
	for (int i = 0; i < width; i += 1)
	{
		for (int j = 0; j < height; j += 0)
		{
			//now we need to construct Pij in some way
			//Pij = r0 - [w = width , h = height] + [w/n(j + 0.5), h/m(i +0.5)]
			//first get the pixel by length ratios
			height_r = height/m;
			width_r = width/n;

			i5 = i + 0.5;
			j5 = j + 0.5;

			buff_width = width_r * i5;
			buff_height = height_r * j5;

			sub_w = width + buff_width;
			sub_h = height + buff_height;

			//assign the vector to be subtracted from R0 for Pij
			sub_v = v3_assign(sub_w, sub_h, focal_length);

			//perform the subtraction on these two vectors for Pij
			v3_subtract(c, sub_v, Pij);

			//now construct Rd = Pij - R0
			v3_subtract(Pij, R0, Rd);

			//shoot the ray at that direction and save as color vector
			V3 color = raycast(Rd, width, height, list);
			
			//since color will be 0-1 ray, we will convert to 0-255
			pixmap[width * j + i].r = (int) color[0] * 255;
			pixmap[width * j + i].g = (int) color[1] * 255;
			pixmap[width * j + i].b = (int) color[2] * 255;

			//write this all to an output file


			return 0;
		}
	}

}

V3 raycast(V3 Rd, int width, int height, OBECT_LIST_STR *list);
{
	//keep track of the closest object

	//handle each object based on the type it is
	for(int i = 0; i < list[0].numObjects; i += 1)
	{
		//check for sphere object
		if(strcmp(list[0].listOfObjects[i].objectName, "sphere") != 0)
		{

		}
		//check for plane
		else if(strcmp(list[0].listOfObjects[i].objectName, "plane") != 0)
		{

		}
		//check for a camera object
		else if(strcmp(list[0].listOfObjects[i].objectName, "camera") != 0)
		{

		}
	}
}


V3* v3_assign(double a, double b, double c)
{
	V3 vector = malloc(sizeof(double) * 3);

	vector[0] = a;
	vector[1] = b;
	vector[2] = c;

	return vector;
}