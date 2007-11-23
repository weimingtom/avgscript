#include <fstream>
#include <iostream>
#include "../../../Include/Compiler/TomComp.h"
#include "../../../Include/StdBasicLib/TomStdBasicLib.h"

using namespace std;

// Source File
char* srcFile;
char buffer[1024];

void WrapPrint(TomVM& vm) {cout << vm.GetStringParam(1).c_str();}
void WrapPrintr(TomVM& vm) {cout << vm.GetStringParam(1).c_str() << endl;}

void Myfuction_printv(TomVM& vm)
{
	printf("add one function param is %s\n",vm.GetStringParam(1).c_str());
}

void startCompiler () {
	cout << "AVG script v0.1 a basic like script" << endl;
	cout << endl;

	// Create compiler and virtual machine
	TomVM vm;
	TomBasicCompiler comp(vm);

	// Register function wrappers
	comp.AddFunction("print",  WrapPrint,  compParamTypeList()<<VTP_STRING, false, false, VTP_INT);
	comp.AddFunction("printr", WrapPrintr, compParamTypeList()<<VTP_STRING, false, false, VTP_INT);
	comp.AddFunction("printv", Myfuction_printv, compParamTypeList()<<VTP_STRING, false, false, VTP_INT);
	// Register other functions
	InitTomStdBasicLib(comp);
	// Open File
	cout<<"Opening file "<<srcFile<<"..."<<endl;
	ifstream file(srcFile);

	// Validate file
	if(!file || file.eof()){
		cout << "File does not exit!" << endl;
		exit(1);
	}

	// Load file source into buffer
	while(!file.eof()){
		file.getline(buffer,1023);
		comp.Parser().SourceCode().push_back(buffer);
	}

	// Compile program
	cout<<"Compiling..."<<endl;
	comp.ClearError();
	comp.Compile();
	if (comp.Error()) {
		cout << endl << "COMPILE ERROR!: " << comp.GetError().c_str() << endl;
		return;
	}

	// Run program
	cout<<"Running..."<<endl;
	vm.Reset();
	do {
		//processEvents();
		vm.Continue(1000);
	} while (!(vm.Error() || vm.Done()));

	// Check for virtual machine error
	if(vm.Error()) cout << endl << "RUNTIME ERROR!: " << vm.GetError().c_str() << endl;
	else cout << endl << "Done!" << endl;
}

int main (int argc, char* argv[]) {
	// Set srcFile & catch argument errors
	if(argc==2) {srcFile = argv[1];}
	else {cout << "Incorrect number of arguments!" << endl; return 0;}
	// InitSDL(Width, Height, Title)
	startCompiler();
	getchar();
	return 0;
};