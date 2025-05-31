#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <fstream>
#include "clang-c/Index.h"
#include "reflect.h"
using namespace std;
using namespace reflect;

int main()
{
    Generator generator("../source.hpp", "source.generated.h");
    return 0;
}