#include <string>
#include <algorithm>
#include <vector>
#include <string.h>

enum CMD {
	CMD_PUSH,
	CMD_POP,
	CMD_OUT,
	CMD_ADD,
	CMD_INT,
	CMD_IN
};

enum REGISTR {
	REG_AX,
	REG_BX,
	REG_CX,
	REG_DX
};

enum FLAG {
	LABEL,
	COMMAND,
	ARGUMENT,
	REG,
	ERR
};

class Command {
public:
	size_t cmd;
	size_t arg;
	size_t line;
	size_t tok_num;
	char flag;
	Command();
};

Command::Command () :
	cmd(0),
	arg(0),
	line(0),
	tok_num(0),
	flag('\0')
{}

class CustomSort_line {
public:
	bool operator () (const Command &c1, const Command &c2)
	{
		return (c1.tok_num < c2.tok_num);
	}
};

class Token {
public:
	std::string str;
	int line;
	size_t tok_num;
	Token(size_t size);
};

Token::Token (size_t size) : 
	str (new char [size]),
	line (-1), 
	tok_num(0)
	{}

void Token_Dump (std::vector<Token> vec_tok) {
	size_t i;
	for (i = 0; i < vec_tok.size(); i++) {
		printf("token:%s         line:%d    token number:%d    \n", vec_tok[i].str.c_str(), vec_tok[i].line, vec_tok[i].tok_num);
	}
}

void Command_Dump (std::vector<Command> vec_cmd) {
	size_t i = 0;
	for (i = 0; i < vec_cmd.size(); i++) {
		printf("command:%d   arg:%d  line:%d   token num:%d   flag:%c  \n", vec_cmd[i].cmd, vec_cmd[i].arg, vec_cmd[i].line, vec_cmd[i].tok_num, vec_cmd[i].flag);
	}
}

class Label {
public:
	size_t label_name;
	size_t cmd_num;
	int line;
	Label(size_t start);
};

Label::Label (size_t start) :
	label_name(0),
	cmd_num(0),
	line(-1)
	{}

int Label_name(char* str) {
	size_t i = 0;
	bool label_true = true;
	for (i = 0; i < (sizeof(str)/sizeof(char)); i++) {
		if (isdigit(str[i])) {
			label_true = true;
		} else {
			label_true = false;
			break;
		}
	}
	if (label_true) {
		return atoi(str);
	} else {
		return -1;
	}
}

bool Arg_Ok (std::string str) {
	size_t offset = 0;
	if(str[offset] == '-')
		++offset;
	return str.find_first_not_of("0123456789", offset) == std::string::npos;
}

std::vector<Token> read_file (std::vector<Label> vec_label) {
	FILE* file = fopen("prog.txt", "r");
	FILE* file_out = fopen("output.txt", "w");
  	 if (file == NULL) {
		printf ("Error"); 
	}
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	std::vector<char> fileVec (fileSize + 1);
	memset(fileVec.data(), '\0', fileSize);
	fread(fileVec.data(), sizeof(char), fileSize, file);
	std::vector<char>::iterator iter1 = fileVec.begin();
	std::vector<char>::iterator iter2 = fileVec.begin();
	while(iter1 != fileVec.end()) {
		iter1 = std::find(iter1, fileVec.end(), ';');
		iter2 = std::find(iter1, fileVec.end(), '\n');
		fileVec.erase(iter1, iter2);
		
	};
	vec_label.reserve(fileSize);
	std::vector<Token> vec_tok(fileSize, Token(fileSize));
	std::string str();
	char buf[fileSize];
	memset(buf, '\0', fileSize + 2);
	size_t i = 0;
	size_t l = 0;
	size_t k = 0;
	size_t _tok_num = 0;
	size_t line = 1;
	size_t label_num = 0;
	Token _token(fileSize);
	while (fileVec[l] != '\0') {
		i = 0;
		if ((fileVec[l] != ' ')&&(fileVec[l] != '\0')&&(fileVec[l] != '\n')) {
			while ((fileVec[l] != ' ')&&(fileVec[l] != '\0')&&(fileVec[l] != '\n')) {
				buf[i] = fileVec[l];
				i++;
				l++;
			}
			_tok_num++;
			if ((buf[i-1] == ':')||(buf[0] == ':')) {
				vec_label[label_num].cmd_num = _tok_num;
				vec_label[label_num].label_name = Label_name(buf);
				vec_label[label_num].line = line;
				label_num++;
			} 
			_token.str = buf;
			_token.line = line;
			_token.tok_num = _tok_num;
			vec_tok[k] = _token;
			memset(buf, '\0', fileSize + 2);
			k++;
			
		}
		if (fileVec[l] == '\n') {
			line++;
		}
		l++;
	}
	/*Token_Dump(vec_tok);
	printf ("%d  ", vec_label[1].label_name);*/
	return vec_tok;
}

void asm_compiler () {
	const std::string cmds[6]= {"push", "pop", "out", "add", "int", "in"};
 	const std::string registr[4]= {"ax", "bx", "cx", "dx"};
	const char _flag[10] = {'L', 'C', 'A', 'R', 'E'};
	Label _label(0);
	std::vector<Label> vec_label(12, 0); 
	std::vector<Token> vec_tok = read_file(vec_label);
	std::vector<Command> vec_cmd(vec_tok.size() + vec_label.size());
	size_t i = 0;
	size_t k = 0;
	for (i = 0; i < vec_label.size(); i++) {
		vec_cmd[i].flag = _flag[LABEL];
		vec_cmd[i].cmd = vec_label[i].label_name;
		vec_cmd[i].tok_num = vec_label[i].cmd_num;
		vec_cmd[i].line = vec_label[i].line;
		k = i;
	}
	i = k;
	std::string str;
	size_t j = k + 1;
	i = 0;
	while(j < vec_cmd.size()) {
		str = vec_tok[i].str;
		if (!str.compare(cmds[CMD_PUSH])) {
			vec_cmd[j].flag = _flag[COMMAND];
			vec_cmd[j].cmd = CMD_PUSH;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (!str.compare(cmds[CMD_POP])) {
			vec_cmd[j].flag = _flag[COMMAND];
			vec_cmd[j].cmd = CMD_POP;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (!str.compare(cmds[CMD_OUT])) {
			vec_cmd[j].flag = _flag[COMMAND];
			vec_cmd[j].cmd = CMD_OUT;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (!str.compare(cmds[CMD_INT])) {
			vec_cmd[j].flag = _flag[COMMAND];
			vec_cmd[j].cmd = CMD_INT;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (!str.compare(cmds[CMD_IN])) {
			vec_cmd[j].flag = _flag[COMMAND];
			vec_cmd[j].cmd = CMD_INT;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (!str.compare(cmds[CMD_ADD])) {
			vec_cmd[j].flag = _flag[COMMAND];
			vec_cmd[j].cmd = CMD_ADD;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (!str.compare(registr[REG_AX])) {
			vec_cmd[j].flag = _flag[REG];
			vec_cmd[j].cmd = REG_AX;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (!str.compare(registr[REG_BX])) {
			vec_cmd[j].flag = _flag[REG];
			vec_cmd[j].cmd = REG_BX;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (!str.compare(registr[REG_CX])) {
			vec_cmd[j].flag = _flag[REG];
			vec_cmd[j].cmd = REG_CX;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (!str.compare(registr[REG_DX])) {
			vec_cmd[j].flag = _flag[REG];
			vec_cmd[j].cmd = REG_DX;
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (Arg_Ok(str)) {
			vec_cmd[j].flag = _flag[ARGUMENT];
			vec_cmd[j].cmd = std::stoi(str);
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if ((str[str.size() - 1] = ':')||(str[0] = ':')) { 
			vec_cmd[j].flag = _flag[LABEL];
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else {
			vec_cmd[j].flag = _flag[ERR];
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		}
		j++;
		i++;
	}
	std::sort (vec_cmd.begin(), vec_cmd.end(), CustomSort_line());
	Command_Dump(vec_cmd);
	printf("%d", k);
} 