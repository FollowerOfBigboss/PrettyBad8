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

	DebuggerUi();

	inline void attach(Debugger* dbg) { debugger = dbg; }
	inline void deattach(Debugger* dbg) { debugger = nullptr; }

	void draw();

	void DebuggerStatus();
	void DrawCpuDebugger();
	void GetDisassembly();

	void DrawGraphicsDebugger();
	void DrawStack();
	void DrawKey();


private:

	void UpdateDebuggerTemporaryValues();
	void ApplyChanged();

	void DrawDisassembly();
	void DrawBreakPointList();


	void UpdateTemporaryKey();

	InstructionInfo GetInstructionInfo(int address);

	Debugger* debugger;

	// Temporary variables used for imgui
	int TemporaryV[16];
	int TemporaryPC;
	int TemporaryI;
	int TemporaryST;
	int TemporaryDT;
	int TemporarySP;
	int TemporaryKey[16];

	bool bStopAfterReset = false;
	bool bShowDisassembler = true;
	bool bShowBreakpointList = false;
	bool track_pc = false;
		
	std::vector<InstructionInfo> ininfo;
	bool selected[1792];

	// Identifiers for input boxes
	const char* const templabelsV[16] =
	{
		"##V0", "##V1", "##V2", "##V3", "##V4", "##V5",
		"##V6", "##V7", "##V8", "##V9", "##V10", "##V11",
		"##V12", "##V13", "##V14", "##V15"
	};

	int IMcurritem = 0;
};

#endif

