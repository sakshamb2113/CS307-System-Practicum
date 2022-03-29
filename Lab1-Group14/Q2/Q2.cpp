#include <iostream>
#include <unistd.h>
#include <strings.h>
#include <iomanip>
#include <fstream>
#include <pthread.h>
#include <chrono>

using namespace std;

// Number of Students
#define NumStudents 5

// Maximum Thinking Time (in seconds)
#define maxThinkingTime 2

// Eating Time (in seconds)
#define eatingTime 20

// Simulation Time (is seconds)
#define simTime 1800

// Time Scaling Factor
#define scale 1000

// Defining States
string waiting = "Waiting for Spoons";
string oneSpoon = "One spoon acquired.";
string eating = "Both spoons acquired and Eating(20 seconds)";
string thinking = "Thinking (< 2 seconds)";

struct AnalysisData {
    int total_eating_time = 0;
    double total_thinking_time = 0.0;
    double total_waiting_time = 0.0;
} data[NumStudents];

// cout mutex
pthread_mutex_t coutMtx = PTHREAD_MUTEX_INITIALIZER;

// Required 
pthread_mutex_t spoonMtx[NumStudents];

// Will Store the current States of all the Students/Threads
string currStates[NumStudents];

void print(string message) {
    pthread_mutex_lock(&coutMtx);
    cout << message << "\n" << flush;
    pthread_mutex_unlock(&coutMtx);
}

// logger
void log(int threadID, string message) {
    print("Thread " + to_string(threadID) + " : " + message);
}

// Scaling Simulation Time
void Sleep(int x) {
    usleep(x*1e6 / scale);
}

void printStates() {
    for (int i = 0; i < NumStudents; i++) 
        cout << "Thread " << i << " : " << currStates[i] << "\n" << flush;
    
    cout << "------------------------\n" << flush;
}

// State Functions
bool waitingForSpoons(int threadID) {
    pthread_mutex_lock(&coutMtx);
    currStates[threadID] = waiting;
    printStates();
    pthread_mutex_unlock(&coutMtx);

    int leftSpoon = threadID, rightSpoon = (threadID + 1) % 5, rc;
    
    clock_t start = clock();
    bool flag = false;
    while (true) {
        rc = pthread_mutex_trylock(&spoonMtx[leftSpoon]);
        if (rc == 0) {
            flag = true;
            break;
        }

        rc = pthread_mutex_trylock(&spoonMtx[rightSpoon]);
        if (rc == 0) 
            break;
    }
    clock_t end = clock();

    data[threadID].total_waiting_time += double(end - start) / (double(CLOCKS_PER_SEC));

    return flag;
}

bool oneSpoonAquired(int threadID, bool leftSpoonAquired) {
    pthread_mutex_lock(&coutMtx);
    currStates[threadID] = oneSpoon;
    printStates();
    pthread_mutex_unlock(&coutMtx);

    int leftSpoon = threadID, rightSpoon = (threadID + 1) % 5, rc;

    clock_t start = clock();
    if (leftSpoonAquired == false) {
        while (true) {
            rc = pthread_mutex_trylock(&spoonMtx[leftSpoon]);
            if (rc == 0)
                return true;
            else if (clock() - start > 100*CLOCKS_PER_SEC/scale) {
                pthread_mutex_unlock(&spoonMtx[leftSpoon]);
                return false;
            }
        }
    }
    else {
        while (true) {
            rc = pthread_mutex_trylock(&spoonMtx[rightSpoon]);
            if (rc == 0)
                return true;
            else if (clock() - start > 100*CLOCKS_PER_SEC/scale) {
                pthread_mutex_unlock(&spoonMtx[rightSpoon]);
                return false;
            }
        }
    }
}

void eatingFood(int threadID) {
    pthread_mutex_lock(&coutMtx);
    currStates[threadID] = eating;
    printStates();
    pthread_mutex_unlock(&coutMtx);

    Sleep(eatingTime);
    data[threadID].total_eating_time += 20;
}

void thinkingState(int threadID) {
    pthread_mutex_lock(&coutMtx);
    currStates[threadID] = thinking;
    printStates();
    pthread_mutex_unlock(&coutMtx);
    
    int leftSpoon = threadID, rightSpoon = (threadID + 1) % 5;

    pthread_mutex_unlock(&spoonMtx[leftSpoon]);
    pthread_mutex_unlock(&spoonMtx[rightSpoon]);

    int waitTime = rand() % int(maxThinkingTime * 1e6);
    usleep((waitTime) / scale);

    data[threadID].total_thinking_time += waitTime/1e6;
}

// Student Thread Function
void *Student(void *threadID) {
    int tid = (long)threadID;

    bool leftSpoonAquired = 0;

    while (true) {
        leftSpoonAquired = waitingForSpoons(tid);

        if (oneSpoonAquired(tid, leftSpoonAquired) == false)
            continue;

        eatingFood(tid);

        thinkingState(tid);
    }

    pthread_exit(NULL);
}

int main() {
    // Redirecting Output to log File
    ofstream logFile("logs.txt");
    streambuf* stream_buffer_file = logFile.rdbuf(); 
    cout.rdbuf(stream_buffer_file);

    // Initializing state-variables
    for (int i = 0; i < NumStudents; i++) {
        currStates[i] = waiting;
        spoonMtx[i] = PTHREAD_MUTEX_INITIALIZER;
    }

    // Creating new Threads
    pthread_t students[NumStudents];
    long rc, i;

    // Initializing Threads
    for (i = 0; i < NumStudents; i++) {
        // log(0, "Creating New Thread");
        rc = pthread_create(&students[i], NULL, Student, (void *)i);

        if (rc) {
            cerr << "Error Creating New Thread, " << rc << "\n";
            exit(-1);
        }
    }

    // Waiting for 30 simulation minutes
    Sleep(simTime);

    // Killing other threads
    for (i = 0; i < NumStudents; i++) {
        rc = pthread_cancel(students[i]);
    }

    cout << "Eating : Thinking : Waiting\n";
    for (int i = 0; i < NumStudents; i++) {
        cout << data[i].total_eating_time << " : " << data[i].total_thinking_time << " : " << data[i].total_waiting_time << "\n";
    }

    return 0;
}
