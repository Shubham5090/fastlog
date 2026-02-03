<div align="center">

# 🚀 FastLog

### **CPU-Optimized Log Processing Engine in C++**

[![C++](https://img.shields.io/badge/Language-C++17-blue.svg)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/Status-In%20Development-orange.svg)]()
[![License](https://img.shields.io/badge/License-MIT-green.svg)]()

</div>

---

## 📖 Overview

**FastLog** is a high-performance C++ application designed to efficiently process massive log files (ranging from 100MB to multiple GBs). It focuses on extreme optimization, leveraging low-level CPU and memory techniques to parse, filter, and analyze data at lightning speeds.

### Key Capabilities
- 📂 **High-Speed I/O**: Reads large files efficiently.
- 🔍 **Parsing**: Extracts structured data from raw log lines.
- ⚡ **Filtering**: rapid filtering by log level, user ID, or timestamps.
- 📊 **Aggregation**: Computes statistics (counts, averages, top-K) in real-time.
- ⚙️ **Optimization**: Implements advanced CPU & memory optimizations.

---

## 🎯 Objective

The primary goal is to **progressively optimize** a baseline C++ program. This project serves as a practical exploration of:
- **Memory Management**: Reducing allocations and improving access patterns.
- **CPU Architecture**: Exploiting cache locality and instruction pipelining.
- **Concurrency**: Implementing multithreading for parallel processing.

---

## 🧠 Learning Objectives

- **Basic File I/O**: Efficient reading of large datasets.
- **Benchmarking**: Measuring throughput and latency accurately.
- **Memory Optimization**: Custom allocators, zero-copy parsing.
- **CPU Optimization**: Branch prediction, SIMD, cache friendliness.
- **Multithreading**: Thread pools, lock-free structures.

---

## 🏆 Goals

- [ ] **Build a Complete Engine**: Read, parse, filter, and analyze hundreds of MBs of logs.
- [ ] **Establish Baselines**: Create a simple implementation to benchmark against.
- [ ] **Optimize Data Structures**: Use memory-efficient layouts to minimize overhead.
- [ ] **Cache Optimization**: Improve data layout to reduce cache misses.
- [ ] **Achieve 4x Speedup**: Document and reach a 400% performance improvement over the baseline.

---

## 📅 Timeline

### **Month 1: Foundation**
- [ ] **Basic I/O**: Implementing efficient file reading.
- [ ] **Parsing & Filtering**: Core logic for handling log data.
- [ ] **Benchmarking**: Setting up tools to measure performance (throughput/time).
- [ ] **Aggregation**: Computing basic stats (count per level, average latency).
- [ ] **Core Concepts**: Deep dive into performance measurement.

### **Month 2: Deep Optimization**
- [ ] **CPU & Memory**: Profiling and refactoring for hardware efficiency.
- [ ] **Custom Parsers**: Writing specialized parsers faster than standard libraries.
- [ ] **Multithreading Theory**: Understanding concurrency models.
- [ ] **Parallel Implementation**: Applying threads to hit benchmarks.

### **Month 3: Finalization**
- [ ] **Polish**: Clean up code and documentation.
- [ ] **Backlog**: Address any remaining tasks or bugs.
- [ ] **Final Review**: Prepare the repository for showcase.

---
<div align="center">
    <sub>Built with ❤️ by Shubham</sub>
</div>
