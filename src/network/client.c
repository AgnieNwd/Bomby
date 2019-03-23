//
// Created by Sviatoslav Prylutsky on 2/5/19.
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <pthread.h>
#include "../gui/headers/gui.h"
#include "../network/headers/client.h"
#include "headers/client.h"

clientNetworkParams* getClientParams()
{
    static clientNetworkParams * cParams = NULL;

    if(cParams==NULL)
    {
        cParams->socket=-1;
        cParams->times.tv_sec = 3;
        cParams->times.tv_nsec = 0;
        //Initialization by default
        cParams->serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        cParams->serverAddr.sin_port = htons(atoi("1234"));
        cParams->serverAddr.sin_family = AF_INET;
    }
    return cParams;
}

int runGame = 1;
static int mysocket = -1;
struct sockaddr_in server;
struct timespec ts = {2, 0 };

int connectToServer(){


    for(int i=0;i<3;i++)
    {
        printf("[~] try to connect to server  \n");

        if (connect(mysocket, (struct sockaddr *)&server, sizeof(server)) >= 0) {

            return 0;
        }
        printf("[~] connection failed, i try again  \n");
        nanosleep(&ts,NULL);
        close(mysocket);
        mysocket = socket(AF_INET, SOCK_STREAM, 0);
        if (mysocket < 0) {
            perror("socket()");
            return -1;
        }

    }
    printf("[~] i cant connect after try 3 times. Maybe server is down? \n");
    return 1;
}



void * sendPacketToServer()
{
   while(runGame) {


        char action[1];
       memset(action, '\n', sizeof(action));
        action[0] = getPressedKey();
        if (action[0] != 10) {

            if (action[0] == 'p')
            {
                gameDestroy();
                runGame=0;
                pthread_exit(0);
            }
            if (send(mysocket, action, sizeof(action), 0) < 0) {
                puts("[-] send failed\n");
                close(mysocket);
                break;
            }
        }
    }
    return 0;
}
game_info_t g;
void * readServerPacket()
{
    //char mapFromServer[10][10];
    while (runGame)
    {

        if (recv(mysocket, mapFromServer, sizeof(mapFromServer), MSG_WAITALL) <= 0) {
            puts("loose connection, try to reconnect...\n");
            if(connectToServer()!=0)
            {
                perror("connect()");
                puts("readServerPacket server down...\n");
                break;
            }
        }
        printGraphicMap(g);
        memset(&g, '\n', sizeof(g));
    }

    return 0;
}


int startClient(char* port,char *ip)
{



    mysocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mysocket < 0) {
        perror("socket()");
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(atoi(port));
    server.sin_family = AF_INET;

    if(connectToServer()!=0)
    {
        perror("connect()");
        return 1;
    }

    printf("[+] connected to server  \n");

    pthread_t threadReceiver;
    if (pthread_create(&threadReceiver, NULL, readServerPacket, NULL) != 0) {
        printf("main error: can't create receiver thread \n");
    }

    pthread_t threadSender;
    if (pthread_create(&threadSender, NULL, sendPacketToServer, NULL) != 0) {
        printf("main error: can't create sender thread \n");
    }

   pthread_join(threadReceiver,NULL);
   pthread_join(threadSender,NULL);

   close(mysocket);
    printf("exit \n");
    return 0;
}
