#pragma once
#include <any>
#include <functional>
#include "trait.h"

using std::any;
using std::function;
namespace reflect
{
    class MemberFunction
    {
        friend class RawTypeDescriptorBuilder;

    public:
        /**
         * * @brief 无返回值的非const成员函数
         */
        template <typename Class, typename... Args>
        explicit MemberFunction(void (Class::*func)(Args...))
        {
            argCount_ = sizeof...(Args);
            func_ = [this, func](void *argsPtr) -> any
            {
                auto &args = *static_cast<array<ArgWrap, sizeof...(Args) + 1> *>(argsPtr);
                auto tp = asTuple<Class &, Args...>(args);
                std::apply(func, tp);
                return any{};
            };
        }

        /**
         * * @brief 无返回值的const成员函数
         */
        template <typename Class, typename... Args>
        explicit MemberFunction(void (Class::*func)(Args...) const)
        {
            argCount_ = sizeof...(Args);
            func_ = [this, func](void *argsPtr) -> any
            {
                auto &args = *static_cast<array<ArgWrap, sizeof...(Args) + 1> *>(argsPtr);
                auto tp = asTuple<Class &, Args...>(args);
                std::apply(func, tp);
                return any{};
            };
            isConst_ = true;
        }
        /**
         * * @brief 有返回值的非const成员函数
         */
        template <typename Class, typename Return, typename... Args>
        explicit MemberFunction(Return (Class::*func)(Args...))
        {
            argCount_ = sizeof...(Args);
            func_ = [this, func](void *argsPtr) -> any
            {
                auto &args = *static_cast<array<ArgWrap, sizeof...(Args) + 1> *>(argsPtr);
                auto tp = asTuple<Class &, Args...>(args);
                return std::apply(func, tp);
            };
        }
        /**
         * @brief 有返回值的const成员函数
         */
        template <typename Class, typename Return, typename... Args>
        explicit MemberFunction(Return (Class::*func)(Args...) const)
        {
            argCount_ = sizeof...(Args);
            func_ = [this, func](void *argsPtr) -> any
            {
                auto &args = *static_cast<array<ArgWrap, sizeof...(Args) + 1> *>(argsPtr);
                auto tp = asTuple<Class &, Args...>(args);
                return std::apply(func, tp);
            };
            isConst_ = true;
        }

        template <typename Class, typename... Args>
        any invoke(Class &obj, Args &&...args) const
        {
            if (argCount_ != sizeof...(Args))
                throw std::runtime_error("Argument count mismatch");
            array<ArgWrap, sizeof...(Args) + 1> argsArray = {ArgWrap(obj), ArgWrap(std::forward<Args>(args))...};
            return func_(&argsArray);
        }

        inline bool isConst() const
        {
            return isConst_;
        }
        inline const string &getName() const
        {
            return name_;
        }

    private:
        function<any(void *)> func_;
        bool isConst_ = false; // 是否为const成员函数
        size_t argCount_ = 0;  // 参数个数
        string name_;
    };
}