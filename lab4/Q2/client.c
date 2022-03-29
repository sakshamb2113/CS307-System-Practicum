#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080
   
char* divisor= "11001";
char* errorstring="110001011001101011001011010010110000";  // for 3 bytes
                // 010000110101001000010001100000111111
int divlen=5;
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

int CRC(char* data, char* key){

    int k = strlen(key);
    int n = strlen(data);
    char *aug_data = malloc(n+k);
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

char* CRCUtil(int sock, char* data, char* key){
    int k =strlen(key);
    int len = strlen(data);
    int nn = 8+k-1;
    char* buffer = malloc(nn+1);
    for(int l=0;l<len;l+=nn){
        for(int i=0;i<nn;i++){
            buffer[i]=data[i];
        }
        buffer[nn]='\0';
        int ok=CRC(buffer, key);
        if(ok==1){
            printf("there is no error in byte #%d \n",l/nn);   
        }
        else{
            printf("there was some error in the transmission of byte #%d.\n",l/nn);
        }
    }
}


int main(int argc, char const *argv[])
{
    //sock will be the file descriptor
    int csoc=0, vread;
    struct sockaddr_in serv_addr;
    char *ini = "hello from client";
    char buffer[1024]={0};
    // AF_INET for ipv4 protocol
    // SOCK_STREAM for tcp
    // 0 protocol value for IP 
    if ((csoc=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        printf("\nerror in creating socket\n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nerror:Invalid address\n");
        return -1;
    }
   
    if (connect(csoc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nerror in Connection\n");
        return -1;
    }
    send(csoc, ini, strlen(ini), 0);
    vread = read(csoc, buffer, 1024);
    printf("%s\n",buffer);
    int n=0;
    while(buffer[n]!='\0'){
        n++;
    }
    char* data = malloc(n+1);
    for(int i=0;i<n+1;i++){
        data[i]=buffer[i];
    }
    data=errorstring; // use for showing in case of wrong transmission 
    CRCUtil(csoc,data,divisor);
    return 0;
}