#include <string.h>

int asm_ () {
	FILE* file = fopen("input.dat", "r");
	system("dir");
	 if (file == NULL) {
		 printf ("Error"); 
		 return -1;
	} else { 
		printf ("Ok\n");
	};
	size_t N = 10;
	size_t i = 0;   
	char* buf = (char*) malloc (sizeof(char)*N);
	memset(buf, '0', 11);
	while (getc(file) != EOF) {
		buf[i] = getc(file);
		i += 1;
		if (i >= N) {
			N += 10;
			buf = (char*) realloc (buf, sizeof(char)*N);        
		}   
	} 
	fclose(file);
	puts(buf);
	return 0;
}
