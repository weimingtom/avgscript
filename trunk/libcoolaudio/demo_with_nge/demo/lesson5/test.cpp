// bigproject.cpp : Defines the entry point for the console application.
//
#pragma warning(disable : 4786)
#include "libnge.h"
#include "interface.h"
#include <time.h>
SDL_Joystick* joystick;
SDL_Event event; 
IVideoDriver* sys ;
play_ops ops;
CTexture* bg[2];
int done = 0;
int id = 0;
float scale = 1.0;
float angel = 0.0;
int angelenable = 0;
//³õÊ¼ÒôÁ¿Îª64£¨0-128£©
int volume = 64;

void ButtonPressedProc(SDL_JoyButtonEvent * jbutton)
{
    switch(jbutton -> button)
    {
	case 8: 
		ops.volume((volume++)%0x128);
		break;
	case 6: 
		ops.volume((volume--)%0x128);
		break;
	case 7: 
		break;
	case 9: 
		break;
	case 2:	
		ops.pause();
		break;
	case 1:
		ops.resume();
		break;
	case 3:
		ops.stop();
		ops.load("test.mp3");
		ops.play();
		break;
   	case 11:
   		done = 1;
		break;
	case 0:
		angelenable = !angelenable;
		break;
    }
	
}                   

void KeyPressedProc(SDL_keysym * keysym)
{
    
	switch(keysym -> sym)
    {

	case SDLK_UP: 
		ops.volume((volume++)%128);
		break;
	case SDLK_DOWN: 
		ops.volume((volume--)%128);
		break;
	case SDLK_LEFT: 
		
		break;
	case SDLK_RIGHT: 
		
		break; 
	case SDLK_s:
		ops.pause();
		break;
   	case SDLK_e:
		ops.resume();
		break;
	case SDLK_h:
		ops.stop();
		ops.load("test.mp3");
		ops.play();
		break;
	case SDLK_j:
		angelenable = !angelenable;
		angel = 0;
		break;
    }
	
} 


void InputProc()
{
	while( SDL_PollEvent( &event ) )
    {
		switch(event.type)
		{
		case SDL_QUIT:
			done = 1;
			break;
		case SDL_KEYDOWN:
			KeyPressedProc(&event.key.keysym);
			break;
		case SDL_JOYBUTTONDOWN:
			ButtonPressedProc(&event.jbutton);
			break;
		}
	}
}


int DrawScene()
{
	sys->BeginScene();
	sys->RenderQuad(bg[id],0,0,480,272,0,0,scale,scale,angel,0xffffffff);
	sys->EndScene();
	scale+=0.001;
	if(scale>2)
		scale = 0.1;
	if(angelenable)
		angel+=1;
    return( TRUE );
}

extern "C"
int main(int argc, char* argv[])
{
	CoolAudioDefaultInit();
	NGE_InitSDL(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);
	
	if(SDL_NumJoysticks()) {
		joystick = SDL_JoystickOpen(0);
	}
	sys = NGE_GetVideoDriver();
	bg[0] = LoadTexture("gamebg1.png");
	bg[1] = LoadTexture("gamebg2.png");
	sys->FpsInit();
	//init
	
	MP3PlayInit(&ops);
	ops.volume(volume);
	//load and play
	int res = ops.load("__SCE__lesson5/Test.mp3");
	ops.play();
	while ( !done )
	{
		InputProc();
		DrawScene();
		sys->ShowFps();
		SDL_Delay(1);
	}
	FreeTexture(bg[0]);
	FreeTexture(bg[1]);
	MP3PlayFini();
	NGE_QuitSDL();
	
	return 0;
}

