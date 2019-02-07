#
# Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/boostorg/beast
#




#SET(CMAKE_C_COMPILER /usr/local/gcc-8.1.0/bin/gcc-8.1.0)
#SET(CMAKE_CXX_COMPILER /usr/local/gcc-8.1.0/bin/g++-8.1.0)
# DONT DO THAT... INSTEAD export new gcc as such
#export CC=/usr/local/gcc-8.1.0/bin/gcc-8.1.0
#export CXX=/usr/local/gcc-8.1.0/bin/g++-8.1.0


cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

#project(CppCon2018 VERSION 0.1.0 LANGUAGES CXX)
project(WebSocketServer)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Boost_USE_STATIC_LIBS ON)

if(MSVC)
  set (Boost_USE_STATIC_RUNTIME ON)
  add_definitions(-D_WIN32_WINNT=0x0601)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS=1)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
  add_definitions(-D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
  add_compile_options(/MP)
  string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER build)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

file(GLOB_RECURSE BEAST_FILES
  ${Boost_INCLUDE_DIRS}/boost/beast/*.hpp
  ${Boost_INCLUDE_DIRS}/boost/beast/*.ipp)

source_group(TREE ${Boost_INCLUDE_DIRS}/boost/beast PREFIX beast FILES ${BEAST_FILES})

file(GLOB APP_FILES
  beast.hpp
  http_session.cpp
  http_session.hpp
  listener.cpp
  listener.hpp
  main.cpp
  net.hpp
  shared_state.cpp
  shared_state.hpp
  websocket_session.cpp
  websocket_session.hpp
  chat_client.html
  README.md)

source_group("" FILES ${APP_FILES})

#add_executable(${PROJECT_NAME} ${APP_FILES} ${BEAST_FILES})
add_library(${PROJECT_NAME}  STATIC  ${APP_FILES} ${BEAST_FILES})

if(NOT WIN32)
  find_package(Threads)
  target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${Boost_SYSTEM_LIBRARY} -latomic )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
  VS_STARTUP_PROJECT ${PROJECT_NAME}r)
