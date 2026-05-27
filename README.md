# Priority-Based Real-Time Operating System (RTOS) Core

A lightweight, custom **Cooperative Real-Time Operating System (RTOS)** core engineered from scratch in ANSI C. This project demonstrates the fundamental concepts of embedded systems programming, including task scheduling, context management, and synchronization.

## 🧠 System Overview
This RTOS implements a **Priority-Based Cooperative Scheduler**. Unlike preemptive schedulers that interrupt tasks, this system relies on tasks yielding control, making it ideal for resource-constrained environments where deterministic behavior is required.

### Key Components
* **TCB (Thread Control Block):** The heart of the task management, storing state, priority, and metadata for each thread.
* **Round-Robin & Priority Scheduling:** A selection engine that identifies and executes the highest-priority `READY` task.
* **Synchronization Primitives:**
    * **Mutexes:** Implements ownership-based locking for shared resource protection.
    * **Semaphores:** Implements a token-based system for signaling and resource counting.

## 📂 Project Tree
```text
priority-rtos-core/
├── include/
│   └── rtos.h         # System definitions, TCB structures, and API headers
├── src/
│   └── rtos.c         # Scheduler logic, context switching, and sync primitives
├── tests/
│   └── main.c         # Simulation of multi-tasking workloads with priority competition
└── RTOS_Simulation.ipynb # Google Colab environment for cloud-based compilation & execution
