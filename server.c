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
		printf("1 arguement is required\n");
	}
    char buf[100];
    pid_t pid;
    int k;
    int *board;
    socklen_t len;
    int sock_desc,temp_sock_desc;
    struct sockaddr_in server,client;

    key_t key = ftok(".",'a');
    int shmid = shmget(key, 9 * sizeof(int), IPC_CREAT | 0666);
    if(shmid<0){
    	printf("cannot create shared memory\n");
    }
    board = shmat(shmid, 0, 0);

    for(int i=0;i<3;i++){
    	for(int j=0;j<3;j++){
        	board[3*i + j] = 0;
    	}
    }


    memset(&server,0,sizeof(server));
    memset(&client,0,sizeof(client));

    sock_desc=socket(AF_INET,SOCK_STREAM,0);
    if(sock_desc==-1)
    {
        printf("Error in socket creation");
        exit(1);
    }

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port = atoi(argv[1]);

    k=bind(sock_desc,(struct sockaddr*)&server,sizeof(server));
    if(k==-1)
    {
        printf("Error in binding");
        exit(1);
    }

    k=listen(sock_desc,20);
    if(k==-1)
    {
        printf("Error in listening");
        exit(1);
    }

    pid = fork();
    if(pid>0){
	    len=sizeof(client);//VERY IMPORTANT
	    temp_sock_desc=accept(sock_desc,(struct sockaddr*)&client,&len);
	    if(temp_sock_desc==-1)
	    {
	        printf("Error in temporary socket creation");
	        exit(1);
	    }

	    while(1)
	    {
	    	printf("Player 1's turn\n");
	    	for(int i=0;i<9;i++){
	    		printf("%d ",board[i]);
	    	}
	        k=recv(temp_sock_desc,buf,100,0);
	        if(k==-1)
	        {
	            printf("Error in receiving");
	            exit(1);
	        }

	        printf("Message got from player 1 is : %s",buf);
	        printf("\nEnter data to be sent to client: ");

	        fgets(buf,100,stdin);
	        if(strncmp(buf,"end",3)==0)
	            break;

	        k=send(temp_sock_desc,buf,100,0);
	        if(k==-1)
	        {
	            printf("Error in sending");
	            exit(1);
	        }
	    }
	    close(temp_sock_desc);
	}

	else{
	    len=sizeof(client);//VERY IMPORTANT
	    temp_sock_desc=accept(sock_desc,(struct sockaddr*)&client,&len);
	    if(temp_sock_desc==-1)
	    {
	        printf("Error in temporary socket creation");
	        exit(1);
	    }

	    while(1)
	    {
	    	printf("Player 2's turn\n");
	        k=recv(temp_sock_desc,buf,100,0);
	        if(k==-1)
	        {
	            printf("Error in receiving");
	            exit(1);
	        }

	        printf("Message got from player 2 is : %s",buf);
	        printf("\nEnter data to be sent to client: ");

	        fgets(buf,100,stdin);
	        if(strncmp(buf,"end",3)==0)
	            break;

	        k=send(temp_sock_desc,buf,100,0);
	        if(k==-1)
	        {
	            printf("Error in sending");
	            exit(1);
	        }
	    }
	    close(temp_sock_desc);
	}
    
    exit(0);
    return 0;
}