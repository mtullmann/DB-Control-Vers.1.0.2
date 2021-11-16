#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include "dirent.h"
#include "music.h"
#define ae 132
#define gae 142
#define oe 148
#define goe 153
#define ue 'z' + 7
#define gue 154
#define ss 225
#define ton 7
#define SCHWARZ 0
#define BLAUd 1
#define GRUENd 2
#define TURKEYSEd 3
#define ROTd 4
#define VIOLETTd 5
#define GELBd 6
#define WEISSd 7
#define GRAU 8
#define BLAUh 9
#define GRUENh 10
#define TURKEYSEh 11
#define ROTh 12
#define VIOLETTh 13
#define GELBh 14
#define WEISSh 15
#define DEGREE 248
#define ablechint(g) (g<'0'||g>'9')
#define GROSS(a) ((a>='A')&&(a<='Z'))
#define KLEIN(a) ((a>='a')&&(a<='z'))
#define GROSS_2_KLEIN(a) a+GROSS(a)*('a'-'A')
#define BELL 7

//#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define HINTERGRUND(hintergrundFarbe,farbe) 16*hintergrundFarbe+farbe
#define rcol 15
#define rcolor color(rcol)

extern struct DbLsNote dbLs_song[];
extern struct DbLsNote dbLs_song1[];
extern struct DbLsNote dbLs_siren[];
extern struct DbLsNote dbLs_beep[];
extern struct DbLsNote dbLs_bigben[];
extern struct DbLsNote dbLs_ryb[];
extern struct DbLsNote dbLs_avdm[];
extern struct DbLsNote dbLs_slpda[];
extern struct DbLsNote dbLs_yankee[];
extern struct DbLsNote dbLs_kw[];
extern struct DbLsNote dbLs_sm[];

extern struct DbLsNote dbLs_coffee[];
extern struct DbLsNote dbLs_FF_tokyo[];
#define MaxSongs 32

extern struct DbLsNote* dbLs_ALL[MaxSongs];

void wait(int tim);
bool anyKey();
void flush();
bool waitS(int tim, bool start);
void color(int);
double percent(int max, int current);
void flushW();
bool bufEmty();
char ynrespond();
void bell();
int int_inp();
int int_inp(int from, int to);
void str_inp(char* text, int size);
bool str_inp(FILE* f, char* text, int size);

bool generateDir(const char* name);
bool generateDir(const char* name, DIR** dir);
bool fileExist(const char* name);
bool fileExist(const char* name, FILE* f);
void printx(char c, int x);
void printx(char c, const char* x);
void printxCx(char c, const  char* x);
void printxCx(char c, const char* x, int c1, int c2, int c3);
void percentGraph(double percent, const char* format, bool colorx, int steps);
