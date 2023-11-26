/*
 * @Author: suzy HideSue@github.com
 * @Date: 2023-11-24 16:58:03
 * @LastEditors: suzy HideSue@github.com
 * @LastEditTime: 2023-11-25 19:05:22
 * @FilePath: /tinystl/type_traits.h
 * @Description: 
 * 
 * Copyright (c) 2023 by ${suzy HideSue@github.com}, All Rights Reserved. 
 */
#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include <cstddef>

namespace tinystl
{
    // 五种迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
    };

    template<class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::difference_type      difference_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
    };

    // 原生指针偏特化
    template<class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    template<class T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    template<class Iterator>
    inline typename iterator_traits<Iterator>::value_type *value_type (const Iterator &) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    // 类型萃取
    struct true_type {};
    struct false_type {};

    template<class type>
    struct type_traits {
        typedef true_type this_dummy_member_must_be_first;
        typedef false_type has_trivial_default_constructor;
        typedef false_type has_trivial_copy_constructor;
        typedef false_type has_trivial_assignment_constructor;
        typedef false_type has_trivial_destructor;
        typedef false_type is_POD_type;
    };

    template<>
    struct type_traits<char> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<signed char> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<unsigned char> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<short> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<unsigned short> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<int> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<unsigned int> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<long> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<unsigned long> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<float> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<double> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<>
    struct type_traits<long double> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };

    template<class T>
    struct type_traits<T*> {
        typedef true_type has_trivial_default_constructor;
        typedef true_type has_trivial_copy_constructor;
        typedef true_type has_trivial_assignment_constructor;
        typedef true_type has_trivial_destructor;
        typedef true_type is_POD_type;
    };
} // namespace tinystl

#endif