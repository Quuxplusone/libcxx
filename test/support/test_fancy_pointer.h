//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef TEST_FANCY_POINTER_H
#define TEST_FANCY_POINTER_H

#include <type_traits>
#include <new>
#include <memory>
#include <utility>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <cassert>

#include "test_macros.h"

#pragma once

#include <cstddef>


template<class T, class CRTP>
struct fancy_ptr_base {
    constexpr T *ptr() const noexcept { return m_ptr; }
    constexpr explicit operator T*() const noexcept { return ptr(); }
    constexpr explicit operator bool() const noexcept { return ptr() != nullptr; }
    constexpr bool operator==(CRTP b) const { return ptr() == b.ptr(); }
    constexpr bool operator!=(CRTP b) const { return ptr() != b.ptr(); }
    constexpr bool operator==(decltype(nullptr)) const { return ptr() == nullptr; }
    constexpr bool operator!=(decltype(nullptr)) const { return ptr() != nullptr; }
    constexpr bool operator<(CRTP b) const { return ptr() < b.ptr(); }
    constexpr bool operator<=(CRTP b) const { return ptr() <= b.ptr(); }
    constexpr bool operator>(CRTP b) const { return ptr() > b.ptr(); }
    constexpr bool operator>=(CRTP b) const { return ptr() >= b.ptr(); }
    constexpr T& operator*() const noexcept { return *ptr(); }
    constexpr T* operator->() const noexcept { return ptr(); }
    constexpr CRTP& operator+=(ptrdiff_t i) { m_ptr += i; return as_crtp(); }
    constexpr CRTP& operator-=(ptrdiff_t i) { m_ptr -= i; return as_crtp(); }
    constexpr CRTP& operator++() { ++m_ptr; return as_crtp(); }
    constexpr CRTP& operator--() { --m_ptr; return as_crtp(); }
    constexpr CRTP operator++(int) { auto r(as_crtp()); ++*this; return r; }
    constexpr CRTP operator--(int) { auto r(as_crtp()); --*this; return r; }
    constexpr CRTP operator+(ptrdiff_t i) const { auto r(as_crtp()); r += i; return r; }
    constexpr CRTP operator-(ptrdiff_t i) const { auto r(as_crtp()); r -= i; return r; }
    constexpr ptrdiff_t operator-(CRTP b) const { return ptr() - b.ptr(); }

protected:
    T *m_ptr = nullptr;
private:
    constexpr CRTP& as_crtp() { return *static_cast<CRTP*>(this); }
    constexpr const CRTP& as_crtp() const { return *static_cast<const CRTP*>(this); }
};

template<class CRTP>
struct fancy_ptr_base<void, CRTP> {
    constexpr void *ptr() const noexcept { return m_ptr; }
    constexpr explicit operator void*() const noexcept { return ptr(); }
    constexpr explicit operator bool() const noexcept { return ptr() != nullptr; }
    constexpr bool operator==(CRTP b) const { return ptr() == b.ptr(); }
    constexpr bool operator!=(CRTP b) const { return ptr() != b.ptr(); }
    constexpr bool operator==(decltype(nullptr)) const { return ptr() == nullptr; }
    constexpr bool operator!=(decltype(nullptr)) const { return ptr() != nullptr; }
    constexpr bool operator<(CRTP b) const { return ptr() < b.ptr(); }
    constexpr bool operator<=(CRTP b) const { return ptr() <= b.ptr(); }
    constexpr bool operator>(CRTP b) const { return ptr() > b.ptr(); }
    constexpr bool operator>=(CRTP b) const { return ptr() >= b.ptr(); }
protected:
    void *m_ptr = nullptr;
};

template<class T>
class test_fancyptr : public fancy_ptr_base<T, test_fancyptr<T>> {
    size_t m_payload = 0;
public:
    test_fancyptr() = default;
    test_fancyptr(decltype(nullptr)) {}

    template<
        class U,
        typename std::enable_if<std::is_convertible<U*, T*>::value, bool>::type = true
    >
    test_fancyptr(const test_fancyptr<U>& rhs) :
        test_fancyptr(rhs.ptr(), rhs.payload()) {}

    template<
        class U,
        typename std::enable_if<!std::is_convertible<U*, T*>::value, bool>::type = true,
        decltype(static_cast<T *>(std::declval<U *>())) = nullptr
    >
    explicit test_fancyptr(const test_fancyptr<U>& rhs) :
        test_fancyptr(static_cast<T *>(rhs.ptr()), rhs.payload()) {}

    explicit test_fancyptr(T *p, size_t n) : m_payload(n) { this->m_ptr = p; }

    size_t payload() const { return m_payload; }

    template<bool B = !std::is_void<T>::value, typename std::enable_if<B, bool>::type = true>
    static test_fancyptr pointer_to(typename std::conditional<std::is_void<T>::value, int, T>::type & p) {
        return test_fancyptr(&p, -1);
    }
};

#endif  // TEST_FANCY_POINTER_H
