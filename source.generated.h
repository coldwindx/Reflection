#pragma once
#include "reflect.h"
#include "source.hpp"
class MyClass_Ref {
public:
	MyClass_Ref() {
		reflect::AddClass<MyClass>("MyClass")
		.addMemberFunction("Func", &MyClass::Func)
		.addMemberVariable("field", &MyClass::field);
	}
};

