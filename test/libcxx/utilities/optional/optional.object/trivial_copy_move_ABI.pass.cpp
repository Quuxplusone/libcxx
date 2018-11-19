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

// <optional>

// template <class T> class optional

// Test that we properly provide the trivial copy operations by default.

// FreeBSD provides the old ABI. This test checks the new ABI so we need
// to manually turn it on.
#if defined(__FreeBSD__)
#define _LIBCPP_ABI_UNSTABLE
#endif

#include <optional>
#include <type_traits>
#include <utility>
#include <cstdlib>
#include <cassert>

#if !__has_extension(trivially_relocatable)
#error "oops"
#endif

#include "test_macros.h"

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

struct [[clang::trivially_relocatable]] TrivialRelocEmpty {
    TrivialRelocEmpty(const TrivialRelocEmpty&);
    TrivialRelocEmpty(TrivialRelocEmpty&&);
    ~TrivialRelocEmpty();
};
static_assert(std::is_trivially_relocatable<TrivialRelocEmpty>::value, "");

struct [[clang::trivially_relocatable]] TrivialReloc {
    int i;
    TrivialReloc(const TrivialReloc&);
    TrivialReloc(TrivialReloc&&);
    ~TrivialReloc();
};
static_assert(std::is_trivially_relocatable<TrivialReloc>::value, "");

int main()
{
    {
        typedef std::optional<short> P;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
#if TEST_STD_VER >= 11
    {
        typedef std::optional<short> P;
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::optional<NonTrivialDtor>;
        static_assert(!std::is_trivially_destructible<P>::value, "");
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(!std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::optional<NonTrivialCopy>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(!std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::optional<NonTrivialMove>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(!std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::optional<DeletedCopy>;
        static_assert(!std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(std::is_trivially_move_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::optional<Trivial>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(std::is_trivially_move_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::optional<TrivialMove>;
        static_assert(!std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(std::is_trivially_move_constructible<P>::value, "");
        static_assert(HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::optional<TrivialReloc>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
    {
        using P = std::optional<TrivialRelocEmpty>;
        static_assert(std::is_copy_constructible<P>::value, "");
        static_assert(!std::is_trivially_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
        static_assert(!std::is_trivially_move_constructible<P>::value, "");
        static_assert(!HasTrivialABI<P>::value, "");
        static_assert(std::is_trivially_relocatable<P>::value, "");
    }
#endif
}
