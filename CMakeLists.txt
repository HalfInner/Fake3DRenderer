cmake_minimum_required(VERSION 3.15)
project(Fake3DRenderer)

set(CMAKE_CXX_STANDARD 17)

set(OPENGL_LIB_PATH C:/Program Files\ \(x86\)/Windows\ Kits/10/Lib/10.0.18362.0/um/x64/)

set(GLEW_INCLUDE_PATH C:/Program\ Files\ \(x86\)/glew/include/)
set(GLEW_LIB_PATH C:/Program\ Files\ \(x86\)/glew/lib/)
set(GLEW_DLL_PATH C:/Program\ Files\ \(x86\)/glew/bin/glew32.dll)

set(GLM_INCLUDE_PATH C:/Users/kajbr/Programming/glm)

message(${GLEW_INCLUDE_PATH})

set(GLFW3_INCLUDE_PATH C:/include/)
set(GLFW3_LIB_PATH C:/lib/)

set(INCLUDE_PATH include/)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror")

add_executable(Fake3DRenderer main.cpp source/BasicCamera.cc source/Fake3DEngine.cc source/TextureManager.cc)
target_compile_definitions(Fake3DRenderer PUBLIC GLEW_STATIC)
target_include_directories(Fake3DRenderer PUBLIC
        ${INCLUDE_PATH} ${GLFW3_INCLUDE_PATH} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_PATH})
target_link_directories(Fake3DRenderer PUBLIC ${OPENGL_LIB_PATH} ${GLFW3_LIB_PATH} ${GLEW_LIB_PATH})
target_link_libraries(Fake3DRenderer PUBLIC -static-libgcc -static-libstdc++ -static pthread glew32 OpenGL32 GlU32 glfw3)