#include "Global.h"

IVideoDriver* g_pVideoDriver;

IVideoDriver* GetVideoDriver()
{
	return g_pVideoDriver;
}
