# FastLog

## Project Goal
**FastLog** is a high-performance C++ log processing engine designed to handle massive datasets efficiently. The primary objective is to build a baseline implementation and then iteratively optimize it to achieve a **4x speedup** through advanced techniques in memory management, concurrency, and I/O.

This project serves as a real-world playground for systems programming concepts:
- Zero-copy parsing
- SIMD optimization
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

## Milestones

### Month 1: Foundation
- [ ] **Baseline I/O**: Efficient file reading (chunks/mmap).
- [ ] **Basic Parser**: Convert raw lines to structured `LogEntry` objects.
- [ ] **Stats Engine**: Aggregate counts by level and service.
- [ ] **Benchmarking Harness**: Establish the "Ground Truth" performance.

### Month 2: Optimization
- [ ] **Profiling**: Identify hotspots (CPU/Memory).
- [ ] **Multithreading**: Implement thread pool for parallel processing.
- [ ] **Memory Optimization**: Reduce allocations (std::string overhead).
- [ ] **Goal**: Achieve 4x throughput vs Month 1 baseline.

### Month 3: Polish
- [ ] **API Cleanup**: internal interfaces are stable and clean.
- [ ] **Final Benchmarks**: Comprehensive report generation.
- [ ] **Demo**: One-click script to show processing of a 1GB+ file.