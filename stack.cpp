#include <stdio.h>
#include <stdlib.h>

class Stack {
private:
	double * data_;
	size_t size_;
	size_t capacity_;
public:
	void Push (double value);
	double Pop ();
	bool Empty ();
	void Clear();
	int Size ();
	int Capacity ();
	bool Ok();
	void Dump ();
	Stack(size_t size);
};

Stack::Stack (size_t size) :
	data_ (new double [size]),
	capacity_ (size),
	size_ (0)
	{}

void Stack::Push (double value) {
	data_[size_++] = value;
};

double Stack::Pop () {
	double a = data_[--size_];
	return a;
}

bool Stack::Empty () {
	return (bool)(!size_);
} 

void Stack::Clear () {
	if (!Empty()) {
		while (size_) {
			data_[--size_] = 0;
		}
	}
}

int Stack::Size () {
	return size_;
}

int Stack::Capacity () {
	return capacity_;
}

bool Stack::Ok () {
	return (((data_&&capacity_&&size_<capacity_)||(!data_&&!capacity_&&!size_))&&this);
}

void Stack::Dump () {
	if (Ok()) {
		if (data_) {
			if (size_) {
				printf("Capacity: %d \n", capacity_);
				printf("Size: %d \n", size_);
				printf("Stack data: \n");
				while (size_--) {
					printf("	%g \n", data_[size_]);
				}
			} else {
				printf("Empty stack");
			}
		} else {
			printf("No data");
		}
	} else {
		printf("Error");
	}
}

int main()
{
	Stack s(1000);
	s.Push (1);
	s.Push (2);
	s.Push (323);
	s.Clear();
	s.Dump();
	return 0;
}