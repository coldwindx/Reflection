#pragma once
#include "reflect.h"
#include "/home/zhulin/workspace/Reflection/source.hpp"
class HelloController_Ref {
public:
	HelloController_Ref() {
		reflect::TypeDescriptorBuilder<HelloController>("HelloController");
	}
};

