project(CHIP8)
cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()


SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
SET(STATIC_SFML FALSE CACHE BOOL "Choose whether SFML is linked statically or not.")
SET(STATIC_RUNTIME FALSE CACHE BOOL "Use statically linked standard/runtime libraries? This option must match the one used for SFML.")

if(CMAKE_COMPILER_IS_GNUCXX)
  if(XY_STATIC_RUNTIME)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y -static")
  else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")
  endif()
endif()


SET(CMAKE_CXX_FLAGS_DEBUG "-g -D_DEBUG_")
SET(CMAKE_CXX_FLAGS_RELEASE "-O4 -DNDEBUG")


if(STATIC_SFML)
  SET(SFML_STATIC_LIBRARIES TRUE)
endif()

if(WIN32)
  find_package(SFML 2 REQUIRED graphics window system audio main)
else()
  find_package(SFML 2 REQUIRED graphics window system audio)
endif()

if(UNIX)
  find_package(X11 REQUIRED)
endif()

find_package(SFGUI REQUIRED)

include_directories(
  ${SFML_INCLUDE_DIR}
  ${SFGUI_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/include)

if(X11_FOUND)
  include_directories(${X11_INCLUDE_DIRS})
endif()


SET(SRC_DIR "src")
include("src/CMakeLists.txt")

if(WIN32)
  add_executable(CHIP8 WIN32 ${CHIP8_SRC}) 
else()
  add_executable(CHIP8 ${CHIP8_SRC})
endif()

target_link_libraries(CHIP8
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES}
  ${SFGUI_LIBRARY})

if(UNIX)
  target_link_libraries(CHIP8
    ${X11_LIBRARIES})
endif()
