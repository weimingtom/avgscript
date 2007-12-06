#include "interface.h"
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <pspaudiocodec.h> 
#include <pspmpeg.h>
#include <pspdisplay.h>

int SetupCallbacks(); 

PSP_MODULE_INFO("MP3 TEST", 0x1000, 1, 1); 
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
    
	int result = pspSdkLoadStartModule("flash0:/kd/me_for_vsh.prx", PSP_MEMORY_PARTITION_KERNEL); 
    
	result = pspSdkLoadStartModule("flash0:/kd/videocodec.prx", PSP_MEMORY_PARTITION_KERNEL); 
    
	result = pspSdkLoadStartModule("flash0:/kd/audiocodec.prx", PSP_MEMORY_PARTITION_KERNEL); 
    
	result = pspSdkLoadStartModule("flash0:/kd/mpegbase.prx", PSP_MEMORY_PARTITION_KERNEL); 
    
	result = pspSdkLoadStartModule("flash0:/kd/mpeg_vsh.prx", PSP_MEMORY_PARTITION_USER); 
    
	pspSdkFixupImports(result); 
    
	sceMpegInit(); 
   
	//init 
	MP3PlayInit(&ops);
	//load and play
	int res = ops.load("ms0:/Test.mp3");
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
	
	MP3PlayFini();
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
