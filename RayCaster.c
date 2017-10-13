#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "PropertyParser.h"
#include "RayCaster.h"
#include "3dMathLib/3dmath.h"

double sphere_intersect(OBJECT_STR object, V3 Rd, V3 R0)
{
	double intersect;
	V3 t = malloc(sizeof(double ) * 3);
	double x,y,z;

	//define variables from Spheres data
	V3 s_pos = v3_assign(object.properties[1].data[0], object.properties[1].data[1], object.properties[1].data[2]);

	double r = object.properties[2].data[0];
	double r_sqr = r * r;

	//a = x2 + y2 + z2
	double a = (Rd[0] * Rd[0]) + (Rd[1] * Rd[1]) + (Rd[2] * Rd[2]);

	double b = ( ((2*Rd[0]) * (R0[0] - s_pos[0])) + ((2*Rd[1])*(R0[1] - s_pos[1])) +
			((2*Rd[2]) *(R0[2] - s_pos[2])) );

	double c = ((s_pos[0] * s_pos[0]) + (s_pos[1] * s_pos[1]) + (s_pos[2] * s_pos[2]))
			+ ((Rd[0] * Rd[0]) + (Rd[1] * Rd[1]) + (Rd[2]*Rd[2])) + 
				((-2) *(s_pos[0]*Rd[0]) + (s_pos[1]*Rd[1]) + (s_pos[2]*Rd[2]))
				- r_sqr;

	//find discriminant
	double discriminant = (b*b) - (4*a*c);
	//printf("%lf discriminant \n", discriminant);
	//return null if less than 0 (no intersection)
	if(discriminant < 0)
	{
		//printf("No hit on sphere \n");
		//set to background
		//heee
		intersect = INFINITY;
		return intersect;
	}
	else
	{
		//find the intersection neartest R0
		intersect = ((-b) - sqrt((b*b)-(4*a*c))/(2*a));
		x = (R0[0] + (intersect*Rd[0]));
		y = (R0[1] + (intersect*Rd[1]));
		z = (R0[2] + (intersect*Rd[2]));
		//printf("Hit on sphere! \n");
		//create the point to be returned
		

		return intersect;
	}

}

double plane_intersect(OBJECT_STR object, V3 Rd, V3 R0)
{
	//begin plane intersection test!
	//store variables such as the normal
	V3 normal = v3_assign(object.properties[2].data[0],object.properties[2].data[1],object.properties[2].data[2]);
	V3 p_pos  = v3_assign(object.properties[1].data[0],object.properties[1].data[1],object.properties[1].data[2]);
	V3 sub_v = malloc(sizeof(double) * 3);

	v3_subtract(sub_v, R0, p_pos);
	double top = v3_dot(sub_v, normal);
	//printf("top: %lf, \n", top);

	double bottom = v3_dot(Rd, normal);
	//printf("bottom: %lf \n", bottom);
	if(bottom == 0)
	{
		return INFINITY;
	}

	double t = top/bottom;
	if(t > 0 )
	{
		return t;
	}
	return INFINITY;
}

void camerawork()
{

}

void writeToP3(Pixel* pixmap, int pixwidth, int pixheight, char* output)
{
	int value1, value2, value3;

	FILE* fs = fopen(output, "w");

	char buffer[pixwidth];
	//place the beginning of the magic number
	fputc('P', fs);

	fputc('3', fs);

	fputs(" \n", fs);

	fputs("#This has been a raycasting event brought to you by Thomas Back\n", fs);

	//include the width and height
	sprintf(buffer, "%d", pixwidth);

	fputs(buffer, fs);

	fputc(' ', fs);

	//sprintf(buffer, "%d", pixheight);

	fputs(buffer, fs);

	fputs(" \n", fs);

	sprintf(buffer, "%d", 255);

	fputs(buffer, fs);

	fputs("\n", fs);

	//header information added
	//header data complete; begin entering in the image data
	for(int row = 0; row < pixheight; row += 1)
	{
		for(int col = 0; col < pixwidth; col += 1)
		{	
			//unpack bytes into value var
			value1 = pixmap[pixwidth*row + col].r;
			value2 = pixmap[pixwidth*row + col].g;
			value3 = pixmap[pixwidth*row + col].b;

			//now convert value to ascii and write to file
			sprintf(buffer, "%d", value1);

			fputs(buffer, fs);
			fputc('\n', fs);

			sprintf(buffer, "%d", value2);

			fputs(buffer, fs);
			fputc('\n', fs);

			sprintf(buffer, "%d", value3);

			fputs(buffer, fs);
			fputc('\n', fs);

		}

	}

	//finally close the file from writing
	fclose(fs);

}

V3 raycast(V3 Rd, V3 R0, OBJECT_LIST_STR *list)
{
	//keep track of the closest object
	//t will be pointer to hold object and color
	double last_t;
	double t ;
	double closest_t;
	V3 to_return;
	V3 closest_color = v3_assign(1,0.9,0);;

	//handle each object based on the type it is
	for(int i = 0; i < list[0].numObjects; i += 1)
	{
		//check for sphere object
		if(strcmp(list[0].listOfObjects[i].objectName, "sphere") == 0)
		{
			//perform intersection test
			//printf("Sphere! \n");
			t = sphere_intersect(list[0].listOfObjects[i], Rd, R0);
		}	
		//check for plane
		else if(strcmp(list[0].listOfObjects[i].objectName, "plane") == 0) 
		{
			//perform intersection test
			//printf("plane! \n");
			t = plane_intersect(list[0].listOfObjects[i], Rd, R0);
			// if(t != NULL)
			// {
			// 	printf("%lf z pos of plane is \n", t[2]);
			// }
		}
		//check for a camera object
		else if(strcmp(list[0].listOfObjects[i].objectName, "camera") == 0)
		{
			//actually have no clue what to do with the camera lol
			//printf("Camera! \n");
			camerawork();
			continue;
		}
	
		if(i == 0)
		{
			closest_color = v3_assign(list[0].listOfObjects[i].properties[0].data[0],list[0].listOfObjects[i].properties[0].data[1],list[0].listOfObjects[i].properties[0].data[2]);
			last_t = t;
		}
		else if(t < last_t)
		{
			closest_color = v3_assign(list[0].listOfObjects[i].properties[0].data[0],list[0].listOfObjects[i].properties[0].data[1],list[0].listOfObjects[i].properties[0].data[2]);
			last_t = t;
		}
		else
		{
			last_t = t;
		}
		
	}

	if(last_t == INFINITY)
	{
		//return background color if no hits
		
		return closest_color;
	}

	return closest_color;
}

int render(int n, int m, OBJECT_LIST_STR *list, char* output)
{
	double focal_length = -1;
	double width, height;
	double Px, Py;

	//get width and height from camera
	for(int k = 0; k < list[0].numObjects; k += 1)
	{
		if(strcmp(list[0].listOfObjects[k].objectName, "camera") == 0)
		{
			printf("We have a camera at %d \n", k);
			width = list[0].listOfObjects[k].properties[0].data[0];
			height = list[0].listOfObjects[k].properties[1].data[0];
		}
	}
	
	//create the origin R0 
	V3 R0 = v3_assign(0,0,0);

	//Pij
	V3 Pij, Rd;
	Pij = malloc(sizeof(double) * 3);
	Rd = malloc(sizeof(double) * 3);

	// buffers for vector mathematics
	double buff_height, buff_width;
	//ratios for p x mm
	double pixheight = height/m;
	double pixwidth = width/n;
	//allocate area for pixmap to be written onto
	Pixel* pixmap = malloc(sizeof(Pixel) * m * n);

	printf("Currently Casting");
	//Here we have the i and j from P ij
	for (int i = 0; i < m; i += 1)
	{
		//assign Py
		Py = 0 - (height/2) + (pixheight * (i + 0.5));

		for (int j = 0; j < n; j += 1)
		{
			//now we need to construct Pij in some way
			//Pij = r0 - [w = width , h = height] + [w/n(j + 0.5), h/m(i +0.5)]

			//Assign Px
			Px = 0 - (width/2) + (pixwidth * (j + 0.5));
			//assign the vector to be subtracted from R0 for Pij
			Pij = v3_assign(Px, Py, focal_length);
		
			//now construct Rd = Pij - R0
			v3_subtract(Rd, Pij, R0);

			//shoot the ray at that direction and save as color vector
			V3 color = raycast(Rd, R0, list);

			pixmap[n * i + j].r = color[0] * 255;
			pixmap[n * i + j].g = color[1] * 255;
			pixmap[n * i + j].b = color[2] * 255;
			
		}
		printf(".");
	}

	writeToP3(pixmap, n, m, output);
	printf("Complete \n");
	return 0;

}




