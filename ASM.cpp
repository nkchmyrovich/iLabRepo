#include <vector>
#include <string.h>
#include <string>
#include <algoritHm>

const std::string cmds[14]= {"push", "pop", "out", "add", "int", "in", "jmp", "mul", "dec", "div", "ja", "je", "call", "ret"};
const std::string registr[4]= {"ax", "bx", "cx", "dx"};
const char _flag[10] = {'L', 'C', 'A', 'R', 'E', 'F'};

enum CMD {
	CMD_PUSH = 0,
	CMD_POP,
	CMD_OUT,
	CMD_ADD,
	CMD_INT,
	CMD_IN,
	CMD_JMP,
	CMD_MUL,
	CMD_DEC,
	CMD_DIV,
	CMD_JA,
	CMD_JE,
	CMD_CALL,
	CMD_RET
};

enum REGISTR {
	REG_AX = 0,
	REG_BX,
	REG_CX,
	REG_DX
};

enum FLAG {
	LABEL = 0,
	COMMAND,
	ARGUMENT,
	REG,
	ERR,
	FREE
};

class CommandFinal {
public:
	int cmd;
	int arg;
	int line;
	char flag_arg;
	CommandFinal();
};

CommandFinal::CommandFinal() :
	cmd(-1),
	arg(-1),
	line(-1),
	flag_arg('\0')
	{}

void CommandFinal_Dump (std::vector<CommandFinal>& vec_cmd_f) {
	for(size_t i = 0; i < vec_cmd_f.size(); i++) {
		printf ("Command:%d Arg:%d Flag_arg:%c \n", vec_cmd_f[i].cmd, vec_cmd_f[i].arg, vec_cmd_f[i].flag_arg);
	}
}


class Command {
public:
	int cmd;
	size_t line;
	size_t tok_num;
	std::string label_name;
	char flag;
	int cmd_num;
	Command();
};

Command::Command () :
	label_name(),
	cmd(-1),
	line(0),
	tok_num(0),
	flag('\0'),
	cmd_num(-1)
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
	str (),
	line (-1), 
	tok_num(0)
	{}


void Token_Dump (std::vector<Token>& vec_tok) {
	size_t i;
	for (i = 0; i < vec_tok.size(); i++) {
		printf("token:%s         line:%d    token number:%d    \n", vec_tok[i].str.c_str(), vec_tok[i].line, vec_tok[i].tok_num);
	}
}

void Command_Dump (std::vector<Command>& vec_cmd) {
	size_t i = 0;
	for (i = 0; i < vec_cmd.size(); i++) {
		if (vec_cmd[i].tok_num == 0) { break;}
		printf("command:%d     line:%d   token num:%d   flag:%c  labelname:%s\n", vec_cmd[i].cmd, vec_cmd[i].line, vec_cmd[i].tok_num, vec_cmd[i].flag, vec_cmd[i].label_name.c_str());
	}
}

class Label {
public:
	std::string label_name;
	size_t cmd_num;
	int line;
	Label(size_t start);
};

Label::Label (size_t start) :
	label_name (),
	cmd_num(0),
	line(-1)
	{}


class ASM {
public:
	size_t label_num;
	std::vector<char> fileVec;
	std::vector<Token> vec_tok;
	std::vector<Label> vec_label;
	std::vector<Command> vec_cmd;
	std::vector<CommandFinal> vec_cmd_f;
	void Read_file();
	void Compiling();
	void Semantic_analysis();
	void File_out();
	void Run();
	ASM(size_t size);
	~ASM();
};

ASM::ASM (size_t size) :
	fileVec(size),
	vec_tok(size, size),
	vec_label(size, 0),
	vec_cmd(size),
	vec_cmd_f(size),
	label_num(0)
	{}

ASM::~ASM () {
	
}
void Label_Dump (std::vector<Label>& vec_label) {
	for (size_t i = 0; i < vec_label.size() + 1; i++) {
		if (vec_label[i].line == -1) { break;}
		printf("label name:%s cmd_num:%d label_num:%d \n", vec_label[i].label_name.c_str(), vec_label[i].cmd_num, i);
	}
}

bool Arg_Ok (std::string str) {
	size_t offset = 0;
	if(str[offset] == '-')
		++offset;
	return str.find_first_not_of("0123456789", offset) == std::string::npos;
}

size_t Label_name (std::string str, std::vector<Label>& vec_label) {
	bool ok = false;
	for (size_t i = 0; i < vec_label.size(); i++) {
		if (!str.compare(vec_label[i].label_name)) {
			return i;
			ok = true;
		}
	}
	if(!ok) {
		return -1;
	}
}

std::string _label_name (std::string str) {
	str.erase(str.size()-1, 1);
	return str;
}

std::string _label_name_1 (std::string str) {
	str.erase(0, 1);
	return str;
}

void free_agr_cmd (std::vector<CommandFinal>& vec_cmd_f, std::vector<Command>& vec_cmd, size_t* i, size_t* j);

void cmd_compilation (std::vector<Command>& vec_cmd, std::vector<Token>& vec_tok, size_t* i, size_t* j);

void  jmp_func (std::vector<CommandFinal>& vec_cmd_f, std::vector<Command>& vec_cmd, std::vector<Label>& vec_label, size_t& i, size_t& j);

void ASM::Read_file() {
	FILE* file = fopen("prog.txt", "r");
  	 if (file == NULL) {
		printf ("Error"); 
	}
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	std::vector<char> fileVec (fileSize + 1, '\0');
	fread(fileVec.data(), sizeof(char), fileSize, file);
	std::vector<char>::iterator iter1 = fileVec.begin();
	std::vector<char>::iterator iter2 = fileVec.begin();
	while(iter1 != fileVec.end()) {
		iter1 = std::find(iter1, fileVec.end(), ';');
		iter2 = std::find(iter1, fileVec.end(), '\n');
		fileVec.erase(iter1, iter2);
	};
	std::string str();
	char buf[fileSize] = {};
	size_t i = 0;
	size_t l = 0;
	size_t k = 0;
	size_t _tok_num = 0;
	size_t line = 1;
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
			if (buf[i-1] == ':') {
				vec_label[label_num].cmd_num = _tok_num;
				vec_label[label_num].label_name = _label_name((std::string)buf);
				vec_label[label_num].line = line;
				label_num++;
				memset(buf, '\0', fileSize + 2);
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
	fclose(file);
	/*Token_Dump(vec_tok);*/
}

#define Command_Compilation 
cmd_compilation(vec_cmd, vec_tok, &i, &j);\
vec_cmd[j].cmd_num = cmd_num;\
cmd_num++;

#define Reg_Compilation 
vec_cmd[j].flag = _flag[REG];\
vec_cmd[j].tok_num = vec_tok[i].tok_num;\
vec_cmd[j].line = vec_tok[i].line;

void ASM::Compiling() {
	size_t i = 0;
	std::string _str;
	size_t j = 0;
	int cmd_num = 0;
	while(j < vec_tok.size()) {
		if (vec_tok[i].line == -1) { break;};
		_str = vec_tok[i].str;
		if (!_str.compare(cmds[CMD_PUSH])) {
			vec_cmd[j].cmd = CMD_PUSH;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_POP])) {
			vec_cmd[j].cmd = CMD_POP;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_DEC])) {
			vec_cmd[j].cmd = CMD_DEC;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_DIV])) {
			vec_cmd[j].cmd = CMD_DIV;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_MUL])) {
			vec_cmd[j].cmd = CMD_MUL;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_OUT])) {
			vec_cmd[j].cmd = CMD_OUT;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_INT])) {
			vec_cmd[j].cmd = CMD_INT;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_IN])) {
			vec_cmd[j].cmd = CMD_IN;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_ADD])) {
			vec_cmd[j].cmd = CMD_ADD;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_JA])) {
			vec_cmd[j].cmd = CMD_JA;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_JE])) {
			vec_cmd[j].cmd = CMD_JE;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_CALL])) {
			vec_cmd[j].cmd = CMD_CALL;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_RET])) {
			vec_cmd[j].cmd = CMD_RET;
			Command_Compilation;
		} else if (!_str.compare(cmds[CMD_JMP])) { 
			vec_cmd[j].cmd = CMD_JMP;
			Command_Compilation;
		} else if (!_str.compare(registr[REG_AX])) {
			vec_cmd[j].cmd = REG_AX;
			Reg_Compilation;
		} else if (!_str.compare(registr[REG_BX])) {
			vec_cmd[j].cmd = REG_BX;
			Reg_Compilation;
		} else if (!_str.compare(registr[REG_CX])) {
			vec_cmd[j].cmd = REG_CX;
			Reg_Compilation;
		} else if (!_str.compare(registr[REG_DX])) {
			vec_cmd[j].cmd = REG_DX;
			Reg_Compilation;
		} else if (Arg_Ok(_str)) {
			vec_cmd[j].flag = _flag[ARGUMENT];
			vec_cmd[j].cmd = atoi(_str.c_str());
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		} else if (_str[0] == ':') { 
			vec_cmd[j].flag = _flag[LABEL];
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
			vec_cmd[j].label_name = _label_name_1(_str);
		} else {
			vec_cmd[j].flag = _flag[ERR];
			vec_cmd[j].tok_num = vec_tok[i].tok_num;
			vec_cmd[j].line = vec_tok[i].line;
		}
		j++;
		i++;
	}
	/*std::sort (vec_cmd.begin(), vec_cmd.end(), CustomSort_line());*/
	Command_Dump(vec_cmd);
}

#undef Command_Compilation
#undef Reg_Compilation

#define Err_Semantic 
vec_cmd_f[j].flag_arg = _flag[ERR];\
vec_cmd_f[j].line = vec_cmd[i].line;\
j++;

void ASM::Semantic_analysis () {
	size_t j = 0;
	for (size_t i = 0; i < vec_cmd.size(); i++) {
		if (vec_cmd[i].tok_num) {
			if(vec_cmd[i].flag == _flag[COMMAND]) {
					switch (vec_cmd[i].cmd) {
						case CMD_ADD :
							free_agr_cmd (vec_cmd_f, vec_cmd, &i, &j);
							j++;
							break;
						case CMD_DEC :
							free_agr_cmd (vec_cmd_f, vec_cmd, &i, &j);
							j++;
							break;
						case CMD_IN :
							free_agr_cmd (vec_cmd_f, vec_cmd, &i, &j);
							j++;
							break;
						case CMD_MUL :
							free_agr_cmd (vec_cmd_f, vec_cmd, &i, &j);
							j++;
							break;
						case CMD_DIV :
							free_agr_cmd (vec_cmd_f, vec_cmd, &i, &j);
							j++;
							break;
						case CMD_INT :
							free_agr_cmd (vec_cmd_f, vec_cmd, &i, &j);
							j++;
						case CMD_JMP : 
							jmp_func(vec_cmd_f, vec_cmd, vec_label, i, j);
							break;
						case CMD_OUT :
							free_agr_cmd (vec_cmd_f, vec_cmd, &i, &j);
							j++;
							break;
						case CMD_POP : 
							if (vec_cmd[i+1].flag == _flag[REG]) {
								vec_cmd_f[j].cmd = vec_cmd[i].cmd;
								vec_cmd_f[j].arg = vec_cmd[i+1].cmd;
								vec_cmd_f[j].line = vec_cmd[i].line;
								vec_cmd_f[j].flag_arg = vec_cmd[i+1].flag;
								i++;
								j++;
							} else if (vec_cmd[i+1].flag == _flag[COMMAND]) {
								free_agr_cmd(vec_cmd_f, vec_cmd, &i, &j);
								j++;
							} else {
								Err_Semantic;
							}
							break;
						case CMD_PUSH :
							if (vec_cmd[i+1].flag == _flag[ARGUMENT]) {
								vec_cmd_f[j].cmd = vec_cmd[i].cmd;
								vec_cmd_f[j].arg = vec_cmd[i+1].cmd;
								vec_cmd_f[j].line = vec_cmd[i].line;
								vec_cmd_f[j].flag_arg = vec_cmd[i+1].flag;
								i++;
								j++;
							} else if (vec_cmd[i+1].flag == _flag[REG]) {
								vec_cmd_f[j].cmd = vec_cmd[i].cmd;
								vec_cmd_f[j].arg = vec_cmd[i+1].cmd;
								vec_cmd_f[j].line = vec_cmd[i].line;
								vec_cmd_f[j].flag_arg = vec_cmd[i+1].flag;
								i++;
								j++;
							} else {
								Err_Semantic;
							}
							break;
						case CMD_JA :
							jmp_func(vec_cmd_f, vec_cmd, vec_label, i, j);
							break;
						case CMD_JE :
							jmp_func(vec_cmd_f, vec_cmd, vec_label, i, j);
							break;
						case CMD_CALL :
							free_agr_cmd (vec_cmd_f, vec_cmd, &i, &j);
							j++;
							break;
						case CMD_RET :
							free_agr_cmd (vec_cmd_f, vec_cmd, &i, &j);
							j++;
							break;
					}
			} else if (vec_cmd[i].flag == _flag[REG]) {
					Err_Semantic;
			} else if (vec_cmd[i].flag == _flag[LABEL]) {
					Err_Semantic;
			} else if (vec_cmd[i].flag == _flag[ERR]) {
					Err_Semantic;
			}else if (vec_cmd[i].flag == _flag[ARGUMENT]) {
					Err_Semantic;
			} else { 
					Err_Semantic;
			}
		} 
	}
	/*CommandFinal_Dump(vec_cmd_f);
	Label_Dump(vec_label);*/
}

void ASM::File_out() {
	FILE* file_out = fopen("output.txt", "w");
	size_t cmd_size = 0;
	bool not_err = true;
	for (size_t i = 0; i < vec_cmd_f.size(); i++) {
		if (vec_cmd_f[i].line == -1) { break;}
		if (vec_cmd_f[i].flag_arg == _flag[ERR]) {
			not_err = false;
			fprintf(file_out, "%c %d \n", vec_cmd_f[i].flag_arg, vec_cmd_f[i].line);
		}
		cmd_size++;
	}
	if (not_err) {
		fprintf(file_out, "%d \n",cmd_size);
		for (size_t i = 0; i < vec_cmd_f.size(); i++) {
			if (vec_cmd_f[i].line == -1) { break;}
			fprintf(file_out, "%d %d %c \n", vec_cmd_f[i].cmd, vec_cmd_f[i].arg, vec_cmd_f[i].flag_arg);
		}
		fprintf(file_out, "%d \n", label_num);
		if (vec_label.size()) {
			for(size_t i = 0; i < vec_label.size(); i++) {
				if (vec_label[i].line == -1) { break;}
				fprintf(file_out, "%d ", vec_cmd[vec_label[i].cmd_num + 1].cmd_num - 1);
			}
		}
	}
	fclose(file_out);
}

void ASM::Run () {
	Read_file();
	Compiling();
	Semantic_analysis();
	File_out();
}

void free_agr_cmd (std::vector<CommandFinal>& vec_cmd_f, std::vector<Command>& vec_cmd, size_t* i, size_t* j) {
	if ((vec_cmd[*i+1].flag == _flag[ARGUMENT])||(vec_cmd[*i+1].flag == _flag[REG])) {
		vec_cmd_f[*j].flag_arg = _flag[ERR];
		vec_cmd_f[*j].line = vec_cmd[*i].line;
	} else {
		vec_cmd_f[*j].cmd = vec_cmd[*i].cmd;
		vec_cmd_f[*j].arg = 0;
		vec_cmd_f[*j].flag_arg = _flag[FREE];
		vec_cmd_f[*j].line = vec_cmd[*i].line;
	}
}

void cmd_compilation (std::vector<Command>& vec_cmd, std::vector<Token>& vec_tok, size_t* i, size_t* j) {
	vec_cmd[*j].flag = _flag[COMMAND];
	vec_cmd[*j].tok_num = vec_tok[*i].tok_num;
	vec_cmd[*j].line = vec_tok[*i].line;
}

void jmp_func (std::vector<CommandFinal>& vec_cmd_f,std::vector<Command>& vec_cmd, std::vector<Label>& vec_label, size_t& i, size_t& j) {
	if (vec_cmd[i+1].flag == _flag[LABEL]) {
		if (Label_name (vec_cmd[i+1].label_name, vec_label) == -1) {
			vec_cmd_f[j].flag_arg = _flag[ERR];
			vec_cmd_f[j].line = vec_cmd[i].line;
		} else {
			vec_cmd_f[j].cmd = vec_cmd[i].cmd;
			vec_cmd_f[j].arg = Label_name (vec_cmd[i+1].label_name, vec_label);
			vec_cmd_f[j].line = vec_cmd[i].line;
			vec_cmd_f[j].flag_arg = vec_cmd[i+1].flag;
			i++;
			j++;
		}
	} else {
		vec_cmd_f[j].flag_arg = _flag[ERR];
		vec_cmd_f[j].line = vec_cmd[i].line;
	}
} 