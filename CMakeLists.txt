cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message("generator is setto ${CMAKE_GENERATOR}")

project(mojsolver) # change the name here


file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# this is so stupid
set(CMAKE_SIZEOF_VOID_P 4)


if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  # ensure 32 bit on clang
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target i386-pc-windows-msvc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target i386-pc-windows-msvc")
  add_definitions("--target=i386-pc-windows-msvc")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  lib/minhook/include
  lib/gd.h/include
  lib/gd.h/
  lib/mat-dash/include
  )
  target_compile_definitions(mojsolver PRIVATE CC_DLL=)

add_subdirectory(lib/minhook)
add_subdirectory(lib/cocos-headers)
add_subdirectory(lib/mat-dash)
target_link_libraries(${PROJECT_NAME} minhook cocos2d mat-dash opengl32)