//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <memory>

// template <class InputIt, class Size, class ForwardIt>
// pair<InputIt, ForwardIt> uninitialized_relocate_n(InputIt, Size, ForwardIt);

#include <memory>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

struct Counted {
    static int count;
    static int constructed;
    static void reset() { count = constructed = 0; }
    explicit Counted(int&& x) : value(x) { x = 0; ++count; ++constructed; }
    Counted(Counted const&) { assert(false); }
    ~Counted() { assert(count > 0); --count; }
    friend void operator&(Counted) = delete;
    int value;
};
int Counted::count = 0;
int Counted::constructed = 0;

struct ThrowsCounted {
    static int count;
    static int constructed;
    static int throw_after;
    static void reset() { throw_after = count = constructed = 0; }
    explicit ThrowsCounted(int&& x) {
        ++constructed;
        if (throw_after > 0 && --throw_after == 0) {
            TEST_THROW(1);
        }
        ++count;
        x = 0;
    }
    ThrowsCounted(ThrowsCounted const&) { assert(false); }
    ~ThrowsCounted() { assert(count > 0); --count; }
    friend void operator&(ThrowsCounted) = delete;
};
int ThrowsCounted::count = 0;
int ThrowsCounted::constructed = 0;
int ThrowsCounted::throw_after = 0;

void test_ctor_throws()
{
#ifndef TEST_HAS_NO_EXCEPTIONS
    using It = forward_iterator<ThrowsCounted*>;
    const int N = 5;
    int values[N] = {1, 2, 3, 4, 5};
    alignas(ThrowsCounted) char pool[sizeof(ThrowsCounted)*N] = {};
    ThrowsCounted* p = (ThrowsCounted*)pool;
    try {
        ThrowsCounted::throw_after = 4;
        std::uninitialized_relocate_n(values, N, It(p));
        assert(false);
    } catch (...) {}
    assert(ThrowsCounted::count == 0);  // the ones constructed have been destroyed automatically
    assert(ThrowsCounted::constructed == 4); // fourth construction throws
    assert(values[0] == 0);
    assert(values[1] == 0);
    assert(values[2] == 0);
    assert(values[3] == 4);
    assert(values[4] == 5);
#endif
}

void test_counted()
{
    using It = input_iterator<int*>;
    using FIt = forward_iterator<Counted*>;
    const int N = 5;
    int values[N] = {1, 2, 3, 4, 5};
    alignas(Counted) char pool[sizeof(Counted)*N] = {};
    Counted* p = (Counted*)pool;
    auto ret = std::uninitialized_relocate_n(It(values), 1, FIt(p));
    assert(ret.first == It(values+1));
    assert(ret.second == FIt(p + 1));
    assert(Counted::constructed == 1);
    assert(Counted::count == 1);
    assert(p[0].value == 1);
    assert(values[0] == 0);
    ret = std::uninitialized_relocate_n(It(values+1), N-1, FIt(p+1));
    assert(ret.first == It(values+N));
    assert(ret.second == FIt(p + N));
    assert(Counted::count == 5);
    assert(Counted::constructed == 5);
    assert(p[1].value == 2);
    assert(p[2].value == 3);
    assert(p[3].value == 4);
    assert(p[4].value == 5);
    assert(values[1] == 0);
    assert(values[2] == 0);
    assert(values[3] == 0);
    assert(values[4] == 0);
    std::destroy(p, p+N);
    assert(Counted::count == 0);
}

void test_unique_ptr_forward()
{
    using T = std::unique_ptr<int>;
    const int N = 5;
    alignas(T) char sourcepool[sizeof(T) * N];
    alignas(T) char destpool[sizeof(T) * N];
    T *source = (T*)sourcepool;
    T *dest = (T*)destpool;
    for (int i=0; i < N; ++i) {
        ::new ((void*)&source[i]) T(std::make_unique<int>(i));
    }
    auto result = std::uninitialized_relocate_n(source, 3, dest);
    assert(result.first == source + 3);
    assert(result.second == dest + 3);
    assert(*dest[0] == 0);
    assert(*dest[1] == 1);
    assert(*dest[2] == 2);
    assert(*source[3] == 3);
    assert(*source[4] == 4);
    std::destroy(dest, dest + 3);
    std::destroy(source + 3, source + N);
}

void test_unique_ptr_backward()
{
    using T = std::unique_ptr<int>;
    const int N = 5;
    alignas(T) char sourcepool[sizeof(T) * N];
    alignas(T) char destpool[sizeof(T) * N];
    T *source = (T*)sourcepool;
    T *dest = (T*)destpool;
    for (int i=0; i < N; ++i) {
        ::new ((void*)&source[i]) T(std::make_unique<int>(i));
    }
    auto result = std::uninitialized_relocate_n(
        std::make_reverse_iterator(source + 3),
        3, dest);
    assert(result.first == std::make_reverse_iterator(source));
    assert(result.second == dest + 3);
    assert(*dest[0] == 2);
    assert(*dest[1] == 1);
    assert(*dest[2] == 0);
    assert(*source[3] == 3);
    assert(*source[4] == 4);
    std::destroy(dest, dest + 3);
    std::destroy(source + 3, source + N);
}

int main() {
    test_counted();
    test_ctor_throws();
    test_unique_ptr_forward();
    test_unique_ptr_backward();
}
