//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// void insert(const_iterator it, const value_type& x);

// UNSUPPORTED: c++98, c++03

#include <vector>
#include <cassert>

static int assignments = 0;

template<bool TR>
struct _LIBCPP_TRIVIALLY_RELOCATABLE_IF(TR) MyClass {
    int value;
    explicit MyClass(int i) : value(i) {}
    MyClass(const MyClass& rhs) {
        value = rhs.value;
    }
    MyClass(MyClass&& rhs) noexcept {
        value = rhs.value;
    }
    MyClass& operator=(const MyClass& rhs) {
        ++assignments;
        value = rhs.value;
        return *this;
    }
    MyClass& operator=(MyClass&& rhs) {
        ++assignments;
        value = rhs.value;
        return *this;
    }
    ~MyClass() {}
};

template<bool TR>
void test()
{
    using T = MyClass<TR>;
    std::vector<T> vec;
    vec.reserve(5);
    vec.push_back(T(1));
    vec.push_back(T(2));
    vec.push_back(T(3));

    assignments = 0;
    vec.insert(vec.begin() + 2, T(4));
    if (TR) {
        assert(assignments == 0);  // relocate upward without assigning
    } else {
        assert(assignments >= 1);
    }

    assignments = 0;
    vec.insert(vec.begin() + 2, T(5));
    if (TR) {
        assert(assignments == 0);  // relocate upward without assigning
    } else {
        assert(assignments >= 1);
    }
}

int main(int, char**)
{
    test<true>();
    test<false>();
    return 0;
}
