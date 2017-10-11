#ifndef _RAYCASTER_H
#define _RAYCASTER_H

typedef struct Pixel {
	unsigned char r,g,b;
} Pixel;

int render(int height, int width, OBJECT_LIST_STR *list, char* output);

#endif /*_RAYCASTER_H */
