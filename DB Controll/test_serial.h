#pragma once
#include "serial.h"
#include "utility.h"

#define DB_MAXLENGTH 255

#define DB_UNKNOWN 0x07
#define DB_CLEAR 0x08
#define DB_RESET 0x17
#define DB_LED 0x40
#define DB_LOUD 0x49
#define DB_BTN 0x32
#define DB_RFID 0x35
#define DB_CS 0x62
#define DB_LS 0x51
#define DB_LSS 0x52 //LS STOP
#define DB_BM 0x48
#define DB_IRS 0x30
#define DB_USS_M 0x33
#define DB_USS_G 0x34
#define DB_BT 0x18
#define DB_BT_OS 0x19
#define DB_MC 0x47
#define DB_MC_D 0x46
#define DB_BUFFER_C 0x20
#define DB_BUFFER_x 0x21
#define DB_EEPROM 0x22
#define DB_COPY_SONG 0x53
#define DB_CAT_SONG 0x54
#define DB_IRC_BUFFER 0x23
#define DB_IRC_SEND 0x55

extern HANDLE db_port;
extern bool mcDebug;

bool db_reset();
bool db_cmd(uint8_t cmd, uint8_t lenIn, uint8_t* parameterIn, uint8_t* parameterOut);
void db_data_transmission_byte(double* x);