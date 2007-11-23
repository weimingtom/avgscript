/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * main.c - Basic sample to demonstrate the kprintf handler.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: main.c 1095 2005-09-27 21:02:16Z jim $
 */
#include <pspkernel.h>
#include <pspdebug.h>
#include <fstream>
#include <iostream>
#include "Compiler/TomComp.h"
#include "StdBasicLib/TomStdBasicLib.h"
using namespace std;
PSP_MODULE_INFO("AVGScript", 0x1000, 1, 1);
/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(0);

/* Define printf, just to make typing easier */
#define printf	pspDebugScreenPrintf


// Source File
char* srcFile;
char buffer[1024];

//void WrapPrint(TomVM& vm) {cout << vm.GetStringParam(1).c_str();}
//void WrapPrintr(TomVM& vm) {cout << vm.GetStringParam(1).c_str() << endl;}
void WrapPrint(TomVM& vm) {printf("%s",vm.GetStringParam(1).c_str());}
void WrapPrintr(TomVM& vm) {printf("%s\n",vm.GetStringParam(1).c_str());}

void startCompiler () {
	printf("AVG script v0.1 a basic like script\n");
	// Create compiler and virtual machine
	TomVM vm;
	TomBasicCompiler comp(vm);

	// Register function wrappers
	comp.AddFunction("print",  WrapPrint,  compParamTypeList()<<VTP_STRING, false, false, VTP_INT);
	comp.AddFunction("printr", WrapPrintr, compParamTypeList()<<VTP_STRING, false, false, VTP_INT);

	// Register other functions
	InitTomStdBasicLib(comp);
	// Open File
	/*ifstream file(srcFile);

	// Validate file
	if(!file || file.eof()){
		exit(1);
	}

	// Load file source into buffer
	while(!file.eof()){
		file.getline(buffer,1023);
		comp.Parser().SourceCode().push_back(buffer);
	}*/
	comp.Parser().SourceCode().push_back("print \"d=(c*(a+b)/8+9)*10 d=\"");
	comp.Parser().SourceCode().push_back("end");
	// Compile program
	printf("Compiling...\n");
	comp.ClearError();
	comp.Compile();
	if (comp.Error()) {
		printf("COMPILE ERROR!: %s\n",comp.GetError().c_str());
		return;
	}

	// Run program
	printf("Running...\n");
	vm.Reset();
	do {
		//processEvents();
		vm.Continue(1000);
	} while (!(vm.Error() || vm.Done()));

	// Check for virtual machine error
	if(vm.Error()) 
		printf("RUNTIME ERROR!: %s\n", vm.GetError().c_str());
	else 
		printf("Done!\n");
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

	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, THREAD_ATTR_USER, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

extern "C"
int main (int argc, char* argv[]) {
	pspDebugScreenInit();
	SetupCallbacks();
	srcFile ="simple.avgs";
	startCompiler();
	while(1){
		;
	}
	return 0;
};


