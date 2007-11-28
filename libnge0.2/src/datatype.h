#ifndef  _DATATYPE_H_
#define  _DATATYPE_H_
#ifdef WIN32
   #include <windows.h>		// Header File For Windows
   #include <gl\gl.h>			// Header File For The OpenGL32 Library
   #include <gl\glu.h>			// Header File For The GLu32 Library
   #include <gl\glaux.h>		// Header File For The Glaux Library
#else
     	#include <pspkernel.h>
	#include <pspdisplay.h>
	#include <pspdebug.h>
	#include <stdlib.h>
	#include <pspctrl.h>
	#include <pspgu.h>
	#include <pspgum.h>  
#endif   
/** @file datatype.h
 *  @brief datatype define file.
 */
#ifndef uint32 
#define uint32 unsigned int
#endif
#ifndef uint8 
#define uint8  unsigned char
#endif

#ifndef uint16 
	#define uint16 unsigned short
#endif

#ifndef u32
	#define u32 unsigned int
#endif

#ifndef u8
	#define u8 unsigned char 
	#endif
	#ifndef u16
	#define u16 unsigned short
	#endif
#ifndef word
	typedef unsigned short word;
	#endif
	#ifndef dword
	typedef unsigned long dword;
	#endif	
/**
 * pspsdk defined those but why it not define above...
 */
#ifdef WIN32
	#ifndef int32 
	#define int32  int
	#endif
	
	#ifndef int16 
	#define int16 short
	#endif
	#ifndef uint8 
	#define uint8  unsigned char
	#endif
	#ifndef int8 
	#define int8  char
	#endif
	
	
	
#endif

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 272
#define SCREEN_BPP     32
#define TRUE  1
#define FALSE 0

//#define SCR_WIDTH    480
//#define SCR_HEIGHT   272
/**
 *  @brief make rgba .
 */
#define RGBA(r,g,b,a) ((r)+((g)<<8)+((b)<<16)+((a)<<24))
#define GET_COLOR_R(color) (color&0x000000ff)
#define GET_COLOR_G(color) ((color>>8)&0x000000ff)
#define GET_COLOR_B(color) ((color>>16)&0x000000ff)
#define GET_COLOR_A(color) ((color>>24)&0x000000ff)

/**
 *  @brief A type definition for point .
 */
typedef struct {
	float x; /**< x coodinate. */
	float y; /**< y coodinate. */
}CPointf;

/**
 * @brief A type definition for rectagle
 */
typedef struct
{
	float top;    /**< top of rect. */
	float left;   /**< left of rect. */
	float right;  /**< right of rect. */
	float bottom; /**< bottom of rect. */
}CRectf;


/**
 * @brief CTexture hold texture.
 */
struct CTexture
{
public:	
	float    w;       /**<  original picture width. */
	float    h;       /**<  original picture height. */
	float    texw;    /**<  texture width,align by power of 2. */
	float    texh;    /**<  texture height,align by power of 2.*/
	uint32   texid;   /**<  texid using in opengl. */
	uint32   swizzle; /**< swizzle tag using in pspgu. */
	uint8*   data;    /**< swizzled data using in pspgu. */
	uint32   mode;    /**< texture mode. */
	float  rcentrex;  /**< rotate centre y ,default is original picture centre */
	float  rcentrey;  /**< rotate centre y ,default is original picture centre */
      /**
       * set rotate centre.
       * @param x      float,x coodinate of rotate centre.
       * @param y      float,y coodinate of rotate centre.
       * @return non-return. 
       */
	void SetRcentre(float x,float y) {rcentrex =x,rcentrey=y;}
};

/**
 * @brief use for cache data (inner use).
 */
typedef struct _CacheData{
	int key;
	int size;
	int style;
	CTexture texture;
}CacheData;

/**
 * @brief use for cache list (inner use).
 */
typedef struct _Item {
    CacheData* data;
    struct _Item *next;
    struct _Item *prev;
}Item;

#endif