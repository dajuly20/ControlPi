
#export CC=/usr/local/gcc-8.1.0/bin/gcc-8.1.0
#export CXX=/usr/local/gcc-8.1.0/bin/g++-8.1.0   

#SET(CMAKE_C_COMPILER /usr/local/gcc-8.1.0/bin/gcc-8.1.0)
#SET(CMAKE_CXX_COMPILER /usr/local/gcc-8.1.0/bin/g++-8.1.0)

cmake_minimum_required (VERSION 3.0.0)

project(ControlPi)

if("${USE_TIDY}" STREQUAL "true")
    find_program(
        CLANG_TIDY_EXE
        NAMES "clang-tidy"
        DOC "Path to clang-tidy executable"
    )

    if(NOT CLANG_TIDY_EXE)
        message(FATAL_ERROR "clang-tidy not found.")
    else()
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(DO_C_CLANG_TIDY "${CLANG_TIDY_EXE}" "-p=." "-header-filter=.h" "-checks=*,-llvm-header-guard,-llvm-include-order,-clang-diagnostic-ignored-optimization-argument,-google-readability-todo,-fuchsia-overloaded-operator,-fuchsia-default-arguments,-hicpp-signed-bitwise")
        set(DO_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" "-p=." "-header-filter=.hpp" "-checks=*,-llvm-header-guard,-bugprone-macro-parentheses,-modernize-use-default-member-init,-clang-diagnostic-ignored-optimization-argument,-google-runtime-references,-google-readability-todo,-fuchsia-overloaded-operator,-fuchsia-default-arguments,-cert-dcl50-cpp,-fuchsia-trailing-return,-hicpp-vararg,-cppcoreguidelines-pro-type-vararg")
        set(CMAKE_C_CLANG_TIDY ${DO_C_CLANG_TIDY})
        set(CMAKE_CXX_CLANG_TIDY ${DO_CXX_CLANG_TIDY})
    endif()
else()
    set(CMAKE_C_CLANG_TIDY "")
    set(CMAKE_CXX_CLANG_TIDY "")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS FALSE)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-weak-vtables -Wno-padded -stdlib=libc++")

    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything -Wno-padded")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything -Wno-padded -Wno-sign-conversion -Wno-unreachable-code-break -Wno-switch-enum -Wno-float-conversion -Wno-conversion -Wno-double-promotion -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-unused-macros")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -Wstrict-aliasing=1 -Wold-style-cast")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -Wstrict-aliasing=1")

    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wformat=2 -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes -Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -ansi -Waggregate-return  -Wunreachable-code -Wconversion -Wstrict-overflow=5 -Wsign-conversion")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wformat=2 -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wbad-function-cast -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -pedantic -ansi -Waggregate-return  -Wunreachable-code -Wstrict-overflow=5")
else()
     message(FATAL_ERROR "no valid compiler")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if("${USE_SANITIZER}" STREQUAL "thread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=thread")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=thread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=thread")
elseif("${USE_SANITIZER}" STREQUAL "address")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=address")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=address")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=address")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=address -fsanitize=undefined")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=address -fsanitize=undefined")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=address -fsanitize=undefined")
    endif()
elseif("${USE_SANITIZER}" STREQUAL "memory")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        if (IS_DIRECTORY ${MSAN_LIBCXX_PATH})
            string (REPLACE "-stdlib=libc++" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

            add_compile_options("-nostdinc++")

            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=memory  -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fsanitize=undefined  -I${MSAN_LIBCXX_PATH}/include/c++/v1 -Wno-reserved-id-macro")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize=memory  -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fsanitize=undefined")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize-recover=all -fsanitize-memory-track-origins -fsanitize=memory  -fsanitize-memory-use-after-dtor -fsanitize=undefined -L${MSAN_LIBCXX_PATH}/lib -Wl,-rpath,${MSAN_LIBCXX_PATH}/lib")
        else()
            message( FATAL_ERROR "USE_SANITIZER==memory needs lib++ build with msan defined by MSAN_LIBCXX_PATH" )
        endif()
    else()
        message( FATAL_ERROR "cant use USE_SANITIZER==memory with gcc" )
    endif()
endif()


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(src/WebSocket)


add_executable(${PROJECT_NAME} 
#${APP_FILES}
regReplaceExtension.cpp
main.cpp
iterationSwitchGuard.cpp
src/CommandProcessor.cpp
src/IOChannels/IO_Channel_Virtual_Pipe.cpp
src/IOChannels/IO_Channel_Hw.cpp
src/IOChannels/IO_Channel_Virtual.cpp
src/IOChannels/IO_Channel_Virtual_Timer.cpp
src/IOChannels/IO_Channel.cpp
src/IOChannels/IO_Channel_Hw_PiFace.cpp
src/IOChannels/IO_Channel_Virtual_Memory.cpp
src/ChannelEntitys/Channel_Entity_Memory.cpp
src/ChannelEntitys/Channel_Entity.cpp
src/ChannelEntitys/Channel_Entities_PiFace.cpp
src/ChannelEntitys/Channel_Entity_TimerOutput.cpp
src/ChannelEntitys/Channel_Entity_TimerTrigger.cpp
src/ChannelEntitys/Channel_Entity_Pipe_RX.cpp
src/ChannelEntitys/Channel_Entity_Pipe_TX.cpp
src/ConfigParser.cpp
IO_Channel_AccesWrapper.cpp
boolLogicParser.cpp
)


#target_link_libraries(${PROJECT_NAME} asan)
target_link_libraries(${PROJECT_NAME} WebSocketServer)
#target_link_libraries(${PROJECT_NAME} ./src/WebSocket/bin/libWebSocketServer.so)

target_link_libraries(${PROJECT_NAME} /usr/local/lib/libpifacedigitalcpp.a)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libmcp23s17.a)

target_link_libraries(${PROJECT_NAME} Threads::Threads)


#enable_testing()


#add_subdirectory(knx_tunnel_shared)
