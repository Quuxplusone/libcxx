# This is just my unofficial test harness.
# Clearly it's not suitable for check-in.

for i in deque.cons/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in deque.modifiers/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in *.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in deque.capacity/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in deque.special/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
