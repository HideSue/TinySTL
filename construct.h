/*
 * @Author: suzy HideSue@github.com
 * @Date: 2023-11-24 15:57:51
 * @LastEditors: suzy HideSue@github.com
 * @LastEditTime: 2023-11-25 19:10:35
 * @FilePath: /tinystl/construct.h
 * @Description: 一些全局函数，包含构造、销毁
 * 
 * Copyright (c) 2023 by ${suzy HideSue@github.com}, All Rights Reserved. 
 */
#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include<new>
#include "type_traits.h"

namespace tinystl
{
    template<class T1, class T2>
    inline void construct (T1 *p, T2 &value) {
        new(p) T1(value); // placement new， 在p位置处构造value
    }

    template <class T>
    inline void destroy (T *pointer) {
        pointer->~T(); // 直接调用析构函数
    }

    template <class ForwardIterator>
    inline void __destroy_aux (ForwardIterator first, ForwardIterator last, false_type) {
        for (; first != last; ++first) {
            destroy(&*first);
        }
    }

    template <class ForwardIterator>
    inline void __destroy_aux (ForwardIterator, ForwardIterator, true_type) {}

    template <class ForwardIterator, class T>
    inline void __destroy (ForwardIterator first, ForwardIterator last, T *) {
        typedef typename tinystl::type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }

    template <class ForwardIterator>
    inline void destroy (ForwardIterator first, ForwardIterator last) {
        __destroy(first, last, value_type(first));
    }

} // namespace tinystl

#endif