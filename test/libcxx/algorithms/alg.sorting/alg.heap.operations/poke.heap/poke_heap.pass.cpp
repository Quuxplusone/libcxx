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
//   poke_heap(Iter first, Iter last);

#include <algorithm>
#include <random>
#include <cassert>
#include <stdio.h>

std::mt19937 randomness;

void test(int M, int N)
{
    int* inputs = new int [N];
    for (int i = 0; i < N; ++i)
        inputs[i] = i;
    std::shuffle(inputs, inputs+N, randomness);
    int* heap = new int[M];
    int* control = new int[M];
    std::make_heap(inputs, inputs+M);
    std::copy(inputs, inputs+M, heap);
    std::copy(inputs, inputs+M, control);
    for (int i = 0; i < N; ++i) {
        std::pop_heap(control, control+M);
        control[M-1] = inputs[i];
        heap[0] = inputs[i];
        std::push_heap(control, control+M);
        std::poke_heap(heap, heap+M);
        assert(std::is_heap(control, control+M));
        assert(std::is_heap(heap, heap+M));
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
    int ia[] = {1, 2, 3};
    std::poke_heap(ia, ia);  // no-op
    assert(ia[0] == 1);
    std::poke_heap(ia, ia+1);  // no-op
    assert(ia[0] == 1);
    std::poke_heap(ia, ia+2);
    assert(ia[0] == 2 && ia[1] == 1);

  return 0;
}
