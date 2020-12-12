#pragma once

#include <cstring>
#include <new>
#include <stdexcept>
#include <memory>
#include <vector>

template<typename T>
class MemoryManagement
{
public:
    const size_t pool_size = 0;
    char * memory_pool = nullptr;
    T * current_ptr = nullptr;
    std::vector<bool> use_pool_mask;

    explicit MemoryManagement(size_t pool_size_) : pool_size(pool_size_)
    {
        memory_pool = new char[pool_size*sizeof(T)];
        memset(memory_pool, 0, pool_size*sizeof(T));
        current_ptr = (T *)memory_pool;
        use_pool_mask = std::vector<bool>(pool_size, false);
    }

    template<typename U>
    MemoryManagement(const MemoryManagement<U> & other, size_t pool_size_) : MemoryManagement(pool_size_)
    {
        memcpy(memory_pool, other.memory_pool, other.pool_size*sizeof(T));
        use_pool_mask.assign(other.use_pool_mask.front(), other.use_pool_mask.back());
        current_ptr += other.pool_size - 1;
    }

    ~MemoryManagement()
    {
        delete memory_pool;
        use_pool_mask.clear();
    }

    T * custom_malloc(size_t size) {
        if(1 > size) {
            return nullptr;
        }
        current_ptr += size;
        return current_ptr - size;
    }

    void custom_deallocate(T * ptr, size_t size)
    {
        auto shift_ptr = ptr - (T *)memory_pool;
        while (size > 0) {
            use_pool_mask[shift_ptr+size-1] = false;
            size--;
        }
    }

    bool can_hold(size_t need_size) {
        return pool_size_allow() >= need_size;
    }

    size_t pool_size_allow() {
        auto free = this->pool_size - (((unsigned long long)current_ptr - (unsigned long long)memory_pool)/sizeof(T));
        if (free<0) {
            throw std::runtime_error("memory corrupting");
        }
        return free;
    }
};

template<typename T, size_t PoolSize = 2>
class CustomAllocator
{
public:
    // Needed for std::allocator_traits
    using value_type = T;
private:
    std::vector<std::unique_ptr<MemoryManagement<T>>> managers;
    size_t pool_size = PoolSize;

public:
    explicit CustomAllocator(size_t size) : pool_size(size)
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
        if (!managers.back()->can_hold(size)) {
            pool_size *= 2;
            managers.push_back(std::make_unique<MemoryManagement<T>>(pool_size));
        }
        T * ptr = static_cast<T *>(managers.back()->custom_malloc(size));
        if (ptr == NULL && size > 0) {
            throw std::bad_alloc();
        }
        return ptr;
    }

    // Needed for std::allocator_traits
    void deallocate(T * ptr, size_t size)
    {
        managers.back()->custom_deallocate(ptr, size);
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