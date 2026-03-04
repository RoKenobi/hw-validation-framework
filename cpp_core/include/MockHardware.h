#ifndef MOCK_HARDWARE_H
#define MOCK_HARDWARE_H

#include <cstdint>
#include <map>
#include <random>

class MockHardware {
public:
    MockHardware();
    
    // Register operations
    bool writeRegister(uint32_t addr, uint32_t value);
    uint32_t readRegister(uint32_t addr);
    
    // Memory operations
    bool writeMemory(uint32_t addr, uint8_t value);
    uint8_t readMemory(uint32_t addr);
    
    // Fault injection for testing error handling
    void enableFaultInjection(double fault_rate);
    void disableFaultInjection();
    
    // Status
    bool hasError() const;
    std::string getLastError() const;
    void clearError();

private:
    std::map<uint32_t, uint32_t> registers;
    std::map<uint32_t, uint8_t> memory;
    bool fault_enabled;
    double fault_rate;
    bool error_state;
    std::string last_error;
    std::mt19937 rng;
    
    bool shouldInjectFault();
};

#endif
