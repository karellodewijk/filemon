cmake_minimum_required(VERSION 2.8)
project(filemon)
set (filemon_VERSION 0.1)

project(filemon_tests)

include(FindPkgConfig)
pkg_check_modules(GLIB glib-2.0)

SET(CMAKE_C_FLAGS "-g -Wall")
foreach(arg ${GLIB_CFLAGS})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${arg}")
endforeach(arg ${CMAKE_C_FLAGS})

add_library(filemon SHARED filemon.c)
target_link_libraries(filemon
    ${GLIB_LIBRARIES}
)

add_subdirectory(tests)

SET(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)")
SET(EXEC_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} CACHE PATH "Installation prefix for executables and object code libraries" FORCE)
SET(BIN_INSTALL_DIR ${EXEC_INSTALL_PREFIX}/bin CACHE PATH "Installation prefix for user executables" FORCE)
SET(LIB_INSTALL_DIR ${EXEC_INSTALL_PREFIX}/lib${LIB_SUFFIX} CACHE PATH  "Installation prefix for object code libraries" FORCE)
SET(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation prefix for C header files" FORCE)

CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/filemon.pc.cmake ${CMAKE_CURRENT_SOURCE_DIR}/filemon.pc)

FILE(GLOB pkgconf "${CMAKE_CURRENT_SOURCE_DIR}/filemon.pc")
INSTALL(FILES ${pkgconf} DESTINATION lib/pkgconfig)
