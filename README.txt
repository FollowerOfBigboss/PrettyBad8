This emulator build by me to learn basics of emulation. At first, my scope for this emulator was huge
but i didn't enough have a time to finish my goals so i dropped the project. I just do maintenance time to time to 
ensure it still compiles and runs. Screenshots can be found in screenshots folder.

I used these resources when i building my emulator:

	http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
	https://en.wikipedia.org/wiki/CHIP-8
	https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/ 
	http://www.komkon.org/~dekogel/vision8.html (Shamelesssly stole DRW instruction code)
	
Emulator have
- Emulation Core
- Renderer
- User Interface
- Gamepad Support
- Config Parser
- Debugger (Enable it in emu.cfg)
- Save States (Quick Load) [Compression is supported for savestates]

Emulator don't have 
- Dynarec (Dynamic Recompiler)
- Sound

Dependencies:
- GLFW (https://github.com/glfw/glfw)
- imgui (https://github.com/ocornut/imgui)
- glm (https://github.com/g-truc/glm)
- ImGuiFileDialog (https://github.com/aiekick/ImGuiFileDialog)
- zlib (https://github.com/madler/zlib)

Building:
	git clone --recurse-submodules https://github.com/FollowerOfBigboss/PrettyBad8
	mkdir build
	cd build
	cmake ..
	Open the solution
