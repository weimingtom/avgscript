#include "IDriver_ogl.h"

IVideoDriver::IVideoDriver()
{
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void SetClearColor(int color)
{	m_colorint = color;
	m_clrcolor.r = 
}
int  GetClearColor(){  return m_clrcolor; }; 

void IVideoDriver::BeginScene(int flag)
{
	glClearColor( m_clrcolor.r, m_clrcolor.g, m_clrcolor.b, m_clrcolor.a );
	if(flag)
		glClear( GL_COLOR_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
	glOrtho(0,480,272,0,-1, 1);
	m_update = 0;
}

void IVideoDriver::EndScene()
{
}

void IVideoDriver::RenderQuad(CTexture* texture,float sx,float sy,float sw,float sh,float dx,float dy,
				float xscale,float yscale,float angle,int color)
{
}
				
void IVideoDriver::RenderQuadFast(CTexture* texture,float dx,float dy)
{
}

void IVideoDriver::FpsInit()
{
}

void IVideoDriver::ShowFps()
{
}
void IVideoDriver::DrawLine(float x1, float y1, float x2, float y2, int color)
{
}

void IVideoDriver::DrawLine(Cpointf p1, Cpointf p2, int color)
{
}

void IVideoDriver::DrawRect(float x, float y, float width, float height,int color)
{
}

void IVideoDriver::DrawRect(CRectf rect,int color)
{
}

void IVideoDriver::FillRect(float x, float y, float width, float height,int color)
{
}

void IVideoDriver::FillRect(CRectf rect,int color)
{
}

