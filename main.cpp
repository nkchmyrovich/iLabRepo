#include "Stack.h"

int main()
{
Stack s (15);
{
    Stack s1 (s);
}
s.Push (1.0);
s.PrintError();
return 0;
}