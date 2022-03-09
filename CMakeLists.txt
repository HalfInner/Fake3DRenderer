cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME Fake3DRenderer)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE TRUE)


add_executable(${PROJECT_NAME} 
        main.cpp 
        source/BasicCamera.cc 
        source/Fake3DEngine.cc 
        source/TextureManager.cc)

add_subdirectory(3pp/glfw)
add_subdirectory(3pp/glm)
add_subdirectory(3pp/glad)

add_dependencies(${PROJECT_NAME} glad)
add_dependencies(${PROJECT_NAME} glfw)

target_include_directories(${PROJECT_NAME} 
        PUBLIC include
        PUBLIC 3pp/glfw/include
        PUBLIC 3pp/glm
        PUBLIC ${GLAD_INCLUDE_DIRS})

target_link_directories(${PROJECT_NAME}
        PUBLIC out/build/3pp/glfw/src/
        PUBLIC ./out/build/3pp/glad)

# Linux:        -static  rt glfw3 glad GLU dl  pthread
# Windows VS:   -static  glfw3 glad OpenGL32 GlU32
target_link_libraries(${PROJECT_NAME} 
        PUBLIC  -static-libgcc 
        -static-libstdc++ 
        -static  glfw3 glad OpenGL32 GlU32 pthread
)
