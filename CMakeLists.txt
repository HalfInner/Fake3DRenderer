cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME Fake3DRenderer)
project(${PROJECT_NAME})

set(CMAKE_VERBOSE_MAKEFILE FALSE)

add_executable(${PROJECT_NAME}
        main.cpp
        source/BasicCamera.cc
        source/Fake3DEngine.cc
        source/TextureManager.cc)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

add_subdirectory(3pp/glfw)
add_subdirectory(3pp/glm)
add_subdirectory(3pp/glad)

add_dependencies(${PROJECT_NAME} glad)
add_dependencies(${PROJECT_NAME} glfw)

target_include_directories(${PROJECT_NAME} 
        PRIVATE include
        PRIVATE 3pp/glfw/include
        PRIVATE 3pp/glm
        PRIVATE ${GLAD_INCLUDE_DIRS})

target_link_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_BINARY_DIR}/3pp/glfw/src/
        PRIVATE ${CMAKE_BINARY_DIR}/3pp/glad)

set(LINUX_LIBS "rt;glfw3;glad;GLU;dl;pthread")
set(WIN_LIBS "glfw3;glad;OpenGL32;GlU32")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        "$<$<PLATFORM_ID:Linux>:${LINUX_LIBS}>"
        "$<$<PLATFORM_ID:Windows>:${WIN_LIBS}>"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC _USE_MATH_DEFINES)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/texture/PoolBall8.jpg
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)
