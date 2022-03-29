#include<bits/stdc++.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

int** a;
int** b;
int** mult;
int n;

int main(int argc, char* argv[]){
    auto start = high_resolution_clock::now();
    int n,i,j,k;
    cout << "Enter size of SQUARE matrix:" << endl;

    n=stoi(argv[1]);
    
    a = new int*[n];
    for(i=0;i<n;i++) a[i] = new int[n];

    
    b = new int*[n];
    for(i=0;i<n;i++) b[i] = new int[n];


    // int a[3][3]={{1,2,3},{4,5,6},{7,8,9}};
    // int b[3][3]={{0,0,1},{2,1,3},{4,0,9}};

    // int mult[3][3]={{0,0,0},{0,0,0},{0,0,0}};

    
    mult = new int*[n];
    for(i=0;i<n;i++) mult[i] = new int[n];

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

    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            for(k=0;k<n;k++){
                mult[i][j]+=a[i][k]*b[k][j];
            }
        }
    }

    cout << "mult" << endl;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            cout << mult[i][j] << " ";
        }
        cout << endl;
    }

    delete a;
    delete b;
    delete mult;

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

}