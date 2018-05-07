//===------------------------ memory_resource.cpp -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "memory_resource"

#ifndef _LIBCPP_HAS_NO_ATOMIC_HEADER
#include "atomic"
#endif
#include "memory"
#if !defined(_LIBCPP_HAS_NO_THREADS)
#include "mutex"
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

namespace pmr {

// memory_resource

// new_delete_resource()

class _LIBCPP_TYPE_VIS __new_delete_memory_resource_imp
    : public memory_resource
{
public:
    ~__new_delete_memory_resource_imp() = default;

protected:
    virtual void* do_allocate(size_t __size, size_t __align)
        { return _VSTD::__libcpp_allocate(__size, __align); /* FIXME */}

    virtual void do_deallocate(void * __p, size_t, size_t __align)
        { _VSTD::__libcpp_deallocate(__p, __align); /* FIXME */ }

    virtual bool do_is_equal(memory_resource const & __other) const _NOEXCEPT
        { return &__other == this; }
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
// When compiled in C++14 this initialization should be a constant expression.
// Only in C++11 is "init_priority" needed to ensure initialization order.
#if _LIBCPP_STD_VER > 11
_LIBCPP_SAFE_STATIC
#endif
ResourceInitHelper res_init  __attribute__((init_priority (101)));

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

// 23.12.5, mem.res.pool

template<class T>
struct __mr_holder {
    memory_resource *res;
    void *ptr;
    size_t size;
    size_t align;

    __mr_holder(memory_resource *res, size_t size, size_t align)
        : res(res), ptr(res->allocate(size, align)), size(size), align(align)
    {}

    T *release() {
        T *result = static_cast<T*>(ptr);
        ptr = nullptr;
        return result;
    }

    T *get() const {
        return static_cast<T*>(ptr);
    }

    T *operator->() const {
        return static_cast<T*>(ptr);
    }

    ~__mr_holder() {
        if (ptr) {
            res->deallocate(ptr, size, align);
        }
    }
};

struct __pool_resource_chunk {
    size_t bytes;
    size_t alignment;
    void *allocation;
    __pool_resource_chunk *next;
};

// 23.12.5.3, mem.res.pool.ctor

unsynchronized_pool_resource::unsynchronized_pool_resource(const pool_options& opts, memory_resource* upstream)
    : __res_(upstream), __chunk_(nullptr), __options_(opts)
{
    __options_.max_blocks_per_chunk = 0;
    __options_.largest_required_pool_block = 0;
}

unsynchronized_pool_resource::~unsynchronized_pool_resource()
{
    release();
}

// 23.12.5.4, mem.res.pool.mem

void unsynchronized_pool_resource::release()
{
    __pool_resource_chunk *next = nullptr;
    for (__pool_resource_chunk *chunk = __chunk_; chunk != nullptr; chunk = next) {
        next = chunk->next;
        if (chunk->allocation) {
            __res_->deallocate(chunk->allocation, chunk->bytes, chunk->alignment);
            chunk->allocation = nullptr;
        }
        __res_->deallocate(chunk, sizeof(__pool_resource_chunk), alignof(__pool_resource_chunk));
        __chunk_ = next;
    }
}

void* unsynchronized_pool_resource::do_allocate(size_t bytes, size_t align)
{
    __mr_holder<void> allocation(__res_, bytes, align);
    __mr_holder<__pool_resource_chunk> chunk(__res_, sizeof(__pool_resource_chunk), alignof(__pool_resource_chunk));
    chunk->allocation = allocation.release();
    chunk->bytes = bytes;
    chunk->alignment = align;
    chunk->next = __chunk_;
    __chunk_ = chunk.release();
    return __chunk_->allocation;

    // A pointer to allocated storage (6.6.4.4.1) with a size of at least bytes.
    // The size and alignment of the allocated memory shall meet the requirements for
    // a class derived from memory_resource (23.12).
    // If the pool selected for a block of size bytes is unable to satisfy the memory request
    // from its own internal data structures, it will call upstream_resource()->allocate()
    // to obtain more memory. If bytes is larger than that which the largest pool can handle,
    // then memory will be allocated using upstream_resource()->allocate().
}

void unsynchronized_pool_resource::do_deallocate(void* p, size_t bytes, size_t align)
{
    // Returns the memory at p to the pool. It is unspecified if, or under what circumstances,
    // this operation will result in a call to upstream_resource()->deallocate().

    for (__pool_resource_chunk **pchunk = &__chunk_; *pchunk != nullptr; pchunk = &(*pchunk)->next) {
        __pool_resource_chunk *chunk = *pchunk;
        if (chunk->allocation == p) {
            __res_->deallocate(p, bytes, align);
            chunk->allocation = nullptr;
            __pool_resource_chunk *next = chunk->next;
            __res_->deallocate(chunk, sizeof(__pool_resource_chunk), alignof(__pool_resource_chunk));
            *pchunk = next;
            break;
        }
    }
}

// 23.12.6, mem.res.monotonic.buffer

static size_t roundup(size_t count, size_t alignment)
{
    size_t mask = alignment - 1;
    return (count + mask) & ~mask;
}

static void *try_allocate_from_chunk(__monotonic_buffer_header *header, size_t bytes, size_t align)
{
    if (!header || !header->__start_) return nullptr;
    if (header->__capacity_ < bytes) return nullptr;
    void *new_ptr = static_cast<char *>(header->__start_) + header->__used_;
    size_t new_capacity = (header->__capacity_ - header->__used_);
    void *aligned_ptr = _VSTD::align(align, bytes, new_ptr, new_capacity);
    if (aligned_ptr == nullptr) {
        return nullptr;
    }
    header->__used_ = (header->__capacity_ - new_capacity) + bytes;
    return aligned_ptr;
}

monotonic_buffer_resource::monotonic_buffer_resource(void* buffer, size_t buffer_size, memory_resource* upstream)
    : __res_(upstream)
{
    __original_.__start_ = buffer;
    __original_.__next_ = nullptr;
    __original_.__capacity_ = buffer_size;
    __original_.__alignment_ = 1;
    __original_.__used_ = 0;
    __next_buffer_size_ = buffer_size >= 1 ? buffer_size : 1;
}

monotonic_buffer_resource::~monotonic_buffer_resource()
{
    release();
}

void monotonic_buffer_resource::release()
{
    const size_t header_size = sizeof(__monotonic_buffer_header);

    __original_.__used_ = 0;
    while (__original_.__next_ != nullptr) {
        __monotonic_buffer_header *header = __original_.__next_;
        __monotonic_buffer_header *next_header = header->__next_;
        size_t aligned_capacity = header->__capacity_ + header_size;
        __res_->deallocate(header->__start_, aligned_capacity, header->__alignment_);
        __original_.__next_ = next_header;
    }
}

void* monotonic_buffer_resource::do_allocate(size_t bytes, size_t align)
{
    if (void *result = try_allocate_from_chunk(&__original_, bytes, align)) {
        return result;
    }
    if (void *result = try_allocate_from_chunk(__original_.__next_, bytes, align)) {
        return result;
    }
    // Allocate a brand-new chunk.
    const size_t header_size = sizeof(__monotonic_buffer_header);
    const size_t header_align = alignof(__monotonic_buffer_header);

    if (align < header_align) {
        align = header_align;
    }

    size_t aligned_capacity = roundup(bytes, header_align) + header_size;

    if (aligned_capacity < __next_buffer_size_) {
        aligned_capacity = roundup(__next_buffer_size_ - header_size, header_align) + header_size;
    }

    void *result = __res_->allocate(aligned_capacity, align);
    __monotonic_buffer_header *header = (__monotonic_buffer_header *)((char *)result + aligned_capacity - header_size);
    header->__start_ = result;
    header->__capacity_ = aligned_capacity - header_size;
    header->__alignment_ = align;
    header->__used_ = 0;
    header->__next_ = __original_.__next_;
    __original_.__next_ = header;

    size_t prospective_next_buffer_size = (__next_buffer_size_ * 5) / 4;
    if (prospective_next_buffer_size <= __next_buffer_size_) {
        prospective_next_buffer_size = __next_buffer_size_ + 1;
    }
    __next_buffer_size_ = prospective_next_buffer_size;

    return try_allocate_from_chunk(__original_.__next_, bytes, align);
}


} // namespace pmr

_LIBCPP_END_NAMESPACE_STD
