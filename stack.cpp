#include <stdio.h>
#include <stdlib.h>

#define Stack_Capasity 10000

class Stack {
private:
	double data [Stack_Capasity];
	int size;
public:
	void Push (double value);
	double Pop ();
	void Empty ();
	int Size ();
	int Capacity ();
	Size_Const () {
		size = 0;
	};
};

void Stack::Push (double value) {
	data[size++] = value;
};

double Stack::Pop () {
	double a = data[--size];
	return a;
}

void Stack::Empty () {
	while (size) {
		data[size--] = 0;
	}
}

int Stack::Size () {
	return size;
}

int Stack::Capacity () {
	return (int)(sizeof(data)/8);
}

Stack::Stack (size_t size) :

int main(int argc, char **argv)
{
	Stack s;
	s.Push (1);
	s.Push (2);
	s.Push (3234);
	printf("%g    ", s.Pop());
	printf("%d   ", s.Size());
	s.Empty();
	printf("%d     ", s.Size());
	printf("%d", s.Capacity());
	return 0;
}