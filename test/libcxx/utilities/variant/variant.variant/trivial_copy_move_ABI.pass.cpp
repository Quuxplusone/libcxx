//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// The test fails due to the missing is_trivially_constructible intrinsic.
// XFAIL: gcc-4.9
// UNSUPPORTED: c++03, c++11, c++14

// <variant>

// template <class... Ts> class variant

// Test that we properly provide the trivial copy operations by default.

// FreeBSD provides the old ABI. This test checks the new ABI so we need
// to manually turn it on.
#if defined(__FreeBSD__)
#define _LIBCPP_ABI_UNSTABLE
#endif

#include <type_traits>
#include <utility>
#include <variant>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"

// Define the P1144 name for convenience.
#define is_trivially_relocatable __libcpp_is_trivially_relocatable


#if defined(_LIBCPP_DEPRECATED_ABI_DISABLE_PAIR_TRIVIAL_COPY_CTOR)
#error Non-trivial ctor ABI macro defined
#endif

template <class T>
struct HasTrivialABI : std::integral_constant<bool,
    std::is_trivially_destructible<T>::value
    && (!std::is_copy_constructible<T>::value || std::is_trivially_copy_constructible<T>::value)
#if TEST_STD_VER >= 11
   && (!std::is_move_constructible<T>::value || std::is_trivially_move_constructible<T>::value)
#endif
> {};

#if TEST_STD_VER >= 11
struct NonTrivialDtor {
    NonTrivialDtor(NonTrivialDtor const&) = default;
    ~NonTrivialDtor();
};
NonTrivialDtor::~NonTrivialDtor() {}
static_assert(!HasTrivialABI<NonTrivialDtor>::value, "");

struct NonTrivialCopy {
    NonTrivialCopy(NonTrivialCopy const&);
};
NonTrivialCopy::NonTrivialCopy(NonTrivialCopy const&) {}
static_assert(!HasTrivialABI<NonTrivialCopy>::value, "");

struct NonTrivialMove {
    NonTrivialMove(NonTrivialMove const&) = default;
    NonTrivialMove(NonTrivialMove&&);
};
NonTrivialMove::NonTrivialMove(NonTrivialMove&&) {}
static_assert(!HasTrivialABI<NonTrivialMove>::value, "");

struct DeletedCopy {
    DeletedCopy(DeletedCopy const&) = delete;
    DeletedCopy(DeletedCopy&&) = default;
};
static_assert(HasTrivialABI<DeletedCopy>::value, "");

struct TrivialMove {
  TrivialMove(TrivialMove &&) = default;
};
static_assert(HasTrivialABI<TrivialMove>::value, "");

struct Trivial {
    Trivial(Trivial const&) = default;
};
static_assert(HasTrivialABI<Trivial>::value, "");
#endif

struct _LIBCPP_TRIVIALLY_RELOCATABLE TrivialRelocEmpty {
    TrivialRelocEmpty(const TrivialRelocEmpty&);
    TrivialRelocEmpty(TrivialRelocEmpty&&);
    ~TrivialRelocEmpty();
};
#if __has_extension(trivially_relocatable)
static_assert(std::is_trivially_relocatable<TrivialRelocEmpty>::value, "");
#endif

struct _LIBCPP_TRIVIALLY_RELOCATABLE TrivialReloc {
    int i;
    TrivialReloc(const TrivialReloc&);
    TrivialReloc(TrivialReloc&&);
    ~TrivialReloc();
};
#if __has_extension(trivially_relocatable)
static_assert(std::is_trivially_relocatable<TrivialReloc>::value, "");
#endif

int main()
{
    {
        typedef std::variant<int, int, short> P;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
#if TEST_STD_VER >= 11
    {
        typedef std::variant<int, int, short> P;
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::variant<int, NonTrivialDtor, int>;
        static_assert(!std::is_trivially_destructible<P>::value, "");
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(!std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::variant<int, NonTrivialCopy, int>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(!std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::variant<int, NonTrivialMove, int>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(!std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::variant<int, DeletedCopy, int>;
        static_assert(!std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(std::is_trivially_move_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::variant<int, Trivial, int>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(std::is_trivially_move_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::variant<int, TrivialMove, int>;
        static_assert(!std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(std::is_trivially_move_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
#if __has_extension(trivially_relocatable)
    {
        using P = std::variant<int, TrivialReloc, int>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::variant<int, TrivialRelocEmpty, int>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
#endif
#endif
}
