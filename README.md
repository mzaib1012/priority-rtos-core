# Priority-Based Real-Time Operating System (RTOS) Core

A lightweight, custom cooperative Real-Time Operating System (RTOS) core engineered entirely from scratch using ANSI C. This project models low-level embedded OS kernels by managing task state progressions, executing context switching based on priority matrices, and establishing reliable task synchronization.

## 🛠️ System Architecture Diagram & Layout
```text
priority-rtos-core/
├── include/
│   └── rtos.h         # Blueprint definitions (TCBs, Mutex, Semaphores)
├── src/
│   └── rtos.c         # Scheduler Engine & Sync Primitives Implementation
├── tests/
│   └── main.c         # Simulated Multi-Priority Task Workloads
└── RTOS_Simulation.ipynb # Cloud Compiler Visualizer Runtime
