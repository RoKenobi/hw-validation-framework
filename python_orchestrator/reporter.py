#!/usr/bin/env python3
import json
import os
from datetime import datetime

def generate_html_report(json_file="report.json", output_file="report.html"):
    """Generate HTML report from JSON results"""
    
    try:
        with open(json_file, "r") as f:
            data = json.load(f)
    except FileNotFoundError:
        print(f"Error: {json_file} not found")
        return
    
    # Calculate statistics
    total = data["total_tests"]
    passed = data["passed"]
    failed = data["failed"]
    pass_rate = data["pass_rate"]
    
    # Determine overall status
    status_color = "#28a745" if failed == 0 else "#dc3545"
    status_text = "PASSED" if failed == 0 else "FAILED"
    
    html = f"""<!DOCTYPE html>
<html>
<head>
    <title>Hardware Validation Report</title>
    <style>
        body {{ font-family: Arial, sans-serif; margin: 40px; }}
        .header {{ padding: 20px; background: {status_color}; color: white; border-radius: 5px; }}
        .stats {{ display: flex; gap: 20px; margin: 20px 0; }}
        .stat-box {{ padding: 15px; background: #f8f9fa; border-radius: 5px; text-align: center; }}
        .stat-number {{ font-size: 24px; font-weight: bold; }}
        table {{ width: 100%; border-collapse: collapse; margin-top: 20px; }}
        th, td {{ padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }}
        th {{ background: #f8f9fa; }}
        .pass {{ color: #28a745; font-weight: bold; }}
        .fail {{ color: #dc3545; font-weight: bold; }}
    </style>
</head>
<body>
    <div class="header">
        <h1>🔧 Hardware Validation Report</h1>
        <p>Generated: {data["timestamp"]}</p>
        <h2>Overall Status: {status_text}</h2>
    </div>
    
    <div class="stats">
        <div class="stat-box">
            <div class="stat-number">{total}</div>
            <div>Total Tests</div>
        </div>
        <div class="stat-box">
            <div class="stat-number" style="color: #28a745;">{passed}</div>
            <div>Passed</div>
        </div>
        <div class="stat-box">
            <div class="stat-number" style="color: #dc3545;">{failed}</div>
            <div>Failed</div>
        </div>
        <div class="stat-box">
            <div class="stat-number">{pass_rate:.1f}%</div>
            <div>Pass Rate</div>
        </div>
    </div>
    
    <h3>Test Details</h3>
    <table>
        <tr>
            <th>Test Name</th>
            <th>Status</th>
            <th>Attempts</th>
        </tr>
"""
    
    for test in data["tests"]:
        status_class = "pass" if test["status"] == "PASS" else "fail"
        html += f"""        <tr>
            <td>{test["name"]}</td>
            <td class="{status_class}">{test["status"]}</td>
            <td>{test["attempts"]}</td>
        </tr>
"""
    
    html += """    </table>
</body>
</html>"""
    
    with open(output_file, "w") as f:
        f.write(html)
    
    print(f"HTML report generated: {output_file}")

if __name__ == "__main__":
    generate_html_report()
