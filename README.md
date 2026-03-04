# 🔧 Hardware Validation Framework
An automated hardware validation suite combining C++ test core with Python orchestration. Designed to simulate silicon validation workflows, detect flaky tests, and generate comprehensive performance reports.

## 🎯 Project Overview

This framework mimics real-world hardware validation pipelines used at companies like AMD, Intel, and NVIDIA. It provides:

- **Mock Hardware Interface:** Simulates register reads/writes with configurable fault injection.
- **Automated Test Runner:** Python-based orchestration with retry logic for flaky test detection.
- **CI/CD Integration:** GitHub Actions pipeline for continuous regression testing.
- **HTML Reporting:** Generates visual test reports with pass/fail metrics and timing data.

## 🚀 Key Features

- **Mock Silicon Model:** C++ class simulating hardware registers with fault injection capabilities.
- **Test Case Framework:** Modular test cases for register validation, memory boundaries, and error recovery.
- **Flaky Test Detection:** Automatic retry mechanism (3 attempts) to identify intermittent failures.
- **Multi-Format Reports:** Outputs JSON for CI pipelines and HTML for human review.
- **Extensible Architecture:** Easy to add new test cases or hardware models.

## 🏗️ System Architecture

```text
┌─────────────────────────────────────────────────────────────────┐
│                    Python Orchestrator                          │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────────┐ │
│  │  runner.py  │  │ reporter.py │  │  requirements.txt       │ │
│  └──────┬──────┘  └──────┬──────┘  └─────────────────────────┘ │
│         │                │                                      │
│         └───────┬────────┘                                      │
│                 │ (subprocess)                                  │
└─────────────────┼───────────────────────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────────────────────┐
│                      C++ Test Core                              │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │ MockHardware.h  │  │  TestSuite.h    │  │    main.cpp     │ │
│  │ MockHardware.cpp│  │ TestCases.cpp   │  │                 │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
│                                                                 │
│  Simulates: Register File, Memory Map, Fault Injection          │
└─────────────────────────────────────────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────────────────────┐
│                      Output & Reports                           │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────────┐ │
│  │ report.json │  │ report.html │  │  logs/                  │ │
│  └─────────────┘  └─────────────┘  └─────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

## 🛠️ Build Instructions

### Prerequisites
- CMake 3.10+
- C++17 compatible compiler (g++ or clang)
- Python 3.8+
- pip for Python dependencies

### Step 1: Build C++ Core
```bash
cd cpp_core
mkdir build && cd build
cmake ..
make
```

### Step 2: Install Python Dependencies
```bash
cd ../../python_orchestrator
pip install -r requirements.txt
```

### Step 3: Run Test Suite
```bash
cd python_orchestrator
python3 runner.py
```

## 📖 Usage

### Run All Tests
```bash
python3 runner.py --all
```

### Run Specific Test
```bash
python3 runner.py --test register_write
```

### Generate HTML Report
```bash
python3 runner.py --report html
```

### CI/CD Mode (No Interactive Output)
```bash
python3 runner.py --ci-mode
```

## 🧪 Test Cases

| Test Name | Description | Expected Result |
|-----------|-------------|-----------------|
| `register_write` | Write 0xFFFFFFFF to register, read back | Pass |
| `register_read` | Read default register value | Pass |
| `memory_boundary` | Test edge cases at memory boundaries | Pass |
| `fault_injection` | Verify error handling on injected faults | Pass |
| `stress_test` | 1000 consecutive read/write operations | Pass |

## 📊 Sample Output

### Console Output
```text
=== Hardware Validation Framework ===
[INFO] Starting test suite...
[RUN] test_register_write... PASS (0.002s)
[RUN] test_register_read... PASS (0.001s)
[RUN] test_memory_boundary... PASS (0.003s)
[RUN] test_fault_injection... PASS (0.002s)
[RUN] test_stress_test... PASS (0.150s)
=========================================
Results: 5/5 Passed (100.00%)
Total Time: 0.158s
[INFO] Report saved to reports/report.html
```

### JSON Report
```json
{
  "timestamp": "2024-01-15T10:30:00Z",
  "total_tests": 5,
  "passed": 5,
  "failed": 0,
  "pass_rate": 100.0,
  "tests": [
    {"name": "register_write", "status": "PASS", "duration_ms": 2},
    {"name": "register_read", "status": "PASS", "duration_ms": 1}
  ]
}
```

## 🔄 CI/CD Pipeline

This project includes GitHub Actions for continuous integration:

- **On Push:** Automatically builds C++ core and runs all tests.
- **On Pull Request:** Validates changes before merge.
- **Artifact Storage:** Test reports are saved as workflow artifacts.

View the pipeline: [Actions Tab](https://github.com/yourusername/hw-validation-framework/actions)

## 📁 Project Structure

```
hw-validation-framework/
├── cpp_core/           # C++ test core and mock hardware
├── python_orchestrator/# Python automation and reporting
├── reports/            # Generated test reports
├── .github/workflows/  # CI/CD pipeline configuration
└── README.md           # This file
```

## 🐛 Debugging

### Enable Verbose Logging
```bash
python3 runner.py --verbose
```

### Check C++ Core Directly
```bash
cd cpp_core/build
./test_runner --test register_write
```

### View Flaky Test Logs
```bash
cat reports/flaky_tests.log
```

## 📄 License
Distributed under the MIT License. See `LICENSE` for more information.

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/new_test`)
3. Add your test case to `cpp_core/src/TestCases.cpp`
4. Run the test suite locally
5. Submit a pull request

## 📧 Contact

For questions or issues, please open a GitHub Issue.
