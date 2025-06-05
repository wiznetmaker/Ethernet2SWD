
#include "port_common.h"
#include "httpParser.h"
#include "http_fwup.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern uint8_t *pHTTP_RX;
extern uint8_t *pHTTP_TX;

#define BUFFER_SIZE (30*1024)

extern bool swdloader_flash_buffer(const uint8_t* buffer, size_t size);

uint8_t http_update_firmware(st_http_request * p_http_request, uint8_t *buf)
{
  int sock = p_http_request->socket;
  uint8_t *uri = p_http_request->URI;
  
  
    // Parse boundary from URI
    uint8_t boundary[128] = {0};
    uint8_t boundary_len = 0;
    char *boundary_pos = strstr((char *)uri, "boundary=");
    if (boundary_pos) {
        sscanf(boundary_pos, "boundary=%127s", boundary);
        boundary_len = strlen((char *)boundary);
        printf("boundary = %s, boundary_len = %d\n", boundary, boundary_len);
    } else {
        printf("Boundary not found in URI.\n");
        return 0;
    }

    // Allocate upload buffer
    uint8_t *upload_buf = malloc(BUFFER_SIZE);
    if (!upload_buf) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    // Copy initial received data
    int total_len = p_http_request->recv_len;
    memcpy(upload_buf, pHTTP_RX, total_len);

    // Receive remaining data until socket is done
int retry_count = 0;
const int max_retries = 50;

while (total_len < BUFFER_SIZE && retry_count < max_retries) {
    int rx_ready = getSn_RX_RSR(sock);
    if (rx_ready > 0) {
        if ((total_len + rx_ready) >= BUFFER_SIZE) {
            printf("Firmware too large.\n");
            free(upload_buf);
            return 0;
        }

        int rx_len = recv(sock, upload_buf + total_len, rx_ready);
        total_len += rx_len;
        retry_count = 0;  // reset on successful recv
    } else {
        retry_count++;
        sleep_ms(100);  // short delay to wait for more data
    }
}
    printf("Total len = %d\r\n",total_len);
    // Find header end (\r\n\r\n)
    char *header_end = NULL;
    for (int i = 0; i < total_len - 3; i++) {
        if (upload_buf[i] == '\r' && upload_buf[i+1] == '\n' &&
            upload_buf[i+2] == '\r' && upload_buf[i+3] == '\n') {
            header_end = (char *)&upload_buf[i];
            int offset = header_end - (char *)upload_buf;
            printf("Header end found at offset: %d\n", offset);
            break;
        }
    }

    if (!header_end) {
        printf("Header end not found.\n");
        free(upload_buf);
        return 0;
    }

    char *multipart_start = header_end + 4;
    char *firmware_start = strstr(multipart_start, "\r\n\r\n");
    if (!firmware_start) {
        printf("Inner multipart header not found.\n");
        free(upload_buf);
        return 0;
    }
    firmware_start += 4;
    int body_len = total_len - (firmware_start - (char *)upload_buf);
    //int body_len = total_len - (body_start - (char *)upload_buf);
    printf("body_len=%d\n", body_len);
    // Copy only the body to firmware buffer
    uint8_t *flash_temp_buf = malloc(BUFFER_SIZE);
    if (!flash_temp_buf) {
        printf("Firmware buffer allocation failed.\n");
        free(upload_buf);
        return 0;
    }

    memcpy(flash_temp_buf, firmware_start, body_len);
    int firmware_len = body_len;

    char full_boundary[140];
    snprintf(full_boundary, sizeof(full_boundary), "--%s--", boundary);
    int full_boundary_len = strlen(full_boundary);

    char *last_boundary = NULL;
    for (int i = firmware_len - full_boundary_len; i >= 0; i--) {
        if (memcmp(flash_temp_buf + i, full_boundary, full_boundary_len) == 0) {
            last_boundary = (char *)(flash_temp_buf + i);
            printf("Last boundary match at offset %d\n", i);
            break;
        }
    }

    if (last_boundary) {
    firmware_len = (uint8_t *)last_boundary - flash_temp_buf;

    // Trim trailing CRLF
    while (firmware_len > 0 &&
          (flash_temp_buf[firmware_len - 1] == '\r' || flash_temp_buf[firmware_len - 1] == '\n')) {
        firmware_len--;
    }
    }

    printf("Final firmware size: %d bytes\n", firmware_len);
    for (int i = 0; i < firmware_len && i < 16; i++) {
        printf("%02X ", flash_temp_buf[i]);
    }
    printf("\n");

    int padded_len = (firmware_len + 3) & ~3;  // round up to nearest multiple of 4

    // Pad trailing bytes with 0xFF (safe for flash or RAM)
    for (int i = firmware_len; i < padded_len; i++) {
        flash_temp_buf[i] = 0xFF;
    }

    // Flash to target MCU
    extern bool swdloader_flash_buffer(const uint8_t *buf, size_t len);
    bool success = swdloader_flash_buffer(flash_temp_buf, padded_len);

    free(upload_buf);
    free(flash_temp_buf);
    return success ? 1 : 0;
}