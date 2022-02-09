#include "sheet.h"

#define MAX_CATEGORY 100
#define FAILS_PER_CATEGORY 2+12
#define MAX_FAIL FAILS_PER_CATEGORY*MAX_CATEGORY
#define STAR_START 30
#define STAR_END 25

static FILE* sheet = NULL;
static char cSstartTime[22] = "";
static char cSentTime[22] = "";
static char cSportS[20] = "";
static int cScategoryCnt = 0;
static int cSfalueCnt = 0;
static char cSfalue[MAX_FAIL][201];
static int cSfalueCCnt[MAX_CATEGORY];
char cSnameOfTest[201] = "";

static bool cSconnect = 0;
static char cScategoryS[MAX_CATEGORY][100];
static void startTime();
static bool write = 1;
void cSdisable() {
	write = 0;
}
void cSenable() {
	write = 1;
}
void cScreate() {
	if (NULL == (sheet = fopen(PER_SHEET, "a+"))) {
		color(ROTh);
		printf("No \"%s\" file could be generated!!\n", PER_SHEET);
		rcolor;
		return;
	}
	rcolor;
	printf("Do you want to enter a");
	color(GELBh);
	printf(" name for the test");
	rcolor;
	printf("? (y/n)\n");
	if ('y' == ynrespond()) {
		printf(": ");
		str_inp(cSnameOfTest, 200);
		rcolor;
	}
	else {
		strcpy(cSnameOfTest, "untitled");
	}
	startTime();
	rewind(sheet);
	fseek(sheet, SEEK_SET, SEEK_CUR);
	cSconnect = 1;
	cScategoryCnt = 0;
	for (int i = 0; i != (int)MAX_CATEGORY; i++)
		cSfalueCCnt[i] = 0;

	cSfalueCnt = 0;
	for (int i = 0; i != STAR_START; i++)
		fprintf(sheet, "*");
	fprintf(sheet, "\n\tStart\n");

	for (int i = 0; i != STAR_START; i++)
		fprintf(sheet, "*");
	fprintf(sheet, "\n");
	fprintf(sheet, "Name: %s\n", cSnameOfTest);
}
void cSaddMsg(const char* x) {
	if (!cSconnect)return;
	if (write)
		fprintf(sheet, x);
}
void cSaddENote(const char* x) {
	if (!cSconnect)return;
	if (write)
		fprintf(sheet, "|EditorNote::\"%s\"\\\"%s\"\\_%s",PER_PNAME, cScategoryS[cScategoryCnt],x);
}
void cScategory(char* x) {
	if (!cSconnect)return;
	if (!write)
		return;
	strcpy(cScategoryS[cScategoryCnt], x);
	fprintf(sheet, "\n#Test category: %s\n", x);
}
void cScError(const char* x) {
	char y[250] = "";
	if (!cSconnect)return;
	if (!write)
		return;
	strcpy(cSfalue[cSfalueCnt], x);
	cSfalueCCnt[cScategoryCnt]++;
	strcat(y, "*");
	strcat(y, x);
	cSaddMsg(y);
	cSfalueCnt++;
	//color(ROTh);
	//printf(x);
	//color(rcol);
}
void cScategoryEnd() {
	if (!cSconnect)return;
	fprintf(sheet, "\nResult of %s: ", cScategoryS[cScategoryCnt]);
	color(GELBh);
	printf("\nResult of %s: ", cScategoryS[cScategoryCnt]);
	rcolor;
	if (cSfalueCCnt[cScategoryCnt] == 0) {
		fprintf(sheet, "No Errors occurred!\n");
		color(GRUENh);
		printf("No Errors occurred!\n");
		rcolor;
	}
	else {
		char x[100001] = "";
		sprintf(x, "%i Error%soccurred\n", cSfalueCCnt[cScategoryCnt], (cSfalueCCnt[cScategoryCnt] == 1) ? " " : "s ");
		fprintf(sheet, x);

		color(ROTh);
		printf(x);
		for (int i = 0; i != (cSfalueCCnt[cScategoryCnt] + 1); i++) {
			fprintf(sheet, cSfalue[(cSfalueCnt - cSfalueCCnt[cScategoryCnt] + i)]);
			printf(cSfalue[(cSfalueCnt - cSfalueCCnt[cScategoryCnt] + i)]);
		}
		rcolor;

	}
	fprintf(sheet, "#End of %s\n", cScategoryS[cScategoryCnt]);
	color(GELBd);
	printf("#End of %s\n", cScategoryS[cScategoryCnt]);
	rcolor;
	cScategoryCnt++;
}
void cSport(char* x) {
	strcpy(cSportS, x);
}
void endTime()
{
	struct tm* c;
	time_t now;
	now = time(0);
	c = localtime(&now);
	sprintf(cSentTime, "%02i:%02i:%02i_%02i.%02i.%04i", c->tm_hour, c->tm_min, c->tm_sec, c->tm_mday, c->tm_mon + 1, c->tm_year + 1900);
}
void startTime()
{
	struct tm* c;
	time_t now;
	now = time(0);
	c = localtime(&now);
	sprintf(cSstartTime, "%02i:%02i:%02i_%02i.%02i.%04i", c->tm_hour, c->tm_min, c->tm_sec, c->tm_mday, c->tm_mon + 1, c->tm_year + 1900);
}
void db_data_transmission_byte(double*);
void cSend() {
	if (!cSconnect)return;
	double xava[2] = { 0,0 };
	db_data_transmission_byte(xava);
	int currCnt = 0;
	char x[10001] = "";
	for (int i = 0; i != STAR_END; i++)
		fprintf(sheet, "*");
	fprintf(sheet, "\n");
	endTime();
	fprintf(sheet, "%s connected to %s\n", PER_DEVICE, cSportS);
	fprintf(sheet, "Started at: %s \tEnded at: %s\n", cSstartTime, cSentTime);
	fprintf(sheet, "%0.0lf Byte%swere recived & %0.0lf Byte%swere sent!!\n", xava[1], (xava[1] != 1) ? " " : "s ", xava[0], (xava[0] != 1) ? " " : "s ");
	sprintf(x, "\n\n##Completly test results: %i Error%soccurred!\n", cSfalueCnt, (cSfalueCnt == 1) ? " " : "s ");

	fprintf(sheet, x);
	color(GELBh);
	printf(x);

	rcolor;
	for (int in = 0; in != cScategoryCnt; in++) {

		fprintf(sheet, "\n#Result of %s: ", cScategoryS[in]);
		color(GELBh);
		printf("\n#Result of %s: ", cScategoryS[in]);
		if (cSfalueCCnt[in] == 0) {
			fprintf(sheet, "No Errors occurred!\n");
			color(GRUENh);
			printf("No Errors occurred!\n");
		}
		else {

			color(ROTh);
			sprintf(x, "%i Error%soccurred\n", cSfalueCCnt[in], cSfalueCCnt[in] == 1 ? " " : "s ");
			printf(x);
			fprintf(sheet, x);

			for (int i = 0; i != (cSfalueCCnt[in]); i++) {
				fprintf(sheet, cSfalue[currCnt]);
				printf(cSfalue[currCnt]);
				currCnt++;
			}
			rcolor;
		}
		fprintf(sheet, "#End of result %s\n", cScategoryS[in]);
		color(GELBd);
		printf("#End of result %s\n", cScategoryS[in]);
		rcolor;
	}
	fprintf(sheet, "\n##End of Completly test results\n");
	for (int i = 0; i != STAR_END; i++)
		fprintf(sheet, "*");
	fprintf(sheet, "\n");

	fclose(sheet);
	color(rcol);
}

void cSaskIfFine() {
	color(GELBh);
	printf("Does %s work fine? (y/n)\n", cScategoryS[cScategoryCnt]);
	rcolor;
	char c = 0;
	flush();
	if ('y' != ynrespond()) {
		printf("Please type in an error message!\nFor newline type \\n\n:");
		char err[253] = "";
		char err2[253] = "~";
		color(ROTh);
		//flush();
		do {
			gets_s(err, 251);
		} while (err[0] == '\0' || err[0] == '\n');
		//printf("%i \"%c\"\n",err[0],err[0]);
		rcolor;
		for (int i = 0; err[i + 1] != '\0'; i++)
			if (err[i] == '\\' && err[i + 1] == 'n') {
				err[i] = '\n';
				err[i + 1] = ' ';
			}

		strcat(err2, err);
		strcat(err2, "\n");
		cScError(err2);
	}
	else {
		cSaddMsg("~Everything worked fine\n");
	}
}
