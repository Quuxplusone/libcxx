# This is just my unofficial test harness.
# Clearly it's not suitable for check-in.

for i in string.cons/*.pass.cpp; do
    g++ -std=c++1z -isystem ../../../../include -DTEST_STD_VER=17 -I ../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY -DAJO_TRIVIALLY_CTABLE_FANCY $i
    ./a.out
done
for i in string.modifiers/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY -DAJO_TRIVIALLY_CTABLE_FANCY $i
    ./a.out
done
for i in *.pass.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY -DAJO_TRIVIALLY_CTABLE_FANCY $i
    ./a.out
done
for i in string.access/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY -DAJO_TRIVIALLY_CTABLE_FANCY $i
    ./a.out
done
echo 'We expect string.capacity/over_max_size.pass.cpp to segfault.'
for i in string.capacity/*.cpp; do
    echo $i
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY -DAJO_TRIVIALLY_CTABLE_FANCY $i
    ./a.out
done
echo 'No more segfaults, now.'
for i in string.iterators/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY -DAJO_TRIVIALLY_CTABLE_FANCY $i
    ./a.out
done
for i in string.nonmembers/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY -DAJO_TRIVIALLY_CTABLE_FANCY $i
    ./a.out
done
for i in string.ops/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY -DAJO_TRIVIALLY_CTABLE_FANCY $i
    ./a.out
done
for i in string.require/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY -DAJO_ITERABLE_FANCY -DAJO_TRIVIALLY_CTABLE_FANCY $i
    ./a.out
done
