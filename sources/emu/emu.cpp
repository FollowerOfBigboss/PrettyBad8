#include "emu.h"
#include <GLFW/glfw3.h>

void Emu::init()
{
	ShowCpuDebugger = false;
	ShowGraphicsDebugger = false;
	ShowStackView = false;
	ShowKeyView = false;
	ShouldDrawMenuBar = true;
	RomLoaded = false;
	DrawFile = false;
	ShowSettings = false;
	Vsync = true;

	clockspeed = 500;
	
	gquads.init();
	debugger.attach(&vm);
	gdebugger.attach(&debugger);
}

void Emu::DrawMenuBar()
{
	if (ShouldDrawMenuBar)
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Open", 0, &DrawFile);


				if (ImGui::MenuItem("Close", 0))
				{

					if (RomLoaded == true)
					{
						RomLoaded = false;
						vm.reset();
					}

				}

				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::MenuItem("Settings", 0, &ShowSettings);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug"))
			{
				ImGui::MenuItem("CPU Debugger", 0, &ShowCpuDebugger);
				ImGui::MenuItem("Graphics Debugger", 0, &ShowGraphicsDebugger);
				ImGui::MenuItem("Stack View", 0, &ShowStackView);
				ImGui::MenuItem("Key View", 0, &ShowKeyView);
				ImGui::EndMenu();
			}

			gdebugger.DrawDebuggerStatus();
			ImGui::EndMainMenuBar();
		}

	}
}

void Emu::DrawDebuggerStuf()
{
	if (ShowGraphicsDebugger)
	{
		gdebugger.DrawGraphicsDebugger(&ShowGraphicsDebugger);
	}
	
	if (ShowStackView)
	{
		gdebugger.DrawStack(&ShowStackView);
	}
	
	if (ShowKeyView)
	{
		gdebugger.DrawKey(&ShowKeyView);
	}
	
	if (ShowCpuDebugger)
	{
		gdebugger.DrawCpuDebugger(&ShowCpuDebugger);
	}	
}

void Emu::DrawSettingsWindow(bool* open)
{
	ImGui::Begin("Settings", open);
	
	if (ImGui::BeginTabBar("##Tabs"))
	{
		if (ImGui::BeginTabItem("General"))
		{
			if (ImGui::Checkbox("Vsync", &Vsync))
			{
				glfwSwapInterval((int)Vsync);
			}

			ImGui::Text("VM cpu speed");
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.35f);
			ImGui::InputInt("##clockspeed", &clockspeed, 100);
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				clockspeed = 500;
			}


			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Controls"))
		{
			const char* items[] = { "No Controller", "Keyboard", "Controller"};
			static int item_current_idx = 0;
			const char* combo_label = items[item_current_idx];

			ImGui::Text("Default Input Device");
			if (ImGui::BeginCombo("##ControllerBox", items[item_current_idx]))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::EndTabItem();
		}


		ImGui::EndTabBar();
	}

	ImGui::End();
}

void Emu::DrawOtherWindows()
{
	if (DrawFile)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".ch8,.*", ".");

		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

				vm.init();
				vm.loadrom(filePathName);
				RomLoaded = true;
				gdebugger.UpdateDisassembly();
			}

			ImGuiFileDialog::Instance()->Close();
			DrawFile = false;
		}
	}

	if (ShowSettings)
	{
		DrawSettingsWindow(&ShowSettings);
	}
}

void Emu::DecideDebuggerStatus()
{
	if (debugger.debugger_status == DebuggerStatus::debugger_pause || debugger.debugger_status == DebuggerStatus::debugger_breakpoint_hit)
		return;

	if (RomLoaded == true && (ShowCpuDebugger == true || ShowGraphicsDebugger == true || ShowKeyView == true || ShowStackView == true))
		debugger.debugger_status = DebuggerStatus::debugger_running;
	else
		debugger.debugger_status = DebuggerStatus::debugger_not_running;
}

void Emu::EmuLoop()
{
	if (debugger.debugger_status != DebuggerStatus::debugger_not_running)
	{
		debugger.run();
	}
	else
	{
		vm.run(clockspeed);
	}
}

void Emu::run()
{
	DecideDebuggerStatus();
	DrawMenuBar();
	DrawOtherWindows();
	DrawDebuggerStuf();

	EmuLoop();
	gquads.update(vm);
}

void Emu::SaveState()
{
	fs = fopen("ch8state.st", "wb");
	Sstate state;
	state.magic = 0x63387374;

	for (int i = 0; i < 16; i++)
	{
		state.V[i] = vm.V[i];
		state.Key[i] = vm.Key[i];
		state.stack[i] = vm.stack[i];
	}

	state.I = vm.I;
	state.ST = vm.ST;
	state.DT = vm.DT;
	state.PC = vm.PC;
	state.SP = vm.SP;
	memcpy(state.gfx, vm.gfx, sizeof(state.gfx));

	fwrite(&state, sizeof(state), 1, fs);
	fclose(fs);
}

void Emu::LoadState()
{
	Sstate state;
	fs = fopen("ch8state.st", "rb");
	fread(&state, sizeof(state), 1, fs);


	for (int i = 0; i < 16; i++)
	{
		vm.V[i] = state.V[i];
		vm.Key[i] = state.Key[i];
		vm.stack[i] = state.stack[i];
	}

	vm.I = state.I;
	vm.ST = state.ST;
	vm.DT = state.DT;
	vm.PC = state.PC;
	vm.SP = state.SP;
	memcpy(vm.gfx, state.gfx, sizeof(state.gfx));
	fclose(fs);
}

