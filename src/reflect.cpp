#include <iostream>
#include "reflect.h"

reflect::Generator::Generator(const char *source, const char *target)
{
    stream.open(target, std::ios::out | std::ios::trunc);
    if (!stream.is_open())
        throw std::runtime_error("Failed to open target file: " + string(target));
    MetaCompiler compiler(source);
    auto metaDatas = compiler.compile();

    stream << "#pragma once\n";
    stream << "#include \"reflect.h\"\n";
    stream << "#include \"" << source << "\"\n";

    for (auto &[key, value] : metaDatas)
    {
        stream << "class " << key << "_Ref {\n";
        stream << "public:\n";
        stream << "\t" << key << "_Ref() {\n";
        stream << "\t\treflect::TypeDescriptorBuilder<" << key << ">(\"" << key << "\")";
        for (auto &member : value)
        {
            if (member.key == "field")
                stream << "\n\t\t.property";
            if (member.key == "method")
                stream << "\n\t\t.method";
            stream << "(\"" << member.value << "\", &" << key << "::" << member.value << ")";
        }
        stream << ";\n\t}\n};\n\n";
    }
}

reflect::Generator::~Generator()
{
    stream.close();
}
