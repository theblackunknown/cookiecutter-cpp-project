cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project({{ cookiecutter.project_name }}
        VERSION {{ cookiecutter.version }}
        DESCRIPTION "{{ cookiecutter.project_description }}"
        LANGUAGES NONE
)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard")
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "Use C++ compiler extensions")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/Debug/bin" CACHE PATH "Where to output binaries in Debug" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/Debug/lib" CACHE PATH "Where to output libraries in Debug" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/bin" CACHE PATH "Where to output binaries in Release" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/lib" CACHE PATH "Where to output libraries in Release" FORCE)

add_subdirectory(lib)
add_subdirectory(app)
