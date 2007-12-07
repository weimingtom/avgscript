#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#ifdef WIN32
#include "win_private/CFileSystem_win.h"
#else
#include "psp_private/CFileSystem_psp.h"
#endif

#endif
