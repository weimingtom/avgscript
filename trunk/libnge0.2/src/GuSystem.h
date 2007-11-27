#ifndef GUSYSTEM_H_
#define GUSYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>
#include <psprtc.h>
#include "IDriver.h"
#include "vram.h"
#include "SDL.h"
#include "SDL_image.h"
/**
 *  a gu implament of IVideoDriver.
 */
class CGuSystem: public IVideoDriver
{
public:
	/**
 	 *  constructor.
        */
	CGuSystem();
	virtual void BeginScene() ;
	virtual void EndScene() ;

	virtual void RenderQuad(CTexture* texture,float sx,float sy,float sw,float sh,float dx,
			float dy,float xscale = 1.0,float yscale = 1.0,float angle = 0.0,int color=0xffffffff);
     
	virtual void RenderQuadFast(CTexture* texture,float dx,float dy){};
     
	virtual void FpsInit();
	virtual void ShowFps();
protected:
private:
	void InitGu(void);
	void* m_drawbuf;
	void* m_displaybuf;
	void* m_zbuf;
	ScePspFVector3 m_transmatrix;
	/**debug show fps*/
	u64 m_lasttick;
	u64 m_currtick;
	u32 m_tickres;
	float m_currms;
	int m_fcount;
};
#endif
