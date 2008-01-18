#ifndef _DEF_H_
#define _DEF_H_

//common
#ifndef NULL
#define NULL 0
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif




//Display
#define MAX_DISPLAY_CELLS_ROW 8		
#define MAX_DISPLAY_CELLS_COL 15
#define CELL_SIZE 32                //cell size 32*32 pixel
#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 272

//draw
#define XSCALE 1.0 
#define YSCALE 1.0
#define ANGLE  0.0
#define DEFAULTCOLOR 0xffffffff  

#define MAX_Z_ORDER 99999 

struct _RECT
{
	int left;
	int top;
	int width;
	int height;
	_RECT(int l, int t ,int w, int h)
	{
		left = l;
		top = t;
		width = w;
		height = h;
	}
};


#endif