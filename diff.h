#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <locale.h>
#include <fstream>

const std::string _NONE = "N";

const std::string node_val[3] = {"0", "1", "-1"};

const std::string _operator[5] = {"+", "-", "*", "/", "^"};

const std::string _function[2] = {"cos", "sin"};

enum NODE_VAL {
	ZERO,
	ONE,
	M_ONE
};

enum _OPERATOR {
	SUM,
	DEC,
	MUL,
	DIV,
	DEG
};

enum _FUNCTION {
	COS,
	SIN
};

enum TYPE {
	CONST = 0,
	VAR, //1
	FUNC, //2
	OPERATOR, //3
	NONE //4
};

class Expression {
public:
	int type;
	std::string value;
	Expression(const Expression& that);
	Expression();
	~Expression();
};

Expression::Expression(const Expression& that) :
	type(that.type),
	value(that.value)
	{}

Expression::Expression() :
	type(-1),
	value()
	{}

Expression::~Expression() {
	
};

class Node {
public:
	Node* root;
	Expression val;
	Node* leftCh;
	Node* rightCh;
	void Print (FILE* file_out);
	void Printf();
	void Read (FILE* file_read);
	void AddLeft (Node* node);
	void AddRight (Node* node);
	Node();
	Node(std::string str, int _type);
	Node(const Node& that);
	~Node();
};

Node::Node () :
	root (NULL),
	val (),
	leftCh (NULL),
	rightCh (NULL)
	{
		
	}

Node::Node (std::string str, int _type) :
	root (NULL),
	val (),
	leftCh (NULL),
	rightCh (NULL)
	{
		val.value = str;
		val.type = _type;
	}

Node::Node(const Node& that) :
	root (that.root),
	val (that.val),
	leftCh (that.leftCh),
	rightCh (that.rightCh)
	{
	}

Node::~Node () {
	
}

std::string _Dec (std::string val) {
	int a = atoi(val.c_str()) - 1;
	char buf [5];
	sprintf(buf, "%d", a);
	std::string str_buf = buf;
	return str_buf;
}

void Node::Print (FILE* file_out) {
	fprintf(file_out, " ( %d %d %s", val.type, val.value.size(), val.value.c_str());
	if (leftCh) leftCh->Print(file_out);
	if (rightCh) rightCh->Print(file_out);
	if ((!rightCh)&&(!leftCh)) fprintf(file_out, " X");
	fprintf(file_out, " )");
}

void Node::Printf () {
	printf(" ( %d %d %s", val.type, val.value.size(), val.value.c_str());
	if (leftCh) leftCh->Printf();
	if (rightCh) rightCh->Printf();
	if ((!rightCh)&&(!leftCh)) printf(" X");
	printf(" )");
}

void Node::Read (FILE* file_read) {
	char buf_[100] = "";
	char sym;
	size_t len = 0;
	fscanf(file_read, " ( %d ", &val.type);
	fscanf(file_read, "%d ", &len);
	size_t i = 0;
	for(i = 0; i < len; i++) {
		fscanf(file_read, "%c", &buf_[i]);
	}
	val.value = buf_;
	fseek(file_read, 1, SEEK_CUR);
	fscanf(file_read, "%c", &sym);
	if (sym == '(') {
		leftCh = new Node ();
		leftCh->root = this;
		fseek(file_read, -1, SEEK_CUR);
		leftCh->Read(file_read);
		rightCh = new Node ();
		rightCh->root = this;
		fseek(file_read, -1, SEEK_CUR);
		rightCh->Read(file_read);
	} else if (sym == 'X') {
		fseek(file_read, 2, SEEK_CUR);
		leftCh = NULL;
		rightCh = NULL;
	}
	fscanf(file_read, " )");
}

void Node::AddLeft (Node* node) {
	if (!leftCh) {
		leftCh = new Node ();
		leftCh = node;
		leftCh->root = this;
	} else {
		leftCh = node;
	}

}

void Node::AddRight (Node* node) {
	if (!rightCh) {
		rightCh = new Node ();
		rightCh = node;
		rightCh->root = this;
	} else {
		rightCh = node;
	}

}

class Diff {
public:
	FILE* file_read;
	FILE* file_out;
	Node root;
	Node* _Diff(Node* root);
	Node* Copy(Node* root);
	Diff();
	~Diff();
};

Diff::Diff () :
	root(),
	file_read(fopen("diff_read.txt", "r")),
	file_out(fopen("diff_out.txt", "w"))
	{}

Diff::~Diff () {
	
}


Node* Diff::Copy (Node* root) {
	if (root) {
	Node* rootCpy = new Node ();
	rootCpy->val.value = root->val.value;
	rootCpy->val.type = root->val.type;
	rootCpy->root = root->root;
	rootCpy->leftCh = Copy(root->leftCh);
	rootCpy->rightCh = Copy(root->rightCh);
	return rootCpy;
	}
}

Node* Diff::_Diff (Node* root) {
	switch(root->val.type) {
		case CONST:
			root->val.value = node_val[ZERO];
			break;
		case VAR:
			root->val.value = node_val[ONE];
			root->val.type = CONST;
			break;
		case FUNC:
			if (!root->val.value.compare(_function[COS])) {
				Node* lftCh = Copy(root->leftCh);
				Node* lftCh1 = Copy(root->leftCh);
				root->val.value = _operator[MUL];
				root->val.type = OPERATOR;
				Node* _node_mul = new Node (_operator[MUL], OPERATOR);
				Node* _node_sin = new Node (_function[SIN], FUNC);
				Node* _node_m_one = new Node (node_val[M_ONE], CONST);
				Node* _node_none = new Node (_NONE, NONE);
				root->AddLeft(_node_mul);
				root->AddRight(_Diff(lftCh));
				root->leftCh->AddLeft(_node_sin);
				root->leftCh->AddRight(_node_m_one);
				root->leftCh->leftCh->AddLeft(lftCh1);
				root->leftCh->leftCh->AddRight(_node_none);
			} else if (!root->val.value.compare(_function[SIN])) {
				
			}
			break;
		case OPERATOR:
			if ((!root->val.value.compare(_operator[SUM]))||(!root->val.value.compare(_operator[DEC]))) {
				_Diff(root->leftCh);
				_Diff(root->rightCh);
			} else if (!root->val.value.compare(_operator[MUL])) {
				root->val.value = _operator[SUM];
				Node* lftCh = Copy(root->leftCh);
				Node* rghtCh = Copy(root->rightCh);
				Node* rghtCh1 =  Copy(root->rightCh);
				Node* lftCh1 = Copy(root->leftCh);
				root->leftCh->val.value = _operator[MUL];
				root->leftCh->val.type = OPERATOR;
				root->rightCh->val.value = _operator[MUL];
				root->rightCh->val.type = OPERATOR;
				root->leftCh->AddLeft(lftCh);
				root->leftCh->AddRight(_Diff(rghtCh));
				root->rightCh->AddRight(rghtCh1);
				root->rightCh->AddLeft(_Diff(lftCh1));
			} else if (!root->val.value.compare(_operator[DIV])) {
				Node* lftCh = Copy(root->leftCh);
				Node* rghtCh = Copy(root->rightCh);
				Node* lftCh1 = Copy(root->leftCh);
				Node* rghtCh1 = Copy(root->rightCh);
				Node* rghtCh2 = Copy(root->rightCh);
				Node* rghtCh3 = Copy(root->rightCh);
				root->leftCh->val.value = _operator[DEC];
				root->leftCh->val.type = OPERATOR;
				root->rightCh->val.value = _operator[MUL];
				root->rightCh->val.type = OPERATOR;
				root->rightCh->AddRight(rghtCh);
				root->rightCh->AddLeft(rghtCh3);
				Node* _Node = new Node (_operator[MUL], OPERATOR);
				root->leftCh->AddLeft(_Node);
				root->leftCh->AddRight(_Node);
				root->leftCh->leftCh->AddLeft(_Diff(lftCh));
				root->leftCh->leftCh->AddRight(rghtCh1);
				root->leftCh->rightCh->AddLeft(_Diff(rghtCh2));
				root->leftCh->rightCh->AddRight(lftCh1);
			} else if (!root->val.value.compare(_operator[DEG])) {
				switch (root->leftCh->val.type) {
					case VAR:
						switch (root->rightCh->val.type) {
							case VAR:
								
								break;
							case CONST: {
								Node* lftCh = Copy(root->leftCh);
								Node* rghtCh = Copy(root->rightCh);
								Node* rghtCh1 =  Copy(root->rightCh);
								Node* lftCh1 = Copy(root->leftCh);
								root->val.value = _operator[MUL];
								root->val.type = OPERATOR;
								Node* _node_deg = new Node (_operator[DEG], OPERATOR);
								Node* _node_dec_val = new Node (_Dec(rghtCh->val.value), CONST);
								Node* _node_mul = new Node (_operator[MUL], OPERATOR);
								root->AddLeft(_node_deg);
								root->leftCh->AddLeft(lftCh);
								root->leftCh->AddRight(_node_dec_val);
								root->AddRight(_node_mul);
								root->rightCh->AddLeft(_Diff(lftCh1));
								root->rightCh->AddRight(rghtCh1);
							}
								break;
						}
						break;
					case CONST:
						switch (root->rightCh->val.type) {
							case VAR:
								
								break;
							case CONST:
								root->val.value = node_val[ZERO];
								root->val.type = CONST;
								root->leftCh = NULL;
								root->rightCh = NULL;
								break;
						}
						break;
				}
			}
			
			break;
	}
	return root;
}