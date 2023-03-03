cmake_minimum_required(VERSION 3.17.0)

# Must setup the PICO SDK before the project macro call
include(cmake/pico-sdk.cmake)

project(environment-monitor C CXX ASM)
string(TIMESTAMP BUILD_TIME "%s")

if(NOT DEFINED BUILD_VERSION)
  set(BUILD_VERSION "0.0.0+${BUILD_TIME}")
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL Debug)
  set(CMAKE_BUILD_TYPE Release)
endif()

# ##############################################################################
# CONFIGURATION ##
# ##############################################################################

initialize_pico()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/generated/configuration.hpp.in
               ${CMAKE_BINARY_DIR}/generated/configuration.hpp @ONLY)

# ##############################################################################
# BUILD  ##
# ##############################################################################

add_executable(${PROJECT_NAME})

target_compile_options(
  ${PROJECT_NAME} PRIVATE -Wall -Werror -Wno-format -Wno-unused-function
                          -Wno-maybe-uninitialized)

target_include_directories(
  ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
                          ${CMAKE_CURRENT_SOURCE_DIR}/build)

target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c)

pico_add_extra_outputs(${PROJECT_NAME})
