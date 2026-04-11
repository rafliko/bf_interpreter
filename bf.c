#include <stdio.h>
#include "colors.h"

#define DEBUG

#define MEMSIZE 30000
#define SCRIPTSIZE 4096
#define STACKSIZE 512

char memory[MEMSIZE] = {0};
char* mp = memory;

int stack[STACKSIZE] = {0};
int* sp = stack;

struct loop {
	int start;
	int end;
};

struct loop loops[STACKSIZE];
int loopcount = 0;

int bf_set_loops(char* script)
{
	#ifdef DEBUG
	printf("%s%s\n%s",CYN,script,CRESET);
	#endif
	
	int i = 0;
	while (script[i] != '\0') {
		if (script[i] == '[') {
			*sp = i;
			sp++;
		}
		if (script[i] == ']') {
			sp--;
			if (sp < stack) { // underflow
				printf("%sERROR:%s Unclosed brackets.\n",RED,CRESET);
				return 1;
			}
			loops[loopcount].start = *sp;
			loops[loopcount].end = i;
			loopcount++;
		}
		i++;
	}

	if (sp != stack) {
		printf("%sERROR:%s Unclosed brackets.\n",RED,CRESET);
		return 1;
	}

	#ifdef DEBUG
	for (int i=0; i<loopcount; i++) {
		printf("%sLoop %d: S%d E%d%s\n",CYN,i,loops[i].start,loops[i].end,CRESET);
	}
	putchar('\n');
	#endif

	return 0;
}

int bf_execute(char* script)
{	
	if (bf_set_loops(script) == 1) return 1;
	
	int i = 0;
	while (script[i] != '\0') {
		switch (script[i]) {
		case '>':
			mp++;
			break;
		case '<':
			mp--;
			break;
		case '+':
			(*mp)++;
			break;
		case '-':
			(*mp)--;
			break;
		case '.':
			putchar(*mp);
			break;
		case ',':
			*mp = getchar();
			break;
		case '[':
		    if (*mp == 0) {
				for (int j=0; j<loopcount; j++) {
					if (loops[j].start==i) {
						i = loops[j].end;
						break;
					}
				}
			}
			break;
		case ']':
			for (int j=0; j<loopcount; j++) {
				if (loops[j].end==i) {
					i = loops[j].start-1;
					break;
				}
			}
			break;
		default:
			break;
		}
		i++;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("%sERROR:%s: Invalid number of arguments.\n",RED,CRESET);
		return 1;
	}

	FILE *fptr;
	char ch;
	char script[SCRIPTSIZE];
    char *sptr = script;
	
	fptr = fopen(argv[1], "r");
	if (fptr == NULL) {
		printf("%sERROR:%s File does not exist.\n",RED,CRESET);
		return 1;
	}
	while ((ch = fgetc(fptr)) != EOF) {
		*sptr = ch;
		sptr++;
	}
	fclose(fptr);

	return bf_execute(script);
}
