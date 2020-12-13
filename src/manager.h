#pragma once

#include <cstring>
#include <new>
#include <stdexcept>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>

template<typename T>
class MemoryManagement {
public:
    size_t pool_size = 0;
private:
    char * memory_pool = nullptr;
    std::vector<bool> use_pool_mask;

public:

    explicit MemoryManagement() : pool_size(2)
    {
        memory_pool = new char[pool_size*sizeof(T)];
        memset(memory_pool, 0, pool_size*sizeof(T));
        use_pool_mask = std::vector<bool>(pool_size, false);
    }

    explicit MemoryManagement(size_t pool_size_) : pool_size(pool_size_)
    {
        memory_pool = new char[pool_size*sizeof(T)];
        memset(memory_pool, 0, pool_size*sizeof(T));
        use_pool_mask = std::vector<bool>(pool_size, false);
    }

    ~MemoryManagement()
    {
        delete memory_pool;
        use_pool_mask.clear();
    }

    T * custom_malloc(size_t size) {
        if(1 > size) {
            throw std::bad_alloc();
        }
        auto free_ptr = std::search_n(use_pool_mask.begin(), use_pool_mask.end(), size, false);
        auto ptr = (T *)(memory_pool + (sizeof(T))*free_ptr._M_offset);
        *free_ptr = true;
        return ptr;
    }

    void custom_deallocate(T * ptr, size_t size)
    {
        auto shift_ptr = ptr - (T *)memory_pool;
        while (size > 0) {
            use_pool_mask[shift_ptr+size-1] = false;
            size--;
        }
    }

    bool can_hold(size_t need_size)
    {
        return use_pool_mask.end() != std::search_n(use_pool_mask.begin(), use_pool_mask.end(), need_size, false);
    }

    bool in_pool(T const * ptr)
    {
        return  ptr >= (T *)memory_pool && ptr <= ((T *)memory_pool + pool_size);
    }
};