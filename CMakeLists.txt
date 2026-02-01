cmake_minimum_required(VERSION 3.26)
project(action-recorder)

set(CMAKE_CXX_STANDARD 23)

if (MSVC)
    # Force static runtime library
    foreach(flag_var
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_DEBUG)
        string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endforeach()
endif()

add_executable(action-recorder main.cpp
        script.h
        utils.cpp
        utils.cpp
        script.cpp
        hardcoded.h)
