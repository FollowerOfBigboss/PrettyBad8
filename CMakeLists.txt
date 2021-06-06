project(PrettyBad8)

include_directories(includes)

add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/glm EXCLUDE_FROM_ALL)

add_executable(EmuPBad8 
			   sources/main.cpp
			   sources/vm/debugger.cpp
			   sources/vm/debugger.h
			   sources/vm/draw.h
			   sources/vm/ui.h
			   sources/vm/vm.cpp
			   sources/vm/vm.h
			   )

add_library(glad 
			includes/GLAD/khrplatform.h
			includes/GLAD/glad.h 
			includes/GLAD/glad.c
			)


add_library(Dear_ImGui
			includes/imgui/imconfig.h
			includes/imgui/imgui.cpp
			includes/imgui/imgui.h
			includes/imgui/imgui_demo.cpp
			includes/imgui/imgui_draw.cpp
			includes/imgui/imgui_impl_glfw.cpp
			includes/imgui/imgui_impl_glfw.h
			includes/imgui/imgui_impl_opengl3.cpp
			includes/imgui/imgui_impl_opengl3.h
			includes/imgui/imgui_internal.h
			includes/imgui/imgui_tables.cpp
			includes/imgui/imgui_widgets.cpp
			includes/imgui/imstb_rectpack.h
			includes/imgui/imstb_textedit.h
			includes/imgui/imstb_truetype.h
)

target_link_libraries(Dear_ImGui glfw)
target_link_libraries(EmuPBad8 glm glfw glad Dear_ImGui)