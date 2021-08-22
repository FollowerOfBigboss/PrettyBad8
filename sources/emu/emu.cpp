#include "emu.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <array>

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

	pmode.pressed = true;
	pcont.pressed = true;

	clockspeed = 500;
	CurrentInput = EmuInput::Keyboard;
	gquads.init();
	debugger.attach(&vm);
	gdebugger.attach(&debugger);

	keymap[0] = GLFW_KEY_1;
	keymap[1] = GLFW_KEY_2;
	keymap[2] = GLFW_KEY_3;
	keymap[3] = GLFW_KEY_4;
						  
	keymap[4] = GLFW_KEY_Q;
	keymap[5] = GLFW_KEY_W;
	keymap[6] = GLFW_KEY_E;
	keymap[7] = GLFW_KEY_R;
						  
	keymap[8] = GLFW_KEY_A;
	keymap[9] = GLFW_KEY_S;
	keymap[10] = GLFW_KEY_D;
	keymap[11] = GLFW_KEY_F;
						  
	keymap[12] = GLFW_KEY_Z;
	keymap[13] = GLFW_KEY_X;
	keymap[14] = GLFW_KEY_C;
	keymap[15] = GLFW_KEY_V;

	contmap.fill({ -1 , -1});
	loadconfig();
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

				if (ImGui::MenuItem("Exit"))
				{
					// Don't care about deconstruction of emulator exit immediatly
					exit(0);
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
				glfwSwapInterval(static_cast<int>(Vsync));
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
			std::array<std::string, 3> strarr = { "No Input", "Keyboard", "No Controller Connected" };
			if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE)
			{
				strarr[2] = glfwGetJoystickName(GLFW_JOYSTICK_1);
			}
		
			const char* combo_label = strarr[CurrentInput].c_str();

			ImGui::Text("Default Input Device");
			if (ImGui::BeginCombo("##ControllerBox", strarr[CurrentInput].c_str()))
			{
				for (int n = 0; n < strarr.size(); n++)
				{
					const bool is_selected = (CurrentInput == n);
					if (ImGui::Selectable(strarr[n].c_str(), is_selected))
						CurrentInput = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (CurrentInput == EmuInput::Keyboard)
			{
				for (int i = 0; i < 16; i++)
				{
					if (((i) % 4) == 0)
						ImGui::NewLine();
					else
						ImGui::SameLine();
				
					if (pmode.keytochanged == i && pmode.pressed == false)
					{
						if (ImGui::Button("Waiting for keypress"))
						{
							pmode.pressed = true;
						}
					}
					else
					{
					 	char tmp[10] = { 0 };
					 	snprintf(tmp, 10, "%s##%i", glfwGetKeyName(keymap[i], 0), i);
						if (ImGui::Button(tmp))
						{
							pmode.pressed = false;
							pmode.keytochanged = i;
						}
					}
				}
			}

			if (CurrentInput == EmuInput::Controller)
			{

				for (int i = 0; i < 16; i++)
				{
					if (((i) % 4) == 0)
						ImGui::NewLine();
					else
						ImGui::SameLine();

					if (pcont.keytochanged == i && pcont.pressed == false)
					{
						if (ImGui::Button("Waiting for keypress"))
						{
							pcont.pressed = true;
						}
					}
					else
					{
						char tmp[20] = { 0 };
						
						if (contmap[i].key == -1)
						{
							snprintf(tmp, 20, "%s##%i", "Not Mapped!", i);
						}
						else
						{
							snprintf(tmp, 20, "%i##%i", contmap[i].key, i);
						}
						if (ImGui::Button(tmp))
						{
							pcont.pressed = false;
							pcont.keytochanged = i;
						}
					}
				}
			}

			ImGui::EndTabItem();
		}
		else
		{
			pcont.pressed = true;
			pcont.keytochanged = -1;

			pmode.pressed = true;
			pmode.pressed = -1;
		}

		ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x - 110, ImGui::GetContentRegionAvail().y - 40));
		ImGui::Dummy(ImVec2(ImGui::GetWindowWidth() - 130, 0));
		ImGui::SameLine();
		
		if (ImGui::Button("Apply", ImVec2(100, 30)))
		{
			cfg.WriteToConfig();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void Emu::DrawOtherWindows()
{
	if (DrawFile)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", ".");

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
	if (debugger.debugger_status != DebuggerStatus::debugger_not_running && RomLoaded == true)
	{
		debugger.run();
	}
	else if (RomLoaded == true)
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
	FILE* fs = fopen("ch8state.st", "wb");
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
	FILE* fs = fopen("ch8state.st", "rb");
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

void Emu::presskey(int key)
{
	for (int i = 0; i < 16; i++)
	{
		if (key == keymap[i])
		{
			vm.Key[i] = 1;
		}
	}
}

void Emu::releasekey(int key)
{
	for (int i = 0; i < 16; i++)
	{
		if (key == keymap[i])
		{
			vm.Key[i] = 0;
		}
	}
}

void Emu::handlecontroller()
{
	int buttoncount;
	const unsigned char* buttonptr = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttoncount);

	for (int i = 0; i < 16; i++)
	{
		if (buttonptr[i] == 1 && pcont.pressed == false)
		{
			contmap[pcont.keytochanged].key = i;
			contmap[pcont.keytochanged].maploc = pcont.keytochanged;
			pcont.pressed = true;
			break;
		}
	}



	for (int i = 0; i < 16; i++)
	{
		if (buttonptr[i] == 1)
		{
			for (int a = 0; a < 16; a++)
			{
				if (contmap[a].key == i)
				{
					vm.Key[contmap[a].maploc] = 1;
				}
			}
		}
	}
		
}

void Emu::loadconfig()
{

	FILE* fs = fopen("emu.cfg", "rb");
	long fsize;
	fseek(fs, 0, SEEK_END);
	fsize = ftell(fs);
	rewind(fs);
	char* mem = (char*)malloc(fsize+1);
	fread(mem, 1, fsize, fs);
	mem[fsize] = '\0';
	std::string buf = mem;
	buf.erase(std::remove(buf.begin(), buf.end(), '\r'), buf.end());
	fclose(fs);
	free(mem);

	cfg.init(*this);
	cfg.OpenConfig();
	cfg.ParseConfig();
}





