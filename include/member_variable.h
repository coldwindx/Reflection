#pragma once

#include <string>
#include <any>
#include <functional>

using std::any;
using std::any_cast;
using std::function;
using std::string;

namespace reflect
{
    class MemberVariable
    {
        friend class RawTypeDescriptorBuilder;

    public:
        MemberVariable() = default;

        template <typename Class, typename Var>
        MemberVariable(Var Class::*var)
        {
            getter = [var](any obj) -> any
            {
                return any_cast<const Class *>(obj)->*var;
            };
            setter = [var](any obj, any val) -> void
            {
                auto *self = any_cast<Class *>(obj);
                self->*var = any_cast<Var>(val);
            };
        }

        const string &getName() const
        {
            return name_;
        }

        template <typename Var, typename Class>
        Var getValue(const Class &obj) const
        {
            return any_cast<Var>(getter(&obj));
        }

        template <typename Var, typename Class>
        void setValue(Class &obj, Var value) const
        {
            setter(&obj, value);
        }

    private:
        string name_;
        function<any(any)> getter{nullptr};
        function<void(any, any)> setter{nullptr};
    };
}