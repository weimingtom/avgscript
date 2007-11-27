#include "libnge_global.h"
#include <SDL.h>
#include <SDL_image.h>
#ifdef WIN32
	#include <gl/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glaux.h>
#else
   	#include <pspkernel.h>
	#include <pspdisplay.h>
	#include <pspdebug.h>
	#include <stdlib.h>
	#include <pspctrl.h>
	#include <pspgu.h>
	#include <pspgum.h>

#endif
static int id_malloc = 1;
static inline int RoundUpToPowerOfTwo(int x)
{
	int b = x;
	int n;
	for (n = 0; b != 0; n++) b >>= 1;
	b = 1 << n;
	if (b == 2 * x) b >>= 1;
	return b;
}

#ifdef WIN32
CTexture* LoadTexture( const char* filename )	
{
	CTexture* texture = NULL;
	char* buffer;
    /* Create storage space for the texture */
    int y = 0;
	char* line;
	int bpb = 0;
	SDL_Surface*  pTextureImage = IMG_Load(filename);
	if( pTextureImage != NULL )
	{
		texture= new CTexture;
		glGenTextures( 1, &texture->texid );

		glBindTexture( GL_TEXTURE_2D, texture->texid );
		texture->w = pTextureImage->w;
		texture->h = pTextureImage->h;
		texture->texw = RoundUpToPowerOfTwo(pTextureImage->w);
		texture->texh = RoundUpToPowerOfTwo(pTextureImage->h);
		texture->rcentrex = pTextureImage->w/2;
		texture->rcentrey = pTextureImage->h/2;
		bpb = pTextureImage->format->BytesPerPixel;
	/*	if(bpb!=4){
			SDL_PixelFormat fmt;
			memset(&fmt,0,sizeof(SDL_PixelFormat));
			fmt.BitsPerPixel = 0x20;
			fmt.BytesPerPixel = 0x04;
			/*fmt.Gshift = 0x08;
			fmt.Bshift = 0x10;
			fmt.Ashift = 0x18;
			fmt.Rmask = 0x000000ff;
			fmt.Gmask = 0x0000ff00;
			fmt.Bmask = 0x00ff0000;
			fmt.Amask = 0xff000000;
			fmt.alpha = 0x00;
			SDL_Surface* tmp = SDL_ConvertSurface(pTextureImage,&fmt,SDL_SWSURFACE);
			SDL_FreeSurface( pTextureImage );
			pTextureImage = tmp;
			bpb =4;
		}*/
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		buffer=(char*)malloc(texture->texw*texture->texh*bpb);
		memset(buffer,0,texture->texw*texture->texh*bpb);
		line = (char*)pTextureImage->pixels;
		for(y=0;y<pTextureImage->h;y++){
				memcpy(buffer+y*((int)(texture->texw*bpb)),line,texture->w*bpb);
				line=line+(int)(texture->w*bpb);
		}
		int type = (bpb==4?GL_RGBA:GL_RGB);
		glTexImage2D( GL_TEXTURE_2D, 0, bpb,texture->texw,texture->texh, 0,
			type, GL_UNSIGNED_BYTE,buffer );
		
		if(buffer)
			free(buffer);
		
		
	}
	
	if( pTextureImage != NULL )
	{
		SDL_FreeSurface( pTextureImage );
		pTextureImage = NULL;
	}
	return texture;
}


void FreeTexture(CTexture* tex)
{
	if(tex!=NULL){
		glDeleteTextures( 1, &	tex->texid );
		delete(tex);
		tex =NULL;
	}

}

CTexture* Surface2Texture(SDL_Surface* pTextureImage)
{
	CTexture* texture = NULL;
	char* buffer;
    /* Create storage space for the texture */
    int y = 0;
	char* line;
	int bpb = 0;
	if( pTextureImage != NULL )
	{
		texture= new CTexture;
		glGenTextures( 1, &texture->texid );

		glBindTexture( GL_TEXTURE_2D, texture->texid );
		texture->w = pTextureImage->w;
		texture->h = pTextureImage->h;
		texture->texw = RoundUpToPowerOfTwo(pTextureImage->w);
		texture->texh = RoundUpToPowerOfTwo(pTextureImage->h);
		texture->rcentrex = pTextureImage->w/2;
		texture->rcentrey = pTextureImage->h/2;
		bpb = pTextureImage->format->BytesPerPixel;
		/*	if(bpb!=4){
			SDL_PixelFormat fmt;
			memset(&fmt,0,sizeof(SDL_PixelFormat));
			fmt.BitsPerPixel = 0x20;
			fmt.BytesPerPixel = 0x04;
			fmt.Gshift = 0x08;
			fmt.Bshift = 0x10;
			fmt.Ashift = 0x18;
			fmt.Rmask = 0x000000ff;
			fmt.Gmask = 0x0000ff00;
			fmt.Bmask = 0x00ff0000;
			fmt.Amask = 0xff000000;
			fmt.alpha = 0x00;
			SDL_Surface* tmp = SDL_ConvertSurface(pTextureImage,&fmt,SDL_SWSURFACE);
			SDL_FreeSurface( pTextureImage );
			pTextureImage = tmp;
			bpb =4;
		}*/
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		buffer=(char*)malloc(texture->texw*texture->texh*bpb);
		memset(buffer,0,texture->texw*texture->texh*bpb);
		line = (char*)pTextureImage->pixels;
		for(y=0;y<pTextureImage->h;y++){
				memcpy(buffer+y*((int)(texture->texw*bpb)),line,texture->w*bpb);
				line=line+(int)(texture->w*bpb);
		}
		int type = (bpb==4?GL_RGBA:GL_RGB);
		glTexImage2D( GL_TEXTURE_2D, 0, bpb,texture->texw,texture->texh, 0,
			type, GL_UNSIGNED_BYTE,buffer );
		
		if(buffer)
			free(buffer);
	}
	
	if( pTextureImage != NULL )
	{
		SDL_FreeSurface( pTextureImage );
		pTextureImage = NULL;
	}
	return texture;
}


#else


static void swizzle_fast(u8* out, const u8* in, unsigned int width, unsigned int height)
{
   unsigned int blockx, blocky;
   unsigned int j;
 
   unsigned int width_blocks = (width / 16);
   unsigned int height_blocks = (height / 8);
 
   unsigned int src_pitch = (width-16)/4;
   unsigned int src_row = width * 8;
 
   const u8* ysrc = in;
   u32* dst = (u32*)out;
 
   for (blocky = 0; blocky < height_blocks; ++blocky)
   {
      const u8* xsrc = ysrc;
      for (blockx = 0; blockx < width_blocks; ++blockx)
      {
         const u32* src = (u32*)xsrc;
         for (j = 0; j < 8; ++j)
         {
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            src += src_pitch;
         }
         xsrc += 16;
     }
     ysrc += src_row;
   }
}

CTexture* LoadTexture(const char* filename)
{
	CTexture* texture =NULL;
	unsigned char* buffer;
    	/* Create storage space for the texture */
    	int y = 0;
	unsigned char* line;
	int bpb = 0;
	SDL_Surface*  pTextureImage = IMG_Load(filename);
	if( pTextureImage != NULL )
	{
		texture= new CTexture;
		
		texture->w = pTextureImage->w;
		texture->h = pTextureImage->h;
		texture->texw = RoundUpToPowerOfTwo(pTextureImage->w);
		texture->texh = RoundUpToPowerOfTwo(pTextureImage->h);
		texture->rcentrex = pTextureImage->w/2;
		texture->rcentrey = pTextureImage->h/2;
		bpb = pTextureImage->format->BytesPerPixel;
			if(bpb!=4){
			SDL_PixelFormat fmt;
			memset(&fmt,0,sizeof(SDL_PixelFormat));
			fmt.BitsPerPixel = 0x20;
			fmt.BytesPerPixel = 0x04;
			fmt.Gshift = 0x08;
			fmt.Bshift = 0x10;
			fmt.Ashift = 0x18;
			fmt.Rmask = 0x000000ff;
			fmt.Gmask = 0x0000ff00;
			fmt.Bmask = 0x00ff0000;
			fmt.Amask = 0xff000000;
			fmt.alpha = 0xff;
			SDL_Surface* tmp = SDL_ConvertSurface(pTextureImage,&fmt,SDL_SWSURFACE);
			SDL_FreeSurface( pTextureImage );
			pTextureImage = tmp;
			bpb =4;
		}
		buffer=(unsigned char*)malloc((int)(texture->texw*texture->texh*bpb));
		texture->data = (unsigned char*)malloc((int)(texture->texw*texture->texh*bpb));
		memset(buffer,0,(int)(texture->texw*texture->texh*bpb));
		line = (unsigned  char*)pTextureImage->pixels;
		for(y=0;y<pTextureImage->h;y++){
				memcpy(buffer+y*((int)(texture->texw*bpb)),line,texture->w*bpb);
				line=line+(int)(texture->w*bpb);
		}
		sceKernelDcacheWritebackAll();
		texture->mode = (bpb==4?GU_PSM_8888:GU_PSM_5650 );
		
		swizzle_fast(texture->data,buffer,(int)texture->texw*bpb,(int)texture->texh);
		sceKernelDcacheWritebackAll();
		texture->swizzle = 1;
		texture->rcentrex = pTextureImage->w/2;
		texture->rcentrey = pTextureImage->h/2;
		free(buffer);
		
	}
	
	if( pTextureImage != NULL )
	{
		SDL_FreeSurface( pTextureImage );
		pTextureImage = NULL;
	}
	texture->texid = id_malloc++;
	return texture;
}

void FreeTexture(CTexture* tex)
{
	if(tex){
		if(tex->data){
		   free(tex->data);
		   tex->data = NULL;
		}
	      free(tex);
	      tex=NULL;	
	}	
}

CTexture* Surface2Texture(SDL_Surface* pTextureImage)
{
	CTexture* texture =NULL;
	unsigned char* buffer;
    	/* Create storage space for the texture */
    	int y = 0;
	unsigned char* line;
	int bpb = 0;
	if( pTextureImage != NULL )
	{
		texture= new CTexture;
		
		texture->w = pTextureImage->w;
		texture->h = pTextureImage->h;
		texture->texw = RoundUpToPowerOfTwo(pTextureImage->w);
		texture->texh = RoundUpToPowerOfTwo(pTextureImage->h);
		texture->rcentrex = pTextureImage->w/2;
		texture->rcentrey = pTextureImage->h/2;
		bpb = pTextureImage->format->BytesPerPixel;
			if(bpb!=4){
			SDL_PixelFormat fmt;
			memset(&fmt,0,sizeof(SDL_PixelFormat));
			fmt.BitsPerPixel = 0x20;
			fmt.BytesPerPixel = 0x04;
			fmt.Gshift = 0x08;
			fmt.Bshift = 0x10;
			fmt.Ashift = 0x18;
			fmt.Rmask = 0x000000ff;
			fmt.Gmask = 0x0000ff00;
			fmt.Bmask = 0x00ff0000;
			fmt.Amask = 0xff000000;
			fmt.alpha = 0xff;
			SDL_Surface* tmp = SDL_ConvertSurface(pTextureImage,&fmt,SDL_SWSURFACE);
			SDL_FreeSurface( pTextureImage );
			pTextureImage = tmp;
			bpb =4;
		}
		buffer=(unsigned char*)malloc((int)(texture->texw*texture->texh*bpb));
		texture->data = (unsigned char*)malloc((int)(texture->texw*texture->texh*bpb));
		memset(buffer,0,(int)(texture->texw*texture->texh*bpb));
		line = (unsigned  char*)pTextureImage->pixels;
		for(y=0;y<pTextureImage->h;y++){
				memcpy(buffer+y*((int)(texture->texw*bpb)),line,texture->w*bpb);
				line=line+(int)(texture->w*bpb);
		}
		sceKernelDcacheWritebackAll();
		texture->mode = (bpb==4?GU_PSM_8888:GU_PSM_5650 );
		
		swizzle_fast(texture->data,buffer,(int)texture->texw*bpb,(int)texture->texh);
		sceKernelDcacheWritebackAll();
		texture->swizzle = 1;
		texture->rcentrex = pTextureImage->w/2;
		texture->rcentrey = pTextureImage->h/2;
		free(buffer);
		
	}
	
	if( pTextureImage != NULL )
	{
		SDL_FreeSurface( pTextureImage );
		pTextureImage = NULL;
	}
	texture->texid = id_malloc++;
	return texture;
}





#endif