#pragma once

#include <type_traits>
#include <any>
#include <array>
#include <tuple>
using std::any;
using std::any_cast;
using std::false_type;
using std::true_type;

using std::array;
using std::forward_as_tuple;
using std::index_sequence;
using std::tuple;

namespace reflect
{
    template <typename T>
    struct RefTrait
    {
        static constexpr int value = 0;
    };

    template <typename T>
    struct RefTrait<T &>
    {
        static constexpr int value = 1;
    };

    template <typename T>
    struct IsConst : false_type
    {
    };

    template <typename T>
    struct IsConst<T &> : false_type
    {
    };

    template <typename T>
    struct IsConst<T *> : false_type
    {
    };

    template <typename T>
    struct IsConst<const T> : true_type
    {
    };

    template <typename T>
    struct IsConst<const T &> : true_type
    {
    };

    template <typename T>
    struct IsConst<const T *> : true_type
    {
    };

    class ArgWrap
    {
    public:
        template <typename T>
        ArgWrap(T &&value)
        {
            refType = RefTrait<T>::value;
            isConst = IsConst<T>::value;
            if (1 == refType)
                storage = &value;
            else
                storage = value;
        }

        template <typename T>
        T cast()
        {
            using RawT = std::remove_cv_t<std::remove_reference_t<T>>;
            constexpr int castRefType = RefTrait<T>::value;
            constexpr bool castIsConst = IsConst<T>::value;

            if constexpr (0 == castRefType)
                return 0 == refType ? any_cast<RawT>(storage) : (isConst ? *any_cast<const RawT *>(storage) : *any_cast<RawT *>(storage));
            // 值类型转换为引用类型
            if (0 == refType)
                return *any_cast<RawT>(&storage);
            // 引用类型转换为引用类型
            if constexpr (castIsConst)
                return isConst ? *any_cast<const RawT *>(storage) : *any_cast<RawT *>(storage);
            if (isConst)
                throw std::runtime_error("Cannot cast const reference to non-const reference");
            return *any_cast<RawT *>(storage);
        }

    private:
        int refType{0};      // 0: value, 1: reference
        bool isConst{false}; // 是否为const类型
        any storage{};
    };

    template <typename... Args, size_t N, size_t... Is>
    tuple<Args...> asTuple(array<ArgWrap, N> &args, index_sequence<Is...>)
    {
        return forward_as_tuple(args[Is].template cast<Args>()...);
    }

    template <typename... Args, size_t N, typename = std::enable_if_t<sizeof...(Args) == N>>
    tuple<Args...> asTuple(array<ArgWrap, N> &args)
    {
        return asTuple<Args...>(args, std::make_index_sequence<N>());
    }
}