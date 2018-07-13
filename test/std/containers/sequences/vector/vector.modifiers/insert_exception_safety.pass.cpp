//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// void insert(const_iterator it, const value_type& x);

#include <vector>
#include <cassert>

static int countdown = 0;
static int constructions = 0;
static int destructions = 0;

struct MyError {};

template<bool TR, bool CC, bool MC, bool CA, bool MA>
struct _LIBCPP_TRIVIALLY_RELOCATABLE_IF(TR) MyClass {
    int value;
    explicit MyClass(int i) : value(i) { ++constructions; }
    MyClass(const MyClass& rhs) _NOEXCEPT_(!CC) {
        value = rhs.value;
        if (CC && countdown-- == 0) throw MyError();
        ++constructions;
    }
    MyClass(MyClass&& rhs) _NOEXCEPT_(!MC) {
        value = rhs.value;
        if (MC && countdown-- == 0) throw MyError();
        ++constructions;
    }
    MyClass& operator=(const MyClass& rhs) _NOEXCEPT_(!CA) {
        value = rhs.value;
        if (CA && countdown-- == 0) throw MyError();
        return *this;
    }
    MyClass& operator=(MyClass&& rhs) _NOEXCEPT_(!MA) {
        value = rhs.value;
        if (MA && countdown-- == 0) throw MyError();
        return *this;
    }
    ~MyClass() { ++destructions; }
};

template<int Mask>
void test()
{
    using T = MyClass<!!(Mask & 1), !!(Mask & 2), !!(Mask & 4), !!(Mask & 8), !!(Mask & 16)>;
    for (int n = 0; true; ++n) {
        {
            std::vector<T> vec;
            vec.reserve(5);
            try {
                countdown = n;
                vec.push_back(T(1));
                vec.push_back(T(2));
                vec.push_back(T(3));
                vec.insert(vec.begin() + 2, T(4));
                vec.insert(vec.begin() + 2, T(5));
                // if we reach here, no exception was thrown
                break;
            } catch (const MyError&) {
                assert(constructions == destructions + static_cast<int>(vec.size()));
            }
        }
        // destroy the vector and check the invariant again
        assert(constructions == destructions);
    }
}

int main(int, char**)
{
    test<0x00>();
    test<0x01>();
    test<0x02>();
    test<0x03>();
    test<0x04>();
    test<0x05>();
    test<0x06>();
    test<0x07>();
    test<0x08>();
    test<0x09>();
    test<0x0a>();
    test<0x0b>();
    test<0x0c>();
    test<0x0d>();
    test<0x0e>();
    test<0x0f>();
    test<0x10>();
    test<0x11>();
    test<0x12>();
    test<0x13>();
    test<0x14>();
    test<0x15>();
    test<0x16>();
    test<0x17>();
    test<0x18>();
    test<0x19>();
    test<0x1a>();
    test<0x1b>();
    test<0x1c>();
    test<0x1d>();
    test<0x1e>();
    test<0x1f>();
    return 0;
}
