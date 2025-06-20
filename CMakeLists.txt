# CMake minimum required version
cmake_minimum_required(VERSION 3.12)
 
# Set board
#set(BOARD_NAME WIZnet_Ethernet_HAT)
# set(BOARD_NAME W5100S_EVB_PICO)
# set(BOARD_NAME W5500_EVB_PICO)
set(BOARD_NAME W55RP20_EVB_PICO)
# set(BOARD_NAME W6100_EVB_PICO)
# set(BOARD_NAME W6300_EVB_PICO)
# set(BOARD_NAME W5100S_EVB_PICO2)
# set(BOARD_NAME W5500_EVB_PICO2)
# set(BOARD_NAME W6100_EVB_PICO2)
# set(BOARD_NAME W6300_EVB_PICO2)

# Set WIZchip Clock Speed
set(WIZCHIP_SPI_SCLK_SPEED 43)
add_definitions(-D_WIZCHIP_SPI_SCLK_SPEED=${WIZCHIP_SPI_SCLK_SPEED})

if(${BOARD_NAME} STREQUAL WIZnet_Ethernet_HAT)
    #set(PICO_PLATFORM rp2350) #If you are using WIZnet_ethernet_HAT with Pico2, uncomment it.
    set(WIZNET_CHIP W5100S)
    add_definitions(-D_WIZCHIP_=W5100S)
    add_definitions(-DDEVICE_BOARD_NAME=WIZnet_Ethernet_HAT)
    
elseif(${BOARD_NAME} STREQUAL W5100S_EVB_PICO)
    set(WIZNET_CHIP W5100S)
    add_definitions(-D_WIZCHIP_=W5100S)
    add_definitions(-DDEVICE_BOARD_NAME=W5100S_EVB_PICO)
    
elseif(${BOARD_NAME} STREQUAL W5500_EVB_PICO)
    set(WIZNET_CHIP W5500)
    add_definitions(-D_WIZCHIP_=W5500)
    add_definitions(-DDEVICE_BOARD_NAME=W5500_EVB_PICO)

elseif(${BOARD_NAME} STREQUAL W55RP20_EVB_PICO)
    set(WIZNET_CHIP W5500)
    add_definitions(-D_WIZCHIP_=W5500)
    add_definitions(-DDEVICE_BOARD_NAME=W55RP20_EVB_PICO)

elseif(${BOARD_NAME} STREQUAL W6100_EVB_PICO)
    set(WIZNET_CHIP W6100)
    add_definitions(-D_WIZCHIP_=W6100)
    add_definitions(-DDEVICE_BOARD_NAME=W6100_EVB_PICO)

elseif(${BOARD_NAME} STREQUAL W6300_EVB_PICO)
    set(WIZNET_CHIP W6300)
    add_definitions(-D_WIZCHIP_=W6300)
    add_definitions(-DDEVICE_BOARD_NAME=W6300_EVB_PICO)
# Set QSPI MODE for W6300
    set(WIZCHIP_QSPI_MODE QSPI_QUAD_MODE)
    # set(WIZCHIP_QSPI_MODE QSPI_DUAL_MODE)
    # set(WIZCHIP_QSPI_MODE QSPI_SINGLE_MODE)
    
    add_definitions(-D_WIZCHIP_QSPI_MODE_=${WIZCHIP_QSPI_MODE})

elseif(${BOARD_NAME} STREQUAL W5100S_EVB_PICO2)
    set(PICO_PLATFORM rp2350)
    set(WIZNET_CHIP W5100S)
    add_definitions(-D_WIZCHIP_=W5100S)
    add_definitions(-DDEVICE_BOARD_NAME=W5100S_EVB_PICO2)
    
elseif(${BOARD_NAME} STREQUAL W5500_EVB_PICO2)
    set(PICO_PLATFORM rp2350)
    set(WIZNET_CHIP W5500)
    add_definitions(-D_WIZCHIP_=W5500)
    add_definitions(-DDEVICE_BOARD_NAME=W5500_EVB_PICO2)

elseif(${BOARD_NAME} STREQUAL W6100_EVB_PICO2)
    set(PICO_PLATFORM rp2350)
    set(WIZNET_CHIP W6100)
    add_definitions(-D_WIZCHIP_=W6100)
    add_definitions(-DDEVICE_BOARD_NAME=W6100_EVB_PICO2)

elseif(${BOARD_NAME} STREQUAL W6300_EVB_PICO2)
    set(PICO_PLATFORM rp2350)
    set(WIZNET_CHIP W6300)
    add_definitions(-D_WIZCHIP_=W6300)
    add_definitions(-DDEVICE_BOARD_NAME=W6300_EVB_PICO2)
# Set QSPI MODE for W6300
    # Set QSPI MODE for W6300
    set(WIZCHIP_QSPI_MODE QSPI_QUAD_MODE)
    # set(WIZCHIP_QSPI_MODE QSPI_DUAL_MODE)
    # set(WIZCHIP_QSPI_MODE QSPI_SINGLE_MODE)
    
    add_definitions(-D_WIZCHIP_QSPI_MODE_=${WIZCHIP_QSPI_MODE})

else() 
    message(FATAL_ERROR "BOARD_NAME is wrong = ${BOARD_NAME}")
endif()

if(DEFINED WIZCHIP_QSPI_MODE AND WIZCHIP_QSPI_MODE STREQUAL "QSPI_QUAD_MODE" AND WIZCHIP_SPI_SCLK_SPEED GREATER_EQUAL 25)
    add_compile_options(-O0)
    message(STATUS "Compiler optimization disabled (W6300 + QSPI_QUAD_MODE + ${WIZCHIP_SPI_SCLK_SPEED} MHz)")
endif()

message(STATUS "WIZNET_CHIP = ${WIZNET_CHIP}")


if(NOT DEFINED PICO_SDK_PATH)
    set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/libraries/pico-sdk)
    message(STATUS "PICO_SDK_PATH = ${PICO_SDK_PATH}")
endif()

include(pico_sdk_import.cmake)
include(wiznet_pico_c_sdk_version.cmake)

# Set project name
set(PROJECT_NAME WIZNET-PICO-C)

# Set project informations
project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Set the project root directory if it's not already defined, as may happen if
# the tests folder is included directly by a parent project, without including
# the top level CMakeLists.txt.
if(NOT DEFINED WIZNET_DIR)
    set(WIZNET_DIR ${CMAKE_SOURCE_DIR}/libraries/ioLibrary_Driver)
    message(STATUS "WIZNET_DIR = ${WIZNET_DIR}")
endif()

if(NOT DEFINED PORT_DIR)
    set(PORT_DIR ${CMAKE_SOURCE_DIR}/port)
    message(STATUS "PORT_DIR = ${PORT_DIR}")
endif()


# Hardware-specific examples in subdirectories:
add_subdirectory(examples)

# Add libraries in subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/libraries)

add_subdirectory(${PORT_DIR})

# Set compile options
add_compile_options(
        -Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )
