#include <string.h>

enum COMMANDS {
CMD_PUSH,
CMD_POP,
CMD_OUT,
CMD_ADD,
CMD_INT,
CMD_IN,
CMD_ERR,
CMD_ARG_ERR
};

enum REGISTR {
	REG_AX,
	REG_BX,
	REG_CX,
	REG_DX
};

size_t token_read(char* buf, char* str, size_t pos);

const char cmds[20][20] = {"push", "pop", "out", "add", "int", "in"};
const char registr[10][10] = {"ax", "bx", "cx", "dx"};


size_t token_read (char* buf, char* str, size_t pos) {
	size_t i = pos;
	size_t j = 0;
	while ((buf[i] != ' ')&&(buf[i] != '\0')) {
		str[j] = buf[i];
		i++;
		j++;
	}
	return i;
}



int asm_ () {
	const char cmds[20][20] = {"push", "pop", "out", "add", "int", "in"};
	const char registr[10][10] = {"ax", "bx", "cx", "dx"};
	FILE* file = fopen("prog.txt", "r");
	FILE* file_out = fopen("output.txt", "w");
	 if (file == NULL) {
		 printf ("Error"); 
		 return -1;
	};
	fseek(file, 0, SEEK_END);
	long file_len = ftell(file);
	char* buf = (char*)malloc(sizeof(char) * file_len);
	fseek(file, 0, SEEK_SET);
	memset(buf, '-', file_len);
	fread(buf, 1, file_len, file);
	buf[file_len] = '\0';
	size_t i = 0;
	char* str = (char*)malloc(sizeof(char) * file_len);
		str[file_len] = '\0';
	while (buf[i] != '\0') {
		i = token_read(buf, str, i);
		if (!strcmp(str, cmds[CMD_PUSH])) {
			memset(str, '\0', file_len);
			i = token_read(buf, str, i+1);
			if (!strcmp(str, registr[REG_AX])) {
				fprintf(file_out, "%d", CMD_PUSH);
				fprintf(file_out, "%c%d",'r', REG_AX);
			} else if (!strcmp(str, registr[REG_BX])) {
				fprintf(file_out, "%d", CMD_PUSH);
				fprintf(file_out, "%c%d",'r', REG_BX);
			} else if (!strcmp(str, registr[REG_CX])) {
				fprintf(file_out, "%d", CMD_PUSH);
				fprintf(file_out, "%c%d", 'r', REG_CX);
			} else if (!strcmp(str, registr[REG_DX])) {
				fprintf(file_out, "%d", CMD_PUSH);
				fprintf(file_out, "%c%d", 'r', REG_DX);
			} else if (('0' < str[0])&&(str[0] <= '9')) {
				for (int k = 0; k < strlen(str); k++) {
					if (!(('0' <= str[k])&&(str[k] <= '9'))) {
						fprintf(file_out, "%d", CMD_ARG_ERR);
						return -1;
					};
				}
				fprintf(file_out, "%d", CMD_PUSH);
				fprintf(file_out, "%s", str);
			} else {
				fprintf(file_out, "%d", CMD_ERR);
				return -1;
			}
		} else if (!strcmp(str, cmds[CMD_POP])) {
			memset(str, '\0', file_len);
			i = token_read(buf, str, i+1);
			if (!strcmp(str, registr[REG_AX])) {
				fprintf(file_out, "%d", CMD_POP);
				fprintf(file_out, "%c%d",'r', REG_AX);
			} else if (!strcmp(str, registr[REG_BX])) {
				fprintf(file_out, "%d", CMD_POP);
				fprintf(file_out, "%c%d",'r', REG_BX);
			} else if (!strcmp(str, registr[REG_CX])) {
				fprintf(file_out, "%d", CMD_POP);
				fprintf(file_out, "%c%d", 'r', REG_CX);
			} else if (!strcmp(str, registr[REG_DX])) {
				fprintf(file_out, "%d", CMD_POP);
				fprintf(file_out, "%c%d", 'r', REG_DX);
			} else if (('0' < str[0])&&(str[0] <= '9')) {
				for (int k = 0; k < strlen(str); k++) {
					if (!(('0' <= str[k])&&(str[k] <= '9'))) {
						fprintf(file_out, "%d", CMD_ARG_ERR);
						return -1;
					};
				}
				fprintf(file_out, "%d", CMD_POP);
				fprintf(file_out, "%s", str);
			} else {
				fprintf(file_out, "%d", CMD_ERR);
				return -1;
			}
		} else if (!strcmp(str, cmds[CMD_OUT])) {
			fprintf(file_out, "%d", CMD_OUT);
		} else if (!strcmp(str, cmds[CMD_ADD])) {
			fprintf(file_out, "%d", CMD_ADD);
		}  else if (!strcmp(str, cmds[CMD_INT])) {
			fprintf(file_out, "%d", CMD_INT);
		} else if (!strcmp(str, cmds[CMD_IN])) {
			fprintf(file_out, "%d", CMD_IN);
		} else {
			fprintf(file_out, "%d", CMD_ERR);
			return -1;
		}
		memset(str, '\0', file_len);
		str[file_len] = '\0';
		i++;
	}
	
	return 0;
}
