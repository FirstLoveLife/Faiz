//
// Created by firstlove on 11/3/18.
//

#include <memory>
#include <cassert>

void do_nothing(int*) {}

int main()
{
    const std::unique_ptr<int> p1(new int(1));
    assert(!(p1 == nullptr));
    assert(!(nullptr == p1));
    assert(!(p1 < nullptr));
    assert( (nullptr < p1));
    assert(!(p1 <= nullptr));
    assert( (nullptr <= p1));
    assert( (p1 > nullptr));
    assert(!(nullptr > p1));
    assert( (p1 >= nullptr));
    assert(!(nullptr >= p1));

    const std::unique_ptr<int> p2;
    assert( (p2 == nullptr));
    assert( (nullptr == p2));
    assert(!(p2 < nullptr));
    assert(!(nullptr < p2));
    assert( (p2 <= nullptr));
    assert( (nullptr <= p2));
    assert(!(p2 > nullptr));
    assert(!(nullptr > p2));
    assert( (p2 >= nullptr));
    assert( (nullptr >= p2));
}
