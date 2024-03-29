cmake_minimum_required(VERSION 3.20)
project(PrettyBad8)

if (debug)
	add_compile_definitions(PDEBUG)
endif()

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

	       sources/render/simple_renderer.h
	       sources/render/simple_renderer.cpp

	       sources/emu/emu.h
	       sources/emu/emu.cpp

	       sources/emu/debugger_ui.h
	       sources/emu/debugger_ui.cpp

	       sources/vm/vm.h
	       sources/vm/vm.cpp

	       sources/vm/debugger.h
	       sources/vm/debugger.cpp
		   
	       sources/config_parser/config_parser.h
	       sources/config_parser/config_parser.cpp
	
	       sources/dynarec/dynarec.h
	       sources/dynarec/dynarec.cpp
)

add_library(GLAD
	    3rdparty/glad/khrplatform.h
	    3rdparty/glad/glad.h 
	    3rdparty/glad/glad.c
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

target_compile_definitions(Dear_ImGui PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(Dear_ImGui PRIVATE 3rdparty)

add_library(zlib
	    3rdparty/zlib/crc32.h
	    3rdparty/zlib/deflate.h
	    3rdparty/zlib/gzguts.h
	    3rdparty/zlib/inffast.h
	    3rdparty/zlib/inffixed.h
	    3rdparty/zlib/inflate.h
	    3rdparty/zlib/inftrees.h
	    3rdparty/zlib/trees.h
	    3rdparty/zlib/zconf.h
	    3rdparty/zlib/zlib.h
	    3rdparty/zlib/zutil.h
	    3rdparty/zlib/adler32.c
	    3rdparty/zlib/compress.c
	    3rdparty/zlib/crc32.c
	    3rdparty/zlib/deflate.c
	    3rdparty/zlib/gzclose.c
	    3rdparty/zlib/gzlib.c
	    3rdparty/zlib/gzread.c
	    3rdparty/zlib/gzwrite.c
	    3rdparty/zlib/infback.c
	    3rdparty/zlib/inffast.c
	    3rdparty/zlib/inflate.c
	    3rdparty/zlib/inftrees.c
	    3rdparty/zlib/trees.c
	    3rdparty/zlib/uncompr.c
	    3rdparty/zlib/zutil.c
)

# Replace #include <imgui.h> with #include "imgui.h"

file(READ "3rdparty/imgui/ImGuiFileDialog.h" filedata)
string(REPLACE "#include <imgui.h>"  "#include \"imgui.h\""  filedata "${filedata}")
file(WRITE  "3rdparty/imgui/ImGuiFileDialog.h" "${filedata}")

			

target_link_libraries(Dear_ImGui glfw)
target_link_libraries(EmuPBad8 glm glfw GLAD Dear_ImGui zlib)
