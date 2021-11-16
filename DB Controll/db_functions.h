#pragma once

#include "DB_serial.h"
#include "utility.h"
#include <iostream>
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
#include "sheet.h"

#define DB_MC_TICKS_PER_TURN       468
#define DB_MC_WHEEL_CIRCUMFERENCE  217
uint8_t sysloud = 10;
uint16_t db_circumference_value = DB_MC_WHEEL_CIRCUMFERENCE;
char mc_notes[201];
bool dbLs_setLoud(uint8_t loud);
bool dbLs_getLoud(uint8_t* loud);
bool db_Beep() {
	uint8_t loud = 0;
	if (!dbLs_getLoud(&loud))
		return 0;
	if (!dbLs_setLoud(sysloud))
		return 0;
	uint8_t in[DB_MAXLENGTH] = { 0 };
	in[0] = 0;
	in[1] = 0;
	in[2] = 0;
	if (!db_cmd(DB_LS, 3, in, NULL)) {
		return 0;
	}
	wait(300);
	return dbLs_setLoud(loud);
}
bool db_Siren() {
	uint8_t loud = 0;
	if (!dbLs_getLoud(&loud))
		return 0;
	if (!dbLs_setLoud(sysloud))
		return 0;
	uint8_t in[DB_MAXLENGTH] = { 0 };
	in[0] = 1;
	in[1] = 0;
	in[2] = 0;
	if (!db_cmd(DB_LS, 3, in, NULL)) {
		return 0;
	}
	wait(300);
	return dbLs_setLoud(loud);
}

bool bt_os(bool val);
void exitS() {
	color(TURKEYSEh);
	printf("Push 'e' to exit this sketch!!\n");
	rcolor;
	while ('e' != _getch());
}
bool db_load_song_to_bot(uint8_t num);
bool bt_get_cmd(FILE*f,const char* in, char* out);
enum DB_LED_ID
{
	DB_LED_FRONT_RIGHT_OUT = 0,
	DB_LED_FRONT_RIGHT_IN = 1,
	DB_LED_FRONT_LEFT_IN = 2,
	DB_LED_FRONT_LEFT_OUT = 3,

	DB_LED_BUTTON_BLUE = 4,
	DB_LED_BUTTON_GREEN = 5,
	DB_LED_BUTTON_RED = 6,

	DB_LED_BACK_LEFT_OUT = 7,
	DB_LED_BACK_LEFT_IN = 8,
	DB_LED_BACK_RIGHT_IN = 9,
	DB_LED_BACK_RIGHT_OUT = 10
};
bool dbLed_setColor(uint8_t led, uint8_t r, uint8_t g, uint8_t b);
bool db_clearBuffer() {
	uint8_t paramin[10] = { 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00 };
	printf("Initialisyse direct buffer clearing (Chip and PC)! (For secure connection and safty!)\n");
	if (db_cmd(DB_BUFFER_C, 10, paramin, NULL)) {
		color(GRUENh);
		printf("Buffer cleared!\n");
		rcolor; return 1;
	}
	else {
		color(ROTh);
		printf("Fail clearing buffer\n");
		rcolor;
		return 0;
	}
}
int db_unknown() {
	static uint8_t first = 1;
	uint8_t i1[DB_MAXLENGTH];
	int lps = 0;
	int lps0 = 0;
	//printf("This is a test if we get a usefully commands from Discbot!\nNow we should get an unknown request\n");
	do {

		lps0++;
		db_cmd(DB_UNKNOWN, 0, NULL, i1);
		if ((i1[1] != 0) || (i1[0] != 0)) {
			lps++;
			color(ROTh);
		}
		else
			color(GELBh);


		printf("\rEstablish a secure connection %03.2f%%", (float)percent(80, lps));

		wait(25);

		if (lps0 == 80) {
			color(GRUENh);

			printf("\rEstablish a secure connection %03.2f%%", (float)percent(80, lps));
			rcolor;
			if (first) {
				color(ROTh);
				printf("\nMake sure your COM port is right, flash the %s and restart the program!\n", PER_DEVICE);
				bell();
				color(rcol);
				first = 0;
				return -1;
			}
			else {
				color(ROTh);
				printf("\nDiscBot malefunction!!\nPlease do a hardware reset manually!!!\nThan push any key!\n");
				printf("%c", BELL);
				color(rcol);
				_getch();
			}
			first = 0;
			wait(500);
		}
	} while (lps0 != 80 && (i1[1] != 0 || i1[0] != 0));
	if (lps0 != lps)
		color(GRUENh);
	else
		color(ROTh);
	printf("\rEstablish a secure connection %03.2f%%", (float)percent(80, 80));
	rcolor;
	printf("\n");
	if (lps == 80) {
		color(ROTh);
		printf("\tThe connection is not ready! (%i loops)\n", lps);
		color(rcol);
	}
	else {
		color(GRUENh);
		printf("\tThe connection is now ready! (%i loops)\n", lps);
		color(rcol);
	}
	//printf("\n");
	if (lps == 0)
		return 1;
	return 0;
}
void reset() {
	printf("Initialisyse Hardware reset!\n");
	db_cmd(DB_RESET, 0, NULL, NULL);
	waitS(2000, 1);
	int i = 2;
	while (!waitS(2000, 0)) {
		printf("\rWait %i seconds", i);
		wait(1000);
		i--;
	}
	printf("\r");
	color(GRUENh);
	printf("Hardware reset done!\n");
	color(rcol);
}
bool db_reset() {
	db_clearBuffer();
	reset();
	uint8_t kl = 0;
	//db_cmd(DB_CLEAR, 0, NULL, NULL);
	for (uint8_t i = 0; kl != 3; i++) {
		switch (db_unknown()) {
		case 1:kl++; break;
		case 0:kl = 0; break;
		case -1:return 0; break;
		default:return 0; break;
		}
	}
	db_clearBuffer();
	return 1;
}
bool db_ledOnOff() {
	printTitle(" - LED_On_Off test");
	color(GELBh);
	printf("Start LED_On_Off\n");
	rcolor;
	bool ret = 1;
	printf("All LED's are flashing in same colors!\n");
	printf("Do you want to change the bink delay (100 ms) ? (y/n)\n");
	int del = 100;
	if (ynrespond() == 'y') {
		printf("Put in the number of milliseconds yuo want to wait, then push enter: ");
		do {
			if (del < 0 || del>999999999) {
				printf("invalid number please try again! :");
			}
			del = int_inp();
			flush();

		} while (del < 0 || del>999999999);
	}
	color(GELBh);
	printf("Press any key to start!\n");
	printf("Press enter to stop!\n");
	rcolor;
	_getch();
	uint8_t param[100];
	uint8_t ln = 0;
	uint8_t inOn[] = { 0, 255, 255, 255 };
	uint8_t inOff[] = { 0, 0, 0, 0 };
	uint8_t col = 0;
	bool en = 0;
	//char colo[6] = "white";
	//flush();
	while (anyKey());
	do {
		if ((col % 30) == 0) {
			waitS(300, 1);
			while (!waitS(300, 0) && en == 0) {
				en |= anyKey();
			}
		}

		printf("\nON : ");
		switch (col % 4) {
		case 0:inOn[1] = 255; inOn[2] = 255; inOn[3] = 255; rcolor; printf("white"); break;
		case 1:inOn[1] = 255; inOn[2] = 0; inOn[3] = 0; color(ROTh); printf("red"); rcolor; break;
		case 2:inOn[1] = 0; inOn[2] = 255; inOn[3] = 0; color(GRUENh); printf("green"); rcolor; break;
		case 3:inOn[1] = 0; inOn[2] = 0; inOn[3] = 255; color(BLAUh); printf("blue"); rcolor; break;
		}
		//printf(colo);
		col++;
		printf("\n");
		for (uint8_t i = 0; i != 11; i++) {
			inOn[0] = i;
			//printf("\tLED %i", i);
			if (!db_cmd(DB_LED, 4, inOn, param)) {
				en = 1;
			}
			if (param[0] != DB_LED || param[1] != 0) {
				color(ROTh);
				printf("Callback ERROR by Discbot!");
				rcolor;
				return  0;
			}
			//wait(25);
			if (i == 10)
				color(GRUENh);
			else
				color(GELBh);
			printf("\r Executation status: %4.2f%%", (float)percent(10, i));
			rcolor;
			en |= anyKey();
		}
		waitS(del, 1);
		while (!waitS(del, 0) && en == 0) {
			en |= anyKey();
		}

		//printf("\n");
		printf("\tOFF");
		for (uint8_t i = 0; i != 11; i++) {
			//printf("\tLED %i", i);
			inOff[0] = i;
			if (!db_cmd(DB_LED, 4, inOff, param)) {
				en = 1;
			}

			//wait(25);
			en |= anyKey();
		}
		waitS(del, 1);
		while (!waitS(del, 0) && en == 0) {
			en |= anyKey();
		}
		//printf("\n");
	} while (!en);
	color(GELBd);
	printf("\nEnd LED_On_Off\n");
	rcolor;
	flush();
	return ret;
}

bool db_Tbtn() {
	uint8_t ou[21];
	char s[51] = "";
	char col[3][21] = { "red","green","blue" };
	//char gcol[3][21] = { "Red","Green","Blue" };
	printf("Switch all switches to uC and do not push any buttons!\nThen push any key\n");
	_getch();
	db_cmd(DB_BTN, 0, NULL, ou);
	//printf("ou=%i\n", ou[2]);
	if (ou[2] == 0) {
		printf("No button was pushed\n");
		db_Beep();
		//cScError("No button was pushed\n");
	}
	else {
		for (uint8_t i = 0; i != 3; i++) {
			if ((ou[2] & (1 << i))) {
				printf("The ");
				switch (i) {
				case 0:color(ROTh); break;
				case 1:color(GRUENh); break;
				case 2:color(BLAUh); break;
				}
				printf(col[i]);
				color(rcol);
				printf(" button was pushed instead of none!\n");
				sprintf(s, "The button %s was pushed instead of none!\n", col[i]);
				cScError(s);
				db_Siren();
			}
		}
	}
	bool f = 0;
	for (uint8_t oi = 0; oi != 3; oi++) {
		printf("Push the ");
		switch (oi) {
		case 0:color(ROTh); dbLed_setColor(6, 255, 0, 0); break;
		case 1:color(GRUENh); dbLed_setColor(6, 0, 0, 0);  dbLed_setColor(5, 0, 255, 0); break;
		case 2:color(BLAUh); dbLed_setColor(5, 0, 0, 0); dbLed_setColor(4, 0, 0, 255); break;
		}
		printf(col[oi]);
		color(rcol);
		printf(" button, if nothing is changeing push enter and the %s button!\n", col[oi]);
		do {
			if (!db_cmd(DB_BTN, 0, NULL, ou))
				return 0;
			wait(100);
		} while (!(f = anyKey()) && !(ou[2] & (1 << oi)));
		dbLed_setColor(6, 0, 0, 0);
		dbLed_setColor(5, 0, 0, 0);
		dbLed_setColor(4, 0, 0, 0);
		while (f)
		{
			f = 0;
			wait(500);
			while ('\n' == _getch()) {
				//wait(50);
			}
		}
		if (anyKey()) {
			color(VIOLETTh);
			printf("Release the Enter key to continue!!!\n");
			rcolor;
			while (anyKey()) {
				wait(500);
				while ('\n' == _getch()) {

				}
			}
		}

		char s[80] = "";
		//printf("ou=%i\n", ou[2]);
		if (ou[2] == 0) {
			printf("No button was pushed instead of the ");
			switch (oi) {
			case 0:color(ROTh); break;
			case 1:color(GRUENh); break;
			case 2:color(BLAUh); break;
			}
			printf(col[oi]);
			color(rcol);
			printf(" Button\n");
			sprintf(s, "No button was pushed instead of the %s Button\n", col[oi]);
			cScError(s);
			db_Siren();
		}
		else {
			for (uint8_t i = 0; i != 3; i++) {
				if ((ou[2] & (1 << i))) {
					printf("The button ");
					switch (i) {
					case 0:color(ROTh); break;
					case 1:color(GRUENh); break;
					case 2:color(BLAUh); break;
					}
					printf(col[i]);
					color(rcol);
					printf(" was pushed!\n");
					if (oi != i) {
						sprintf(s, "The button %s was pushed instead of the %s button\n", col[i], col[oi]);
						cScError(s);
						db_Siren();
					}
					else {
						sprintf(s, "The %s button was pushed\n", col[i]);
						cSaddMsg(s);
						db_Beep();
					}
				}
				else {
					if (oi == i) {
						sprintf(s, "The button %s was not pushed\n", col[i]);
						cScError(s);
						db_Siren();
					}
				}
			}
		}
		//wait(1000);
	}
	for (uint8_t i = 0; i != 3; i++) {
		dbLed_setColor(4 + i, 0, 0, 0);
	}
	exitS();
	return 1;
}
bool db_Trfid() {
	uint8_t out[DB_MAXLENGTH];
	uint8_t stcd[6] = { DB_RFID,4, 0,0,0,0 };
	int in = 0, kt = 0;
	bool jk = 0;
	char s[201] = "";
	printf("In this sketch you have to put a RFID-card to the reader (under the DiscBot)!\nThen the UUID of the RFID-card appare on the screen!\nYou can compare them with your smartphone or any other RFID (or NFC) reader!\n");
	printf("Do you want to open an App (Android and IOS) ? (y/n)\n");
	if ('y' == ynrespond()) {
		system("start https://play.google.com/store/apps/details?id=com.wakdev.wdnfc");
		system("start https://apps.apple.com/at/app/nfc-tools/id1252962749");
	}
	color(GELBh);
	printf("To start push enter!\nTo stop press enter!\n");
	rcolor;
	while (!anyKey());
	flush();

	do {
		in++;

		for (kt = 0; !db_cmd(DB_RFID, 0, NULL, out) && kt != 10; kt++, wait(25));
		if (kt == 10) {
			printf("Please test the connection between DiscBot and the computer!\n");
			return 0;
		}
		else {
			if (out[1] != 0 && (memcmp(out, stcd, 6) != 0)) {

				for (uint8_t i = 0; i != 6; i++)
					stcd[i] = out[i];
				printf("Card found\tUUID : ");

				cSaddMsg("Card found\tUUID : ");
				for (uint8_t i = 0; i != 4; i++) {
					printf(" %02X ", out[(3 - i) + 2]);
					sprintf(s, " %02X ", out[(3 - i) + 2]);
					cSaddMsg(s);
				}
				cSaddMsg("\n");
				jk = 0;
				printf("\n");
				if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 255, 0))
					return 0;
				if (!db_Beep())
					return 0;
			}
			else {
				if (out[1] == 0 && !jk) {
					jk = 1;
					printf("No card found yet!\n");
					cSaddMsg("No card found yet!\n");
					if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 255, 0, 0))
						return 0;
					if (!db_Siren())
						return 0;
				}
			}
			if ((in % 8) == 0)
				for (uint8_t i = 0; i != 6; i++)
					stcd[i] = out[i];
		}

		waitS(3, 1);
		while (!waitS(3, 0) && !anyKey()) {
		}
	} while (!anyKey());
	flush();
	if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0))
		return 0;
	cSaskIfFine();


	return 1;
}
bool db_Tcs() {
	uint8_t kt = 0;
	uint8_t out[DB_MAXLENGTH];
	int timo = 1000;
	char cx[12] = "";
	printf("This sketch show you the colors from the color sensor of the DiscBot!\n");
	color(GELBh);
	printf("Do you want to change the timeout from %i milliseconds? (y/n)\n", timo);
	rcolor;
	if ('y' == ynrespond()) {
		color(GELBh);
		printf("Type in an intervall of milliseconds you want the requests: ");
		rcolor;
		do {
			timo = 0;
			timo = int_inp();
		} while (timo <= 0);
	}
	color(GELBh);
	printf("To stop push the enter key!\nTo start push any key!\n");
	rcolor;
	_getch();
	do {
		for (kt = 0; !db_cmd(DB_CS, 0, NULL, out) && kt != 10; kt++, wait(25));
		if (kt == 10) {
			printf("Please test the connection between DiscBot and the computer!\n");

		}
		else {
			int v = 0;
			int sum = 0;
			for (uint8_t i = 0; i != 3; i++) {
				switch (i) {
				case 0:strcpy(cx, "left"); v = 5; break;
				case 1:strcpy(cx, "middle"); v = 11; break;
				case 2:strcpy(cx, "right"); v = 17; break;
				}
				printf("\n\tColor %s\n", cx);
				printf("Color index: %i\n", out[2 + i]);
				//printf("Color quality: %i %i\n", out[4 + i], out[3 + i]);
				printf("Color quality: %i\n", (out[4 + i] << 8) + out[3 + i]);


				for (uint8_t ii = 0; ii != 3; ii++) {
					sum = v + ii;
					switch (ii) {
					case 0:strcpy(cx, "red");
						if (out[sum] >= 150)
							color(ROTh);
						break;
					case 1:strcpy(cx, "green");
						if (out[sum] >= 150)
							color(GRUENh);
						break;
					case 2:strcpy(cx, "blue");
						if (out[sum] >= 150)
							color(BLAUh);
						break;
					}
					printf("Color %s: %i\n", cx, out[sum]);
					rcolor;
					//printf("Color %s: %02X num:%i  %i\n", cx, out[sum], sum, v);
				}
				dbLed_setColor(6 - i, out[v + 0], out[v + 1], out[v + 2]);
			}

		}
		waitS(timo, 1);
		while (!waitS(timo, 0) && !anyKey());
	} while (!anyKey());
	for (uint8_t i = 0; i != 3; i++)
		dbLed_setColor(6 - i, 0, 0, 0);

	flush();
	cSaskIfFine();
	return 1;
}
bool dbLed_setColor(uint8_t led, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t in[12] = { led, r,g,b };
	uint8_t out[DB_MAXLENGTH];
	if (!db_cmd(DB_LED, 4, in, out))
		return 0;
	return 1;
}
bool db_Tled() {
	int tiver = 500;

	do {
		printf("In this sketch the Led's are flashing!\nIf you are ready push any key!\n");
		_getch();
		do {
			printf("Do you want to wait for each LED %i milliseconds? (y/n)\n", tiver);
			if ('n' == ynrespond()) {
				tiver = 999999;
				printf("Set your timeout: ");
				do {
					tiver = int_inp();
				} while (tiver == 999999);
				flush();
			}
			else {
				tiver = 50;
			}
		} while (tiver <= 0);
		uint8_t cntW = 0;
		uint8_t colorx[3] = { 255,255,255 };
		for (uint8_t i = 0; i != 4; i++) {
			printf("Color (%i) : ", i);
			switch (i) {
			case 0:colorx[0] = 255; colorx[1] = 255; colorx[2] = 255; rcolor; printf("white"); break;
			case 1:colorx[0] = 255; colorx[1] = 0; colorx[2] = 0; color(ROTh); printf("red"); rcolor; break;
			case 2:colorx[0] = 0; colorx[1] = 255; colorx[2] = 0; color(GRUENh); printf("green"); rcolor; break;
			case 3:colorx[0] = 0; colorx[1] = 0; colorx[2] = 255; color(BLAUh); printf("blue"); rcolor; break;
			}
			printf(" :\t");

			for (cntW = 0; cntW != 12; cntW++) {
				printf("LED %i\t", cntW + 1);
				switch (cntW) {
				case 0:dbLed_setColor(cntW, colorx[0], colorx[1], colorx[2]); break;
				case 1:
				case 2:
				case 3:
					dbLed_setColor(cntW, colorx[0], colorx[1], colorx[2]);
					dbLed_setColor(cntW - 1, 0, 0, 0); break;
				case 4:dbLed_setColor(cntW - 1, 0, 0, 0);
				case 5:
				case 6:
				case 7:
					dbLed_setColor(cntW + 3, colorx[0], colorx[1], colorx[2]);
					dbLed_setColor(cntW + 2, 0, 0, 0); break;
				case 8:dbLed_setColor(cntW + 2, 0, 0, 0);
				case 9:
				case 10:dbLed_setColor(14 - cntW, colorx[0], colorx[1], colorx[2]);
					dbLed_setColor(15 - cntW, 0, 0, 0); break;
				case 11:dbLed_setColor(15 - cntW, 0, 0, 0); break;

				}
				if (cntW != 11) {
					wait(tiver);
				}
				else {
					wait(25);
					printf("\n");
				}
			}
		}
		printf("Again? (y/n)\n");
	} while (ynrespond() == 'y');
	cSaskIfFine();
	return 1;
}
bool dbLs_setLoud(uint8_t loud) {
	uint8_t in[3] = { loud,0,0 };
	return db_cmd(DB_LOUD, 1, in, NULL);
}
bool dbLs_getLoud(uint8_t* loud) {
	uint8_t out[3] = { 0,0,0 };
	*loud = 0;
	if (!db_cmd(DB_LOUD, 0, NULL, out))
		return 0;
	*loud = out[2];
	return 1;
}

bool db_Tls() {
	dbLs_setLoud(102);
	int loud = -1;
	int son = 1000;
	int mod = 1000;
	uint8_t in[DB_MAXLENGTH];
	do {
		printf("This sketch plays a song from the DiscBot loudspeaker!\nTurn the switch to uC\n");

		//	do {

		color(GELBh);
		printf("Please set your volume (0-100=sound) (101=mute) (102=standard) and punch enter:");

		rcolor;
		//do {
		//	loud = -1;
		loud = int_inp(0, 102);
		/* } while (loud < 0);
			if (loud == -1) {
				color(ROTh);
				printf("Wrong value set!\ttry again!\n");
				rcolor;
			}*/
			//} while (loud < 0 || loud>102);
		if (!dbLs_setLoud(loud))
			return 0;
		printf("Repeating the song continuously? (y/n)\n");
		in[2] = (ynrespond() == 'y');
		//do {

		printf("Select a song:\n");
		for (uint8_t i = 0; i != MaxSongs; i++) {
			if (i % 2) {
				color(GELBh);
			}
			printf("(%02i) ... ", i + 1);
			uint16_t len = 0;
			for (len = 0; dbLs_ALL[i][len].pitch != DB_LS_LAST; len++);
			printf("%4.01f sec (real %5.01f sec) -", (float)((float)sonlen((DbLsNote*)dbLs_ALL[i]) / 1000), (float)((float)sonlenDB((DbLsNote*)dbLs_ALL[i]) / 1000));
			printf("%3i note%s", len, (len == 1) ? "  " : "s ");
			printf(" - \"");
			switch (i) {
			case 0:printf("Beep"); break;
			case 1:printf("Sirene"); break;
			case 2:printf("muss i denn zum st%cdtele hinaus", 132); break;
			case 3:printf("AURA LEE (America)"); break;
			case 4:printf("BIG BEN (England)"); break;
			case 5:printf("ROW, ROW, ROW YOUR BOAT (Traditional)"); break;
			case 6:printf("AH! Vous DIRAI-JE, MAMAN (Morgen kommt der Weihnachtsmann) (France melody from 1771)"); break;
			case 7:printf("SUR LE PONT D'AVIGNON (France)"); break;
			case 8:printf("YANKEE DOODLE (America)"); break;
			case 9:printf("KAISER-WALZER (Johann Strauss, sun 1825-1899)"); break;
			case 10:printf("SUPER MARIO LAND (1989) (Martin Tullmann)"); break;
			case 11:printf("SUPER MARIO BROS (1983)"); break;
			case 12:printf("SUPER MARIO BROS UNDERWORLD (1983)"); break;
			case 13:printf("PAC-MAN (1980)"); break;
			case 14:printf("Dart Vader theme (Imperial March) - Star wars (1980)"); break;
			case 15:printf("Astronomia (2010) (Coffin-Dance-Theme)"); break;
			case 16:printf("Entertainer (1902) (Scott Joplin)"); break;
			case 17:printf("Take on Me (1985)"); break;
			case 18:printf("Pirates Of The Caribbean Theme Song (2003)"); break;
			case 19:printf("Star Trek (1966)"); break;
			case 20:printf("Tetris (1984)"); break;
			case 21:printf("WICKI (1974) (Martin Tullmann)"); break;
			case 22:printf("The Campfire Song Song (2002)"); break;
			case 23:printf("Harry Potter (2001)"); break;
			case 24:printf("Simpsons"); break;
			case 25:printf("KNIGHT RIDER"); break;
			case 26:printf("Mii Chanel"); break;
			case 27:printf("Cantina Band (Star Wars)"); break;
			case 28:printf("Pink Panter"); break;
			case 29:printf("F%cr Elise", ue); break;
			case 30:printf("Tokyo Drift (2006)"); break;
			case 31:printf("Happy birthday (or so what)"); break;
			default:printf("undefined"); break;
			}
			printf("\"\n");
			if (!((i + 1) % 5) && (MaxSongs > (i + 1))) {
				printf("\n");

			}
			rcolor;
		}
		printf("Type in any number and press enter: ");
		son = int_inp(1, MaxSongs);
		son--;
		/*if (son > MaxSongs || son < 0) {
			color(ROTh);
			printf("Wrong value set!\n\ttry again!\n");
			rcolor;
		}*/
		//} while (son > MaxSongs - 1 || son < 0);
		double lenmax = sonlen((DbLsNote*)dbLs_ALL[son]);
		color(GRUENh);
		printf("The song lasts %0.3lf seconds!\n", lenmax / 1000);
		rcolor;
		if (son >= 3) {
			if (!db_load_song_to_bot(son)) {
				color(ROTh);
				printf("Transmission FAIL!!\n");
				rcolor;
				return 0;
			}
			else {
				color(GRUENh);
				printf("Transmission PASS!!\n");
				rcolor;
			}
		}
		printf("Do you want to play this song on the computer? (y/n)\n");
		if ('n' != ynrespond()) {
			do {

				color(GRUENh);
				printf("Song is playing please wait ...\n");
				rcolor;
				playSong((DbLsNote*)dbLs_ALL[son]);
				printf("Song was played, we can go on!\nAgain? (y/n)\n");
			} while ('y' == ynrespond());
		}
		//in[0] = son;
		if (son < 3)
			in[0] = son;
		else
			in[0] = 3;

		mod = 0;
		do {
			if (mod > 10 || mod < 0) {
				color(ROTh);
				printf("Wrong value set!\ttry again!\n");
				rcolor;
			}
			printf("Set mode (0...regular, 1...stacato1, ..., 9...stacato9, 10...smooth) and push enter: ");
			mod = int_inp();
		} while (mod > 10 || mod < 0);
		in[1] = mod;
		if (!db_cmd(DB_LS, 3, in, NULL))
			return 0;
		color(GELBh);
		printf("Push enter to stop the song!\n");
		rcolor;
		if (son == 25) {
			//_getch();
			wait(1000);
			flush();
			flush();
			flush();
			while (!anyKey()) {

				dbLed_setColor(DB_LED_FRONT_LEFT_OUT, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_LEFT_OUT, 0, 0, 0);
				dbLed_setColor(DB_LED_FRONT_LEFT_IN, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_LEFT_IN, 0, 0, 0);

				//if (!anyKey())
				//	break;

				dbLed_setColor(DB_LED_BUTTON_RED, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_RED, 0, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_GREEN, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_BLUE, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_BLUE, 0, 0, 0);
				//if (!anyKey())
				//	break;


				dbLed_setColor(DB_LED_FRONT_RIGHT_IN, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_IN, 0, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_OUT, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_OUT, 0, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_IN, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_RIGHT_IN, 0, 0, 0);
				//if (!anyKey())
				//	break;

				dbLed_setColor(DB_LED_BUTTON_BLUE, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_BLUE, 0, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_GREEN, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_RED, 255, 0, 0);
				dbLed_setColor(DB_LED_BUTTON_RED, 0, 0, 0);
				//if (!anyKey())
				//	break;

				dbLed_setColor(DB_LED_FRONT_LEFT_IN, 255, 0, 0);
				dbLed_setColor(DB_LED_FRONT_LEFT_IN, 0, 0, 0);


			}
			for (uint8_t i = 0; i != 7; i++) {
				dbLed_setColor(i, 0, 0, 0);
			}
		}
		else {
			_getch();
			flush();
			while (!anyKey());
			flush();
		}
		color(ROTh);
		printf("Stop song!\n");
		rcolor;
		if (!db_cmd(DB_LSS, NULL, NULL, NULL)) return 0;
		printf("Again? (y/n)\n");
	} while (ynrespond() == 'y');
	cSaskIfFine();

	return 1;
}
bool db_cBtsend(char* data) {
	return db_cmd(DB_BT, strlen(data), (uint8_t*)data, NULL);
	//return 1;
}
bool db_cBtsend(char* data, int len) {
	return db_cmd(DB_BT, (uint8_t)len, (uint8_t*)data, NULL);
	//return 1;
}
bool db_cBtget(char* data) {
	wait(400);
	uint8_t param[256] = "";
	if (db_cmd(DB_BT, NULL, NULL, param)) {
		for (uint8_t i = 0; i != param[1]; i++)
			data[i] = (char)param[i + 2];
		data[param[1]] = '\0';
		return 1;
	}
	return 0;
}
bool db_cBtAnalysis(char* datin, uint8_t* saetze, char* datout) {
	if (datin[0] != '\n' || datin[1] != '+' || datin[2] != '+' || datin[3] != '+') {
		return 0;
	}
	if (datin[4] == '\0') {
		datout[0] = '\0';
		*saetze = 0;
		return 1;
	}
	uint8_t i = 4;
	uint8_t io = 0;

	*saetze = 0;

	//	printf("Sat0: %i\n", *saetze);
	do {
		*saetze = *saetze + 1;
		if (*saetze == 255) {
			return 0;
		}
		//printf("Sat: %i\n", *saetze);
		do {
			datout[io] = datin[i];
			////	printf("%c", datin[i]);
			io++;
			i++;
		} while (datin[i] != '\n' || datin[i + 1] != '+' || datin[i + 2] != '+' || datin[i + 3] != '+');
		//printf("%c%c%c%c", datin[i], datin[i + 1], datin[i + 2], datin[i + 3]);
		i += 4;
		datout[io] = '\0';
		io++;
		//printf("if (%i (%c) ==%i)\n", datin[i], datin[i], '\0');
	} while (datin[i] != '\0');
	return 1;
}
bool db_BTWC(const char* cmd, char* out, char* paramin, char* paramout);
void bT_CONVERT(char* in);

int db_BTAT() {
	char cm[50] = "";
	char cm1[50] = "";
	char cm2[50] = "";
	uint8_t num = 0;

	char out[36];
	char paramin[36];
	char paramout[36];
	db_BTWC("AT", out, paramin, paramout);
	if (!bt_get_cmd(NULL,"ATS", cm) && !strcmp(cm, "nan")) {
		color(ROTh);
		printf("NO AT command for \"AT\" is defined!!!\n");
		printf("Please enter something so fast as possible!!!\n");
		rcolor;
	}

	if (!bt_get_cmd(NULL,"ATS", cm))
		return 0;
	if (!db_cBtsend(cm))
		return -1;
	if (!bt_get_cmd(NULL,"ATG", cm))
		return 0;
	if (!db_cBtget(cm1))
		return -1;
	if (!db_cBtAnalysis(cm1, &num, cm2))
		return 3;

	//printf("%s == %s\n",cm2,cm);


	if (!strcmp(cm2, cm))
		return 1;
	color(ROTh);
	printf("Callback should be: \"");
	printf(cm);
	printf("\" instead of \"");
	printf(cm2);
	printf("\"\n");
	rcolor;
	return 2;
}
bool db_BTWC(const char* cmd, char* out, char* paramin, char* paramout) {
	bt_os(0);
	char cm1[51];
	char cm[51];
	char ou[51];
	char cp[51];
	char* cpp = NULL;
	strcpy(cm1, cmd);
	strcat(cm1, "S");
	if (paramin == NULL) {
		strcat(cm1, "?");
	}
	uint8_t num = 0;
	if (paramout != NULL) {
		strcpy(paramout, "nan");
	}
	char oux[51];
	if (!bt_get_cmd(NULL,(char*)cm1, cm)) {
		color(ROTh);
		printf("%s is not defined in HM file! (%s) \n", cmd, cm1);
		rcolor;
		return 0;
	}
	if (!strcmp(cm, "\\-")) {
		color(VIOLETTh);
		printf("(\"%s\") command is not supportet by bluetooth!!\n", cmd);
		rcolor;
		return 1;
	}
	//PARAM SEARCH
	for (uint8_t i = 0; cm[i] != '\0'; i++) {
		if (cm[i] == '<' && cm[i + 1] != '\0' && cm[i + 2] != '\0' && cm[i + 1] == 'P' && cm[i + 2] == '>') {
			//	printf("Parameter found!\n");
				//printf("%s , (char) %c , %s\n", cm, cm[i], &cm[i + 3]);
			strcpy(cp, cm);
			cm[i] = '\0';

			//printf("%p \"%c\"\n", &cp[i + 3], cp[i + 3]);
			cpp = &cp[i + 3];
			//printf("%p\n", cpp);


			//printf("Replace parameter to %s!\n", paramin);
			strcat(cm, paramin);
			strcat(cm, cpp);
			//printf("param after=\"%s\"\n", cpp);
			//printf("Str = \"%s\"\n", cm);
		}
	}
	//printf("PUT IN %s\n", cm);
	if (!db_cBtsend(cm))
		return 0;
	if (!db_cBtget(ou))
		return 0;
	bool aparam = 0;
	if (!db_cBtAnalysis(ou, &num, oux)) {
		color(ROTh);
		printf("Analystic error!\n");
		strcpy(paramout, "nan");
		rcolor;
		return 1;
	}
	if (num != 0) {
		//printf("GET = %s\n", oux);

		//printf("GET OUT %s\n", oux);
		//PARAM SEARCH
		char c = '\0';
		strcpy(cm, cm1);
		c = cm[strlen(cm) - 1];
		//printf("Char end %c\n", c);
		if (c == '?')
			cm[strlen(cm) - 2] = 'G';
		else
			cm[strlen(cm) - 1] = 'G';
		//printf("Search cmd : %s\n", cm);
		if (bt_get_cmd(NULL,(char*)cm, ou)) {
			cm[0] = '\0';
			/*printf("Recive should be: \"");
			printf(ou);
			printf("\"\n");*/
			for (uint8_t i = 0; ou[i] != '\0'; i++) {
				if (ou[i] == '<' && ou[i + 1] != '\0' && ou[i + 2] != '\0' && ou[i + 1] == 'P' && ou[i + 2] == '>') {
					cpp = &ou[i + 3];
					/*printf("At end \"");
					printf(cpp);
					printf("\"\n");*/
					memcpy(cm, ou, i);
					cm[i] = '\0';
					/*printf("At begin \"");
					printf(cm);
					printf("\"\n");*/
					aparam = 1;
				}
			}
			char c2[2] = { '\0','\0' };

			if (aparam) {
				if (cm[0] != '\0' && 0 != memcmp(cm, oux, strlen(cm))) {
					color(ROTh);
					printf("Begin str:\"");
					for (uint8_t i = 0; i != strlen(cm); i++) {
						printf("%c", oux[i]);
					}
					printf("\" should be: \"");
					for (uint8_t i = 0; i != strlen(cm); i++) {
						printf("%c", cm[i]);
					}
					printf("\"\n\tHave a look in HM file (command \"%s\")\n", cm1);
					rcolor;
					return 1;
				}
				if (cm[0] == '\0') {
					if (cpp[0] == '\0') {
						strcpy(paramout, oux);
						return 1;
					}
					strcpy(paramout, "");
					for (uint8_t i = 0; 0 != strcmp(oux + i, cpp) && oux[i] != '\0'; i++) {
						c2[0] = oux[i];
						strcat(paramout, c2);
						if (oux[i + 1] == '\0' && cpp[0] != '\0') {
							color(ROTh);
							printf("No ending line detected after paremeter by BT input!!!\n");
							cScError("No ending line detected after paremeter by BT input!!!\n");
							rcolor;
						}
					}
					return 1;
				}
				else {
					strcpy(paramout, "");
					for (uint8_t i = strlen(cm); 0 != strcmp(oux + i, cpp) && oux[i] != '\0'; i++) {
						c2[0] = oux[i];
						strcat(paramout, c2);
						if (oux[i + 1] == '\0' && cpp[0] != '\0') {
							color(ROTh);
							printf("No ending line detected after paremeter by BT input!!!\n");
							cScError("No ending line detected after paremeter by BT input!!!\n");
							rcolor;
						}
					}
					return 1;
				}
			}
		}
		else {
			printf("%s is not defined in HM file!\n", cm);
		}

	}
	return 1;
}
void bT_CONVERT(char* in) {
	int i = -1;
	char c = '\0';
	char* cp;
	char ca[1001] = "";
	char cx[1001] = "";
	do {
		i++;
		//printf("char %c\n", cy[i]);
		switch (in[i]) {
		case '\\':
			//printf("active (%c) ", cy[i + 1]);
			switch (in[i + 1]) {
			case '_':c = ' ';  break;
			case 'n':c = '\n'; break;
			case 't':c = '\t'; break;
			case 'r':c = '\r'; break;
			case '\\':c = '\\'; break;
			case '\"':c = '\"'; break;
			default:c = in[i + 1];
			}
			//printf(": will :\"%c\"\n", c);
			cx[0] = c;
			cx[1] = '\0';
			in[i] = '\0';
			if (c != ' ')
				cp = (in + i + 2);
			else
				cp = (in + i + 2);

			//printf("cp = %s\n", cp);
			strcpy(ca, in);
			strcat(ca, cx);
			strcat(ca, cp);
			strcpy(in, ca);
			//printf("OUT \"");
			//printf(cy);
			//printf("\"\n");
			break;
		default:break;
		}
	} while (in[i] != '\0');

}
bool db_cBT() {
	printf("In this sketch you can send and read data from the bluetooth module!\n");
	printf("Do you want to skip this? (y/n)\n");
	if ('y' == ynrespond())
		return 1;
	printf("Do you need an App for BLE terminal (Android and IOS)? (y/n)\n");
	if ('y' == ynrespond()) {
		system("start https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal");
		system("start https://apps.apple.com/de/app/bluetooth-terminal/id1058693037");
	}
	do {
		color(GELBh);
		printf("Send data to BT ...\n");
		rcolor;
		char err[255] = "";
		/*color(GELBh);
		printf("Type in 'h' in order to use hexadecimal\n");
		rcolor;*/
		printf("\tType in your message for BT\n:");

		if (1 || 'h' != _getch()) {
			printf("ASCII\t '\\t', '\\n', '\\r' ' ' ...\n");
			do {
				gets_s(err, 251);
			} while (err[1] == '\0');
			bT_CONVERT(err);
			if (!db_cBtsend(err))return 0;
			err[0] = '\0';
		}/*
		else {
			color(GELBh);
			printf("HEX:");
			rcolor;
			printf("Type in the length:");
			char* datP = NULL;
			char* datPB = NULL;
			int len = 0;
			do {
				len=int_inp();
			} while (len - 2 > 255);
			datP = (char*)malloc(len);
			if (datP == NULL) {
				color(ROTh);
				printf("No RAM aviable!\n");
				rcolor;
				return 0;
			}
			for (int i = 0; i != len; i++) {
				printf("NUM %02i: ");
				datPB[i] = 0;
					scanf("%x", &datP[i]);
				flush();
				datPB
			}
			db_cBtsend(datP, len);
			free(datP);
			free(datPB);
		}*/

		printf("BT get data ...\n");

		if (!db_cBtget(err)) return 0;
		//printf(err);
		//printf("\"\n");
		uint8_t sat;
		char out[300] = "";
		char* pout;
		pout = out;
		if (!db_cBtAnalysis(err, &sat, out)) {
			color(ROTh);
			printf("Data ERROR!!!\n");
			rcolor;
			cScError("Data ERROR!!!\n");
		}
		else {
			color(GRUENh);
			printf("Data packages: %i\n", sat);
			rcolor;
			for (uint8_t i = 0; i != sat; i++) {
				color(GRUENd);
				printf("Pack %i : \"", i + 1);
				color(GRUENh);
				printf(pout);
				color(GRUENd);
				printf("\"\n");
				rcolor;
				pout += strlen(pout) + 1;
			}
		}
		printf("Again? (y/n)\n");
	} while ('n' != ynrespond());
	return 1;
}
#define IRS_FRONT 0
#define IRS_BACK 1
#define IRS_LEFT 2
#define IRS_RIGHT 3
bool db_irs(uint8_t* value) {
	uint8_t in[12];
	uint8_t out[DB_MAXLENGTH];
	uint8_t x = 255;
	/*switch (sensor) {
	case IRS_FRONT:x = 0x80; break;
	case IRS_BACK:x = 0x20; break;
	case IRS_LEFT:x = 0x10; break;
	case IRS_RIGHT:x = 0x40; break;
	default:return 0;
	}*/
	x = 0;
	x |= 0x80;
	x |= 0x20;
	x |= 0x10;
	x |= 0x40;
	in[0] = x;
	if (!db_cmd(DB_IRS, 1, in, out))
		return 0;

	memcpy(value, &(out[2]), 4);

	/*	printf("input %i\n", sensor);
		for (uint8_t i = 0; i != 4; i++)
			printf("%i sas %i\n", i, out[2 + i]);*/

			//printf("Choose %i (%i)\n", x, *value);
	return 1;
}
uint8_t analysis_irs(int sensor, uint8_t value[]) {
	uint8_t x = 0;
	switch (sensor) {
	case IRS_FRONT: x = 0; break;
	case IRS_BACK: x = 1; break;
	case IRS_LEFT: x = 2; break;
	case IRS_RIGHT:x = 3; break;
	}
	return value[x];
}


bool db_Tirs() {
	uint8_t mess[DB_MAXLENGTH];
	printf("In this sketch the infreared sensors are going to messureing the distances of the next object!\n");
	bool b = 0;
	unsigned int in = 0;
	color(GRUENh);

	printf("Press any key to start!\n");
	rcolor;

	_getch();
	do {
		in++;
		b = 0;
		//uint8_t i = 0;
		//printf("Status %3.3lf%%\r", 0);
		/*for (i = 0; i != 4 && db_irs(i, &mess[i]); i++) {
			printf("Status %3.3lf%%\r", percent(4, i));
		}*/
		if (!db_irs(mess))
			return 0;
		//analysis_irs(0,mess);
		//printf("Status %3.3lf%%\n", percent(4, 4));
		printf("The messurements are: front: %i cm\tback: %i cm\tleft: %i cm\tright: %i cm\n", mess[0], mess[1], mess[2], mess[3]);
		/*if (in < 5) {
			color(ROTd);
			printf("\tWarning these values are messured from ADC, no security if they are realy connected!!\n\tRetry this process several times!!\n");
			rcolor;
		}*/
		while (db_irs(mess) && !anyKey()) {
			for (uint8_t i = 0; i != 4; i++) {
				percentGraph(percent(50, mess[i]), "%5.2f%%", 1, 8);//7
				printf(" %3i cm ", mess[i]);
			}
			printf("\r");
			wait(100);
		}
		printf("\n");
		printf("Again? (y/n)\n");
	} while ('y' == ynrespond());
	cSaskIfFine();
	return 1;
}
#define USS_FRONT 0x08
#define USS_BACK 0x02
#define USS_LEFT 0x01
#define USS_RIGHT 0x04
bool db_uss_result(uint8_t sensor, uint8_t* value) {
	uint8_t in[DB_MAXLENGTH] = { 0 };
	uint8_t out[DB_MAXLENGTH] = { 0 };/*
	if (!db_cmd(DB_CLEAR, 1, in, out))
		return 0;
	*/
	uint8_t x = 0;
	//printf("RESULT sensor %i\n ", sensor);
	switch (sensor) {
	case 0:x = USS_FRONT; break;
	case 1:x = USS_BACK; break;
	case 2:x = USS_LEFT; break;
	case 3:x = USS_RIGHT; break;
	}
	in[0] = x;
	//printf("\tval %02X\n", in[0]);

	if (!db_cmd(DB_USS_G, 1, in, out))
		return 0;
	if (out[1] != 0)
		*value = out[2];
	else
		*value = 0;
	return 1;
}
bool db_uss(uint8_t sensor) {
	uint8_t in[DB_MAXLENGTH];
	//uint8_t out[DB_MAXLENGTH];
	/*
	if (!db_cmd(DB_CLEAR, 1, in, out))
		return 0;
	*/
	uint8_t x = 0;
	//printf("sensor %i \n", sensor);
	switch (sensor) {
	case 0:x = USS_FRONT; break;
	case 1:x = USS_BACK; break;
	case 2:x = USS_LEFT; break;
	case 3:x = USS_RIGHT; break;
	}
	in[0] = x;
	//printf("\t val %02X\n", in[0]);
	if (!db_cmd(DB_USS_M, 1, in, NULL))
		return 0;
	//wait(200);
	//if (!db_cmd(DB_USS_G, 1, in, out))
	//	return 0;
	//*value = out[2];
	return 1;
}
bool db_Tuss() {
	printf("In this sketch the values of the uss will be displayed on screen!\n");
	uint8_t sensou[4] = { 0 ,0 ,0, 0 };
	uint8_t i = 0;
	char cv[100] = "";
	char s[100] = "";

	printf("Push any key to start!\n");
	_getch();
	do {
		/*for (i = 0; i != 4; i++) {
			switch (i) {
			case 0: strcpy(cv, "front"); break;
			case 1: strcpy(cv, "back"); break;
			case 2: strcpy(cv, "left"); break;
			case 3: strcpy(cv, "right"); break;
			}
			color(GELBh);
			printf("Is the %s sensor connected? (y/n)\n", cv);
			rcolor;
			if ('y' == _getch()) {
				if (!db_uss(i)) {
					db_reset();
					return 0;
				}
			}
			//if (i == 2)
			//	db_reset();
		}
		*/
		for (uint8_t i = 0; i != 4; i++) {
			if (!db_uss(i)) {
				db_reset();
				return 0;
			}
		}
		for (i = 0; i != 4; i++) {
			if (!db_uss_result(i, &sensou[i])) {
				db_reset();
				return 0;
			}
		}
		//if (i == 2)
		//	db_reset();
	/*}
	for (i = 0; i != 4; i++) {
		switch (i) {
		case 0: strcpy(cv, "front"); break;
		case 1: strcpy(cv, "back"); break;
		case 2: strcpy(cv, "left"); break;
		case 3: strcpy(cv, "right"); break;
		}
		printf("%s", cv);
		printf(" : ");
		if (sensou[i] == 0) {
			printf("nan");
			cv[0] += 'A' - 'a';
			sprintf(s, "%s sensor is not connected or had to much distance to messure\n", cv);
			cScError(s);
		}
		else {
			printf("%i cm", sensou[i]);
			sprintf(s, "Sensor %s was messureing %i cm\n", cv, sensou[i]);
			cSaddMsg(s);
		}
		printf("\t");
	}*/

		printf("The messurements are: ");
		for (i = 0; i != 4; i++) {
			switch (i) {
			case 0: strcpy(cv, "front"); break;
			case 1: strcpy(cv, "back"); break;
			case 2: strcpy(cv, "left"); break;
			case 3: strcpy(cv, "right"); break;
			}
			printf("%s", cv);
			printf(" : ");
			if (sensou[i] == 0) {
				color(ROTh);
				printf("nan");
				rcolor;
				cv[0] += 'A' - 'a';
				sprintf(s, "%s sensor is not connected or had to much distance to messure!\n", cv);
				cScError(s);
			}
			else {
				color(GRUENh);
				printf("%i cm", sensou[i]);
				rcolor;
				sprintf(s, "Sensor %s was messureing %i cm\n", cv, sensou[i]);
				cSaddMsg(s);
			}
			printf("\t");
		}
		printf("\n");
		for (i = 0; i != 4; i++) {
			if (sensou[i] == 0) {
				color(ROTd);
				printf("nan means the sensor is not connected\n\t or give no request\n\tor the distance is to high!\n");
				rcolor;
				break;
			}
		}
		while (!anyKey()) {
			for (i = 0; i != 4; i++) {
				if (!db_uss(i)) {
					db_reset();
					return 0;
				}
				if (!db_uss_result(i, &sensou[i])) {
					db_reset();
					return 0;
				}
				percentGraph((double)sensou[i], "%4.1lf%%\t", 1, 9);
				if (sensou[i] == 0) {
					printf("=");
					color(ROTh);
					printf(" nan  \t");
				}
				else {
					printf("= %3icm\t", sensou[i]);
				}
				rcolor;
			}
			printf("\r");
		}
		flush();
		printf("\n");
		printf("Again? (y/n)\n");
	} while ('y' == ynrespond());
	cSaskIfFine();
	//free(&cv);
	return 1;
}
bool db_Tbm() {
	uint8_t out[DB_MAXLENGTH];
	char s[201] = "";
	double right = 0, left = 0;
	printf("In this sketch the brightness sensors are going to messureing the brightness of the front!\n");
	do {
		if (!db_cmd(DB_BM, 0, NULL, out))
			return 0;
		printf("The messurements are: left: %0.2f%% (%i of 255)\tright: %0.2f%% (%i of 255)\n", (float)((float)out[2] / 255) * 100, out[2], (float)((float)out[3] / 255) * 100, out[3]);
		sprintf(s, "The messurements are:\n\tleft: %0.2f %% (%i of 255)\n\tright: %0.2f%% (%i of 255)\n", (float)((float)out[2] / 255) * 100, out[2], (float)((float)out[3] / 255) * 100, out[3]);
		cSaddMsg(s);
		printf("Push Enter to exit while(1)\n");
		while (!anyKey()) {
			if (!db_cmd(DB_BM, 0, NULL, out))
				return 0;
			left = out[2];
			right = out[3];
			percentGraph(percent(255, left), "%6.2lf%%", 1, 25);
			printf("\t");
			percentGraph(percent(255, right), "%6.2lf%%", 1, 25);
			wait(100);
			printf("\r");

		}
		printf("\r");
		//printx(' ', 10);
		printf("\n");
		printf("Again? (y/n)\n");
	} while (ynrespond() == 'y');
	cSaskIfFine();
	return 1;
}
bool bt_askUseF(char* filename) {
	filename[0] = '\0';
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir("HM\\")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			printf("%s\n", ent->d_name);
		}
		printf("END dir");
		closedir(dir);
		return 1;
	}
	else {
		printf("Fail open dir\n");
		return 0;
	}
}
int strlcmp(char* x1, char* x2) {
	char x11[100];
	char x21[100];
	strcpy(x11, x1);
	strcpy(x21, x2);
	for (uint8_t i = 0; x1[i] != '\0'; i++) {
		x11[i] = GROSS_2_KLEIN(x1[i]);
	}
	for (uint8_t i = 0; x2[i] != '\0'; i++) {
		x21[i] = GROSS_2_KLEIN(x2[i]);
	}
	return !strcmp(x11, x21);
}
bool show_bt_dir(char* filename) {
	DIR* dir;
	FILE* f = NULL;
	struct dirent* ent;
	const int max_len = 103;
	char tex[max_len] = "";
	int maxFiles = 0;
	char* names;
	char* ccp = NULL;
	char userNote[1001] = "";
	int namesp = 0;
	char text[201] = "";
	if ((f = fopen("HM\\use.TXT", "r")) != NULL) {
		strcpy(text, "HM\\");
		int i = strlen(text);
		while ((text[i] = fgetc(f)) != EOF) {
			i++;
		}
		text[i] = '\0';
		//printf("FILE : %s\n", text);
		strcat(filename, &text[strlen("HM\\")]);
		fclose(f);
		if ((f = fopen(text, "r")) != NULL) {

			//	printf("FILE : %s\n", text);
			fclose(f);
			return 1;
		}
		else {
			color(ROTh);
			printf("Invalid input at file \"HM\\use.TXT\"\n");
			rcolor;
			color(GELBh);
			printf("Fix bug...\n");
			rcolor;
		}
	}
	else {
		color(ROTh);
		printf("File \"HM\\use.TXT\" not found!\n");
		color(GELBh);
		printf("Create one...\n");
		rcolor;
	}
	if ((dir = opendir("HM\\")) != NULL) {
		color(GRUENh);
		printf("Open \"HM\\\" directory...\n");
		rcolor;
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] != '.') {
				if (strlcmp((char*)".TXT", &ent->d_name[ent->d_namlen - 4])) {
					if (strcmp(ent->d_name, "use.TXT")) {
						strcpy(tex, ent->d_name);
						tex[ent->d_namlen - 4] = '\0';
						color(GELBh);
						printf("(%i)", ++maxFiles);
						rcolor;
						printf("\t \"%s\" ", tex);
						char fnam[101];
						sprintf(fnam, "HM\\%s.TXT", tex);
						FILE* f2 = fopen(fnam, "a+");
						//printf("OPEN \"%s\"\n", fnam);
						if (f2!=NULL&&bt_get_cmd(f2, "USERNOTE", userNote))
							printf("\n\t Note: \"%s\"", userNote);
						if (f2 != NULL)
							fclose(f2);
						printf("\n");
					}
				}
			}
		}

		rewinddir(dir);
		if ((names = (char*)malloc(maxFiles * max_len)) == NULL) {
			color(ROTh);
			printf("Not enought free RAM-memory available!!(Error)\n");
			rcolor;
			printf("Press any key!\n");
			_getch();
			return 0;
		}
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] != '.') {
				if (strlcmp((char*)".TXT", &ent->d_name[ent->d_namlen - 4])) {
					if (strcmp(ent->d_name, "use.TXT")) {
						strcpy(tex, ent->d_name);
						tex[ent->d_namlen - 4] = '\0';
						//	printf("\t %s\n", tex);
						memcpy(&names[namesp], tex, strlen(tex) + 1);
						namesp += (strlen(tex) + 1);

					}
				}
			}
		}
		//printf("END dir");
		closedir(dir);


		printf("%i file%c found\n", maxFiles, (maxFiles == 1) ? '\0' : 's');
		if (maxFiles == 1) {
			color(GRUENh);
			printf("Auto choose \"%s\"! If you want to change delete \"\\HM\\use.txt\"!\n", names);
			rcolor;
			printf("Press any key!\n");
			_getch();
			strcpy(filename, names);
			strcat(filename, ".TXT");
			f = fopen("HM\\use.TXT", "w+");
			if (f != NULL) {
				fprintf(f, "%s", filename);
				fclose(f);
			}
			free(names);
			return 1;
		}
		else {

			int sel = 0;
			do {
				color(GELBh);
				printf("Select a file: ");
				rcolor;
				sel = int_inp(1, maxFiles);
				/*if (sel <= 0 || sel > maxFiles) {
					color(ROTh);
					printf("Invalid input!!\n");
					rcolor;
				}*/
			} while (sel <= 0 || sel > maxFiles);
			ccp = &names[strlen(names) + 1];
			if (sel != 1)
			{
				//	printf("ccp %s\n", ccp);
				for (sel -= 2; sel != 0; sel--) {
					ccp = &ccp[strlen(ccp) + 1];
					//printf("ccp %s\n", ccp);
				}
				strcpy(filename, ccp);
			}
			else
			{
				strcpy(filename, names);
				printf("name: %s\n", filename);
			}
			color(GRUENh);
			printf("Selected file \"%s\"\n", filename);
			rcolor;
			free(names);
			strcat(filename, ".TXT");
			f = fopen("HM\\use.TXT", "w+");
			if (f != NULL) {
				fprintf(f, "%s", filename);
				fclose(f);
			}
			return 1;
		}
	}
	color(ROTh);
	printf("Open \"HM\\\" directory...\n");
	printf("No dir!\n");
	rcolor;
	_getch();
	return 0;
}
bool bt_get_cmd(FILE* fj, const char* in, char* out) {
	FILE* f;
	char filename[101] = "";
	char filenameX[101] = "";
	char line[101] = "";
	if (fj == NULL) {
		show_bt_dir(filename);
		if ((f = fopen("HM\\use.TXT", "r")) == NULL) {

			if ((f = fopen("HM\\use.TXT", "a+")) == NULL) {
				color(ROTh);
				printf("File can't be generated!\n");
				rcolor;
				return 0;
			}
			else {
				fprintf(f, "BT_version.TXT");
				fclose(f);
				return bt_get_cmd(NULL,in, out);
			}
		}
		fclose(f);
		f = fopen("HM\\use.TXT", "a+");
		if (f == NULL) {
			color(ROTh);
			printf("File can't be generated!\n");
			rcolor;
			return 0;
		}
		fscanf(f, "%s", filename);
		if (filename[0] == '\0')
			fprintf(f, "BT_version.TXT");

		fclose(f);

		if (filename[0] != '\0') {
			strcpy(filenameX, "HM\\");
			strcat(filenameX, filename);

			f = fopen(filenameX, "r");
			//	printf("Open file %s\n", filenameX);
			if (f == NULL) {
				color(ROTh);
				printf("File can't be open!!\n");
				rcolor;
				bell();
			}
		}

		if (filename[0] == '\0' || f == NULL) {
			color(ROTh);
			printf("The setting in File \"%s\" to open \"%s\" is invalid!! (directory: HM\\)\nType in an valid File!!\n", "use.TXT", filename);
			if (f != NULL)
				fclose(f);
			bell();
			system("HM\\use.txt");
			return bt_get_cmd(NULL, in, out);
		}

	}
	else {
		f = fj;
	}
	
	char c = '\n';
	int i = 0;
	char l[101] = "";
	char cx[101] = "";
	char cy[101] = "";
	char ca[101] = "";
	char* cp;
	do {
		line[0] = '\0';
		c = '\n';
		i = 0;
		strcpy(cx, "");
		strcpy(cy, "");
		do {
			fscanf(f, "%s %s", cx, cy);
		} while (c != '\n' && c != EOF);
		if (c != EOF) {


			//printf("%s %s\n", cx, cy);
			if (cy[0] == '\0' && cx[0] == '\0')
			{
				//printf("NONE\n");
				return 0;
			}
			else
			{
				//printf("CMP \"%s\"==\"%s\"\n", in, cx);

				if (!strcmp(in, cx)) {
					//printf("YASSS!\n");
					if (!strcmp(cy, "\\-")) {
						strcpy(out, cy);
						return 1;
					}
					bT_CONVERT(cy);
					strcpy(out, cy);
					//printf("out be : \"");
					//printf(out);
					//printf("\"\n");
					return 1;
				}
			}
		}
	} while (c != EOF);
	color(ROTh);
	printf("NO IDEA!\n");
	rcolor;
	return 0;
}
bool bt_chng_config(char* cmd, bool askneed) {
	char out[22] = "";
	char paramin[22] = "";
	char paramout[22] = "";
	char text[51] = "";
	if (askneed) {
		color(GELBh);
		printf("Type in your parameter: ");
		rcolor;
		do {
			gets_s(paramin, sizeof(paramin));
		} while (!strcmp(paramin, ""));
	}
	if (!db_BTWC(cmd, out, paramin, paramout))
	{
		return 0;
	}
	if (askneed) {

		color(GRUENh);
		printf("Verify\n");
		rcolor;
		if (!db_BTWC(cmd, out, NULL, paramout)) {
			return 0;
		}
		if (!strcmp(paramin, paramout)) {
			color(GRUENh);
			sprintf(text, "Parameter verified, written \"%s\"\n", paramout);
			printf(text);
			cSaddMsg(text);
			rcolor;
			return 1;
		}
		else {
			color(ROTh);
			sprintf(text, "Parameter verified, written \"%s\" insted of \"%s\"\n", paramout, paramin);
			printf(text);
			cScError(text);
			rcolor;
			return 0;
		}
	}
	//printf("Returned : %s\n", paramout);
	return 1;
}
bool bt_configchng() {
	color(GELBh);
	printf("Select option:\n");
	rcolor;
	char chang[][101] = { {"name"},{"pin"},{"type"},{"sleepmode"},{"system LED"},{"role"},{"UUID"},{"notify"},{"stoppbits"},{"parity"},{"at begin"},{"connection timeout"},{"reset"},{"factory reset"},{""} };
	char changbt[][101] = { {"NAME"},{"PASS"},{"TYPE"},{"PWR"},{"LED"},{"ROLE"},{"UUID"},{"NOTI"},{"STOP"},{"PARI"},{"IMME"} ,{"TCON"},{"RESET"},{"RENEW"} };
	bool chanbtb[100];
	bool ask[101];
	for (int i = 0; i != 12; i++) {
		ask[i] = 1;
	}
	ask[13] = 0;
	ask[12] = 0;
	int enter = 0;
	int d = 0;
	//bool askneed = 1;
	char xin[101] = "";
	char xout[101] = "";
	int cntd = 0;
	for (d = 0; chang[d][0] != '\0'; d++) {
		chanbtb[d] = 0;
		strcpy(xin, changbt[d]);
		strcat(xin, "S");

		bt_get_cmd(NULL,xin, xout);
		if (strcmp(xout, "\\-")) {
			chanbtb[d] = 1;
			cntd++;
			printf("(%02i) change :\t\"", cntd);
			color(GELBh);
			printf(chang[d]);
			rcolor;
			printf("\" \n");
		}
		else {
			color(ROTh);
			printf("\t\t%s not suportet\n", chang[d]);
			rcolor;
		}
	}
	do {
		color(GELBh);
		printf("Select your operation: ");
		rcolor;
		enter = int_inp();
	} while (enter <= 0 || enter > cntd);
	int x = 0;
	int xc = 0;
	for (xc = 0; (enter) != x; xc++) {
		if (chanbtb[xc]) {
			x++;
		}
		//	printf("%s is %i cnt %i\n", chang[xc], chanbtb[xc], x);
	}
	enter = xc - 1;

	//printf("num %i\n", enter);

	//printf("Change %s\n", chang[enter - 1]);
	color(GRUENh);
	printf("Change %s is in process...\n", chang[enter]);
	rcolor;
	if (!bt_chng_config(changbt[enter], ask[enter])) {
		color(ROTh);
		printf("An error occurred!\n");
		bell();
		rcolor;
	}
	else {
		color(GRUENh);
		printf("Settings successfully written!\n");
		rcolor;
	}

	return 1;
}
bool bt_os(bool val) {
	uint8_t paramin = val;
	return db_cmd(DB_BT_OS, 1, &paramin, NULL);
}
bool bt_config() {
	char out[36];
	char paramin[36];
	char paramout[36];
	for (uint8_t i = 0; i != strlen("Bluetooth configuration!!!\n") - 1; i++) {
		printf("*");
	}
	printf("\n");
	printf("Bluetooth configuration!!!\n");
	for (uint8_t i = 0; i != strlen("Bluetooth configuration!!!\n") - 1; i++) {
		printf("*");
	}
	printf("\n");
	printf("Here you can see the basic information of your BLE module!\n");
	color(GELBh);
	printf("\tThis might need some time please be patient!\n");
	rcolor;
	bool b = 0;
	bool bxx = 0;
	int err = 0;
	bool x = 0;
	char filenam[201] = "";
	do {
		b = 0;
		x = 0;
		char cx[101] = "";
		color(GELBh);
		printf("Sending an test sequence to module!...\n");
		rcolor;
		switch (err = db_BTAT()) {
		case -1:color(ROTh); printf("Connection error's!\n"); rcolor; return 0; break;
		case 0:color(ROTh); printf("BT file error!\n"); rcolor; b = 1; break;
		case 1:color(GRUENh); printf("Everything work fine!\n"); rcolor; break;
		case 2:color(ROTh); printf("Wrong or no request!\n"); rcolor;
			color(GELBh);
			printf("Select other modules? (y/n)\n");
			rcolor;
			b = 1;
			if (ynrespond() == 'y') {
				FILE* f;
				if ((f = fopen("HM\\use.TXT", "w+")) != NULL) {
					if (show_bt_dir(filenam)) {
						fprintf(f, "%s", filenam);
						x = 1;
						b = 0;
					}

					fclose(f);
				}
				else {
					color(ROTh);
					printf("File error!!!\n");
					rcolor;
				}

			}
			break;
		case 3:color(ROTh); printf("Analystic error!\n"); rcolor; b = 1; break;
		}
		rcolor;
		if (b) {
			bxx = 1;
			printf("Do you want to retry? (y/n)\n");
			if (ynrespond() == 'n')
				bxx = 0;
		}
	} while (b && bxx || x);
	switch (err) {
	case -1:return 0; break;
	case 0:cScError("BT file error!\n"); break;
	case 1:cSaddMsg("AT request works fine!\n"); break;
	case 2:cScError("AT request works not fine (wrong/no request)!\n"); break;
	case 3:cSaddMsg("AT request works not fine (data analystic)!\n");  break;
	}
	if (b)
		return 1;

	color(GELBh);
	printf("Other information's might be shown here:\n");
	rcolor;

	printf("\n");
	color(GELBh);
	printf("Basic:\n");
	rcolor;
	db_BTWC("VERS", out, NULL, paramout);
	color(GRUENh);
	printf("Version: \""); printf(paramout); printf("\"\n");
	rcolor;

	db_BTWC("NAME", out, NULL, paramout);
	color(GRUENh);
	printf("Name: \""); printf(paramout); printf("\"\n");
	rcolor;

	db_BTWC("PASS", out, NULL, paramout);
	color(GRUENh);
	printf("Pin: \""); printf(paramout); printf("\"\n");
	rcolor;// 

	db_BTWC("TYPE", out, NULL, paramout);
	color(GRUENh);
	printf("Type: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 0:printf("Not need PIN Code"); break;
		case 1:printf("Auth not need PIN"); break;
		case 2:printf("Auth with PIN"); break;
		case 3:printf("Auth and bond"); break;
		default:printf("nan"); break;
		}
	}
	else
		printf(paramout);
	printf("\"\n");
	rcolor;

	db_BTWC("PWR", out, NULL, paramout);
	color(GRUENh);
	//printf(paramout);
	printf("Sleepmode: ");
	switch (paramout[0] - '0') {
		//case :color(ROTh); printf("wrong callback! \""); printf(paramout); printf("\""); break;
	case 1:printf("auto"); break;
	case 0:printf("off"); break;
	default: printf(paramout); break;
	}
	printf("\n");
	rcolor;

	db_BTWC("LED", out, NULL, paramout);
	color(GRUENh);
	printf("System LED: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 0:printf("Connected High; Unconnected blink (500ms)"); break;
		case 1:printf("Connected High; Unconnected Low"); break;
		default:printf("nan ("); printf(paramout); printf(")"); break;
		}
	}
	else
		printf(paramout);
	printf("\"\n");
	rcolor;

	db_BTWC("TEMP", out, NULL, paramout);
	color(GRUENh);
	printf("Temperature: %s%cC\n", paramout, DEGREE);
	rcolor;

	db_BTWC("BATT", out, NULL, paramout);
	color(GRUENh);
	printf("Battery: "); printf(paramout); printf("%%\n");
	rcolor;

	db_BTWC("HELP", out, NULL, paramout);
	color(GRUENh);
	printf("Help: "); printf(paramout); printf("\n");
	rcolor;


	color(GELBh);
	printf("\nConnection BT:\n");
	rcolor;
	db_BTWC("ROLE", out, NULL, paramout);
	color(GRUENh);
	printf("Role: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 0:printf("Peripheral"); break;
		case 1:printf("Central"); break;
		default:printf(paramout); break;
		}
	}
	else
		printf(paramout);
	printf("\"\n");
	rcolor;

	db_BTWC("UUID", out, NULL, paramout);
	color(GRUENh);
	printf("UUID of service: %s\n", paramout);
	rcolor;

	db_BTWC("RSSI", out, NULL, paramout);
	color(GRUENh);
	printf("RSSI: %s\n", paramout);
	rcolor;

	db_BTWC("ADDR", out, NULL, paramout);
	color(GRUENh);
	printf("Mac address: \""); printf(paramout); printf("\"\n");
	rcolor;

	db_BTWC("RADD", out, NULL, paramout);
	color(GRUENh);
	printf("Mac address of last connected device: \""); printf(paramout); printf("\"\n");
	rcolor;

	db_BTWC("TCON", out, NULL, paramout);
	color(GRUENh);
	printf("Time how long the connection should be hold to address: ");
	uint8_t xlen = 0;
	for (xlen = 0; paramout[xlen] != '\0'; xlen++) {
		if (paramout[xlen] != '0') {
			break;
		}
	}
	if (paramout[xlen] != '\0') {
		printf(paramout);
		printf(" ms");
	}
	else {
		printf("\"Connect forever\"");
	}
	printf("\n");
	rcolor;


	color(GELBh);
	printf("\nConnection UART:\n");
	rcolor;

	db_BTWC("NOTI", out, NULL, paramout);
	color(GRUENh);
	printf("Notify: %s\n", strcmp(paramout, "1") == 0 ? "true" : (strcmp(paramout, "0") == 0 ? "false" : "nan"));
	rcolor;

	db_BTWC("STOP", out, NULL, paramout);
	color(GRUENh);
	printf("Stopbits: ");
	if (ablechint(paramout[0])) { printf(paramout); }
	else {
		printf("%c", paramout[0] + 1);
		printf(" bit");
		if (paramout[0] + 1 != '1')
			printf("'s");
	} printf(" (recommend 2 bit's)\n");
	rcolor;

	db_BTWC("PARI", out, NULL, paramout);
	color(GRUENh);
	printf("Parity: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 0:printf("None"); break;
		case 1:printf("EVEN"); break;
		case 2:printf("ODD"); break;
		default:printf(paramout); break;
		}
	}
	else
		printf(paramout);
	printf("\" (recomment: None)\n");
	rcolor;

	db_BTWC("IMME", out, NULL, paramout);
	color(GRUENh);
	printf("At beginn: \"");
	if (!ablechint(paramout[0]))
	{
		switch (paramout[0] - '0') {
		case 1:printf("Only use AT Commands works, till the start command"); break;
		case 0:printf("Module work immediately"); break;
		default:printf(paramout); break;
		}
	}
	else
		printf(paramout);
	printf("\"\n");
	rcolor;
	color(ROTd);
	printf("Warning!! If you change \"Stopbits, Bautrate or Parity\" the module might be not ready to work!\n");
	printf("\tIf your module does not work please load (CCLoader) the \"HMSoft V540\" to the module, in order to get it work!\n");
	printf("\t\tDelete \"HM\\use.TXT\" and select \"HMSoft_V540\" for BT configuration at programm restart!\n");
	rcolor;

	printf("\n");
	for (uint8_t i = 0; i != strlen("Bluetooth configuration!!!\n") - 1; i++) {
		printf("*");
	}
	printf("\n");
	return 1;
}
void db_MC_length(uint16_t ticks, double* len_mm) {
	*len_mm = (double)(((double)((double)(ticks * 100 / DB_MC_TICKS_PER_TURN) * db_circumference_value)) / 100);
}
void data_8216(uint8_t* in, uint16_t* out) {
	*out = (in[0] << 8) + in[1];
}
void data_1628(uint16_t in, uint8_t* out) {
	out[0] = (uint8_t)(in >> 8);
	out[1] = (uint8_t)(in & 0xff);
}
bool db_EEP(uint16_t addr, uint8_t* value, bool get) {
	uint8_t paramin[3];
	paramin[2] = *value;
	uint8_t paramout[3];
	data_1628(addr, paramin);
	if (!get) {
		return db_cmd(DB_EEPROM, 3, paramin, NULL);
	}
	if (!db_cmd(DB_EEPROM, 2, paramin, paramout))
		return 0;
	*value = paramout[2];
	return 1;
}
bool db_circumference() {
	color(GELBh);
	printf("\t***Circumference settings\n");
	rcolor;
	uint8_t circumference[2];
	uint16_t circumferencex = 0;
	uint16_t circumferenceset = 0;
	int x = 0;
	if (!db_EEP(1, &circumference[0], 1))
		return 0;
	if (!db_EEP(0, &circumference[1], 1))
		return 0;
	data_8216(circumference, &circumferencex);
	color(GRUENh);
	if ((circumferencex < 180) || (circumferencex >= 230))
		printf("Circumference = %i mm (on default not saved in Memory)\n", (circumferencex = DB_MC_WHEEL_CIRCUMFERENCE));
	else
		printf("Circumference = %i mm\n", circumferencex);
	if (circumference[0] == 255 && circumference[1] == 255) {
		printf("\tThe chip was erased by this adress!\n");
	}
	db_circumference_value = circumferencex;
	char s[101] = "";
	sprintf(s, "The circumference is: %i mm\n", db_circumference_value);
	cSaddMsg(s);
	color(GELBh);
	printf("Do you want to set a other circumference (unit mm)? (y/n)\n");
	rcolor;
	if ('y' == ynrespond()) {
		circumferencex = 0;
		do {
			printf("Enter valid value: ");
			x = int_inp(180, 230 - 1);
			circumferencex = (uint16_t)x;
		} while ((circumferencex < 180) || (circumferencex >= 230));
		printf("Set value = %i\n", circumferencex);
		circumferenceset = circumferencex;
		if (db_circumference_value != circumferencex) {
			data_1628(circumferencex, circumference);
			if (!db_EEP(1, &circumference[0], 0))
				return 0;
			if (!db_EEP(0, &circumference[1], 0))
				return 0;
			color(GRUENh);
			printf("Verify...\n");
			rcolor;
			circumferencex = 0;
			if (!db_EEP(1, &circumference[0], 1))
				return 0;
			if (!db_EEP(0, &circumference[1], 1))
				return 0;
			data_8216(circumference, &circumferencex);
			sprintf(s, "\tThe circumference is changed to: %i mm\n", circumferencex);
			cSaddMsg(s);
		}
		else {
			color(GRUENh);
			printf("Writeing not nessersary!\n");
			rcolor;
		}

		color(GRUENh);
		if ((circumferencex < 180) || (circumferencex >= 230))
			printf("Circumference = %i (on default not saved in Memory)\n", (circumferencex = DB_MC_WHEEL_CIRCUMFERENCE));
		else
			printf("Circumference = %i\n", circumferencex);
		if (circumference[0] == 255 && circumference[1] == 255) {
			printf("\tThe chip was erased by this adress!\n");
		}
		rcolor;
		db_circumference_value = circumferencex;
		if (circumferenceset != circumferencex) {
			color(ROTh);
			cScError("The chip was unsuccessfully written!!!\n");
			printf("The chip was unsuccessfully written!!!\n");
		}
		else {
			color(GRUENh);
			cSaddMsg("The chip was successfully written!!!\n");
			printf("The chip was successfully written!!!\n");
		}
		rcolor;

	}
	return 1;
}
bool get_MC_Data(uint16_t* data) {
	uint8_t out[12 * 2 + 2] = { 0x46, 0x16, 0x0b, 0x2f, 0x03, 0x54, 0x0e, 0x83, 0x07, 0xf1, 0x06, 0x9d, 0x0e, 0x8e, 0x0a, 0x4e, 0x03, 0x18, 0x0e, 0x11, 0x07, 0x8f, 0x06, 0x78, 0x0e, 0xa7
	};
	//db_cmd(DB_MC_D, 0, NULL, out);
	//data[i] = data[i];

	for (uint8_t i = 0; i != 12 * 2 + 2; i++) {
		out[i] = 0;
	}
	db_cmd(DB_MC_D, 0, NULL, out);
	for (uint8_t i = 0; i != 12; i++) {
		data[i] = (out[(i * 2) + 2] << 8) + out[(i * 2) + 1 + 2];
		/*switch (i % 6) {
		case 0:printf("Dedected as forward Left:"); break;
		case 1:printf("Dedected as Backward Left:"); break;
		case 2:printf("Dedected as general Left:"); break;
		case 3:printf("Dedected as forward Right:"); break;
		case 4:printf("Dedected as Backward Right:"); break;
		case 5:printf("Dedected as general Right:"); break;
		}
		printf("Data %i %i\n", i, data[i]);*/
	}
	return 1;
}
void db_McAnaly(uint16_t* mc_dat, const char* xc) {
	color(GELBh);
	char s[101] = "";
	printf("***\tGeneral spin\n");
	rcolor;
	char dirs[2][26] = { "left","right" };
	for (uint8_t i = 0; i != 2; i++) {
		uint8_t diff = (((i == 1) ? 1 : 0) * 3);
		if ((mc_dat[0 + diff] + mc_dat[1 + diff]) == mc_dat[2 + diff]) {
			color(GRUENh);

			if (mc_dat[2 + diff] == 0) {
				color(ROTh);
			}
		}
		else
			color(ROTh);


		printf("%4i + %4i = %4i\t(%s wheel)\n", mc_dat[0 + diff], mc_dat[1 + diff], mc_dat[2 + diff], dirs[i]);
		if ((mc_dat[0 + diff] + mc_dat[1 + diff]) != mc_dat[2 + diff]) {
			//cScError("Motor forward buffer overdriven\n");
			sprintf(s, "Motor %s buffer overdriven\n\tThe wheels were to fast or wrong/not connected with the DiscBot!!!\n", xc);
			printf("\t%s", s);
			cScError(s);
		}
		if (mc_dat[2 + diff] == 0) {
			color(ROTh);
			sprintf(s, "The motor %s was not turning!\n", dirs[i]);
			printf("\t%s", s);
			cScError(s);

		}
		rcolor;
	}
	color(GRUENh);
	if (mc_dat[0 + 2] < mc_dat[3 + 2]) {
		printf("Left wheel spinned %3.3lf%% more than right!\n", 100 - percent(mc_dat[3 + 2], mc_dat[0 + 2]));
	}
	else {
		if (mc_dat[0 + 2] == mc_dat[3 + 2]) {
			printf("Both wheel's spinned exact the same distance!!\n");
		}
		else {
			printf("Right wheel spinned %3.3lf%% more than left!\n", 100 - percent(mc_dat[0 + 2], mc_dat[3 + 2]));
		}
	}
	rcolor;
	color(GELBh);
	printf("***\t%s drive\n", xc);

	double perc = 0;
	//percent(mc_dat[2], mc_dat[1]);

	if (!strcmp(xc, "Forward"))
		perc = percent(mc_dat[2], mc_dat[0]);
	else
		perc = percent(mc_dat[2], mc_dat[1]);
	const char xcv[] = "%% (should be a low value, under 50%)\n";
	sprintf(s, "%s left drive %3.3lf", xc, perc);
	strcat(s, xcv);
	if (50 > perc) {
		color(GRUENh);
		cSaddMsg(s);
	}
	else {
		color(ROTh);
		cScError(s);
	}
	printf(s);
	if (!strcmp(xc, "Forward"))
		perc = percent(mc_dat[2 + 3], mc_dat[0 + 3]);
	else
		perc = percent(mc_dat[2 + 3], mc_dat[1 + 3]);
	sprintf(s, "%s right drive %3.3lf", xc, perc);

	strcat(s, xcv);
	if (50 > perc) {
		color(GRUENh);
		cSaddMsg(s);
	}
	else {
		color(ROTh);
		cScError(s);
	}
	printf(s);
	rcolor;

}
#define LS_BYTES sizeof(DbLsNote)
#define N_P_T 38

bool db_transportSong(uint8_t len, const DbLsNote* notes, bool cat) {
	len++;
	DbLsNote noteL;
	noteL.pitch = DB_LS_LAST;
	noteL.loud = 0;
	uint8_t in[(N_P_T)*LS_BYTES + (2) * LS_BYTES];
	uint16_t lenc = 0;
	uint8_t notx[LS_BYTES + (2) * LS_BYTES];
	color(GELBh);

	for (lenc = 0; lenc != len; lenc++) {
		//printf("Frequency : %i Hz, length %i , loud: %i %%\n", notes[lenc].pitch, notes[lenc].length, notes[lenc].loud);
		printf("\t  Export of data %02.02lf %%\r", percent(len, lenc));
		struct DbLsNote notesx;
		notesx.pitch = notes[lenc].pitch;
		if (notesx.pitch > 1) {
			notesx.pitch = (uint16_t)(16000000 / notesx.pitch);
		}
		data_1628(notesx.pitch, notx);
		//notx[0] = notesx.pitch;
		//notx[1]=notesx.pitch;
		//data_1628(notes[lenc].length, &(notx[2]));
		notx[2] = notLenDB[notes[lenc].length];
		notx[3] = notes[lenc].loud;
		//printf("\n%i %i pitch %i Length %i loud\n", notx[0], notx[1], notx[2], notx[3]);
		for (uint8_t u = 0; u != LS_BYTES; u++) {
			in[u + (lenc * LS_BYTES)] = notx[u];
		}
	}
	color(GRUENh);
	printf("\t  Export of data %02.02lf %%\n", percent(len, len));
	rcolor;

	if (cat == 1)
		return db_cmd(DB_CAT_SONG, (len) * (LS_BYTES), in, NULL);
	return db_cmd(DB_COPY_SONG, (len) * (LS_BYTES), in, NULL);

}
bool db_load_song_to_bot(uint8_t num) {
	color(ROTh);
	printf("\tEXPORT song nr %i to %s\n", num + 1, PER_DEVICE);
	rcolor;
	uint16_t len = 0;
	uint16_t lenc = 0;
	for (len = 0; dbLs_ALL[num][len].pitch != DB_LS_LAST; len++);
	printf("\t**Start transport of %i package%s %i note%s", len / N_P_T + ((len % N_P_T) != 0), ((len / N_P_T + ((len % N_P_T) != 0)) > 1) ? "s," : ",", len, ((len) > 1) ? "s\n" : "\n");
	if (len > N_P_T) {
		len -= N_P_T;
		if (!db_transportSong(N_P_T, dbLs_ALL[num], 0)) return 0;
		lenc += N_P_T;
	}
	else {
		if (!db_transportSong(len, dbLs_ALL[num], 0)) return 0;
		len = 0;
	}
	while (len != 0) {
		if (len > N_P_T) {
			if (!db_transportSong(N_P_T, &dbLs_ALL[num][lenc], 1)) return 0;
			lenc += N_P_T;
			len -= N_P_T;
		}
		else {
			if (!db_transportSong(len, &dbLs_ALL[num][lenc], 1)) return 0;
			lenc += len;
			len -= len;
		}
	}
	return 1;
}
bool db_mc() {
	char s[101] = "";
	char s1[101] = "";
	char s2[101] = "";
	char s3[101] = "";
	uint16_t mc_dat[12];
	if (!db_circumference()) {
		color(ROTh);
		printf("Circumference connection error\n");
		cScError("Circumference connection error\n");
		rcolor;
		return 0;
	}
	printf("In this sketch we're testing the functionallity of the motors and it's sensors!\n");
	color(ROTd);
	printf("\tWarning!! %s will start moveing!!!\n", PER_DEVICE);
	bool forw = 0;
	bool back = 0;
	bool right = 0;
	bool left = 0;
	do {
		forw = 0;
		back = 0;
		right = 0;
		left = 0;
		color(GELBh);
		printf("Push 'x' to start!!\n");
		rcolor;
		while (_getch() != 'x');
		color(GRUENh);
		printf("Wheel spining...\n");
		rcolor;
		if (db_cmd(DB_MC, 0, NULL, NULL) == 0) {
			return 0;
		}
		color(GELBh);
		for (int i = 0; i != 1750; i++) {
			printf("\rWork in process (%2.2lf%%)", percent(1750, i));
			wait(1);
		}
		color(GRUENh);
		printf("\rWork in process (100%%)");
		rcolor;
		printf(" - Finish\n");
		color(GRUENh);
		printf("Start analystics...\n");
		rcolor;
		if (!get_MC_Data(mc_dat)) {
			return 0;
		}
		color(GELBh);
		printf("\n***\tSpin forward\n");
		cSaddMsg("\n***\tSpin forward\n");
		rcolor;
		//todo: cScError("ERROR");
		double len[12];
		for (uint8_t i = 0; i != 12; i++) {

			sprintf(s, "Receive as ");
			bool imp = 0;

			switch (i % 6) {
			case 0:strcat(s, "forward lef");
				//important not 0
				forw = 1;
				left = 1;
				if (i < 6)
					imp = 1;
				break;
			case 1:strcat(s, "backward lef");
				back = 1;
				left = 1;
				//important not 0
				if (i > 6)
					imp = 1;
				break;
			case 2:strcat(s, "general lef"); break;
			case 3:strcat(s, "forward righ");
				forw = 1;
				right = 1;
				//important not 0
				if (i < 6)
					imp = 1;
				break;
			case 4:strcat(s, "backward righ");
				//important not 0
				back = 1;
				right = 1;
				if (i > 6)
					imp = 1;
				break;
			case 5:strcat(s, "general righ"); break;
			}

			//start color
			if (!imp || mc_dat[i]) {//mc_dat!=0
				color(GRUENh);
			}
			else {
				color(ROTh);
			}
			//end color
			if ((i % 3) == 0 && (i != 0) && i != 5) {
				printf("\n");
				cSaddMsg("\n");
			}

			db_MC_length(mc_dat[i], &len[i]);
			sprintf(s1, "t:\t%4i ticks\t(turn %4.1lf mm)\n", mc_dat[i], len[i]);
			strcat(s, s1);
			printf(s);

			if (mc_dat[i] == 0 && left && forw && mc_dat[i + 1] == 0) {
				printf("Fix PE4\n");
			}
			if (mc_dat[i] == 0 && right && forw && mc_dat[i + 1] == 0) {
				printf("Fix PE5\n");
			}
			if (mc_dat[i] == 0 && imp) {
				printf("\tWheel was not turning\n");
				cScError(s);
			}
			else {
				cSaddMsg(s);

			}

			rcolor;
			if (i == 5) {
				color(GELBh);
				printf("\n***\tSpin backward");
				cSaddMsg("\n***\tSpin backward");
				rcolor;
			}
		}
		color(GRUENh);
		printf("\nMain analcstics...\n");
		color(GELBh);
		printf("***Forward spin\n");
		db_McAnaly(mc_dat, "Backward");//forward
		color(GELBh);
		printf("\n***Backward spin\n");
		db_McAnaly(&mc_dat[6], "Forward");//Backward
		rcolor;
		printf("Again? (y/n)\n");
	} while (ynrespond() == 'y');

	return 1;
}

#define _dbMc_WF_Left(a) summary[a][0][0]	//forward movement (messured)
#define _dbMc_WF_Right(a) summary[a][0][1]
#define _dbMc_WB_Left(a) summary[a][1][0]	//backward movement (messured)
#define _dbMc_WB_Right(a) summary[a][1][1]
#define _dbMc_WXM_Left(a) summary[a][2][0]	//general movement
#define _dbMc_WXM_Right(a) summary[a][2][1]
#define _dbMc_WXT_Left(a) summary[a][3][0]	//forward movment (not messured)
#define _dbMc_WXT_Right(a) summary[a][3][1]
#define _dbMc_REAL_Left(a) summary[a][4][0]	//Real debounced
#define _dbMc_REAL_Right(a) summary[a][4][1]


uint16_t mcTolerantBouncing = 25;//% backward drive
uint16_t mcTolerantReal = 10;//% movement from Real
uint16_t mcTolerantLeft2Right = 25;//% ballance
uint16_t mcAccuracy = 0;//+-
uint16_t mcMaxTicks = 1100;
uint16_t mcMinTicks = 200;

#define LEN_TEST 100001



void mc_defineSummFor(uint16_t summary[2][5][2], char c, bool r, bool b, int value) {
	uint16_t mcNorm = (mcMaxTicks + mcMinTicks) / 2;
	printf("Max = %i\n", mcMinTicks);
	printf("Min = %i\n", mcMaxTicks);
	printf("Normal = %i\n", mcNorm);
	const char site[2][6] = { "left","right" };
	const char dir[2][5] = { "for","back" };
	color(GELBh);
	printf("WARNING thiese values are only for simulation properties!\n");
	rcolor;
	switch (c) {
	case 'b':
		printf("Bouncing wheel %s %sward\n", site[r], dir[b]);
		if (!r) {
			if (!b)
				_dbMc_WF_Left(b);
			else
				_dbMc_WB_Left(b);
		}
		break;
	default:color(ROTh); printf("\nINVALID VALUE \"%c\" for simulation\n", c); break;
	}
	rcolor;
}
void mc_text_output(uint16_t summary[2][5][2]) {
	char text[1000] = "";
	char text1[100] = "";
	sprintf(text, "\n\n********************\n");
	sprintf(text, "*****RAW OUTPUT*****\n");
	sprintf(text, "********************\n");
	sprintf(text1, "Normal messured movement\n");
	strcat(text, text1);
	sprintf(text1, "\tLeft forward: %i\n", _dbMc_WF_Left(0));
	strcat(text, text1);
	sprintf(text1, "\tLeft forward: -%i\n", _dbMc_WF_Left(1));
	strcat(text, text1);
	sprintf(text1, "\tLeft backward: -%i\n", _dbMc_WB_Left(0));
	strcat(text, text1);
	sprintf(text1, "\tLeft backward: %i\n", _dbMc_WB_Left(1));
	strcat(text, text1);
	sprintf(text1, "\n");
	strcat(text, text1);
	sprintf(text1, "\tRight forward: %i\n", _dbMc_WF_Right(0));
	strcat(text, text1);
	sprintf(text1, "\tRight forward: -%i\n", _dbMc_WF_Right(1));
	strcat(text, text1);
	sprintf(text1, "\tRight backward: -%i\n", _dbMc_WB_Right(0));
	strcat(text, text1);
	sprintf(text1, "\tRight backward: %i\n", _dbMc_WB_Right(1));
	strcat(text, text1);
	sprintf(text1, "\n");
	strcat(text, text1);
	sprintf(text1, "General messured movement\n");
	strcat(text, text1);
	sprintf(text1, "\tLeft forward: %i\n", _dbMc_WXM_Left(0));
	strcat(text, text1);
	sprintf(text1, "\tLeft backward: %i\n", _dbMc_WXM_Left(1));
	strcat(text, text1);
	sprintf(text1, "\tRight forward: %i\n", _dbMc_WXM_Right(0));
	strcat(text, text1);
	sprintf(text1, "\tRight backward: %i\n", _dbMc_WXM_Right(1));
	strcat(text, text1);
	sprintf(text1, "\n");
	strcat(text, text1);
	sprintf(text1, "Not normal messured forward movement\n");
	strcat(text, text1);
	sprintf(text1, "\tLeft forward: %i\n", _dbMc_WXT_Left(0));
	strcat(text, text1);
	sprintf(text1, "\tLeft backward: -%i\n", _dbMc_WXT_Left(1));
	strcat(text, text1);
	sprintf(text1, "\tRight forward: %i\n", _dbMc_WXT_Right(0));
	strcat(text, text1);
	sprintf(text1, "\tRight backward: -%i\n", _dbMc_WXT_Right(1));
	strcat(text, text1);
	sprintf(text1, "\n");
	strcat(text, text1);
	sprintf(text1, "Debounced messurement of general movement\n");
	strcat(text, text1);
	sprintf(text1, "\tLeft forward: %i\n", _dbMc_REAL_Left(0));
	strcat(text, text1);
	sprintf(text1, "\tLeft backward: %i\n", _dbMc_REAL_Left(1));
	strcat(text, text1);
	sprintf(text1, "\tRight forward: %i\n", _dbMc_REAL_Right(0));
	strcat(text, text1);
	sprintf(text1, "\tRight backward: %i\n", _dbMc_REAL_Right(1));
	strcat(text, text1);
	sprintf(text1, "\n");
	strcat(text, text1);
	printf(text);
	/*FILE* f;
	f = fopen("output.txt", "a+");
	if (f == NULL)
	{
		color(ROTh);
		printf("File can't created!\n");
		rcolor;
		return;
	}
	fprintf(f, text);
	fclose(f);*/
}
#define CABLE_CLERANCE 25
#define CABLE_CHARACTER 219

#define MC_NOT_TESTED 0
#define MC_OK 1
#define MC_WARN 2
#define MC_NOK 3

#define CABLE_LINE(a) for(char ii=0;ii!=8;ii++){for(char iii=0;iii!=2;iii++){for (char i = 0; i != CABLE_CLERANCE; i++)color((!((i - 2) % 4)) ? a[i / 4] : SCHWARZ), printf("%c", CABLE_CHARACTER); if(iii==0)printf("\t");else printf("\n");}}
#define CABLE_WHITE_LINE color(WEISSd);for(char ii=0;ii!=2;ii++){for (char i = 0; i != CABLE_CLERANCE; i++)printf("%c", CABLE_CHARACTER);if(ii==0)printf("\t");else printf("\n");}rcolor
void print_MC_cable(char check[2][6]) {
	rcolor;
	printf("\t%sMotor cable pinout%s!!\n\n", (check == NULL) ? "\t" : "   ", (check == NULL) ? "" : " and analyse");
	color(GRUENh);
	printf(" ***CABLE LEFT WHEEL ***\t ***CABLE RIGHT WHEEL***\n");
	rcolor;

	char col[6] = { ROTh,GRAU,GELBh,GRUENd,BLAUh,WEISSh };
	char warn[4] = { GRAU,GRUENh,GELBh,ROTh };
	printf("\n");
	printf(" M2A +3V PE5 PK5 GND M1A \t M1B +3V PE4 PK4 GND M2B \n");
	//printf("12*   *123*   *   *   *12\n");
	CABLE_WHITE_LINE;

	for (char ii = 0; ii != 2; ii++) {
		for (char i = 0; i != CABLE_CLERANCE; i++)
			color((!((i - 2) % 4)) ? ((check != NULL) ? warn[check[ii][i / 4]] : GRAU) : WEISSd), printf("%c", CABLE_CHARACTER);
		if (ii == 0)
			printf("\t");
	}rcolor;
	printf("\tOriginal\t(At board)\n");
	CABLE_WHITE_LINE;
	CABLE_LINE(col);
	CABLE_WHITE_LINE;
	for (char ii = 0; ii != 2; ii++) {
		for (char i = 0; i != CABLE_CLERANCE; i++)
			color((((i / 4) == 1 || (i / 4) == 4) && (i % 4) == 2) ? WEISSh : WEISSd), printf("%c", CABLE_CHARACTER);
		if (ii == 0)
			printf("\t");
	}
	rcolor;
	printf("\tCreated\t\t(At motor)\n");
	CABLE_WHITE_LINE;
	printf(" M2A +3V E1  E2  GND M1A \t MB1 +3V E1  E2  GND MB2 \n");

	rcolor;
	printf("\n\n\n");
	for (char i = 0; i != 6; i++) {
		color(col[i]);
		printf("%c ", CABLE_CHARACTER);
		rcolor;
		switch (5 - i) {
		case 0:printf("White\t: transport the power (with polarity) to the motor"); break;
		case 1:printf("Blue\t: transport the power (+3V) to the decoder & LED's"); break;
		case 2:printf("Green\t: decoder's signal for controller (E1)"); break;
		case 3:printf("Yellow: decoder's signal for controller (E2)"); break;
		case 4:printf("Black\t: ground"); break;
		case 5:printf("Red\t: transport the power (with polarity) to the motor"); break;
		default:printf("INTERNAL ERROR!!!!\n"); break;
		}
		printf("\n");
	}
	printf("\n\n\n");
	if (check == NULL)
		return;
	rcolor;
	printf("\t**Status\n");
	for (char i = 0; i != 4; i++) {
		color(warn[i]);
		printf("%c ", CABLE_CHARACTER);
		rcolor;
		switch (i) {
		case 0:printf("Silver: cable not tested, too much errors for clear results"); break;
		case 1:printf("Green\t: cable ok"); break;
		case 2:printf("Yellow: warning you should check this connection"); break;
		case 3:printf("Red\t: this connection is insecure, check the wired connection immediately"); break;
		default:printf("INTERNAL ERROR!!!!\n"); break;
		}
		printf("!\n");
	}
	printf("\n\n\n");
}
#define MC_WRONG_DRIVE_POLARITY 0
#define MC_CORRECT_DRIVE 1
#define MC_WRONG_DRIVE 2
#define MC_WARNING 3
#define MC_E_CONNECT 4

#define _dbMc_Wd(a) summary[a]			//forward movement (messured)   _dbMc_Wd(0)[b][w];
#define _dbMc_WXM(a) summary[a][2]	//general movement
#define _dbMc_WXT(a) summary[a][3]	//forward movment (not messured)
#define _dbMc_REAL(a) summary[a][4]	//Real debounced
void mc_status(bool show, bool res) {
	if (show) {
		color(GELBh);
		printf("+++Status: ");
		if (res)
		{
			color(GRUENh);
			printf("PASS");
		}
		else {
			color(ROTh);
			printf("FAIL");
		}
		printf("\n");
		rcolor;
	}
}
bool _MC_accuracy(uint16_t summary[2][5][2], bool show, bool wheel, bool towheel, char* result) {
	bool res = 1;
	const char sdir[2][5] = { "for","back" };
	const char swheel[2][6] = { "left","right" };//Ticks %4i (%6.1lf) %5s-wheel %4sward (%7s)\t
	char mystatus[2][2] = { 0,0,0,0 };
	uint16_t minMax[2][2][2] = { 0,0,0,0,0,0,0,0 };
	const char smystatus[4][13] = { "not measured","ok","too high","too low" };
	if (show) {
		color(GELBh);
		printf("\t\t+++Valid data check\n");
		rcolor;
	}
	//general ==forward +backward
	uint16_t sum = 0;

	for (uint8_t d = 0; d != 2; d++) {
		for (uint8_t w = wheel; w != (towheel + 1); w++)
		{
			minMax[d][w][0] = _dbMc_WXM(d)[w] - mcAccuracy;
			minMax[d][w][1] = _dbMc_WXM(d)[w] + mcAccuracy;
			sum = _dbMc_Wd(d)[0][w] + _dbMc_Wd(d)[1][w];
			if (sum > minMax[d][w][1]) {
				mystatus[d][w] = 2;
			}
			else
				if (sum < minMax[d][w][0]) {
					mystatus[d][w] = 3;
				}
				else
					mystatus[d][w] = 1;
			if (show)
			{
				switch (mystatus[d][w]) {
				case 1:color(GRUENh); break;
				case 2:
				case 3:color(ROTh); break;
				default:color(GELBh); break;
				}
				printf("%s-wheel %4sward %4i= %4i + %4i (general = for + backward)(min %4i, max %4i, +-%3i) (%7s)\t", swheel[w], sdir[d], _dbMc_WXM(d)[w], _dbMc_Wd(d)[0][w], _dbMc_Wd(d)[1][w], minMax[d][w][0], minMax[d][w][1], mcAccuracy, smystatus[mystatus[d][w]]);
				rcolor;
			}
		}
		if (show)
			printf("\n");
	}
	for (char b = 0; b != 2 && res; b++) {
		for (char d = 0; d != 2 && res; d++) {
			for (char w = 0; w != 2 && res; w++) {
				res = !(mystatus[d][w] > 1);
			}
		}
	}
	mc_status(show, res);
	return res;
	return 1;
}
bool _MC_balancer(uint16_t summary[2][5][2], bool show, bool wheel, bool towheel, char* result) {
	bool res = 1;
	const char sdir[2][10] = { "for","back" };
	const char swheel[2][10] = { "left","right" };//Ticks %4i (%6.1lf) %s-wheel %4sward (%7s)\t
	char mystatus[2] = { 0,0 };
	double myvalue[2] = { 0,0 };
	uint16_t minMax[2][2] = { 0,0,0,0, };
	const char smystatus[4][13] = { "not measured","ok","too high","too low" };
	if (show) {
		color(GELBh);
		printf("\t\t+++Ballancer check\n");
		rcolor;
	}

	//Left to right
	if (show) {
		color(GELBh);
		printf("\t++General real values\n");
		rcolor;
	}
	for (uint8_t d = 0; d != 2; d++) {
		for (uint8_t w = wheel; w != towheel + 1; w++) {
			if (_dbMc_REAL(d)[0] > _dbMc_REAL(d)[1]) {
				minMax[d][0] = _dbMc_REAL(d)[0] * (mcTolerantLeft2Right) / 100;
				minMax[d][1] = _dbMc_REAL(d)[0] * (100 + mcTolerantLeft2Right) / 100;

				if (_dbMc_REAL(d)[1] > minMax[d][1]) {
					mystatus[d] = 2;
					myvalue[d] = (double)100 * _dbMc_REAL(d)[1] / minMax[d][1] - 100;
				}
				else if (_dbMc_REAL(d)[1] < minMax[d][0]) {
					mystatus[d] = 3;
					myvalue[d] = 100 - (double)100 * _dbMc_REAL(d)[1] / minMax[d][0];
				}
				else {
					mystatus[d] = 1;
					myvalue[d] = 0;
				}
			}
			else if (_dbMc_REAL(d)[1] < _dbMc_REAL(d)[0]) {
				minMax[d][0] = _dbMc_REAL(d)[1] * (mcTolerantLeft2Right) / 100;
				minMax[d][1] = _dbMc_REAL(d)[1] * (100 + mcTolerantLeft2Right) / 100;

				if (_dbMc_REAL(d)[0] > minMax[d][1]) {
					mystatus[d] = 2;
					myvalue[d] = (double)100 * _dbMc_REAL(d)[0] / minMax[d][1] - 100;
				}
				else if (_dbMc_REAL(d)[0] < minMax[d][0]) {
					mystatus[d] = 3;
					myvalue[d] = 100 - (double)100 * _dbMc_REAL(d)[0] / minMax[d][0];
				}
				else {
					mystatus[d] = 1;
					myvalue[d] = 0;
				}
			}
			else {
				minMax[d][1] = _dbMc_REAL(d)[1] * (100 + mcTolerantLeft2Right) / 100;
				minMax[d][0] = _dbMc_REAL(d)[1] * (mcTolerantLeft2Right) / 100;
				mystatus[d] = 1;
				myvalue[d] = 0;
			}

			if (mystatus[d] == 3)
				res = 0;
			if (show) {

				if (mystatus[d] > 2)
					color(ROTh);
				else
					color(GRUENh);
				printf("%4sward %s-wheel (l = %4i, r = %4i) (min %4i, max %4i) %5.3lf%% %7s\t", sdir[d], swheel[w], _dbMc_REAL(d)[0], _dbMc_REAL(d)[1], minMax[d][0], minMax[d][1], myvalue[d], smystatus[mystatus[d]]);
				rcolor;
			}
		}
		if (show)
			printf("\n");
	}
	if (show) {
		color(GELBh);
		printf("\t++General messured values (no matter for errors; just if cables crashes)\n");
		rcolor;
	}
	for (uint8_t d = 0; d != 2; d++) {
		for (uint8_t w = wheel; w != towheel + 1; w++) {
			if (_dbMc_WXM(d)[0] > _dbMc_WXM(d)[1]) {
				minMax[d][0] = _dbMc_WXM(d)[0] * (mcTolerantLeft2Right) / 100;
				minMax[d][1] = _dbMc_WXM(d)[0] * (100 + mcTolerantLeft2Right) / 100;
				
				if (_dbMc_WXM(d)[1] > minMax[d][1]) {
					mystatus[d] = 2;
					myvalue[d] = (double)100 * _dbMc_WXM(d)[1] / minMax[d][1] - 100;
				}
				else if (_dbMc_WXM(d)[1] < minMax[d][0]) {
					mystatus[d] = 3;
					myvalue[d] = 100 - (double)100 * _dbMc_WXM(d)[1] / minMax[d][0];
				}
				else {
					mystatus[d] = 1;
					myvalue[d] = 0;
				}
			}
			else if (_dbMc_WXM(d)[1] > _dbMc_WXM(d)[0]) {
				minMax[d][0] = _dbMc_WXM(d)[1] * (mcTolerantLeft2Right) / 100;
				minMax[d][1] = _dbMc_WXM(d)[1] * (100 + mcTolerantLeft2Right) / 100;

				if (_dbMc_WXM(d)[1] > minMax[d][1]) {
					mystatus[d] = 2;
					myvalue[d] = (double)100 * _dbMc_WXM(d)[1] / minMax[d][1] - 100;
				}
				else if (_dbMc_WXM(d)[1] < minMax[d][0]) {
					mystatus[d] = 3;
					myvalue[d] = 100 - (double)100 * _dbMc_WXM(d)[1] / minMax[d][0];
				}
				else {
					mystatus[d] = 1;
					myvalue[d] = 0;
				}
			}
			else {
				minMax[d][1] = _dbMc_WXM(d)[1] * (100 + mcTolerantLeft2Right) / 100;
				minMax[d][0] = _dbMc_WXM(d)[1] * (mcTolerantLeft2Right) / 100;
				mystatus[d] = 1;
				myvalue[d] = 0;
			}
			if (show) {

				if (mystatus[d] > 1)
					color(ROTh);
				else
					color(GRUENh);
				printf("%4sward %s-wheel (l = %4i, r = %4i) (min %4i, max %4i) %5.3lf%% %7s\t", sdir[d], swheel[w], _dbMc_WXM(d)[0], _dbMc_WXM(d)[1], minMax[d][0], minMax[d][1], myvalue[d], smystatus[mystatus[d]]);
				rcolor;
			}
		}
		if (show)
			printf("\n");
	}

	mc_status(show, res);
	return res;
}
bool _MC_debouncer(uint16_t summary[2][5][2], bool show, bool wheel, bool towheel, char* result) {
	bool res = 1;
	const char sdir[2][10] = { "for","back" };
	const char swheel[2][10] = { "left","right" };//Ticks %4i (%6.1lf) %s-wheel %4sward (%7s)\t
	char mystatus[2][2] = { 0,0,0,0 };
	float mypercent[2][2] = { 0,0,0,0 };
	uint16_t minMax[2][2][2] = { 0,0,0,0,0,0,0,0 };
	const char smystatus[4][13] = { "not measured","ok","too high","too low" };
	if (show) {
		color(GELBh);
		printf("\t\t+++Advanced debounce detection\n");
		rcolor;
	}
	for (uint8_t d = 0; d != 2; d++) {
		if (show) {
			color(GELBh);
			printf("\t++%4sward spin (general wheel spin values)\n", sdir[d]);
			rcolor;
		}
		for (uint8_t w = wheel; w != (towheel + 1); w++) {
			mystatus[d][w] = 1;
			minMax[d][w][0] = _dbMc_WXM(d)[w];
			minMax[d][w][1] = ((_dbMc_WXM(d)[w] * (100 + mcTolerantReal)) / 100);
			if (minMax[d][w][1] < _dbMc_WXM(d)[w]) {
				res = 0;
				mystatus[d][w] = 2;
			}
			else
				if (minMax[d][w][0] > _dbMc_WXM(d)[w]) {
					res = 0;
					mystatus[d][w] = 3;
				}

			if (show) {
				if (_dbMc_REAL(d)[w] > minMax[d][w][1]) {
					mystatus[d][w] = 2;
					mypercent[d][w] = 100 * _dbMc_REAL(d)[w] / minMax[d][w][1] - 100;
				}
				else if (_dbMc_REAL(d)[w] < minMax[d][w][0]) {
					mystatus[d][w] = 3;
					mypercent[d][w] = 100 - (double)100 * _dbMc_REAL(d)[w] / minMax[d][w][0];
				}
				else {
					mystatus[d][w] = 1;
					mypercent[d][w] = 0;
				}

				switch (mystatus[d][w]) {
				case 1:color(GRUENh); break;
				case 2:
				case 3:color(ROTh); break;
				default:color(GELBh); break;
				}
				double mclength[2] = { 0,0 };
				db_MC_length(_dbMc_REAL(d)[w], mclength);
				db_MC_length(_dbMc_WXM(d)[w], &(mclength[1]));
				printf("Ticks real %s-wheel: %4i (%6.1lf mm) messured: %4i (%6.1lf mm) (min %3i, max %4i (%5.3f%% %7s, max +-%2i%%))\t", swheel[w], _dbMc_REAL(d)[w], mclength[0], _dbMc_WXM(d)[w], mclength[1], minMax[d][w][0], minMax[d][w][1], mypercent[d][w], smystatus[mystatus[d][w]], mcTolerantReal);
				if (wheel != towheel) {
					printf("\n");
					printx('\t', 13);
				}
				rcolor;
			}
		}
		if (show)
			printf("\n");
	}
	mc_status(show, res);
	return res;
}
#define MC_WD	0b00000001
#define MC_OVER	0b0000010
bool _MC_minMaxer(uint16_t summary[2][5][2], bool show, bool wheel, bool towheel, char* result) {
	bool res = 1;
	const char sdir[2][10] = { "for","back" };
	const char swheel[2][10] = { "left","right" };
	char mystatus[2][2][2] = { 0,0,0,0,0,0,0,0 };
	int mypercent[2][2][2] = { 0,0,0,0,0,0,0,0 };
	uint16_t minMax[2][2][2][2] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	const char smystatus[4][13] = { "not measured","ok","too high","too low" };
	char text[300] = "";
	if (show) {
		color(GELBh);
		printf("\t\t+++Maximum, minimum and simple bounce Checker!+++\n");
		rcolor;
	}
	for (uint8_t d = 0; d != 2; d++) {
		if (show) {
			color(GELBh);
			printf("\t++%sward spin\n", sdir[d]);//Spin Car
			rcolor;
		}
		for (uint8_t b = 0; b != 2; b++) {//wheel spin "b means backward=0"

			for (uint8_t w = wheel; w != towheel + 1; w++) {//wheel
				if (d == 0) {//Car spin
					if (b == 0) {//wheel spin
						//forward high backward minimum
						minMax[d][b][w][0] = mcMinTicks;
						minMax[d][b][w][1] = mcMaxTicks;
						if (_dbMc_Wd(d)[b][w] > mcMaxTicks) {
							mystatus[d][b][w] = 2;
							mypercent[d][b][w] = 100 * _dbMc_Wd(d)[b][w] / mcMaxTicks - 100;
							*result |= MC_OVER;
						}
						else
							if (_dbMc_Wd(d)[b][w] < mcMinTicks) {
								mystatus[d][b][w] = 3;
								mypercent[d][b][w] = 100 - 100 * _dbMc_Wd(d)[b][w] / mcMinTicks;
								*result |= MC_OVER;
							}
							else
								mystatus[d][b][w] = 1;
					}
					else {
						minMax[d][b][w][0] = 0;
						minMax[d][b][w][1] = ((_dbMc_Wd(d)[!b][w] * (mcTolerantBouncing)) / 100);
						if (_dbMc_Wd(d)[b][w] > minMax[d][b][w][1]) {
							mystatus[d][b][w] = 2;
							mypercent[d][b][w] = 100 * _dbMc_Wd(d)[b][w] / minMax[d][b][w][1] - 100;
							*result |= MC_WD;
						}
						else
							mystatus[d][b][w] = 1;
					}
				}
				else {
					if (b != 0) {//wheel spin
						minMax[d][b][w][0] = mcMinTicks;
						minMax[d][b][w][1] = mcMaxTicks;
						if (_dbMc_Wd(d)[b][w] > mcMaxTicks) {
							mystatus[d][b][w] = 2;
							mypercent[d][b][w] = 100 * _dbMc_Wd(d)[b][w] / mcMaxTicks - 100;
							*result |= MC_OVER;
						}
						else
							if (_dbMc_Wd(d)[b][w] < mcMinTicks) {
								mystatus[d][b][w] = 3;
								mypercent[d][b][w] = 100 - 100 * _dbMc_Wd(d)[b][w] * mcMinTicks;
								*result |= MC_OVER;
							}
							else
								mystatus[d][b][w] = 1;
					}
					else {
						minMax[d][b][w][0] = 0;
						minMax[d][b][w][1] = ((_dbMc_Wd(d)[!b][w] * (mcTolerantBouncing)) / 100);
						if (_dbMc_Wd(d)[b][w] > minMax[d][b][w][1]) {
							mystatus[d][b][w] = 2;
							mypercent[d][b][w] = 100 * _dbMc_Wd(d)[b][w] / minMax[d][b][w][1] - 100;
							*result |= MC_WD;
						}
						else
							mystatus[d][b][w] = 1;
					}
				}
				if (show) {
					switch (mystatus[d][b][w]) {
					case 1:color(GRUENh); break;
					case 2:
					case 3:color(ROTh); break;
					default:color(GELBh); break;
					}
					if (mypercent[d][b][w]) {
						sprintf(text, " (This value is %3i%% %5s the norm)", mypercent[d][b][w], (mystatus[d][b][w] == 2) ? "over" : "under");
					}
					else
						text[0] = '\0';
					double mclength = 0;
					db_MC_length(_dbMc_Wd(d)[b][w], &mclength);
					printf("Ticks %4i (min %4i, max %4i) (%6.1lf mm) %s-wheel %4sward (%7s)%38s\t", _dbMc_Wd(d)[b][w], minMax[d][b][w][0], minMax[d][b][w][1], mclength, swheel[w], sdir[b], smystatus[mystatus[d][b][w]], text);
					rcolor;
				}
			}
			if (show)
				printf("\n");
		}
	}
	for (char b = 0; b != 2 && res; b++) {
		for (char d = 0; d != 2 && res; d++) {
			for (char w = 0; w != 2 && res; w++) {
				res = !(mystatus[d][b][w] > 1);
			}
		}
	}
	if (show) {
		if (*result & MC_WD) {
			color(ROTh);
			strcpy(text, " not");
		}
		else
		{
			color(GRUENh);
			strcpy(text, "");
		}
		printf("\tThe wheels where%s enought driven in the correct direction!\n", text);
		rcolor;
		if (*result & MC_OVER) {
			color(ROTh);
			strcpy(text, "");
		}
		else
		{
			color(GRUENh);
			strcpy(text, " not");
		}
		printf("\tThe wheels where%s overdriven!\n", text);
		rcolor;
	}
	mc_status(show, res);
	return res;
}
const char mcDecider_steps = 4;

int mcDecider(uint16_t summary[2][5][2], bool show, bool wheel, bool towheel) {
	char results[mcDecider_steps + 1] = { 0 };
	if (show)
	{
		color(GELBh);
		printf("\t\t\t++++MOTOR PROTOCOL++++\n");
		rcolor;
	}
	const char steps = mcDecider_steps;
	bool stR[steps] = { 0 };
	uint8_t sum = 0;
	for (uint8_t i = 0; i != steps; i++)
		stR[i] = 0;

	typedef bool (*fcx)(uint16_t summary[2][5][2], bool show, bool wheel, bool towheel, char* result);
	fcx func;
	void* fu[steps] = { _MC_minMaxer ,_MC_debouncer ,_MC_balancer ,_MC_accuracy };
	/*stR[0] = _MC_minMaxer(summary, show, wheel, towheel);
	stR[1] = _MC_debouncer(summary, show, wheel, towheel);
	stR[2] = _MC_balancer(summary, show, wheel, towheel);
	func;*/
	for (uint8_t i = 0; i != steps; i++) {
		func = (fcx)fu[i];
		results[i] = 0;
		stR[i] = func(summary, show, wheel, towheel, &(results[i]));
		sum += stR[i];
	}

	if (show)
		printf("\tResult %i from %i passed\n", sum, steps);
	if (sum == steps)
		return MC_CORRECT_DRIVE;
	if (!stR[3]) {//0
		return MC_E_CONNECT;
	}
	if (results[0] & MC_WD) {//1
		return MC_WRONG_DRIVE_POLARITY;
	}
	if (!stR[2]) {//3
		return MC_WRONG_DRIVE;
	}
	if (results[0] & MC_OVER || !stR[2]) {//1 3
		return MC_WARNING;
	}

	return -1;
}
char mcFBanalyser(uint16_t summary[2][5][2], bool l, bool show) {
	char dir[2][6] = { "left","right" };
	int result = mcDecider(summary, show, l, l);
	char text[1000] = "";
	int col = 0;
	switch (result) {
	case MC_CORRECT_DRIVE://No errors
		col = GRUENh;
		sprintf(text, "OK! \"no error\" No Errors found!");
		break;
	case MC_WRONG_DRIVE://unregularity
		col = GELBh;
		sprintf(text, "WARNING! \"to low %s wheel balance\" Make shure the wheel is running whithout obstacles! Look at the advanced description, at least for bouncing errors!", dir[l]);
		break;
	case MC_WRONG_DRIVE_POLARITY://to high percent false direction
		col = ROTh;
		sprintf(text, "ERROR! \"high percent in false direction\" Wrong polarity (E1, E2), if wheels turned correctly. Otherwise wrong polarity at (M1, M2)!");
		break;
	case MC_WARNING://to low or to high drive value
		col = GELBh;
		sprintf(text, "WARNING! \"invalid ticks\" Unsave connections (bouncing)! Look at the advanced description!");
		break;
	case MC_E_CONNECT://Data were not right transmitted
		col = GELBh;
		sprintf(text, "ERROR! \"invalid data\" Unsave connection to %s (COM/USB bit's)! Look at the advanced description and try again!", PER_DEVICE);
		break;
	default://internal error
		col = ROTh;
		sprintf(text, "UNDEFINED ERROR! Look at the advanced description to find your error!");
		break;
	}
	color(VIOLETTh);
	printf("\t***Analyser %s: ", dir[l]);
	color(col);
	printf("%s", text);
	color(VIOLETTh);
	printf("***\n");
	rcolor;
	rcolor;
	return result;
}
bool mc_Data_Converter(uint16_t summary[2][5][2]) {
	uint8_t param[40 + 5];
	if (!db_cmd(DB_MC_D, 0, NULL, param))
		return 0;
	if (param[1] != 42)
	{
		color(VIOLETTh);
		printf("Try the old mode!\n\tOtherwise you will recive an error, which isn't true! (y/n)\n");
		rcolor;
		if (ynrespond() == 'y')
			db_mc();
		else
			return 0;
	}
	uint8_t* x;
	x = &(param[0]);
	for (uint8_t i = 0; i != 10; i++)
	{
		data_8216(x += 2, &summary[0][i / 2][(i % 2)]);
		//	uartnprintf(0,2,(uint8_t)(summary[0][i/2][(i%2)] >> 8), (uint8_t)(summary[0][i/2][(i%2)] & 0xff));
	}
	for (uint8_t i = 0; i != 10; i++)
	{
		data_8216(x += 2, &summary[1][i / 2][(i % 2)]);
		//	uartnprintf(0,2,(uint8_t)(summary[1][i/2][(i%2)] >> 8), (uint8_t)(summary[1][i/2][(i%2)] & 0xff));
	}
	/*for (uint8_t i = 0; i != 10; i++) {
		if (summary[0][i / 2][(i % 2)] != i * 60) {
			color(ROTh);
			printf("Error Data0 capture is %i should %i\n", summary[0][i / 2][(i % 2)],i*60);
			rcolor;
		}
		else {
			color(GRUENh);
			printf("OK Data0 capture\n");
			rcolor;
		}
	}
	for (uint8_t i = 0; i != 10; i++) {
		if (summary[1][i / 2][(i % 2)] != i * 300) {
			color(ROTh);
			printf("Error Data0 capture is %i should %i\n", summary[1][i / 2][(i % 2)], i * 300);
			rcolor;
		}
		else {
			color(GRUENh);
			printf("OK Data1 capture\n");
			rcolor;
		}
	}*/
	return 1;
}

#define MC_PRESET dir_MPreset("preset")
#define MC_SIMULATION dir_MPreset("tests")
#define MC_GENERAL dir_MPreset(NULL)
#define MC_NAMES_BF_SIZE 10001
bool mcAddPreset(uint16_t* param, const char* name) {
	const int mparam = 6;
	//uint16_t* param[mparam] = { &mcMaxTicks ,&mcMinTicks ,&mcTolerantBouncing ,&mcTolerantReal ,&mcTolerantLeft2Right,&mcAccuracy };

	//*param[i] = param[i];
	char pname[101] = "";
	FILE* f;
	strcpy(pname, MC_PRESET);
	strcat(pname, "//");
	strcat(pname, name);
	strcat(pname, ".pre");
	if ((f = fopen(pname, "w+")) == NULL) {
		color(ROTh);
		printf("File \"%s\" can't open!\n", pname);
		rcolor;
		return 0;
	}
	for (uint8_t i = 0; i != mparam; i++) {
		fprintf(f, "%i\n", param[i]);
	}
	fclose(f);
	return 1;
}
void _MC_param_text(uint16_t param[6]) {
	printf("\tMax drive ticks (in correct direction): %4i\n", param[0]);
	printf("\tMin drive ticks (in correct direction): %4i\n", param[1]);
	printf("\tMax drive ticks (in wrong direction): +%3i%% to correct direction\n", param[2]);
	printf("\tDrive ticks real (debounced) to normal: +%3i%%\n", param[3]);
	printf("\tDriver ballanced left to right: +-%3i%%\n", param[4]);
	printf("\tAccuracy for valid data (to dedect an overrun): +-%3i\n", param[5]);
}
bool mcShowprest(const char* name, bool show, uint16_t param[6], uint8_t col) {
	char* pname = NULL;
	uint16_t vparam[6] = { 0 };
	pname = (char*)malloc(strlen(name) + strlen(MC_PRESET) + 10);
	if (pname == NULL)
	{
		color(ROTh);
		printf("No free RAM is available!!!\n");
		rcolor;
		return 0;
	}
	strcpy(pname, MC_PRESET);
	strcat(pname, "\\");
	strcat(pname, name);
	strcat(pname, ".pre");
	FILE* f;
	if ((f = fopen(pname, "r")) == NULL) {
		if (show) {
			color(ROTh);
			printf("File \"%s\" does not exist!!!\n", pname);
			rcolor;
		}
		free(pname);
		return 0;
	}
	rewind(f);
	for (uint8_t i = 0; i != 6; i++) {
		fscanf(f, "%i", &vparam[i]);
	}
	free(pname);
	if (show) {
		color(col);
		printf("Presets of \"%s\"\n", name);
		rcolor;
		_MC_param_text(vparam);
	}
	if (param != NULL)
	{
		for (uint8_t i = 0; i != 6; i++) {
			param[i] = vparam[i];
		}
	}
	return 1;
}
bool generateMC() {
	DIR* dir;
	char text[101] = "";
	char sdir[101]="";
	strcpy(sdir,MC_GENERAL);
	if (!generateDir(sdir))
		return 0;

	strcpy(text, MC_PRESET);
	if (!generateDir(text))
		return 0;

	strcpy(text, MC_SIMULATION);
	if (!generateDir(text))
		return 0;

	strcpy(text, MC_PRESET);
	strcat(text, "\\Default.pre");
	if (!fileExist(text)) {
		const uint8_t mparam = 6;
		uint16_t param[mparam] = {
			mcMaxTicks ,mcMinTicks ,mcTolerantBouncing ,
			mcTolerantReal ,mcTolerantLeft2Right,mcAccuracy
		};
		return mcAddPreset(param, "Default");
	}
	return 1;
}
unsigned int _MC_name_preset(char* names) {
	DIR* dir;
	struct dirent* ent;
	char text[MC_NAMES_BF_SIZE] = "";
	char text1[MC_NAMES_BF_SIZE] = "";
	unsigned int cnt = 0;
	if (!generateDir(MC_PRESET, &dir)) {
		color(ROTh);
		printf("Dir error!!!\n");
		rcolor;
		return 0;
	}
	strcpy(text, MC_PRESET);
	strcat(text, "\\Default.pre");
	if (fileExist(text) != NULL) {
		cnt = 1;
		strcat(text1, "Default:");
	}
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] != '.') {
			if (!strcmp((char*)".pre", &(ent->d_name[ent->d_namlen - 4]))) {

				//	printf("name \"%s\"end \"%s\" %i \n", ent->d_name, &(ent->d_name[ent->d_namlen - 4]), strcmp((char*)".pre", &(ent->d_name[ent->d_namlen - 4])));
				if (strcmp(ent->d_name, "Default.pre")) {
					strcpy(text, ent->d_name);
					text[ent->d_namlen - 4] = '\0';
					++cnt;
					strcat(text1, text);
					strcat(text1, ":");
				}
				//printf("\"%s\"\n", ent->d_name);
			}
		}
	}
	closedir(dir);
	//printf("%i presets are available\n", cnt);
	if (names != NULL) {
		strcpy(names, text1);
	}
	return cnt;
}
void _MC_param_show();

void _MC_param_change(uint16_t param1[6]) {
	rcolor;
	const int mparam = 6;
	uint16_t* param[mparam] = { &mcMaxTicks ,&mcMinTicks ,&mcTolerantBouncing ,&mcTolerantReal ,&mcTolerantLeft2Right,&mcAccuracy };

	uint16_t paramx[mparam] = { 0 };
	if (param1 != NULL) {
		for (uint8_t i = 0; i != mparam; i++)
			paramx[i] = param1[i];
		for (uint8_t i = 0; i != mparam; i++)
			param[i] = &(paramx[i]);
	}
	_MC_param_text(paramx);
	do {
		printf("Which ");
		color(GELBh);
		printf("parameter");
		rcolor;
		printf(" do you want to ");
		color(GELBh);
		printf("change");
		rcolor;
		printf("? (1 - %i): ", mparam);
		int x = int_inp(1, mparam) - 1;
		printf("New value: ");
		*param[x] = int_inp(0, 9999);
		_MC_param_text(paramx);
		rcolor;
		printf("Are you finish? (y/n)\n");
	} while (ynrespond() == 'n');
	if (param1 != NULL) {
		for (uint8_t i = 0; i != mparam; i++)
			param1[i] = *param[i];
	}


}

bool _MC_convert_Nr(char* kett, int nr, bool show, char* out) {
	if (nr < 1)
		return 0;
	nr--;
	if (kett == NULL) {
		printf("\n_MC_convert_Nr Kett:\"%s\"\n", kett);
		return 0;
	}
	int i = 0;
	for (i = 0; i != nr; i) {
		if (*kett == '\0')
		{
			if (show)
			{
				color(ROTh);
				printf("Nr %i was not found!!!\n", nr++);
				rcolor;
				return 0;
			}
		}
		if (*(++kett) == ':')
			i++;
	}
	memcpy(out, kett + ((kett[0] == ':') ? 1 : 0), strlen(kett));
	i = 0;
	while (out[i] != '\0') {
		if (out[i] == ':') {
			out[i] = '\0';
			break;
		}
		i++;
	}
	//printf("out %s\n", out);
	return 1;
}
void _MC_presetcreate(int nr) {

	char namesx[MC_NAMES_BF_SIZE] = "";
	int max = _MC_name_preset(namesx);

	printf("|");
	printx('_', strlen("Create Preset"));
	printf("|\n|Create preset|\n|");
	printx('_', strlen("Create Preset"));
	printf("|\n");
	char x[101] = "Untitled";
	char x1[101] = "";
	uint16_t param[6] = { 0 };
	int xnum = 0;
	bool b = 0;
	const char stdLen = strlen("Untitled");
	if (mcShowprest(x, 0, NULL, GELBh)) {
		strcat(x, "0");
		while (mcShowprest(x, 0, NULL, GELBh)) {
			sscanf(&x[stdLen], "%i", &xnum);
			sprintf(x, "Untitled%i", ++xnum);
		}

	}
	color(GELBh);
	printf("Name preset as \"%s\"? (y/n)\n\trecommend n\n", x);
	rcolor;
	if (ynrespond() == 'n') {
		do {
			printf("\n\nName: ");
			str_inp(x, sizeof(x));
			if ((b = mcShowprest(x, 0, NULL, GELBh))) {
				color(ROTh);
				printf("Name \"%s\" is already taken!!!\n\tTry another name again...\n");
				rcolor;
			}
		} while (b);
	}
	if (nr > 0) {
		if (_MC_convert_Nr(namesx, nr, 1, x1)) {
			mcShowprest(x1, 0, param, GELBh);
			mcAddPreset(param, x);
			_MC_name_preset(namesx);
			_MC_param_text(param);
		}
		else {
			color(ROTh);
			printf("ERROR: Don't mess around with presets directory!!!\n");
			rcolor;
		}
	}
	else {
		printf("Do you want to ");
		color(GELBh);
		printf("copy data from another preset");
		rcolor;
		printf("? (y/n)\n");
		if (ynrespond() == 'y') {
			printf("Select (1-%i): ", max);
			if (_MC_convert_Nr(namesx, int_inp(1, max), 1, x1)) {
				printf("Name: %s\n", x1);
				mcShowprest(x1, 0, param, GELBh);
				mcAddPreset(param, x);
			}
			else {
				color(ROTh);
				printf("ERROR: Don't mess around with presets directory!!!\n");
				rcolor;
			}
		}
	}
	printf("Do you want to ");
	color(GELBh);
	printf("change");
	rcolor;
	printf(" a paramenter? (y/n)\n");
	if (ynrespond() == 'y')
		_MC_param_change(param);
	if (!mcAddPreset(param, x)) {
		color(ROTh);
		printf("ERROR: Don't mess around with presets directory!!!\n");
		rcolor;
	}
	else {
		color(GRUENh);
		printf("Saved as \"%s\\%s.pre\"\n", MC_PRESET, x);
		rcolor;
	}
	color(GRUENh);
	printf("Push any key to continue!");
	_getch();
	rcolor;
	printf("\r                              \r");

}
void _MC_presetsedit(int nr) {
	char namesx[MC_NAMES_BF_SIZE] = "";
	char x1[MC_NAMES_BF_SIZE] = "";
	uint16_t param[6] = { 0 };
	int max = _MC_name_preset(namesx);
	if (nr == 0) {
		printf("Select (1-%i): ", max);
		nr = int_inp(1, max);
	}
	if (_MC_convert_Nr(namesx, nr, 1, x1)) {
		mcShowprest(x1, 0, param, GELBh);
		_MC_param_change(param);
		mcAddPreset(param, x1);
	}
	else {
		color(ROTh);
		printf("ERROR: Don't mess around with presets directory!!!\n");
		rcolor;
	}
}
bool _MC_load_preset(bool change, bool ifa);
void _MC_presetshow_all() {
	uint16_t* paramxx[6] = { &mcMaxTicks ,&mcMinTicks ,&mcTolerantBouncing ,&mcTolerantReal ,&mcTolerantLeft2Right,&mcAccuracy };
	char names[MC_NAMES_BF_SIZE] = "";
	char name[MC_NAMES_BF_SIZE] = "";
	char text[MC_NAMES_BF_SIZE] = "";
	uint16_t param[16] = { 0 };
	do {
		printf("Currnet settings:\n");
		uint16_t paramg[6] =
		{
	mcMaxTicks ,mcMinTicks ,mcTolerantBouncing ,
	mcTolerantReal ,mcTolerantLeft2Right,mcAccuracy
		};
		_MC_param_text(paramg);
		unsigned int num = _MC_name_preset(names);
		if (0 == num)
		{
			color(GELBh);
			printf("There is no preset saved now!\n");
			rcolor;
			printf("\tDo you want to ");
			color(GELBh);
			printf("create");
			rcolor;
			printf(" one now? (y/n)\n");
			if (ynrespond() == 'y')
				_MC_presetcreate(0);
		}
		printf("Do you want to ");
		color(GELBh);
		printf("see all settings (y)");
		rcolor;
		printf(" or ");
		color(GELBh);
		printf("names only (n)");
		rcolor;
		printf("?\n");
		bool b = (ynrespond() != 'y');
		bool end = 1;
		do {
			end = 1;
			printf("This %i preset%s available\n", num, (num == 1) ? " is" : "s are");
			for (int i = 0; i != num; i++) {
				_MC_convert_Nr(names, i + 1, 1, name);
				if (b) {
					if ((i % 2) == 1)
						color(GELBh);
					printf("(%i)\t%s\n", i + 1, name);
					rcolor;
				}
				else {
					color(GELBh);
					printf("(%i) ", i + 1);
					rcolor;
					mcShowprest(name, 1, NULL, GELBh);
				}
			}
			if (b) {
				printf("\nDo you want to ");
				color(GELBh);
				printf("see all settings");
				rcolor;
				printf("? (y/n)\n");
				b = (ynrespond() != 'y');
				end = b;
			}
		} while (!b && !end);
		printf("Do you want to ");
		color(GELBh);
		printf("edit (e)");
		rcolor;
		printf(" or ");
		color(GELBh);
		printf("select (advanced settings)(s)");
		rcolor;
		printf(" or ");
		color(GELBh);
		printf("add (a)");
		rcolor;
		printf(" or ");
		color(GELBh);
		printf("cancel (c)");
		rcolor;
		printf(" or ");
		color(GELBh);
		printf("load preference to setting (p)");
		rcolor;
		printf(" a preset? (e/s/a/c/p)\n");
		char c = 0;
		int sel = 0;
		do {

			b = 0;
			switch ((c = _getch())) {
			case 'e':printf("\r                                                                 \r");
				_MC_presetsedit(0);
				break;
			case 's':printf("\r                                                                 \r");
				printf("Select (1-%i): ", num);
				sel = int_inp(1, num);
				if (_MC_convert_Nr(names, sel, 1, name)) {
					printf("Do you want to ");
					color(GELBh);
					printf("edit (e)");
					rcolor;
					printf(" or ");
					color(GELBh);
					printf("show (s)");
					rcolor;
					printf(" or ");
					color(GELBh);
					printf("add (copy and edit) (a)");
					rcolor;
					printf(" or ");
					color(GELBh);
					printf("delete (d)");
					rcolor;
					printf(" or ");
					color(GELBh);
					printf("move preset to current settings (m)");
					rcolor;
					printf(" or ");
					color(GELBh);
					printf("cancel (c)");
					rcolor;
					printf(" a preset? (e/s/a/d/m/c)\n");
					do {
						b = 0;
						switch ((c = _getch())) {
						case 'a':printf("\r                                                                 \r");
							_MC_presetcreate(sel); break;
						case 'e':printf("\r                                                                 \r");
							_MC_presetsedit(sel);
							break;
						case 'd':printf("\r                                                                 \r");
							strcpy(text, "del ");
							strcat(text, MC_PRESET);
							strcat(text, "\\");
							strcat(text, name);
							strcat(text, ".pre");
							//printf("%s\n", text);
							system(text);
							break;
						case 's':printf("\r                                                                 \r");
							mcShowprest(name, 1, param, GELBh);
							_MC_param_text(param);
							break;
						case 'm':printf("\r                                                                 \r");
							mcShowprest(name, 1, param, GELBh);
							for (uint8_t i = 0; i != 6; i++)
								*paramxx[i] = param[i];
							break;
						default:
							color(ROTh); printf("\r'%c' is not defined, use 'e', 's', 'a' or 'c' instead!!!", c); b = 1;
							rcolor;
							break;
						}
					} while (b);
					b = 0;
				}
				else {
					color(ROTh);
					printf("ERROR: Don't mess around with presets directory!!!\n");
					rcolor;
				}
				break;
			case 'a':printf("                                                                 \r");
				_MC_presetcreate(0);
				break;
			case 'c':printf("                                                                 \r");
				break;
			case 'p':printf("                                                                 \r");
				_MC_load_preset(0, 0); break;

			default:color(ROTh); printf("\r'%c' is not defined, use 'e', 's', 'a', 'c' or 'p' instead!!!", c); b = 1;
				rcolor;
				break;
			}
		} while (b);
		printf("See all settings again? (y/n)\n");
	} while (ynrespond() == 'y');
}
void _MC_param_show() {
	color(GELBh);
	printf("\t\t++++Motor test parameters++++\n");
	rcolor;
	uint16_t param[6] = {
		mcMaxTicks ,mcMinTicks ,mcTolerantBouncing ,
		mcTolerantReal ,mcTolerantLeft2Right,mcAccuracy
	};
	_MC_param_text(param);
	printf("\n");
	printf("Do you want to ");
	color(GELBh);
	printf("change analystic parameters");
	rcolor;
	printf("? (y/n)\n");

	if (ynrespond() == 'y')
	{
		printf("Do you want to ");
		color(GELBh);
		printf("use, make or edit a preset");
		rcolor;
		printf("? (y/n)\n");
		if (ynrespond() == 'n')
			_MC_param_change(NULL);
		else {
			_MC_presetshow_all();
		}
	}

}
bool _MC_save_data(uint16_t param[6], uint16_t summary[2][5][2], char* name) {
	DIR* dir;
	struct dirent* ent;
	char text[LEN_TEST] = "Test";
	char text1[LEN_TEST] = "";
	int cnt = 0;

	uint8_t i = 0;
	uint8_t ii = 0;
	uint8_t iii = 0;

	strcpy(name, "");
	if (!generateDir(MC_SIMULATION, &dir)) {
		color(ROTh);
		printf("Dir error!!!\n");
		rcolor;
		return 0;
	}
	i = 0;
	while ((ent = readdir(dir)) != NULL) {
		i++;
	}
	if (i > 1000) {
		color(ROTh);
		printf("Dir error!!!\n");
		rcolor;
		return 0;
	}
	rewinddir(dir);
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] != '.') {
			if (!strcmp((char*)".tst", &(ent->d_name[ent->d_namlen - 4]))) {

				//	printf("name \"%s\"end \"%s\" %i \n", ent->d_name, &(ent->d_name[ent->d_namlen - 4]), strcmp((char*)".pre", &(ent->d_name[ent->d_namlen - 4])));

				strcpy(text, ent->d_name);
				text[ent->d_namlen - 4] = '\0';
				++cnt;
				strcat(text1, text);
				strcat(text1, ":");
				//	printf("\"%s\"\n", ent->d_name);

			}
		}
	}
	if (strlen(text1) > LEN_TEST - 101) {
		color(ROTh);
		printf("Please delete some files (\"%s\") in the future!!!\n", MC_SIMULATION);
		rcolor;
	}
	closedir(dir);
	FILE* f;
	cnt--;
	do {
		sprintf(text, "%s\\Test%9i.tst", MC_SIMULATION, ++cnt);
	} while (fileExist(text));
	f = fopen(text, "w+");
	if (f == NULL)
	{
		color(ROTh);
		printf("File error!!!\n");
		rcolor;
		return 0;
	}
	fprintf(f, "%s\n", cSnameOfTest);
	fprintf(f, "%s\n", mc_notes);
	for (i = 0; i != 6; i++)
		fprintf(f, "%i\n", param[i]);

	for (i = 0; i != 2; i++)
		for (ii = 0; ii != 5; ii++)
			for (iii = 0; iii != 2; iii++)
				fprintf(f, "%i\n", summary[i][ii][iii]);
	fclose(f);
	if (name) {
		strcpy(name, text + strlen(MC_SIMULATION) + 1);
		name[strlen(name) - 4] = '\0';
	}

	return 1;
}
bool _MC_load_preset(bool change, bool ifa) {
	char doc[51] = "";
	strcpy(doc, MC_PRESET);
	strcat(doc, "\\use.txt");
	FILE* f;
	if (NULL != (f = fopen(doc, "r"))) {
		fclose(f);
	}
	else {
		if (!ifa) {
			change = 1;
			if (NULL == (f = fopen(doc, "a"))) {
				color(ROTh);
				printf("Preset file create Error!\n");
				rcolor;
				return 0;
			}
		}
		else {
			return 0;
		}
	}
	char names[LEN_TEST] = "";
	char name[LEN_TEST] = "";
	uint16_t* paramxx[6] = { &mcMaxTicks ,&mcMinTicks ,&mcTolerantBouncing ,&mcTolerantReal ,&mcTolerantLeft2Right,&mcAccuracy };
	int num = 0;
	uint16_t param[6] = { 0 };
	do {
		if (change) {
			if (f != NULL)
				fclose(f);
			if ((f = fopen(doc, "w")) == NULL) {
				color(ROTh);
				printf("File can't rewritten!!!\n");
				rcolor;
				return 0;
			}
			num = _MC_name_preset(names);
			for (int i = 1; i != num + 1; i++) {
				_MC_convert_Nr(names, i, 1, name);
				//mcShowprest(name,0,NULL);
				if (i % 2)
					color(GELBh);
				printf("(%i)\n", i);
				rcolor;
				mcShowprest(name, 1, param, GELBh);
				rcolor;
			}
			color(GELBh);
			printf("Choose your preset as preference: ");
			num = int_inp(1, num);
			rcolor;
			fprintf(f, "%s", name);
			fclose(f);
			_MC_convert_Nr(names, num, 0, name);

		}
		else {
			if (f != NULL)
				fclose(f);
			if ((f = fopen(doc, "r")) == NULL) {
				color(ROTh);
				if (!ifa)
					printf("File can't open!!!\n");
				rcolor;
				return 0;
			}
			for (int i = 0; ((name[i] = fgetc(f)) != EOF); i++) {
				name[i + 1] = '\0';
				//printf("%c", name[i]);
			}
			name[strlen(name) - 1] = '\0';

		}
		if (!mcShowprest(name, change, param, GELBh)) {
			change = 1;
		}
		else
			change = 0;
	} while (change && !ifa);
	if (!change) {
		for (uint8_t i = 0; i != 6; i++)
			*paramxx[i] = param[i];
		if (!ifa) {
			color(GRUENh);
			printf("All data (from \"%s\") are set to actual preset.\n", doc);
			rcolor;
		}
	}
	if (f != NULL)
		fclose(f);
	return 1;
}
bool _readOnly_MCtest_Data(FILE* f, uint16_t param[6], uint16_t summary[2][5][2], char c[2][201], bool printD) {
	if (f == NULL && printD) {
		color(ROTh);
		printf("FILE EMTY\n");
	}
	int i = 0;
	uint8_t ii = 0;
	uint8_t iii = 0;
	int k = 0, j = 0;
	rewind(f);
	if (c != NULL) {
		if (str_inp(f, c[0], 200) == 0) {
			strcpy(c[0], "No imput");
			if (printD) {
				color(ROTh); printf("Emty or not enought data[name]\n"); rcolor;
			}
		}

		if (str_inp(f, c[1], 200) == 0) {
			strcpy(c[1], "No imput");
			if (printD) {
				color(ROTh);
				printf("Emty or not enought data [name]\n");
				rcolor;
			}
		}

	}
	else {
		char buff[201];
		uint8_t tb = 1;
		tb &= str_inp(f, buff, 200);
		tb &= str_inp(f, buff, 200);
		if (printD && !tb) {
			color(ROTh);
			printf("Emty or not enought data [name]\n");
			rcolor;
		}
	}
	for (i = 0; i != 6; i++) {

		if (fscanf(f, "%05i\n", &k) == -1) {
			if (printD) {
				color(ROTh);
				printf("Emty or not enought data [param]\n");
				rcolor;
			}
			return 0;
		}
		if (param != NULL)
			param[i] = (uint16_t)k;
	}
	for (i = 0; i != 2; i++)
		for (ii = 0; ii != 5; ii++)
			for (iii = 0; iii != 2; iii++) {
				//printf("%i", fscanf(f, "%i", text))

				if (fscanf(f, "%05i\n", &j) == -1) {
					if (printD) {
						color(ROTh);
						printf("Emty or not enought data [Summary]\n");
						rcolor;
					}
					return 0;
				}
				if (summary != NULL)
					summary[i][ii][iii] = (uint16_t)j;
				//printf("%i\n", j);
			}
	return 1;
}
bool _MC_get_offline_data(uint16_t param[6], uint16_t summary[2][5][2], char c[2][201]) {
	char text[LEN_TEST] = "";
	char text1[LEN_TEST] = "";
	static FILE* fp[1000];
	int ch = 0;
	bool b[1000];
	char ctemp[2][201];
	char temp[210];
	char searchItemName[201];
	char searchItemNote[201];
	char searchItemFile[201];
	bool searchActive = 0;

	int i = 0;
	for (i = 0; i != 1000; i++)
		fp[i] = NULL;

	printf("Select the your analysis which you want to look at.\n");
	printf("(  0) EXIT\n");
	/*printf("Type ");
	color(GELBh);
	printf("'y'");
	rcolor;
	printf(" to ");
	color(GELBh);
	printf("show all names and notes from analyses");
	rcolor;
	printf("\n\tType ");
	color(GELBh);
	printf("'n'");
	rcolor;
	printf(" to ");
	color(GELBh);
	printf("exit");
	rcolor;
	printf("!\n");
	if (ynrespond() == 'n')
		return 0;*/
	strcpy(text1, MC_SIMULATION);

	DIR* dir;
	struct dirent* ent;
	int cnt = 0;
	int unt = 0;
	char* cy;
	do {
		do {
			ch = -1;
			if ((dir = opendir(MC_SIMULATION)) != NULL) {
				/* print all the files and directories within directory */
				while ((ent = readdir(dir)) != NULL && ((!searchActive) || (searchActive && unt < cnt))) {
					cy = &(ent->d_name[strlen(ent->d_name) - 4]);
					if (strcmp(cy, ".tst") == 0) {

						strcpy(text1, MC_SIMULATION);
						strcat(text1, "\\");
						strcat(text1, ent->d_name);
						fp[cnt] = fopen(text1, "r");


						if (fp[cnt] != NULL) {
							b[cnt] = _readOnly_MCtest_Data(fp[cnt], NULL, NULL, ctemp, 0);
							if (!searchActive) {
								if ((cnt % 2) == 0)
									color(GELBh);
								for (uint8_t ui = 0; ui != 2; ui++) {
									strcpy(temp, "\"");
									strcat(temp, ctemp[ui]);
									strcat(temp, "\"");
									strcpy(ctemp[ui], temp);
								}
								if ((cnt % 5) == 0 && cnt != 0)
									printf("\n");
								printf("(%3i) Name: %-20s", cnt + 1, ctemp[0]);
								printf("\t");
								printf("Notes: %-20s", ctemp[1]);
								printf("\t");


								printf("File: \"%s\" (File ", ent->d_name);

								if (!b[cnt]) {
									printf("not ");
								}
								if (!b[cnt])
									color(ROTh);
								else
									color(GRUENh);
								printf("redy!!!)\n");
								rcolor;
							}
							else {//SEARCH

								uint8_t p = 0;
								/*printf("%i von %i\n", unt, cnt);
								printf("%s == %s\n", searchItemFile, ent->d_name);
								printf("%s == %s\n", searchItemName, ctemp[0]);
								printf("%s == %s\n", searchItemNote, ctemp[1]);
								*/
								if (!strcmp(searchItemFile, ent->d_name) || !strlen(searchItemFile))
									p++;

								if (!strcmp(searchItemName, ctemp[0]) || !strlen(searchItemName))
									p++;

								if (!strcmp(searchItemNote, ctemp[1]) || !strlen(searchItemNote))
									p++;


								if (p == 3) {
									ch = unt;
									color(GRUENh);
									printf("Nr. %i valid\n", unt + 1);
									rcolor;
								}
								unt++;
							}
						}
						else {
							color(ROTh);
							b[cnt] = 0;
							printf("Can't open \"%s\"\n", text1);
							rcolor;
						}
						if (!searchActive)
							cnt++;
					}
				}
			}
			if (!searchActive && (ch == -1 || !b[ch])) {
				printf("END dir\n");
				closedir(dir);
				printf("Numeric Search 'n' or Character Search 'y'\n");
				if (ynrespond() == 'y') {
					searchActive = 1;
					strcpy(searchItemName, "");
					strcpy(searchItemNote, "");
					strcpy(searchItemFile, "");

					printf("Name? (y/n)\n");
					if (ynrespond() == 'y') {
						printf(":");
						str_inp(searchItemName, 200);
					}
					printf("Note? (y/n)\n");
					if (ynrespond() == 'y') {
						printf(":");
						str_inp(searchItemNote, 200);
					}
					printf("File? (y/n)\n");
					if (ynrespond() == 'y') {
						printf(":");
						str_inp(searchItemFile, 200);
					}
				}
				else {
					printf("\nYour choice : ");
					do {
						if ((ch = int_inp(0, cnt)) == 0)
							return 0;
						ch--;
						if (!b[ch]) {
							color(ROTh);
							printf("Choose a not damaged file.\n");
							rcolor;
							printf("Your choice:");
						}
					} while (!b[ch]);
				}
			}

		} while (searchActive && unt != cnt && (ch == -1 || b[ch]));

		if (b[ch]) {
			printf("Selected: %i\n", ch + 1);
			_readOnly_MCtest_Data(fp[ch], param, summary, c, 1);
		}
		else {
			color(ROTh);
			printf("Select a valid file!!!\n");
			rcolor;
		}
		for (i = 0; i != cnt; i++) {
			if (fp[i] != NULL)
				fclose(fp[i]);
		}
		if (ch == -1) {
			color(ROTh);
			printf("No search found!!\n");
			rcolor;
		}
		searchActive = 0;
		cnt = 0;
		unt = 0;
	} while (ch == -1 || !b[ch]);
	return 1;
}
bool mc_Test_only() {
	uint8_t i = 0;
	char text[101] = "";
	char err[101] = "";
	uint16_t* paramxx[6] = { &mcMaxTicks ,&mcMinTicks ,&mcTolerantBouncing ,&mcTolerantReal ,&mcTolerantLeft2Right,&mcAccuracy };
	_MC_load_preset(0, 0);
	//_MC_name_preset(NULL);

	//inti variables
	char outDecider = 0;
	rcolor;

	uint16_t summary[2][5][2] = { 0 };
	/*for (char i = 0; i != 5; i++) {
		summary[0][i][0] = 0;
		summary[0][i][1] = 0;
		summary[1][i][0] = 0;
		summary[1][i][1] = 0;
	}*/
	//print_MC_cable(NULL);
	char farb[2][6] = {
		{1,1,1,1,1,1},
		{1,1,1,1,1,1}
	};
	//Summerise LR FB
	uint16_t paramOffline[7];
	if (!mcOperateOffline) {
		if (!mcDebug) {
			if (!mc_Data_Converter(summary)) {
				color(ROTh);
				printf("Connection ERROR\n");
				rcolor;
				return 0;
			}
		}
		else {
			uint16_t mcNorm = (mcMaxTicks + mcMinTicks) / 2;
			for (char i = 0; i != 5; i++) {
				summary[0][i][0] = mcNorm;
				summary[0][i][1] = mcNorm;
				summary[1][i][0] = mcNorm;
				summary[1][i][1] = mcNorm;
			}
			mc_defineSummFor(summary, 'b', 0, 0, 7);
		}
	}
	else {
		if (!_MC_get_offline_data(paramOffline, summary, NULL)) {
			//color(ROTh);
			//printf("invalid Data!!!");
			//rcolor;
			return 0;
		}
		for (i = 0; i != 6; i++)
			*(paramxx[i]) = paramOffline[i];
	}
	color(GELBh);
	_MC_param_show();

	mc_text_output(summary);
	uint64_t lrcnt[2] = { 0,0 };
	for (i = 0; i != 4; i++) {
		lrcnt[0] += (summary[0][i][0] + summary[1][i][0]);
		lrcnt[1] += (summary[0][i][1] + summary[1][i][1]);
	}
	/*lrcnt[0] = 1;
	lrcnt[1] = 1;*/
	char dir[2][10] = { "left","right" };//2,6
	bool b = 0;
	do {
		strcpy(text, "");
		strcpy(err, "");
		b = 0;
		printf("Do you want to see the motor analystic seperat from each wheel? (y/n)\n\t(not nessersary if you want a nice overview)\n");
		bool show = ('y' == ynrespond());
		for (i = 0; i != 2; i++) {
			if (lrcnt[i] != 0) {
				color(GRUENh);
				printf("*** Start motor advanced analyser \"%s\"\n", dir[i]);
				rcolor;
				switch ((outDecider = mcFBanalyser(summary, i, show)))
				{
				case MC_CORRECT_DRIVE:
					break;
				case MC_WRONG_DRIVE_POLARITY:
					farb[i][0] = MC_WARN;
					farb[i][5] = MC_WARN;

					farb[i][2] = MC_NOK;
					farb[i][3] = MC_NOK;
					farb[i][4] = MC_NOT_TESTED;
					farb[i][1] = MC_NOT_TESTED;
					break;
				case MC_WARNING:
					farb[i][0] = MC_OK;
					farb[i][5] = MC_OK;

					farb[i][2] = MC_WARN;
					farb[i][3] = MC_WARN;
					farb[i][4] = MC_NOT_TESTED;
					farb[i][1] = MC_NOT_TESTED;

					break;
				case MC_WRONG_DRIVE:
					farb[i][0] = MC_NOT_TESTED;
					farb[i][5] = MC_NOT_TESTED;

					farb[i][2] = MC_WARN;
					farb[i][3] = MC_WARN;
					farb[i][4] = MC_OK;
					farb[i][1] = MC_OK;

					break;
				case MC_E_CONNECT:
					farb[i][0] = MC_NOT_TESTED;
					farb[i][5] = MC_NOT_TESTED;

					farb[i][2] = MC_NOT_TESTED;
					farb[i][3] = MC_NOT_TESTED;
					farb[i][4] = MC_NOT_TESTED;
					farb[i][1] = MC_NOT_TESTED;
					break;
				default:
					farb[i][0] = MC_NOK;
					farb[i][5] = MC_NOK;

					farb[i][2] = MC_NOK;
					farb[i][3] = MC_NOK;
					farb[i][4] = MC_NOK;
					farb[i][1] = MC_NOK;
					break;
				}
				printf("\n");
			}
			else {
				printf("Does the %s wheel turn?(y/n)\n", dir[i]);
				if (ynrespond() == 'n') {
					farb[i][0] = MC_NOK;
					farb[i][5] = MC_NOK;
					farb[i][2] = MC_NOT_TESTED;
					farb[i][3] = MC_NOT_TESTED;
					outDecider = MC_WRONG_DRIVE_POLARITY;
				}
				else {
					farb[i][2] = MC_NOK;
					farb[i][3] = MC_NOK;
					outDecider = MC_WRONG_DRIVE_POLARITY;
				}
				printf("Does the %s motor LED lightning?(y/n)\n", dir[i]);

				if (ynrespond() == 'n') {
					farb[i][1] = MC_NOK;
					farb[i][4] = MC_NOK;
					outDecider = MC_WRONG_DRIVE_POLARITY;
				}
			}
		}
		if (outDecider != MC_CORRECT_DRIVE) {
			switch (outDecider) {
			case MC_WRONG_DRIVE:strcpy(err, "driven in false direction"); break;
			case MC_E_CONNECT:strcpy(err, "connection errors (USB)"); break;
			case MC_WRONG_DRIVE_POLARITY:strcpy(err, "connection errors by DiscBot's wireing"); break;
			case MC_WARNING:strcpy(err, "ballance or other warnings"); break;
			default:strcpy(err, "error not committed"); break;
			}
			//sprintf(text, "The motor drive has some errors \"%s\"! Use \"%s\" to generate analystics with preset \"%s\"", err, NULL, NULL);
			sprintf(text, "The motor drive has some errors \"%s\"! Open the last file in the offline menue.\n", err);
			cScError(text);
		}

		printf("\n\n");
		print_MC_cable(farb);
		rcolor;
		printf("Do you want to see the advanced results? (y/n)\n");
		if (ynrespond() == 'y') {
			//char results[mcDecider_steps][200];

			mcDecider(summary, 1, 0, 1);
			//for (uint8_t i = 0; i != mcDecider_steps; i++)
				//free(results[i]);
		}
		uint16_t paramxx[6] = { mcMaxTicks ,mcMinTicks ,mcTolerantBouncing ,mcTolerantReal ,mcTolerantLeft2Right,mcAccuracy };
		char nam[21] = "";
		//printf("Hello World!\n");
		if (!mcOperateOffline) {
			printf("Do you want to ");
			color(GELBh);
			printf("set notes");
			rcolor;
			printf("? (y/n)\n");
			if (ynrespond() == 'y') {
				str_inp(mc_notes, 200);
			}
			else {
				strcpy(mc_notes, "No notes");
			}
			if (_MC_save_data(paramxx, summary, nam)) {
				color(GRUENh);
				printf("Saved as \"%s\"!\n", nam);
				char xtx[101] = "";
				sprintf(xtx, "For more information open file \"%s\" with the offline menue!!!\n", nam);
				cSaddMsg(xtx);
			}
			else {
				color(ROTh);
				printf("Can't save file!!!\n");
			}
		}

		rcolor;
		b = 0;

		if (mcOperateOffline) {
			printf("Please push 'r'!!!\n");
			while ('r' != _getch());
			//	exit(0);
		}
		printf("Run again with other parameters? (y/n)\n");
		if (ynrespond() == 'y') {
			color(GELBh);
			_MC_param_show();
			b = 1;
		}
	} while (b);
	rcolor;
	return 1;
}
bool db_mc_new() {
	if (!db_circumference()) {
		color(ROTh);
		printf("Circumference connection error\n");
		cScError("Circumference connection error\n");
		rcolor;
		return 0;
	}
	printf("In this sketch we're testing the functionallity of the motors and it's sensors!\n");
	color(ROTd);
	printf("\tWarning!! %s will start moveing!!!\n", PER_DEVICE);
	rcolor;
	do {
		color(GELBh);
		printf("Press 'x' to start expanded motor analyser!\nIf you want to cancle press 'c'!\n");
		rcolor;
		char c = 0;
		while ((c = _getch()) != 'x' && c != 'c');
		if (c != 'c') {
			color(GRUENh);
			printf("Wheel spining...\n");
			rcolor;
			if (db_cmd(DB_MC, 0, NULL, NULL) == 0) {
				return 0;
			}
			color(GELBh);
			for (int i = 0; i != 1750; i++) {
				printf("\rWork in process (%2.2lf%%)", percent(1750, i));
				wait(1);
			}
			color(GRUENh);
			printf("\rWork in process (100%%)");
			rcolor;
			printf(" - Finish\n");
			mc_Test_only();
		}
		color(GELBh);
		printf("Again? (y/n)\n");
		rcolor;
	} while (ynrespond() == 'y');
	return 1;
}
bool db_ircClearB() {
	uint8_t in[2] = { 0 ,0 };
	return db_cmd(DB_IRC_BUFFER, 1, in, NULL);
}
bool db_irc_send(uint8_t code[4]) {
	return db_cmd(DB_IRC_SEND, 4, code, NULL);
}
bool db_irc_recive(uint8_t* len, uint8_t* code) {
	uint8_t out[255];
	*len = 0;
	if (!db_cmd(DB_IRC_BUFFER, 0, NULL, out))
		return 0;
	*len = (out[1] / 4);
	for (uint8_t i = 0; i != out[1]; i++) {
		code[i] = out[i + 2];
	}
	return 1;
}
void db_irc_analystic(uint8_t len, uint8_t* code, uint8_t out[4]) {
	uint8_t lenx = 0;
	lenx = (len) * 4;
	for (uint8_t i = 0; i != 4; i++) {
		out[i] = code[len + i];
	}
}
bool db_ircT() {
	printf("In this scketch we're testing the InfraRed Communication!\n");
	printf("Make the IR module ready, then");
	color(GELBh);
	printf(" push any key!\n");
	rcolor;
	_getch();
	bool rec = 0;

	if (!db_ircClearB())
		return 0;
	color(GRUENh);
	printf("IRC buffer cleared!\n");
	rcolor;
	uint8_t len = 0;
	uint8_t cod[255];
	uint8_t codout[4];
	printf("Now you can test your ");
	color(GELBh);
	printf("IR remote");
	printf("!\n");
	color(GELBh);
	printf("Push enter to stop!\n");
	rcolor;
	do {
		if (!db_irc_recive(&len, cod))
			return 0;
		if (!db_ircClearB())
			return 0;
		for (uint8_t i = 0; i != len; i++) {
			db_irc_analystic(i, cod, codout);
			printf("The module recieves:");
			for (uint8_t u = 0; u != 4; u++)
				printf(" %02X", codout[u]);
			printf("\n");
			if (!rec)
				if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 255, 0))
					return 0;
			rec = 1;
			waitS(500, 1);
			if (!db_Beep())
				return 0;
		}
		if (rec)
			if (waitS(1500, 0)) {
				rec = 0;
				if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0))
					return 0;
			}

	} while (!anyKey());
	//wait(500);
	flush();
	if (!dbLed_setColor(DB_LED_BUTTON_GREEN, 0, 0, 0))
		return 0;
	printf("Now you can test your ");
	color(GELBh);
	printf("IR transmitter LED");
	rcolor;
	printf("!\n");
	color(ROTd);
	printf("\tYou might need a other %s (with IR reciver module) in order to test this\n\tDBOS / dbosX command 0x57 0x00\n", PER_DEVICE);
	rcolor;
	do {
		printf("Type in your Data to send via IR!\n");
		int inp = 0;
		for (uint8_t i = 0; i != 4; i++) {
			do {
				if (inp == 1000)
				{
					color(ROTh);
					printf("Wrong value try again!\n\tTry from \"00\" up to \"FF\"\n");
					rcolor;
				}
				inp = 1000;
				printf("Type in data byte %i (in HEX): ", i + 1);
				scanf("%X", &inp);
				flush();
			} while (inp == 1000);
			cod[i] = inp;
		}
		do {
			if (!db_irc_send(cod))
				return 0;
			color(GRUENh);
			printf("Succsessfully transmitted\n");
			color(ROTd);
			printf("\tCheck if this command would be sent!!!\n");
			rcolor;
			printf("Send same again? (y/n)\n");
		} while (ynrespond() == 'y');
		printf("Again? (y/n)\n");
	} while (ynrespond() == 'y');
	cSaskIfFine();
	return 1;
}

int wtn(char* c) {
	if (!strcmp(c, "btn"))
		return 0;
	if (!strcmp(c, "rfid"))
		return 1;
	if (!strcmp(c, "irc"))
		return 2;
	if (!strcmp(c, "mc"))
		return 3;
	if (!strcmp(c, "cs"))
		return 4;
	if (!strcmp(c, "gs"))
		return 5;
	if (!strcmp(c, "irs"))
		return 6;
	if (!strcmp(c, "led"))
		return 7;
	if (!strcmp(c, "ls"))
		return 8;
	if (!strcmp(c, "uss"))
		return 9;
	if (!strcmp(c, "bt"))
		return 10;
	if (!strcmp(c, "bm"))
		return 11;
	return -1;
}
bool manager(char* ex) {
	char* pex;
	char cx[9] = "";
	char c = '\0', cy = -1;
	uint8_t i = 0;
	pex = ex;
	bool res = 1;
	bool allok = 1;
	cSenable();
	//printf("EXE: \"%s\"\n", ex);
	//c = ex[0];
	do {
		allok = 1;
		c = *ex;
		if (c != ' ') {
			cx[i] = c;
			i++;
		}
		if (c == ' ' || c == '\0') {
			//printf("command : \"%s\" number: %i\n", cx, wtn(cx));
			cx[i] = '\0';
			if (cx[0] != '\0') {
				//printf("command : \"%s\" number: %i\n", cx, wtn(cx));
				if ((cy = wtn(cx)) != -1) {
					color(GELBh);
					printf("\n\t****Start %s_Check****\n\n", cx);
					rcolor;
					cScategory(cx);
					char title[31] = "";
					strcpy(title, " - ");
					strcat(title, cx);
					title[strlen(title) - strlen(cx)] = title[strlen(title) - strlen(cx)] + ('A' - 'a');
					strcat(title, " check");
					printTitle(title);
				}
				switch (cy) {
				case 0:allok = db_Tbtn(); break;
				case 1:allok = db_Trfid(); break;
				case 2:allok = db_ircT(); break;
				case 3:allok = db_mc_new(); break;
				case 4:allok = db_Tcs(); break;
				case 5:allok = printf("Gyro sensor is not supported yet!\n"); break;
				case 6:allok = db_Tirs(); break;
				case 7:allok = db_Tled(); break;
				case 8:allok = db_Tls(); break;
				case 9:allok = db_Tuss(); break;
				case 10:
					bt_os(1);
					printf("BLE can be tested in the DiscBot mobile app!\nDo you need or want the link? (y/n)\n");
					if ('y' == ynrespond()) {
						system("start https://www.bikebird.at/StepByStep/");
						printf("Type in your email and password to access Bikebird StepByStep\n\tThen klick on \"20_MikroController\"-\"85_DiscBot\"-\"App Downloaden\"!\n\tWarning: it is an Android App only!!!\n");
					}
					printf("If you finished disconnect your module from your phone and press any key!\n");
					_getch();
					bt_os(0);

					bt_config();
					printf("Do you want to change the configuration? (y/n)\n");
					while (ynrespond() == 'y') {
						bt_configchng();
						printf("View configuration? (y/n)\n");
						if (ynrespond() == 'y')
							bt_config();
						printf("Change config again? (y/n)\n");
					}

					db_cBT();

					cSaskIfFine();
					bt_os(1);
					break;
				case 11:allok = db_Tbm(); break;
				default:color(ROTh); printf("Command \"%s\" is not defined!\nPlease delete it from the configuration File!\n", cx); system(PER_CONFIG); color(rcol); break;
				}
				if (!allok) {

					db_reset();
					color(ROTh);
					printf("%s connection invalid!!!\nTest your connection for issues!!!\n", PER_DEVICE);
					color(rcol);
					cScError("Connection Error from DiscBot!!\n");
				}
				if (cy != -1) {

					cScategoryEnd();
					color(GELBd);
					printf("\n\n\t****End %s_Check****\n\n", cx);
					rcolor;
					color(GRUENd);
					printf("Push enter to go on!!\n");
					rcolor;
					flushW();
				}
			}
			db_clearBuffer();

			i = 0;
			cx[i] = '\0';
		}
		ex++;
		//printf("Push enter if the program is waiting here!\n");
	} while (c != '\0');

	printf("\n");


	return 1;
}
bool manager_offline() {
	_MC_load_preset(0, 1);
	mcOperateOffline = 1;
	cSdisable();
	printTitle(" - Offline menue");
	const int max = 7;
	const char menue[] = "Offline menue (settings)";
	do {
		color(ROTh);
		printx('*', strlen(menue));
		rcolor;
		printf("\n%s\n", menue);
		color(ROTh);
		printx('*', strlen(menue));
		rcolor;
		printf("\n");
		for (uint8_t i = 0; i != max; i++) {
			if (i % 2)
				color(GELBh);
			else
				rcolor;
			printf("(%i)\t...\t", i + 1);
			switch (i) {
			case 0:printf("Show motor results"); break;
			case 1:printf("BT preference (Select your BT-Device)"); break;
			case 2:printf("MC presets (manage presets)"); break;
			case 3:printf("MC preference (set your preference to the global settings, until the program restarts)"); break;
			case 4:printf("MC Pinout (show you the pinout of the motorpins)"); break;
			case (max - 1):printf("Exit program"); break;
			case (max - 2):printf("Go online"); break;
			}
			printf("\n");
		}
		color(GELBh);
		printf("\nChoose your operation: ");
		rcolor;
		char name[101] = "";
		char filenam[101] = "";
		char name1[101] = "";
		bool b = 0;
		switch (int_inp(1, max) - 1) {
		case 0:mc_Test_only(); break;
		case 1:
			do {
				b = 0;
				show_bt_dir(name); printf("Selected file: \"%s\"\n", name);
				color(GELBh);
				printf("Do you want to change file? (y/n)");
				if (ynrespond() == 'y') {
					FILE* f;
					if ((f = fopen("HM\\use.TXT", "w+")) != NULL) {
						if (show_bt_dir(filenam)) {
							fprintf(f, "%s", filenam);
						}

						fclose(f);
					}
					else {
						color(ROTh);
						printf("File error!!!\n");
						rcolor;
					}

				}
			} while (b);
			break;
		case 2:_MC_presetshow_all(); break;
		case 3:_MC_load_preset(0, 0); break;
		case 4:print_MC_cable(NULL); break;
		case max - 1:endofP(); break;
		case max - 2:mcOperateOffline = 0; return 1; break;
		default:color(ROTh); printf("Ups! This function is not ready!\n"); rcolor; break;
		}
		rcolor;
		printf("Do you want to go online? (y/n)");
	} while (ynrespond() == 'n');
	mcOperateOffline = 0;
	return 1;
}
