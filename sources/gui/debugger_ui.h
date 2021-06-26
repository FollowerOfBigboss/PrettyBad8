#ifndef DBGUI
#define DBGUI

#include "../vm/debugger_n.h"
#include "imgui/imgui.h"


class DebuggerUi
{
public:
	int tmp;
	void attach(Debugger* dbg)
	{
		debugger = dbg;
	}

	void deattach(Debugger* dbg)
	{
		debugger = nullptr;
	}

	void sync()
	{

		for (int i = 0; i < 16; i++)
		{
			TV[i] = debugger->get_value_of_register(i);
		}
		
		TPC = debugger->get_value_of_register(Registers::PC);
		TemporaryI = debugger->get_value_of_register(Registers::I);
		TemporaryST = debugger->get_value_of_register(Registers::ST);
		TemporaryDT = debugger->get_value_of_register(Registers::DT);
		TemporarySP = debugger->get_value_of_register(Registers::SP);

	}

	void draw()
	{
		sync();
		DebuggerStatus();
		BreakPointList();
		DrawCpuDebugger();
	}


	void DebuggerStatus()
	{
		int status = debugger->get_status();
		std::string str_status = debugger->get_status_str();
		ImVec4 ColorVec;

		switch (status)
		{

			case DebuggerStatus::debugger_running:
				ColorVec = ImVec4(0.678f, 1.000f, 0.184f, 1.0f);  
				break;
			
			case DebuggerStatus::debugger_pause: 
				ColorVec = ImVec4(1.000f, 1.000f, 1.000f, 1.0f); 
				break;
			
			case DebuggerStatus::debugger_breakpoint_hit: 
				ColorVec = ImVec4(1.000f, 0.000f, 0.000f, 1.0f); 
				break;
			
			case DebuggerStatus::debugger_not_running: 
				ColorVec = ImVec4(1.000f, 1.000f, 0.000f, 1.0f); 
				break;
		}

		ImGui::TextColored(ColorVec, "Debugger Status: %s", str_status.c_str());
	}

	void BreakPointList()
	{
		ImGui::Begin("BreakPoint List");
		if ( ImGui::BeginListBox("##BreakpointList", ImVec2(-FLT_MIN, -FLT_MIN)) )
		{
			for (int n = 0; n < debugger->BreakpointList.size(); n++)
			{
				const bool is_selected = (current_item == n);
				std::string t = std::to_string(debugger->BreakpointList[n]);
				if (ImGui::Selectable(t.c_str(), is_selected))
				{
					current_item = n;
				}

				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}

		ImGui::End();
	}

	void DrawCpuDebugger()
	{
		ImGui::Begin("CPU Debugger", 0, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
		
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::MenuItem("Stop After Reset", 0, &bStopAfterReset);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				ImGui::MenuItem("Show Disassembler", 0, &bShowDisassembler);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 205, 50, 255));
		if (ImGui::Button("Resume"))
		{
			debugger->set_status(DebuggerStatus::debugger_running);
		}

		ImGui::PopStyleColor();

		ImGui::SameLine();
		
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 255));
		if (ImGui::Button("Pause"))
		{
			debugger->set_status(DebuggerStatus::debugger_pause);
		}

		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::Button("Reset");



		ImGui::SameLine();

		if (ImGui::Button("Single Step"))
		{
			if (debugger->debugger_status == DebuggerStatus::debugger_running)
			{
				debugger->set_status(DebuggerStatus::debugger_pause);
			}


			if (debugger->debugger_status != DebuggerStatus::debugger_running)
			{
				debugger->SingleStep();
			}
		}


		ImGui::SameLine();

		if (ImGui::Button("Step Into"))
		{
			if (debugger->debugger_status == DebuggerStatus::debugger_running)
			{
				debugger->set_status(DebuggerStatus::debugger_pause);
			}

			if (debugger->debugger_status != DebuggerStatus::debugger_running)
			{
				debugger->StepInto();
			}
		}

		for (int i = 0; i < 16; i++)
		{
			ImGui::Text("V%i", i);
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
			ImGui::InputInt(templabelsV[i], &TV[i], 0);

			if (!((i + 1) % 4 == 0))
			{
				ImGui::SameLine();
			}

		}

		ImGui::Text("PC");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
		ImGui::InputInt("##TemporaryPC", &TPC, 0);

		ImGui::SameLine();

		ImGui::Text("I");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
		ImGui::InputInt("##TemporaryI", &TemporaryI, 0);

		ImGui::SameLine();

		ImGui::Text("ST");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
		ImGui::InputInt("##TemporaryST", &TemporaryST, 0);

		ImGui::SameLine();

		ImGui::Text("DT");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
		ImGui::InputInt("##TemporaryDT", &TemporaryDT, 0);

		ImGui::Text("SP");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
		ImGui::InputInt("##TemporarySP", &TemporarySP, 0);

		ImGui::End();
	}

private:
	Debugger* debugger;
	int current_item = 0;

	int TV[16];
	int TPC;

	int TemporaryI;
	int TemporaryST;
	int TemporaryDT;
	int TemporarySP;

	bool bStopAfterReset = false;


	bool bShowDisassembler = true;

	const char* templabelsV[16] =
	{
		"##V0", "##V1", "##V2", "##V3", "##V4", "##V5",
		"##V6", "##V7", "##V8", "##V9", "##V10", "##V11",
		"##V12", "##V13", "##V14", "##V15"
	};

};

#endif

