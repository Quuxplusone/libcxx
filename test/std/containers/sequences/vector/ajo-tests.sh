# This is just my unofficial test harness.
# Clearly it's not suitable for check-in.

for i in ../vector.bool/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in vector.cons/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in vector.modifiers/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in *.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in vector.capacity/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in vector.data/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in vector.special/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
