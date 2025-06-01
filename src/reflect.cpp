#include <iostream>
#include <cstring>
#include "reflect.h"

reflect::Generator::Generator(const char *source, const char *target)
{
    MetaCompiler compiler(source);
    auto metaDatas = compiler.compile();
    printf("Generating reflection code for %s to %s\n", source, target);
    stream.open(target, std::ios::out | std::ios::trunc);
    if (!stream.is_open())
        throw std::runtime_error("Failed to open target file: " + string(target));

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

string reflect::Generator::replaceSrcName(const string &originName, const string &suffix)
{
    size_t name_len = originName.length();
    size_t suffix_len = suffix.length();

    size_t pos = originName.rfind(suffix);
    if (pos == std::string::npos || pos != originName.length() - 2)
        return string();

    string gen_name = originName.substr(0, name_len - suffix_len) + ".generated" + suffix;;
    return gen_name;
}