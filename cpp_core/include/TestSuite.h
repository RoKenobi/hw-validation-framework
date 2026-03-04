#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <string>
#include <functional>
#include <chrono>

struct TestResult {
    std::string name;
    bool passed;
    double duration_ms;
    std::string message;
};

class TestSuite {
public:
    TestSuite();
    
    // Register a test case
    void registerTest(const std::string& name, std::function<bool()> test_func);
    
    // Run all registered tests
    void runAll();
    
    // Run specific test
    bool runTest(const std::string& name);
    
    // Get results
    std::vector<TestResult> getResults() const;
    int getPassedCount() const;
    int getFailedCount() const;
    double getTotalTime() const;

private:
    struct TestCase {
        std::string name;
        std::function<bool()> func;
    };
    
    std::vector<TestCase> tests;
    std::vector<TestResult> results;
    double total_time_ms;
};

#endif
