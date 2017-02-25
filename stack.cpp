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
	if (Ok()) {
		if (value) {
			data_[size_++] = value;
		} else {
			printf("Wrong value\n");
		}
	} else {
		Dump();
	}
}

double Stack::Pop () {
	if (Ok()) {
		if (size_) {
			double a = data_[--size_];
			return a;
		} else {
			printf("Nothing to pop\n");
		}
	} else {
		Dump();
	}
}

bool Stack::Empty () {
	if (Ok()) {
		return !size_;
	} else {
		Dump();
	}
} 

void Stack::Clear () {
	if (Ok()) {
		if (!Empty()) {
			while (size_) {
				data_[--size_] = 0;
			}
		}
	} else {
		Dump();
	}
}

int Stack::Size () {
	if (Ok()) {
		return size_;
	} else {
		Dump();
	}
}

int Stack::Capacity () {
	if (Ok()) {
		return capacity_;
	} else {
		Dump();
	}
}

bool Stack::Ok () {
	return (((data_&&capacity_&&size_<capacity_)||(!data_&&!capacity_&&!size_))&&this);
}

void Stack::Dump () {
	if (Ok()) {
		if (data_) {
			if (size_) {
				size_t i = size_;
				printf("Capacity: %d \n", capacity_);
				printf("Size: %d \n", size_);
				printf("Stack data: \n");
				while (i--) {
					printf("	%g \n", data_[i]);
				}
			} else {
				printf("Empty stack\n");
			}
		} else {
			printf("No data\n");
		}
	} else {
		printf("Error\n");
	}
}

int main()
{
	Stack s(1000);
	s.Push (1);
	s.Push (2);
	s.Push (123);
	s.Pop();
	s.Pop();
	s.Pop();
	s.Pop();
	s.Dump();
	return 0;
}