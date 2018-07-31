# This is just my unofficial test harness.
# Clearly it's not suitable for check-in.

for i in unord.map.cnstr/*.pass.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in unord.map.modifiers/*.cpp; do
    g++ -std=c++1z -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in *.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in unord.map.elem/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
for i in unord.map.swap/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY $i
    ./a.out
done
