//===------------------------ memory_resource.cpp -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "experimental/memory_resource"

#ifndef _LIBCPP_HAS_NO_ATOMIC_HEADER
#include "atomic"
#elif !defined(_LIBCPP_HAS_NO_THREADS)
#include "mutex"
#endif

_LIBCPP_BEGIN_NAMESPACE_LFTS_PMR

// memory_resource

//memory_resource::~memory_resource() {}

// new_delete_resource()

class _LIBCPP_TYPE_VIS __new_delete_memory_resource_imp
    : public memory_resource
{
    void *do_allocate(size_t size, size_t align) override {
#ifdef _LIBCPP_HAS_NO_ALIGNED_ALLOCATION
        if (__is_overaligned_for_new(align))
            __throw_bad_alloc();
#endif
        return _VSTD::__libcpp_allocate(size, align);
    }

    void do_deallocate(void *p, size_t n, size_t align) override {
      _VSTD::__libcpp_deallocate(p, n, align);
    }

    bool do_is_equal(memory_resource const & other) const _NOEXCEPT override
        { return &other == this; }

public:
    ~__new_delete_memory_resource_imp() override = default;
};

// null_memory_resource()

class _LIBCPP_TYPE_VIS __null_memory_resource_imp
    : public memory_resource
{
public:
    ~__null_memory_resource_imp() = default;

protected:
    virtual void* do_allocate(size_t, size_t) {
        __throw_bad_alloc();
    }
    virtual void do_deallocate(void *, size_t, size_t) {}
    virtual bool do_is_equal(memory_resource const & __other) const _NOEXCEPT
    { return &__other == this; }
};

namespace {

union ResourceInitHelper {
  struct {
    __new_delete_memory_resource_imp new_delete_res;
    __null_memory_resource_imp       null_res;
  } resources;
  char dummy;
  _LIBCPP_CONSTEXPR_AFTER_CXX11 ResourceInitHelper() : resources() {}
  ~ResourceInitHelper() {}
};

// Detect if the init_priority attribute is supported.
#if (defined(_LIBCPP_COMPILER_GCC) && defined(__APPLE__)) \
  || defined(_LIBCPP_COMPILER_MSVC)
// GCC on Apple doesn't support the init priority attribute,
// and MSVC doesn't support any GCC attributes.
# define _LIBCPP_INIT_PRIORITY_MAX
#else
# define _LIBCPP_INIT_PRIORITY_MAX __attribute__((init_priority(101)))
#endif

// When compiled in C++14 this initialization should be a constant expression.
// Only in C++11 is "init_priority" needed to ensure initialization order.
#if _LIBCPP_STD_VER > 11
_LIBCPP_SAFE_STATIC
#endif
ResourceInitHelper res_init _LIBCPP_INIT_PRIORITY_MAX;

} // end namespace


memory_resource * new_delete_resource() _NOEXCEPT {
    return &res_init.resources.new_delete_res;
}

memory_resource * null_memory_resource() _NOEXCEPT {
    return &res_init.resources.null_res;
}

// default_memory_resource()

static memory_resource *
__default_memory_resource(bool set = false, memory_resource * new_res = nullptr) _NOEXCEPT
{
#ifndef _LIBCPP_HAS_NO_ATOMIC_HEADER
    _LIBCPP_SAFE_STATIC static atomic<memory_resource*> __res =
        ATOMIC_VAR_INIT(&res_init.resources.new_delete_res);
    if (set) {
        new_res = new_res ? new_res : new_delete_resource();
        // TODO: Can a weaker ordering be used?
        return _VSTD::atomic_exchange_explicit(
            &__res, new_res, memory_order::memory_order_acq_rel);
    }
    else {
        return _VSTD::atomic_load_explicit(
            &__res, memory_order::memory_order_acquire);
    }
#elif !defined(_LIBCPP_HAS_NO_THREADS)
    _LIBCPP_SAFE_STATIC static memory_resource * res = &res_init.resources.new_delete_res;
    static mutex res_lock;
    if (set) {
        new_res = new_res ? new_res : new_delete_resource();
        lock_guard<mutex> guard(res_lock);
        memory_resource * old_res = res;
        res = new_res;
        return old_res;
    } else {
        lock_guard<mutex> guard(res_lock);
        return res;
    }
#else
    _LIBCPP_SAFE_STATIC static memory_resource* res = &res_init.resources.new_delete_res;
    if (set) {
        new_res = new_res ? new_res : new_delete_resource();
        memory_resource * old_res = res;
        res = new_res;
        return old_res;
    } else {
        return res;
    }
#endif
}

memory_resource * get_default_resource() _NOEXCEPT
{
    return __default_memory_resource();
}

memory_resource * set_default_resource(memory_resource * __new_res) _NOEXCEPT
{
    return __default_memory_resource(true, __new_res);
}

// 23.12.6, mem.res.monotonic.buffer

static size_t roundup(size_t count, size_t alignment)
{
    size_t mask = alignment - 1;
    return (count + mask) & ~mask;
}

void *monotonic_buffer_resource::__initial_header::__try_allocate_from_chunk(
    size_t bytes, size_t align)
{
    if (!__cur_)
        return nullptr;
    void *new_ptr = static_cast<void*>(__cur_);
    size_t new_capacity = (__end_ - __cur_);
    void *aligned_ptr = _VSTD::align(align, bytes, new_ptr, new_capacity);
    if (aligned_ptr != nullptr)
        __cur_ = static_cast<char*>(new_ptr) + bytes;
    return aligned_ptr;
}

void *monotonic_buffer_resource::__chunk_header::__try_allocate_from_chunk(
    size_t bytes, size_t align)
{
    void *new_ptr = static_cast<void*>(__cur_);
    size_t new_capacity = (reinterpret_cast<char*>(this) - __cur_);
    void *aligned_ptr = _VSTD::align(align, bytes, new_ptr, new_capacity);
    if (aligned_ptr != nullptr)
        __cur_ = static_cast<char*>(new_ptr) + bytes;
    return aligned_ptr;
}

void* monotonic_buffer_resource::do_allocate(size_t bytes, size_t align)
{
    const size_t header_size = sizeof(__chunk_header);
    const size_t header_align = alignof(__chunk_header);

    auto previous_allocation_size = [&]() {
        if (__chunks_ != nullptr)
            return __chunks_->__allocation_size();

        size_t newsize = (__initial_.__start_ != nullptr) ?
            (__initial_.__end_ - __initial_.__start_) : __initial_.__size_;

        return roundup(newsize, header_align) + header_size;
    };

    if (void *result = __initial_.__try_allocate_from_chunk(bytes, align))
        return result;
    if (__chunks_ != nullptr) {
        if (void *result = __chunks_->__try_allocate_from_chunk(bytes, align))
            return result;
    }

    // Allocate a brand-new chunk.

    if (align < header_align)
        align = header_align;

    size_t aligned_capacity = roundup(bytes, header_align) + header_size;
    size_t previous_capacity = previous_allocation_size();

    if (aligned_capacity <= previous_capacity) {
        size_t newsize = 2 * (previous_capacity - header_size);
        aligned_capacity = roundup(newsize, header_align) + header_size;
    }

    char *start = (char *)__res_->allocate(aligned_capacity, align);
    __chunk_header *header =
        (__chunk_header *)(start + aligned_capacity - header_size);
    header->__next_ = __chunks_;
    header->__start_ = start;
    header->__cur_ = start;
    header->__align_ = align;
    __chunks_ = header;

    return __chunks_->__try_allocate_from_chunk(bytes, align);
}

_LIBCPP_END_NAMESPACE_LFTS_PMR
