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

static size_t roundup(size_t count, size_t alignment)
{
    size_t mask = alignment - 1;
    return (count + mask) & ~mask;
}

struct __pool_resource_adhoc_pool_header {
    size_t bytes;
    size_t alignment;
    void *allocation;
    __pool_resource_adhoc_pool_header *next;
};

void __pool_resource_adhoc_pool::release(memory_resource *upstream)
{
    while (__first_ != nullptr) {
        __header *next = __first_->next;
        upstream->deallocate(__first_->allocation, __first_->bytes, __first_->alignment);
        __first_ = next;
    }
}

void *__pool_resource_adhoc_pool::do_allocate(memory_resource *upstream, size_t bytes, size_t align)
{
    const size_t header_size = sizeof(__header);
    const size_t header_align = alignof(__header);

    if (align < header_align) {
        align = header_align;
    }

    size_t aligned_capacity = roundup(bytes, header_align) + header_size;

    void *result = upstream->allocate(aligned_capacity, align);

    __header *h = (__header *)((char *)result + aligned_capacity - header_size);
    h->allocation = result;
    h->bytes = aligned_capacity;
    h->alignment = align;
    h->next = __first_;
    __first_ = h;
    return result;
}

void __pool_resource_adhoc_pool::do_deallocate(memory_resource *upstream, void *p, size_t bytes, size_t align)
{
    _LIBCPP_ASSERT(__first_ != nullptr, "deallocating a block that was not allocated with this allocator");
    if (__first_->allocation == p) {
        __header *next = __first_->next;
        upstream->deallocate(p, bytes, align);
        __first_ = next;
    } else {
        for (__header *h = __first_; h != nullptr; h = h->next) {
            if (h->next != nullptr && h->next->allocation == p) {
                __header *next = h->next->next;
                upstream->deallocate(p, bytes, align);
                h->next = next;
                return;
            }
        }
        _LIBCPP_ASSERT(false, "deallocating a block that was not allocated with this allocator");
    }
}

struct __pool_resource_vacancy_header {
    __pool_resource_vacancy_header *next_vacancy;
};

struct __pool_resource_fixed_pool_header {
    __pool_resource_vacancy_header *first_vacancy;
    size_t bytes;
    size_t alignment;
    void *allocation;
    __pool_resource_fixed_pool_header *next;

    bool allocation_contains(const char *p) const {
        // TODO: This part technically relies on undefined behavior.
        return allocation <= p && p < ((char*)allocation + bytes);
    }
};

class __pool_resource_fixed_pool {
    using __header = __pool_resource_fixed_pool_header;
    __header *__first_;

public:
    explicit __pool_resource_fixed_pool() : __first_(nullptr) {}
    void release(memory_resource *upstream);
    void *try_allocate_from_vacancies();
    void *do_allocate_with_new_chunk(memory_resource *upstream, size_t block_size);
    void do_evacuate(void *__p);

    static const size_t __default_alignment = alignof(max_align_t);
};

void __pool_resource_fixed_pool::release(memory_resource *upstream)
{
    while (__first_ != nullptr) {
        __header *next = __first_->next;
        upstream->deallocate(__first_->allocation, __first_->bytes, __first_->alignment);
        __first_ = next;
    }
}

void *__pool_resource_fixed_pool::try_allocate_from_vacancies()
{
    for (__header *h = __first_; h != nullptr; h = h->next) {
        if (h->first_vacancy != nullptr) {
            void *result = h->first_vacancy;
            h->first_vacancy = h->first_vacancy->next_vacancy;
            return result;
        }
    }
    return nullptr;
}

void *__pool_resource_fixed_pool::do_allocate_with_new_chunk(memory_resource *upstream, size_t block_size)
{
    static_assert(__default_alignment >= alignof(std::max_align_t), "");
    static_assert(__default_alignment >= alignof(__header), "");
    static_assert(__default_alignment >= alignof(__pool_resource_vacancy_header), "");

    const size_t header_size = sizeof(__header);

    size_t aligned_capacity = roundup(block_size, alignof(__header)) + header_size;

    void *result = upstream->allocate(aligned_capacity, __default_alignment);

    __header *h = (__header *)((char *)result + aligned_capacity - header_size);
    h->allocation = result;
    h->bytes = aligned_capacity;
    h->alignment = __default_alignment;
    h->next = __first_;
    h->first_vacancy = nullptr;
    __first_ = h;
    return result;
}

void __pool_resource_fixed_pool::do_evacuate(void *p)
{
    _LIBCPP_ASSERT(__first_ != nullptr, "deallocating a block that was not allocated with this allocator");
    for (__header *h = __first_; h != nullptr; h = h->next) {
        if (h->allocation_contains((char*)p)) {
            __pool_resource_vacancy_header *v = (__pool_resource_vacancy_header *)(p);
            v->next_vacancy = h->first_vacancy;
            h->first_vacancy = v;
            return;
        }
    }
    _LIBCPP_ASSERT(false, "deallocating a block that was not allocated with this allocator");
}

size_t unsynchronized_pool_resource::__pool_block_size(int i) const
{
    return size_t(1) << (i + __log2_smallest_block_size);
}

int unsynchronized_pool_resource::__pool_index(size_t bytes, size_t align) const
{
    if (align > alignof(std::max_align_t) || bytes > (1 << __num_fixed_pools_)) {
        return __num_fixed_pools_;
    } else if (bytes <= __smallest_block_size) {
        return 0;
    } else {
        int i = 0;
        bytes -= 1;
        bytes >>= __log2_smallest_block_size;
        while (bytes != 0) {
            bytes >>= 1;
            i += 1;
        }
        return i;
    }
}

unsynchronized_pool_resource::unsynchronized_pool_resource(const pool_options& opts, memory_resource* upstream)
    : __res_(upstream), __fixed_pools_(nullptr), __options_(opts)
{
    if (__options_.largest_required_pool_block == 0) {
        __options_.largest_required_pool_block = (size_t(1) << 20);
    } else if (__options_.largest_required_pool_block < __smallest_block_size) {
        __options_.largest_required_pool_block = __smallest_block_size;
    } else if (__options_.largest_required_pool_block > (size_t(1) << 30)) {
        __options_.largest_required_pool_block = (size_t(1) << 30);
    }
    __num_fixed_pools_ = 1;
    size_t capacity = __smallest_block_size;
    while (capacity < __options_.largest_required_pool_block) {
        capacity <<= 1;
        __num_fixed_pools_ += 1;
    }

    __options_.max_blocks_per_chunk = 0;
}

unsynchronized_pool_resource::~unsynchronized_pool_resource()
{
    release();
}

void unsynchronized_pool_resource::release()
{
    __adhoc_pool_.release(__res_);
    if (__fixed_pools_ != nullptr) {
        const int n = __num_fixed_pools_;
        for (int i=0; i < n; ++i) {
            __fixed_pools_[i].release(__res_);
        }
        __res_->deallocate(__fixed_pools_, __num_fixed_pools_ * sizeof(__pool_resource_fixed_pool), alignof(__pool_resource_fixed_pool));
        __fixed_pools_ = nullptr;
    }
}

void* unsynchronized_pool_resource::do_allocate(size_t bytes, size_t align)
{
    // A pointer to allocated storage (6.6.4.4.1) with a size of at least bytes.
    // The size and alignment of the allocated memory shall meet the requirements for
    // a class derived from memory_resource (23.12).
    // If the pool selected for a block of size bytes is unable to satisfy the memory request
    // from its own internal data structures, it will call upstream_resource()->allocate()
    // to obtain more memory. If bytes is larger than that which the largest pool can handle,
    // then memory will be allocated using upstream_resource()->allocate().

    int i = __pool_index(bytes, align);
    if (i == __num_fixed_pools_) {
        return __adhoc_pool_.do_allocate(__res_, bytes, align);
    } else {
        if (__fixed_pools_ == nullptr) {
            using P = __pool_resource_fixed_pool;
            __fixed_pools_ = (P*)__res_->allocate(__num_fixed_pools_ * sizeof(P), alignof(P));
            P *first = __fixed_pools_;
            P *last = __fixed_pools_ + __num_fixed_pools_;
            for (P *pool = first; pool != last; ++pool) {
                ::new((void*)pool) P;
            }
        }
        void *result = __fixed_pools_[i].try_allocate_from_vacancies();
        if (result == nullptr) {
            result = __fixed_pools_[i].do_allocate_with_new_chunk(__res_, __pool_block_size(i));
        }
        return result;
    }
}

void unsynchronized_pool_resource::do_deallocate(void* p, size_t bytes, size_t align)
{
    // Returns the memory at p to the pool. It is unspecified if, or under what circumstances,
    // this operation will result in a call to upstream_resource()->deallocate().

    int i = __pool_index(bytes, align);
    if (i == __num_fixed_pools_) {
        return __adhoc_pool_.do_deallocate(__res_, p, bytes, align);
    } else {
        _LIBCPP_ASSERT(__fixed_pools_ != nullptr, "deallocating a block that was not allocated with this allocator");
        __fixed_pools_[i].do_evacuate(p);
    }
}

// 23.12.6, mem.res.monotonic.buffer

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
