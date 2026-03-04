#include <iostream>
#include <string>
#include <cstring>
#include "TestSuite.h"
#include "MockHardware.h"

void printUsage(const char* progname) {
    std::cout << "Usage: " << progname << " [options]" << std::endl;
    std::cout << "  --test <name>   Run specific test" << std::endl;
    std::cout << "  --list          List all available tests" << std::endl;
    std::cout << "  --json          Output results in JSON format" << std::endl;
    std::cout << "  --help          Show this help message" << std::endl;
}

int main(int argc, char** argv) {
    TestSuite suite;
    
    // Register all tests
    extern void registerAllTests(TestSuite&);
    registerAllTests(suite);
    
    std::string specific_test;
    bool json_output = false;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--test") == 0 && i + 1 < argc) {
            specific_test = argv[++i];
        } else if (strcmp(argv[i], "--list") == 0) {
            std::cout << "Available tests:" << std::endl;
            std::cout << "  - register_write" << std::endl;
            std::cout << "  - register_read" << std::endl;
            std::cout << "  - memory_boundary" << std::endl;
            std::cout << "  - fault_injection" << std::endl;
            std::cout << "  - stress_test" << std::endl;
            return 0;
        } else if (strcmp(argv[i], "--json") == 0) {
            json_output = true;
        } else if (strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        }
    }
    
    // Run tests
    if (!specific_test.empty()) {
        suite.runTest(specific_test);
    } else {
        suite.runAll();
    }
    
    // Output results
    if (json_output) {
        std::cout << "{" << std::endl;
        std::cout << "  \"total_tests\": " << (suite.getPassedCount() + suite.getFailedCount()) << "," << std::endl;
        std::cout << "  \"passed\": " << suite.getPassedCount() << "," << std::endl;
        std::cout << "  \"failed\": " << suite.getFailedCount() << "," << std::endl;
        std::cout << "  \"pass_rate\": " << (suite.getPassedCount() * 100.0 / (suite.getPassedCount() + suite.getFailedCount())) << std::endl;
        std::cout << "}" << std::endl;
    } else {
        std::cout << "=========================================" << std::endl;
        std::cout << "Results: " << suite.getPassedCount() << "/" 
                  << (suite.getPassedCount() + suite.getFailedCount()) 
                  << " Passed (" 
                  << (suite.getPassedCount() * 100.0 / (suite.getPassedCount() + suite.getFailedCount())) 
                  << "%)" << std::endl;
        std::cout << "Total Time: " << suite.getTotalTime() << "ms" << std::endl;
    }
    
    return suite.getFailedCount() > 0 ? 1 : 0;
}
