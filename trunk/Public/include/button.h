//********************************************
//  project: avgscript 
//  filename: button.h
//  author:   sgch1982@gmail.com 
//  date:     200712
//  descript: common button class 
//  history: v1.0
//********************************************
#ifndef _BUTTON_H_
#define _BUTTON_H_
#include "CommonWnd.h"

class Cbutton :public CCommonWnd
{
public:
	Cbutton();
	~Cbutton();

	virtual void Draw();
private:
	
};


#endif