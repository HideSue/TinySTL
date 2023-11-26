/*
 * @Author: suzy HideSue@github.com
 * @Date: 2023-11-23 12:49:24
 * @LastEditors: suzy HideSue@github.com
 * @LastEditTime: 2023-11-25 19:55:01
 * @FilePath: /tinystl/alloc.h
 * @Description: alloc头文件，用于分配和回收内存，实现了一个内存池
 * 
 * Copyright (c) 2023 by ${suzy HideSue@github.com}, All Rights Reserved. 
 */
#ifndef ALLOC_H
#define ALLOC_H

#include<cstddef>
#include<cstdlib>
#include<iostream>

namespace tinystl
{
    union obj {
        union obj *free_list_link;  // 指针，指向一个obj
        char client_data[1];        // char数组，只有一个元素 
    };

    enum {ALIGN = 8};                       // 小型区块的上调边界
    enum {MAX_BYTES = 128};                 // 小型区块的上限
    enum {NFREELISTS = MAX_BYTES / ALIGN};  // freelist个数
    
    class alloc {
        private:
            static obj *free_list[NFREELISTS];  // NFREELISTS条freelist
            static char *start_free;            // 内存池起始位置
            static char *end_free;              // 内存池结束位置
            static size_t heap_size;            // 已申请的内存大小
        private:
            /**
             * @description: 将bytes上调至8的倍数
             * @param {size_t} bytes
             * @return {*}
             */            
            static size_t round_up (size_t bytes); 
            /**
             * @description: 根据区块大小，决定使用第几条freelist
             * @param {size_t} bytes
             * @return {*}
             */            
            static size_t freelist_index (size_t bytes);
            /**
             * @description: 返回一个大小为n的对象
             * @param {size_t} n
             * @return {*}
             */            
            static void* refill (size_t n);
            /**
             * @description: 取空间以容纳 nobjs * size 大小的区块，如果条件不允许，nobjs可能会有所调整 
             * @param {size_t} size
             * @param {int} &nobjs
             * @return {*}
             */            
            static char *chunk_alloc (size_t size, size_t &nobjs);
        public:
            /**
             * @description: 空间配置函数，n大于128调用malloc，否则检查对应的freelist是否有可用区块，有直接用，没有则上调倍数，调用refill
             * @param {size_t} n
             * @return {*}
             */        
            static void *allocate (size_t n);
            /**
             * @description: 按照1、2级配置器回收区块
             * @param {void} *p
             * @param {size_t} n
             * @return {*}
             */            
            static void deallocate (void *p, size_t n);
            static void *reallocate (void *p, size_t old_sz, size_t new_sz);
    };

    // 静态成员变量初始化
    char *alloc::start_free = nullptr;
    char *alloc::end_free = nullptr;
    size_t alloc::heap_size = 0;
    obj *alloc::free_list[NFREELISTS] = {
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };
    
    inline size_t alloc::round_up (size_t bytes) {
        // 这样就能上调至 ALIGN 的倍数，你就说吊不吊
        return (((bytes) + ALIGN - 1) & ~(ALIGN - 1));
    }

    inline size_t alloc::freelist_index (size_t bytes) {
        return (((bytes) + ALIGN - 1) / ALIGN - 1);
    }
    
    void *alloc::refill (size_t n) {
        // 注意这里的n已经上调至8的倍数
        // 一次性申请20个区块
        size_t nobjs = 20;
        char *chunk = chunk_alloc(n, nobjs);
        
        obj *my_free_list;
        obj *result;
        obj *current_obj, *next_obj;
        int i;
        
        // 只申请到了1个区块
        if (1 == nobjs) {
            return chunk;
        }
        // 其他情况
        my_free_list = free_list[freelist_index(n)];
        result = (obj *) chunk;
        my_free_list = next_obj = (obj *) (chunk + n);
        for (i = 1; ; i++) {
            current_obj = next_obj;
            next_obj = (obj *) ((char *) next_obj + n);
            if (nobjs - 1 == i) {
                current_obj->free_list_link = 0;
                break;
            } else {
                current_obj->free_list_link = next_obj;
            }
        }
        return result;
    }

    char *alloc::chunk_alloc (size_t size, size_t &nobjs) {
        // size已经上调至8的倍数
        char *result;
        size_t total_bytes = size * nobjs;   
        size_t bytes_left = end_free - start_free; // 内存池剩余空间
        if (bytes_left >= total_bytes) {
            // 内存池剩余空间完全满足需求
            result = start_free;
            start_free += total_bytes;
            return result;
        } else if (bytes_left >= size) {
            // 内存池剩余空间不能完全满足需求，但至少能满足一个
            nobjs = bytes_left / size; // 内存池剩余空间能满足多少个区块
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        } else {
            // 内存池剩余空间一个区块都满足不了
            size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4); // 计算出一个合适的空间大小（2*此时我们需要的空间+一些额外空间）
            if (bytes_left > 0) {
                // 内存池还有一些空间
                obj *my_free_list = free_list[freelist_index(bytes_left)]; // 这是和剩余空间最接近的区块的freelist
                ((obj *)start_free)->free_list_link = my_free_list; // 头插法，把剩余空间给最接近的区块的freelist
                my_free_list = (obj *) start_free;

                // 这里有个疑问
                // 假设 bytes_left = 6，会把这6个字节放到放到freelist[0]处
                // 一个obj*大小是8，((obj *)start_free)->free_list_link不能放在这里，这样会造成问题
                // 并且freelist[0]应该返回一个 8 * n 大小的空间

                // 解决：size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
                // 每次申请的内存总是8的倍数，每次取得空间也是8得倍数
                // 所以剩下的空间一定是8的倍数。这里是reelist_index(bytes_left)而不是reelist_index(round_up(bytes_left))也说明了这个问题
            }
            start_free = (char *)malloc(bytes_to_get);
            if (0 == start_free) {
                // heap空间不足，检查我们的freelist
                int i;
                obj *my_free_list, *p;
                for (int i = size; i <= MAX_BYTES; i += ALIGN) {
                    // 再次注意，size已经上调至8的倍数
                    // 从当前size在的freelist往右数，找到第一块可用的区块返回(只给一块就够了！)
                    my_free_list = free_list[freelist_index(i)];
                    p = my_free_list;
                    if (p != nullptr) {
                        my_free_list = p->free_list_link;
                        start_free = (char *) p;
                        end_free = start_free + i;
                        return chunk_alloc(size, nobjs);
                    }
                }
                end_free = nullptr; // 没有空间了，一点都没了
                std::cout << "没有空间了！" << std::endl;
                throw std::bad_alloc();
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
        
    }

    void *alloc::allocate (size_t n) {
        obj *my_free_list;
        obj *result;
        if (n > (size_t) MAX_BYTES) {
            return malloc(n);
        }
        // 找到合适的freelist
        my_free_list = free_list[freelist_index(n)];
        result = my_free_list;
        if (result == nullptr) {
            void *r = refill(round_up(n));
            return r;
        }
        my_free_list = result->free_list_link;
        return result;
    }

    void alloc::deallocate (void *p, size_t n) {
        if (n > (size_t) MAX_BYTES) {
            free(p);
            return;
        }
        obj *q = (obj *) p;
        obj *my_free_list;
        my_free_list = free_list[freelist_index(n)];
        q->free_list_link = my_free_list; // 头插
        my_free_list = q;
    }
} // namespace tinystl


#endif