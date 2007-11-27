#ifndef _OGLSYSTEM_H_
#define _OGLSYSTEM_H_

#include <gl/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include "IDriver.h"
 
/**
 *  opengl实现的IVideoDriver版本.
 */
class COglSystem: public IVideoDriver
{
public:
	/**
 	 *  构造函数.
         */
	COglSystem();
	virtual void BeginScene(int flag);
      	virtual void EndScene();
      	virtual void RenderQuad(CTexture* texture,float sx,float sy,float sw,float sh,float dx,float dy,
	virtual void RenderQuadFast(CTexture* texture,float dx,float dy);
        virtual void FpsInit();
        virtual void ShowFps(); 
        virtual void DrawLine(float x1, float y1, float x2, float y2, int color);
        virtual void DrawLine(Cpointf p1, Cpointf p2, int color);
        virtual void DrawRect(float x, float y, float width, float height,int color);
        virtual void DrawRect(CRectf rect,int color);
        virtual void FillRect(float x, float y, float width, float height,int color);
        virtual void FillRect(CRectf rect,int color);
protected:
private:
	int m_update;
	int m_frame;
	int m_t0;
};
#endif
