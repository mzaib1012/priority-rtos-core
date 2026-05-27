#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>
#include <stddef.h>

#include_next <stdio.h> // Fallback safety for cloud compilation environment

// Define maximum limits for our lightweight RTOS
#define MAX_TASKS 5
#define STACK_SIZE 1024

// Task States
typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    SUSPENDED
} TaskState;

// Thread Control Block (TCB) Structure
typedef struct {
    int id;                 // Unique task identifier
    const char* name;       // Human-readable task name
    TaskState state;        // Current execution status
    int priority;           // Higher number = higher priority
    uint32_t period_ms;     // Cooperative execution frequency tracking
    uint32_t last_run;      // Timestamp of the last execution block
    void (*task_function)(void); // Pointer to the C function this task runs
} TCB;

// Synchronization Structure: Semaphore
typedef struct {
    int value;              // Available token count
    int blocked_tasks[MAX_TASKS]; // Tracking IDs of tasks waiting for this token
    int blocked_count;
} Semaphore;

// Synchronization Structure: Mutex (Binary Semaphore with Ownership)
typedef struct {
    int locked;             // 1 if locked, 0 if free
    int owner_id;           // Task ID holding the lock (-1 if free)
} Mutex;

// Core RTOS System Functions
void rtos_init(void);
int rtos_create_task(const char* name, void (*func)(void), int priority, uint32_t period_ms);
void rtos_start(void);
void rtos_yield(void);

// Sync Primitives Functions
void rtos_sem_init(Semaphore* sem, int initial_value);
void rtos_sem_wait(Semaphore* sem, int task_id);
void rtos_sem_post(Semaphore* sem);

void rtos_mutex_init(Mutex* mutex);
void rtos_mutex_lock(Mutex* mutex, int task_id);
void rtos_mutex_unlock(Mutex* mutex);

#endif // RTOS_H

