# cmake-format: off
cmake_minimum_required(VERSION 3.17.0)

if(NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Must setup the PICO SDK before the project macro call
include(cmake/pico-sdk.cmake)

project(environment-monitor C CXX ASM)
string(TIMESTAMP BUILD_TIME "%s")

###################
## CONFIGURATION ##
###################

initialize_pico()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED BUILD_VERSION)
    set(BUILD_VERSION "0.0.0+${BUILD_TIME}")
endif()

if((NOT DEFINED SERVER_IP) OR (NOT DEFINED WIRELESS_SSID))
    message(WARNING "In order to use wireless connectivity, you will need to supply SERVER_IP and SSID")
endif()

if(NOT DEFINED SERVER_PORT)
    set(SERVER_PORT 4242)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/generated/configuration.hpp.in
    ${CMAKE_BINARY_DIR}/generated/configuration.hpp
    @ONLY
)

#############
##  BUILD  ##
#############

add_executable(${PROJECT_NAME})

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
        -Wall -Werror -Wno-format -Wno-unused-function -Wno-maybe-uninitialized
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/connectivity/lwip
        ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
        src/connectivity/connection-status.cpp
        src/connectivity/wifi-connection.cpp
        src/sensors/dht.cpp

        src/main.cpp
        src/utilities.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    pico_cyw43_arch_lwip_poll
    pico_stdlib
    hardware_i2c
)
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_add_extra_outputs(${PROJECT_NAME})
# cmake-format: on
