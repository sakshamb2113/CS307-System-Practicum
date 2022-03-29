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

void * ThreadedMS(void* arg){
    int part = *((int *) arg);
    int lo = (part*n)/nthreads;
    int hi = ((part+1)*n)/nthreads; 
    merge_sort(arr,lo,hi-1);
}
//first call 0,n/4-1
//second call n/4,n/2-1
//third call n/2,(3*n)/4-1
//fourth call (3*n)/4,n-1

void merge_final(){
    // merge(arr,0,n/4-1,n/2-1);
    // merge(arr,n/2,(3*n)/4-1,n-1);
    // merge(arr,0,n/2-1,n-1);
    vector<pair<int,int>> v;
    for(int i=0;i<nthreads;i++){
        v.push_back({(i*n)/nthreads,((i+1)*n)/nthreads-1}); 
    }
    
    int pre=0,nxtpre=nthreads,curr=1;
    while(curr!=0){ 
        curr=0;
        for(int i=0;i<nxtpre/2;i++){
            
            merge(arr,v[pre+2*i].first,v[pre+2*i].second,v[pre+2*i+1].second);
            v.push_back({v[pre+2*i].first,v[pre+2*i+1].second});
            curr++;
        }
        if(nxtpre%2 && nxtpre!=1){
            v.push_back({v[pre+nxtpre-1].first,v[pre+nxtpre-1].second});
            curr++;
        }
        pre+=nxtpre;
        nxtpre=curr;
    }

    

}

int main(int argc, char* argv[]){
    int i,j;
    nthreads=stoi(argv[1]);
    n=stoi(argv[2]);
    // for(i=0;i<n;i++){
    //     cin >> arr[i];
    // }

    srand (time(NULL));
    for(i=0;i<n;i++) arr[i]=rand()%1000;

    cout << "Initial array: ";
    for(i=0;i<n;i++) cout << arr[i] << " ";
    cout << "\n";

    
    auto start = high_resolution_clock::now();

    pthread_t* threads = new pthread_t[nthreads];
    int* part = new int[nthreads];
    for(i=0;i<nthreads;i++){
        part[i]=i;
        pthread_create(&threads[i], NULL, ThreadedMS, (void*) (part+i));
    }

    for(i=0;i<nthreads;i++){
        pthread_join(threads[i],NULL);
    }

    merge_final();
    
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