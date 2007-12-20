#ifndef _DEF_H_
#define _DEF_H_

//common
#ifndef NULL
#define NULL 0
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

//CIni
#define MAX_KEY_LEN 128
#define MAX_INDEX_LEN 128
#define MAX_VALUE_BUF_LEN 256

//CMap

#define MAX_MAP_TEXTURE_NUM 512		//max pic num 
#define MAX_MAP_CELLS 64			//max map size 64*64
#define MAX_MAP_ROWS 64
#define MAX_MAP_COLS 64

#define GROUND_LAYER_1   0x01		//ground layer
#define OBJECT_LAYER_1   0x02       //object layer1
#define OBJECT_LAYER_2   0x04       //object layer2
#define ALL_LAYER (GROUND_LAYER_1|OBJECT_LAYER_1|OBJECT_LAYER_2)


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