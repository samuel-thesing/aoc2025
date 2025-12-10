cmake_minimum_required(VERSION 3.29)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY
            "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

project(aoc_2025)

include(FetchContent)

set(CMAKE_CXX_STANDARD 26)

set(WORKSPACE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(WORKSPACE_DEPS_DIR "${WORKSPACE_DIR}/deps/")
set(WORKSPACE_ARTIFACT_DIR "${WORKSPACE_DIR}/bin-int/")
set(WORKSPACE_BINARY_DIR "${WORKSPACE_DIR}/bin/")

# prevent installing to system directories.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${WORKSPACE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WORKSPACE_BINARY_DIR})

set(CMAKE_INSTALL_PREFIX "${WORKSPACE_BINARY_DIR}" CACHE INTERNAL "")

#===========================================================================================
# spdlog

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.16.0
)

FetchContent_GetProperties(spdlog)
FetchContent_MakeAvailable(spdlog)

#===========================================================================================
# z3
# First, try to find Z3 on the system
set(Z3_MIN_VERSION "4.15.3")
find_package(Z3 ${Z3_MIN_VERSION} CONFIG QUIET)

if(Z3_FOUND)
    message(STATUS "Found system Z3 version ${Z3_VERSION_STRING}")
    # Z3_LIBRARIES will contain z3::libz3
else()
    message(STATUS "System Z3 not found or version too old, fetching Z3 ${Z3_MIN_VERSION}")

    set(CMAKE_POLICY_DEFAULT_CMP0092 NEW)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON)
    # Fallback to FetchContent
    FetchContent_Declare(Z3
            GIT_REPOSITORY https://github.com/Z3Prover/z3
            GIT_TAG        z3-${Z3_MIN_VERSION}
    )
    FetchContent_MakeAvailable(Z3)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS OFF)

    # Add the C++ API include directory for z3++.h
    if(TARGET libz3)
        target_include_directories(libz3 INTERFACE
                $<BUILD_INTERFACE:${z3_SOURCE_DIR}/src/api/c++>
        )
    endif()

    # Create an alias to match the system install target name
    if(NOT TARGET z3::libz3)
        add_library(z3::libz3 ALIAS libz3)
    endif()
endif()

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
#===========================================================================================


foreach(DAY RANGE 1 12)
    if (DAY LESS 10)
        set(DAY_NAME "0${DAY}")
    else()
        set(DAY_NAME "${DAY}")
    endif()
    set(DAY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/${DAY_NAME}")
    if(IS_DIRECTORY ${DAY_PATH})
        add_subdirectory("src/${DAY_NAME}")
    endif()
endforeach()




