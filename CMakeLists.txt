#   Copyright (C) 2022 by Kyle Hayes
#   Author Kyle Hayes  kyle.hayes@gmail.com
#
# This software is available under either the Mozilla Public license
# version 2.0 (MPL 2.0) or the GNU LGPL version 2 (or later) license, whichever
# you choose.
#
# MPL 2.0:
#
#   This Source Code Form is subject to the terms of the Mozilla Public
#   License, v. 2.0. If a copy of the MPL was not distributed with this
#   file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
#
# LGPL 2:
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU Library General Public License as
#   published by the Free Software Foundation; either version 2 of the
#   License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU Library General Public
#   License along with this program; if not, write to the
#   Free Software Foundation, Inc.,
#   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


# the project is version 2.5
set (libplctag_VERSION_MAJOR 2)
set (libplctag_VERSION_MINOR 5)
set (libplctag_VERSION_PATCH 4)
set (VERSION "${libplctag_VERSION_MAJOR}.${libplctag_VERSION_MINOR}.${libplctag_VERSION_PATCH}")

set (LIB_NAME_SUFFIX "${libplctag_VERSION_MAJOR}.${libplctag_VERSION_MINOR}.${libplctag_VERSION_PATCH}")

cmake_minimum_required (VERSION 3.4...3.19)

# prevent expansion of quoted things that could be variables in if()
if(${CMAKE_VERSION} VERSION_GREATER 3.1)
    cmake_policy(SET CMP0054 NEW)
endif()

set(ANDROID_BUILD 0)

if(${CROSS_BUILD_TYPE} MATCHES "Linux-Arm6")
    # this also announces that we are cross compiling
    set(CMAKE_SYSTEM_NAME Linux)

    # set the CPU/architecture type
    set(CMAKE_SYSTEM_PROCESSOR armv6)

    # where are we going to get the cross compiler?
    set(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabihf-gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++")

    # where are the include files and libraries?
    set(CMAKE_FIND_ROOT_PATH "/usr/arm-linux-gnueabihf")

    # search programs in the host environment only.
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

    message("Building for Linux (Ubuntu/Debian) Arm v6")
elseif(${CROSS_BUILD_TYPE} MATCHES "Linux-Arm7")
    # this also announces that we are cross compiling
    set(CMAKE_SYSTEM_NAME Linux)

    # set the CPU/architecture type
    set(CMAKE_SYSTEM_PROCESSOR armv7l)

    # where are we going to get the cross compiler?
    set(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabihf-gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++")

    # where are the include files and libraries?
    set(CMAKE_FIND_ROOT_PATH "/usr/arm-linux-gnueabihf")

    # search programs in the host environment only.
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

    message("Building for Linux (Ubuntu/Debian) Arm v7 32-bit")
elseif(${CROSS_BUILD_TYPE} MATCHES "Linux-Aarch64")
    # this also announces that we are cross compiling
    set(CMAKE_SYSTEM_NAME Linux)

    # set the CPU/architecture type
    set(CMAKE_SYSTEM_PROCESSOR ARM64)

    # where are we going to get the cross compiler?
    set(CMAKE_C_COMPILER "/usr/bin/aarch64-linux-gnu-gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/aarch64-linux-gnu-g++")

    # where are the include files and libraries?
    set(CMAKE_FIND_ROOT_PATH "/usr/aarch64-linux-gnu")

    # search programs in the host environment only.
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

    message("Building for Linux (Ubuntu/Debian) Arm v8 64-bit")
elseif(${CROSS_BUILD_TYPE})
    message("Unsupported cross-compilation build type: ${CROSS_BUILD_TYPE}!")
    message("   Use -DCROSS_BUILD_TYPE=<type>")
    message("   where <type> is one of:")
    message("      Linux-Arm6 - older Linux 32-bit Arm v6 systems.")
    message("      Linux-Arm7 - newer Linux 32-bit Arm v7l systems.")
    message("      Linux-Aarch64 - newer Linux 64-bit Arm v8 systems.")
    return()
endif()

# set compiler and flags for 32-bit MinGW builds
if (${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
    SET(CMAKE_C_COMPILER gcc.exe)
    SET(CMAKE_C_FLAGS "-m32 -mno-ms-bitfields -D_WIN32_WINNT=0x0600")
    SET(CMAKE_CXX_COMPILER g++.exe)
    SET(CMAKE_CXX_FLAGS "-m32 -mno-ms-bitfields -D_WIN32_WINNT=0x0600")
endif()

# build examples flag
set(BUILD_EXAMPLES 0 CACHE BOOL "Build examples or not")

# default setting for 32-bit builds
set(BUILD_32_BIT 0 CACHE BOOL "Linux 32-bit build selector")

# this is the root libplctag project
project (libplctag_project)

# make sure our outputs are going somewhere sane
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_dist)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_dist)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_dist)

# we need threads
find_package(Threads REQUIRED)

# set the main paths.
set ( base_SRC_PATH "${PROJECT_SOURCE_DIR}/src" )
set ( lib_SRC_PATH "${base_SRC_PATH}/lib" )
set ( protocol_SRC_PATH "${base_SRC_PATH}/protocols" )
set ( ab_SRC_PATH "${protocol_SRC_PATH}/ab" )
set ( mb_SRC_PATH "${protocol_SRC_PATH}/mb" )
set ( util_SRC_PATH "${base_SRC_PATH}/util" )
set ( example_SRC_PATH "${base_SRC_PATH}/examples" )
set ( test_SRC_PATH "${base_SRC_PATH}/tests" )
set ( cli_SRC_PATH "${base_SRC_PATH}/contrib/cli" )
set ( pybind11_SRC_PATH "${PROJECT_SOURCE_DIR}/pybind11/include/pybind11")
set ( python_SRC_PATH "/usr/include/python3.10")
set ( cpp_SRC_PATH "/usr/include/c++/9")
set ( example_PATH "src/pybind11/")

# OS-specific files for the platform code.
set ( platform_SRC_PATH "${base_SRC_PATH}/platform/posix" )

# where to find include files.
include_directories("${base_SRC_PATH}")
include_directories("${platform_SRC_PATH}")
include_directories("${protocol_SRC_PATH}")
include_directories("${pybind11_SRC_PATH}")
include_directories("${python_SRC_PATH}")
include_directories("${cpp_SRC_PATH}")
include_directories("/usr/local/include")
include_directories("/usr/include")
include_directories("/usr/include/c++/9/backward")
include_directories("/usr/include/x86_64-linux-gnu/c++/9")
include_directories("/usr/lib/gcc/x86_64-linux-gnu/9/include")

SET(C11_SUPPORT False)
SET(C99_FLAGS "")

# C compiler specific settings
if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # using Clang

    SET(STR_OP_OVERFLOW_ON "")
    SET(STR_OP_OVERFLOW_OFF "")

    set(BASE_RELEASE_FLAGS "${CMAKE_C_FLAGS} -Os -Wall -pedantic -Wextra -Wconversion -fms-extensions -fno-strict-aliasing -fvisibility=hidden ${STR_OP_OVERFLOW_OFF}")
    set(BASE_DEBUG_FLAGS "${CMAKE_C_FLAGS} -g -Wall -pedantic -Wextra -Wconversion -fms-extensions -fno-strict-aliasing -fvisibility=hidden ${STR_OP_OVERFLOW_ON}")
    set(BASE_RELEASE_LINK_FLAGS "")
    set(BASE_DEBUG_LINK_FLAGS "")

    if(APPLE)
        set(BASE_RELEASE_FLAGS "${BASE_RELEASE_FLAGS} -D_DARWIN_C_SOURCE")
        set(BASE_DEBUG_FLAGS "${BASE_DEBUG_FLAGS} -D_DARWIN_C_SOURCE")
    else()
        set(BASE_RELEASE_FLAGS "${BASE_RELEASE_FLAGS} -D__USE_POSIX=1 -D_POSIX_C_SOURCE=200809L")
        set(BASE_DEBUG_FLAGS "${BASE_DEBUG_FLAGS} -D__USE_POSIX=1 -D_POSIX_C_SOURCE=200809L")
    endif()

	set(C11_SUPPORT true)
	set(C99_FLAGS "-std=c99" )

    # check to see if we are building 32-bit or 64-bit
    if(BUILD_32_BIT)
        set(BASE_RELEASE_FLAGS "${BASE_RELEASE_FLAGS} -m32")
        set(BASE_DEBUG_FLAGS "${BASE_DEBUG_FLAGS} -m32")
        set(BASE_RELEASE_LINK_FLAGS "${BASE_RELEASE_LINK_FLAGS} -m32")
        set(BASE_DEBUG_LINK_FLAGS "${BASE_DEBUG_LINK_FLAGS} -m32")
    endif()
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # using GCC

	SET(C11_CHECK "")

	# get GCC version
	if(${CMAKE_VERSION} VERSION_GREATER 2.8.9)
		MESSAGE(STATUS "CMake is newer, using a sane way to get the GCC version..")
		SET(COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
	else()
		# old CMake.   Do this the hard way.
		MESSAGE(STATUS "CMake is old, using the old way to get the GCC version.")
		EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE COMPILER_VERSION)
		MESSAGE(STATUS "COMPILER_VERSION=" ${COMPILER_VERSION})
	endif()

	if(${COMPILER_VERSION} VERSION_GREATER 5.1 OR ${COMPILER_VERSION} VERSION_GREATER 5.1)
		# The compiler has the C99-c11-compat check.
		MESSAGE(STATUS "Compiler supports C99/C11 compatibility check." )
		SET(C11_CHECK "-Wc99-c11-compat" )
		SET(C11_SUPPORT True)
	else()
		MESSAGE(STATUS "Compiler does not support C99/C11 compatibility check." )
	endif()

    if(${COMPILER_VERSION} VERSION_GREATER 8.0 AND ${COMPILER_VERSION} VERSION_LESS 11.0)
        # the compiler supports the -Wstringop-overflow flag.
        # There is clearly a bug in the compiler that throws this warning inconsistently.
        # Suppress it for RELEASE builds.
        MESSAGE(STATUS "Compiler version ${COMPILER_VERSION} supports string op overflow checking.  Suppress for Release builds.")
        SET(STR_OP_OVERFLOW_ON "-Wstringop-overflow")
        SET(STR_OP_OVERFLOW_OFF "-Wno-stringop-overflow")
    else()
        MESSAGE(STATUS "Compiler version ${COMPILER_VERSION} does not support string op overflow checking.")
        SET(STR_OP_OVERFLOW_ON "")
        SET(STR_OP_OVERFLOW_OFF "")
    endif()

    set(BASE_RELEASE_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -Wconversion -fms-extensions -fno-strict-aliasing -fvisibility=hidden ${STR_OP_OVERFLOW_OFF} -D__USE_POSIX=1 -D_POSIX_C_SOURCE=200809L")
    set(BASE_DEBUG_FLAGS "${CMAKE_C_FLAGS} -g -Wall -pedantic -Wextra -Wconversion -fms-extensions -fno-strict-aliasing -fvisibility=hidden ${STR_OP_OVERFLOW_ON} -D__USE_POSIX=1 -D_POSIX_C_SOURCE=200809L")
	set(C99_FLAGS "-std=c99" )
    set(BASE_RELEASE_LINK_FLAGS "")
    set(BASE_DEBUG_LINK_FLAGS "")

    # check to see if we are building 32-bit or 64-bit
    if(BUILD_32_BIT)
        set(BASE_RELEASE_FLAGS "${BASE_RELEASE_FLAGS} -m32")
        set(BASE_DEBUG_FLAGS "${BASE_DEBUG_FLAGS} -m32")
        set(BASE_RELEASE_LINK_FLAGS "${BASE_RELEASE_LINK_FLAGS} -m32")
        set(BASE_DEBUG_LINK_FLAGS "${BASE_DEBUG_LINK_FLAGS} -m32")
    endif()
elseif (CMAKE_C_COMPILER_ID STREQUAL "Intel")
    # using Intel C/C++
    MESSAGE("Intel C compiler not supported!")
    RETURN()
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C/C++
    set(BASE_RELEASE_FLAGS "${CMAKE_C_FLAGS} /W3")
    set(BASE_DEBUG_FLAGS "${CMAKE_C_FLAGS} /W3")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # /MD$<$<STREQUAL:$<CONFIGURATION>,Debug>:d>
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BASE_FLAGS "${BASE_DEBUG_FLAGS}")
    set(BASE_LINK_FLAGS "${BASE_DEBUG_LINK_FLAGS}")
else()
    set(BASE_FLAGS "${BASE_RELEASE_FLAGS}")
    set(BASE_LINK_FLAGS "${BASE_RELEASE_LINK_FLAGS}")
endif()

#MESSAGE("BASE_FLAGS=${BASE_FLAGS}")

#set(BASE_C_FLAGS "${BASE_FLAGS} -std=c99 -Wc++-compat ")
set(BASE_C_FLAGS "${BASE_FLAGS} -std=c99")
set(STATIC_COMPILE_FLAGS "")
set(STATIC_LINK_FLAGS "-static")

set(BASE_CXX_FLAGS "${BASE_FLAGS}")

MESSAGE("BASE_C_FLAGS=${BASE_C_FLAGS}")
MESSAGE("BASE_CXX_FLAGS=${BASE_CXX_FLAGS}")

# generate version file from CMake info.
CONFIGURE_FILE("${lib_SRC_PATH}/version.h.in" "${lib_SRC_PATH}/version.h" @ONLY)

# set up the library sources
set ( libplctag_SRCS "${lib_SRC_PATH}/init.c"
                     "${lib_SRC_PATH}/init.h"
                     "${lib_SRC_PATH}/libplctag.h"
                     "${lib_SRC_PATH}/lib.c"
                     "${lib_SRC_PATH}/tag.h"
                     "${lib_SRC_PATH}/version.h"
                     "${lib_SRC_PATH}/version.c"
                     "${ab_SRC_PATH}/ab.h"
                     "${ab_SRC_PATH}/ab_common.c"
                     "${ab_SRC_PATH}/ab_common.h"
                     "${ab_SRC_PATH}/cip.c"
                     "${ab_SRC_PATH}/cip.h"
                     "${ab_SRC_PATH}/defs.h"
                     "${ab_SRC_PATH}/eip_cip.c"
                     "${ab_SRC_PATH}/eip_cip.h"
                     "${ab_SRC_PATH}/eip_cip_special.c"
                     "${ab_SRC_PATH}/eip_cip_special.h"
                     "${ab_SRC_PATH}/eip_lgx_pccc.c"
                     "${ab_SRC_PATH}/eip_lgx_pccc.h"
                     "${ab_SRC_PATH}/eip_plc5_dhp.c"
                     "${ab_SRC_PATH}/eip_plc5_dhp.h"
                     "${ab_SRC_PATH}/eip_plc5_pccc.c"
                     "${ab_SRC_PATH}/eip_plc5_pccc.h"
                     "${ab_SRC_PATH}/eip_slc_dhp.c"
                     "${ab_SRC_PATH}/eip_slc_dhp.h"
                     "${ab_SRC_PATH}/eip_slc_pccc.c"
                     "${ab_SRC_PATH}/eip_slc_pccc.h"
                     "${ab_SRC_PATH}/error_codes.c"
                     "${ab_SRC_PATH}/error_codes.h"
                     "${ab_SRC_PATH}/pccc.c"
                     "${ab_SRC_PATH}/pccc.h"
                     "${ab_SRC_PATH}/session.c"
                     "${ab_SRC_PATH}/session.h"
                     "${ab_SRC_PATH}/tag.h"
                     "${mb_SRC_PATH}/modbus.c"
                     "${mb_SRC_PATH}/modbus.h"
                     "${protocol_SRC_PATH}/system/system.c"
                     "${protocol_SRC_PATH}/system/system.h"
                     "${protocol_SRC_PATH}/system/tag.h"
                     "${util_SRC_PATH}/atomic_int.c"
                     "${util_SRC_PATH}/atomic_int.h"
                     "${util_SRC_PATH}/attr.c"
                     "${util_SRC_PATH}/attr.h"
                     "${util_SRC_PATH}/byteorder.h"
                     "${util_SRC_PATH}/debug.c"
                     "${util_SRC_PATH}/debug.h"
                     "${util_SRC_PATH}/hash.c"
                     "${util_SRC_PATH}/hash.h"
                     "${util_SRC_PATH}/hashtable.c"
                     "${util_SRC_PATH}/hashtable.h"
                     "${util_SRC_PATH}/macros.h"
                     "${util_SRC_PATH}/rc.c"
                     "${util_SRC_PATH}/rc.h"
                     "${util_SRC_PATH}/vector.c"
                     "${util_SRC_PATH}/vector.h"
                     "${platform_SRC_PATH}/platform.c"
                     "${platform_SRC_PATH}/platform.h"
                    #  "${python_SRC_PATH}/Python.h"
                    #  "${pybind11_SRC_PATH}/pybind11.h"
                     )

# set the compiler flags
FOREACH( lib_src ${libplctag_SRCS} )
    set_source_files_properties(${lib_src} PROPERTIES COMPILE_FLAGS "${C99_FLAGS} ${BASE_C_FLAGS} ${C11_CHECK}")
ENDFOREACH()

# shared library
add_library(plctag_dyn SHARED ${libplctag_SRCS} )

# set various properties on them
set_target_properties(plctag_dyn PROPERTIES SOVERSION "${libplctag_VERSION_MAJOR}.${libplctag_VERSION_MINOR}" OUTPUT_NAME "plctag")

if(BASE_LINK_FLAGS)
    set_target_properties(plctag_dyn PROPERTIES LINK_FLAGS "${BASE_LINK_FLAGS}")
endif()


# static library
add_library(plctag_static STATIC ${libplctag_SRCS} )
set_target_properties(plctag_static PROPERTIES LINK_FLAGS "${BASE_LINK_FLAGS}")
set_target_properties(plctag_static PROPERTIES VERSION "${libplctag_VERSION_MAJOR}.${libplctag_VERSION_MINOR}" OUTPUT_NAME "plctag")

set(tool_lib "plctag_dyn")


if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(plctag_dyn "${CMAKE_THREAD_LIBS_INIT}")
    target_link_libraries(plctag_static "${CMAKE_THREAD_LIBS_INIT}")
endif()
set ( example_PROGRAMS async
async_stress
barcode_test
busy_test
data_dumper
list_tags_logix
list_tags_micro8x0
multithread
multithread_cached_read
multithread_plc5
multithread_plc5_dhp
plc5
simple
simple_dual
slc500
stress_api_lock
stress_test
string_non_standard_udt
string_standard
test_auto_sync
test_callback
test_callback_ex
test_connection_group
test_many_tag_perf
test_raw_cip
test_reconnect
test_shutdown
test_special
test_string
test_tag_attributes
thread_stress
toggle_bit
toggle_bool
write_string
tag_rw
tag_rw2
)

set ( example_PROG_UTIL utils_posix.c )
set ( example_LIBRARIES ${tool_lib} pthread )
set_source_files_properties("${example_SRC_PATH}/${example_PROG_UTIL}" PROPERTIES COMPILE_FLAGS "${C99_FLAGS} ${BASE_C_FLAGS} ${C11_CHECK}" )

# set the compile and link properties for all examples.
foreach ( example ${example_PROGRAMS} )
    set_source_files_properties("${example_SRC_PATH}/${example}.c" PROPERTIES COMPILE_FLAGS "${C99_FLAGS} ${BASE_C_FLAGS} ${C11_CHECK}" )
    add_executable( ${example} "${example_SRC_PATH}/${example}.c" "${example_SRC_PATH}/${example_PROG_UTIL}" "${example_SRC_PATH}/utils.h" )
    target_link_libraries(${example} ${example_LIBRARIES} )

    if(BASE_LINK_FLAGS)
        set_target_properties(${example} PROPERTIES LINK_FLAGS "${BASE_LINK_FLAGS}")
    endif()
endforeach(example)


CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/src/lib/libplctag.h" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libplctag.h" COPYONLY)
# set_source_files_properties("${example_PATH}/pybind11_example.cpp" PROPERTIES COMPILE_FLAGS "-O3 -Wall -shared -std=c++11 -fPIC$(python3.10 -m pybind11 --includes)" )
# add_executable(pybind11_example "${example_PATH}/pybind11_example.cpp" "${pybind11_SRC_PATH}/pybind11.h" "${python_SRC_PATH}/Python.h") 
# target_link_libraries(pybind11_example "${example_LIBRARIES}")

add_subdirectory(pybind11)
pybind11_add_module(pybind11_example "${example_PATH}/pybind11_example.cpp")
# target_link_libraries(pybind11_example PRIVATE pybind11::pybind11 pybind11::embed pybind11::module pybind11::lto "${libplctag_SRCS}")
target_link_libraries(pybind11_example PRIVATE ${example_LIBRARIES})


# add the cli
if(ANDROID_BUILD)
    MESSAGE("Building for Android.")
endif(ANDROID_BUILD)

# for installation
if(UNIX)
	install(TARGETS plctag_dyn DESTINATION lib${LIB_SUFFIX})
	install(TARGETS plctag_static DESTINATION lib${LIB_SUFFIX})
elseif(WIN32)
	install(TARGETS plctag_dyn DESTINATION lib${LIB_SUFFIX})
	install(TARGETS plctag_static DESTINATION lib${LIB_SUFFIX})
endif()

if(ANDROID_BUILD)
    message("Skipping package config and header file copy for Android build.")
else()
    install(FILES "${lib_SRC_PATH}/libplctag.h" DESTINATION include)
    if(EXISTS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libplctag.pc")
        install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libplctag.pc" DESTINATION "lib${LIB_SUFFIX}/pkgconfig")
    endif()
endif(ANDROID_BUILD)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

# Debugging
# print_all_variables()
# target_compile_definitions(libplctag PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
