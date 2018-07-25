//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++03, c++11, c++14
//
// <type_traits>
//
// Test that many standard types are trivially relocatable.

#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <forward_list>
#include <functional>
#include <future>
#include <initializer_list>
#include <ios>
#include <iterator>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <memory_resource>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <set>
#include <shared_mutex>
#include <stack>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>


#if __has_extension(trivially_relocatable)
struct T { T(); T(const T&); ~T(); };
struct _LIBCPP_TRIVIALLY_RELOCATABLE R { R(); R(const R&); ~R(); };
struct SM { void lock_shared(); void unlock_shared(); ~SM(); };

static_assert(std::is_trivially_relocatable<R>::value, "");
static_assert(!std::is_trivially_relocatable<T>::value, "");
static_assert(std::is_trivially_relocatable<T*>::value, "");
static_assert(std::is_trivially_relocatable<std::reference_wrapper<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::initializer_list<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::allocator<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::pmr::polymorphic_allocator<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::default_delete<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::hash<std::string>>::value, "");
static_assert(std::is_trivially_relocatable<std::less<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::less<void>>::value, "");
static_assert(std::is_trivially_relocatable<std::unique_ptr<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::shared_ptr<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::weak_ptr<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::pair<R,R>>::value, "");
static_assert(!std::is_trivially_relocatable<std::pair<R,T>>::value, "");
static_assert(std::is_trivially_relocatable<std::tuple<R,R,R>>::value, "");
static_assert(!std::is_trivially_relocatable<std::tuple<R,T,R>>::value, "");
static_assert(std::is_trivially_relocatable<std::variant<R,R,R>>::value, "");
static_assert(!std::is_trivially_relocatable<std::variant<R,T,R>>::value, "");
static_assert(std::is_trivially_relocatable<std::optional<R>>::value, "");
static_assert(!std::is_trivially_relocatable<std::optional<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::any>::value, "");
static_assert(std::is_trivially_relocatable<std::locale>::value, "");
static_assert(std::is_trivially_relocatable<std::exception_ptr>::value, "");
static_assert(!std::is_trivially_relocatable<std::exception>::value, "");
static_assert(!std::is_trivially_relocatable<std::error_category>::value, "");
static_assert(std::is_trivially_relocatable<std::error_code>::value, "");
static_assert(std::is_trivially_relocatable<std::error_condition>::value, "");
static_assert(std::is_trivially_relocatable<std::errc>::value, "");
static_assert(std::is_trivially_relocatable<std::type_index>::value, "");
static_assert(std::is_trivially_relocatable<std::array<R, 5>>::value, "");
static_assert(!std::is_trivially_relocatable<std::array<T, 5>>::value, "");
static_assert(std::is_trivially_relocatable<std::deque<T>>::value, "");
//static_assert(std::is_trivially_relocatable<std::forward_list<T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::list<T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::map<T,T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::multimap<T,T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::multiset<T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::set<T>>::value, "");
//static_assert(std::is_trivially_relocatable<std:::unordered_map<T,T>>::value, "");
//static_assert(std::is_trivially_relocatable<std:::unordered_multimap<T,T>>::value, "");
//static_assert(std::is_trivially_relocatable<std:::unordered_multiset<T>>::value, "");
//static_assert(std::is_trivially_relocatable<std:::unordered_set<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::vector<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::stack<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::queue<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::priority_queue<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::pmr::deque<T>>::value, "");
//static_assert(std::is_trivially_relocatable<std::pmr::forward_list<T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::pmr::list<T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::pmr::map<T,T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::pmr::multimap<T,T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::pmr::multiset<T>>::value, "");
static_assert(!std::is_trivially_relocatable<std::pmr::set<T>>::value, "");
//static_assert(std::is_trivially_relocatable<std::pmr::unordered_map<T,T>>::value, "");
//static_assert(std::is_trivially_relocatable<std::pmr::unordered_multimap<T,T>>::value, "");
//static_assert(std::is_trivially_relocatable<std::pmr::unordered_multiset<T>>::value, "");
//static_assert(std::is_trivially_relocatable<std::pmr::unordered_set<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::pmr::vector<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::array<R, 5>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::array<T, 5>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::deque<T>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::forward_list<T>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::list<T>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::map<T,T>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::multimap<T,T>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::multiset<T>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::set<T>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::vector<T>::iterator>::value, "");
static_assert(std::is_trivially_relocatable<std::reverse_iterator<int*>>::value, "");
static_assert(std::is_trivially_relocatable<std::back_insert_iterator<std::deque<int>>>::value, "");
static_assert(std::is_trivially_relocatable<std::front_insert_iterator<std::list<int>>>::value, "");
static_assert(std::is_trivially_relocatable<std::insert_iterator<std::vector<int>>>::value, "");
static_assert(std::is_trivially_relocatable<std::istream_iterator<R>>::value, "");
static_assert(!std::is_trivially_relocatable<std::istream_iterator<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::ostream_iterator<R>>::value, "");
static_assert(std::is_trivially_relocatable<std::ostream_iterator<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::char_traits<char>>::value, "");
static_assert(std::is_trivially_relocatable<std::regex_traits<char>>::value, "");
static_assert(std::is_trivially_relocatable<std::regex>::value, "");
static_assert(std::is_trivially_relocatable<std::cmatch>::value, "");
static_assert(std::is_trivially_relocatable<std::smatch>::value, "");
static_assert(std::is_trivially_relocatable<std::string>::value, "");
static_assert(std::is_trivially_relocatable<std::wstring>::value, "");
static_assert(std::is_trivially_relocatable<std::pmr::cmatch>::value, "");
static_assert(std::is_trivially_relocatable<std::pmr::smatch>::value, "");
static_assert(std::is_trivially_relocatable<std::pmr::string>::value, "");
static_assert(std::is_trivially_relocatable<std::pmr::wstring>::value, "");
static_assert(std::is_trivially_relocatable<std::integral_constant<int, 42>>::value, "");
static_assert(std::is_trivially_relocatable<std::index_sequence<1,2,3,4>>::value, "");
static_assert(std::is_trivially_relocatable<std::ratio<1, 2>>::value, "");
static_assert(std::is_trivially_relocatable<std::chrono::system_clock::duration>::value, "");
static_assert(std::is_trivially_relocatable<std::chrono::system_clock::time_point>::value, "");
static_assert(std::is_trivially_relocatable<std::streampos>::value, "");
static_assert(std::is_trivially_relocatable<std::bitset<42>>::value, "");
static_assert(std::is_trivially_relocatable<std::bitset<420>>::value, "");
static_assert(std::is_trivially_relocatable<std::unique_lock<std::mutex>>::value, "");
static_assert(std::is_trivially_relocatable<std::shared_lock<SM>>::value, "");
static_assert(std::is_trivially_relocatable<std::thread>::value, "");
static_assert(std::is_trivially_relocatable<std::thread::id>::value, "");
static_assert(std::is_trivially_relocatable<std::promise<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::promise<T&>>::value, "");
static_assert(std::is_trivially_relocatable<std::promise<void>>::value, "");
static_assert(std::is_trivially_relocatable<std::future<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::future<T&>>::value, "");
static_assert(std::is_trivially_relocatable<std::future<void>>::value, "");
static_assert(std::is_trivially_relocatable<std::shared_future<T>>::value, "");
static_assert(std::is_trivially_relocatable<std::shared_future<T&>>::value, "");
static_assert(std::is_trivially_relocatable<std::shared_future<void>>::value, "");
static_assert(!std::is_trivially_relocatable<std::function<void()>>::value, "");  // contains pointer into self
static_assert(!std::is_trivially_relocatable<std::packaged_task<T(T)>>::value, "");  // contains pointer into self
static_assert(!std::is_trivially_relocatable<std::packaged_task<T&(T)>>::value, "");  // contains pointer into self
static_assert(!std::is_trivially_relocatable<std::packaged_task<void(T)>>::value, "");  // contains pointer into self

static_assert(!std::is_relocatable<std::atomic<int>>::value, "");
static_assert(!std::is_relocatable<std::atomic_flag>::value, "");
static_assert(!std::is_relocatable<std::condition_variable>::value, "");
static_assert(!std::is_relocatable<std::condition_variable_any>::value, "");
static_assert(!std::is_relocatable<std::lock_guard<std::mutex>>::value, "");
static_assert(!std::is_relocatable<std::mutex>::value, "");
static_assert(!std::is_relocatable<std::seed_seq>::value, "");

#endif

int main() {}
