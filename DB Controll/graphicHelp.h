#pragma once
struct GPU_GRAPH {
	int colr[100];
	char c[1000];
};
void gpu_rectangle(int len, int height, char symb, char colr, GPU_GRAPH* g) {
	g->c[0] = '\0';
	for(int i=0;i!=100;i++)
	g->colr[i]= colr;
	int l = 0;
	for (int i = 0; i != len * 2;i++) {
		g->c[l++] = symb;
		if ((len*2-1)==i&&height!=0) {
			height--;
			i = -1;
			g->c[l++] = '\n';
		}
	}
	g->c[l++] = '\0';
	//printf(g->c);
}
void gpu_extractLine(char *c,int line,char *out) {
	int l = 0;
	for (int i = 0; i != line;i++) {
		while (c[l]!='\0'&& c[l++] != '\n');
	}
	strcpy(out,&c[l]);
	l = 0;
	while (out[l] != '\0' && out[l++] != '\n');
	if (out[l] != '\0')
		out[l-1]='\0';
}