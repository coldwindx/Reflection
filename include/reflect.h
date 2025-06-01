#pragma once
#include <fstream>
#include "registry.h"
#include "type_descriptor_builder.h"
#include "compiler.h"
namespace reflect
{
    template <typename Class>
    static TypeDescriptorBuilder<Class> AddClass(const std::string &name)
    {
        return TypeDescriptorBuilder<Class>(name);
    }

    template <typename Class>
    static vector<TypeDescriptor *> Get()
    {
        return Registry::instance().get();
    }

    inline TypeDescriptor &GetByName(const std::string &name)
    {
        return *Registry::instance().find(name);
    }

    class Generator
    {
    public:
        Generator(const char *source, const char *target);
        ~Generator();
        // protected:
        static string replaceSrcName(const string &originName, const string &suffix);

    private:
        std::ofstream stream;
    };
}