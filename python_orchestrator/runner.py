#!/usr/bin/env python3
import subprocess
import json
import os
import sys
import time
from datetime import datetime

class TestRunner:
    def __init__(self, cpp_binary="../cpp_core/build/test_runner"):
        self.cpp_binary = cpp_binary
        self.results = []
        self.retry_count = 3
        
    def run_test(self, test_name):
        """Run a single test with retry logic for flaky test detection"""
        for attempt in range(self.retry_count):
            try:
                result = subprocess.run(
                    [self.cpp_binary, "--test", test_name],
                    capture_output=True,
                    text=True,
                    timeout=30
                )
                
                if result.returncode == 0:
                    return {"name": test_name, "status": "PASS", "attempts": attempt + 1}
                else:
                    if attempt < self.retry_count - 1:
                        print(f"[RETRY] {test_name} (attempt {attempt + 2}/{self.retry_count})")
                        time.sleep(0.5)
                    else:
                        return {"name": test_name, "status": "FAIL", "attempts": attempt + 1}
                        
            except subprocess.TimeoutExpired:
                if attempt < self.retry_count - 1:
                    print(f"[TIMEOUT] {test_name} - Retrying...")
                else:
                    return {"name": test_name, "status": "TIMEOUT", "attempts": attempt + 1}
            except Exception as e:
                return {"name": test_name, "status": "ERROR", "error": str(e)}
        
        return {"name": test_name, "status": "FAIL", "attempts": self.retry_count}
    
    def run_all(self):
        """Run all tests"""
        tests = ["register_write", "register_read", "memory_boundary", 
                 "fault_injection", "stress_test"]
        
        print("=== Hardware Validation Framework ===")
        print(f"[INFO] Starting test suite at {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        
        for test in tests:
            result = self.run_test(test)
            self.results.append(result)
            print(f"[{result['status']}] {test} ({result['attempts']} attempt(s))")
        
        return self.results
    
    def save_report(self, filename="report.json"):
        """Save results to JSON"""
        report = {
            "timestamp": datetime.now().isoformat(),
            "total_tests": len(self.results),
            "passed": sum(1 for r in self.results if r["status"] == "PASS"),
            "failed": sum(1 for r in self.results if r["status"] in ["FAIL", "TIMEOUT", "ERROR"]),
            "pass_rate": sum(1 for r in self.results if r["status"] == "PASS") / len(self.results) * 100,
            "tests": self.results
        }
        
        with open(filename, "w") as f:
            json.dump(report, f, indent=2)
        
        print(f"[INFO] Report saved to {filename}")
        return report

def main():
    runner = TestRunner()
    
    if len(sys.argv) > 1 and sys.argv[1] == "--all":
        runner.run_all()
        runner.save_report("../reports/report.json")
    elif len(sys.argv) > 2 and sys.argv[1] == "--test":
        test_name = sys.argv[2]
        result = runner.run_test(test_name)
        print(f"[{result['status']}] {test_name}")
    else:
        runner.run_all()
        runner.save_report("../reports/report.json")

if __name__ == "__main__":
    main()
