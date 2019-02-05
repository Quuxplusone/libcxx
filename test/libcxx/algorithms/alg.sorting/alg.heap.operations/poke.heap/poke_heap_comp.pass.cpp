//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<RandomAccessIterator Iter>
//   requires ShuffleIterator<Iter>
//         && LessThanComparable<Iter::value_type>
//   void
//   __poke_heap(Iter first, Iter last);

#include <algorithm>
#include <functional>
#include <random>
#include <cassert>

#include "test_macros.h"

std::mt19937 randomness;

void test(int M, int N)
{
    int* inputs = new int [N];
    for (int i = 0; i < N; ++i)
        inputs[i] = i;
    std::shuffle(inputs, inputs+N, randomness);
    int* heap = new int[M];
    int* control = new int[M];
    std::make_heap(inputs, inputs+M, std::greater<int>());
    std::copy(inputs, inputs+M, heap);
    std::copy(inputs, inputs+M, control);
    for (int i = 0; i < N; ++i) {
        std::pop_heap(control, control+M, std::greater<int>());
        control[M-1] = inputs[i];
        heap[0] = inputs[i];
        std::push_heap(control, control+M, std::greater<int>());
        std::__poke_heap(heap, heap+M, std::greater<int>());
        assert(std::is_heap(control, control+M, std::greater<int>()));
        assert(std::is_heap(heap, heap+M, std::greater<int>()));
        assert(std::is_permutation(heap, heap+M, control));
    }
    delete [] control;
    delete [] heap;
    delete [] inputs;
}

int main(int, char**)
{
    test(100, 1000);

    // Test small cases
    int ia[] = {3, 2, 1};
    std::__poke_heap(ia, ia, std::greater<int>());  // no-op
    assert(ia[0] == 3);
    std::__poke_heap(ia, ia+1, std::greater<int>());  // no-op
    assert(ia[0] == 3);
    std::__poke_heap(ia, ia+2, std::greater<int>());
    assert(ia[0] == 2 && ia[1] == 3);

  return 0;
}
