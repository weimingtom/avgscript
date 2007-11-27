#ifndef _LIBNGE_GLOBAL_H_
#define _LIBNGE_GLOBAL_H_
#include "IDriver.h"
#include <SDL.h>
#ifdef _cplusplus
extern "C"{
#endif
	CTexture* LoadTexture( const char* filename );
	void FreeTexture(CTexture* tex);
	CTexture* Surface2Texture(SDL_Surface* surface);
#ifdef _cplusplus
}
#endif

#endif