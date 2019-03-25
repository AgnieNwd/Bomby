//
// Created by dy on 01/03/19.
//

#include "headers/menu.h"

void init_co()
{
    ConnectionProps *param = (ConnectionProps*)malloc(sizeof(ConnectionProps));
    param->ip = NULL;
    param->port =NULL;
}

void error(char *message)
{
    printf("Vous avez fait une erreur : %s", message);
}
