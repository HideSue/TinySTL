/*
 * @Author: suzy HideSue@github.com
 * @Date: 2023-11-24 16:50:20
 * @LastEditors: suzy HideSue@github.com
 * @LastEditTime: 2023-11-25 19:47:18
 * @FilePath: /tinystl/uninitialized.h
 * @Description: 全局函数，对容器进行初始化
 * 
 * Copyright (c) 2023 by ${suzy HideSue@github.com}, All Rights Reserved. 
 */
#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H

#include "type_traits.h"
#include "construct.h"

namespace tinystl
{

    template<class InputIterator, class ForwardIterator>
    ForwardIterator __uninitialized_copy_aux (InputIterator first, InputIterator last, ForwardIterator result, true_type) {
        // todo 这里应该用copy算法，从底层复制
        ForwardIterator cur = result;
        for (; first != last; ++first, ++cur) {
            *cur = *first;
        }
        return cur;
    }

    template<class InputIterator, class ForwardIterator>
    ForwardIterator __uninitialized_copy_aux (InputIterator first, InputIterator last, ForwardIterator result, false_type) {
        ForwardIterator cur = result;
        for (; first != last; ++first, ++cur) {
            construct(&*cur, *first);
        }
        return cur;
    }

    template<class InputIterator, class ForwardIterator, class T>
    ForwardIterator __uninitialized_copy (InputIterator first, InputIterator last, ForwardIterator result, T *) {
        typedef typename type_traits<T>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, result, is_POD());
    }

    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy (InputIterator first, InputIterator last, ForwardIterator result) {
        __uninitialized_copy(first, last, result, value_type(result));
    }

    template<class ForwardIterator, class T>
    void uninitialized_fill (ForwardIterator first, ForwardIterator last, const T &result) {

    }

    template<class ForwardIterator, class Size, class T>
    ForwardIterator __uninitialized_fill_n_aux (ForwardIterator first, Size n, const T &x, true_type) {
        // todo
    }

    template<class ForwardIterator, class Size, class T>
    ForwardIterator __uninitialized_fill_n_aux (ForwardIterator first, Size n, const T &x, false_type) {
        ForwardIterator cur = first;
        for (; n > 0; --n, ++cur) {
            construct(&*cur, x);
        }
        return cur;
    }

    template<class ForwardIterator, class Size, class T, class T1>
    ForwardIterator __uninitialized_fill_n (ForwardIterator first, Size n, const T &x, T1 *) {
        typedef typename type_traits<T1>::is_POD_type is_POD;
        return __uninitialized_fill_n_aux(first, n, x, is_POD());
    }

    template<class ForwardIterator, class Size, class T>
    ForwardIterator uninitialized_fill_n (ForwardIterator first, Size n, const T &x) {
        return __uninitialized_fill_n(first, n, x, value_type(first));
    }
} // namespace tinystl

#endif