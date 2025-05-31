#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "member_variable.h"
#include "member_function.h"

using std::string;
using std::string_view;
using std::vector;

namespace reflect
{
    class TypeDescriptor
    {
        friend class RawTypeDescriptorBuilder;

    public:
        const string &getName() const
        {
            return name;
        }
        const vector<MemberVariable> &getMemberVariables() const
        {
            return memberVars;
        }
        const vector<MemberFunction> &getMemberFunctions() const
        {
            return memberFuncs;
        }
        MemberVariable getMemberVariable(string_view name) const
        {
            for (const auto &var : memberVars)
                if (var.getName() == name)
                    return var;
            throw std::runtime_error("Member variable not found");
        }
        MemberFunction getMemberFunction(string_view name) const
        {
            for (const auto &func : memberFuncs)
                if (func.getName() == name)
                    return func;
            throw std::runtime_error("Member function not found");
        }

    private:
        string name;
        vector<MemberVariable> memberVars;
        vector<MemberFunction> memberFuncs;
    };
}