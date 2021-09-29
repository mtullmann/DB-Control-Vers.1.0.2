#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

void print_error(const char* context);
HANDLE open_serial_port(const char* device, uint32_t baud_rate);
int write_port(HANDLE port, uint8_t* buffer, size_t size);
SSIZE_T read_port(HANDLE port, uint8_t* buffer, size_t size);
int jrk_set_target(HANDLE port, uint16_t target);
int jrk_get_variable(HANDLE port, uint8_t offset, uint8_t* buffer, uint8_t length);
int jrk_get_target(HANDLE port);
int jrk_get_feedback(HANDLE port);
