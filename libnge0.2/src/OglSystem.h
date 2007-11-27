#ifndef _OGLSYSTEM_H_
#define _OGLSYSTEM_H_

#include <gl/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include "IDriver.h"
 
/**
 *  a opengl implament of IVideoDriver.
 */
class COglSystem: public IVideoDriver
{
public:
	/**
 	 *  constructor.
        */
	COglSystem();
	virtual void BeginScene() ;
	virtual void EndScene() ;
	virtual void RenderQuad(CTexture* texture,float sx,float sy,float sw,float sh,float dx,
			float dy,float xscale = 1.0,float yscale = 1.0,float angle = 0.0,int color=0xffffffff);
	virtual void RenderQuadFast(CTexture* texture,float dx,float dy);
 	virtual CTexture* LoadTexture(const char* filename);
	virtual void FpsInit();
	virtual void ShowFps();
protected:
private:
	int m_update;
	int m_frame;
	int m_t0;
};
#endif
