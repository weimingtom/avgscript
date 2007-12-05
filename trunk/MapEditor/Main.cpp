#pragma warning(disable : 4786)
#include "libnge.h"
#include <time.h>

#include "../Public/include/Map.h"
#include "../Public/include/Ini.h"
#include "MapEdit.h"
SDL_Joystick* joystick;
SDL_Event event; 
IVideoDriver* sys ;

CTexture* bg[2];
int done = 0;
int id = 0;
float scale = 1.0;
float angel = 1.0;
	//CMap mapt;

CMapEdit g_Mapedit;

void ButtonPressedProc(SDL_JoyButtonEvent * jbutton)
{
    switch(jbutton -> button)
    {
	case 8: 
		angel=180;
		break;
	case 6: 
		angel =0;
		break;
	case 7: 
		angel = 360;
		break;
	case 9: 
		break;
	case 2:	
		scale+=0.05;
		break;
	case 1:
		id++;
		id=id%2;
		break;
	case 3:
		angel+=1.0;
		break;
   case 11:
		
		break;
	case 0:
		
		break;
    }
	
}                   

void MouseMoveProc(SDL_MouseMotionEvent* MouseEvent)
{

	g_Mapedit.MoveTo(MouseEvent->x, MouseEvent->y);

	
}

void MouseUpProc(SDL_MouseButtonEvent * MouseButton)
{
	
}

void KeyPressedProc(SDL_keysym * keysym)
{
    
	switch(keysym -> sym)
    {

	case SDLK_UP: 
		g_Mapedit.Move(0,-32);
	//	mapt.ScrollMap(0,-8);
		break;
	case SDLK_DOWN: 
		g_Mapedit.Move(0,32);
	//	mapt.ScrollMap(0,8);
		
		break;
	case SDLK_LEFT: 
		g_Mapedit.Move(-32,0);
	//	mapt.ScrollMap(-8,0);
		
		break;
	case SDLK_RIGHT: 
		g_Mapedit.Move(32,0);
	//	mapt.ScrollMap(8,0);
		
		break; 
	case SDLK_s:
		
		break;
   	case SDLK_e:
		
		break;
	case SDLK_h:
		
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
		case SDL_MOUSEMOTION:
			MouseMoveProc(&event.motion);
		case SDL_MOUSEBUTTONUP:
			MouseUpProc(&event.button);
			break;
		}
	}
}


int DrawScene()
{
	sys->BeginScene();
//	sys->RenderQuad(bg[id],0,0,480,272,0,0,scale,scale,angel,0xffffffff);
//	sys->RenderQuad(mapt.,0,0,480,272,0,0,scale,scale,angel,0xffffffff);
	//mapt.Draw();
	g_Mapedit.Draw();
	sys->EndScene();
    return( TRUE );
}



void test()
{

//	mapt.Init(sys);

	g_Mapedit.Init(sys);
	
	g_Mapedit.Test();


	g_Mapedit.ChangeStatus(MapEdit_Status_Select);
//	mapt.Test();

	
}

extern "C"
int main(int argc, char* argv[])
{
//	return 0;
	NGE_InitSDL(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);
	
	if(SDL_NumJoysticks()) {
		joystick = SDL_JoystickOpen(0);
	}
	sys = NGE_GetVideoDriver();
	bg[0] = LoadTexture("gamebg1.png");
	bg[1] = LoadTexture("gamebg2.png");
	test();
	sys->FpsInit();

	

	while ( !done )
	{
		InputProc();
		DrawScene();
		sys->ShowFps();
	}
	FreeTexture(bg[0]);
	FreeTexture(bg[1]);
	NGE_QuitSDL();
	
	return 0;
}