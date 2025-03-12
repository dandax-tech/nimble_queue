#include <iostream>
//#include <format>

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
        TestStatementEquals([&]() {int i=-1; myqueue.try_pop(i); return i;}, i, std::format("push {}", i));
    }
}

int main() {

    testPushTilFull();
    testPushPop();

    Exit();
}
