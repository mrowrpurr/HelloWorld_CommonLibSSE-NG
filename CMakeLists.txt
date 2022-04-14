cmake_minimum_required(VERSION 3.21)

project(HelloSkse VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Required for compiling CommonLibSSE
add_compile_options(/bigobj)

# Define your SKSE plugin
add_library(HelloSkse SHARED plugin.cpp)

# Link to CommonLibSSE
find_package(CommonLibSSE CONFIG REQUIRED)
target_link_libraries(HelloSkse PRIVATE CommonLibSSE::CommonLibSSE)

# Setup a folder which the output .dll automatically gets copied to!
set(DLL_FOLDER "C:/Users/mrowr/Dropbox/Skyrim/Mod Authoring/Mods/Mrowr Purr's SKSE Plugins - AE/SKSE/Plugins")
add_custom_command(
	TARGET "${PROJECT_NAME}"
	POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}"
	VERBATIM
)
