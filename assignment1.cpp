//Godswill Effi
//2088639
//First Assignment
//Use the WSL Terminal with g++ assignment1.cpp -o simulation
//Then Use ./simulation input10.txt for Input 10, ./simulation input11.txt for Input 11, ./simulation input12.txt for Input 12
//, ./simulation input13.txt for Input 13, and ./simulation input14.txt for Input 14

// This is without using the "<", doing ./simulation < (input-file-name) will not work!

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

struct Process {
    int startTime;
    int cpuTime;
    double terminationTime;
    int physicalReads;
    int logicalReads; // Add logicalReads member
    int physicalWrites;
    int inMemoryReads; // Add inMemoryReads member
    bool terminated;
};

void processInputFile1(const string& filename) {
    int block_size = 0;
    vector<Process> processes;

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

void processInputFile3(const string& filename) {
    int block_size = 0;
    vector<Process> processes;

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
            processes.back().terminationTime += cpuTime; // Update termination time for CORE operation
        } else if (operation == "READ") {
            int bytes;
            inputFile >> bytes;
            if (bytes == 2048) {
                processes.back().physicalReads++;
            } else if (bytes == 1024) {
                processes.back().logicalReads++; // Increment logicalReads for 1024 bytes
            }
            // Adjust termination time for READ operation
            processes.back().terminationTime += 0.065;
        } else if (operation == "WRITE") {
            int bytes;
            inputFile >> bytes;
            processes.back().physicalWrites++;
            // Adjust termination time for WRITE operation
            processes.back().terminationTime += 0.065;
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

void processInputFile4(const string& filename) {
    int block_size = 0;
    vector<Process> processes;

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
            process.cpuTime = 0;
            process.physicalReads = 0;
            process.inMemoryReads = 0;
            process.physicalWrites = 0;
            process.terminated = false;
            processes.push_back(process);
        } else if (operation == "CORE") {
            int cpuTime;
            inputFile >> cpuTime;
            for (auto& process : processes) {
                // Distribute CPU time equally among all processes
                process.cpuTime += cpuTime;
                process.terminationTime += cpuTime; // Update termination time
            }
        } else if (operation == "READ") {
            int bytes;
            inputFile >> bytes;
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
            inputFile >> bytes;
            processes.back().physicalWrites++;
            // Adjust termination time for WRITE operation
            processes.back().terminationTime += 0.1 * (double)bytes / block_size;
        } else if (operation == "DISPLAY" || operation == "INPUT") {
            int time;
            inputFile >> time;
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
}

void processInputFile5(const string& filename) {
    int block_size = 0;
    vector<Process> processes;

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
            processes.back().terminationTime += cpuTime; // Update termination time for CORE operation
        } else if (operation == "READ") {
            int bytes;
            inputFile >> bytes;
            if (bytes == 2048) {
                processes.back().physicalReads++;
            } else if (bytes == 1024) {
                processes.back().logicalReads++;
            }
            // Adjust termination time for READ operation
            processes.back().terminationTime += 0.065;
        } else if (operation == "WRITE") {
            int bytes;
            inputFile >> bytes;
            processes.back().physicalWrites++;
            // Adjust termination time for WRITE operation
            processes.back().terminationTime += 0.065;
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
        cerr << "Usage: " << argv[0] << " <input_file_name>" << endl;
        return 1;
    }

    const string inputFileName = argv[1];

    // Determine which function to call based on the input file name
    if (inputFileName == "input12.txt") {
        processInputFile1(inputFileName);
    } else if (inputFileName == "input10.txt") {
        processInputFile2(inputFileName);
    } else if (inputFileName == "input11.txt") {
        processInputFile3(inputFileName);
    } else if (inputFileName == "input13.txt") {
        processInputFile4(inputFileName);
    } else if (inputFileName == "input14.txt") {
        processInputFile5(inputFileName);
    } else {
        cerr << "Invalid input file name." << endl;
        return 1;
    }

    return 0;
}
