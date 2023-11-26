/*
 * @Author: suzy HideSue@github.com
 * @Date: 2023-11-24 16:23:29
 * @LastEditors: suzy HideSue@github.com
 * @LastEditTime: 2023-11-25 17:21:44
 * @FilePath: /tinystl/simple_alloc.h
 * @Description: 调用alloc，负责空间的创建和回收（又单独套了一层）
 * 
 * Copyright (c) 2023 by ${suzy HideSue@github.com}, All Rights Reserved. 
 */

#ifndef SIMPLE_ALLOC_H
#define SIMPLE_ALLOC_H

#include<cstddef>
#include "alloc.h"

namespace tinystl
{
    template<class T, class Alloc = alloc>
    class simple_alloc {
        public:
            static T* allocate (size_t n) {
                return 0 == n ? nullptr : (T *) Alloc::allocate(n * sizeof(T));
            }
            static T* allocate (void) {
                return (T *) Alloc::allocate(sizeof(T));
            }
            static void deallocate (T *p, size_t n) {
                if (0 != n) Alloc::deallocate(p, n * sizeof(T));
            }
            static void deallocate (T *p) {
                Alloc::deallocate(p, sizeof(T));
            }
    };
} // namespace tinystl

#endif



