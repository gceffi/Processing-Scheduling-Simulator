#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Process {
    int startTime;
    int cpuTime;
    double terminationTime;
    int physicalReads;
    int logicalReads;
    int physicalWrites;
};

int main() {
    int block_size = 0;
    vector<Process> processes;

    string operation;
    double lastCoreTime = 0; // Initialize last core time as double
    double coreTimeAccumulated = 0; // Initialize accumulated core time for Process 0
    double maxTerminationTime = 0; // Initialize maximum termination time seen so far
    bool processZeroTerminated = false; // Flag to track if Process 0 is terminated
    while (cin >> operation) {
        if (operation == "BSIZE") {
            cin >> block_size;
        } else if (operation == "START") {
            Process process;
            cin >> process.startTime;
            process.terminationTime = process.startTime; // Initialize termination time
            process.physicalReads = 0;
            process.logicalReads = 0;
            process.physicalWrites = 0;
            processes.push_back(process);
        } else if (operation == "CORE") {
            int cpuTime;
            cin >> cpuTime;
            processes.back().cpuTime = cpuTime;
            processes.back().terminationTime = max(processes.back().terminationTime, lastCoreTime + cpuTime); // Update termination time
            lastCoreTime = processes.back().terminationTime; // Update last core time

            if (processes.size() > 1) { // If there is more than one process
                if (!processZeroTerminated) { // If Process 0 has not terminated yet
                    coreTimeAccumulated += cpuTime; // Accumulate core time
                }
            }

            // Update maximum termination time seen so far
            maxTerminationTime = max(maxTerminationTime, processes.back().terminationTime);
        } else if (operation == "READ") {
            int bytes;
            cin >> bytes;
            processes.back().physicalReads++;
        } else if (operation == "WRITE") {
            int bytes;
            cin >> bytes;
            processes.back().physicalWrites++;

            if (processes.size() > 1) { // If there is more than one process
                if (!processZeroTerminated) { // If Process 0 has not terminated yet
                    processes[0].terminationTime += coreTimeAccumulated; // Adjust Process 0's termination time
                    processZeroTerminated = true; // Set Process 0 as terminated
                }
            }

            // Reset accumulated core time for Process 0
            coreTimeAccumulated = 0;
        }
    }

    // Output process reports and process states
    cout << fixed << setprecision(0);
    for (int i = 0; i < processes.size(); ++i) {
        Process& process = processes[i];
        cout << "Process " << i << " terminates at t = " << static_cast<int>(process.terminationTime) << "ms." << endl;
        cout << "It performed " << process.physicalReads << " physical read(s), "
             << process.logicalReads << " logical read(s), and "
             << process.physicalWrites << " physical write(s)." << endl;
       
        // Output process states after each process report
        cout << "\nProcess states:" << endl;
        cout << "--------------" << endl;
        for (int j = 0; j < processes.size(); ++j) {
            if (j == i) {
                cout << "  " << j << "  TERMINATED" << endl;
            } else {
                cout << "  " << j << "  RUNNING" << endl;
            }
        }
        cout << endl; // Separate process reports
    }

    return 0;
}
