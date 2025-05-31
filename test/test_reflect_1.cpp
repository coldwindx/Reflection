#include <iostream>
#include "reflect.h"
using namespace std;

class Foo
{
public:
    void Func() const
    {
        cout << str << endl;
    }
    string str;
};

int main()
{
    AddClass<Foo>("Foo")
        .addMemberVariable("str", &Foo::str)
        .addMemberFunction("Func", &Foo::Func);
    Foo foo;

    auto str = GetByName("Foo").getMemberVariables("str");
str.setValue<string>(foo, "Hello, World!");
cout << str.getValue<string>(foo) << endl;

GetByName("Foo").getMemberFunction("Func").invoke(foo);
   cout << GetByName("Foo").getMemberFunction("Func").isConst() << endl;

    return 0;
}