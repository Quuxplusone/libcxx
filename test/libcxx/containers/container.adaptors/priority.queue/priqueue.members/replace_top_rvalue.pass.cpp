//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <queue>

// priority_queue();

// void replace_top(value_type&& v);

#include <queue>
#include <cassert>

#include "MoveOnly.h"

int main()
{
    std::priority_queue<MoveOnly> q;
    q.push(1);
    assert(q.top() == 1);
    q.push(3);
    assert(q.top() == 3);
    q.replace_top(2);
    assert(q.top() == 2);
    q.replace_top(0);
    assert(q.top() == 1);
}
