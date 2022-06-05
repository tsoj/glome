cmake_minimum_required(VERSION 3.11)
project(glome)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ~/.cmake/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    #set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wshadow")
set(CMAKE_CXX_FLAGS_DEBUG "-O2 -g -fno-omit-frame-pointer -DUSE_ASCII_FRAMEBUFFER")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_executable(
        glome
        src/main.cpp
        src/utility.cpp
        src/types.cpp
        src/gl.cpp
        src/obj.cpp
        src/Renderer.cpp
        src/Window.cpp
        src/World.cpp
)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)


target_include_directories(
        glome
        PUBLIC
        ${GLEW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/gli
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/shader-printf
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/json/single_include/nlohmann
)
target_link_libraries(glome
        ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES}
        glfw
        pthread
        )