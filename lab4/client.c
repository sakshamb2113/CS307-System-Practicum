// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080
   
char* divisor= "1101";
char* errorstring="100000001";
int divlen=4;
int N=5;

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
    // printf("CurrString: %s\n", CurrString);
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
        // printf("CurrString: %s\n", CurrString);
    }
    return CurrString;
}

int CRC(char* data, char* key){

    int k = strlen(key);
    int n = strlen(data);
    char *aug_data = malloc(n+k); // doubtful about length
    aug_data = data; // no need to append zeros
    printf("binmod2 result(rem): %s\n",bin_mod_2(aug_data,key));
    char* rem= bin_mod_2(aug_data,key);
    //check whether rem is 0
    int flg=0;
    for(int i=0;i<k-1;i++){
        if(rem[i]!='0'){
            flg=1;
        }   
    }
    if(flg){
        return 0;
    }
    else{
        return 1;
    }
}

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    int n=0;
    while(buffer[n]!='\0'){
        n++;
    }
    char* data = malloc(n+1);
    for(int i=0;i<n+1;i++){
        data[i]=buffer[i];
    }
    // data=errorstring;
    int isok=CRC(data,divisor);
    if(isok==1){
        printf("there is no error");   
    }
    else{
        printf("there was some error in the transmission.");
    }
    printf("%d\n",CRC(data,divisor));
    return 0;
}