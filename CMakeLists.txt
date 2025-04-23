cmake_minimum_required(VERSION 3.10)

project(rosettax87)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rosettax87 loader/main.cpp loader/macho_loader.cpp)

add_executable(libRuntimeRosettax87 
    rosettaRuntime/main.cpp
    rosettaRuntime/X87Register.cpp
    rosettaRuntime/X87State.cpp
    rosettaRuntime/X87.cpp
    #rosettaRuntime/X87Logger.cpp
    # rosettaRuntime/X87Simple.cpp
    rosettaRuntime/Export.cpp
    rosettaRuntime/Log.cpp
    rosettaRuntime/Neon.cpp
    rosettaRuntime/mmath.cpp
)

target_include_directories(libRuntimeRosettax87 PRIVATE
    rosettaRuntime
)

target_link_options(libRuntimeRosettax87 PRIVATE
"-Wl,-no_compact_unwind"
"-Wl,-no_pie"
"-static"
"-nostdlib"
"-Wl,-segaddr,__TEXT,0x0"
"-Wl,-segaddr,__DATA,0x10000"
"-Wl,-pagezero_size,0x0"
"-Wl,-headerpad,0"
"-Wl,-e,start"
"-Wl,-segalign,10"
)

target_compile_options(libRuntimeRosettax87 PRIVATE
    "-fvisibility=hidden"
    "-O3" 
)

target_link_libraries(libRuntimeRosettax87 PRIVATE
    fastermath
)

add_library(fastermath STATIC
    third_party/fastermath/src/exp.c
)

target_include_directories(fastermath PUBLIC
    third_party/fastermath/include
)

target_compile_definitions(fastermath PRIVATE
    "_FM_ALIGN=16"
    "_FM_INTERNAL"
)