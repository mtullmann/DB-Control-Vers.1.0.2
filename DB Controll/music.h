#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#define STD_LOUD 0

#define DB_LS_LAST  0              ///< used to indicate the last tone of a song
#define DB_LS_PAUSE  1              ///< used for a pause
#define DB_LS_H0  (31)
#define DB_LS_C1  (33)
#define DB_LS_CS1  (35)
#define DB_LS_D1  (37)
#define DB_LS_DS1  (39)
#define DB_LS_E1  (41)
#define DB_LS_F1  (44)
#define DB_LS_FS1  (46)
#define DB_LS_G1  (49)
#define DB_LS_GS1  (52)
#define DB_LS_A1  (55)
#define DB_LS_AS1  (58)
#define DB_LS_H1  (62)
#define DB_LS_C2  (65)
#define DB_LS_CS2  (69)
#define DB_LS_D2  (73)
#define DB_LS_DS2  (78)
#define DB_LS_E2  (82)
#define DB_LS_F2  (87)
#define DB_LS_FS2  (93)
#define DB_LS_G2  (98)
#define DB_LS_GS2  (104)
#define DB_LS_A2  (110)
#define DB_LS_AS2  (117)
#define DB_LS_H2  (123)
#define DB_LS_C3  (131)
#define DB_LS_CS3  (139)
#define DB_LS_D3  (147)
#define DB_LS_DS3  (156)
#define DB_LS_E3  (165)
#define DB_LS_F3  (175)
#define DB_LS_FS3  (185)
#define DB_LS_G3  (196)
#define DB_LS_GS3  (208)
#define DB_LS_A3  (220)
#define DB_LS_AS3  (233)
#define DB_LS_H3  (247)
#define DB_LS_C4  (262)
#define DB_LS_CS4  (277)
#define DB_LS_D4  (294)
#define DB_LS_DS4  (311)
#define DB_LS_E4  (330)
#define DB_LS_F4  (349)
#define DB_LS_FS4  (370)
#define DB_LS_G4  (392)
#define DB_LS_GS4  (415)
#define DB_LS_A4  (440)
#define DB_LS_AS4  (466)
#define DB_LS_H4  (494)
#define DB_LS_C5  (523)
#define DB_LS_CS5  (554)
#define DB_LS_D5  (587)
#define DB_LS_DS5  (622)
#define DB_LS_E5  (659)
#define DB_LS_F5  (698)
#define DB_LS_FS5  (740)
#define DB_LS_G5  (784)
#define DB_LS_GS5  (831)
#define DB_LS_A5  (880)
#define DB_LS_AS5  (932)
#define DB_LS_H5  (988)
#define DB_LS_C6  (1047)
#define DB_LS_CS6  (1109)
#define DB_LS_D6  (1175)
#define DB_LS_DS6  (1245)
#define DB_LS_E6  (1319)
#define DB_LS_F6  (1397)
#define DB_LS_FS6  (1480)
#define DB_LS_G6  (1568)
#define DB_LS_GS6  (1661)
#define DB_LS_A6  (1760)
#define DB_LS_AS6  (1865)
#define DB_LS_H6  (1976)
#define DB_LS_C7  (2093)
#define DB_LS_CS7  (2217)
#define DB_LS_D7  (2349)
#define DB_LS_DS7  (2489)
#define DB_LS_E7  (2637)
#define DB_LS_F7  (2794)
#define DB_LS_FS7  (2960)
#define DB_LS_G7  (3136)
#define DB_LS_GS7  (3322)
#define DB_LS_A7  (3520)
#define DB_LS_AS7  (3729)
#define DB_LS_H7  (3951)
#define DB_LS_C8  (4186)
#define DB_LS_CS8  (4435)
#define DB_LS_D8  (4699)
#define DB_LS_DS8  (4978)

#define lengthMUL 1/*
enum DbLsLength
{
	DB_LS_1_1 = (int)(341 * 4 * lengthMUL),       ///< whole note
	DB_LS_1_2 = (int)(341 * 2 * lengthMUL),       ///< half note
	DB_LS_1_3 = (int)(341 * 1.5 * lengthMUL),      ///< half note
	DB_LS_1_4 = (int)(341 * lengthMUL),        ///< quarter note
	DB_LS_1_8 = (int)(170 * lengthMUL),        ///< eighth note
	DB_LS_1_9 = (int)(170 / 2 * lengthMUL),        ///< eighth note
	DB_LS_1_12 = (int)(170/3 * lengthMUL),        ///< eighth note
	DB_LS_1_16 = (int)(85 * lengthMUL),       ///< sixteenth note
	DB_LS_1_32 = (int)(85/2 * lengthMUL),       ///< sixteenth note
	DB_LS_10_SEC = 1000 * 10,   ///< one second
	DB_LS_5_SEC = 1000 * 5,    ///< one second
	DB_LS_3_SEC = 1000 * 3,     ///< one second
	DB_LS_2_SEC = 1000 * 2,     ///< one second
	DB_LS_1_SEC = 1000 * 1     ///< one second
};*/
enum DbLsLength
{
	DB_LS_1_1 = 0,       ///< whole note
	DB_LS_1_2 = 1,       ///< half note
	DB_LS_1_3 = 2,      ///< third note
	DB_LS_1_4 = 3,        ///< quarter note
	DB_LS_1_5 = 4,        ///< quarter note
	DB_LS_1_6 = 5,        ///< quarter note
	DB_LS_1_8 = 6,        ///< eighth note
	DB_LS_1_9 = 7,      ///< eighth note
	DB_LS_1_10 = 8,      ///< eighth note
	DB_LS_1_12 = 9,        ///< eighth note
	DB_LS_1_16 = 10,       ///< sixteenth note
	DB_LS_1_18 = 11,       ///< sixteenth note
	DB_LS_1_32 = 12,       ///< 32 note
	DB_LS_10_SEC = 13,   ///< one second
	DB_LS_5_SEC = 14,    ///< one second
	DB_LS_3_SEC = 15,     ///< one second
	DB_LS_2_SEC = 16,     ///< one second
	DB_LS_1_SEC = 17,     ///< one second
	DB_LS_4_3 = 18     /// quarter triol
};
const uint8_t notLenDB[] = {

	 47, // DB_LS_1_1     ///< whole note
	  23,     // DB_LS_1_2 ///< half note
	  17,    // DB_LS_1_3 ///< third note
	  11,      // DB_LS_1_4 ///< quarter note
	  (uint16_t)(11 / 1.7),      // DB_LS_1_5 ///< quarter note
	  (uint16_t)(11 / 1.9),     // DB_LS_1_6  ///< quarter note
	  5,     // DB_LS_1_8  ///< eighth note
	  4,     // DB_LS_1_9///< eighth note
	  (uint16_t)(4 / 1.7),     // DB_LS_1_10///< eighth note
	  3,      // DB_LS_1_12 ///< eighth note
	  2,     // DB_LS_1_16 ///< sixteenth note
	  (uint16_t)(2 / 1.7),      //DB_LS_1_18 ///< sixteenth note
	  1,       //DB_LS_1_32///< 32 note
	  200,  // DB_LS_10_SEC///< one second
	  100,  // DB_LS_5_SEC ///< one second
	  60,   //  DB_LS_3_SEC///< one second
	  40,   // DB_LS_2_SEC ///< one second
	  20,   // DB_LS_1_SEC ///< one second
	 4     /// quarter triol
};
const int notLenC[] = {
	(int)(341 * 4 * lengthMUL), //C_LS_1_1///< whole note
	(int)(341 * 2 * lengthMUL), //C_LS_1_2 ///< half note
	(int)(341 * 1.5 * lengthMUL), //C_LS_1_3///< half note
	(int)(341 * lengthMUL),// C_LS_1_4///< quarter note
	(int)((341 / 1.5) * lengthMUL), //C_LS_1_5 ///< quarter note
	(int)((341 / 2) * lengthMUL), //C_LS_1_6 ///< quarter note
	(int)(170 * lengthMUL), //C_LS_1_8 ///< eighth note
	(int)((170 / 1.5) * lengthMUL),// C_LS_1_9 ///< eighth note
	(int)((170 / 2) * lengthMUL),// C_LS_1_10///< eighth note
	(int)((170 / 4) * lengthMUL), //C_LS_1_12///< eighth note
	(int)((1000 / 16) * lengthMUL), //C_LS_1_16 ///< eighth note
	(int)((1000 / 18) * lengthMUL),// C_LS_1_18///< eighth note
	(int)((1000 / 32) * lengthMUL), //C_LS_1_32///< sixteenth note
	1000 * 10,// C_LS_10_SEC///< one second
	1000 * 5,// C_LS_5_SEC///< one second
	1000 * 3, //C_LS_3_SEC///< one second
	1000 * 2, //C_LS_2_SEC///< one second
	1000 * 1     //C_LS_1_SEC///< one second
};
struct DbLsNote
{
	uint16_t  pitch;      ///< the pitch of the note
	uint8_t length;     ///< the length of the note
	uint8_t  loud;
};
void playSong(struct DbLsNote* pNotes);
long int sonlen(struct DbLsNote* pNotes);
long int sonlenDB(struct DbLsNote* pNotes);
