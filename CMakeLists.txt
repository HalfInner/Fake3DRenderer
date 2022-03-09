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

target_link_libraries(${PROJECT_NAME} 
        PUBLIC -static-libgcc 
        -static-libstdc++ 
        -static pthread glfw3 OpenGL32 GlU32 glad)


# set(OPENGL_LIB_PATH C:/Program Files\ \(x86\)/Windows\ Kits/10/Lib/10.0.18362.0/um/x64/)
# set(GLEW_INCLUDE_PATH C:/Program\ Files\ \(x86\)/glew/include/)
# set(GLEW_INCLUDE_PATH BENIZ)
# set(GLEW_LIB_PATH C:/Program\ Files\ \(x86\)/glew/lib/)
# set(GLM_INCLUDE_PATH C:/Users/kajbr/Programming/glm)

# message(${GLEW_INCLUDE_PATH})

# set(GLFW3_INCLUDE_PATH C:/include/)
# set(GLFW3_LIB_PATH C:/lib/)
# set(GLFW3_LIB_PATH C:/Users/kajbr/Programming/glfw/build-static/src)

# set(INCLUDE_PATH include/)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

# if ( CMAKE_BUILD_TYPE MATCHES "Release" )
#         # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror ")
# else()
#         # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wall -Wpedantic -Werror")
# endif()



# target_compile_definitions(${PROJECT_NAME} PUBLIC GLEW_STATIC)
# target_include_directories(${PROJECT_NAME} PUBLIC
#         ${INCLUDE_PATH} ${GLFW3_INCLUDE_PATH} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_PATH})
# target_link_directories(${PROJECT_NAME} 
#         PUBLIC ${OPENGL_LIB_PATH}
#         PUBLIC ${GLFW3_LIB_PATH} 
        # PUBLIC ${GLEW_LIB_PATH})

message(Copying\ dependencies)
configure_file(texture/PoolBall8.jpg ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PoolBall8.jpg COPYONLY)