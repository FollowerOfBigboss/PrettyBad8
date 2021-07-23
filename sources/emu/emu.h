#include "imgui/imgui.h"
#include "imgui/ImGuiFileDialog.h"

#include "../vm/debugger.h"
#include "../emu/debugger_ui.h"
#include "../render/simple_renderer.h"

#include <array>

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

struct Emu
{
	DebuggerUi gdebugger;
	Debugger debugger;
	VM vm;

	bool ShowCpuDebugger;
	bool ShowGraphicsDebugger;
	bool ShowStackView;
	bool ShowKeyView;
	bool ShouldDrawMenuBar;

	bool DrawFile;
	bool RomLoaded;
	CRenderQuads gquads;

	bool ShowSettings;
	bool Vsync;
	FILE* fs;
	
	int clockspeed;

	int lastpressedkey;
	int kkep;
	bool kch;

	std::array<int, 16> keymap;

	void init();
	void DrawMenuBar();
	void DrawDebuggerStuf();
	void DrawSettingsWindow(bool* open);

	void DrawOtherWindows();

	void DecideDebuggerStatus();

	void EmuLoop();

	void run();


	void presskey(int key);
	void releasekey(int key);

	void SaveState();
	void LoadState();
};