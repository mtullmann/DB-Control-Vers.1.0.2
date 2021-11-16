#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "utility.h"
#include <Windows.h>

#define PER_CONFIGURE "DB Control\\configuration"
#define PER_SHEET "DB Control\\configuration\\checkSheet.txt"
//#define PER_COM "DB Control\\configuration\\connect.txt"
#define PER_VERS "1.0.2 beta D+ Experimental"
#define PER_CONFIG "DB Control\\configuration\\conf.txt"
#define PER_AVR "DB Control\\configuration\\avrDir.txt"
#define PER_AVR_STD "C:\\WinAVR-20100110\\bin"
#define PER_AVR_FLH "avrdude.exe"
#define PER_TITLE " by Martin Tullmann Vers. "
#define PER_DEVICE "DiscBot"
#define PER_HEX "DB Control\\configuration\\dbosX.hex"
#define DIR_GLOBAL "GLOBAL"

#define PER_AVR "DB Control\\configuration\\avrDir.txt"
#define PER_DEVICE "DiscBot"
#define PER_PNAME "DB Control"
#define NULL 0
void generateC(FILE* f);

static bool cSGlobal = 0;
char* dir_readme() {
	static char x[101] = "";

	strcpy(x, PER_PNAME);
	strcat(x, "\\ReadMe.txt");
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
char* dir_HMfiles() {
	char* x = (char*)malloc(101);
	strcpy(x, DIR_GLOBAL);
	strcat(x, "\\HM-Files");
	return x;
}
void initGlobal() {
	rcolor;
	generateDir(DIR_GLOBAL);
	generateDir(PER_PNAME);

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
		str_inp(ch,101);
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
	color(ROTh);
	printxCx('*', "Global machines", rcol, ROTh, rcol);
	rcolor;
	FILE* f = NULL;
	int coP[251]={0};

	if (fileExist("GLOBAL\\machines.con")) {
		f = fopen("GLOBAL\\machines.con", "r");
		char tex[152] = "";
		for (char i = 0; i != 251&& fgetc(f)!=EOF; i++) {
			
			strcpy(tex, "");
			fscanf(f, "%i", &coP[i]);
			str_inp(f, tex, 151);
			printf("%02i) COM.%i \"%s\"\n",i+1,coP[i],tex);
		}
		fclose(f);
	}
	else {
		color(GELBh);
		printf("No machine found!!\n");
		rcolor;
		printf("\t\tPlease add one in the offline menue!!\n");
	}

}
