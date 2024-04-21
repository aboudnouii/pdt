#include <stdio.h>
#include <stdlib.h>

// Structure for representing memory partitions
typedef struct {
    int state; // State of the partition (0=free, 1=allocated)
    int size;  // Size of the partition
    int baseAddress;  // Starting address of the partition
} MemoryPartition;

// Structure for holding program details in memory, including execution time
typedef struct {
    int programId;        // Identifier for the program (0 indicates no program)
    int partitionSize;    // Size of the partition allocated to the program
    int baseAddress;      // Starting address of the partition
    int executionTime;    // Execution time of the program
} MemoryWindow;

// Linked list node structure for processes
typedef struct ProcessNode {
    char identifier;              // Process identifier, for example, 'p' for process
    int size;                     // Memory size needed by the process
    int executionTime;            // Time required to execute the process
    struct ProcessNode* next;     // Pointer to the next process in the list
} ProcessNode;

ProcessNode* processList = NULL;  // Head pointer for the list of processes

// Initializes memory partitions based on user input
void createPartitions(MemoryPartition partitions[], int count) {
    for (int i = 0; i < count; i++) {
        printf("Enter state for partition %d (0 for free, 1 for allocated): ", i + 1);
        scanf("%d", &partitions[i].state);
        printf("Enter size for partition %d: ", i + 1);
        scanf("%d", &partitions[i].size);
        // Compute base address for each partition based on the previous partition's size
        partitions[i].baseAddress = (i == 0) ? 0 : partitions[i - 1].baseAddress + partitions[i - 1].size;
    }
}

// Displays the current state of memory partitions
void displayPartitions(MemoryPartition partitions[], int count) {
    printf("\n+----------------- Memory Partitions -----------------+\n");
    printf("| Partition | State (0=Free, 1=Allocated) | Size | Base Address |\n");
    printf("+------------------------------------------------------+\n");
    // Iterate over partitions to print their details
    for (int i = 0; i < count; i++) {
        printf("| %9d | %28d | %4d | %12d |\n", i + 1, partitions[i].state, partitions[i].size, partitions[i].baseAddress);
    }
    printf("+------------------------------------------------------+\n");
}

// Displays the main memory allocation for programs
void displayMemory(MemoryWindow memory[], int count) {
    printf("\n+---------------------- Main Memory ----------------------+\n");
    printf("| Partition | Program | Size | Base Address | Exec Time |\n");
    printf("+---------------------------------------------------------+\n");
    // Iterate over memory to print program allocation details
    for (int i = 0; i < count; i++) {
        printf("| %9d | %7d | %4d | %12d | %9d |\n", i + 1, memory[i].programId, memory[i].partitionSize, memory[i].baseAddress, memory[i].executionTime);
    }
    printf("+---------------------------------------------------------+\n");
}

// Initializes the main memory window from the partition table
void initializeMemory(MemoryWindow memory[], MemoryPartition partitions[], int count) {
    for (int i = 0; i < count; i++) {
        // Set initial values for memory window from partition data
        memory[i].programId = 0; // No program allocated initially
        memory[i].partitionSize = partitions[i].size;
        memory[i].baseAddress = partitions[i].baseAddress;
        memory[i].executionTime = 0; // Initial execution time is 0
    }
}

// Allocates memory to a process using first-fit algorithm
void allocateFirstFit(ProcessNode* process, MemoryPartition partitions[], MemoryWindow memory[], int partitionCount, int processId, int executionTime) {
    int allocated = 0; // Flag to check if allocation is successful
    for (int i = 0; i < partitionCount && !allocated; i++) {
        // Check if partition is free and big enough for the process
        if (partitions[i].state == 0 && partitions[i].size >= process->size) {
            // Allocate partition to the process
            memory[i].programId = processId;
            memory[i].executionTime = executionTime;
            partitions[i].state = 1; // Mark partition as allocated
            allocated = 1; // Set flag to indicate successful allocation
        }
    }
    if (!allocated) {
        printf("Memory allocation failed: insufficient partition size.\n");
    }
    displayPartitions(partitions, partitionCount); // Show updated partitions
}

// Deallocates the process with the shortest execution time
void deallocateShortestExecutionTime(MemoryWindow memory[], int count) {
    int shortestTime = 0; // Start with 0, indicating no time yet found
    int shortestIndex = -1; // Index of the process with the shortest time

    // Iterate over memory to find the process with the shortest execution time
    for (int i = 0; i < count; i++) {
        // Check if there's a program and if it has the shortest execution time seen so far
        if (memory[i].programId != 0 && (shortestTime == 0 || memory[i].executionTime < shortestTime)) {
            shortestTime = memory[i].executionTime;
            shortestIndex = i; // Update index of the shortest execution time
        }
    }

    // Deallocate process if found
    if (shortestIndex != -1) {
        printf("Deallocating process %d with execution time %d.\n", memory[shortestIndex].programId, memory[shortestIndex].executionTime);
        memory[shortestIndex].programId = 0; // Reset program ID to indicate deallocation
        memory[shortestIndex].executionTime = 0; // Reset execution time
    } else {
        printf("No process found for deallocation.\n");
    }
}

// Main function to drive the program
int main() {
    int partitionCount;
    printf("\nEnter number of partitions: ");
    scanf("%d", &partitionCount);
    MemoryPartition partitions[partitionCount];
    MemoryWindow memory[partitionCount];

    createPartitions(partitions, partitionCount); // Initialize partitions
    displayPartitions(partitions, partitionCount); // Display initial state of partitions

    initializeMemory(memory, partitions, partitionCount); // Setup main memory
    displayMemory(memory, partitionCount); // Display initial state of main memory

    int processId = 0, userInput = 0;
    do {
        processId++;
        ProcessNode* newProcess = (ProcessNode*)malloc(sizeof(ProcessNode)); // Create a new process
        printf("Enter size of process num %d: ", processId);
        scanf("%d", &newProcess->size);
        printf("Enter execution time: ");
        scanf("%d", &newProcess->executionTime);
        newProcess->identifier = 'p'; // Set process identifier
        newProcess->next = processList; // Insert process at the head of the list
        processList = newProcess;

        allocateFirstFit(newProcess, partitions, memory, partitionCount, processId, newProcess->executionTime); // Allocate memory to process
        printf("If you want to add another process enter 1, if not enter 0: ");
        scanf("%d", &userInput);
    } while (userInput == 1);

    printf("\nFinal state of main memory:\n");
    displayMemory(memory, partitionCount); // Display memory after allocations

    deallocateShortestExecutionTime(memory, partitionCount); // Deallocate process with shortest execution time
    printf("\nState of main memory after deallocation:\n");
    displayMemory(memory, partitionCount); // Display memory after deallocation

    return 0;
}