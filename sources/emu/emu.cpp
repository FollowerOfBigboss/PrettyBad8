#include <iostream>
#include <string>
#include <sstream>
#include <array>

#include "emu.h"
#include <GLFW/glfw3.h>

void Emu::init()
{
	InitDefaultValues();
	loadconfig();
}

void Emu::DrawMenuBar()
{
	if (b_ShouldDrawMenuBar)
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Open", 0, &b_DrawFile);

				if (ImGui::MenuItem("Close", 0))
				{

					if (b_RomLoaded == true)
					{
						b_RomLoaded = false;
						vm.reset();
					}

				}

				if (ImGui::MenuItem("Exit"))
				{
					b_EmuRun = false;
				}

				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::MenuItem("Settings", 0, &b_ShowSettings);
				ImGui::EndMenu();
			}

			if (b_Debug == true)
			{
				if (ImGui::BeginMenu("Debug"))
				{
					ImGui::MenuItem("CPU Debugger", 0, &b_ShowCpuDebugger);
					ImGui::MenuItem("Graphics Debugger", 0, &b_ShowGraphicsDebugger);
					ImGui::MenuItem("Stack View", 0, &b_ShowStackView);
					ImGui::MenuItem("Key View", 0, &b_ShowKeyView);
					ImGui::EndMenu();
				}

				gdebugger.DrawDebuggerStatus();
			}
			ImGui::EndMainMenuBar();
		}

	}
}

void Emu::DrawDebuggerStuf()
{
	if (b_ShowGraphicsDebugger)
	{
		gdebugger.DrawGraphicsDebugger(&b_ShowGraphicsDebugger);
	}
	
	if (b_ShowStackView)
	{
		gdebugger.DrawStack(&b_ShowStackView);
	}
	
	if (b_ShowKeyView)
	{
		gdebugger.DrawKey(&b_ShowKeyView);
	}
	
	if (b_ShowCpuDebugger)
	{
		gdebugger.DrawCpuDebugger(&b_ShowCpuDebugger);
	}	
}

void Emu::DrawSettingsWindow(bool* open)
{
	ImGui::Begin("Settings", open);
	
	if (ImGui::BeginTabBar("##Tabs"))
	{
		if (ImGui::BeginTabItem("General"))
		{
			if (ImGui::Checkbox("Vsync", &b_Vsync))
			{
				glfwSwapInterval(static_cast<int>(b_Vsync));
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

			ImGui::Checkbox("Show FPS", &b_ShowFPS);
#	ifdef PDEBUG
			ImGui::Checkbox("Debug", &b_Debug);
#	endif

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Controls"))
		{
			ImGui::Text("Default Input Device");
		
			std::string str;
			int joystickconnected = glfwJoystickPresent(GLFW_JOYSTICK_1);
			const char* joystickname = nullptr;

			if (joystickconnected == GLFW_TRUE) {
				joystickname = glfwGetJoystickName(GLFW_JOYSTICK_1);
			}

			switch (CurrentInput)
			{
				case EmuInput::NoInput:	str.assign("No Input");	 break;
				case EmuInput::Keyboard: str.assign("Keyboard"); break;
				case EmuInput::Controller: if (joystickconnected == GLFW_TRUE) str.assign(joystickname); break;
			}


			if (ImGui::BeginCombo("##ControllerBox", str.c_str()))
			{
				if (ImGui::Selectable("No Input"))
				{
					CurrentInput = EmuInput::NoInput;
					
				}

				if (ImGui::Selectable("Keyboard"))
				{
					CurrentInput = EmuInput::Keyboard;
				}

				if (joystickconnected == GLFW_TRUE)
				{
					if (ImGui::Selectable(joystickname))
					{
						CurrentInput = EmuInput::Controller;
					}
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
					 	char tmp[20] = { 0 };
					 	snprintf(tmp, 20, "%s##%i", glfwGetKeyName(keymap[i], 0), i);
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
			pmode.keytochanged = -1;
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
	if (b_DrawFile)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", ".");

		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

				vm.init();
				vm.loadrom(filePathName);
				b_RomLoaded = true;
				gdebugger.UpdateDisassembly();
			}

			ImGuiFileDialog::Instance()->Close();
			b_DrawFile = false;
		}
	}

	if (b_ShowSettings)
	{
		DrawSettingsWindow(&b_ShowSettings);
	}
}

void Emu::DecideDebuggerStatus()
{
	if (debugger.debugger_status == DebuggerStatus::debugger_pause || debugger.debugger_status == DebuggerStatus::debugger_breakpoint_hit)
		return;

	if (b_RomLoaded == true && (b_ShowCpuDebugger == true || b_ShowGraphicsDebugger == true || b_ShowKeyView == true || b_ShowStackView == true))
		debugger.debugger_status = DebuggerStatus::debugger_running;
	else
		debugger.debugger_status = DebuggerStatus::debugger_not_running;
}

void Emu::InitDefaultValues()
{
	b_ShowCpuDebugger = false;
	b_ShowGraphicsDebugger = false;
	b_ShowStackView = false;
	b_ShowKeyView = false;
	b_ShouldDrawMenuBar = true;
	b_RomLoaded = false;
	b_DrawFile = false;
	b_ShowSettings = false;
	b_Vsync = true;
	b_ShowFPS = false;

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

	contmap.fill({ -1 , -1 });

	b_EmuRun = true;
	b_Debug = false;

	pcont.keytochanged = -1;
	pcont.pressed = true;

	pmode.keytochanged = -1;
	pmode.pressed = true;
}

void Emu::EmuLoop()
{
	if (debugger.debugger_status != DebuggerStatus::debugger_not_running && b_RomLoaded == true && b_Debug == true)
	{
		debugger.run(clockspeed);
	}
	else if (b_RomLoaded == true)
	{
		vm.run(clockspeed);
	}
}

void Emu::run()
{
	if (b_Debug == true)
	{
		DecideDebuggerStatus();
	}
	DrawMenuBar();

	DrawOtherWindows();

	if (b_Debug == true)
	{
		DrawDebuggerStuf();
	}

	EmuLoop();
	gquads.update(vm);
}

void Emu::SaveState()
{
	FILE* fs = fopen("ch8state.st", "wb");
	Sstate state;
	state.magic = 0x74733863;

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
	
	if (fs == nullptr)
	{
		printf("File load failed!\n");
		return;
	}

	if (fread(&state, sizeof(state), 1, fs) != 1)
	{
		printf("File read failed!\n");
		return;
	}


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
			vm.Key[i] = static_cast<uint8_t>(1);
		}
	}
}

void Emu::releasekey(int key)
{
	for (int i = 0; i < 16; i++)
	{
		if (key == keymap[i])
		{
			vm.Key[i] = static_cast<uint8_t>(0);
		}
	}
}

void Emu::handlecontroller()
{
	memset(vm.Key, 0, sizeof(uint8_t) * 16);

	int buttoncount;
	const unsigned char* buttonptr = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttoncount);

	for (int i = 0; i < buttoncount; i++)
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
			for (int a = 0; a < buttoncount; a++)
			{
				if (contmap[a].key == i)
				{
					vm.Key[contmap[a].maploc] = static_cast<uint8_t>(1);
				}
			}
		}
	}
		
}

void Emu::loadconfig()
{

	FILE* fs = fopen("emu.cfg", "rb");

	if (fs == nullptr)
	{
		printf("File load failed!\n");
		return;
	}

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





