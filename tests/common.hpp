#include <iostream>
#include <exception>
#include <functional>

int fails;
int passes;

template <typename T, typename Func>
void TestStatementEquals(Func&& func, const T& expected, std::string_view message) {
    try {
        T actual = func();
        if (actual == expected) {
            passes++;
            //std::cout << "PASSED: " << message << std::endl;
        } else {
            fails++;
            std::cout << "FAILED: " << message << " expected: " << expected << ", got: " << actual << std::endl;
        }
    } catch (const std::exception& ex) {
        fails++;
        std::cout << "FAILED: " << message << " got exception " << ex.what() << std::endl;
    } catch (...) {
        fails++;
        std::cout << "FAILED: " << message << " got unknown exception" << std::endl;
    }
}

template <typename Exception, typename Func>
void TestStatementThrows(Func&& func, std::string_view message) {
    try {
        auto actual = func();
        std::cout << "FAILED: " << message << " expected exception " << typeid(Exception).name() << ", got: " << actual << std::endl;
        fails++;
    } catch (const Exception& ex) {
        passes++;
        //std::cout << "PASSED: " << message << " got exception " << ex.what() << std::endl;
    } catch (...) {
        fails++;
        std::cout << "FAILED: " << message << " got unknown exception" << std::endl;
    }
}

template <typename Func>
void TestWrap(Func&& func, std::string_view message)
{
    std::cout << "Running " << message << "..." << std::endl;
    try {
        func();
        std::cout << "Running " << message << "...OK" << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "FAILED: " << message << " exception" << ex.what() << std::endl;
    } catch (...) {
        std::cout << "FAILED: " << message << " got unknown exception" << std::endl;
    }
}

#define TESTWRAP( name )  TestWrap(name, #name);

void Exit()
{
    std::cout << "PASSED: " << passes << " FAILED: " << fails << std::endl;
    std::exit(fails > 0 ? 1 : 0);
}