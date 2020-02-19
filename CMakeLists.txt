cmake_minimum_required(VERSION 3.15)
project(Fake3DRenderer)

set(CMAKE_CXX_STANDARD 17)

set(OPENGL_LIB_PATH C:/Program Files\ \(x86\)/Windows\ Kits/10/Lib/10.0.18362.0/um/x64/)

message(hey ${OPENGL_LIB_PATH})

add_executable(Fake3DRenderer main.cpp)
target_compile_definitions(Fake3DRenderer PUBLIC kutang)
target_link_directories(Fake3DRenderer PUBLIC ${OPENGL_LIB_PATH})
target_link_libraries(Fake3DRenderer PUBLIC OpenGL32 GlU32)

