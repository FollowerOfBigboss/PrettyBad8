project(PrettyBad8)

include_directories(3rdparty)

add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/glm EXCLUDE_FROM_ALL)

# Copy backends

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.cpp 
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.h	  
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_opengl3.cpp 
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui)
	 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_opengl3.h 
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )



# ImGuiFileDialog

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ImGuiFileDialog/ImGuiFileDialog.cpp
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ImGuiFileDialog/ImGuiFileDialog.h
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ImGuiFileDialog/ImGuiFileDialogConfig.h
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ImGuiFileDialog/dirent
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )





add_executable(EmuPBad8 
	       sources/main.cpp
	       sources/vm/debugger.cpp
	       sources/vm/debugger.h
	       sources/vm/draw.h
	       sources/vm/draw.cpp
	       sources/vm/ui.h
	       sources/vm/ui.cpp
	       sources/vm/vm.cpp
	       sources/vm/vm.h

	       sources/vm/debugger_n.h
	       sources/gui/debugger_ui.h
)

add_library(GLAD
	    3rdparty/GLAD/khrplatform.h
	    3rdparty/GLAD/glad.h 
	    3rdparty/GLAD/glad.c
)


add_library(Dear_ImGui
	    3rdparty/imgui/imconfig.h
	    3rdparty/imgui/imgui.cpp
	    3rdparty/imgui/imgui.h
	    3rdparty/imgui/imgui_demo.cpp
	    3rdparty/imgui/imgui_draw.cpp
	    3rdparty/imgui/imgui_impl_glfw.cpp
	    3rdparty/imgui/imgui_impl_glfw.h
	    3rdparty/imgui/imgui_impl_opengl3.cpp
	    3rdparty/imgui/imgui_impl_opengl3.h
	    3rdparty/imgui/imgui_internal.h
	    3rdparty/imgui/imgui_tables.cpp
	    3rdparty/imgui/imgui_widgets.cpp
	    3rdparty/imgui/imstb_rectpack.h
	    3rdparty/imgui/imstb_textedit.h
	    3rdparty/imgui/imstb_truetype.h
			
# ImGuiFileDialog			
	    3rdparty/imgui/ImGuiFileDialog.h
 	    3rdparty/imgui/ImGuiFileDialog.cpp
 	    3rdparty/imgui/dirent/dirent.h			
)


# Replace #include <imgui.h> with #include "imgui.h"

file(READ "3rdparty/imgui/ImGuiFileDialog.h" filedata)
string(REPLACE "#include <imgui.h>"  "#include \"imgui.h\""  filedata "${filedata}")
file(WRITE  "3rdparty/imgui/ImGuiFileDialog.h" "${filedata}")

			

target_link_libraries(Dear_ImGui glfw)
target_link_libraries(EmuPBad8 glm glfw GLAD Dear_ImGui)