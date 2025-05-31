#pragma once
#include "raw_type_descriptor_builder.h"
namespace reflect
{

    template <typename Class>
    class TypeDescriptorBuilder
    {
    public:
        explicit TypeDescriptorBuilder(const std::string &name) : builder(name) {}

        template <typename Var>
        TypeDescriptorBuilder &addMemberVariable(const std::string &name, Var Class::*var)
        {
            builder.addMemberVariable(name, var);
            return *this;
        }

        template <typename Func>
        TypeDescriptorBuilder &addMemberFunction(const std::string &name, Func Class::*func)
        {
            builder.addMemberFunction(name, func);
            return *this;
        }

    private:
        RawTypeDescriptorBuilder builder;
    };
}