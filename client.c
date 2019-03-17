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
    int k,move;
    int *board;
    int sock_desc;
    struct sockaddr_in client;
    memset(&client,0,sizeof(client));
    
    key_t key = ftok(".",'a');
    int shmid = shmget(key, 9 * sizeof(int), 0666);
    if(shmid<0){
        printf("cannot create shared memory\n");
    }

    board = shmat(shmid, 0, 0);

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

        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                printf("%d "board[3*i+j]);
            }
            printf("\n");
        }

        printf("\nEnter your move(0-8):\nEnter end to quit\n");
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

        printf("Your move has been recorded in: %s",buf);

        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                printf("%d "board[3*i+j]);
            }
            printf("\n");
        }

    }
    close(sock_desc);
    exit(0);
    return 0;
}