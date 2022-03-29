#include<bits/stdc++.h>
#include<chrono>
#define MAXS 10000000
using namespace std;
using namespace std::chrono;

int n;
int nthreads;
int arr[MAXS];

void merge(int* arr,int lo,int mid,int hi){
    int i,j,k;
    int left[mid-lo+1];
    int right[hi-mid];
    for(i=lo;i<mid+1;i++)
    left[i-lo]=arr[i];
    for(i=mid+1;i<hi+1;i++)
    right[i-mid-1]=arr[i];
    i=0;j=0,k=lo;
    while(i<mid-lo+1 && j<hi-mid){
        if(left[i]>right[j]) arr[k++]=right[j++];
        else arr[k++]=left[i++];
    }
    while(i<mid-lo+1){
        arr[k++]=left[i++];
    }
    while(j<hi-mid){
        arr[k++]=right[j++];
    }

}

void merge_sort(int* arr,int lo,int hi){
    
    if(lo>=hi){
        return;
    }
    
    int mid=(lo+hi)/2;
    merge_sort(arr,lo,mid);
    merge_sort(arr,mid+1,hi);

    merge(arr,lo,mid,hi);

}

int main(int argc, char* argv[]){
    int i,j;

    nthreads=stoi(argv[1]);
    n=stoi(argv[2]);
    
    srand (time(NULL));
    for(i=0;i<n;i++) arr[i]=rand()%1000;
    
    cout << "Initial array: ";
    for(i=0;i<n;i++) cout << arr[i] << " ";
    cout << "\n";

    auto start = high_resolution_clock::now();
    merge_sort(arr,0,n-1);
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);
    
    cout << "Final array: ";
    for(i=0;i<n;i++){
        cout << arr[i] << " "; 
    }
    cout << "\n";

    cout << "Total time: ";
    cout << duration.count() << "\n";
}