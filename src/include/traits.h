/**
 *  traits.h
 *  Some type traits for helper needs
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

#include <concepts>
#include <ranges>

namespace RocksServer {
namespace traits {

    namespace sfinae {
        typedef struct { char _;   } yes;
        typedef struct { yes _[2]; } no;
    }

    // This type trait checks if the `begin` and `end` methods is defined in the class T
    template <typename T>
    class may_iterable
    {
        using decay_t = typename std::decay<T>::type;
        template <typename R> static sfinae::yes test( decltype(&R::begin), decltype(&R::end) ) ;
        template <typename R> static sfinae::no  test(...);
    public:
        // has value std::true_type or std::false_type
        static constexpr auto value = std::integral_constant<bool, sizeof(test<decay_t>(0,0)) == sizeof(sfinae::yes)>();
        static constexpr bool b_val = (sizeof(test<decay_t>(0,0)) == sizeof(sfinae::yes));
    };
    
    // This type trait checks if the `data` and `size` methods is defined in the class T
    template <typename T>
    class may_string
    {
        using decay_t = typename std::decay<T>::type;

        template <typename R> static sfinae::yes test( decltype(static_cast<const char*(R::*)() const>(&R::data)), decltype(&R::size) ) ;
        template <typename R> static sfinae::no  test(...);
    public:
        // has value std::true_type or std::false_type
        static constexpr auto value = std::integral_constant<bool, sizeof(test<decay_t>(0, 0)) == sizeof(sfinae::yes)>();
        static constexpr bool b_val = (sizeof(test<decay_t>(0, 0)) == sizeof(sfinae::yes));
    };

    // 
    // concepts-based traits
    // 

    template <typename T>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

    // "string-like" по смыслу старого may_string, но более гибко:
    // 1) rocksdb::Slice
    // 2) неявно приводится к std::string_view
    // 3) имеет data()/size() (data -> const char*, size -> size_t-подобное)
    template <typename T>
    concept string_like_c = requires(const remove_cvref_t<T>& v) {
        { v.data() } -> std::convertible_to<const char*>;
        { v.size() } -> std::convertible_to<std::size_t>;
    };

    // template <typename T>
    // concept is_slice_c = std::same_as<remove_cvref_t<T>, rocksdb::Slice>;

    template <typename T>
    concept string_arg_c =
        // is_slice_c<T> ||
        // std::convertible_to<T, std::string_view> ||
        string_like_c<T>;

    template <typename T>
    concept range_not_string_c =
        may_iterable<T>::b_val &&
        // std::ranges::range<T> &&
        (!string_arg_c<T>); // string is a range as well,so exclude "string_arg_c"

    template <typename T>
    concept pair_like_c = requires(T v) {
        v.first;
        v.second;
    };
}}
