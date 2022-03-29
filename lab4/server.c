#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<time.h>
#define PORT 8080

char* divisor= "1101";
int divlen=4;


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

char* CRC(char* data, char* key){

    int k = strlen(key);
    int n = strlen(data);
    char *aug_data = malloc(n+k); // doubtful about length
    for(int i=0;i<n;i++){
        aug_data[i]=data[i];
    }
    for(int i=0;i<k-1;i++){
        aug_data[n+i]='0';
    }
    // aug_data ok
    printf("binmod2 result: %s\n",bin_mod_2(aug_data,key));
    char* rem= bin_mod_2(aug_data,key);
    // bin_mod_2 ok
    char* final = malloc(n+k);
    for(int i=0;i<n;i++){
        final[i]=data[i];
    }
    for(int i=0;i<k-1;i++){
        final[i+n]=rem[i];
    }
    return final;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    // char* data="100100";
    // printf("CRC output: %s\n",CRC(data,divisor));
    int N;
    printf("enter n (length of string = 8*n): ");
    scanf("%d",&N);
    srand(time(0));
    char* data = malloc(8*N+1);
    for(int i=0;i<8*N;i++)
    data[i]=rand()%2+'0';

    printf("our randomly generated string: %s\n",data);
    char* hello = CRC(data,divisor);
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("data message sent\n");
    return 0;
}

