/**
 *  traits.h
 *  Some type traits for helper needs
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {
namespace traits {

    namespace sfinae {
        typedef struct { char _; } yes;
        typedef struct { yes _[2]; } no;
    }

    // This type trait checks if the `begin` and `end` methods is defined in the class T
    template <typename T>
    class may_iterable
    {
        typedef typename std::decay<T>::type X;
        template <typename R> static sfinae::yes test( decltype(&R::begin), decltype(&R::end) ) ;
        template <typename R> static sfinae::no test(...);
    public:
        // has value std::true_type or std::false_type
        constexpr static auto value  =  std::integral_constant<bool, sizeof(test<X>(0,0)) == sizeof(sfinae::yes)>();
    };
    
    // This type trait checks if the `data` and `size` methods is defined in the class T
    template <typename T>
    class may_string
    {
        typedef typename std::decay<T>::type X;
        template <typename R> static sfinae::yes test( decltype(&R::data), decltype(&R::size) ) ;
        template <typename R> static sfinae::no test(...);
    public:
        // has value std::true_type or std::false_type
        constexpr static auto value  =  std::integral_constant<bool, sizeof(test<X>(0,0)) == sizeof(sfinae::yes)>();
    };

}}