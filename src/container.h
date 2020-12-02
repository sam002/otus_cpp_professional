#pragma once

#include <cstdint>
#include <string>
#include <vector>

template<typename T>
struct element {
    element *next = nullptr;
    T data;
};

template<typename T, class Allocator = std::allocator<element<T>>>
class container {
    template<typename U>
    class _iterator : public std::iterator<std::input_iterator_tag, U> {
    public:
        _iterator(U *first) {
            current = first;
        }

        _iterator &operator++() {
            current = current->next;
            return *this;
        }

        bool operator!=(const _iterator &it) {
            return current != it.current;
        }

        U &operator*() {
            return *current;
        }

    private:
        U *current;
    };

    Allocator alloc;
    size_t count = 0;
    element<T> *front_el = nullptr;
    element<T> *end_el = nullptr;

public:

    using iterator = _iterator<element<T>>;

    container(std::initializer_list<T> values) {
        for (auto value : values)
            push(value);
    }

    ~container() {
        while (count)
            pop();
    }

    void push(T data) {
        element<T> **ptr = count ? &end_el->next : &front_el;
        *ptr = alloc.allocate(1);
        (*ptr)->data = data;
        end_el = *ptr;

        count++;
    }

    void pop() {
        if (!count)
            return;

        count--;

        auto next_el = front_el->next;

        alloc.deallocate(front_el, 1);

        front_el = next_el;
    }

    void clear() {
        while (count) {
            pop();
        }
    }

    T front() {
        if (count) {
            return front_el->data;
        }
    }

    T back() {
        if (count) {
            return end_el->data;
        }
    }

    iterator begin() {
        return iterator(front_el);
    }

    iterator end() {
        return iterator(end_el->next);
    }

    size_t size() {
        return count;
    }
};