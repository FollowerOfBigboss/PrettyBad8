#include "imgui/imgui.h"
#include "imgui/ImGuiFileDialog.h"

#include "../vm/debugger.h"
#include "../gui/debugger_ui.h"
#include "../render/simple_renderer.h"

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

	void Init();
	void DrawMenuBar();
	void DrawDebuggerStuf();
	void DrawSettingsWindow(bool* open);

	void DrawOtherWindows();

	void EmuLoop();
	void EmuDraw();

	void SaveState();
	void LoadState();
};