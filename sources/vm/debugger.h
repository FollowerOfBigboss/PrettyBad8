#include <vector>

#include "vm.h"

#include "imgui/imgui.h"

#ifndef EMU_DEBUGGER
#define EMU_DEBUGGER
 
struct InstructionData
{
	std::string address;
	std::string bytes;
	std::string decoded_instrucction;
	bool selected;
};

class DebuggerO
{
public:

// Main functions which responsible for provide connection to virtual machine
	void attach(VM& vmInfo);
	void deattach();

// Operations
	void SingleStep();
	void StepInto();
	void RunUntilBreakpoint();

// Information
	void GetDissassembly();
	void GetRegisterInformations();
	void ApplyChangedInformation();

	void GetKeyInformation();
	void GetStackInformation();

// Draw
	void HandleAndDrawDebuggerInput();
	void DrawRegisters();
	void DrawDissassembly();
	void DrawGraphicsDebugger();
	void DrawStack();
	void DrawKey();
	std::string DecodeInstruction(uint16_t opcode);

private:
	// Debugger status
	bool Pause = false;
	bool BreakpointContinue = false;
	bool BreakPointHit = false;

	// Specifier for reset
	bool StopAfterReset = false;

	// Pointer for our vm
	VM* vm_ptr = nullptr;

	// Instruction data
	std::vector<InstructionData> insData;

	// Temporary variables which keeps last state of debugger
	int TemporaryV[16] = { 0 };
	int TemporaryI = 0;
	int TemporaryST = 0;
	int TemporaryDT = 0;
	int TemporaryPC = 0;
	int TemporarySP = 0;
	int TemporaryStack[16] = { 0 };
	int TemporaryKey[16] = { 0 };

	// ImGui  useless vars
	int item_current_idx = 0;
	const char* templabelsV[16] = 
	{ 
		"##V0", "##V1", "##V2", "##V3", "##V4", "##V5",
		"##V6", "##V7", "##V8", "##V9", "##V10", "##V11",
		"##V12", "##V13", "##V14", "##V15" 
	};

};

#endif
