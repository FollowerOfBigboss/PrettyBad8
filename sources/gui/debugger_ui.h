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

	void draw()
	{
		ImGui::Begin("New Debugger");

		ImGui::InputInt("##addr", &tmp, 0);	
		if (ImGui::Button("Add"))
		{
			debugger->AddBreakpoint(tmp);
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove"))
		{
			debugger->RemoveBreakpoint(tmp);
		}
		if (ImGui::Button("Continue"))
		{
			if (debugger->debugger_status == DebuggerStatus::debugger_breakpoint_hit)
			{
				debugger->debugger_status = DebuggerStatus::debugger_run_after_breakpoint_hit;
			}
			else
			{
				debugger->debugger_status = DebuggerStatus::debugger_running;
			}

		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			debugger->debugger_status = DebuggerStatus::debugger_pause;
		}
		ImGui::End();

		DebuggerStatus();
		BreakPointList();
	}


	void DebuggerStatus()
	{
		int status = debugger->get_status();
		std::string str_status = debugger->get_status_str();
		ImVec4 ColorVec;

		switch (status)
		{

			case DebuggerStatus::debugger_running:
				ColorVec =ImVec4(0.678, 1.000, 0.184, 1);  
				break;
			
			case DebuggerStatus::debugger_pause: 
				ColorVec = ImVec4(1.000, 1.000, 1.000, 1); 
				break;
			
			case DebuggerStatus::debugger_breakpoint_hit: 
				ColorVec = ImVec4(1.000, 0.000, 0.000, 1); 
				break;
			
			case DebuggerStatus::debugger_not_active: 
				ColorVec = ImVec4(1.000, 1.000, 0.000, 1); 
				break;
		}

		ImGui::TextColored(ColorVec, "Debugger Status: %s", str_status.c_str());
	}

	void BreakPointList()
	{
		ImGui::Begin("BreakPoint List");

		if (ImGui::BeginListBox("##BreakpointList"))
		{
			for (int n = 0; n < debugger->BreakpointList.size(); n++)
			{
				const bool is_selected = (current_item == n);
				std::string t = std::to_string(debugger->BreakpointList[n]);
				if (ImGui::Selectable(t.c_str(), is_selected))
				{
					current_item = n;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}

		ImGui::End();
	}

private:
	Debugger* debugger;
	int current_item = 0;
};

#endif // !DBGUI

