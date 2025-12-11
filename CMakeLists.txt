cmake_minimum_required(VERSION 3.29)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY
            "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

set(AOC_YEAR 2025)
set(NUM_DAYS 12)

project("aoc_${AOC_YEAR}")

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
# find_package(Z3 ${Z3_MIN_VERSION} CONFIG QUIET)


if(Z3_FOUND)
    message(STATUS "Found system Z3 version ${Z3_VERSION_STRING}")
    # Debug version always crashed
    if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        get_target_property(Z3_RELEASE_LIB z3::libz3 IMPORTED_LOCATION_RELEASE)
        if(Z3_RELEASE_LIB)
            set_target_properties(z3::libz3 PROPERTIES
                    IMPORTED_LOCATION_DEBUG "${Z3_RELEASE_LIB}"
            )
            message(STATUS "Debug build will use Release Z3 library")
        endif()
    endif()
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

get_target_property(Z3_LIB_DEBUG z3::libz3 IMPORTED_LOCATION_DEBUG)
get_target_property(Z3_LIB_RELEASE z3::libz3 IMPORTED_LOCATION_RELEASE)
get_target_property(Z3_IMPLIB_DEBUG z3::libz3 IMPORTED_IMPLIB_DEBUG)
get_target_property(Z3_IMPLIB_RELEASE z3::libz3 IMPORTED_IMPLIB_RELEASE)

function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        if (ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
            if (NOT MATCHED)
                continue()
            endif()
        endif()
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()
dump_cmake_variables("Z3")

function(copy_dlls target)
    if(WIN32)
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:z3::libz3>
                $<TARGET_FILE_DIR:${target}>
        )
    endif()
endfunction()
#===========================================================================================

set(ALL_DAYS "")
foreach(DAY RANGE 1 ${NUM_DAYS})
    if (DAY LESS 10)
        set(AOC_DAY_NAME "0${DAY}")
    else()
        set(AOC_DAY_NAME "${DAY}")
    endif()
    set(DAY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/${AOC_DAY_NAME}")
    if(IS_DIRECTORY ${DAY_PATH})
        add_subdirectory("src/${AOC_DAY_NAME}")
    endif()
    list(APPEND ALL_DAYS "aoc${AOC_YEAR}_${AOC_DAY_NAME}_1")
    list(APPEND ALL_DAYS "aoc${AOC_YEAR}_${AOC_DAY_NAME}_2")
endforeach()

add_custom_target("build_all_days" DEPENDS ${ALL_DAYS})






