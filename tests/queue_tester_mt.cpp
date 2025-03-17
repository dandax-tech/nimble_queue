#include <iostream>
#include <format>
#include <set>
#include <thread>
#include <latch>

#include <nimble/queue.hpp>

#include "common.hpp"


struct writer {
    nimble::queue<int> &queue;
    int write_start;
    int write_end;
    std::latch &sync_point1;
    std::latch &sync_point2;
    std::thread thread;
  
    writer(nimble::queue<int> &queue_, int start, int end, std::latch& sync_point1_, std::latch& sync_point2_)
        : queue(queue_),
        write_start(start), write_end(end),
        sync_point1(sync_point1_), sync_point2(sync_point2_),
        thread(&writer::background_write, this)
    {
    }

    ~writer()
    {
        if (thread.joinable()) thread.join();
    }

    void background_write()
    {
        sync_point1.arrive_and_wait();
        //std::cout << "Thread " << std::this_thread::get_id() << " start" << std::endl;

        for (int i = write_start; i < write_end; i++) {
            TestStatementEquals([&](){ return queue.try_push(i); }, true, "MT push");
        }

        //std::cout << "Thread " << std::this_thread::get_id() << " stop" << std::endl;
        sync_point2.arrive_and_wait();
    }    
};


struct reader {
    nimble::queue<int> &queue;

    std::set<int> read_off;
    std::latch &sync_point1;
    std::latch &sync_point2;
    std::thread thread;

    reader(nimble::queue<int> &_queue,
           std::latch &_sync_point1,
           std::latch &_sync_point2) :
        sync_point1(_sync_point1),
        sync_point2(_sync_point2),
        queue(_queue),
        thread(&reader::background, this)
    {
    }

    // not safe to copy std::thread that's running
    reader(const reader&rhs) = delete;
    reader(reader&&rhs) = delete;
    reader operator=(const reader&rhs) = delete;
    reader operator=(reader&&rhs) = delete;

    ~reader()
    {
        if (thread.joinable()) thread.join();
    }

    void background() {
        sync_point1.arrive_and_wait(); 
        //std::cout << "Go thread " << std::this_thread::get_id() << std::endl;

        int x;
        while (queue.try_pop(x)) {
            read_off.insert(x);
        }    

        //std::cout << "Stop thread " << std::this_thread::get_id() << std::endl;
        sync_point2.arrive_and_wait(); 
    }
};

void st_write_mt_read()
{
    const size_t no_threads = 10;
    const size_t no_items = 10000;
    nimble::queue<int> q(no_items);
    for (int i = 0; i < no_items; i++) {
        q.try_push(i);
    }
    TestStatementEquals([&](){ return q.size(); }, no_items, "Full queue");
    
    std::vector<std::unique_ptr<reader>> readers;
    std::latch sync_point1(no_threads);
    std::latch sync_point2(no_threads);
    for (int th = 0; th < no_threads; th ++) {
        readers.emplace_back(std::make_unique<reader>(q, sync_point1, sync_point2));
    }

    sync_point2.wait();

    std::set<int> common;
    for (auto& r : readers) {
        for (auto &n : r->read_off) {
            TestStatementEquals([&](){ return common.find(n) == common.end(); }, true, "Unique result");
            common.insert(n);
        }
        std::cout << r->read_off.size() << ',';
    }
    std::cout << std::endl;
    TestStatementEquals([&](){ return common.size(); }, no_items, "Full result - size");
    TestStatementEquals([&](){ return *common.begin(); }, 0, "Full result - min");
    TestStatementEquals([&](){ return *common.rbegin(); }, no_items-1, "Full result - max");

    q.dump_stats(std::cout);
}

void mt_write_st_read()
{
    const size_t no_threads = 15;
    const size_t no_items = 9999;
    nimble::queue<int> q(no_items);

    std::vector<std::unique_ptr<writer>> writers;
    std::latch sync_point1(no_threads);
    std::latch sync_point2(no_threads);
    int each = no_items / no_threads;
    for (int th = 0; th < no_threads; th ++) {
        int start = each * th;
        int end = 
            (th == no_threads - 1)
            ? no_items
            : start + each;
        writers.emplace_back(std::make_unique<writer>(q, start, end, sync_point1, sync_point2));
    }

    sync_point2.wait();

    TestStatementEquals([&](){ return q.size(); }, no_items, "Full queue");

    std::set<int> full;
    int out;
    while (q.try_pop(out)) {
        full.insert(out);
    }

    for (auto& w : writers) {
        std::cout << w->write_start << ".." << w->write_end << ',';
    }
    std::cout << std::endl;

    TestStatementEquals([&](){ return full.size(); }, no_items, "Full result - size");
    TestStatementEquals([&](){ return *full.begin(); }, 0, "Full result - min");
    TestStatementEquals([&](){ return *full.rbegin(); }, no_items-1, "Full result - max");

    q.dump_stats(std::cout);
}

int main()
{
    TESTWRAP(st_write_mt_read);
    TESTWRAP(mt_write_st_read);
    Exit();
}