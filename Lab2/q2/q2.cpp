#include <bits/stdc++.h>
using namespace std;
bool cmp(pair<int, int>& a, 
         pair<int, int>& b) 
{ 
    return a.second < b.second; 
}
int main() {
	int N, time_quantum, pending_pointer = 0, time = 0;
	cout << "Enter number of process: ";
	cin >> N;
	cout << "\n";
	int arrival[N],completion[N],remaining[N],start[N], waiting[N], TurnAround[N], response[N];
	float avgWaitTime = 0, avgTAT = 0, avgResponseTime = 0, throughput;
	for(int i = 0; i < N; i++) {
		start[i] = -1;
	}
	cout << "Enter Arrival Time of Each process: ";
	for(int i = 0; i < N; i++) {
		cin >> arrival[i];	
	}
	cout << "\n";
	cout << "Enter Burst Time of Each process: ";
	int burst[N];
	for(int i = 0; i < N; i++) {
		cin >> burst[i];
		remaining[i] = burst[i];
	}
	cout << "\n";
	cout << "Enter Time Quantum:";
	cin >> time_quantum;
	cout << "\n";
	vector<pair<int, int> > sort_arrival;
	for(int i = 0; i < N; i++) {
		sort_arrival.push_back(pair<int, int >(i, arrival[i]));
	}
	sort(sort_arrival.begin(), sort_arrival.end(), cmp);
	queue<int> readyQueue;
	while(pending_pointer != N || !readyQueue.empty()) {
		if(readyQueue.empty()) {
			int nextArrivaltime = sort_arrival[pending_pointer].second;
			while(sort_arrival[pending_pointer].second == nextArrivaltime) {
				readyQueue.push(sort_arrival[pending_pointer].first);
				pending_pointer++;
			}
			time =  nextArrivaltime;
		}
		while(!readyQueue.empty()) {
			int current_process = readyQueue.front();
			readyQueue.pop();
			if(start[current_process] == -1) {
				start[current_process] = time;
			}
			if(remaining[current_process] <= time_quantum) {
				time = time + remaining[current_process];
				completion[current_process] = time;
				while(pending_pointer< N && sort_arrival[pending_pointer].second <= time) {
					readyQueue.push(sort_arrival[pending_pointer].first);
					pending_pointer++;
				}
			}
			else {
				remaining[current_process] = remaining[current_process] - time_quantum;
				time = time + time_quantum;
				while(pending_pointer< N && sort_arrival[pending_pointer].second <= time) {
					readyQueue.push(sort_arrival[pending_pointer].first);
					pending_pointer++;
				}
				readyQueue.push(current_process);
			}			
		}
	}
	for(int i = 0; i < N; i++) {
		TurnAround[i] = completion[i] - arrival[i];
		avgTAT = avgTAT + TurnAround[i];
	}
	avgTAT = avgTAT/N;
	for(int i = 0; i < N; i++) {
		waiting[i] = TurnAround[i] - burst[i];
		avgWaitTime = avgWaitTime + waiting[i]; 
	}
	avgWaitTime = avgWaitTime/N;
	for(int i = 0; i < N; i++) {
		response[i] = start[i] - arrival[i];
		avgResponseTime = avgResponseTime + response[i];
	}
	avgResponseTime = avgResponseTime/N;
	throughput = time/(N*1.00);
	cout << "Process Number" << "\t" << "Arrival Time" << "\t" << "Burst Time   " << "\t" << "Start Time" << "\t" << "Completion Time" << "\t" << "Turn Around" << "  " << "Waiting Time" <<" Response Time"<< "\n";
	for(int i = 0; i < N; i++) {
		cout << i << "\t \t" << arrival[i] << "\t \t" << burst[i] << "\t  \t" << start[i] << "\t \t" << completion[i] << "\t \t" << TurnAround[i] << "\t \t" << waiting[i] << "\t \t" << response[i] << "\n";	
	}
	cout << "Average Turn Around Time:" << avgTAT << "\n";
	cout << "Average Waiting Time:    " << avgWaitTime << "\n";
	cout << "Average Response Time:   " << avgResponseTime << "\n";	
	cout << "Throughput               " << throughput << "\n";
}
