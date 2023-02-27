@ECHO OFF
set glslc=%VK_SDK_PATH%\Bin\glslc.exe
set shader_path=.\shaders
set shader_stages="*.vert","*.frag"

cd %shader_path%
for %%v in (%shader_stages%) do (
	%glslc% ./%%v -o ./%%v.spv
	ECHO [SHADER COMPILER] Compiling %%v to %%v.spv
)