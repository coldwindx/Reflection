#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "type_descriptor.h"

using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace reflect
{
    class Registry
    {
    public:
        static Registry &instance()
        {
            static Registry ins;
            return ins;
        }
        vector<TypeDescriptor *> get() const
        {
            vector<TypeDescriptor *> result;
            for (const auto &[_, v] : descs)
            {
                result.push_back(v.get());
            }
            return result;
        }

        TypeDescriptor *find(const string &name) const
        {
            auto it = descs.find(name);
            if (it != descs.end())
                return it->second.get();
            std::runtime_error("TypeDescriptor not found: " + string(name));
        }

        void push(unique_ptr<TypeDescriptor> desc)
        {
            auto name = desc->getName();
            descs[name] = std::move(desc);
        }

        void clear()
        {
            decltype(descs) temp;
            descs.swap(temp);
        }

    private:
        unordered_map<string, unique_ptr<TypeDescriptor>> descs;
    };
}