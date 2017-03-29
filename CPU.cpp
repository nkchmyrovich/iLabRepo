class RAM {
public:
	std::string cmd;
	std::string arg;
	std::string flag;
	RAM();
};

RAM::RAM () :
	cmd("n"),
	arg(),
	flag()
	{}

class _Label {
public:
	int cmd_num;
	int label_num;
	_Label();
};

_Label::_Label () :
	cmd_num(-1),
	label_num(-1)
	{}

class CPU {
public:
	int AX, BX, CX, DX;
	Stack _StackFunc;
	Stack _Stack;
	int CurrCmd;
	std::vector<RAM> _RAM;
	std::vector<_Label> LabelVec;
	void Read_file();
	void Main_loop();
	CPU (size_t size);
	~CPU ();
};

CPU::CPU (size_t size) :
	_RAM(size),
	LabelVec(size),
	_Stack(size),
	_StackFunc(size)
	{}

CPU::~CPU() {
	
}

void RAM_Dump (std::vector<RAM> _RAM) {
	for (size_t i = 0; i < _RAM.size(); i++) {
		if (_RAM[i].cmd == "n") { break;}
		printf("cmd:%s arg:%s l:%s \n", _RAM[i].cmd.c_str(), _RAM[i].arg.c_str(), _RAM[i].flag.c_str());
	}
}

bool flag_cmp (std::string str, char flag) {
	bool equal = false;
	if (str[0] == flag) equal = true;
	if (str.size() != 1) equal = false;
	return equal;
}

void _Label_Dump(std::vector<_Label> LabelVec) {
	for (size_t i = 0; i < LabelVec.size(); i++) {
		if (LabelVec[i].label_num == -1) { break;}
		printf("Label_num:%d cmd_num:%d \n", LabelVec[i].label_num, LabelVec[i].cmd_num);
	}
}

void CPU::Read_file () {
	FILE* file = fopen("output.txt", "r");
  	 if (file == NULL) {
		printf ("Error"); 
	}
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	std::vector<char> fileVec (fileSize + 1);
	memset(fileVec.data(), '\0', fileSize);
	fread(fileVec.data(), sizeof(char), fileSize, file);
	bool ok = true;
	for (size_t i = 0; i < fileVec.size(); i++) {
		if (!(isalpha(fileVec[i])) || (isdigit(fileVec[i]))) {
			ok = false;
		}
	}
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
	int label_count = 1;
	if (Arg_Ok(vec_tok[0].str)) {
		size_t j = 0;
		for (i = 1; vec_tok[i].line <= atoi(vec_tok[0].str.c_str()) + 1 ; i += 3) {
			_RAM[j].cmd = vec_tok[i].str;
			_RAM[j].arg = vec_tok[i+1].str;
			_RAM[j].flag = vec_tok[i+2].str;
			j++;
			label_count += 3;
		}
	}
	RAM_Dump(_RAM);
	if (Arg_Ok(vec_tok[label_count].str)) {
		size_t j = 0;
		for (i = label_count + 1; i <= label_count + atoi(vec_tok[label_count].str.c_str()); i++) {
			LabelVec[j].label_num = j;
			LabelVec[j].cmd_num = atoi(vec_tok[i].str.c_str());
			j++;
		}
	}
	_Label_Dump(LabelVec);
}

void CPU::Main_loop() {
	CurrCmd = 0;
	size_t cmd_num;
	int a, b;
	for (size_t i = 0; i < _RAM.size(); i++) {
		if (Arg_Ok(_RAM[CurrCmd].cmd)) {
			cmd_num = atoi(_RAM[CurrCmd].cmd.c_str());
			switch (cmd_num) {
				case CMD_ADD :
					if (Arg_Ok(_RAM[CurrCmd].arg)) {
						if (flag_cmp(_RAM[CurrCmd].flag, _flag[FREE])) {
							
						} else {
							
						}
					} else {
						
					}
					break;
				case CMD_DEC :
					if (Arg_Ok(_RAM[CurrCmd].arg)) {
						if (flag_cmp(_RAM[CurrCmd].flag, _flag[FREE])) {
							a = _Stack.Pop();
							a -= 1;
							_Stack.Push(a);
						} else {
							
						}
					} else {
						
					}
					break;
				case CMD_DIV :
					if (Arg_Ok(_RAM[CurrCmd].arg)) {
						if (_RAM[CurrCmd].flag[0] == _flag[FREE]) {
							a = _Stack.Pop();
							b = _Stack.Pop();
							_Stack.Push(a/b);
						} else {
							
						}
					} else {
						
					}
					break;
				case CMD_INT :
					
				case CMD_IN :
					if (Arg_Ok(_RAM[CurrCmd].arg)) {
						if (flag_cmp(_RAM[CurrCmd].flag, _flag[FREE])) {
							scanf("Enter value:%d", &a);
							_Stack.Push(a);
						} else {
							
						}
					} else {
						
					}
				case CMD_JMP :
					if (Arg_Ok(_RAM[CurrCmd].arg)) {
						if (flag_cmp(_RAM[CurrCmd].flag, _flag[LABEL])) {
							a = LabelVec[atoi(_RAM[CurrCmd].arg.c_str())].cmd_num;
							CurrCmd = a - 1;
						} else {
							
						}
					} else {
						
					}
					break;
				case CMD_MUL :
					if (Arg_Ok(_RAM[CurrCmd].arg)) {
						if (flag_cmp(_RAM[CurrCmd].flag, _flag[FREE])) {
							a = _Stack.Pop();
							b = _Stack.Pop();
							_Stack.Push(a*b);
						} else {
							
						}
					} else {
						
					}
					break;
				case CMD_OUT :
					if (Arg_Ok(_RAM[CurrCmd].arg)) {
						if (flag_cmp(_RAM[CurrCmd].flag, _flag[FREE])) {
							a = _Stack.Pop();
							printf("%d", a);
							_Stack.Push(a);
						} else {
							
						}
					} else {
						
					}
				case CMD_POP :
					
				case CMD_PUSH :
					if (flag_cmp(_RAM[CurrCmd].flag, _flag[ARGUMENT])) {
						_Stack.Push(atoi(_RAM[CurrCmd].arg.c_str()));
					} else if (flag_cmp(_RAM[CurrCmd].flag, _flag[REG])) {
						switch (atoi(_RAM[CurrCmd].arg.c_str())) {
							case REG_AX :
								_Stack.Push(AX);
								break;
							case REG_BX :
								_Stack.Push(BX);
								break;
							case REG_CX :
								_Stack.Push(CX);
								break;
							case REG_DX :
								_Stack.Push(DX);
								break;
						}
					}
					break;
				case CMD_JA :
					a = _Stack.Pop();
					b = _Stack.Pop();
					_Stack.Push(b);
					_Stack.Push(a);
					if (a > b) {
						CurrCmd = LabelVec[_RAM[CurrCmd].arg.c_str()].cmd_num - 1;
					}
					break;
				case CMD_JE :
					a = _Stack.Pop();
					b = _Stack.Pop();
					_Stack.Push(b);
					_Stack.Push(a);
					if (a == b) {
						CurrCmd = LabelVec[_RAM[CurrCmd].arg.c_str()].cmd_num - 1;
					}
					break;
				case CMD_RET :
					CurrCmd = _StackFunc.Pop();
					break;
				case CMD_CALL :
					_StackFunc.Push(CurrCmd + 1);
					break 
				
			}
			CurrCmd++;
		}
	}
	_Stack.Dump();
}

