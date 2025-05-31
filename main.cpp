#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <fstream>
#include "clang-c/Index.h"
#include "compiler.h"
using namespace std;
using namespace reflect;

int main()
{
    MetaCompiler compiler("../source.hpp");
    auto metaDatas = compiler.compile();

    const char *header = {"\
#pragma once\n\
#include \"reflect.h\"\n\
#include \"source.hpp\"\n"};
    string output(header);

    for (auto &[key, value] : metaDatas)
    {
        output += "class " + key + "_Ref {\n";
        output += "public:\n";
        output += "\t" + key + "_Ref() {\n";
        output += "\t\treflect::AddClass<" + key + ">(\"" + key + "\")";
        for (auto &member : value)
        {
            if (member.key == "field")
                output += "\n\t\t.addMemberVariable";
            if (member.key == "method")
                output += "\n\t\t.addMemberFunction";
            output += "(\"" + member.value + "\", &" + key + "::" + member.value + ")";
        }
        output += ";\n\t}\n};\n\n";
    }

    ofstream file("../source.generated.h", ios::out | ios::trunc);
    file << output;
    file.close();
    return 0;
}