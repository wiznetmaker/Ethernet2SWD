# MCU dependency files
add_library(MCU_FILES STATIC)

target_link_libraries(MCU_FILES PUBLIC
        cmsis_core
        pico_stdlib
        pico_stdlib_headers
        hardware_clocks
        hardware_dma
        hardware_flash
        hardware_resets
        hardware_spi
        hardware_structs
        )

# ioLibrary_Driver
add_library(IOLIBRARY_FILES STATIC)

target_sources(IOLIBRARY_FILES PUBLIC
        ${PORT_DIR}/ioLibrary_Driver/src/wizchip_spi.c
        ${PORT_DIR}/ioLibrary_Driver/src/wizchip_gpio_irq.c
        )

if(${BOARD_NAME} STREQUAL W55RP20_EVB_PICO OR ${WIZNET_CHIP} STREQUAL W6300)
pico_generate_pio_header(IOLIBRARY_FILES ${PORT_DIR}/ioLibrary_Driver/src/wizchip_qspi_pio.pio)

target_sources(IOLIBRARY_FILES PUBLIC 
        ${PORT_DIR}/ioLibrary_Driver/src/wizchip_qspi_pio.c
)

target_include_directories(IOLIBRARY_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${PORT_DIR}/ioLibrary_Driver/inc
        ${PORT_DIR}
)
endif()

target_include_directories(IOLIBRARY_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${PORT_DIR}/ioLibrary_Driver/inc
        ${PORT_DIR}
        )

target_link_libraries(IOLIBRARY_FILES PUBLIC
        pico_stdlib
        hardware_pio
        hardware_spi
        hardware_dma
        hardware_clocks
        )

target_link_libraries(IOLIBRARY_FILES PRIVATE
        MCU_FILES
        ETHERNET_FILES
        )

# timer
add_library(TIMER_FILES STATIC)

target_sources(TIMER_FILES PUBLIC
        ${PORT_DIR}/timer/timer.c
        )

target_include_directories(TIMER_FILES PUBLIC
        ${PORT_DIR}/timer
        )

target_link_libraries(TIMER_FILES PRIVATE
        pico_stdlib      
        )

# HTTP_SERVER
add_library(HTTPSERVER_FILES STATIC)

target_sources(HTTPSERVER_FILES PUBLIC
        ${PORT_DIR}/http_server/src/http_fwup.c
        ${PORT_DIR}/http_server/src/httpParser.c
        ${PORT_DIR}/http_server/src/httpServer.c
        ${PORT_DIR}/http_server/src/httpUtil.c
        ${PORT_DIR}/http_server/src/swd-interface.cpp
        )

target_include_directories(HTTPSERVER_FILES PUBLIC
        ${PORT_DIR}/http_server/inc
        ${PORT_DIR}
        )
        
target_link_libraries(HTTPSERVER_FILES PUBLIC
        MCU_FILES
        IOLIBRARY_FILES
        swdloader
        )
