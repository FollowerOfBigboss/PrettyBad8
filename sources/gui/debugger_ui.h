#ifndef DBGUI
#define DBGUI

#include "../vm/debugger_n.h"
#include "imgui/imgui.h"

struct InstructionInfo
{
	std::string bytes;
	std::string instruction;
};

class DebuggerUi
{
public:

	void attach(Debugger* dbg);
	void deattach(Debugger* dbg);


	void draw();


	void DebuggerStatus();
	void DrawCpuDebugger();


private:

	void sync();
	void ApplyChanged();

	void DrawDisassembly();
	void DrawBreakPointList();

	InstructionInfo GetInstructionInfo(int address);

	bool selected[1792];

	Debugger* debugger;
	int current_item = 0;

	int TemporaryV[16];
	int TemporaryPC;

	int TemporaryI;
	int TemporaryST;
	int TemporaryDT;
	int TemporarySP;

	bool bStopAfterReset = false;
	
	bool track_pc = false;
	
	bool bShowDisassembler = true;
	bool bShowBreakpointList = false;
	
	const char* templabelsV[16] =
	{
		"##V0", "##V1", "##V2", "##V3", "##V4", "##V5",
		"##V6", "##V7", "##V8", "##V9", "##V10", "##V11",
		"##V12", "##V13", "##V14", "##V15"
	};

};

#endif

