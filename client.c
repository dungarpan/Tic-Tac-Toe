#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("1 arguement is required");
    }

    char buf[100];
    int k;
    int *board;
    int sock_desc;
    struct sockaddr_in client;
    memset(&client,0,sizeof(client));
    
    key_t key = ftok("key_board",65);
    int shmid = shmget(key, 9 * sizeof(int), 0666|IPC_CREAT);
    board = shmat(shmid, 0, 0);

    for(int i=0;i<9;i++){

    }
    sock_desc=socket(AF_INET,SOCK_STREAM,0);

    if(sock_desc==-1)
    {
        printf("Error in socket creation");
        exit(1);
    }

    client.sin_family=AF_INET;
    client.sin_addr.s_addr=INADDR_ANY;
    client.sin_port=atoi(argv[1]);

    k=connect(sock_desc,(struct sockaddr*)&client,sizeof(client));
    if(k==-1)
    {
        printf("Error in connecting to server");
        exit(1);
    }

    while(1)
    {
        printf("\nEnter data to be send to server: ");
        fgets(buf,100,stdin);
        if(strncmp(buf,"end",3)==0)
            break;

        k=send(sock_desc,buf,100,0);
        if(k==-1)
        {
            printf("Error in sending");
            exit(1);
        }

        k=recv(sock_desc,buf,100,0);
        if(k==-1)
        {
            printf("Error in receiving");
            exit(1);
        }

        printf("Message got from server is : %s",buf);
    }
    close(sock_desc);
    exit(0);
    return 0;
}