//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++03
//
// <memory>
//
// class shared_ptr
// class unique_ptr
// class weak_ptr

#include <memory>

class Incomplete;
class NonTriv { ~NonTriv(); };
class _LIBCPP_TRIVIALLY_RELOCATABLE Reloc { ~Reloc(); };

static_assert(std::is_trivially_relocatable<int>::value, "");
static_assert(!std::is_trivially_relocatable<NonTriv>::value, "");
#if __has_extension(trivially_relocatable)
static_assert(std::is_trivially_relocatable<Reloc>::value, "");
#endif

#if __has_extension(trivially_relocatable)
static_assert(std::is_trivially_relocatable< std::shared_ptr<int> >::value, "");
static_assert(std::is_trivially_relocatable< std::shared_ptr<Incomplete> >::value, "");
static_assert(std::is_trivially_relocatable< std::shared_ptr<NonTriv> >::value, "");

static_assert(std::is_trivially_relocatable< std::weak_ptr<int> >::value, "");
static_assert(std::is_trivially_relocatable< std::weak_ptr<Incomplete> >::value, "");
static_assert(std::is_trivially_relocatable< std::weak_ptr<NonTriv> >::value, "");

static_assert(std::is_trivially_relocatable< std::default_delete<int> >::value, "");
static_assert(std::is_trivially_relocatable< std::unique_ptr<int> >::value, "");
static_assert(std::is_trivially_relocatable< std::unique_ptr<Incomplete> >::value, "");
static_assert(std::is_trivially_relocatable< std::unique_ptr<NonTriv> >::value, "");

static_assert(std::is_trivially_relocatable< std::default_delete<int[]> >::value, "");
static_assert(std::is_trivially_relocatable< std::unique_ptr<int[]> >::value, "");
static_assert(std::is_trivially_relocatable< std::unique_ptr<Incomplete[]> >::value, "");
static_assert(std::is_trivially_relocatable< std::unique_ptr<NonTriv[]> >::value, "");
#endif

int main() {}
