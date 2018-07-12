//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_trivially_move_constructible

// REQUIRES: c++11
// XFAIL: gcc-4.9

#include <memory>
#include <type_traits>
#include "test_macros.h"

template <class T>
void test_is_trivially_relocatable()
{
    static_assert( std::is_trivially_relocatable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_trivially_relocatable_v<T>, "");
#endif
}

template <class T>
void test_is_not_trivially_relocatable()
{
    static_assert(!std::is_trivially_relocatable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_trivially_relocatable_v<T>, "");
#endif
}

class Empty
{
};

class NotEmpty
{
public:
    virtual ~NotEmpty();
};

union Union {};

struct bit_zero
{
    int :  0;
};

class Abstract
{
public:
    virtual ~Abstract() = 0;
};

struct A
{
    A(const A&);
};

#if TEST_STD_VER >= 11

struct MoveOnly1
{
    MoveOnly1(MoveOnly1&&);
};

struct MoveOnly2
{
    MoveOnly2(MoveOnly2&&) = default;
};

struct [[clang::trivially_relocatable]] Widget
{
    std::shared_ptr<int> p;
};

#endif

int main()
{
    test_is_not_trivially_relocatable<void>();
    test_is_not_trivially_relocatable<A>();
    test_is_not_trivially_relocatable<Abstract>();
    test_is_not_trivially_relocatable<NotEmpty>();

    test_is_trivially_relocatable<Union>();
    test_is_trivially_relocatable<Empty>();
    test_is_trivially_relocatable<int>();
    test_is_trivially_relocatable<double>();
    test_is_trivially_relocatable<int*>();
    test_is_trivially_relocatable<const int*>();
    test_is_trivially_relocatable<bit_zero>();

    test_is_trivially_relocatable<const int>();
    test_is_trivially_relocatable<Widget>();
    test_is_trivially_relocatable<const Widget>();

#if TEST_STD_VER >= 11
    static_assert(!std::is_trivially_relocatable<MoveOnly1>::value, "");
    static_assert( std::is_trivially_relocatable<MoveOnly2>::value, "");
#endif
}
