#include "play_mp3.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"

static int  PCM_Load (const char *);
static int  PCM_Play (void);
static void PCM_Pause(void);
static int  PCM_Stop (void);
static void PCM_Resume(void);
static void PCM_Seek (int);
static int  PCM_Time (void);
static int  PCM_Eos  (void);
static int  PCM_Volume(int);


