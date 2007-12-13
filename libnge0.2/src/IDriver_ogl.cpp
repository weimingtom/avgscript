#include "IDriver_ogl.h"

#define SINF(a)  (m_sintable[a%360])
#define COSF(a)  (m_costable[a%360])

IVideoDriver::IVideoDriver()
{
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	m_clrcolor.r = 0.0;
	m_clrcolor.g = 0.0;
	m_clrcolor.b = 0.0;
	m_clrcolor.a = 1.0;
	m_colorint = 0xff000000;
	for (int i=0;i<360;i++)
	{
		m_sintable[i] = sinf(i*DEG2RAD);
		m_costable[i] = cosf(i*DEG2RAD);
	}
}

void IVideoDriver::SetClearColor(int color)
{	
	m_colorint = color;
	m_clrcolor.r = GET_COLOR_R(color)/255.0;
	m_clrcolor.g = GET_COLOR_G(color)/255.0;
	m_clrcolor.b = GET_COLOR_B(color)/255.0;
	m_clrcolor.a = GET_COLOR_A(color)/255.0;
}

int  IVideoDriver::GetClearColor()
{  
	return m_colorint; 
}

void IVideoDriver::BeginScene(int flag)
{
	glClearColor( m_clrcolor.r, m_clrcolor.g, m_clrcolor.b, m_clrcolor.a );
	if(flag)
		glClear( GL_COLOR_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0,480.0,272.0,0,-1, 1);
}

void IVideoDriver::EndScene()
{
	SDL_GL_SwapBuffers();
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

void IVideoDriver::DrawCircle(float x, float y, float radius, int color)
{
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color),GET_COLOR_B(color), GET_COLOR_A(color));
	glBegin(GL_LINE_LOOP);

		for(int i=0; i<360;i+=1)
		{
			glVertex2f(x+radius*COSF(i), y+radius*SINF(i));
		}

	glEnd();
	glPopAttrib();

}

void IVideoDriver::FillCircle(float x, float y, float radius, int color)
{
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color),GET_COLOR_B(color), GET_COLOR_A(color));
	glBegin(GL_TRIANGLE_FAN);

		for(int i=0; i<360;i+=1)
		{
			glVertex2f(x+radius*COSF(i), y+radius*SINF(i));
		}

	glEnd();
	glPopAttrib();

}

void IVideoDriver::FillPie(float x, float y, float radius,float theta, int color)
{
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color),GET_COLOR_B(color), GET_COLOR_A(color));
	glBegin(GL_TRIANGLE_FAN);

	//	for(int i=0; i<theta;i+=1)
	//	{
			glVertex2f(0, 0);
			glVertex2f(100, 100+20);
			glVertex2f(100, 100);
	//	}

	glEnd();
	glPopAttrib();

}


void   IVideoDriver::FillEllipse(float x,float y ,float   xradius,   float   yradius,int color)   
 {   
        glPushAttrib(GL_CURRENT_BIT);
		glDisable(GL_TEXTURE_2D);
		glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color),GET_COLOR_B(color), GET_COLOR_A(color));
		glBegin(GL_TRIANGLE_FAN);   
			for   (int   i=0;   i   <   360;   i++)   
			{   
				  glVertex2f(x+COSF(i)*xradius,y+SINF(i)*yradius);   
			}   
        glEnd();
		glPopAttrib();
 }   


void   IVideoDriver::DrawEllipse(float x,float y ,float   xradius,   float   yradius,int color)   
 {   
        glPushAttrib(GL_CURRENT_BIT);
		glDisable(GL_TEXTURE_2D);
		glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color),GET_COLOR_B(color), GET_COLOR_A(color));
		glBegin(GL_LINE_LOOP);   
			for   (int   i=0;   i   <   360;   i++)   
			{   
				  glVertex2f(x+COSF(i)*xradius,y+SINF(i)*yradius);   
			}   
        glEnd();
		glPopAttrib();
 }   




void IVideoDriver::DrawLine(float x1, float y1, float x2, float y2, int color)
{
	if(y1 == 0.0)
		y1 = 0.1;
	if(y2 == 0.0)
		y2 = 0.1;
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color), GET_COLOR_B(color), GET_COLOR_A(color));
	glBegin(GL_LINES);
		glVertex2f(x1, y1);	
		glVertex2f(x2, y2);
	glEnd();
	glPopAttrib();
}

void IVideoDriver::DrawLine(CPointf p1, CPointf p2, int color)
{
	DrawLine(p1.x, p1.y, p2.x, p2.y,color);
}

void IVideoDriver::DrawRect(float dx, float dy, float width, float height,int color)
{
	if(dy == 0.0)
		dy = 0.1;
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color), GET_COLOR_B(color), GET_COLOR_A(color));
	glBegin(GL_LINE_LOOP);
		glVertex2f(dx, dy);	
		glVertex2f(dx,dy+height);
		glVertex2f(dx+width,dy+height);	
		glVertex2f(dx+width,dy);
	glEnd();
	glPopAttrib();
}

void IVideoDriver::DrawRect(CRectf rect,int color)
{
	DrawRect(rect.topleft.x, rect.topleft.y, rect.rightbottom.x, rect.rightbottom.y,color);
}

void IVideoDriver::FillRect(float dx, float dy, float width, float height,int color)
{
	if(dy == 0.0)
		dy = 0.1;
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color), GET_COLOR_B(color), GET_COLOR_A(color));
	glBegin(GL_QUADS);
		glVertex2f(dx, dy);	
		glVertex2f(dx,dy+height);
		glVertex2f(dx+width,dy+height);	
		glVertex2f(dx+width,dy);
	glEnd();
	glPopAttrib();
}

void IVideoDriver::FillRect(CRectf rect,int color)
{
	FillRect(rect.topleft.x, rect.topleft.y, rect.rightbottom.x, rect.rightbottom.y,color);
}

void IVideoDriver::FillPolygon(CPointf* array, int count, int color)
{
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color), GET_COLOR_B(color), GET_COLOR_A(color));
	glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < count; i++)
		{
			glVertex2f(array[i].x,array[i].y);
		}
	glEnd();
	glPopAttrib();
}

void IVideoDriver::DrawPolygon(CPointf* array, int count, int color)
{
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color), GET_COLOR_B(color), GET_COLOR_A(color));
	glBegin(GL_LINE_LOOP);
		for(int i=0; i<count;i++)
		{
			glVertex2f(array[i].x,array[i].y);
		}
	glEnd();
	glPopAttrib();

}

void IVideoDriver::FillRoundRect(float x, float y, float w, float h, float radius, int color)
{
	x+=w+radius;
	y+=radius;
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glColor4ub(GET_COLOR_R(color), GET_COLOR_G(color), GET_COLOR_B(color), GET_COLOR_A(color));
	glBegin(GL_TRIANGLE_FAN);

	glVertex2f(x-5, y);

	int i;
	for(i=0; i<90;i++)
	{
		glVertex2f(x+radius*COSF(i), y+radius*SINF(i));
	}
	for(i=0; i<w; i++)
	{
		glVertex2f(x+radius*COSF(90)-i, y+radius*SINF(90));
	}
	for(i=90; i<180;i++)
	{
		glVertex2f(x+radius*COSF(i)-w, y+radius*SINF(i));
	}
	for(i=0; i<h; i++)
	{
		glVertex2f(x+radius*COSF(180)-w, y+radius*SINF(180)-i);
	}

	for(i=180; i<270;i++)
	{
		glVertex2f(x+radius*COSF(i)-w, y+radius*SINF(i)-h);
	}
	for(i=0; i<w; i++)
	{
		glVertex2f(x+radius*COSF(270)-w+i, y+radius*SINF(270)-h);
	}
	for(i=270; i<360;i++)
	{
		glVertex2f(x+radius*COSF(i),y+radius*SINF(i)-h);
	}
	for(i=0; i<h; i++)
	{
		glVertex2f(x+radius*COSF(0), y+radius*SINF(0)-h+i);
	}

	glVertex2f(x+radius*COSF(0), y+radius*SINF(0));

	glEnd();
	glPopAttrib();
}

