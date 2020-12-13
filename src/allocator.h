#pragma once

#include <cstring>
#include <new>
#include <stdexcept>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include "manager.h"

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