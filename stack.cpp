#include <stdio.h>
#include <stdlib.h>

class Stack {
private:
	double * data_;
	size_t size_;
	size_t capacity_;
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
	size_t CheckErrors();
	size_t last_error_;
	Stack(size_t size);
};

Stack::Stack (size_t size) :
	data_ (new double [size]),
	capacity_ (size),
	size_ (0),
	last_error_ (0)
	{}

void Stack::Push (double value) {
	if (Ok()) {
		data_[size_++] = value;
	} else {
		PrintError();
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
		PrintError();
	}
}

bool Stack::Empty () {
	if (Ok()) {
		return !size_;
	} else {
		PrintError();
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
		PrintError();
	}
}

int Stack::Size () {
	if (Ok()) {
		return size_;
	} else {
		PrintError();
	}
}

int Stack::Capacity () {
	if (Ok()) {
		return capacity_;
	} else {
		PrintError();
	}
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
	if (Ok()) {
		return 0;
	} else {
		if (size_>=capacity_) {
			last_error_ = 1;
		} else if (this == NULL) {
			last_error_ = 2;
		} else if (data_ == NULL) {
			if (capacity_) {
				last_error_ = 3;
			} else if (size_) {
				last_error_ = 4;
			}
		} else if (data_) {
			if (capacity_ == 0) {
				last_error_ = 5;
			}
		} 
	};
	return 1;
} 

void Stack::PrintError() {
	if  (CheckErrors()) {
		if (last_error_ == 1) {
			printf("Error code: %d, size > capacity\n", last_error_);
		} else if (last_error_ == 2) {
			printf("Error code: %d, no stack\n", last_error_);
		} else if (last_error_ == 3) {
			printf("Error code: %d, wrong capacity\n", last_error_);
		} else if (last_error_ == 4) {
			printf("Error code: %d, wrong size\n", last_error_);
		} else if (last_error_ == 5) {
			printf("Error code: %d, stack with no capacity\n", last_error_);
		}
	};
	Dump();
}

int main()
{
	Stack s(1);
	s.Push(1);
	s.Push(2);
	return 0;
}