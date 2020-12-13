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

template<typename T, size_t PoolSize = 2>
class CustomAllocator {
public:
    // Needed for std::allocator_traits
    using value_type = T;
private:
    std::vector<std::unique_ptr<MemoryManagement<T>>> managers;

public:
    explicit CustomAllocator(size_t size)
    {
        managers.push_back(std::make_unique<MemoryManagement<T>>(size));
    }

    // Needed for std::allocator_traits
    CustomAllocator() : CustomAllocator(PoolSize) {};

    // Needed for std::allocator_traits
    template<typename U>
    explicit CustomAllocator(const CustomAllocator<U, PoolSize> & alloc) : CustomAllocator<U, PoolSize>(alloc) {}

    template<typename U, size_t OtherDefaultSize>
    CustomAllocator(const CustomAllocator<U, OtherDefaultSize> & alloc)
    {
        //todo need full copy
        managers = alloc.managers;
    }

    ~CustomAllocator()
    {
        for (auto &manager :managers) {
            manager.release();
        }
        managers.clear();
    }

    // Needed for std::allocator_traits
    T * allocate(size_t size)
    {

        for (std::unique_ptr<MemoryManagement<T>> &manager : managers) {
            if (manager->can_hold(size)) {
                return static_cast<T *>(manager->custom_malloc(size));
            }
        }
        managers.push_back(std::make_unique<MemoryManagement<T>>(managers.back()->pool_size*2));

        return static_cast<T *>(managers.back()->custom_malloc(size));
    }

    // Needed for std::allocator_traits
    void deallocate(T * ptr, size_t size)
    {
        for (std::unique_ptr<MemoryManagement<T>> &manager :managers) {
            if (manager->in_pool(ptr)) {
                manager->custom_deallocate(ptr, size);
            }
        }
        if (managers.back()->can_hold(managers.back()->pool_size)) {
            managers.pop_back();
        }
    }

    template<typename U>
    struct rebind
    {
        typedef CustomAllocator<U, PoolSize> other;
    };
private:
};

// Needed for std::allocator_traits
template<typename T, typename U>
constexpr bool operator==(
        const CustomAllocator<T, 1> & a,
        const CustomAllocator<U, 1> & b) noexcept
{
    return a.memory_pool == b.memory_pool;
}

// Needed for std::allocator_traits
template<typename T, typename U>
constexpr bool operator!=(
        const CustomAllocator<T, 1> & a,
        const CustomAllocator<U, 1> & b) noexcept
{
    return a.memory_pool != b.memory_pool;
}

template<typename T, typename U, size_t X, size_t Y>
constexpr bool operator==(
        const CustomAllocator<T, X> & a,
        const CustomAllocator<U, Y> & b) noexcept
{
    return a.memory_pool == b.memory_pool;
}

// Needed for std::allocator_traits
template<typename T, typename U, size_t X, size_t Y>
constexpr bool operator!=(
        const CustomAllocator<T, X> & a,
        const CustomAllocator<U, Y> & b) noexcept
{
    return a.memory_pool != b.memory_pool;
}