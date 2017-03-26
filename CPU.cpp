class RAM {
public:
	std::string cmd;
	std::string arg;
	std::string flag;
	RAM();
};

RAM::RAM () :
	cmd(),
	arg(),
	flag()
	{}

void RAM_Dump (std::vector<RAM> _RAM) {
	for (size_t i = 0; i < _RAM.size(); i++) {
		printf("cmd:%s arg:%s l:%s \n", _RAM[i].cmd, _RAM[i].arg[0], _RAM[i].arg[1]);
	}
}

class _Label {
public:
	int label_name;
	int label_num;
	_Label();
};

_Label::_Label () :
	label_name(-1),
	label_num(-1)
	{}

/*class CPU {
public:
	Stack _StackFunc;
	Stack _Stack;
	int CurrCmd;
	std::vector<RAM> _RAM;
};*/

std::vector<RAM> Read_file () {
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
	std::vector<RAM> _RAM(fileSize);
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
	if (Arg_Ok(vec_tok[0].str)) {
		size_t j = 0;
		for (i = 1; vec_tok[i].line < atoi(vec_tok[0].str.c_str()); i += 3) {
			_RAM[j].cmd = vec_tok[i].str;
			_RAM[j].arg = vec_tok[i+1].str;
			_RAM[j].flag = vec_tok[i+2].str;
			j++;
		}
	}
	RAM_Dump(_RAM);
	return _RAM;
}