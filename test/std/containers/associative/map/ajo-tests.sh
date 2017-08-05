# This is just my unofficial test harness.
# Clearly it's not suitable for check-in.

for i in map.access/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in map.cons/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in map.modifiers/*.cpp; do
    g++ -std=c++1z -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in *.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in map.ops/*.pass.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in map.special/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
