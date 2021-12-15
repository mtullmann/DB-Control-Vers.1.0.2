#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utility.h"
#include "global.h"

extern char cSnameOfTest[201];


void cSaskIfFine();
void cSend();
void db_data_transmission_byte(double*);
void startTime();
void endTime();
void cSport(char* x);
void cScategoryEnd();
void cScError(const char* x);
void cScategory(char* x);
void cSaddMsg(const char* x);
void cScreate();
void cSenable();
void cSdisable();
