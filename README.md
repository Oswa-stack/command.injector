# CommandInjector v2.0
### C++ Security Testing Tool | OWASP A03:2021 — Command Injection Detection

## What This Tool Does
CommandInjector v2.0 is an offensive security testing tool written 
in C++ that detects command injection vulnerabilities in web 
applications. Tested live against DVWA and successfully identified 
real command injection vulnerabilities.

## OWASP Classification
- Target Vulnerability: OWASP A03:2021 — Injection
- Attack Type: OS Command Injection
- Severity: Critical

## Features
- Automated payload delivery to target input fields
- Detection of vulnerable vs non-vulnerable responses
- Live testing confirmed against DVWA
- Generates detailed vulnerability report

## Technical Stack
- Language: C++
- Target Environment: DVWA on Kali Linux
- Testing Platform: VirtualBox VM

## Files
- `commandinjector.cpp` — Main source code
- `payloads.txt` — Injection payload list
- `report.txt` — Sample vulnerability report output
- `response.txt` — Sample server response log

## Educational Purpose
Built for authorized security testing and educational purposes only.
Never use against systems you do not own or have explicit written
permission to test.

## Author
Oswa | BSc Cybersecurity | BZU Multan
Aspiring Bug Bounty Hunter | Security Researcher
GitHub: https://github.com/Oswa-stack
LinkedIn: https://www.linkedin.com/in/oswa-fatima-cyber/
