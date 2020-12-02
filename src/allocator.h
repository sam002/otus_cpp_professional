#pragma once

#include <cstring>
#include <new>
#include <stdexcept>

/** Base interface by https://github.com/ros2/realtime_support/blob/master/tlsf_cpp/include/tlsf_cpp/tlsf.hpp
 * Greedy algorithm of linear allocator. Reuse memory only if free last chunk.
**/
template<typename T, size_t DefaultPoolSize = 2>
struct custom_allocator
{
    // Needed for std::allocator_traits
    using value_type = T;

    explicit custom_allocator(size_t size)
            : memory_pool(nullptr), pool_size(size)
    {
        initialize(size);
    }

    // Needed for std::allocator_traits
    custom_allocator()
            : memory_pool(nullptr)
    {
        initialize(DefaultPoolSize);
    }

    // Needed for std::allocator_traits
    template<typename U>
    custom_allocator(const custom_allocator<U> & alloc)
            : memory_pool(alloc.memory_pool), pool_size(alloc.pool_size)
    {
    }

    template<typename U, size_t OtherDefaultSize>
    custom_allocator(const custom_allocator<U> & alloc)
            : memory_pool(alloc.memory_pool), pool_size(alloc.pool_size)
    {
    }

    ~custom_allocator()
    {
        if (memory_pool) {
            delete memory_pool;
            memory_pool = nullptr;
        }
    }

    size_t initialize(size_t size)
    {
        pool_size = size;
        if (!memory_pool) {
            memory_pool = new char[pool_size];
            memset(memory_pool, 0, pool_size);
            current_ptr = (T *)(memory_pool + sizeof(size_t));
        }
        return pool_size;
    }

    void set_current_chunk_size(size_t size)
    {
        auto current_size_ptr = (size_t *)(current_ptr - sizeof(size_t));
        *current_size_ptr = size;
    }


    // Needed for std::allocator_traits
    T * allocate(size_t size)
    {
        T * ptr = static_cast<T *>(custom_malloc(size * sizeof(T)));
        if (ptr == NULL && size > 0) {
            throw std::bad_alloc();
        }
        return ptr;
    }

    T * custom_malloc(size_t size) {
        if(size < pool_size_allow()) {
            auto new_pool_size = pool_size*2;
            auto new_memory_pool = new char[pool_size*2];
            memcpy(new_memory_pool, memory_pool, pool_size);

            delete memory_pool;

            pool_size = new_pool_size;
            memory_pool = new_memory_pool;
        }
        current_ptr += size*sizeof(T);
        set_current_chunk_size(size);
        return current_ptr;
    }

    size_t pool_size_allow() {
        return pool_size - (size_t)memory_pool + (size_t)current_ptr;
    }

    // Needed for std::allocator_traits
    void deallocate(T * ptr, size_t size)
    {
        auto * chunk_size_ptr = (size_t *)((char *)ptr - sizeof(size_t));
        if(ptr + *chunk_size_ptr == current_ptr) {
            if (size == *chunk_size_ptr) {
                *chunk_size_ptr = 0;
            }
            current_ptr = ptr;
        }
    }

    template<typename U>
    struct rebind
    {
        typedef custom_allocator<U> other;
    };

    char * memory_pool;
    size_t pool_size;
    T * current_ptr;
};

// Needed for std::allocator_traits
template<typename T, typename U>
constexpr bool operator==(
        const custom_allocator<T> & a,
        const custom_allocator<U> & b) noexcept
{
    return a.memory_pool == b.memory_pool;
}

// Needed for std::allocator_traits
template<typename T, typename U>
constexpr bool operator!=(
        const custom_allocator<T> & a,
        const custom_allocator<U> & b) noexcept
{
    return a.memory_pool != b.memory_pool;
}

template<typename T, typename U, size_t X, size_t Y>
constexpr bool operator==(
        const custom_allocator<T, X> & a,
        const custom_allocator<U, Y> & b) noexcept
{
    return a.memory_pool == b.memory_pool;
}

// Needed for std::allocator_traits
template<typename T, typename U, size_t X, size_t Y>
constexpr bool operator!=(
        const custom_allocator<T, X> & a,
        const custom_allocator<U, Y> & b) noexcept
{
    return a.memory_pool != b.memory_pool;
}
