#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>

#define PER_CONFIGURE "configuration"
#define PER_SHEET "configuration\\checkSheet.txt"
#define PER_COM "configuration\\connect.txt"
#define PER_VERS "1.0.2 beta D+ Experimental"
#define PER_CONFIG "configuration\\conf.txt"
#define PER_AVR "configuration\\avrDir.txt"
#define PER_AVR_STD "C:\\WinAVR-20100110\\bin"
#define PER_AVR_FLH "avrdude.exe"
#define PER_TITLE " by Martin Tullmann Vers. "
#define PER_DEVICE "DiscBot"
#define PER_HEX "configuration\\dbosX.hex"
#define DIR_GLOBAL "GLOBAL"

#define PER_CONFIGURE "configuration"
#define PER_AVR "configuration\\avrDir.txt"
#define PER_DEVICE "DiscBot"
#define PER_PNAME "DB Control"
#define NULL 0

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
void initGlobal() {
	char c[101];
	strcpy(c, "mkdir ");
	strcat(c, "\"");
	strcat(c, DIR_GLOBAL);
	strcat(c, "\"");

	system(c);

	
	char* xp = dir_readme();
	FILE* f;
	if (!(f=fopen(xp, "r"))) {
		strcpy(c, "mkdir ");
	strcat(c, "\"");
	strcat(c, PER_PNAME);
	strcat(c, "\"");
	system(c);
	 }
	else {
		fclose(f);
	}
	//fclose(fopen(dir_readme(), "a+"));
}
