#include <stdio.h>

//#define DEBUG

#define MEMSIZE 3000
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
	printf("%s\n", script);
	#endif
	
	int i = 0;
	while (script[i] != '\0') {
		if (script[i] == '[') {
			*sp = i;
			sp++;
		}
		if (script[i] == ']') {
			sp--;
			loops[loopcount].start = *sp;
			loops[loopcount].end = i;
			loopcount++;
		}
		i++;
	}

	if (sp != stack) {
		printf("ERROR: Unclosed brackets.\n");
		return 1;
	}

	#ifdef DEBUG
	for (int i=0; i<loopcount; i++) {
		printf("Loop %d: S%d E%d\n",i,loops[i].start,loops[i].end);
	}
	#endif

	return 0;
}

void bf_execute(char* script)
{	
	if (bf_set_loops(script) == 1) return;
	
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
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("ERROR: Invalid number of arguments.\n");
		return 1;
	}

	FILE *fptr;
	char ch;
	char script[SCRIPTSIZE];
    char *sptr = script;
	
	fptr = fopen(argv[1], "r");
	while ((ch = fgetc(fptr)) != EOF) {
		*sptr = ch;
		sptr++;
	}
	fgets(script, SCRIPTSIZE, fptr);
	fclose(fptr);

	bf_execute(script);
	
	return 0;
}
