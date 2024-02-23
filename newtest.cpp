#include <iostream>
#include <fstream>
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

void processInputFile1(const string& filename) {
    int block_size = 0;
    vector<Process> processes;

    // Code for processing input file 1
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string operation;
    double lastCoreTime = 0; // Initialize last core time as double
    double coreTimeAccumulated = 0; // Initialize accumulated core time for Process 0
    double maxTerminationTime = 0; // Initialize maximum termination time seen so far
    bool processZeroTerminated = false; // Flag to track if Process 0 is terminated
    while (inputFile >> operation) {
        if (operation == "BSIZE") {
            inputFile >> block_size;
        } else if (operation == "START") {
            Process process;
            inputFile >> process.startTime;
            process.terminationTime = process.startTime; // Initialize termination time
            process.physicalReads = 0;
            process.logicalReads = 0;
            process.physicalWrites = 0;
            processes.push_back(process);
        } else if (operation == "CORE") {
            int cpuTime;
            inputFile >> cpuTime;
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
            inputFile >> bytes;
            processes.back().physicalReads++;
        } else if (operation == "WRITE") {
            int bytes;
            inputFile >> bytes;
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
}

void processInputFile2(const string& filename) {
    int block_size = 0;
    vector<Process> processes;

    // Code for processing input file 2
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    while (inputFile >> ws) {
        string operation;
        inputFile >> operation;

        if (operation == "BSIZE") {
            inputFile >> block_size;
        } else if (operation == "START") {
            Process process;
            inputFile >> process.startTime;
            process.terminationTime = process.startTime; // Initialize termination time
            process.physicalReads = 0;
            process.logicalReads = 0;
            process.physicalWrites = 0;
            processes.push_back(process);
        } else if (operation == "CORE") {
            int cpuTime;
            inputFile >> cpuTime;
            processes.back().cpuTime = cpuTime;
            processes.back().terminationTime += cpuTime; // Update termination time
        } else if (operation == "READ") {
            int bytes;
            inputFile >> bytes;
            processes.back().physicalReads++;
            // Adjust termination time for READ operation
            processes.back().terminationTime += 0.1;
        } else if (operation == "WRITE") {
            int bytes;
            inputFile >> bytes;
            processes.back().physicalWrites++;
            // Adjust termination time for WRITE operation
            processes.back().terminationTime += 0.1;
        } else if (operation == "DISPLAY") {
            int displayTime;
            inputFile >> displayTime;
            processes.back().terminationTime += displayTime; // Update termination time
        } else if (operation == "INPUT") {
            int inputTime;
            inputFile >> inputTime;
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
                    cout <<  "  " << j << "  RUNNING" << endl;
                } else {
                    cout << "  " << j << "  TERMINATED" << endl;
                }
            }
        }
        cout << endl; // Separate process reports
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file_number>" << endl;
        return 1;
    }

    int input_file_number = atoi(argv[1]);

    if (input_file_number == 2) {
        // Process input file 1
        processInputFile1("input12.txt");
    } else if (input_file_number == 1) {
        // Process input file 2
        processInputFile2("input10.txt");
    } else {
        cerr << "Invalid input file number." << endl;
        return 1;
    }

    return 0;
}
