#ifndef EMU
#define EMU

#include "imgui/imgui.h"
#include "imgui/ImGuiFileDialog.h"

#include "../vm/debugger.h"
#include "../emu/debugger_ui.h"
#include "../render/simple_renderer.h"

#include "../config_parser/config_parser.h"

#include <array>

enum EmuInput : int
{
	NoInput = 0,
	Keyboard,
	Controller
};


struct controlmap
{
	int key;
	int maploc;
};

struct PressMode
{
	int keytochanged;
	bool pressed;
};

struct Sstate
{
	uint32_t magic;
	// uint8_t compression; // Reserved for future use
	uint8_t V[16];
	uint16_t I;
	uint8_t ST;
	uint8_t DT;
	uint16_t PC;
	uint8_t SP;
	uint8_t Key[16];
	uint16_t stack[16];
	uint8_t gfx[64 * 32];
};

class Emu
{
public:
	// Indicates emulator is running or not
	bool b_EmuRun;


	bool b_Debug;

	// VM class
	VM vm;

	// Indicates current input method
	int CurrentInput;
	
	// Indicates state of vsync
	bool b_Vsync;

	// Another ImGui variable for MenuBar
	bool b_ShouldDrawMenuBar;
	
	// Representative variable for vm clock speed
	int clockspeed;

	std::array<int, 16> keymap;
	PressMode pmode;

	// This array contains structers of controlmap which is used for input
	std::array<controlmap, 16> contmap;


	void init();
	void run();
	
	// Wrapper methods for keyboard and controller input
	void presskey(int key);
	void releasekey(int key);
	void handlecontroller();

	// Savestate methods
	void SaveState();
	void LoadState();

private:

	void InitDefaultValues();

	// Emulator loop
	void EmuLoop();

	// ImGui methods for drawing gui
	void DrawMenuBar();
	void DrawDebuggerStuf();
	void DrawSettingsWindow(bool* open);
	void DrawOtherWindows();
	
	// Absurd method
	void DecideDebuggerStatus();

	// Renderer class
	CRenderQuads gquads;

	// Config class and method
	Config cfg;
	void loadconfig();

	// This struct used for input mapping in controllers
	PressMode pcont;
	
	// Debugger classes
	DebuggerUi gdebugger;
	Debugger debugger;


	// ImGui variables
	bool b_ShowCpuDebugger;
	bool b_ShowGraphicsDebugger;
	bool b_ShowStackView;
	bool b_ShowKeyView;
	bool b_DrawFile;
	bool b_ShowSettings;

	// Indicates if rom loaded or not
	bool b_RomLoaded;
};

#endif