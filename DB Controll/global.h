#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "utility.h"
#define PER_VERS "1.0.2 beta D+ Experimental"
#define PER_CONFIGURE "configuration"
#define PER_SHEET "GLOBAL\\configuration\\checkSheet.txt"
#define PER_COM "configuration\\connect.txt"
#define PER_CONFIG "configuration\\conf.txt"
#define PER_AVR "configuration\\avrDir.txt"
#define PER_AVR_STD "C:\\WinAVR-20100110\\bin"
#define PER_AVR_FLH "avrdude.exe"
#define PER_TITLE "title DB Control by Martin Tullmann Vers. "
#define PER_DEVICE "DiscBot"
#define PER_PNAME "DB Control"
#define PER_HEX "DB Control\\configuration\\dbosX.hex"
#define DIR_PROG_PATH "DB Control"
#define DIR_GLOBAL "GLOBAL"
#define DIR_BT "Bluetooth modules"
#define BT_ENDING ".BT"



extern bool cSGlobal;
char* dir_readme();
char* dir_specialThanks();
void openConf();
char* dir_HMfiles();
void buildData(char* build);

void renewConf();
void openReadMe();

void global_port_add();
void printTitle(const char* tit);
void cSopen();
void initGlobal();
bool bt_get_cmd(FILE* fj, const char* in, char* out);
bool show_bt_dir(char* filename);
char* dir_MPreset(const char* y);
