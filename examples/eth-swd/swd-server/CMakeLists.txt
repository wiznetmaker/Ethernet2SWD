set(TARGET_NAME eth-swd)

add_executable(${TARGET_NAME}
        ${TARGET_NAME}.cpp
        firmware.h
        web_page.cpp
        )

target_link_libraries(${TARGET_NAME} PRIVATE
        pico_stdlib
        hardware_spi
        hardware_dma
        swdloader
        ETHERNET_FILES
        IOLIBRARY_FILES
        HTTPSERVER_FILES
        )

pico_enable_stdio_usb(${TARGET_NAME} 1)
pico_enable_stdio_uart(${TARGET_NAME} 0)

pico_add_extra_outputs(${TARGET_NAME})
