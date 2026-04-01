# FastLog

## Project Goal
**FastLog** is a high-performance C++ log processing engine designed to handle massive datasets efficiently. The primary objective is to build a baseline implementation and then iteratively optimize it to achieve a **4x speedup** through advanced techniques in memory management, concurrency, and I/O.

This project serves as a real-world playground for systems programming concepts:
- Zero-copy parsing
- Multithreading and concurrency
- Lock-free data structures
- Custom memory allocators

## Log Format
The engine processes ISO8601-timestamped log files with the following structure:
```text
<TIMESTAMP> <LEVEL> <USER> <LATENCY> "<MESSAGE>"
```

**Example:**
```text
2025-08-25T11:33:35Z WARN user2920 152ms "User authenticated"
2026-10-25T03:13:06Z ERROR user8469 66ms "Permission denied"
```

## Directory Structure

```text
.
├── .github/             # GitHub configuration (e.g., CI workflows)
├── benchmarks/          # Performance benchmarking suites
├── datasets/            # Sample log datasets for testing and benchmarks
├── docs/                # Project documentation and track details
├── include/             # C++ header files
│   └── fastlog/         # Core library headers
├── log generator/       # Utilities to generate test log data
├── src/                 # Source code
│   ├── base.cpp         # Baseline implementation (single-threaded, simple parsing)
│   └── final.cpp        # Optimized implementation (multithreaded, zero-copy parsing)
└── tests/               # Unit and integration tests
```
## Code Structure

### `base.cpp`

- Implements the **baseline version** of the system  
- Uses straightforward parsing and aggregation  
- Single-threaded execution  
- Focuses on correctness and establishing initial performance metrics  

**Purpose:**  
Acts as the reference point for measuring optimization gains  

---

### `final.cpp`

- Implements the **optimized version** of the system  
- Uses improved parsing techniques (e.g., zero-copy parsing)  
- Incorporates multithreading for parallel processing  
- Reduces memory allocations and improves cache efficiency  

**Purpose:**  
Demonstrates performance improvements and achieves the target speedup over the baseline  

## How to Run

### Prerequisites
- C++17 compliant compiler (GCC/Clang/MSVC)

### Build
```bash
g++ -Iinclude src/main.cpp -o fastlog
```

### Run
```bash
./fastlog datasets/sample.log
```
