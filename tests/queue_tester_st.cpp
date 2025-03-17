#include <iostream>
#include <format>

#include <nimble/queue.hpp>

#include "common.hpp"


void testPushTilFull()
{
    nimble::queue<int> myqueue(100);

    for (int i = 0; i < 100; i++) {
        TestStatementEquals([&]() {return myqueue.try_push(i);}, true, std::format("push {}", i));
    }
    TestStatementEquals([&]() {return myqueue.try_push(100);}, false,"push 100");
}

void testPushPop()
{
    nimble::queue<int> myqueue(100);

    for (int i = 0; i < 10; i++) {
        TestStatementEquals([&]() {return myqueue.try_push(i);}, true, std::format("push {}", i));
    }

    for (int i = 0; i < 10; i++) {
        TestStatementEquals([&]() {int i=-1; myqueue.try_pop(i); return i;}, i, std::format("pop {}", i));
    }
}

void testPushWrap(size_t size)
{
    nimble::queue<int> myqueue(size);

    for (int times = 0; times < 20; times ++)
    {
        for (int i = 0; i < size; i++) {
            TestStatementEquals([&]() {return myqueue.try_push(times*size + i);}, true, std::format("push {},{}", times, i));
        }

        for (int i = 0; i < size; i++) {
            TestStatementEquals([&]() {int i=-1; myqueue.try_pop(i); return i;}, times*size + i, std::format("pop {},{}", times, i));
        }
    }
}

int main() {

    testPushTilFull();
    testPushPop();
    testPushWrap(100);
    testPushWrap(1000);
    testPushWrap(1); 

    Exit();
}
