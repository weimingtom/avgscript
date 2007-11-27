#include "libnge.h"

IVideoDriver* driver =0;
SDL_Surface* surface;
void NGE_InitSDL(int flag)
{
	if ( SDL_Init(flag) < 0 ) 
   		 exit(1);
}
IVideoDriver* NGE_GetVideoDriver()
{
	//only one instants
	if(driver != NULL)
	    return driver;
	#ifdef WIN32
	 
			SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    
			surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
		        SDL_OPENGL|SDL_GL_DOUBLEBUFFER );
	       driver = new COglSystem;
	#else
	       driver = new CGuSystem;
	#endif
	
    return  driver;             
}

void NGE_QuitSDL()
{
	if(driver){
		delete driver;
		driver = NULL;
	}
	SDL_Quit();
}