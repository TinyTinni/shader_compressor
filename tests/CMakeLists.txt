cmake_minimum_required (VERSION 2.8.7)
project (compress-decompress-test)

#set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )
ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/single_include CACHE INTERNAL "Path to include folder for Catch.")

include_directories(${CATCH_INCLUDE_DIR} "../")
enable_testing(true)  # Enables unit-testing.


add_executable(compressor "../compress.c")
add_executable(tests "Source.cpp" "../decompress.h")

set(TEST_FILE "test_text.txt" CACHE PATH "Input Test File")

add_custom_command(TARGET tests
                   PRE_BUILD
                   COMMAND compressor "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_FILE}" "${CMAKE_CURRENT_SOURCE_DIR}/strcmprs_out.h"  
)

add_dependencies(tests catch)
add_definitions("-DSOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"")