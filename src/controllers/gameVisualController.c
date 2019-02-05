/*
 *This file* control the aparance of instances in the the game via SDL
 *
 */

#include "../headers/bomberman.h"

void showMapContent()
{

}


Player* game_init()
{
    Player * game = NULL;
    game = malloc(sizeof(Player));
    game->screenSize.x = 640;
    game->screenSize.y = 480;
    game->pWindow = NULL;
    game->pRenderer = NULL;
    // game->player1Position.x = 200;
    // game->player1Position.y = 150;
    game->player1Position.w = 40;
    game->player1Position.h = 40;
    game->ifBombe = 0;
    game->Player = NULL;


    //Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Impossible d'initialiser la SDL : %s\n", SDL_GetError());
        game_destroy(game);
        return (NULL);
    }
    //Creation fenêtre
    game->pWindow = SDL_CreateWindow("Bomberman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game->screenSize.x, game->screenSize.y, SDL_WINDOW_SHOWN);
    if (game->pWindow) {
        //Crea renderer
        game->pRenderer = SDL_CreateRenderer(game->pWindow, -1, SDL_RENDERER_ACCELERATED);
        if (!game->pRenderer) {
            fprintf(stderr, "Impossible de créer le renderer SDL : %s\n", SDL_GetError());
            game_destroy(game);
            return (NULL);
        }
    } else {
        fprintf(stderr, "Impossible de créer le fenetre SDL : %s\n", SDL_GetError());
        game_destroy(game);
        return (NULL);
    }



    SDL_Surface* surfaceBomby = IMG_Load("./images/bombe.png");
    if (!surfaceBomby) {
        fprintf(stderr, "Impossible de charger l'image bombe.png : %s\n", IMG_GetError());
        game_destroy(game);
        return (NULL);
    } else { 
        game->pTexturePlayer = SDL_CreateTextureFromSurface(game->pRenderer, surfaceBomby);
        if (!game->pTexturePlayer) {
            fprintf(stderr, "Impossible de charger  la texture : %s\n", SDL_GetError());
            game_destroy(game);
            return (NULL);
        }
        SDL_FreeSurface(surfaceBomby);
    }



    //Chargement texture map + perso 
    game->img_texture[1] = IMG_Load("./images/tileset_bomberman.bmp");
    game->img_texture[2] = IMG_Load("./images/perso.bmp");
    if (!game->img_texture[1]) {
        fprintf(stderr, "Impossible de charger l'image 10 : %s\n", IMG_GetError());
        game_destroy(game);
        return (NULL);
    } else if (!game->img_texture[2]) {
        fprintf(stderr, "Impossible de charger l'image 11 : %s\n", IMG_GetError());
        game_destroy(game);
        return (NULL);
    }
    
    return (game);
}

void game_destroy(Player *game)
{

    int x;
    int y;
    if (game) {
        if (game->pTexturePlayer) {
            SDL_DestroyTexture(game->pTexturePlayer);
        }

        for (y = 0; y < NOMBRE_BLOCS_LARGEUR; y++) {
            for (x = 0; x < NOMBRE_BLOCS_HAUTEUR; x++) {
                if (game->textures[y][x]) {
                    SDL_DestroyTexture(game->textures[y][x]);
                }
            }
        }        
        if (game->pRenderer) {
            SDL_DestroyRenderer(game->pRenderer);
        }        
        if (game->pWindow) {
            SDL_DestroyWindow(game->pWindow);
        }
        SDL_Quit();
        free(game);
    }
}

void game_show(Player* game, char* direction)
{
    int y;
    int x;
    SDL_Rect Rect_dest;
    SDL_Rect Src_dest;
    //clean l'ecran
    SDL_SetRenderDrawColor(game->pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(game->pRenderer);

    //Afficher la map + joueur 
    for(y=0; y < NOMBRE_BLOCS_LARGEUR; y++) {
        for(x=0; x < NOMBRE_BLOCS_HAUTEUR; x++) {
            Src_dest = get_src_dest(game->map[y][x], direction); // cas qui gère l'image à afficher
            Rect_dest.x = x * 50;
            Rect_dest.y = y * 50;
            Rect_dest.w = 50;
            Rect_dest.h = 50;
            SDL_RenderCopy(game->pRenderer, game->textures[y][x], &Src_dest, &Rect_dest);
        }
    }

    //afficher la bombe
    if(game->ifBombe == 1) {
        //printf( "%d\n", game->bombe->position.x);
        SDL_RenderCopy(game->pRenderer, game->pTexturePlayer, NULL, &game->Player->position);
    }

    //Afficher rendu
    SDL_RenderPresent(game->pRenderer);
}

int game_event(Player* game)
{
    if (game)
        fprintf(stderr, " ");
    int result = 0;
    SDL_Event e;
    game_show(game, "null");
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            //quitte le jeu
            result = -1;
        } else if (e.type == SDL_KEYDOWN) {
            //gerer touche du clavier
            switch (e.key.keysym.sym) {
            case SDLK_ESCAPE :
                result = -1;
                break;               
            case SDLK_UP:
                game_movePlayer(game, e.key.keysym.sym);
                game_show(game, "up");
                break;
            case SDLK_DOWN:
                game_movePlayer(game, e.key.keysym.sym);
                game_show(game, "down");
                break;
            case SDLK_LEFT:
                game_movePlayer(game, e.key.keysym.sym);
                game_show(game, "left");
                break;
            case SDLK_RIGHT:
                game_movePlayer(game, e.key.keysym.sym);
                game_show(game, "right");
                break;
            case SDLK_d:
                game_dropBombe(game);
                game_show(game, "null");
                break;
            default:
                fprintf(stderr, "touche inconnue %d\n", e.key.keysym.sym);
                break;
            }
        }
    }
    return (result);
}