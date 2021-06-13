project(PrettyBad8)

include_directories(3rdparty)

add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/glm EXCLUDE_FROM_ALL)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.cpp 
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.h	  
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_opengl3.cpp 
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui)
	 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_opengl3.h 
	 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui )

add_executable(EmuPBad8 
			   sources/main.cpp
			   sources/vm/debugger.cpp
			   sources/vm/debugger.h
			   sources/vm/draw.h
			   sources/vm/ui.h
			   sources/vm/ui.cpp
			   sources/vm/vm.cpp
			   sources/vm/vm.h
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
)

target_link_libraries(Dear_ImGui glfw)
target_link_libraries(EmuPBad8 glm glfw GLAD Dear_ImGui)