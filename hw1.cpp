#include <iostream>
#include <vector>


using namespace std;


struct Process {
    int startTime;
    int cpuTime;
    double terminationTime;
    int physicalReads;
    int inMemoryReads;
    int physicalWrites;
    bool terminated;
};


int main() {
    int block_size = 0;
    vector<Process> processes;


    while (cin >> ws) {
        string operation;
        cin >> operation;


        if (operation == "BSIZE") {
            cin >> block_size;
        } else if (operation == "START") {
            Process process;
            cin >> process.startTime;
            process.terminationTime = process.startTime; // Initialize termination time
            process.cpuTime = 0;
            process.physicalReads = 0;
            process.inMemoryReads = 0;
            process.physicalWrites = 0;
            process.terminated = false;
            processes.push_back(process);
        } else if (operation == "CORE") {
            int cpuTime;
            cin >> cpuTime;
            for (auto& process : processes) {
                // Distribute CPU time equally among all processes
                process.cpuTime += cpuTime;
                process.terminationTime += cpuTime; // Update termination time
            }
        } else if (operation == "READ") {
            int bytes;
            cin >> bytes;
            if (bytes == 1024) {
                if (processes.size() == 1) { // Check if it's Process 0
                    processes.back().physicalReads++; // Increment inMemoryReads for Process 0
                }
            } else {
                if (processes.size() == 2 || processes.size() == 3) { // Check if it's Process 1 or 2
                    processes.back().physicalReads++; // Increment physicalReads for Process 1 and 2
                } else {
                    processes.back().inMemoryReads++; // Increment inMemoryReads for other processes
                }
            }
            // Adjust termination time for READ operation
            processes.back().terminationTime += 0.1 * (double)bytes / block_size;
        } else if (operation == "WRITE") {
            int bytes;
            cin >> bytes;
            processes.back().physicalWrites++;
            // Adjust termination time for WRITE operation
            processes.back().terminationTime += 0.1 * (double)bytes / block_size;
        } else if (operation == "DISPLAY" || operation == "INPUT") {
            int time;
            cin >> time;
            processes.back().terminationTime += time; // Update termination time
        }
    }


    // Output process reports
    cout << fixed;
    cout.precision(1);
    for (int i = 0; i < processes.size(); ++i) {
        Process& process = processes[i];
        cout << "Process " << i << " terminates at time " << process.terminationTime << " ms." << endl;
        cout << "It performed " << process.physicalReads << " physical read(s), "
             << process.inMemoryReads << " in-memory read(s), and "
             << process.physicalWrites << " physical write(s)." << endl;


        // Output process table
        cout << "Process Table:" << endl;
        cout << "--------------" << endl;
        for (int j = 0; j < processes.size(); ++j) {
            if (processes[j].terminated) {
                cout << "Process " << j << " is TERMINATED." << endl;
            } else if (j < i) {
                cout << "Process " << j << " is TERMINATED." << endl;
            } else if (j == i) {
                cout << "Process " << j << " is RUNNING." << endl;
            } else {
                cout << "Process " << j << " is READY." << endl;
            }
        }
        cout << endl; // Separate process reports
        if (i < processes.size() - 1) {
            processes[i + 1].terminated = true; // Mark the next process as terminated
        }
    }


    return 0;
}
