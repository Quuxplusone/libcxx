//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// priority_queue();

// void __replace_top(const value_type& v);

#include <queue>
#include <cassert>

int main(int, char**)
{
    std::priority_queue<int> q;
    q.push(1);
    assert(q.top() == 1);
    q.push(3);
    assert(q.top() == 3);
    q.__replace_top(2);
    assert(q.top() == 2);
    q.__replace_top(0);
    assert(q.top() == 1);

  return 0;
}
