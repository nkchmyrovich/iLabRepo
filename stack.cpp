#include <stdio.h>
#include <stdlib.h>

const size_t CAPACITY_SIZE_ERR = 1;
const size_t NO_STACK_ERR = 2;
const size_t WR_CAPACITY_ERR = 3;
const size_t WR_SIZE_ERR = 4;
const size_t NO_CAPACITY_ERR = 5; 

class Stack {
private:
	double * data_;
	size_t size_;
	size_t capacity_;
	size_t last_error_;
public:
	void Push (double value);
	double Pop();
	bool Empty();
	void Clear();
	int Size();
	int Capacity();
	bool Ok();
	void Dump();
	void PrintError();
	Stack(size_t size);
	~Stack();
	size_t CheckErrors();
};

Stack::Stack (size_t size) :
	data_ (new double [size]),
	capacity_ (size),
	size_ (0),
	last_error_ (0)
	{}

Stack::~Stack () {
	delete data_;
	size_ = NULL;
	capacity_ = NULL;
	last_error_ = NULL;
}

void Stack::Push (double value) {
	if (!Ok()) {
		PrintError();
		return;
	}
	data_[size_++] = value;
}

double Stack::Pop () {
	if (!Ok()) {
		PrintError();
		return 0;
	}
	if (!size_) {
		printf("Nothing to pop\n");
		return 0;
	}
	return data_[--size_];
}

bool Stack::Empty () {
	if (!Ok()) {
		PrintError();
		return 0;
	}
	return !size_;
} 

void Stack::Clear () {
	if (!Ok()) {
		PrintError();
		return;
	}
	if (!Empty()) {
		while (size_) {
			data_[--size_] = 0;
		}
	}
}

int Stack::Size () {
	if (!Ok()) {
		PrintError();
		return 0;
	}
	return size_;
}

int Stack::Capacity () {
	if (!Ok()) {
		PrintError();
		return 0;
	}
	return capacity_;
}

bool Stack::Ok () {
	return (((data_&&capacity_&&size_<capacity_)||(!data_&&!capacity_&&!size_))&&this);
}

void Stack::Dump () {
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
}

size_t Stack::CheckErrors () {
	if (!Ok()) {
		if (size_>=capacity_) {
			last_error_ = CAPACITY_SIZE_ERR;
		} else if (this == NULL) {
			last_error_ = NO_STACK_ERR;
		} else if (data_ == NULL) {
			if (capacity_) {
				last_error_ = WR_CAPACITY_ERR;
			} else if (size_) {
				last_error_ = WR_SIZE_ERR;
			}
		} else if (data_) {
			if (capacity_ == 0) {
				last_error_ = NO_CAPACITY_ERR;
			}
		} 
		return 1;
	}
	return 0;
} 

void Stack::PrintError() {
	if  (CheckErrors()) {
		switch (last_error_) {
		case 1:
			printf("Error code: %d, size > capacity\n", last_error_);
			break;
		case 2:
			printf("Error code: %d, no stack\n", last_error_);
			break;
		case 3:
			printf("Error code: %d, wrong capacity\n", last_error_);
			break;
		case 4:
			printf("Error code: %d, wrong size\n", last_error_);
			break;
		case 5:
			printf("Error code: %d, stack with no capacity\n", last_error_);
			break;
		}
	}
	Dump();
}

int main()
{
Stack s (15); 
{ 
Stack s1 (s);
s1.PrintError();
} 
s.Push (1.0);
return 0;
}