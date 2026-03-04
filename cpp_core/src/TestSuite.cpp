#include "TestSuite.h"
#include <iostream>
#include <algorithm>

TestSuite::TestSuite() : total_time_ms(0.0) {}

void TestSuite::registerTest(const std::string& name, std::function<bool()> test_func) {
    tests.push_back({name, test_func});
}

void TestSuite::runAll() {
    for (const auto& test : tests) {
        runTest(test.name);
    }
}

bool TestSuite::runTest(const std::string& name) {
    auto it = std::find_if(tests.begin(), tests.end(),
        [&name](const TestCase& t) { return t.name == name; });
    
    if (it == tests.end()) {
        std::cerr << "[ERROR] Test not found: " << name << std::endl;
        return false;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    bool passed = it->func();
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::milli> elapsed = end - start;
    
    TestResult result;
    result.name = name;
    result.passed = passed;
    result.duration_ms = elapsed.count();
    result.message = passed ? "PASS" : "FAIL";
    
    results.push_back(result);
    total_time_ms += elapsed.count();
    
    std::cout << "[RUN] " << name << "... " << (passed ? "PASS" : "FAIL") 
              << " (" << elapsed.count() << "ms)" << std::endl;
    
    return passed;
}

std::vector<TestResult> TestSuite::getResults() const {
    return results;
}

int TestSuite::getPassedCount() const {
    int count = 0;
    for (const auto& r : results) {
        if (r.passed) count++;
    }
    return count;
}

int TestSuite::getFailedCount() const {
    int count = 0;
    for (const auto& r : results) {
        if (!r.passed) count++;
    }
    return count;
}

double TestSuite::getTotalTime() const {
    return total_time_ms;
}
