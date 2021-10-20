#ifndef EMU
#define EMU

#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/ImGuiFileDialog.h>

#include "../vm/debugger.h"
#include "../emu/debugger_ui.h"
#include "../render/simple_renderer.h"

#include "../config_parser/config_parser.h"

// https://stackoverflow.com/questions/1537964/visual-c-equivalent-of-gccs-attribute-packed
#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif


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

PACK
(
struct SstateData
{
	uint8_t V[16]; // 16 bytes
	uint16_t I; // 2 bytes
	uint8_t ST; // 1 byte
	uint8_t DT; // 1 byte
	uint16_t PC; // 2 bytes
	uint8_t SP; // 1 byte
	uint8_t Key[16]; // 16 bytes
	uint16_t stack[16]; // 32 bytes
	uint8_t gfx[64 * 32]; // 2048 bytes
}
);

PACK
(
struct FileHeader
{
	uint32_t magic;
	uint8_t compression;
	uint32_t bufsize;
}
);


class Emu
{
public:
	// Indicates emulator is running or not
	bool b_EmuRun;

	// Debug indicator
	bool b_Debug;

	// Indicates current input method
	int CurrentInput;
	
	// Indicates state of vsync
	bool b_Vsync;

	// Another ImGui variable for MenuBar
	bool b_ShouldDrawMenuBar;

	// VM class
	VM vm;

	bool b_ShowFPS;
	// Representative variable for vm clock speed
	int clockspeed;

	std::array<int, 16> keymap;
	PressMode pmode;

	// This array contains structers of controlmap which is used for input
	std::array<controlmap, 16> contmap;

	// This struct used for input mapping in controllers
	PressMode pcont;

	// compression flag
	bool useCompression;

	ImVec4 ClearColor;
	ImVec4 WhiteColor;

	void init();
	void run();
	
	// Wrapper methods for keyboard and controller input
	void presskey(int key);
	void releasekey(int key);
	void handlecontroller();

	// Savestate methods
	void SaveState();
	void LoadState();


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


private:

};

#endif