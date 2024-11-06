#include <iostream>
#include <exception>
using namespace std;
void Func()
{
    cerr << "terminate handler called";
    abort();
}
int main()
{
    set_terminate(Func);
    throw 0;
    return 0;
}