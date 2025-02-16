#include "plugin.h"
#include <strsafe.h>
#include <stdio.h>
#define MAX_SCAN_SIZE 0x1000
static int count = 0;
static bool use = false;
class asmattribute

{
public:
	bool read;
	bool write;
	char register_[32];


};
asmattribute parse_asm_operands(const char* instruction) {
	asmattribute attr;


	const char* operands = strchr(instruction, ' ');
	if (!operands) {
		return attr;
	}

	
	while (*operands == ' ') {
		operands++;
	}

	
	if (strchr(operands, '[') != NULL) {
		attr.read = false;

		const char* start = strchr(operands, '[');
		const char* end = strchr(start, ']');
		if (start && end) {
			int len = end - start - 1;
			if (len < sizeof(attr.register_) - 1) {
				strncpy(attr.register_, start + 1, len);
				attr.register_[len] = '\0';
			}
		}
	}


	const char* comma = strchr(operands, ',');
	if (comma && strchr(comma, '[') != NULL) {
		attr.write = false;
		attr.read = true;  

		
		const char* start = strchr(comma, '[');
		const char* end = strchr(start, ']');
		if (start && end) {
			int len = end - start - 1;
			if (len < sizeof(attr.register_) - 1) {
				strncpy(attr.register_, start + 1, len);
				attr.register_[len] = '\0';
			}
		}
	}
	else if (comma) {
		attr.write = true;
		attr.read = false;
	}

	return attr;
}
void CB_Breakpoint(CBTYPE cbType, void* callbackInfo)
{


	BPXTYPE type = bp_hardware;
	PLUG_CB_BREAKPOINT* bpInfo = (PLUG_CB_BREAKPOINT*)callbackInfo;
	if (bpInfo->breakpoint->type == type&& use==true)
	{
		duint eip = DbgValFromString("dis.prev(cip)");




		char disassembly[128];
		GuiGetDisassembly(eip, disassembly);

		asmattribute asmattribute = parse_asm_operands(disassembly);
		duint addres = DbgValFromString(asmattribute.register_);

		

		if (addres == 0)
		{
			return;
		}

		for (int i = 0; i < 128; i++)
		{
			if (saveaddresa[i].addres==0)
			{
				continue;
			}
			if (addres >= saveaddresa[i].addres && addres <= saveaddresa[i].addres + saveaddresa[i].range)
			{
				return;
			}
		}





		DbgCmdExec("r");
	}
}

HRESULT addrbp_func(PSTR Args, PSTR Args2)
{

	
	HRESULT hRes = S_OK;
	char* context = NULL;
	char* context2 = NULL;
	char* param = NULL;
	char* param2 = NULL;
	ULONG_PTR modbase = NULL;
	char* endptr;
	const unsigned char* abc = 0;
	// strip command and trailing whitespaces
	strtok_s(Args, " ", &param);
	strtok_s(param, " ", &context);
	strtok_s(context, " ", &context2);
	duint eip = DbgValFromString("cip");
	saveaddresa[count].addres = strtol(param, &endptr, 16);
	if (strcmp(context, " ") == 0)
	{
		saveaddresa[count].range = 0;
	}
	else{
		saveaddresa[count].range = atoi(context);
	}
	


	use = true;
	_plugin_logprintf("%d", saveaddresa[count].range);
	count++;
	return 1;
}

static bool cbaddrbp(int argc, char* argv[])
{
	_plugin_logputs("memaddrbp  execute");




	addrbp_func((PSTR)argv[0], (PSTR)argv[1]);
	return true;
}

static bool clear(int argc, char* argv[])
{

	memset(saveaddresa, 0, sizeof(saveaddres) * 128);
	_plugin_logputs("clear");




	
	return true;
}

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
    if (!_plugin_registercommand(pluginHandle, "memaddrbp", cbaddrbp, true))
        _plugin_logputs("Plugin loading failed");

	if (!_plugin_registercommand(pluginHandle, "clear", clear, true))
		_plugin_logputs("Plugin loading failed");
    
    return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here (clearing menus optional).
bool pluginStop()
{
    _plugin_unregistercommand(pluginHandle, "memaddrbp");
	_plugin_unregistercommand(pluginHandle, "clear");
    return true;
}

//Do GUI/Menu related things here.
void pluginSetup()
{
}
