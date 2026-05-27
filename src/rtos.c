#include "../include/rtos.h"
#include <stdio.h>
#include <string.h>

// Global System Variables
static TCB task_pool[MAX_TASKS];
static int total_tasks = 0;
static int current_task_id = -1;
static uint32_t system_tick = 0; // Simulated system clock tracker

// 1. Initialize the RTOS Core
void rtos_init(void) {
    total_tasks = 0;
    current_task_id = -1;
    system_tick = 0;
    memset(task_pool, 0, sizeof(task_pool));
    printf("[KERNEL] RTOS Core Engine Initialized.\n");
}

// 2. Register a Task into the System
int rtos_create_task(const char* name, void (*func)(void), int priority, uint32_t period_ms) {
    if (total_tasks >= MAX_TASKS) {
        printf("[KERNEL ERROR] Cannot create task '%s'. Maximum task limit reached.\n", name);
        return -1;
    }

    task_pool[total_tasks].id = total_tasks;
    task_pool[total_tasks].name = name;
    task_pool[total_tasks].state = READY;
    task_pool[total_tasks].priority = priority;
    task_pool[total_tasks].period_ms = period_ms;
    task_pool[total_tasks].last_run = 0;
    task_pool[total_tasks].task_function = func;

    printf("[KERNEL] Registered Task %d: '%s' (Priority: %d, Period: %dms)\n", 
           total_tasks, name, priority, period_ms);
    
    total_tasks++;
    return total_tasks - 1;
}

// 3. The Scheduler (Highest-Priority Ready Task First Selector)
static void rtos_schedule(void) {
    int highest_priority = -1;
    int next_task_id = -1;

    // Simulate clock passage for time-managed workloads
    system_tick += 10; 

    // Search for the highest priority task that is READY
    for (int i = 0; i < total_tasks; i++) {
        if (task_pool[i].state == READY) {
            if (task_pool[i].priority > highest_priority) {
                highest_priority = task_pool[i].priority;
                next_task_id = i;
            }
        }
    }

    // Context Switch execution block
    if (next_task_id != -1) {
        current_task_id = next_task_id;
        task_pool[current_task_id].state = RUNNING;
        
        printf("\n--- [SCHEDULER] Context Switch -> Running Task %d [%s] ---\n", 
               task_pool[current_task_id].id, task_pool[current_task_id].name);
        
        // Execute the assigned C function pointer
        task_pool[current_task_id].task_function();
    } else {
        printf("[SCHEDULER] Idle State. No tasks ready.\n");
        // Reset all completed cooperative tasks back to ready for simulation loop continuousness
        for(int i = 0; i < total_tasks; i++) {
            if (task_pool[i].state != BLOCKED) {
                task_pool[i].state = READY;
            }
        }
    }
}

// 4. Cooperative Yield Primitive
void rtos_yield(void) {
    if (current_task_id != -1) {
        // If it wasn't blocked by a sync primitive, return it to ready
        if (task_pool[current_task_id].state == RUNNING) {
            task_pool[current_task_id].state = READY;
        }
        printf("[TASK] Task %d [%s] gracefully yielded control.\n", 
               current_task_id, task_pool[current_task_id].name);
    }
    rtos_schedule();
}

// 5. Start the Engine Execution
void rtos_start(void) {
    printf("[KERNEL] Starting Priority-Based Scheduler Engine...\n");
    rtos_schedule();
}

// ============================================================================
// SYNCHRONIZATION PRIMITIVES (Mutex & Semaphore Implementation)
// ============================================================================

void rtos_sem_init(Semaphore* sem, int initial_value) {
    sem->value = initial_value;
    sem->blocked_count = 0;
}

void rtos_sem_wait(Semaphore* sem, int task_id) {
    if (sem->value > 0) {
        sem->value--; // Take token
        printf("[SEM] Task %d acquired semaphore token. Tokens left: %d\n", task_id, sem->value);
    } else {
        // Block task
        task_pool[task_id].state = BLOCKED;
        sem->blocked_tasks[sem->blocked_count++] = task_id;
        printf("[SEM BLOCK] Resource empty! Task %d status changed to BLOCKED.\n", task_id);
        rtos_yield(); // Handover control to next highest task
    }
}

void rtos_sem_post(Semaphore* sem) {
    sem->value++;
    printf("[SEM] Semaphore token released. Tokens available: %d\n", sem->value);
    if (sem->blocked_count > 0) {
        // Unblock the oldest waiting task (FIFO style rescue)
        int unblocked_id = sem->blocked_tasks[0];
        task_pool[unblocked_id].state = READY;
        
        // Shift remaining blocked queue
        for (int i = 0; i < sem->blocked_count - 1; i++) {
            sem->blocked_tasks[i] = sem->blocked_tasks[i+1];
        }
        sem->blocked_count--;
        printf("[SEM UNBLOCK] Task %d woken up and moved to READY state.\n", unblocked_id);
    }
}

void rtos_mutex_init(Mutex* mutex) {
    mutex->locked = 0;
    mutex->owner_id = -1;
}

void rtos_mutex_lock(Mutex* mutex, int task_id) {
    if (!mutex->locked) {
        mutex->locked = 1;
        mutex->owner_id = task_id;
        printf("[MUTEX] Lock acquired by Task %d.\n", task_id);
    } else {
        printf("[MUTEX CONFLICT] Locked already! Task %d blocking until released by Task %d.\n", task_id, mutex->owner_id);
        task_pool[task_id].state = BLOCKED;
        rtos_yield();
    }
}

void rtos_mutex_unlock(Mutex* mutex) {
    printf("[MUTEX] Released by Task %d.\n", mutex->owner_id);
    int released_owner = mutex->owner_id;
    mutex->locked = 0;
    mutex->owner_id = -1;

    // Wake up any task waiting on this specific mutex block
    for(int i = 0; i < total_tasks; i++) {
        if(task_pool[i].state == BLOCKED) {
             task_pool[i].state = READY;
             printf("[MUTEX UNBLOCK] Task %d woken up.\n", i);
             break; // Wake up one for now to check priority handling
        }
    }
}
