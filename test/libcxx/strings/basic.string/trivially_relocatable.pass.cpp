//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <string>

#include <memory>
#include <string>
#include <type_traits>

// Define the P1144 name for convenience.
#define is_trivially_relocatable __libcpp_is_trivially_relocatable


namespace fa {
    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            using other = FakeAllocator<U>;
        };
        template<class U>
        FakeAllocator(const FakeAllocator<U>&);
        // The defaulted (trivial) copy constructor is still present here!
    };
    static_assert(std::is_trivially_copy_constructible<FakeAllocator<int>>::value, "");
    static_assert(std::is_trivially_relocatable<FakeAllocator<int>>::value, "");

    using string = std::basic_string<char, std::char_traits<char>, FakeAllocator<char>>;
    using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, FakeAllocator<wchar_t>>;
}

namespace fa2 {
    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            using other = FakeAllocator<U>;
        };
        template<class U>
        FakeAllocator(const FakeAllocator<U>&);
        FakeAllocator(const FakeAllocator&);
    };
    static_assert(!std::is_trivially_copy_constructible<FakeAllocator<int>>::value, "");
    static_assert(!std::is_trivially_relocatable<FakeAllocator<int>>::value, "");

    using string = std::basic_string<char, std::char_traits<char>, FakeAllocator<char>>;
    using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, FakeAllocator<wchar_t>>;
}

namespace fa3 {
    template<class T>
    struct FakePointer {
        int offset;
        FakePointer();
        FakePointer(const FakePointer&) = default;
        FakePointer& operator=(const FakePointer&) = default;
        T& operator*() const;
        T* operator->() const;
        FakePointer operator + (unsigned int) const;
        FakePointer& operator += (unsigned int);
        FakePointer operator ++(int);
        FakePointer& operator ++();
        FakePointer operator --(int);
        FakePointer& operator --();
    };
    static_assert(std::is_trivially_copy_constructible<FakePointer<int>>::value, "");
    static_assert(std::is_trivially_relocatable<FakePointer<int>>::value, "");

    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            using other = FakeAllocator<U>;
        };
        using pointer = FakePointer<T>;
    };

    using string = std::basic_string<char, std::char_traits<char>, FakeAllocator<char>>;
    using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, FakeAllocator<wchar_t>>;
}

namespace fa4 {
    template<class T>
    struct FakePointer {
        int offset;
        FakePointer();
        FakePointer(const FakePointer&);
        FakePointer& operator=(const FakePointer&);
        T& operator*() const;
        T* operator->() const;
        FakePointer operator + (unsigned int) const;
        FakePointer& operator += (unsigned int);
        FakePointer operator ++(int);
        FakePointer& operator ++();
        FakePointer operator --(int);
        FakePointer& operator --();
    };
    static_assert(!std::is_trivially_copy_constructible<FakePointer<int>>::value, "");
    static_assert(!std::is_trivially_relocatable<FakePointer<int>>::value, "");

    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            using other = FakeAllocator<U>;
        };
        using pointer = FakePointer<T>;
    };

    using string = std::basic_string<char, std::char_traits<char>, FakeAllocator<char>>;
    using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, FakeAllocator<wchar_t>>;
}


static_assert(std::is_trivially_relocatable< std::allocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa2::FakeAllocator<int> >::value, "");
static_assert(std::is_trivially_relocatable< fa4::FakeAllocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa4::FakePointer<int> >::value, "");

template<class VT, bool Expected>
void check_trivial_relocatability()
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    // Debugging adds significant operations to string's move-constructor and destructor.
    static_assert(std::is_trivially_relocatable< VT >::value == false, "");
#elif !__has_extension(trivially_relocatable)
    // Making string trivially relocatable requires using the attribute.
    static_assert(std::is_trivially_relocatable< VT >::value == false, "");
#else
    static_assert(std::is_trivially_relocatable< VT >::value == Expected, "");
#endif
}

int main()
{
    check_trivial_relocatability< std::string, true >();
    check_trivial_relocatability< std::wstring, true >();

    check_trivial_relocatability< fa::string, true >();
    check_trivial_relocatability< fa::wstring, true >();

    check_trivial_relocatability< fa2::string, false >();
    check_trivial_relocatability< fa2::wstring, false >();

    check_trivial_relocatability< fa3::string, true >();
    check_trivial_relocatability< fa3::wstring, true >();

    check_trivial_relocatability< fa4::string, false >();
    check_trivial_relocatability< fa4::wstring, false >();
}
