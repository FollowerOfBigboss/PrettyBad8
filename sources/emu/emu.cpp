#include <iostream>
#include <string>
#include <sstream>
#include <array>

#include "emu.h"
#include <GLFW/glfw3.h>
#include <zlib/zlib.h>

void CostumColorPicker(const std::string& str, ImVec4& color)
{
	int compcounter = 0;
	int size = 0;
	std::string idcounter;

	ImGui::Text(str.c_str());
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.10f);

	size = snprintf(NULL, 0, "##%s_drgflt%i_", str.c_str(), compcounter);
	idcounter.resize(size);
	snprintf(&idcounter[0], idcounter.size(), "##%s_drgflt%i_", str.c_str(), compcounter);
	compcounter++;

	ImGui::DragFloat(idcounter.c_str(), &color.x, 0, 0.0f, 1.0f);
	ImGui::SameLine();

	idcounter.clear();
	size = snprintf(NULL, 0, "##%s_drgflt%i_", str.c_str(), compcounter);
	idcounter.resize(size);
	snprintf(&idcounter[0], idcounter.size(), "##%s_drgflt%i_", str.c_str(), compcounter);
	compcounter++;

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.10f);
	ImGui::DragFloat(idcounter.c_str(), &color.y, 0, 0.0f, 1.0f);
	ImGui::SameLine(0);

	idcounter.clear();
	size = snprintf(NULL, 0, "##%s_drgflt%i_", str.c_str(), compcounter);
	idcounter.resize(size);
	snprintf(&idcounter[0], idcounter.size(), "##%s_drgflt%i_", str.c_str(), compcounter);
	compcounter++;

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.10f);
	ImGui::DragFloat(idcounter.c_str(), &color.z, 0, 0.0f, 1.0f);

	ImGui::SameLine();

	idcounter.clear();
	size = snprintf(NULL, 0, "##%s_btn%i_", str.c_str(), compcounter);
	idcounter.resize(size);
	snprintf(&idcounter[0], idcounter.size(), "##%s_btn%i_", str.c_str(), compcounter);
	compcounter++;

	bool open_popup = ImGui::ColorButton(idcounter.c_str(), color, 0);

	idcounter.clear();
	size = snprintf(NULL, 0, "##%s_pckr%i_", str.c_str(), compcounter);
	idcounter.resize(size);
	snprintf(&idcounter[0], idcounter.size(), "##%s_pckr%i_", str.c_str(), compcounter);
	compcounter++;

	if (open_popup)
	{
		ImGui::OpenPopup(str.c_str());
	}

	if (ImGui::BeginPopup(str.c_str()))
	{
		ImGui::ColorPicker4(idcounter.c_str(), (float*)&color,  ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_Float);
		ImGui::EndPopup();
	}

}

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
			ImGui::Checkbox("Use Compression", &useCompression);


			CostumColorPicker("Clear Color", ClearColor);
			CostumColorPicker("White Color", WhiteColor);

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
			cfg.WriteToConfig("emu.cfg");
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

	useCompression = 0;

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

	WhiteColor.x = 1.0f;
	WhiteColor.y = 1.0f;
	WhiteColor.z = 1.0f;
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
	gquads.update(vm, WhiteColor);
}

void Emu::SaveState()
{
	FILE* fs = fopen("ch8state.st", "wb");
	FileHeader hdr;
	hdr.magic = 0x74733863;

	if (useCompression == true)
	{
		hdr.compression = 1;
		SstateData sd;
		constexpr int chunksize = sizeof(SstateData);
		memcpy(sd.V, vm.V, 16);
		memcpy(sd.Key, vm.Key, 16);
		memcpy(sd.stack, vm.stack, 16 * 2);
		sd.I = vm.I;
		sd.ST = vm.ST;
		sd.DT = vm.DT;
		sd.PC = vm.PC;
		sd.SP = vm.SP;
		memcpy(sd.gfx, vm.gfx, 2048);
		
		uint8_t* chunkout = new uint8_t[chunksize];
		unsigned long destout;

		uint8_t* chunk = new uint8_t[chunksize];

		memcpy(chunk, &sd, chunksize);
		compress(chunkout, &destout, chunk, chunksize);

		uint8_t* newchunk = new uint8_t[destout];
		memcpy(newchunk, chunkout, destout);

		delete[] chunkout;
		delete[] chunk;


		hdr.bufsize = destout;
		fwrite(&hdr, sizeof(FileHeader), 1, fs);
		fwrite(newchunk, 1, destout, fs);
		delete[] newchunk;
		printf("Raw chunk size %i\nCompressed chunk size %i\n", chunksize, destout);
	
	}

	else
	{
		hdr.compression = 0;
		hdr.bufsize = sizeof(SstateData);
		fwrite(&hdr, sizeof(FileHeader), 1, fs);

		SstateData state;
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
	}

	fclose(fs);
}

void Emu::LoadState()
{
//		uint8_t* unc = (uint8_t*)malloc(chunksize);
//		unsigned long dest;
//		uncompress(unc, &dest, chunkout, chunksize);

	FILE* fs = fopen("ch8state.st", "rb");
	FileHeader hdr;
	fread(&hdr, sizeof(FileHeader), 1, fs);

	if (hdr.magic != 0x74733863)
	{
		printf("Magic didn't match\n");
		return;
	}

	if (hdr.compression == 0)
	{
		printf("Sizeof %i\n", hdr.bufsize);
		SstateData sd;
		fread(&sd, sizeof(SstateData), 1, fs);
		for (int i = 0; i < 16; i++)
		{
			vm.V[i] = sd.V[i];
			vm.Key[i] = sd.Key[i];
			vm.stack[i] = sd.stack[i];
		}
		
		vm.I = sd.I;
		vm.ST = sd.ST;
		vm.DT = sd.DT;
		vm.PC = sd.PC;
		vm.SP = sd.SP;
		memcpy(vm.gfx, sd.gfx, sizeof(sd.gfx));

		fclose(fs);
		return;
	}
	
	if (hdr.compression == 1)
	{
		printf("Sizeof %i\n", hdr.bufsize);

		uint8_t* buffer = new uint8_t[hdr.bufsize];
		fread(buffer, 1, hdr.bufsize, fs);

		uint8_t* unc = new uint8_t[sizeof(SstateData)];
		unsigned long dest;
		uncompress(unc, &dest, buffer, hdr.bufsize);

		SstateData sd;
		memcpy(&sd, unc, sizeof(SstateData));
		delete[] unc;
		delete[] buffer;

		for (int i = 0; i < 16; i++)
		{
			vm.V[i] = sd.V[i];
			vm.Key[i] = sd.Key[i];
			vm.stack[i] = sd.stack[i];
		}

		vm.I = sd.I;
		vm.ST = sd.ST;
		vm.DT = sd.DT;
		vm.PC = sd.PC;
		vm.SP = sd.SP;
		memcpy(vm.gfx, sd.gfx, sizeof(sd.gfx));

		fclose(fs);
		return;
	}
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

	cfg.init(*this);
	cfgerr ecfg = cfg.OpenConfig("emu.cfg");

	// Loading emulator config failed most likely due to config is not exist
	if (ecfg == cfgerr::unsuccessful)
	{
		printf("Loading emulator config failed!\n");
		return;
	}

	if (ecfg == cfgerr::successful)
	{
		cfg.ParseConfig();
	}
}





