#include "MockHardware.h"
#include <sstream>

MockHardware::MockHardware() 
    : fault_enabled(false), fault_rate(0.0), error_state(false), rng(42) {
    // Initialize default register values
    registers[0x0000] = 0x00000000; // Status register
    registers[0x0004] = 0x00000001; // Version register
    registers[0x0008] = 0xFFFFFFFF; // Config register
}

bool MockHardware::shouldInjectFault() {
    if (!fault_enabled) return false;
    std::uniform_real_distribution<> dist(0.0, 1.0);
    return dist(rng) < fault_rate;
}

bool MockHardware::writeRegister(uint32_t addr, uint32_t value) {
    if (shouldInjectFault()) {
        error_state = true;
        last_error = "Fault injection: Register write failed at 0x" + 
                     std::to_string(addr);
        return false;
    }
    registers[addr] = value;
    return true;
}

uint32_t MockHardware::readRegister(uint32_t addr) {
    if (shouldInjectFault()) {
        error_state = true;
        last_error = "Fault injection: Register read failed at 0x" + 
                     std::to_string(addr);
        return 0xFFFFFFFF; // Error value
    }
    if (registers.find(addr) != registers.end()) {
        return registers[addr];
    }
    return 0x00000000; // Default value for unregistered addresses
}

bool MockHardware::writeMemory(uint32_t addr, uint8_t value) {
    if (shouldInjectFault()) {
        error_state = true;
        last_error = "Fault injection: Memory write failed at 0x" + 
                     std::to_string(addr);
        return false;
    }
    memory[addr] = value;
    return true;
}

uint8_t MockHardware::readMemory(uint32_t addr) {
    if (shouldInjectFault()) {
        error_state = true;
        last_error = "Fault injection: Memory read failed at 0x" + 
                     std::to_string(addr);
        return 0xFF; // Error value
    }
    if (memory.find(addr) != memory.end()) {
        return memory[addr];
    }
    return 0x00;
}

void MockHardware::enableFaultInjection(double rate) {
    fault_enabled = true;
    fault_rate = rate;
}

void MockHardware::disableFaultInjection() {
    fault_enabled = false;
    fault_rate = 0.0;
}

bool MockHardware::hasError() const {
    return error_state;
}

std::string MockHardware::getLastError() const {
    return last_error;
}

void MockHardware::clearError() {
    error_state = false;
    last_error.clear();
}
