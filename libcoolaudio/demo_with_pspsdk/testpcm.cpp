#include "interface.h"
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <pspdisplay.h>

int SetupCallbacks(); 

PSP_MODULE_INFO("PCM TEST", 0x1000, 1, 1); 
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU); 

__attribute__ ((constructor)) 
void loaderInit(){ 
   pspKernelSetKernelPC(); 
   pspSdkInstallNoDeviceCheckPatch(); 
   pspSdkInstallNoPlainModuleCheckPatch(); 
   pspSdkInstallKernelLoadModulePatch(); 
} 
play_ops ops;
SceCtrlData input;

int main(int argc, char *argv[]){
	
	SetupCallbacks(); 
	pspDebugScreenInit();
	pspDebugScreenSetXY(0, 2);
    
	//init 
	PCMPlayInit(&ops);
	//load and play
	int res = ops.load("ms0:/Test.wav");
	pspDebugScreenPrintf("%d\n", res);
	
	if (res == 1) {
		ops.play();
		pspDebugScreenPrintf("playing....\n");
		sceCtrlReadBufferPositive(&input, 1);
		do{
			sceKernelDelayThread(100);
			sceCtrlReadBufferPositive(&input, 1);
		}while( ops.eos()!=1||!(input.Buttons & PSP_CTRL_TRIANGLE));
	}
	
	PCMPlayFini();
	sceKernelExitGame();
	return 0;
}

/* Exit callback */ 
int exit_callback(int arg1, int arg2, void *common) 
{ 
   sceKernelExitGame(); 
   return 0; 
} 


/* Callback thread */ 
int CallbackThread(SceSize args, void *argp) 
{ 
   int cbid; 

   cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL); 
   sceKernelRegisterExitCallback(cbid); 

   sceKernelSleepThreadCB(); 

   return 0; 
} 


/* Sets up the callback thread and returns its thread id */ 
int SetupCallbacks(void) 
{ 
   int thid = 0; 

   thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0); 
   if(thid >= 0) 
   { 
      sceKernelStartThread(thid, 0, 0); 
   } 

   return thid; 
}
