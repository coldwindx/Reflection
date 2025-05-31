#pragma once
#include <memory>
#include "type_descriptor.h"
using std::unique_ptr;
namespace reflect
{
    class RawTypeDescriptorBuilder
    {
    public:
        explicit RawTypeDescriptorBuilder(const string &name);
        RawTypeDescriptorBuilder(const RawTypeDescriptorBuilder &other) = delete;
        ~RawTypeDescriptorBuilder();

        template <typename Class, typename Var>
        void property(const string &name, Var Class::*var)
        {
            MemberVariable variable(var);
            variable.name_ = name;
            desc->memberVars.push_back(variable);
        }

        template <typename Class, typename Func>
        void method(const string &name, Func Class::*func)
        {
            MemberFunction function(func);
            function.name_ = name;
            desc->memberFuncs.push_back(function);
        }

    private:
        unique_ptr<TypeDescriptor> desc{nullptr};
    };
}