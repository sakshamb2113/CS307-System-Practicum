#include<bits/stdc++.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

// struct args{
//     int** a;
//     int** b;
//     int** c;
//     int n;
//     int part;
// };

int** a;
int** b;
int** mult;
int n;

void* multiply(void* part){
    int i,j,k;
    int number=*((int*) part);
    // cout << number << endl;
    // if(number==3){
    //     for(i=3*(n/4);i<n;i++){
    //         if(number>=n) break;

    //         for(j=0;j<n;j++){
    //             for(k=0;k<n;k++){
    //                 // cout << i << " " << j << endl;
    //                 mult[i][j]+=a[i][k]*b[k][j];
    //             }
    //         }
    //     }
    // }
    // else{
        for(i=(n*number)/4;i<((number+1)*n)/4;i++){
            for(j=0;j<n;j++){
                for(k=0;k<n;k++){
                    // cout << i << " " << j << endl;
                    mult[i][j]+=a[i][k]*b[k][j];
                }
            }
        }
    // }


}


int main(int argc, char* argv[]){
    auto start = high_resolution_clock::now();

    int i,j,k;
    cout << "Enter size of SQUARE matrix:" << endl;
    n=stoi(argv[1]);
    
    // int a[3][3]={{1,2,3},{4,5,6},{7,8,9}};
    // int b[3][3]={{0,0,1},{2,1,3},{4,0,9}};

    // int mult[3][3]={{0,0,0},{0,0,0},{0,0,0}};


    a = new int*[n];
    for(i=0;i<n;i++) a[i] = new int[n];

    
    b = new int*[n];
    for(i=0;i<n;i++) b[i] = new int[n];


    
    mult = new int*[n];
    for(i=0;i<n;i++) mult[i] = new int[n];

    // args * myargs = new args();

    srand (time(NULL));
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            a[i][j]=rand()%100;
            b[i][j]=rand()%100;
            mult[i][j]=0;
        }
    }


    int sum=0;
    
    cout << "a" << endl;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            cout << a[i][j] << " ";
        }
        cout << endl;
    }

    cout << "b" << endl;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            cout << b[i][j] << " ";
        }
        cout << endl;
    }

    // myargs->a=a;
    // myargs->b=b;
    // myargs->c=mult;
    // myargs->n=n;
    pthread_t * threadar;
    threadar = new pthread_t[4];

    int cnt=4;
    int part[4];
    for(i=0;i<4;i++){
        // *part[i]=i;
        // cout << "hello" << endl;
        // cout << *part[i] << endl;
        // int* p;
        // *p=i;
        part[i]=i;
        // cout << "hello" << endl;

        pthread_create(&threadar[i], NULL, multiply, (void*) (part+i));
    }

    for(i=0;i<4;i++){
        pthread_join(threadar[i], NULL);
    }

    cout << "result" << endl;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            cout << mult[i][j] << " ";
        }
        cout <<  endl;
    }
    delete a;
    delete b;
    delete mult;

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
}