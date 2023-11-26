cmake_minimum_required(VERSION 3.10)
project(physim)

find_library(SFML_GRAPHICS_LIBRARY sfml-graphics-s HINTS "dependencies/SFML-2.6.1/lib")
find_library(SFML_WINDOW_LIBRARY sfml-window-s HINTS "dependencies/SFML-2.6.1/lib")
find_library(SFML_SYSTEM_LIBRARY sfml-system-s HINTS "dependencies/SFML-2.6.1/lib")
find_library(FREETYPE_LIBRARY freetype HINTS "dependencies/SFML-2.6.1/lib")

# Check if the libraries are found
if(NOT SFML_GRAPHICS_LIBRARY OR NOT SFML_WINDOW_LIBRARY OR NOT SFML_SYSTEM_LIBRARY OR NOT FREETYPE_LIBRARY)
    message(FATAL_ERROR "SFML libraries (and dependencies) not found.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_GENERATOR_PLATFORM "x64")

add_definitions(-DSFML_STATIC)

file(GLOB_RECURSE PHYSIM_SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE PHYSIM_HEADER_FILES "include/*.h")

set(IMGUI_SOURCE_FILES
    dependencies/imgui/imgui.cpp
    dependencies/imgui/imgui_widgets.cpp
    dependencies/imgui/imgui_draw.cpp
    dependencies/imgui/imgui_tables.cpp
    dependencies/imgui/imgui_demo.cpp
    # Add other relevant source files from ImGui
)

set(IMGUI_HEADER_FILES
    dependencies/imgui/imgui.h
    dependencies/imgui/imconfig.h
    dependencies/imgui/imgui_internal.h
    # Add other relevant header files from ImGui
)

set(IMGUI_SFML_SOURCE_FILES
    dependencies/imgui-sfml/imgui-SFML.cpp
    # Add other relevant source files from ImGui-SFML
)

set(IMGUI_SFML_HEADER_FILES
    dependencies/imgui-sfml/imgui-SFML.h
    # Add other relevant header files from ImGui-SFML
)

include_directories(include)
include_directories(dependencies/imgui)
include_directories(dependencies/imgui-sfml)
include_directories(dependencies/SFML-2.6.1/include)

add_executable(physim 
${PHYSIM_SOURCE_FILES} ${PHYSIM_HEADER_FILES}
${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES}
${IMGUI_SFML_SOURCE_FILES} ${IMGUI_SFML_HEADER_FILES}
)

target_link_libraries(physim
    ${SFML_GRAPHICS_LIBRARY}
    ${SFML_WINDOW_LIBRARY}
    ${SFML_SYSTEM_LIBRARY}
    winmm 
	opengl32 
	${FREETYPE_LIBRARY} 
	user32
	gdi32
)