#pragma once
#include "DB_serial.h"
bool db_monitor = 0;
bool mcDebug = 0;
HANDLE db_port;

static double dat[2] = { 0,0 };
void db_data_transmission_byte(double* x) {
	x[0] = dat[0];
	x[1] = dat[1];
}


bool db_cmd(uint8_t cmd, uint8_t lenIn, uint8_t* parameterIn, uint8_t* parameterOut) {
	dat[0] += (double)(lenIn + 2);
	static int cnt = 0;
	static uint8_t buffer[DB_MAXLENGTH];
	static uint8_t buffer2[DB_MAXLENGTH];
	static uint8_t i = 0;
	buffer[0] = cmd;
	buffer[1] = (uint8_t)lenIn;
	if (parameterIn != NULL) {

		for (i = 2; i != lenIn + 2; i++) {
			buffer[i] = parameterIn[i - 2];
		}
	}
	write_port(db_port, buffer, (lenIn + 2));
	if (db_monitor) {
		printf("\nSET %02X %02X :", buffer[0], buffer[1]);
		for (i = 0; i != buffer[1]; i++)
			printf(" %02X", buffer[2 + i]);
		printf("\n");
	}

	static int cnti;
	cnti = 0;
	if (cmd != DB_BUFFER_C) {

		do {
			cnti++;
			wait(25);
			if (cmd == DB_USS_G)
				wait(50);
			/*if (cmd == DB_BT)
				wait(50);*/
			if (cmd == DB_RESET) {
				wait(500);
				return 1;
			}
			if (cmd == DB_MC_D) {
				wait(500);
			}
			if (cmd == DB_RFID) {
				wait(500);
			}
			if (cmd == DB_IRS) {
				wait(50);
			}
			if (cmd == DB_IRC_SEND) {
				wait(250);
			}
			/*if (cmd == DB_COPY_SONG) {
				wait(500);
			}
			if (cmd == DB_CAT_SONG) {
				wait(500);
			}*/
			read_port(db_port, buffer, 2);
			if (db_monitor)
				printf("GET %02X %02X\n", buffer[0], buffer[1]);


#ifdef SHOW_WARNINGS
			if (DB_UNKNOWN != cmd)
				if (buffer[0] != cmd) {
					//printf("Command not aquired!!\nCallback code %02X\n", buffer[0]);
					switch (buffer[0]) {
					case 0:color(GELBh); printf("Unknown request!\n"); color(rcol); break;
					case 1:color(GELBh); printf("Invalid request!\n"); color(rcol); break;
					case 2:color(GELBh); printf("Blocked request!\n"); color(rcol); break;
					case 3:color(GELBh); printf("Invalid parameter!\n"); color(rcol); break;
					case 4:color(GELBh); printf("Timeout!\n"); color(rcol); break;
					case 5:color(GRUENh); printf("Start asyncron!\n"); color(rcol); break;
					case 6:color(GRUENh); printf("End asyncron!\n"); color(rcol); break;
					default:if (cmd != DB_RESET) { color(ROTh); printf("ERROR (%02X) not in list!\nThere might be a problem with the sensors!\nMake a hadware reset, when you try the next sketch!\n\tOtherwise the Errors could continues!\n", buffer[0]); color(rcol); return 0; } break;
					}
				}
#endif
		} while ((buffer[0] == 6 || buffer[0] == 5) && cnti <= 10 && cmd != DB_RESET);
		if (cnti > 10) {
#ifdef SHOW_WARNINGS
			color(ROTh);
			printf("\tThe request is invalid and overraned!\n");
			rcolor;
#endif
			return 0;
		}
		for (i = 0; i != 2; i++)
			buffer2[i] = buffer[i];

		if (parameterOut != NULL) {
			for (i = 0; i != 2; i++)
				parameterOut[i] = buffer[i];
		}
		if (buffer2[1] < DB_MAXLENGTH) {
			dat[1] += (double)(buffer2[1] + 2);
			read_port(db_port, buffer, buffer2[1]);

			if (parameterOut != NULL) {
				for (i = 0; i != buffer2[1]; i++)
					parameterOut[i + 2] = buffer[i];
			}
			if (db_monitor) {

				printf("GET ");
				printf("%02X %02X ", buffer2[0], buffer2[1]);
			}
			for (i = 0; i != buffer2[1]; i++) {
				buffer2[i + 2] = buffer[i];
				if (db_monitor)
					printf("%02X ", buffer[i]);

			}
			if (db_monitor)
				printf("\n");
		}
	}
	if (db_monitor)
		printf("\nBuffer Clearing\n");
	switch (cmd) {
	case DB_LED:
		if (buffer2[0] != DB_LED || buffer2[1] != 0) {
			color(ROTh);
			printf("Callback ERROR by Discbot!\n");
			color(rcol);
			return 0;
		}
		break;
	case DB_LOUD:
		if (lenIn == 0) {
			if (buffer2[0] != DB_LOUD || buffer2[1] != 1) {
				return 0;
			}
		}
		if (lenIn == 1) {
			if (buffer2[0] != DB_LOUD || buffer2[1] != 0) {
				return 0;
			}
		}break;
	case DB_BTN:
		if (lenIn == 0) {
			if (buffer2[0] != DB_BTN || buffer2[1] != 1) {
				return 0;
			}
		}
		break;
	case DB_RFID:
		if (lenIn == 0) {
			if (buffer2[0] != DB_RFID || (buffer2[1] != 0 && buffer2[1] != 4)) {
				return 0;
			}
		}
		break;
	case DB_CS:
		if (lenIn == 0) {
			if (buffer2[0] != DB_CS || buffer2[1] != 18) {
				return 0;
			}
		}break;
	case DB_LS:
		if (buffer2[0] != DB_LS || buffer2[1] != 0) {
			return 0;
		}break;
	case DB_LSS:
		if (lenIn == 0) {
			if (buffer2[0] != DB_LSS || buffer2[1] != 0) {
				return 0;
			}
		}break;
	case DB_BM:
		if (lenIn == 0) {
			if (buffer2[0] != DB_BM || buffer2[1] != 2) {
				return 0;
			}
		}break;
	case DB_IRS:
		if (lenIn == 0) {
			if (buffer2[0] != DB_IRS || buffer2[1] != 4) {
				return 0;
			}
		}break;
	case DB_USS_G:
		if (lenIn == 0) {
			if (buffer2[0] != DB_USS_G) {
				return 0;
			}
		}break;
	case DB_USS_M:
		if (lenIn == 0) {
			if (buffer2[0] != DB_USS_M) {
				return 0;
			}
		}break;
	case DB_BT:
		if (lenIn == 0) {
			if (buffer2[0] != DB_BT) {
				return 0;
			}
		}break;
	case DB_BUFFER_C:

		if (lenIn == 0x0A) {
			wait(500);
			read_port(db_port, buffer2, 1);
			if (db_monitor)
				printf("GET %02X ", buffer2[0]);
			int room = 0;
			const int max = 50 * 3;
			bool b = 0;
			int endlos = 0;
			do {
				endlos = 0;
				while (room != 12 && endlos <= max && !anyKey()) {
					endlos++;
					if ((endlos % 1) == 0) {
						color(ROTh);
						printf("Buffer clearing in process %3.2lf%% (Press ENTER to leave!!)\r", percent(max, endlos));
						rcolor;
					}
					switch (room) {
					case 1:if (buffer2[0] == 0x0A)room++; else room = 0; break;
					case 2:if (buffer2[0] == 0xFF)room++; else room = 0; break;
					case 3:if (buffer2[0] == 0x00)room++; else room = 0; break;
					case 4:if (buffer2[0] == 0xFF)room++; else room = 0; break;
					case 5:if (buffer2[0] == 0x00)room++; else room = 0; break;
					case 6:if (buffer2[0] == 0xFF)room++; else room = 0; break;
					case 7:if (buffer2[0] == 0x00)room++; else room = 0; break;
					case 8:if (buffer2[0] == 0xFF)room++; else room = 0; break;
					case 9:if (buffer2[0] == 0x00)room++; else room = 0; break;
					case 10:if (buffer2[0] == 0xFF)room++; else room = 0; break;
					case 11:if (buffer2[0] == 0x00)room++; else room = 0; break;
					case 0:if (buffer2[0] == 0x20) room = 1; else room = 0; break;
					default:break;
					}
					if (db_monitor)
						printf("%02X (%i room)", buffer2[0], room);
					if (room != 12)
						read_port(db_port, buffer2, 1);
					//wait(50);
				}

				if (max > endlos)
					b = 0;
				else {
					b = 1;
					color(ROTh);
					printf("\r");
					for (i = 0; i != 100; i++) {
						printf(" ");
					}
					printf("\r");
					printf("Buffer unsucsessfully cleared!\n\tRetry? (y/n)\n\t(You should make a hardware reset instead!!!)\n");
					bell();
					rcolor;
				}
			} while (b && 'y' == _getch());
			if (db_monitor)
				printf("\n");
			printf("\r");
			for (i = 0; i != 100; i++) {
				printf(" ");
			}
			printf("\r");
			if (endlos > max)
				return 0;

			/*if (buffer2[0] != DB_BUFFER_C || buffer2[0] != 0x0A) {
				return 0;
			}*/
		}break;
	case DB_BT_OS:
		if (buffer2[0] != DB_BT_OS || buffer2[1] != 1 || buffer2[2] != parameterIn[0]) {
			return 0;
		}
		break;
	case DB_MC:
		if (buffer2[0] != DB_MC || buffer2[1] != 0) {
			return 0;
		}
		break;
	case DB_MC_D:
		if (buffer2[0] != DB_MC_D || (buffer2[1] != ((12 * 2) + 2) && buffer2[1] != 42)) {
			return 0;
		}
		break;
	case DB_EEPROM:
		if (buffer2[0] != DB_EEPROM || (buffer2[1] != 1 && buffer2[1] != 0)) {
			return 0;
		}
		break;
	case DB_COPY_SONG:
		if (buffer2[0] != DB_COPY_SONG || buffer2[1] != 0) {
			return 0;
		}
		break;
	case DB_CAT_SONG:
		if (buffer2[0] != DB_CAT_SONG || buffer2[1] != 0) {
			return 0;
		}
		break;
	case DB_IRC_BUFFER:
		if (buffer2[0] != DB_IRC_BUFFER || (lenIn == 1 && buffer2[1] != 0)) {
			return 0;
		}
		break;
	case DB_IRC_SEND:
		if (buffer2[0] != DB_IRC_SEND || buffer2[1] != 0) {
			return 0;
		}
		break;
	default:
		if (db_monitor)
			printf("PORT : WORK-NOT-FINE wrong cmd recived\n");

		if (cmd != DB_UNKNOWN && cmd != DB_RESET) {
			color(ROTh);
			printf("Callback was not in list!!!\n");
			rcolor;
		}
		return 0;

	}
	if (db_monitor)
		printf("PORT : WORK-FINE\n");


	return 1;
}

