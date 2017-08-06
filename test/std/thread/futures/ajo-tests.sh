# This is just my unofficial test harness.
# Clearly it's not suitable for check-in.

for i in futures.async/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
for i in futures.errors/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
for i in futures.future_error/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
for i in futures.overview/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
for i in futures.promise/*.pass.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
for i in futures.shared_future/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
for i in futures.state/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
for i in futures.task/futures.task.members/*.pass.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
for i in futures.task/futures.task.nonmembers/*.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
for i in futures.unique_future/*.pass.cpp; do
    g++ -std=c++14 -isystem ../../../../include -DTEST_STD_VER=14 -I ../../../support -DAJO_FANCY $i
    ./a.out
done
