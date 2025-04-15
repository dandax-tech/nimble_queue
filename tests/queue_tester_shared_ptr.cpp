#include <iostream>
#include <format>

#include <nimble/queue.hpp>

#include "common.hpp"


struct Payload {
    static std::atomic<int> instances_created;
    static std::atomic<int> instances_deleted;

    Payload(int data) : data(data) {
        instances_created++;
    }

    ~Payload() {
        instances_deleted++;
    }

    int data;
};

std::atomic<int> Payload::instances_created;
std::atomic<int> Payload::instances_deleted;


void testPushPop()
{
    nimble::queue<std::shared_ptr<Payload>> myqueue(100);

    for (int i = 0; i < 10; i++) {
        auto x = std::make_shared<Payload>(i);
        TestStatementEquals([&]() {return myqueue.try_push(x);}, true, std::format("push {}", i));
    }

    for (int i = 0; i < 10; i++) {
        TestStatementEquals([&]() {std::shared_ptr<Payload> x; myqueue.try_pop(x); return x->data;}, i, std::format("pop {}", i));
    }
}

int main() {
    TESTWRAP(testPushPop);
    TestStatementEquals([](){return (int)(Payload::instances_created);}, (int)(Payload::instances_created), "created/deleted");
}