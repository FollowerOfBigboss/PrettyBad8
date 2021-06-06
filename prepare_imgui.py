import os
import shutil

working_dir = os.getcwd()
SrcTemporary = ""
DstTemporary = ""



if (os.path.exists(os.path.join(working_dir, "includes")) == False):
    os.makedirs(os.path.join(working_dir, "includes/imgui"))
    os.makedirs(os.path.join(working_dir, "includes/GLAD"))

    
CopyBackends = [
                os.path.join(working_dir, "3rdparty/imgui/" + "backends/imgui_impl_opengl3.cpp"),
                os.path.join(working_dir, "3rdparty/imgui/" + "backends/imgui_impl_opengl3.h"),
                os.path.join(working_dir, "3rdparty/imgui/" + "backends/imgui_impl_glfw.h"),
                os.path.join(working_dir, "3rdparty/imgui/" + "backends/imgui_impl_glfw.cpp")
               ]

for b in CopyBackends:
    BackendsDst = os.path.join(working_dir, "3rdparty/imgui/", b.split("/")[-1])
    shutil.copyfile(b, BackendsDst)
    

for i in os.listdir("3rdparty/imgui"):
    if (i.endswith(".h") or i.endswith(".cpp")):
        SrcTemporary = os.path.join(working_dir, "3rdparty/imgui/" + i)
        DstTemporary = os.path.join(working_dir, "includes/imgui/" + i)
        shutil.copyfile(SrcTemporary, DstTemporary)        


for g in os.listdir("3rdparty/GLAD"):
    if (g.endswith(".h") or g.endswith(".c")):
        SrcTemporary = os.path.join(working_dir, "3rdparty/GLAD/" + g)
        DstTemporary = os.path.join(working_dir, "includes/GLAD/" + g)
        shutil.copyfile(SrcTemporary, DstTemporary)        
