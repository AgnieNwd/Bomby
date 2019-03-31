//
// Created by Sviatoslav Prylutsky on 2/4/19.
//

#include "config.h"
#include "instances/headers/map.h"

void initConfigs()
{
    gameConfig.GAME_RUNNING = 0;
    gameConfig.maxClients = 4;
    gameConfig.mapSizeY = MAPX;
    gameConfig.mapSizeX = MAPY;
    playerConfig.maxBombsCnt = 1;
}
