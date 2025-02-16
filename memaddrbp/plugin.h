#pragma once

#include "pluginmain.h"

//plugin data
#define PLUGIN_NAME "StackContains"
#define PLUGIN_VERSION 1
class saveaddres
{
public:
	duint addres;
	duint  range;


};

//functions
bool pluginInit(PLUG_INITSTRUCT* initStruct);
bool pluginStop();
void pluginSetup();
void CB_Breakpoint(CBTYPE cbType, void* callbackInfo);
static saveaddres saveaddresa[128] ;