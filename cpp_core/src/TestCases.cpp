#include "TestSuite.h"
#include "MockHardware.h"
#include <iostream>
#include <cassert>

// Test 1: Register Write/Read
bool test_register_write() {
    MockHardware hw;
    hw.writeRegister(0x0010, 0xABCD1234);
    uint32_t value = hw.readRegister(0x0010);
    return value == 0xABCD1234;
}

// Test 2: Register Read Default
bool test_register_read() {
    MockHardware hw;
    uint32_t version = hw.readRegister(0x0004);
    return version == 0x00000001;
}

// Test 3: Memory Boundary Test
bool test_memory_boundary() {
    MockHardware hw;
    hw.writeMemory(0x0000, 0xAA);
    hw.writeMemory(0xFFFF, 0xBB);
    return hw.readMemory(0x0000) == 0xAA && hw.readMemory(0xFFFF) == 0xBB;
}

// Test 4: Fault Injection Recovery
bool test_fault_injection() {
    MockHardware hw;
    hw.enableFaultInjection(0.1); // 10% fault rate
    
    // Try multiple writes, at least some should succeed
    int success_count = 0;
    for (int i = 0; i < 100; i++) {
        hw.clearError();
        if (hw.writeRegister(0x0020, i)) {
            success_count++;
        }
    }
    
    hw.disableFaultInjection();
    return success_count > 50; // At least 50% should succeed
}

// Test 5: Stress Test
bool test_stress_test() {
    MockHardware hw;
    for (int i = 0; i < 1000; i++) {
        hw.writeRegister(0x0030, i);
        if (hw.readRegister(0x0030) != static_cast<uint32_t>(i)) {
            return false;
        }
    }
    return true;
}

void registerAllTests(TestSuite& suite) {
    suite.registerTest("register_write", test_register_write);
    suite.registerTest("register_read", test_register_read);
    suite.registerTest("memory_boundary", test_memory_boundary);
    suite.registerTest("fault_injection", test_fault_injection);
    suite.registerTest("stress_test", test_stress_test);
}
