#include "OglSystem.h"
#include <SDL.h>
#include <SDL_image.h>
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "glaux.lib" )
#pragma comment( lib, "freetype.lib" )

COglSystem::COglSystem()
{
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
  
}

void COglSystem::BeginScene()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );
	glClear( GL_COLOR_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0,480,272,0,-1, 1);
	m_update = 0;
}

void COglSystem::EndScene()
{
	if(m_update){
		SDL_GL_SwapBuffers();
		m_update = 0;
	}
}


static inline int RoundUpToPowerOfTwo(int x)
{
	int b = x;
	int n;
	for (n = 0; b != 0; n++) b >>= 1;
	b = 1 << n;
	if (b == 2 * x) b >>= 1;
	return b;
}

CTexture* COglSystem::LoadTexture( const char* filename )	
{
	CTexture* texture;
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
		
		
	}
	
	if( pTextureImage != NULL )
	{
		SDL_FreeSurface( pTextureImage );
	}
	return texture;
}

void COglSystem::RenderQuad(CTexture* texture,float sx,float sy,float sw,float sh,float dx,float dy,float xscale ,float yscale,float angle,int color )
{
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(dx+texture->rcentrex*xscale,dy+texture->rcentrey*yscale,0);
	glRotatef(angle,0,0,1);
	glTranslatef(-(dx+texture->rcentrex*xscale),-(dy+texture->rcentrey*yscale),0);
	
    glBindTexture( GL_TEXTURE_2D, texture->texid );
	if(sw==0&&sh==0){
		glPushAttrib(GL_CURRENT_BIT);
		glColor4ub(color&0xff,color>>8&0xff,color>>16&0xff,color>>24&0xff);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(dx, dy, 0);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f( dx+texture->texw*xscale,dy,0);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( dx+texture->texw*xscale,dy+texture->texh*yscale,0);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(dx,dy+texture->texh*yscale,0);	// Top Left Of The Texture and Quad
		glEnd();
		glPopAttrib();
	}
	else{
		glPushAttrib(GL_CURRENT_BIT);
		glColor4ub(color&0xff,color>>8&0xff,color>>16&0xff,color>>24&0xff);
		glBegin(GL_QUADS);
			glTexCoord2f(sx/texture->texw, sy/texture->texh); glVertex3f(dx, dy, 0);	// Bottom Left Of The Texture and Quad
			glTexCoord2f((sx+sw)/texture->texw, sy/texture->texh); glVertex3f( dx+sw*xscale,dy,0);	// Bottom Right Of The Texture and Quad
			glTexCoord2f((sx+sw)/texture->texw, (sy+sh)/texture->texh); glVertex3f( dx+sw*xscale,dy+sh*yscale,0);	// Top Right Of The Texture and Quad
			glTexCoord2f(sx/texture->texw, (sy+sh)/texture->texh); glVertex3f(dx,dy+sh*yscale,0);	// Top Left Of The Texture and Quad
		glEnd();
		glPopAttrib();
	}
	m_update = 1;
}

void COglSystem::RenderQuadFast(CTexture* texture,float dx,float dy)
{
	glBindTexture( GL_TEXTURE_2D, texture->texid );
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(dx, dy, 0);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( dx+texture->texw,dy,0);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( dx+texture->texw,dy+texture->texh,0);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(dx,dy+texture->texh,0);	// Top Left Of The Texture and Quad
	glEnd();
	m_update = 1;
}

void COglSystem::FpsInit()
{
	m_frame = 0;
	m_t0 = 0;
}

void COglSystem::ShowFps()
{
	m_frame++;
   	int t = SDL_GetTicks();
	if (t - m_t0 >= 1000) {
			float seconds = (t - m_t0) / 1000.0;
			float fps = m_frame / seconds;
			printf("%d frames in %g seconds = %g FPS\n", m_frame, seconds, fps);
			m_t0 = t;
			m_frame = 0;

	}
    
}

