#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "test_serial.h"
#include "utility.h"
#include <iostream>
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
#include "sheet.h"
#include "global.h"

extern bool mcOperateOffline;

void reset();
bool manager_offline();
void generate_Files();
bool simpleExample();
bool manager(char* ex);
void endofP();
void initTest();
bool cSopenVisual(int argc, char* argv[]);
