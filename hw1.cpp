#include <iostream>
#include <vector>

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

    while (cin >> ws) {
        string operation;
        cin >> operation;

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
            processes.back().terminationTime += cpuTime; // Update termination time
        } else if (operation == "READ") {
            int bytes;
            cin >> bytes;
            processes.back().physicalReads++;
            // Adjust termination time for READ operation
            processes.back().terminationTime += 0.1;
        } else if (operation == "WRITE") {
            int bytes;
            cin >> bytes;
            processes.back().physicalWrites++;
            // Adjust termination time for WRITE operation
            processes.back().terminationTime += 0.1;
        } else if (operation == "DISPLAY") {
            int displayTime;
            cin >> displayTime;
            processes.back().terminationTime += displayTime; // Update termination time
        } else if (operation == "INPUT") {
            int inputTime;
            cin >> inputTime;
            processes.back().terminationTime += inputTime; // Update termination time
        }
    }

    // Output process reports
    cout << fixed;
    cout.precision(1);
    for (int i = 0; i < processes.size(); ++i) {
        Process& process = processes[i];
        cout << "Process " << i << " terminates at t = " << process.terminationTime << " ms." << endl;
        cout << "It performed " << process.physicalReads << " physical read(s), "
             << process.logicalReads << " logical read(s), and "
             << process.physicalWrites << " physical write(s)." << endl;
        
        // Output process states
        cout << "\nProcess states:" << endl;
        cout << "--------------" << endl;
        for (int j = 0; j < processes.size(); ++j) {
            if (j == i) {
                cout << "  " << j << "  TERMINATED" << endl;
            } else {
                if (processes[j].terminationTime > process.terminationTime) {
                    cout << "  " << j << "  RUNNING" << endl;
                } else {
                    cout << "  " << j << "  TERMINATED" << endl;
                }
            }
        }
        cout << endl; // Separate process reports
    }

    return 0;
}
