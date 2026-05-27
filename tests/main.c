#include "../include/rtos.h"
#include <stdio.h>

// Declare shared synchronization resources
Mutex resource_mutex;
Semaphore counter_sem;

// --- Task 0: High Priority Monitor ---
void task_high_monitor(void) {
    printf("[EXEC] Task High Running: Inspecting system health...\n");
    printf("[EXEC] Task High: Everything stable.\n");
    rtos_yield(); // Voluntarily pass control back to scheduler
}

// --- Task 1: Medium Priority Worker ---
void task_med_worker(void) {
    printf("[EXEC] Task Med Running: Attempting to lock shared resource...\n");
    rtos_mutex_lock(&resource_mutex, 1); // Task ID 1 locks
    
    printf("[EXEC] Task Med: Critical section work in progress...\n");
    
    rtos_yield(); // Yield while holding the lock to see if Task Low gets blocked!
    
    printf("[EXEC] Task Med: Finishing work and unlocking...\n");
    rtos_mutex_unlock(&resource_mutex);
    rtos_yield();
}

// --- Task 2: Low Priority Worker ---
void task_low_worker(void) {
    printf("[EXEC] Task Low Running: Attempting to lock shared resource...\n");
    rtos_mutex_lock(&resource_mutex, 2); // Task ID 2 tries to lock
    
    printf("[EXEC] Task Low: Critical section work completed!\n");
    rtos_mutex_unlock(&resource_mutex);
    rtos_yield();
}

// --- Main System Entry Point ---
int main(void) {
    printf("==================================================\n");
    printf("     CUSTOM COOPERATIVE RTOS INITIALIZATION       \n");
    printf("==================================================\n");

    // Initialize Kernel
    rtos_init();

    // Initialize Mutex and Semaphores
    rtos_mutex_init(&resource_mutex);
    rtos_sem_init(&counter_sem, 1);

    // Register Tasks: Name, Function Pointer, Priority, Period (ms)
    // Higher priority number runs first!
    rtos_create_task("High_Monitor", task_high_monitor, 3, 100);
    rtos_create_task("Med_Worker",   task_med_worker,   2, 200);
    rtos_create_task("Low_Worker",   task_low_worker,   1, 500);

    // Launch the Kernel Thread Loop Simulation
    rtos_start();

    // Simulate subsequent iterations of the scheduling wheel
    for(int iteration = 1; iteration <= 5; iteration++) {
        printf("\n>>>>>> SIMULATION WHEEL TICK CYCLE %d <<<<<<\n", iteration);
        rtos_yield();
    }

    printf("\n==================================================\n");
    printf("     RTOS CORE SIMULATION SUCCESSFULLY ENDED      \n");
    printf("==================================================\n");
    return 0;
}
