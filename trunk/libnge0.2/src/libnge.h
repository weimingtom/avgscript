#include "datatype.h"
#include "libnge_global.h"
#include "IDriver.h"
#include "SDL.h"
#include "SDL_image.h"
#ifdef WIN32
#include <windows.h>		// Header File For Windows
#include "OglSystem.h"
#pragma comment( lib, "SDL_image" )	
#pragma comment( lib, "SDLmain" )
#pragma comment( lib, "SDL" )
#pragma comment( lib, "SDL_mixer" )
#pragma comment( lib, "freetype.lib")
#pragma comment( lib, "libnge.lib" )
#else //psp
#include "GuSystem.h"
#endif


#ifdef __cplusplus
extern "C"{
#endif	

	void NGE_InitSDL(int flag);
	IVideoDriver* NGE_GetVideoDriver();
	void NGE_QuitSDL();

#ifdef __cplusplus
}
#endif

