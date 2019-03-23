//
// Created by Sviatoslav Prylutsky on 2/26/19.
//

#ifndef BOMBERMAN_CLIENT_H
#define BOMBERMAN_CLIENT_H

#include "../../instances/headers/object.h"
#include "../../network/headers/server.h"
#include <time.h>

int startClient(char* port,char *ip);
typedef struct  {
    int socket;
    struct timespec times;
    struct sockaddr_in serverAddr;

} clientNetworkParams;
#endif //BOMBERMAN_CLIENT_H
