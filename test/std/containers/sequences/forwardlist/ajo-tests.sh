# This is just my unofficial test harness.
# Clearly it's not suitable for check-in.

for i in forwardlist.access/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in forwardlist.cons/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in forwardlist.modifiers/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in *.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in forwardlist.iter/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in forwardlist.ops/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
for i in forwardlist.spec/*.cpp; do
    g++ -std=c++14 -isystem ../../../../../include -DTEST_STD_VER=14 -I ../../../../support -DAJO_FANCY $i
    ./a.out
done
