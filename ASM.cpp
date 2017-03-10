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


class Command {
public:
	size_t cmd;
	size_t arg;
	char flag;
	Command();
};

Command::Command () :
	cmd(0),
	arg(0),
	flag('\0')
{}

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

void Token_Dump(std::vector<Token> vec_tok) {
	size_t i;
	for (i = 0; i < vec_tok.size(); i++) {
		printf("token:%s         line:%d    token number:%d    \n", vec_tok[i].str.c_str(), vec_tok[i].line, vec_tok[i].tok_num);
	}
}


class Label {
public:
	size_t label_name;
	size_t cmd_num;
	Label(size_t start);
};

Label::Label (size_t start) :
	label_name(0),
	cmd_num(0)
	{}

int Label_name(char* str) {
	size_t i = 0;
	bool label_true = true;
	for (i = 0; i < (sizeof(str)/sizeof(char))-1; i++) {
		if ((str[i] <= '9')&&(str[i] >= '0')) {
			label_true = true;
		} else {
			label_true = false;
			break;
		}
	}
	if (!label_true) {
		return atoi(str);
	} else {
		return -1;
	}
}

class Commands {
public:
	size_t cmd;
	int arg;
	char flag;
	Commands();
};

Commands::Commands() :
	cmd(0),
	arg(-1),
	flag('\0')
	{}

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
			if (buf[i-1] == ':'){
				vec_label[label_num].cmd_num = _tok_num;
				vec_label[label_num].label_name = Label_name(buf);
				label_num++;
			} else {
			_token.str = buf;
			_token.line = line;
			_token.tok_num = _tok_num;
			vec_tok[k] = _token;
			memset(buf, '\0', fileSize + 2);
			k++;
			}
		}
		if (fileVec[l] == '\n') {
			line++;
		}
		l++;
	}
	return vec_tok;
}

void asm_compiler () {
	const char cmds[20][20] = {"push", "pop", "out", "add", "int", "in"};
 	const char registr[10][10] = {"ax", "bx", "cx", "dx"};
	Label _label(0);
	std::vector<Label> vec_label(0, 0); 
	std::vector<Token> vec_tok = read_file(vec_label);
	std::vector<Command> vec_cmd(vec_tok.size() + vec_label.size());
	
} 