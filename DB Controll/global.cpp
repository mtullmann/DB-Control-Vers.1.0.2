#include "global.h"

bool cSGlobal = 0;

void generateC(FILE* f);
static char numer[5] = "";

void buildData(char* build) {
	static char sprntf[20] = "";
	if (build == NULL)
		printf("Build: ");
	if (sprntf[0] == '\0') {
		int h, m, s;
		int d, y;
		char mos[4] = "";
		//long long int b;
		char bn[15];
		//const char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug","Sep", "Oct", "Nov", "Dec" };
		sscanf(__TIME__, "%i:%i:%i", &h, &m, &s);
		sscanf(__DATE__, "%04s %i %i", mos, &d, &y);
		/*for (mo = 0; mo != 12; mo++) {
			if (!strcmp(months[mo], mos)) {
				mo++;
				break;
			}
		}*/
		sprintf(bn, "%02i:%02i:%02i\n", h, m, s);
		sprintf(sprntf, "%s.%02i.%i", mos, d, y);
		//printf("%i.%i.%i\n", d, mo, y);

		//b = d + (mo * 100) + (y * 10000);
		//b =  s+ (m * 100) + (h * 10000);

		sprintf(sprntf, "%s-%s\n", sprntf, bn);
	}
	if (build == NULL)
		printf("%s\n", sprntf);
	if (build != NULL)
		strcpy(build, sprntf);
}
char* dir_readme() {
	char* x = (char*)malloc(101);
	strcpy(x, DIR_PROG_PATH);
	strcat(x, "ReadMe.txt");
	return x;
}
void openReadMe() {
	char x[101] = "";
	strcpy(x, "\"");
	strcat(x, dir_readme());
	strcat(x, "\"");

	//printf(">>%s\n", x);

	system(x);
}
void printTitle(const char* tit) {
	static char title[111] = "";
	strcpy(title, "title ");
	strcat(title, PER_PNAME);

	strcat(title, PER_TITLE);
	strcat(title, PER_VERS);

	strcat(title, tit);
	system(title);
}
void cSopen() {
	if (!cSGlobal)
		system(PER_SHEET);

}
char* dir_specialThanks() {
	char* x = (char*)malloc(101);
	strcpy(x, PER_PNAME);
	strcat(x, "\\specialThanks.txt");
	return x;
}

char* dir_MPreset(const char* y) {
	static char x[101] = "";
	strcpy(x, PER_PNAME);
	strcat(x, "\\motordata");
	if (y != NULL) {
		strcat(x, "\\");
		strcat(x, y);
	}
	return x;
}
void renewConf() {
	FILE* f = fopen(PER_CONFIG, "w+");
	generateC(f);
	fclose(f);
}
void openConf() {
	char x[101] = "";
	strcpy(x, "\"");
	strcat(x, PER_CONFIG);
	strcat(x, "\"");
	system(x);
}
void global_port_add() {
	FILE* f;
	//fopen("GLOBAL\\machines.con", "a+");
	if ((f = fopen("GLOBAL\\machines.con", "a+")) != NULL) {
		printf("COM-port: ");
		int i = int_inp(1, 20);
		printf("Device-name: ");
		char ch[102] = "";
		str_inp(ch, 101);
		fprintf(f, "\n%i %s", i, ch);
		fclose(f);
	}
	else {
		color(ROTh);
		printf("FAILD!!!\n");
		rcolor;
	}

}
void global_port_manager() {
	while (1) {
		bool haveNO = 0;
		color(ROTh);
		printxCx('*', "Global machines", rcol, ROTh, rcol);
		rcolor;
		FILE* f = NULL;
		int coP[251] = { 0 };

		if (fileExist("GLOBAL\\machines.con")) {
			f = fopen("GLOBAL\\machines.con", "r");
			char tex[251][152];
			char b = 1;
			unsigned char i = 0;
			if (EOF != fgetc(f)) {
				printf("00) ADD Device!\n");
				for (i = 0; i != 251 && b == 1; i++)
				{

					tex[i][0] = '\0';
					tex[i][1] = '\0';
					fscanf(f, "%i", &coP[i]);
					b = str_inp(f, tex[i], 151);
					strcpy(tex[i], &tex[i][1]);
					printf("%02i) COM.%i \"%s\"\n", i + 1, coP[i], tex[i]);
				}

				fclose(f);
				printf("Select one of them: ");
				unsigned char vanderwahl = int_inp(0, i);
				if (vanderwahl != 0) {
					printf("Selected: COM.%i , \"%s\"\n", coP[vanderwahl - 1], tex[vanderwahl - 1]);
					sprintf(numer, "%i", coP[vanderwahl - 1]);
					return;
				}
				global_port_add();

			}
			else
				haveNO = 1;
		}
		else
			haveNO = 1;
		if (haveNO) {
			color(GELBh);
			printf("No machine found!!\n");
			rcolor;
			printf("\t\tPlease add one now!!\n");
			global_port_add();
		}
	}
}
void getportnum(char* num) {
	if (num == NULL)
		return;
	if (numer[0] == '\0') {
		global_port_manager();
	}

	if (num != NULL)
		strcpy(num, numer);
}


void generateBT();
char* dir_BT() {
	static char temp[101] = "";
	strcpy(temp, DIR_GLOBAL);
	strcat(temp, "\\");
	strcat(temp, DIR_BT);
	return temp;
}
void initGlobal() {
	generateDir(PER_PNAME);
	generateDir(DIR_GLOBAL);
	generateDir(dir_BT());

	generateBT();
}
void generateBT() {
	FILE* f;
	char text[] = "ATS AT\\r\\n\nATG OK\\r\\n\n\nUSERNOTE Bluetooth-Low-Energy\\n\\t\\tThis\\_is\\_the\\_standard\\_comand-configuration\\_for\\_the\\_BT\\_module!!!\n\nPOWES \\-\nPOWEG \\-\n\nPOWES? \\-\nPOWEG? \\-\n\nNAMES? AT+NAME\\r\\n\nNAMEG? +NAME=<P>\\r\\n\n\nNAMES AT+NAME<P>\\r\\n\nNAMEG +NAME=<P>\\r\\nOK\\r\\n\n\nPASSS? AT+PASS\\r\\n\nPASSG? +PASS=<P>\\r\\n\n\nPASSS AT+PIN<P>\\r\\n\nPASSG +PIN=<P>\\r\\nOK\\r\\n\n\nRENEWS AT+RENEW\\r\\n\nRESETS AT+RESET\\r\\n\n\nRENEWG +RENEW\\r\\nOK\\r\\n\nRESETG +RESET\\r\\nOK\\r\\n\n\nROLES? AT+ROLE\\r\\n\nROLEG? +ROLE=<P>\\r\\n\n\nROLES AT+ROLE<P>\\r\\n\nROLEG +ROLE=<P>\\r\\nOK\\r\\n\n\nTEMPS? \\-\nTEMPG? \\-\n\nTYPES? AT+TYPE\\r\\n\nTYPEG? +TYPE=<P>\\r\\n\n\nTYPES AT+TYPE<P>\\r\\n\nTYPEG +TYPE=<P>\\r\\nOK\\r\\n\n\nRADDS? AT+RADD\\r\\n\nRADDG? +RADD=<P>\\r\\nOK\\r\\n\n\nVERSS? AT+VERSION\\r\\n\nVERSG? <P>\\r\\n\n\nPWRS? AT+PWRM\\r\\n\nPWRG? +PWRM=<P>\\r\\n\n\nPWRS AT+PWRM<P>\\r\\n\nPWRG +PWRM=<P>\\r\\Ok\\r\\n\n\nRSSIS? \\-\nRSSIG? \\-\n\nIMMES? \\-\nIMMEG? \\-\n\nIMMES AT+IMME\\r\\n\nIMMEG +IMME=<P>\\r\\nOK\\r\\n\n\nNOTIS? AT+NOTI\\r\\n\nNOTIG? +NOTI=<P>\\r\\n\n\nNOTIS AT+NOTI<P>\\r\\n\nNOTIG +NOTI=<P>\\r\\nOK\\r\\n\n\nADDRS? AT+ADDR\\r\\n\nADDRG? +ADDR=<P>\\r\\n\n\nBATTS? \\-\nBATTG? \\-\n\nHELPS? \\-\nHELPG? \\-\n\nMODES? AT+MODE\\r\\n\nMODEG? +MODE=<P>\\r\\n\n\nMODES AT+MODE<P>\\r\\n\nMODEG +MODE=<P>\\r\\nOK\\r\\n\n\nPARIS? AT+PARI\\r\\n\nPARIG? +PARI=<P>\\r\\n\n\nPARIS AT+PARI<P>\\r\\n\nPARIG +PARI=<P>\\r\\nOK\\r\\n\n\nOPS? AT+PCTL\\r\\n\nOPG? +PCTL=<P>\\r\\n\n\nOPS AT+PCTL<P>\\r\\n\nOPG +PCTL=<P>\\r\\nOK\\r\\n\n\nLEDS? \\-\nLEDG? \\-\n\nLEDS \\-\nLEDG \\-\n\nSTOPS? AT+STOP\\r\\n\nSTOPG? +STOP=<P>\\r\\n\n\nSTOPS AT+STOP<P>\\r\\n\nSTOPG +STOP\\r\\nOK\\r\\n\n\nUUIDS? AT+UUID\\r\\n\nUUIDG? +UUID=<P>\\r\\n\n\nUUIDS AT+UUID<P>\\r\\n\nUUIDG +UUID=<P>\\r\\nOK\n\nTCONS? \\-\nTCONG? \\-\n\nTCONS \\-\nTCONG \\-\n\nCONNG OK+CONN\\r\\n\nLOSTG OK+LOST\\r\\n\nWAKEG OK+WAKE\\r\\n\n";
	char text1[] = "ATS AT\nATG OK\n\nUSERNOTE Bluetooth-Low-Energy\\n\\t\\tYou\\_have\\_to\\_select\\_this\\_if\\_you\\_had\\_flashed\\_your\\_module\\_with\\_CCLoader!!!\n\nPOWES AT+POWE<P>\nPOWEG OK+Get:<P>\n\nPOWES? AT+POWE?\nPOWEG? OK+Get:<P>\n\nNAMES? AT+NAME?\nNAMEG? OK+NAME:<P>\n\nNAMES AT+NAME<P>\nNAMEG OK+Set:<P>\n\nPASSS? AT+PASS?\nPASSG? OK+Get:<P>\n\nPASSS AT+PIN<P>\nPASSG OK+Set:<P>\n\nRENEWS AT+RENEW\nRESETS AT+RESET\n\nRENEWG OK+RENEW\nRESETG OK+RESET\n\nROLES? AT+ROLE?\nROLEG? OK+Get:<P>\n\nROLES AT+ROLE<P>\nROLEG OK+Set:<P>\n\nTEMPS? AT+TEMP?\nTEMPG? OK+Get:<P>\n\nTYPES? AT+TYPE?\nTYPEG? OK+Get:<P>\n\nTYPES AT+TYPE<P>\nTYPEG OK+Set:<P>\n\nRADDS? AT+RADD?\nRADDG? OK+RADD:<P>\n\nVERSS? AT+VERS?\nVERSG? <P>\n\nPWRS? AT+PWRM?\nPWRG? OK+Get:<P>\n\nPWRS AT+PWRM<P>\nPWRG OK+Set:<P>\n\nRSSIS? AT+RSSI?\nRSSIG? OK+RSSI:<P>\n\nIMMES? AT+IMME?\nIMMEG? OK+Get:<P>\n\nIMMES AT+IMME<P>\nIMMEG OK+Set:<P>\n\nNOTIS? AT+NOTI?\nNOTIG? OK+Get:<P>\n\nNOTIS AT+NOTI<P>\nNOTIG OK+Set:<P>\n\nADDRS? AT+ADDR?\nADDRG? OK+ADDR:<P>\n\nBATTS? AT+BATT?\nBATTG? OK+Get:<P>\n\nHELPS? AT+HELP?\nHELPG? <P>\n\nMODES? AT+MODE?\nMODEG? OK+Get:<P>\n\nMODES AT+MODE<P>\nMODEG OK+Set:<P>\n\nPARIS? AT+PARI?\nPARIG? OK+Get:<P>\n\nPARIS AT+PARI<P>\nPARIG OK+Set:<P>\n\nOPS? AT+PCTL?\nOPG? OK+Get:<P>\n\nOPS AT+PCTL<P>\nOPG OK+Set:<P>\n\nLEDS? AT+PIO1?\nLEDG? OK+Get:<P>\n\nLEDS AT+PIO1<P>\nLEDG OK+Set:<P>\n\nSTOPS? AT+STOP?\nSTOPG? OK+Get:<P>\n\nSTOPS AT+STOP<P>\nSTOPG OK+Set:<P>\n\nUUIDS? AT+UUID?\nUUIDG? OK+Get:<P>\n\nUUIDS AT+UUID<P>\nUUIDG OK+Set:<P>\n\nTCONS? AT+TCON?\nTCONG? OK+Get:<P>\n\nTCONS AT+TCON<P>\nTCONG OK+Set:<P>\n\nCONNG OK+CONN\nLOSTG OK+LOST\nWAKEG OK+WAKE\n";
	char text2[] = "ATS AT+VERSION\\r\\n\nATG +VERSION=JDY-31-V1.35,Bluetooth\\_V3.0\\r\\n\n\nUSERNOTE Bluetooth-Classic\\n\\t\\tDo\\_not\\_forget\\_the\\_voltage\\_divider\\_on\\_RX\\_of\\_the\\_module!!!\\n\\t\\tIf\\_it\\_does\\_not\\_work\\_look\\_at\\_this\\_file\\_and\\_change\\_the\\_\"ATG\"\\_line\\_to\\_the\\_right\\_Version!!\\n\\t\\t\\tNo\\_real\\_AT\\_command\\_for\\_this\\_was\\_found!!!\n\nPOWES \\-\nPOWEG \\-\n\nPOWES? \\-\nPOWEG? \\-\n\nNAMES? AT+NAME\\r\\n\nNAMEG? +NAME=<P>\\r\\n\n\nNAMES AT+NAME<P>\\r\\n\nNAMEG +OK\\r\\n\n\nPASSS? AT+PIN\\r\\n\nPASSG? +PIN=<P>\\r\\n\n\nPASSS AT+PIN<P>\\r\\n\nPASSG +OK\\r\\n\n\nRENEWS AT+DEFAULT\\r\\n\nRESETS AT+RESET\\r\\n\n\nRENEWG +OK\\r\\n\nRESETG +OK\\r\\n\n\nROLES? \\-\nROLEG? \\-\n\nROLES \\-\nROLEG \\-\n\nTEMPS? \\-\nTEMPG? \\-\n\nTYPES? \\-\nTYPEG? \\-\n\nTYPES \\-\nTYPEG \\-\n\nRADDS? \\-\nRADDG? \\-\n\nVERSS? AT+VERSION\\r\\n\nVERSG? +VERSION=<P>\\r\\n\n\nPWRS? \\-\nPWRG? \\-\n\nPWRS \\-\nPWRG \\-\n\nRSSIS? \\-\nRSSIG? \\-\n\nIMMES? \\-\nIMMEG? \\-\n\nIMMES \\-\nIMMEG \\-\n\nNOTIS? \\-\nNOTIG? \\-\n\nNOTIS \\-\nNOTIG \\-\n\nADDRS? AT+LADDR\\r\\n\nADDRG? +LADDR=<P>\\r\\n\n\nBATTS? \\-\nBATTG? \\-\n\nHELPS? \\-\nHELPG? \\-\n\nMODES? \\-\nMODEG? \\-\n\nMODES \\-\nMODEG \\-\n\nPARIS? \\-\nPARIG? \\-\n\nPARIS \\-\nPARIG \\-\n\nOPS? \\-\nOPG? \\-\n\nOPS \\-\nOPG \\-\n\nLEDS? \\-\nLEDG? \\-\n\nLEDS \\-\nLEDG \\-\n\nSTOPS? \\-\nSTOPG? \\-\n\nSTOPS \\-\nSTOPG \\-\n\nUUIDS? \\-\nUUIDG? \\-\n\nUUIDS \\-\nUUIDG \\-\n\nTCONS? \\-\nTCONG? \\-\n\nTCONS \\-\nTCONG \\-\n\nCONNG CONNECTED\\r\\r\\n\nLOSTG +DISC:SUCCSTSS\\r\\rn\nWAKEG \\-\n";

	char* textes[3] = {
		text1,text,text2
	};
	char names[3][100] = {
		"HMSoft_V540",
		"MLT-BT05_V4.1",
		"JDY-31-SPP"
	};
	char temp[101] = "";
	for (int i = 0; i != 3; i++) {
		strcpy(temp, "");
		strcpy(temp, dir_BT());
		strcat(temp, "\\");
		strcat(temp, names[i]);
		strcat(temp, BT_ENDING);
		//printf("Names: %s\ntemp: %s\n", names[i], temp);
		if ((f = fopen(temp, "r")) == NULL) {
			//printf("Create %s\n",temp);
			f = fopen(temp, "a+");
			if (f != NULL) {
				fprintf(f, textes[i]);
				fclose(f);
			}
			else {
				//printf("Create fail %s\n", temp);

			}
		}
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
	char dirName[101] = "";
	char pathName[101] = "";
	strcpy(dirName, "");
	strcat(dirName, dir_BT());
	strcpy(pathName, dirName);
	strcat(dirName, "\\");
	int tempLen = strlen(dirName);
	strcat(dirName, "use.TXT");
	//printf("**dir_%s\n", dirName);
	//printf("**path_%s\n", pathName);
	if ((f = fopen(dirName, "r")) != NULL) {
		strcpy(text, dir_BT());
		strcat(text, "\\");
		int i = strlen(text);
		while ((text[i] = fgetc(f)) != EOF) {
			i++;
		}
		text[i] = '\0';
		//printf("FILE : %s\n", text);
		strcat(filename, &text[tempLen]);
		fclose(f);
		if ((f = fopen(text, "r")) != NULL) {

			//	printf("FILE : %s\n", text);
			fclose(f);
			return 1;
		}
		else {
			color(ROTh);
			printf("Invalid input at file \"%s\"\n", dirName);
			rcolor;
			color(GELBh);
			printf("Fix bug yourself...\n");
			rcolor;
			system(dirName);
		}
	}
	else {
		color(ROTh);
		printf("File \"%s\" not found!\n", dirName);
		color(GELBh);
		printf("Create one...\n");
		rcolor;
	}
	if ((dir = opendir(pathName)) != NULL) {
		color(GRUENh);
		printf("Open \"%s\" directory...\n", pathName);
		rcolor;
		while ((ent = readdir(dir)) != NULL) {
			//printf("Name: %s\n", ent->d_name);
			if (ent->d_name[0] != '.') {
				if (strlcmp((char*)BT_ENDING, &ent->d_name[ent->d_namlen - strlen(BT_ENDING)])) {
					if (strcmp(ent->d_name, "use.TXT")) {
						strcpy(tex, ent->d_name);
						tex[ent->d_namlen - strlen(BT_ENDING)] = '\0';
						color(GELBh);
						printf("(%i)", ++maxFiles);
						rcolor;
						printf("\t \"%s\" ", tex);
						char fnam[101];
						sprintf(fnam, "%s\\%s%s", pathName, tex, BT_ENDING);
						//printf("%s\n",fnam);
						FILE* f2 = fopen(fnam, "a+");
					//	printf("OPEN \"%s\"\n", fnam);
						if (f2 != NULL && bt_get_cmd(f2, "USERNOTE", userNote))
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
				if (strlcmp((char*)BT_ENDING, &ent->d_name[ent->d_namlen - strlen(BT_ENDING)])) {
					if (strcmp(ent->d_name, "use.TXT")) {
						strcpy(tex, ent->d_name);
						tex[ent->d_namlen - strlen(BT_ENDING)] = '\0';
						//printf("\t %s\n", tex);
						memcpy(&names[namesp], tex, strlen(tex) + 1);
						namesp += (strlen(tex) + 1);

					}
				}
			}
		}
		//printf("END dir\n");
		closedir(dir);


		printf("%i file%c found\n", maxFiles, (maxFiles == 1) ? '\0' : 's');
		if (maxFiles == 1) {
			color(GRUENh);
			printf("Auto choose \"%s\"! If you want to change delete \"%s\"!\n", names, dirName);
			rcolor;
			printf("Press any key!\n");
			_getch();
			strcpy(filename, names);
			strcat(filename, ".TXT");
			f = fopen(dirName, "w+");
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
			f = fopen(dirName, "w+");
			if (f != NULL) {
				fprintf(f, "%s", filename);
				fclose(f);
			}
			return 1;
		}
	}

	color(ROTh);
	printf("Open \"%s\" directory...\n", pathName);
	printf("No dir!\n");
	rcolor;
	_getch();
	return 0;
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
bool bt_get_cmd(FILE* fj, const char* in, char* out) {
	FILE* f;
	char filename[101] = "";
	char filenameX[101] = "";
	char line[101] = "";
	char useFileName[101] = "";
	strcpy(useFileName, dir_BT());
	strcat(useFileName, "\\");
	strcat(useFileName, "use.TXT");
	if (fj == NULL) {
		show_bt_dir(filename);
		if ((f = fopen(useFileName, "r")) == NULL) {

			if ((f = fopen(useFileName, "a+")) == NULL) {
				color(ROTh);
				printf("File can't be generated!\n");
				rcolor;
				return 0;
			}
			else {
				fprintf(f, "BT_version.TXT");
				fclose(f);
				return bt_get_cmd(NULL, in, out);
			}
		}
		fclose(f);
		f = fopen(useFileName, "a+");
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
			strcpy(filenameX, dir_BT());
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
			system(useFileName);
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