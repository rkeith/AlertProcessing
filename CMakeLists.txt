cmake_minimum_required(VERSION 3.15)
project(AlertProcessing LANGUAGES C)

add_executable(AlertProcessing main.c)
set_target_properties(AlertProcessing PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
if(MSVC)
    target_compile_definitions(AlertProcessing PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(AlertProcessing PRIVATE /W4)
else()
    target_compile_options(AlertProcessing PRIVATE -Wall -Wextra -Wpedantic)
endif()

configure_file(aaJsonDemo.json aaJsonDemo.json COPYONLY)
