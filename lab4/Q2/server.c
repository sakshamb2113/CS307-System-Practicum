#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<time.h>
#define PORT 8080

char* divisor= "11001";
int divlen=5;


char* bin_mod_2(char* aug_data, char* key){
    int k = strlen(key);
    int n = strlen(aug_data);
    char* CurrString=malloc(k);
    char* zeroStr=malloc(k);
    for(int i=0;i<k;i++){
        zeroStr[i]='0';
    }
    for(int i=0;i<k-1;i++){
        CurrString[i]=((aug_data[i+1]-'0')^(key[i+1]-'0'))+'0';
    }

    for(int l=k;l<n;l++){
        CurrString[k-1]=aug_data[l]; // copy next bit

        // take XOR
        if(CurrString[0]!='0'){
            for(int i=0;i<k;i++){
                CurrString[i]=((CurrString[i]-'0')^(key[i]-'0'))+'0';
            }
        }        
        for(int i=0;i<k-1;i++)
        CurrString[i]=CurrString[i+1]; // shift string 1 step to left
    }
    return CurrString;
}

char* CRC(char* data, char* key){

    int k = strlen(key);
    int n = strlen(data);
    char *aug_data = malloc(n+k);
    for(int i=0;i<n;i++){
        aug_data[i]=data[i];
    }
    for(int i=0;i<k-1;i++){
        aug_data[n+i]='0';
    }
    printf("binmod2 result: %s\n",bin_mod_2(aug_data,key));
    char* rem= bin_mod_2(aug_data,key);
    return rem;
}

char* CRCUtil(char* data, char* key){
    char* buffer = malloc(9);
    int k=strlen(key);
    int n=strlen(data);
    char* newstring = malloc(n+(n/8)*(k-1)); 
    for(int l=0;l<n;l+=8){
        for(int i=0;i<8;i++){
            buffer[i]=data[i+l];
        }

        buffer[8]='\0';
        char* temp;
        temp=CRC(buffer,key);
        char* rem = malloc(k);
        for(int i=0;i<k-1;i++){
            rem[i]=temp[i];
        }
        rem[k-1]='\0';
        strcat(newstring,buffer);
        strcat(newstring,rem);
    }
    printf("remstring: %s\n",newstring);
    return newstring;
}

int main(int argc, char const *argv[])
{
    int sfd, mysock, vread;
    struct sockaddr_in address;
    int opt=1;
    int addrlen=sizeof(address);
    char buffer[1024]={0};
    int N;
    printf("enter n (length of string = 8*n): ");
    scanf("%d",&N);
    srand(time(0));
    char* data = malloc(8*N+1);
    for(int i=0;i<8*N;i++)
    data[i]=rand()%2+'0';

    printf("our randomly generated string: %s\n",data);
    char* newstring = CRCUtil(data,divisor);

    // Creating socket file descriptor
    if((sfd = socket(AF_INET, SOCK_STREAM, 0))==0){
        perror("creating socket fd failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT,&opt, sizeof(opt))){
        perror("error in set sockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(sfd, (struct sockaddr *)&address,sizeof(address))<0){
        perror("error in bind");
        exit(EXIT_FAILURE);
    }
    //listen for connection request
    if (listen(sfd, 3)<0){
        perror("error in listen");
        exit(EXIT_FAILURE);
    }
    //accept new connections
    if ((mysock = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
        perror("error in accept");
        exit(EXIT_FAILURE);
    }
    // read incoming message
    vread = read(mysock , buffer, 1024);
    printf("%s\n",buffer );
    strcat(data,newstring);
    // send data
    send(mysock , newstring, strlen(newstring), 0);
    printf("data message sent\n");

    return 0;
}
