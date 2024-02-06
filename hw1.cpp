#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Process status enum
enum ProcessStatus {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

// Process structure
struct Process {
    int startTime;
    int currentStep;
    int totalSteps;
    int cpuTime;
    int ssdReads;
    int ssdWrites;
    ProcessStatus status;
};

// SSD queue structure
queue<Process*> ssdQueue;

// Process table
vector<Process> processTable;

// Function to simulate SSD access
void simulateSSDAccess(Process* process, int elapsedTime) {
    // Simulate SSD read
    cout << "Simulating SSD read for Process " << process->startTime << " at time " << elapsedTime << " ms\n";
}

// Function to simulate SSD write
void simulateSSDWrite(Process* process, int elapsedTime) {
    // Simulate SSD write
    cout << "Simulating SSD write for Process " << process->startTime << " at time " << elapsedTime << " ms\n";
}

// Function to handle process termination
void terminateProcess(Process* process, int elapsedTime) {
    // Calculate termination time with fractional milliseconds
    double terminationTime = process->startTime + process->cpuTime;

    // Print debug information
    cout << "Start Time: " << process->startTime << " ms" << endl;
    cout << "CPU Time: " << process->cpuTime << " ms" << endl;
    cout << "Termination Time: " << terminationTime << " ms" << endl;
    
    // Output process report
    cout << "Process " << process->startTime << " terminates at t = " << terminationTime << " ms." << endl;
    cout << "It performed 1" << " physical read(s), 0 logical read(s), and "
         << process->ssdWrites << " physical write(s)." << endl;

    // Update process status to TERMINATED and termination time
    process->status = TERMINATED;
}

// Function to handle process loading into memory
void loadProcess(int startTime, int cpuTime, int ssdRead, int ssdWrite) {
    Process process;
    process.startTime = startTime;
    process.currentStep = 0;
    process.totalSteps = 4; // Assuming each process has 4 steps
    process.cpuTime = cpuTime;
    process.ssdReads = 0;
    process.ssdWrites = 0;
    process.status = READY;

    processTable.push_back(process);
}

// Function to simulate CPU scheduling
void simulateCPU(int& elapsedTime) {
    // FCFS scheduling - process at front of the queue gets CPU
    if (!processTable.empty() && processTable.front().status == READY) {
        Process* currentProcess = &processTable.front();
        currentProcess->status = RUNNING;

        // Simulate CPU time
        cout << "Simulating CPU time for Process " << currentProcess->startTime << " at time " << elapsedTime << " ms\n";
        // ...

        // Update process statistics
        currentProcess->currentStep++;

        // Check if the process has completed all steps
        if (currentProcess->currentStep == currentProcess->totalSteps) {
            // Process termination
            currentProcess->status = TERMINATED;
            terminateProcess(currentProcess, elapsedTime);
            processTable.erase(processTable.begin()); // Remove terminated process from the table
        } else {
            // Process is not terminated, move it to the end of the queue
            currentProcess->status = READY;
            processTable.push_back(*currentProcess);
            processTable.erase(processTable.begin()); // Remove the running process from the front
        }
    }
}

// Function to simulate SSD scheduling
void simulateSSD(int& elapsedTime) {
    // FCFS scheduling - process at front of the queue gets SSD
    if (!ssdQueue.empty()) {
        Process* ssdProcess = ssdQueue.front();

        // Simulate SSD access
        simulateSSDAccess(ssdProcess, elapsedTime);

        // Simulate SSD write (if needed)
        if (ssdProcess->currentStep == 2) {
            simulateSSDWrite(ssdProcess, elapsedTime);
        }

        // Move the process back to the READY queue
        ssdProcess->status = READY;
        processTable.push_back(*ssdProcess);
        ssdQueue.pop(); // Remove the process from the SSD queue
    }
}

int main() {
    int elapsedTime = 0;
    int block_size = 0; // to store block size

    while (cin >> ws) {
        string operation;
        cin >> operation;

        if (operation == "BSIZE") {
            // Read block size
            cin >> block_size;
        } else if (operation == "START") {
            // Process starts
            int startTime;
            cin >> startTime;
            loadProcess(startTime, 0, 0, 0);
        } else if (operation == "CORE") {
            // CPU time request
            int cpuTime;
            cin >> cpuTime;
            // Update current process's CPU time
            processTable.back().cpuTime = cpuTime;
        } else if (operation == "READ") {
            // SSD read request
            int ssdRead;
            cin >> ssdRead;
            // Update current process's SSD read
            processTable.back().ssdReads = ssdRead;
        } else if (operation == "WRITE") {
            // SSD write request
            int ssdWrite;
            cin >> ssdWrite;
            // Update current process's SSD write
            processTable.back().ssdWrites = ssdWrite;
        } else if (operation == "DISPLAY") {
            // Display request
            int displayTime;
            cin >> displayTime;
            // Simulate display time
            // ...
        } else if (operation == "INPUT") {
            // Input request
            int inputTime;
            cin >> inputTime;
            // Simulate input time
            // ...
        }

        // Simulate time passing
        elapsedTime++;

        // Simulate CPU scheduling
        simulateCPU(elapsedTime);

        // Simulate SSD scheduling
        simulateSSD(elapsedTime);
    }

    return 0;
}
