#pragma once
#include "registry.h"
#include "type_descriptor_builder.h"
#include "compiler.h"
namespace reflect
{
    template <typename Class>
    TypeDescriptorBuilder<Class> AddClass(const std::string &name)
    {
        return TypeDescriptorBuilder<Class>(name);
    }

    template <typename Class>
    vector<TypeDescriptor *> Get()
    {
        return Registry::instance().get();
    }

    TypeDescriptor &GetByName(const std::string &name)
    {
        return *Registry::instance().find(name);
    }
}