#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "src/headers/initVariables.h"
int moves[218], best_move;
enum chessPiece whiteNumberOfPiecesSearch[6];
enum chessPiece blackNumberOfPiecesSearch[6];
piece chessboardSearch[8][8];
#include "src/headers/initFunctions.h"

int main(int argc, char *argv[]) {

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
    SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    initPNGs();

    gameLoop();
    
    cleanup();
    return EXIT_SUCCESS;
}

#include "src/headers/chessFunctions.h"