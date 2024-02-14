void createText(const char* text, SDL_Color color, int x, int y, TTF_Font* font) {

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(render, textSurface);

    SDL_Rect textRect = {x, y, textSurface -> w, textSurface -> h};
    SDL_RenderCopy(render, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void createMenu() {

    //Creating background color

    SDL_SetRenderDrawColor(render, 51, 51, 255, 255);
    SDL_RenderClear(render);

    //Chess title

    SDL_Color titleColor = {0, 0, 0, 255};
    font = TTF_OpenFont("fonts/shifty-notes-font/ShiftyNotesRegular.ttf", 50);
    createText("Chess", titleColor, SCREEN_WIDTH / 2 - 45, 100, font);

    //Creating play locally button

    SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
    SDL_Rect playButton = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 150, 200, 50};
    SDL_RenderFillRect(render, &playButton);
    SDL_Color playColor = {0, 0, 0, 255};
    font = TTF_OpenFont("fonts/shifty-notes-font/ShiftyNotesRegular.ttf", 32);
    createText("Play locally", playColor, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 140, font);

    //Creating quit button

    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    SDL_Rect quitButton = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 200, 50};
    SDL_RenderFillRect(render, &quitButton);
    SDL_Color quitColor = {0, 0, 0, 255};
    font = TTF_OpenFont("fonts/shifty-notes-font/ShiftyNotesRegular.ttf", 32);
    createText("Quit", quitColor, SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 + 10, font);

    SDL_RenderPresent(render);
}

void handleMenuInput() {

    int mouseX, mouseY;
    
    while(SDL_PollEvent(&event) != 0) {

            switch(event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&mouseX, &mouseY);

                    //Check for play locally button click

                    if(mouseX >= (SCREEN_WIDTH / 2 - 100) && mouseX <= (SCREEN_WIDTH / 2 + 100) &&
                       mouseY >= (SCREEN_HEIGHT / 2 - 150) && mouseY <= (SCREEN_HEIGHT / 2 - 100))
                        gameState = PLAYING;

                    //Check for quit button click

                    if(mouseX >= (SCREEN_WIDTH / 2 - 100) && mouseX <= (SCREEN_WIDTH / 2 + 100) &&
                       mouseY >= (SCREEN_HEIGHT / 2) && mouseY <= (SCREEN_HEIGHT / 2 + 50))
                        gameState = QUIT;
                    break;
                case SDL_QUIT:
                    gameState = QUIT;
                    break;
            }
        }
}

void createEndScreen() {

    //Removing timerID

    SDL_RemoveTimer(timerID);

    //Setting up text font and color

    font = TTF_OpenFont("fonts/shifty-notes-font/ShiftyNotesRegular.ttf", 36);
    SDL_Color endColor = {0, 0, 0, 255};

    //Check if black won

    if(whiteCheckMate() && !stopEndScreen) {
        createText("Black won!", endColor, 830, 320, font);
        stopEndScreen = true;
    }
    if(remainingTimePlayer1 <= 0 && !stopEndScreen) {
        createText("Black won!", endColor, 830, 320, font);
        stopEndScreen = true;
    }

    //Check if white won

    if(blackCheckMate() && !stopEndScreen) {
        createText("White won!", endColor, 830, 320, font);
        stopEndScreen = true;
    }
    if(remainingTimePlayer2 <= 0 && !stopEndScreen) {
        createText("White won!", endColor, 830, 320, font);
        stopEndScreen = true;
    }

    //Check for draw

    if(checkDraw() & !stopEndScreen) {
        createText("Draw!", endColor, 860, 320, font);
        stopEndScreen = true;
    }

    //Create play again button

    SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
    SDL_Rect playButton = {840, 360, 100, 40};
    SDL_RenderFillRect(render, &playButton);
    SDL_Color playColor = {0, 0, 0, 255};
    font = TTF_OpenFont("fonts/shifty-notes-font/ShiftyNotesRegular.ttf", 26);
    createText("Play again", playColor, 848, 370, font);

    //Create back to menu button

    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    SDL_Rect backButton = {840, 420, 100, 40};
    SDL_RenderFillRect(render, &backButton);
    SDL_Color backColor = {0, 0, 0, 255};
    font = TTF_OpenFont("fonts/shifty-notes-font/ShiftyNotesRegular.ttf", 22);
    createText("Back to menu", backColor, 845, 430, font);

    SDL_RenderPresent(render);
}

void handleEndInput() {

    int mouseX, mouseY;
    
    while(SDL_PollEvent(&event) != 0) {

            switch(event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&mouseX, &mouseY);

                    //Check for play again button click

                    if(mouseX >= 840 && mouseX <= 940 &&
                       mouseY >= 360 && mouseY <= 400) {
                        currentPlayer = WHITE;
                        gameState = PLAYING;
                       }

                    //Check for back to menu button click

                    if(mouseX >= 840 && mouseX <= 940 &&
                       mouseY >= 420 && mouseY <= 460) {
                        currentPlayer = WHITE;
                        gameState = MENU;
                       }
                    break;
                case SDL_QUIT:
                    gameState = QUIT;
                    break;
            }
        }
}

void renderTimer(Uint32 time, int x, int y) {

    //Resetting the space

    SDL_Rect rect;
    rect.h = rect.w = 100;
    rect.x = x;
    rect.y = y;
    SDL_SetRenderDrawColor(render, 51, 51, 255, 255);
    SDL_RenderFillRect(render, &rect);

    //Converting time to minutes and seconds

    int minutes = time / 60000;
    int seconds = (time / 1000) % 60;

    char timerString[6];
    snprintf(timerString, sizeof(timerString), "%02d:%02d", minutes, seconds);

    TTF_Font *timerFont;
    timerFont = TTF_OpenFont("fonts/Digital7.ttf", 40);
    SDL_Color timerColor = {0, 0, 0, 255};

    createText(timerString, timerColor, x, y, timerFont);
}

void createBoard() {

    currentPlayer = WHITE;

    //Making white squares as gray for visibility

    SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
    SDL_RenderClear(render);

    //Making black squares purple

    SDL_Rect rect;
    rect.w = 100;
    rect.h = 100;

    int startPos = 0, y, x;
    for(y = 0; y < 8; ++y) {
        for(x = startPos; x < 8; x+=2) {
            rect.x = x * 100;
            rect.y = y * 100;
            SDL_SetRenderDrawColor(render, 128, 0, 128, 255);
            SDL_RenderFillRect(render, &rect);
        }
        
        startPos = 1 - startPos;
    }

    //Making right side menu color

    SDL_Rect sideMenu;
    sideMenu.w = 200;
    sideMenu.h = 800;
    sideMenu.x = 800;
    sideMenu.y = 0;
    SDL_SetRenderDrawColor(render, 51, 51, 255, 255);
    SDL_RenderFillRect(render, &sideMenu);

    //Creating black pawns

    y = 1;
    for(x = 0; x < 8; ++x) {
    rect.x = x * 100;
    rect.y = y * 100;
    SDL_RenderCopy(render, blackTextures[0], NULL, &rect);
    }

    //Creating black rooks

    y = 0;
    x = 0;
    rect.x = x * 100;
    rect.y = y * 100;
    SDL_RenderCopy(render, blackTextures[1], NULL, &rect);
    x = 7;
    rect.x = x * 100;
    SDL_RenderCopy(render, blackTextures[1], NULL, &rect);

    //Creating black knights

    y = 0;
    x = 1;
    rect.x = x * 100;
    SDL_RenderCopy(render, blackTextures[2], NULL, &rect);
    x = 6;
    rect.x = x * 100;
    SDL_RenderCopy(render, blackTextures[2], NULL, &rect);

    //Creating black bishops

    x = 2;
    rect.x = x * 100;
    SDL_RenderCopy(render, blackTextures[3], NULL, &rect);
    x = 5;
    rect.x = x * 100;
    SDL_RenderCopy(render, blackTextures[3], NULL, &rect);

    //Creating black queen

    x = 4;
    rect.x = x * 100;
    SDL_RenderCopy(render, blackTextures[4], NULL, &rect);

    //Creating black king

    x = 3;
    rect.x = x * 100;
    SDL_RenderCopy(render, blackTextures[5], NULL, &rect);

    //Creating white pawns

    y = 6;
    for(x= 0; x < 8; ++x) {
        rect.x = x * 100;
        rect.y = y * 100;
        SDL_RenderCopy(render, whiteTextures[0], NULL, &rect);
    }

    //Creating white rooks

    y = 7;
    x = 0;
    rect.x = x * 100;
    rect.y = y * 100;
    SDL_RenderCopy(render, whiteTextures[1], NULL, &rect);
    x = 7;
    rect.x = x * 100;
    SDL_RenderCopy(render, whiteTextures[1], NULL, &rect);

    //Creating white knights

    x = 1;
    rect.x = x * 100;
    SDL_RenderCopy(render, whiteTextures[2], NULL, &rect);
    x = 6;
    rect.x = x * 100;
    SDL_RenderCopy(render, whiteTextures[2], NULL, &rect);

    //Creating white bishops

    x = 2;
    rect.x = x * 100;
    SDL_RenderCopy(render, whiteTextures[3], NULL, &rect);
    x = 5;
    rect.x = x * 100;
    SDL_RenderCopy(render, whiteTextures[3], NULL, &rect);

    //Creating white queen

    x = 4;
    rect.x = x * 100;
    SDL_RenderCopy(render, whiteTextures[4], NULL, &rect);

    //Creating white king

    x = 3;
    rect.x = x * 100;
    SDL_RenderCopy(render, whiteTextures[5], NULL, &rect);

    //Creating the chessboard array

    //Creating middle empty colors

    for(x = 0; x < 8; ++x)
        for(y = 2; y < 6; ++y)
            chessboard[x][y].color = EMPTYCOLOR;

    //Pawns
    for(x = 0; x < 8; ++x) {
        chessboard[x][1].type = chessboard[x][6].type = PAWN;
        chessboard[x][1].color = BLACK;
        chessboard[x][6].color = WHITE;
    }

    //Rooks
    chessboard[0][0].type = chessboard[7][0].type = ROOK;
    chessboard[0][0].color = chessboard[7][0].color = BLACK;
    chessboard[0][7].type = chessboard[7][7].type = ROOK;
    chessboard[0][7].color = chessboard[7][7].color = WHITE;

    //Knights
    chessboard[1][0].type = chessboard[6][0].type = KNIGHT;
    chessboard[1][0].color = chessboard[6][0].color = BLACK;
    chessboard[1][7].type = chessboard[6][7].type = KNIGHT;
    chessboard[1][7].color = chessboard[6][7].color = WHITE;

    //Bishops
    chessboard[2][0].type = chessboard[5][0].type = BISHOP;
    chessboard[2][0].color = chessboard[5][0].color = BLACK;
    chessboard[2][7].type = chessboard[5][7].type = BISHOP;
    chessboard[2][7].color = chessboard[5][7].color = WHITE;

    //Queens
    chessboard[4][0].type = chessboard[4][7].type = QUEEN;
    chessboard[4][0].color = BLACK;
    chessboard[4][7].color = WHITE;

    //Kings
    chessboard[3][0].type = chessboard[3][7].type = KING;
    chessboard[3][0].color = BLACK;
    blackKingX = 3;
    blackKingY = 0;
    chessboard[3][7].color = WHITE;
    whiteKingX = 3;
    whiteKingY = 7;

    //Empty
    for(x = 0; x < 8; ++x)
        for(y = 2; y < 6; ++y)
            chessboard[x][y].type = EMPTY;

    //Starting the timer

    remainingTimePlayer1 = initialTime;
    remainingTimePlayer2 = initialTime;
    startTimer();

    SDL_RenderPresent(render);
}

void handlePlayingInput() {

    while(SDL_PollEvent(&event) != 0) {

            switch(event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    handleMouseClick(event.button);
                    break;
                case SDL_QUIT:
                    gameState = QUIT;
                    break;
            }
        }
}

void initPNGs() {

    IMG_Init(IMG_INIT_PNG);

    blackPieces[0] = IMG_Load("blackPieces/pawn.png");
    blackPieces[1] = IMG_Load("blackPieces/rook.png");
    blackPieces[2] = IMG_Load("blackPieces/knight.png");
    blackPieces[3] = IMG_Load("blackPieces/bishop.png");
    blackPieces[4] = IMG_Load("blackPieces/queen.png");
    blackPieces[5] = IMG_Load("blackPieces/king.png");
    blackTextures[0] = SDL_CreateTextureFromSurface(render, blackPieces[0]);
    blackTextures[1] = SDL_CreateTextureFromSurface(render, blackPieces[1]);
    blackTextures[2] = SDL_CreateTextureFromSurface(render, blackPieces[2]);
    blackTextures[3] = SDL_CreateTextureFromSurface(render, blackPieces[3]);
    blackTextures[4] = SDL_CreateTextureFromSurface(render, blackPieces[4]);
    blackTextures[5] = SDL_CreateTextureFromSurface(render, blackPieces[5]);

    whitePieces[0] = IMG_Load("whitePieces/pawn.png");
    whitePieces[1] = IMG_Load("whitePieces/rook.png");
    whitePieces[2] = IMG_Load("whitePieces/knight.png");
    whitePieces[3] = IMG_Load("whitePieces/bishop.png");
    whitePieces[4] = IMG_Load("whitePieces/queen.png");
    whitePieces[5] = IMG_Load("whitePieces/king.png");
    whiteTextures[0] = SDL_CreateTextureFromSurface(render, whitePieces[0]);
    whiteTextures[1] = SDL_CreateTextureFromSurface(render, whitePieces[1]);
    whiteTextures[2] = SDL_CreateTextureFromSurface(render, whitePieces[2]);
    whiteTextures[3] = SDL_CreateTextureFromSurface(render, whitePieces[3]);
    whiteTextures[4] = SDL_CreateTextureFromSurface(render, whitePieces[4]);
    whiteTextures[5] = SDL_CreateTextureFromSurface(render, whitePieces[5]);
}

void cleanup() {
    for(int i = 0; i < 6; ++i) {
        SDL_FreeSurface(blackPieces[i]);
        SDL_FreeSurface(whitePieces[i]);
        SDL_DestroyTexture(blackTextures[i]);
        SDL_DestroyTexture(whiteTextures[i]);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void movePiece(int fromX, int fromY, int toX, int toY) {

    //Check if piece is king
    
    if(chessboard[fromX][fromY].type == KING && chessboard[fromX][fromY].color == WHITE) {
        whiteKingX = toX;
        whiteKingY = toY;
    }
    if(chessboard[fromX][fromY].type == KING && chessboard[fromX][fromY].color == BLACK) {
        blackKingX = toX;
        blackKingY = toY;
    }

    //Updating number of pieces

    if(chessboard[toX][toY].type != EMPTY && chessboard[toX][toY].color == WHITE)
        whiteNumberOfPieces[chessboard[toX][toY].type - 1]--;
    if(chessboard[toX][toY].type != EMPTY && chessboard[toX][toY].color == BLACK)
        blackNumberOfPieces[chessboard[toX][toY].type - 1]--;

    //Checking for repeating moves

    if(currentPlayer == WHITE) {

        if(lastWhiteFromX == toX && lastWhiteFromY == toY && 
           fromX == lastWhiteToX && fromY == lastWhiteToY)
            numberOfRepetitions++;
        else
            numberOfRepetitions = 0;
    }
    if(currentPlayer == BLACK) {

        if(lastBlackFromX == toX && lastBlackFromY == toY && 
           fromX == lastBlackToX && fromY == lastBlackToY)
            numberOfRepetitions++;
        else
            numberOfRepetitions = 0;
    }

    //Create move on chessboard array & check if pawn is promoted

    if(chessboard[fromX][fromY].type == PAWN && (toY == 0 || toY == 7)) {

        chessboard[toX][toY].type = QUEEN;
        chessboard[toX][toY].color = chessboard[fromX][fromY].color;
        lastMovePieceType = chessboard[fromX][fromY].type;
        chessboard[fromX][fromY].type = EMPTY;
        chessboard[fromX][fromY].color = EMPTYCOLOR;

        if(toY == 0) {
            
            whiteNumberOfPieces[PAWN - 1]--;
            whiteNumberOfPieces[QUEEN - 1]++;
        }
        if(toY == 7) {
            
            blackNumberOfPieces[PAWN - 1]--;
            blackNumberOfPieces[QUEEN - 1]++;
        }
    }
    else {
        chessboard[toX][toY].type = chessboard[fromX][fromY].type;
        chessboard[toX][toY].color = chessboard[fromX][fromY].color;
        lastMovePieceType = chessboard[fromX][fromY].type;
        chessboard[fromX][fromY].type = EMPTY;
        chessboard[fromX][fromY].color = EMPTYCOLOR;
    }
    
    //Create last move variables

    lastMoveFromX = fromX;
    lastMoveFromY = fromY;
    if(currentPlayer == WHITE) {
        lastWhiteFromX = fromX;
        lastWhiteFromY = fromY;
        lastWhiteToX = toX;
        lastWhiteToY = toY;
    }
    if(currentPlayer == BLACK) {
        lastBlackFromX = fromX;
        lastBlackFromY = fromY;
        lastBlackToX = toX;
        lastBlackToY = toY;
    }
    
    //Make hasMoved true for starting squares

    if(fromY == 0 || fromY == 1 || fromY == 6 || fromY == 7)
        chessboard[fromX][fromY].hasMoved = true;

    //Delete the piece's texture from it's starting square

    SDL_Rect rect;
    rect.w = rect.h = 100;
    rect.x = fromX * 100;
    rect.y = fromY * 100;
    if((fromX + fromY) % 2)
        SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
    else
        SDL_SetRenderDrawColor(render, 128, 0, 128, 255);
    SDL_RenderFillRect(render, &rect);

    //Delete the opposing piece's texture

    if(chessboard[toX][toY].type != EMPTY) {
    rect.x = toX * 100;
    rect.y = toY * 100;
    if((toX + toY) % 2)
        SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
    else
        SDL_SetRenderDrawColor(render, 128, 0, 128, 255);
    SDL_RenderFillRect(render, &rect);
    }

    //Set the image's texture on the new square

    rect.x = toX * 100;
    rect.y = toY * 100;
    if(chessboard[toX][toY].color == WHITE)
        SDL_RenderCopy(render, whiteTextures[chessboard[toX][toY].type - 1], NULL, &rect);
    else
        SDL_RenderCopy(render, blackTextures[chessboard[toX][toY].type - 1], NULL, &rect);

    if(currentPlayer == WHITE) {
        currentPlayer = BLACK;
        startTimer();
    }
    else 
    if(currentPlayer == BLACK) {
        currentPlayer = WHITE;
        startTimer();
    }

    SDL_RenderPresent(render);
}

void handleMouseClick(SDL_MouseButtonEvent buttonEvent) {

    static int sourceX = -1, sourceY = -1;
    int destX, destY;

    //Calculate clicked position
    
    int clickedSquareX = buttonEvent.x / 100;
    int clickedSquareY = buttonEvent.y / 100;

    //Repaired piece not moving on click bug

    if(currentPlayer == WHITE && chessboard[clickedSquareX][clickedSquareY].color == WHITE &&
       sourceX != -1) {
        sourceX = -1;
        sourceY = -1;
    }
    if(currentPlayer == BLACK && chessboard[clickedSquareX][clickedSquareY].color == BLACK &&
       sourceX != -1) {
        sourceX = -1;
        sourceY = -1;
    }

    if(sourceX == -1) {
        //Check if first square clicked is empty
        if(chessboard[clickedSquareX][clickedSquareY].type == EMPTY)
            return;
        //Check if first square clicked is opposite piece
        if(currentPlayer == WHITE && chessboard[clickedSquareX][clickedSquareY].color == BLACK)
            return;
        if(currentPlayer == BLACK && chessboard[clickedSquareX][clickedSquareY].color == WHITE)
            return;
        //First click
        sourceX = clickedSquareX;
        sourceY = clickedSquareY;
    }
    else {
        //Second click
        destX = clickedSquareX;
        destY = clickedSquareY;
        if(isValidMove(sourceX, sourceY, destX, destY)) {
            movePiece(sourceX, sourceY, destX, destY);
        }
        sourceX = -1;
        sourceY = -1;
        }
}

bool isValidMove(int fromX, int fromY, int toX, int toY) {
    switch(chessboard[fromX][fromY].type) {
        case EMPTY:
            return false;
            break;
        case PAWN:
            if(currentPlayer == WHITE)
                return whitePawnValidMove(fromX, fromY, toX, toY);
            else 
                return blackPawnValidMove(fromX, fromY, toX, toY);
            break;
        case ROOK:
            if(currentPlayer == WHITE)
                return whiteRookValidMove(fromX, fromY, toX, toY);
            else
                return blackRookValidMove(fromX, fromY, toX, toY);
            break;
        case KNIGHT:
            if(currentPlayer == WHITE)
                return whiteKnightValidMove(fromX, fromY, toX, toY);
            else
                return blackKnightValidMove(fromX, fromY, toX, toY);
            break;
        case BISHOP:
            if(currentPlayer == WHITE)
                return whiteBishopValidMove(fromX, fromY, toX, toY);
            else
                return blackBishopValidMove(fromX, fromY, toX, toY);
            break;
        case QUEEN:
            if(currentPlayer == WHITE)
                return whiteQueenValidMove(fromX, fromY, toX, toY);
            else
                return blackQueenValidMove(fromX, fromY, toX, toY);
            break;
        case KING:
            if(currentPlayer == WHITE)
                return whiteKingValidMove(fromX, fromY, toX, toY);
            else
                return blackKingValidMove(fromX, fromY, toX, toY);
            break;
    }
}

bool whitePawnValidMove(int fromX, int fromY, int toX, int toY) {
    
    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == BLACK)
        return false;

    //Check if piece will block check

    if(whiteInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = PAWN;
        chessboard[toX][toY].color = WHITE;
        if(whiteInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!whiteInCheck()) {
        chessboard[fromX][fromY].type = EMPTY;
        short auxType = chessboard[toX][toY].type;
        chessboard[toX][toY].type = PAWN;
        short auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].color = WHITE;
        if(whiteInCheck()) {
            chessboard[fromX][fromY].type = PAWN;
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[fromX][fromY].type = PAWN;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Checking if pawn is white and not on starting position

            if(chessboard[fromX][fromY].color == WHITE && fromY != 6) {

                //Checking front move
                
                if((toX == fromX) && (toY == (fromY - 1)) && 
                chessboard[fromX][fromY - 1].type == EMPTY)
                    return true;

                //Checking left take
                
                else 
                    if(chessboard[fromX - 1][fromY - 1].type != EMPTY && 
                       chessboard[fromX - 1][fromY - 1].color == BLACK &&
                       (toX == fromX - 1) && (toY == fromY - 1) &&
                       chessboard[toX][toY].type != KING)
                    return true;

                //Checking right take
                
                else
                    if(chessboard[fromX + 1][fromY - 1].type != EMPTY && 
                       chessboard[fromX + 1][fromY - 1].color == BLACK &&
                       (toX == fromX + 1) && (toY == fromY - 1) &&
                       chessboard[toX][toY].type != KING)
                    return true;
            }

            //Checking if pawn is white and on starting position

            if(chessboard[fromX][fromY].color == WHITE && fromY == 6) {
                    
                //Checking front move
                
                if((toX == fromX) && (toY == (fromY - 1)) && 
                chessboard[fromX][fromY - 1].type == EMPTY)
                    return true;

                //Checking front double move

                else if((toX == fromX) && (toY == (fromY - 2)) &&
                chessboard[fromX][fromY - 1].type == EMPTY &&
                chessboard[fromX][fromY - 2].type == EMPTY)
                    return true;

                //Checking left take
                
                else 
                    if(chessboard[fromX - 1][fromY - 1].type != EMPTY && 
                       chessboard[fromX - 1][fromY - 1].color == BLACK &&
                       (toX == fromX - 1) && (toY == fromY - 1) &&
                       chessboard[toX][toY].type != KING)
                    return true;

                //Checking right take
                
                else
                    if(chessboard[fromX + 1][fromY - 1].type != EMPTY && 
                       chessboard[fromX + 1][fromY - 1].color == BLACK &&
                       (toX == fromX + 1) && (toY == fromY - 1) &&
                       chessboard[toX][toY].type != KING)
                    return true;
                 
                return false;
            }
    
    //Check for en passant

    if(fromY != 3 || toY != 2)
        return false;
    if(toX != (fromX - 1) && toX != (fromX + 1))
        return false;
    if(lastMovePieceType != PAWN)
        return false;
    if(lastMoveFromY != 1)
        return false;
    if(lastMoveFromX != (fromX - 1) && lastMoveFromX != (fromX + 1))
        return false;
    
    //Check for left en passant

    if(toX == (fromX - 1)) {

        if(chessboard[toX][toY].type != EMPTY || chessboard[fromX - 1][fromY].type != PAWN)
            return false;

        chessboard[toX][toY + 1].type = EMPTY;
        chessboard[toX][toY + 1].color = EMPTYCOLOR;
        SDL_Rect rect;
        rect.w = rect.h = 100;
        rect.x = toX * 100;
        rect.y = (toY + 1) * 100;
        if((toX + toY + 1) % 2)
            SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
        else
            SDL_SetRenderDrawColor(render, 128, 0, 128, 255);
        SDL_RenderFillRect(render, &rect);

        return true;
    }

    //Check for right en passant

    if(toX == (fromX + 1)) {

        if(chessboard[toX][toY].type != EMPTY || chessboard[fromX + 1][fromY].type != PAWN)
            return false;

        chessboard[toX][toY + 1].type = EMPTY;
        chessboard[toX][toY + 1].color = EMPTYCOLOR;
        SDL_Rect rect;
        rect.w = rect.h = 100;
        rect.x = toX * 100;
        rect.y = (toY + 1) * 100;
        if((toX + toY + 1) % 2)
            SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
        else
            SDL_SetRenderDrawColor(render, 128, 0, 128, 255);
        SDL_RenderFillRect(render, &rect);

        return true;
    }

    return false;
}

bool blackPawnValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == WHITE)
        return false;

    //Check if piece will block check

    if(blackInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = PAWN;
        chessboard[toX][toY].color = BLACK;
        if(blackInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!blackInCheck()) {
        chessboard[fromX][fromY].type = EMPTY;
        short auxType = chessboard[toX][toY].type;
        chessboard[toX][toY].type = PAWN;
        short auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].color = BLACK;
        if(blackInCheck()) {
            chessboard[fromX][fromY].type = PAWN;
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[fromX][fromY].type = PAWN;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Checking if pawn is black and not on starting position

            if(chessboard[fromX][fromY].color == BLACK && fromY != 1) {

                //Checking front move
                
                if((toX == fromX) && (toY == (fromY + 1)) && 
                chessboard[fromX][fromY + 1].type == EMPTY)
                    return true;

                //Checking left take
                
                else 
                    if(chessboard[fromX - 1][fromY + 1].type != EMPTY && 
                       chessboard[fromX - 1][fromY + 1].color == WHITE &&
                       (toX == fromX - 1) && (toY == fromY + 1) &&
                       chessboard[toX][toY].type != KING)
                    return true;

                //Checking right take
                
                else
                    if(chessboard[fromX + 1][fromY + 1].type != EMPTY && 
                       chessboard[fromX + 1][fromY + 1].color == WHITE &&
                       (toX == fromX + 1) && (toY == fromY + 1) &&
                       chessboard[toX][toY].type != KING)
                    return true;
            }

            //Checking if pawn is black and on starting position

            if(chessboard[fromX][fromY].color == BLACK && fromY == 1) {

                //Checking front move
                
                if((toX == fromX) && (toY == (fromY + 1)) && 
                chessboard[fromX][fromY + 1].type == EMPTY)
                    return true;

                //Checking front double move

                else if((toX == fromX) && (toY == (fromY + 2)) &&
                chessboard[fromX][fromY + 1].type == EMPTY &&
                chessboard[fromX][fromY + 2].type == EMPTY)
                    return true;

                //Checking left take
                
                else 
                    if(chessboard[fromX - 1][fromY + 1].type != EMPTY && 
                       chessboard[fromX - 1][fromY + 1].color == WHITE &&
                       (toX == fromX - 1) && (toY == fromY + 1) &&
                       chessboard[toX][toY].type != KING)
                    return true;

                //Checking right take
                
                else
                    if(chessboard[fromX + 1][fromY + 1].type != EMPTY && 
                       chessboard[fromX + 1][fromY + 1].color == WHITE &&
                       (toX == fromX + 1) && (toY == fromY + 1) &&
                       chessboard[toX][toY].type != KING)
                    return true;
                 
                return false;                
            }

    //Check for en passant

    if(fromY != 4 || toY != 5)
        return false;
    if(toX != (fromX - 1) && toX != (fromX + 1))
        return false;
    if(lastMovePieceType != PAWN)
        return false;
    if(lastMoveFromY != 6)
        return false;
    if(lastMoveFromX != (fromX - 1) && lastMoveFromX != (fromX + 1))
        return false;
    
    //Check for left en passant

    if(toX == (fromX - 1)) {

        if(chessboard[toX][toY].type != EMPTY || chessboard[fromX - 1][fromY].type != PAWN)
            return false;

        chessboard[toX][toY - 1].type = EMPTY;
        chessboard[toX][toY - 1].color = EMPTYCOLOR;
        SDL_Rect rect;
        rect.w = rect.h = 100;
        rect.x = toX * 100;
        rect.y = (toY - 1) * 100;
        if((toX + toY - 1) % 2)
            SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
        else
            SDL_SetRenderDrawColor(render, 128, 0, 128, 255);
        SDL_RenderFillRect(render, &rect);

        return true;
    }

    //Check for right en passant

    if(toX == (fromX + 1)) {

        if(chessboard[toX][toY].type != EMPTY || chessboard[fromX + 1][fromY].type != PAWN)
            return false;

        chessboard[toX][toY - 1].type = EMPTY;
        chessboard[toX][toY - 1].color = EMPTYCOLOR;
        SDL_Rect rect;
        rect.w = rect.h = 100;
        rect.x = toX * 100;
        rect.y = (toY - 1) * 100;
        if((toX + toY - 1) % 2)
            SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
        else
            SDL_SetRenderDrawColor(render, 128, 0, 128, 255);
        SDL_RenderFillRect(render, &rect);
        
        return true;
    }

    return false;
}

bool whiteRookValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == BLACK)
        return false;

    //Check if piece will block check

    if(whiteInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = ROOK;
        chessboard[toX][toY].color = WHITE;
        if(whiteInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!whiteInCheck()) {
        chessboard[fromX][fromY].type = EMPTY;
        short auxType = chessboard[toX][toY].type;
        chessboard[toX][toY].type = ROOK;
        short auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].color = WHITE;
        if(whiteInCheck()) {
            chessboard[fromX][fromY].type = ROOK;
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        chessboard[fromX][fromY].type = ROOK;
    }

    //Checking if movement is in a straight line

    if(fromX != toX && fromY != toY)
        return false;

    //Checking if movement is up
    
    if(fromX == toX && fromY > toY) {
        
        for(int i = fromY - 1; i > toY; --i)
            if(chessboard[toX][i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    //Checking if movement is down

    if(fromX == toX && fromY < toY) {

        for(int i = fromY + 1; i < toY; ++i)
            if(chessboard[toX][i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    //Checking if movement is left

    if(fromY == toY && fromX > toX) {

        for(int i = fromX - 1; i > toX; --i)
            if(chessboard[i][toY].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    //Checking if movement is right

    if(fromY == toY && fromX < toX) {

        for(int i = fromX + 1; i < toX; ++i)
            if(chessboard[i][toY].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }
    return false;
}

bool blackRookValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == WHITE)
        return false;

    //Check if piece will block check

    if(blackInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = ROOK;
        chessboard[toX][toY].color = BLACK;
        if(blackInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!blackInCheck()) {
    chessboard[fromX][fromY].type = EMPTY;
    short auxType = chessboard[toX][toY].type;
    chessboard[toX][toY].type = ROOK;
    short auxColor = chessboard[toX][toY].color;
    chessboard[toX][toY].color = BLACK;
    if(blackInCheck()) {
        chessboard[fromX][fromY].type = ROOK;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        return false;
    }
    chessboard[toX][toY].type = auxType;
    chessboard[toX][toY].color = auxColor;
    chessboard[fromX][fromY].type = ROOK;
    }

    //Checking if movement is in a straight line

    if(fromX != toX && fromY != toY)
        return false;

    //Checking if movement is up
    
    if(fromX == toX && fromY > toY) {
        
        for(int i = fromY - 1; i > toY; --i)
            if(chessboard[toX][i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    //Checking if movement is down

    if(fromX == toX && fromY < toY) {

        for(int i = fromY + 1; i < toY; ++i)
            if(chessboard[toX][i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    //Checking if movement is left

    if(fromY == toY && fromX > toX) {

        for(int i = fromX - 1; i > toX; --i)
            if(chessboard[i][toY].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    //Checking if movement is right

    if(fromY == toY && fromX < toX) {

        for(int i = fromX + 1; i < toX; ++i)
            if(chessboard[i][toY].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }
    return false;
}

bool whiteKnightValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == BLACK)
        return false;

    //Check if piece will block check

    if(whiteInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = KNIGHT;
        chessboard[toX][toY].color = WHITE;
        if(whiteInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!whiteInCheck()) {
    chessboard[fromX][fromY].type = EMPTY;
    short auxType = chessboard[toX][toY].type;
    chessboard[toX][toY].type = KNIGHT;
    short auxColor = chessboard[toX][toY].color;
    chessboard[toX][toY].color = WHITE;
    if(whiteInCheck()) {
        chessboard[fromX][fromY].type = KNIGHT;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        return false;
    }
    chessboard[toX][toY].type = auxType;
    chessboard[toX][toY].color = auxColor;
    chessboard[fromX][fromY].type = KNIGHT;
    }

    //Checking top 2 L squares

    if(toX == (fromX - 1) && toY == (fromY - 2))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == BLACK)
                return true;
    
    if(toX == (fromX + 1) && toY == (fromY - 2))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == BLACK)
                return true;
    
    //Checking left 2 L squares

    if(toX == (fromX - 2) && toY == (fromY - 1))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == BLACK)
                return true;

    if(toX == (fromX - 2) && toY == (fromY + 1))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == BLACK)
                return true;

    //Checking right 2 L squares

    if(toX == (fromX + 2) && toY == (fromY - 1))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == BLACK)
                return true;

    if(toX == (fromX + 2) && toY == (fromY + 1))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == BLACK)
                return true;
    
    //Checking bottom 2 L squares

    if(toX == (fromX - 1) && toY == (fromY + 2))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == BLACK)
                return true;

    if(toX == (fromX + 1) && toY == (fromY + 2))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == BLACK)
                return true;

    return false;
}

bool blackKnightValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == WHITE)
        return false;

    //Check if piece will block check

    if(blackInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = KNIGHT;
        chessboard[toX][toY].color = BLACK;
        if(blackInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!blackInCheck()) {
    chessboard[fromX][fromY].type = EMPTY;
    short auxType = chessboard[toX][toY].type;
    chessboard[toX][toY].type = KNIGHT;
    short auxColor = chessboard[toX][toY].color;
    chessboard[toX][toY].color = BLACK;
    if(blackInCheck()) {
        chessboard[fromX][fromY].type = KNIGHT;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        return false;
    }
    chessboard[toX][toY].type = auxType;
    chessboard[toX][toY].color = auxColor;
    chessboard[fromX][fromY].type = KNIGHT;
    }

    //Checking top 2 L squares

    if(toX == (fromX - 1) && toY == (fromY - 2))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == WHITE)
                return true;
    
    if(toX == (fromX + 1) && toY == (fromY - 2))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == WHITE)
                return true;
    
    //Checking left 2 L squares

    if(toX == (fromX - 2) && toY == (fromY - 1))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == WHITE)
                return true;

    if(toX == (fromX - 2) && toY == (fromY + 1))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == WHITE)
                return true;

    //Checking right 2 L squares

    if(toX == (fromX + 2) && toY == (fromY - 1))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == WHITE)
                return true;

    if(toX == (fromX + 2) && toY == (fromY + 1))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == WHITE)
                return true;
    
    //Checking bottom 2 L squares

    if(toX == (fromX - 1) && toY == (fromY + 2))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == WHITE)
                return true;

    if(toX == (fromX + 1) && toY == (fromY + 2))
        if(chessboard[toX][toY].type == EMPTY)
            return true;
        else 
            if(chessboard[toX][toY].color == WHITE)
                return true;

    return false;
}

bool whiteBishopValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == BLACK)
        return false;

    //Check if piece will block check

    if(whiteInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = BISHOP;
        chessboard[toX][toY].color = WHITE;
        if(whiteInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!whiteInCheck()) {
    chessboard[fromX][fromY].type = EMPTY;
    short auxType = chessboard[toX][toY].type;
    chessboard[toX][toY].type = BISHOP;
    short auxColor = chessboard[toX][toY].color;
    chessboard[toX][toY].color = WHITE;
    if(whiteInCheck()) {
        chessboard[fromX][fromY].type = BISHOP;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        return false;
    }
    chessboard[toX][toY].type = auxType;
    chessboard[toX][toY].color = auxColor;
    chessboard[fromX][fromY].type = BISHOP;
    }

    //Checking if movement is on a diagonal

    bool isNW = false;
    bool isNE = false;
    bool isSW = false;
    bool isSE = false;
    for(int i = 1; i <=8; ++i) {
        if(toX == (fromX - i) && toY == (fromY - i))
            isNW = true;
        if(toX == (fromX + i) && toY == (fromY + i))
            isSE = true;
        if(toX == (fromX + i) && toY == (fromY - i))
            isNE = true;
        if(toX == (fromX - i) && toY == (fromY + i))
            isSW = true;
    }

    if(!isNW && !isNE && !isSW && !isSE)
        return false;

    //Checking specific diagonals

    if(isNW) {

        int numberOfSquares = fromX - toX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX - 1][fromY - 1].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    if(isSE) {

        int numberOfSquares = toX - fromX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX + 1][fromY + 1].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    if(isNE) {

        int numberOfSquares = toX - fromX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX + i][fromY - i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    if(isSW) {

        int numberOfSquares = fromX - toX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX - i][fromY + i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    return false;
}

bool blackBishopValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == WHITE)
        return false;

    //Check if piece will block check

    if(blackInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = BISHOP;
        chessboard[toX][toY].color = BLACK;
        if(blackInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!blackInCheck()) {
    chessboard[fromX][fromY].type = EMPTY;
    short auxType = chessboard[toX][toY].type;
    chessboard[toX][toY].type = BISHOP;
    short auxColor = chessboard[toX][toY].color;
    chessboard[toX][toY].color = BLACK;
    if(blackInCheck()) {
        chessboard[fromX][fromY].type = BISHOP;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        return false;
    }
    chessboard[toX][toY].type = auxType;
    chessboard[toX][toY].color = auxColor;
    chessboard[fromX][fromY].type = BISHOP;
    }

    //Checking if movement is on a diagonal

    bool isNW = false;
    bool isNE = false;
    bool isSW = false;
    bool isSE = false;
    for(int i = 1; i <=8; ++i) {
        if(toX == (fromX - i) && toY == (fromY - i))
            isNW = true;
        if(toX == (fromX + i) && toY == (fromY + i))
            isSE = true;
        if(toX == (fromX + i) && toY == (fromY - i))
            isNE = true;
        if(toX == (fromX - i) && toY == (fromY + i))
            isSW = true;
    }

    if(!isNW && !isNE && !isSW && !isSE)
        return false;

    //Checking specific diagonals

    if(isNW) {

        int numberOfSquares = fromX - toX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX - 1][fromY - 1].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    if(isSE) {

        int numberOfSquares = toX - fromX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX + 1][fromY + 1].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    if(isNE) {

        int numberOfSquares = toX - fromX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX + i][fromY - i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    if(isSW) {

        int numberOfSquares = fromX - toX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX - i][fromY + i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    return false;
}

bool whiteQueenValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == BLACK)
        return false;

    //Check if piece will block check

    if(whiteInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = QUEEN;
        chessboard[toX][toY].color = WHITE;
        if(whiteInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!whiteInCheck()) {
    chessboard[fromX][fromY].type = EMPTY;
    short auxType = chessboard[toX][toY].type;
    chessboard[toX][toY].type = QUEEN;
    short auxColor = chessboard[toX][toY].color;
    chessboard[toX][toY].color = WHITE;
    if(whiteInCheck()) {
        chessboard[fromX][fromY].type = QUEEN;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        return false;
    }
    chessboard[toX][toY].type = auxType;
    chessboard[toX][toY].color = auxColor;
    chessboard[fromX][fromY].type = QUEEN;
    }

    //Checking if movement is straight

    bool isStraight = true;
    if(fromX != toX && fromY != toY)
        isStraight = false;

    //Checking if movement is on a diagonal

    bool isNW = false;
    bool isNE = false;
    bool isSW = false;
    bool isSE = false;
    for(int i = 1; i <=8; ++i) {
        if(toX == (fromX - i) && toY == (fromY - i))
            isNW = true;
        if(toX == (fromX + i) && toY == (fromY + i))
            isSE = true;
        if(toX == (fromX + i) && toY == (fromY - i))
            isNE = true;
        if(toX == (fromX - i) && toY == (fromY + i))
            isSW = true;
    }

    if(!isNW && !isNE && !isSW && !isSE && !isStraight)
        return false;

    if(isStraight) {

    //Checking if movement is up
    
    if(fromX == toX && fromY > toY) {
        
        for(int i = fromY - 1; i > toY; --i)
            if(chessboard[toX][i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    //Checking if movement is down

    if(fromX == toX && fromY < toY) {

        for(int i = fromY + 1; i < toY; ++i)
            if(chessboard[toX][i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    //Checking if movement is left

    if(fromY == toY && fromX > toX) {

        for(int i = fromX - 1; i > toX; --i)
            if(chessboard[i][toY].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    //Checking if movement is right

    if(fromY == toY && fromX < toX) {

        for(int i = fromX + 1; i < toX; ++i)
            if(chessboard[i][toY].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    return false; }

    //Checking specific diagonals

    if(isNW) {

        int numberOfSquares = fromX - toX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX - 1][fromY - 1].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    if(isSE) {

        int numberOfSquares = toX - fromX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX + 1][fromY + 1].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    if(isNE) {

        int numberOfSquares = toX - fromX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX + i][fromY - i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    if(isSW) {

        int numberOfSquares = fromX - toX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX - i][fromY + i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == BLACK)
            return true;
    }

    return false;
}

bool blackQueenValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == WHITE)
        return false;

    //Check if piece will block check

    if(blackInCheck()) {
        short auxType = chessboard[toX][toY].type, auxColor = chessboard[toX][toY].color;
        chessboard[toX][toY].type = QUEEN;
        chessboard[toX][toY].color = BLACK;
        if(blackInCheck()) {
            chessboard[toX][toY].type = auxType;
            chessboard[toX][toY].color = auxColor;
            return false;
        }
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
    }

    //Check if piece moving will cause check

    if(!blackInCheck()) {
    chessboard[fromX][fromY].type = EMPTY;
    short auxType = chessboard[toX][toY].type;
    chessboard[toX][toY].type = QUEEN;
    short auxColor = chessboard[toX][toY].color;
    chessboard[toX][toY].color = BLACK;
    if(blackInCheck()) {
        chessboard[fromX][fromY].type = QUEEN;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        return false;
    }
    chessboard[toX][toY].type = auxType;
    chessboard[toX][toY].color = auxColor;
    chessboard[fromX][fromY].type = QUEEN;
    }

    //Checking if movement is straight

    bool isStraight = true;
    if(fromX != toX && fromY != toY)
        isStraight = false;

    //Checking if movement is on a diagonal

    bool isNW = false;
    bool isNE = false;
    bool isSW = false;
    bool isSE = false;
    for(int i = 1; i <=8; ++i) {
        if(toX == (fromX - i) && toY == (fromY - i))
            isNW = true;
        if(toX == (fromX + i) && toY == (fromY + i))
            isSE = true;
        if(toX == (fromX + i) && toY == (fromY - i))
            isNE = true;
        if(toX == (fromX - i) && toY == (fromY + i))
            isSW = true;
    }

    if(!isNW && !isNE && !isSW && !isSE && !isStraight)
        return false;

    if(isStraight) {

    //Checking if movement is up
    
    if(fromX == toX && fromY > toY) {
        
        for(int i = fromY - 1; i > toY; --i)
            if(chessboard[toX][i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    //Checking if movement is down

    if(fromX == toX && fromY < toY) {

        for(int i = fromY + 1; i < toY; ++i)
            if(chessboard[toX][i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    //Checking if movement is left

    if(fromY == toY && fromX > toX) {

        for(int i = fromX - 1; i > toX; --i)
            if(chessboard[i][toY].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    //Checking if movement is right

    if(fromY == toY && fromX < toX) {

        for(int i = fromX + 1; i < toX; ++i)
            if(chessboard[i][toY].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    return false; }

    //Checking specific diagonals

    if(isNW) {

        int numberOfSquares = fromX - toX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX - 1][fromY - 1].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    if(isSE) {

        int numberOfSquares = toX - fromX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX + 1][fromY + 1].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    if(isNE) {

        int numberOfSquares = toX - fromX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX + i][fromY - i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    if(isSW) {

        int numberOfSquares = fromX - toX;
        for(int i = 1; i < numberOfSquares; ++i)
            if(chessboard[fromX - i][fromY + i].type != EMPTY)
                return false;
        if(chessboard[toX][toY].type == EMPTY || chessboard[toX][toY].color == WHITE)
            return true;
    }

    return false;
}

bool whiteInCheck() {

    //Checking check from pawns

    if(chessboard[whiteKingX - 1][whiteKingY - 1].type == PAWN && 
        chessboard[whiteKingX - 1][whiteKingY - 1].color == BLACK)
        return true;

    if(chessboard[whiteKingX + 1][whiteKingY - 1].type == PAWN && 
        chessboard[whiteKingX + 1][whiteKingY - 1].color == BLACK)
        return true;

    //Checking check from top 2 knights

    if((whiteKingX - 1) >= 0 && (whiteKingX - 1) <= 7 && (whiteKingY - 2) >= 0 && (whiteKingY - 2) <= 7)
        if(chessboard[whiteKingX - 1][whiteKingY - 2].type == KNIGHT &&
           chessboard[whiteKingX - 1][whiteKingY - 2].color == BLACK)
           return true;

    if((whiteKingX + 1) >= 0 && (whiteKingX + 1) <= 7 && (whiteKingY - 2) >= 0 && (whiteKingY - 2) <= 7)
        if(chessboard[whiteKingX + 1][whiteKingY - 2].type == KNIGHT &&
           chessboard[whiteKingX + 1][whiteKingY - 2].color == BLACK)
           return true;

    //Checking check from left 2 knights

    if((whiteKingX - 2) >= 0 && (whiteKingX - 2) <= 7 && (whiteKingY - 1) >= 0 && (whiteKingY - 1) <= 7)
    if(chessboard[whiteKingX - 2][whiteKingY - 1].type == KNIGHT &&
       chessboard[whiteKingX - 2][whiteKingY - 1].color == BLACK)
       return true;

    if((whiteKingX - 2) >= 0 && (whiteKingX - 2) <= 7 && (whiteKingY + 1) >= 0 && (whiteKingY + 1) <= 7)
    if(chessboard[whiteKingX - 2][whiteKingY + 1].type == KNIGHT &&
       chessboard[whiteKingX - 2][whiteKingY + 1].color == BLACK)
       return true;

    //Checking check from right 2 knights

    if((whiteKingX + 2) >= 0 && (whiteKingX + 2) <= 7 && (whiteKingY - 1) >= 0 && (whiteKingY - 1) <= 7)
    if(chessboard[whiteKingX + 2][whiteKingY - 1].type == KNIGHT &&
       chessboard[whiteKingX + 2][whiteKingY - 1].color == BLACK)
       return true;

    if((whiteKingX + 2) >= 0 && (whiteKingX + 2) <= 7 && (whiteKingY + 1) >= 0 && (whiteKingY + 1) <= 7)
    if(chessboard[whiteKingX + 2][whiteKingY + 1].type == KNIGHT &&
       chessboard[whiteKingX + 2][whiteKingY + 1].color == BLACK)
       return true;

    //Checking check from bottom 2 knights

    if((whiteKingX - 1) >= 0 && (whiteKingX - 1) <= 7 && (whiteKingY + 2) >= 0 && (whiteKingY + 2) <= 7)
    if(chessboard[whiteKingX - 1][whiteKingY + 2].type == KNIGHT &&
       chessboard[whiteKingX - 1][whiteKingY + 2].color == BLACK)
       return true;

    if((whiteKingX + 1) >= 0 && (whiteKingX + 1) <= 7 && (whiteKingY + 2) >= 0 && (whiteKingY + 2) <= 7)
    if(chessboard[whiteKingX + 1][whiteKingY + 2].type == KNIGHT &&
       chessboard[whiteKingX + 1][whiteKingY + 2].color == BLACK)
       return true;

    //Checking check from straights

    short up, down, left, right;

    if((whiteKingY - 1) >= 0 && (whiteKingY - 1) <= 7) {
    up = whiteKingY - 1;
    while(chessboard[whiteKingX][up].type == EMPTY && up > 0)
        up--;
    if((chessboard[whiteKingX][up].type == ROOK || chessboard[whiteKingX][up].type == QUEEN) &&
        chessboard[whiteKingX][up].color == BLACK)
        return true;
    }

    if((whiteKingY + 1) >= 0 && (whiteKingY + 1) <= 7) {
    down = whiteKingY + 1;
    while(chessboard[whiteKingX][down].type == EMPTY && down < 7)
        down++;
    if((chessboard[whiteKingX][down].type == ROOK || chessboard[whiteKingX][down].type == QUEEN) &&
        chessboard[whiteKingX][down].color == BLACK)
        return true;
    }
    
    if((whiteKingX - 1) >= 0 && (whiteKingX - 1) <= 7) {
    left = whiteKingX - 1;
    while(chessboard[left][whiteKingY].type == EMPTY && left > 0)
        left--;
    if((chessboard[left][whiteKingY].type == ROOK || chessboard[left][whiteKingY].type == QUEEN) &&
        chessboard[left][whiteKingY].color == BLACK)
        return true;
    }

    if((whiteKingX + 1) >= 0 && (whiteKingX + 1) <= 7) {
    right = whiteKingX + 1;
    while(chessboard[right][whiteKingY].type == EMPTY && right < 7)
        right++;
    if((chessboard[right][whiteKingY].type == ROOK || chessboard[right][whiteKingY].type == QUEEN) &&
        chessboard[right][whiteKingY].color == BLACK)
        return true;
    }

    //Checking check from diagonals

    if((whiteKingX - 1) >= 0 && (whiteKingX - 1) <= 7 && (whiteKingY - 1) >= 0 && (whiteKingY - 1) <= 7) {
    int nwX = whiteKingX - 1;
    int nwY = whiteKingY - 1;
    while(chessboard[nwX][nwY].type == EMPTY && nwX > 0 && nwY > 0) {
        nwX--;
        nwY--;
    }
    if((chessboard[nwX][nwY].type == BISHOP || chessboard[nwX][nwY].type == QUEEN) &&
        chessboard[nwX][nwY].color == BLACK)
        return true;
    }

    if((whiteKingX + 1) >= 0 && (whiteKingX + 1) <= 7 && (whiteKingY + 1) >= 0 && (whiteKingY + 1) <= 7) {
    int seX = whiteKingX + 1;
    int seY = whiteKingY + 1;
    while(chessboard[seX][seY].type == EMPTY && seX < 7 && seY < 7) {
        seX++;
        seY++;
    }
    if((chessboard[seX][seY].type == BISHOP || chessboard[seX][seY].type == QUEEN) &&
        chessboard[seX][seY].color == BLACK)
        return true;
    }

    if((whiteKingX + 1) >= 0 && (whiteKingX + 1) <= 7 && (whiteKingY - 1) >= 0 && (whiteKingY - 1) <= 7) {
    int neX = whiteKingX + 1;
    int neY = whiteKingY - 1;
    while(chessboard[neX][neY].type == EMPTY && neX < 7 && neY > 0) {
        neX++;
        neY--;
    }
    if((chessboard[neX][neY].type == BISHOP || chessboard[neX][neY].type == QUEEN) &&
        chessboard[neX][neY].color == BLACK)
        return true;
    }

    if((whiteKingX - 1) >= 0 && (whiteKingX - 1) <= 7 && (whiteKingY + 1) >= 0 && (whiteKingY + 1) <= 7) {
    int swX = whiteKingX - 1;
    int swY = whiteKingY + 1;
    while(chessboard[swX][swY].type == EMPTY && swX > 0 && swY < 7) {
        swX--;
        swY++;
    }
    if((chessboard[swX][swY].type == BISHOP || chessboard[swX][swY].type == QUEEN) &&
        chessboard[swX][swY].color == BLACK)
        return true;
    }

    return false;
}

bool blackInCheck() {

    //Checking check from pawns

    if(chessboard[blackKingX - 1][blackKingY + 1].type == PAWN && 
        chessboard[blackKingX - 1][blackKingY + 1].color == WHITE)
        return true;

    if(chessboard[blackKingX + 1][blackKingY + 1].type == PAWN && 
        chessboard[blackKingX + 1][blackKingY + 1].color == WHITE)
        return true;

    //Checking check from top 2 knights

    if((blackKingX - 1) >= 0 && (blackKingX - 1) <= 7 && (blackKingY - 2) >= 0 && (blackKingY - 2) <= 7)
        if(chessboard[blackKingX - 1][blackKingY - 2].type == KNIGHT &&
           chessboard[blackKingX - 1][blackKingY - 2].color == WHITE)
           return true;

    if((blackKingX + 1) >= 0 && (blackKingX + 1) <= 7 && (blackKingY - 2) >= 0 && (blackKingY - 2) <= 7)
        if(chessboard[blackKingX + 1][blackKingY - 2].type == KNIGHT &&
           chessboard[blackKingX + 1][blackKingY - 2].color == WHITE)
           return true;

    //Checking check from left 2 knights

    if((blackKingX - 2) >= 0 && (blackKingX - 2) <= 7 && (blackKingY - 1) >= 0 && (blackKingY - 1) <= 7)
    if(chessboard[blackKingX - 2][blackKingY - 1].type == KNIGHT &&
       chessboard[blackKingX - 2][blackKingY - 1].color == WHITE)
       return true;

    if((blackKingX - 2) >= 0 && (blackKingX - 2) <= 7 && (blackKingY + 1) >= 0 && (blackKingY + 1) <= 7)
    if(chessboard[blackKingX - 2][blackKingY + 1].type == KNIGHT &&
       chessboard[blackKingX - 2][blackKingY + 1].color == WHITE)
       return true;

    //Checking check from right 2 knights

    if((blackKingX + 2) >= 0 && (blackKingX + 2) <= 7 && (blackKingY - 1) >= 0 && (blackKingY - 1) <= 7)
    if(chessboard[blackKingX + 2][blackKingY - 1].type == KNIGHT &&
       chessboard[blackKingX + 2][blackKingY - 1].color == WHITE)
       return true;

    if((blackKingX + 2) >= 0 && (blackKingX + 2) <= 7 && (blackKingY + 1) >= 0 && (blackKingY + 1) <= 7)
    if(chessboard[blackKingX + 2][blackKingY + 1].type == KNIGHT &&
       chessboard[blackKingX + 2][blackKingY + 1].color == WHITE)
       return true;

    //Checking check from bottom 2 knights

    if((blackKingX - 1) >= 0 && (blackKingX - 1) <= 7 && (blackKingY + 2) >= 0 && (blackKingY + 2) <= 7)
    if(chessboard[blackKingX - 1][blackKingY + 2].type == KNIGHT &&
       chessboard[blackKingX - 1][blackKingY + 2].color == WHITE)
       return true;

    if((blackKingX + 1) >= 0 && (blackKingX + 1) <= 7 && (blackKingY + 2) >= 0 && (blackKingY + 2) <= 7)
    if(chessboard[blackKingX + 1][blackKingY + 2].type == KNIGHT &&
       chessboard[blackKingX + 1][blackKingY + 2].color == WHITE)
       return true;

    //Checking check from straights

    if((blackKingY - 1) >= 0 && (blackKingY - 1) <= 7) {
    short up = blackKingY - 1;
    while(chessboard[blackKingX][up].type == EMPTY && up > 0)
        up--;
    if((chessboard[blackKingX][up].type == ROOK || chessboard[blackKingX][up].type == QUEEN) &&
        chessboard[blackKingX][up].color == WHITE)
        return true;
    }

    if((blackKingY + 1) >= 0 && (blackKingY + 1) <= 7) {
    short down = blackKingY + 1;
    while(chessboard[blackKingX][down].type == EMPTY && down < 7)
        down++;
    if((chessboard[blackKingX][down].type == ROOK || chessboard[blackKingX][down].type == QUEEN) &&
        chessboard[blackKingX][down].color == WHITE)
        return true;
    }
    
    if((blackKingX - 1) >= 0 && (blackKingX - 1) <= 7) {
    short left = blackKingX - 1;
    while(chessboard[left][blackKingY].type == EMPTY && left > 0)
        left--;
    if((chessboard[left][blackKingY].type == ROOK || chessboard[left][blackKingY].type == QUEEN) &&
        chessboard[left][blackKingY].color == WHITE)
        return true;
    }

    if((blackKingX + 1) >= 0 && (blackKingX + 1) <= 7) {
    short right = blackKingX + 1;
    while(chessboard[right][blackKingY].type == EMPTY && right < 7)
        right++;
    if((chessboard[right][blackKingY].type == ROOK || chessboard[right][blackKingY].type == QUEEN) &&
        chessboard[right][blackKingY].color == WHITE)
        return true;
    }

    //Checking check from diagonals

    if((blackKingX - 1) >= 0 && (blackKingX - 1) <= 7 && (blackKingY - 1) >= 0 && (blackKingY - 1) <= 7) {
    int nwX = blackKingX - 1;
    int nwY = blackKingY - 1;
    while(chessboard[nwX][nwY].type == EMPTY && nwX > 0 && nwY > 0) {
        nwX--;
        nwY--;
    }
    if((chessboard[nwX][nwY].type == BISHOP || chessboard[nwX][nwY].type == QUEEN) &&
        chessboard[nwX][nwY].color == WHITE)
        return true;
    }

    if((blackKingX + 1) >= 0 && (blackKingX + 1) <= 7 && (blackKingY + 1) >= 0 && (blackKingY + 1) <= 7) {
    int seX = blackKingX + 1;
    int seY = blackKingY + 1;
    while(chessboard[seX][seY].type == EMPTY && seX < 7 && seY < 7) {
        seX++;
        seY++;
    }
    if((chessboard[seX][seY].type == BISHOP || chessboard[seX][seY].type == QUEEN) &&
        chessboard[seX][seY].color == WHITE)
        return true;
    }

    if((blackKingX + 1) >= 0 && (blackKingX + 1) <= 7 && (blackKingY - 1) >= 0 && (blackKingY - 1) <= 7) {
    int neX = blackKingX + 1;
    int neY = blackKingY - 1;
    while(chessboard[neX][neY].type == EMPTY && neX < 7 && neY > 0) {
        neX++;
        neY--;
    }
    if((chessboard[neX][neY].type == BISHOP || chessboard[neX][neY].type == QUEEN) &&
        chessboard[neX][neY].color == WHITE)
        return true;
    }

    if((blackKingX - 1) >= 0 && (blackKingX - 1) <= 7 && (blackKingY + 1) >= 0 && (blackKingY + 1) <= 7) {
    int swX = blackKingX - 1;
    int swY = blackKingY + 1;
    while(chessboard[swX][swY].type == EMPTY && swX > 0 && swY < 7) {
        swX--;
        swY++;
    }
    if((chessboard[swX][swY].type == BISHOP || chessboard[swX][swY].type == QUEEN) &&
        chessboard[swX][swY].color == WHITE)
        return true;
    }

    return false;
}

bool whiteKingValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == BLACK)
        return false;

    //Check if move will result in check

    short auxX = whiteKingX, auxY = whiteKingY;
    whiteKingX = toX;
    whiteKingY = toY;
    chessboard[fromX][fromY].type = EMPTY;
    chessboard[fromX][fromY].color = EMPTYCOLOR;
    short auxType = chessboard[toX][toY].type;
    short auxColor = chessboard[toX][toY].color;
    chessboard[toX][toY].type = KING;
    chessboard[toX][toY].color = WHITE;
    if(whiteInCheck()) {
        whiteKingX = auxX;
        whiteKingY = auxY;
        chessboard[fromX][fromY].type = KING;
        chessboard[fromX][fromY].color = WHITE;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        return false;
    }
    chessboard[fromX][fromY].type = KING;
    chessboard[fromX][fromY].color = WHITE;
    chessboard[toX][toY].type = auxType;
    chessboard[toX][toY].color = auxColor;
    whiteKingX = auxX;
    whiteKingY = auxY;

    //Checking up, down, left, right

    if((fromX == toX && (fromY - 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == BLACK))
       return true;

    if((fromX == toX && (fromY + 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == BLACK))
       return true;

    if(((fromX - 1) == toX && fromY == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == BLACK))
       return true;
    
    if(((fromX + 1) == toX && fromY == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == BLACK))
       return true;

    //Checking diagonals

    if(((fromX - 1) == toX && (fromY - 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == BLACK))
       return true;

    if(((fromX + 1) == toX && (fromY - 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == BLACK))
       return true;

    if(((fromX + 1) == toX && (fromY + 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == BLACK))
       return true;

    if(((fromX - 1) == toX && (fromY + 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == BLACK))
       return true;

    //Checking for small castle
    
    if(toX == 1 && toY == 7) {

        if(chessboard[3][7].hasMoved == true || chessboard[0][7].hasMoved == true)
            return false;
        if(chessboard[1][7].type != EMPTY || chessboard[2][7].type != EMPTY)
            return false;
        if(whiteInCheck())
            return false;
        whiteKingX = 2;
        if(whiteInCheck()) {
            whiteKingX = 3;
            return false;
        }
        whiteKingX = 1;
        if(whiteInCheck()) {
            whiteKingX = 3;
            return false;
        }

        //Moving rook

        movePiece(0, 7, 2, 7);
        return true;
    }

    //Checking for long castle

    if(toX == 5 && toY == 7) {

        if(chessboard[3][7].hasMoved == true || chessboard[7][7].hasMoved == true)
            return false;
        if(chessboard[4][7].type != EMPTY || chessboard[5][7].type != EMPTY || chessboard[6][7].type != EMPTY)
            return false;
        if(whiteInCheck())
            return false;
        whiteKingX = 4;
        if(whiteInCheck()) {
            whiteKingX = 3;
            return false;
        }
        whiteKingX = 5;
        if(whiteInCheck()) {
            whiteKingX = 3;
            return false;
        }

        //Moving rook

        movePiece(7, 7, 4, 7);
        return true;
    }
    
    return false;
}

bool blackKingValidMove(int fromX, int fromY, int toX, int toY) {

    //Checking if movement is valid

    if(fromX == toX && fromY == toY)
        return false;
    if(chessboard[toX][toY].type == KING)
        return false;
    if(chessboard[fromX][fromY].color == WHITE)
        return false;

    //Check if move will result in check

    short auxX = blackKingX, auxY = blackKingY;
    blackKingX = toX;
    blackKingY = toY;
    chessboard[fromX][fromY].type = EMPTY;
    chessboard[fromX][fromY].color = EMPTYCOLOR;
    short auxType = chessboard[toX][toY].type;
    short auxColor = chessboard[toX][toY].color;
    chessboard[toX][toY].type = KING;
    chessboard[toX][toY].color = BLACK;
    if(blackInCheck()) {
        blackKingX = auxX;
        blackKingY = auxY;
        chessboard[fromX][fromY].type = KING;
        chessboard[fromX][fromY].color = BLACK;
        chessboard[toX][toY].type = auxType;
        chessboard[toX][toY].color = auxColor;
        return false;
    }
    chessboard[fromX][fromY].type = KING;
    chessboard[fromX][fromY].color = BLACK;
    chessboard[toX][toY].type = auxType;
    chessboard[toX][toY].color = auxColor;
    blackKingX = auxX;
    blackKingY = auxY;

    //Checking up, down, left, right

    if((fromX == toX && (fromY - 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == WHITE))
       return true;

    if((fromX == toX && (fromY + 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == WHITE))
       return true;

    if(((fromX - 1) == toX && fromY == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == WHITE))
       return true;
    
    if(((fromX + 1) == toX && fromY == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == WHITE))
       return true;

    //Checking diagonals

    if(((fromX - 1) == toX && (fromY - 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == WHITE))
       return true;

    if(((fromX + 1) == toX && (fromY - 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == WHITE))
       return true;

    if(((fromX + 1) == toX && (fromY + 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == WHITE))
       return true;

    if(((fromX - 1) == toX && (fromY + 1) == toY) && (chessboard[toX][toY].type == EMPTY || 
       chessboard[toX][toY].color == WHITE))
       return true;

    //Checking for small castle
    
    if(toX == 1 && toY == 0) {

        if(chessboard[3][0].hasMoved == true || chessboard[0][0].hasMoved == true)
            return false;
        if(chessboard[1][0].type != EMPTY || chessboard[2][0].type != EMPTY)
            return false;
        if(blackInCheck())
            return false;
        blackKingX = 2;
        if(blackInCheck()) {
            blackKingX = 3;
            return false;
        }
        blackKingX = 1;
        if(blackInCheck()) {
            blackKingX = 3;
            return false;
        }

        //Moving rook

        movePiece(0, 0, 2, 0);
        return true;
    }

    //Checking for long castle

    if(toX == 5 && toY == 0) {

        if(chessboard[3][0].hasMoved == true || chessboard[7][0].hasMoved == true)
            return false;
        if(chessboard[4][0].type != EMPTY || chessboard[5][0].type != EMPTY || chessboard[6][0].type != EMPTY)
            return false;
        if(blackInCheck())
            return false;
        blackKingX = 4;
        if(blackInCheck()) {
            blackKingX = 3;
            return false;
        }
        blackKingX = 5;
        if(blackInCheck()) {
            blackKingX = 3;
            return false;
        }

        //Moving rook

        movePiece(7, 0, 4, 0);
        return true;
    }

    return false;
}

bool doesWhiteHaveMoves() {

    //Checking board for white pieces

    for(int y = 0; y < 8; ++y)
        for(int x = 0; x < 8; ++x) {

            if(chessboard[x][y].color == WHITE && chessboard[x][y].type != EMPTY) {

                //Checking board until we find 1 valid move

                for(int toY = 0; toY < 8; ++toY)
                    for(int toX = 0; toX < 8; ++toX)
                        if(isValidMove(x, y, toX, toY))
                            return true;
            }
    }

    return false;
}

bool whiteCheckMate() {

    if(whiteInCheck() && !doesWhiteHaveMoves())
        return true;
    return false;
}

bool doesBlackHaveMoves() {

    //Checking board for black pieces

    for(int y = 0; y < 8; ++y)
        for(int x = 0; x < 8; ++x) {

            if(chessboard[x][y].color == BLACK && chessboard[x][y].type != EMPTY) {

                //Checking board until we find 1 valid move

                for(int toY = 0; toY < 8; ++toY)
                    for(int toX = 0; toX < 8; ++toX)
                        if(isValidMove(x, y, toX, toY))
                            return true;
            }
    }

    return false;
}

bool blackCheckMate() {

    if(blackInCheck() && !doesBlackHaveMoves())
        return true;
    return false;
}

bool checkDraw() {

    //Check for stalemate

    if(!whiteInCheck() && !doesWhiteHaveMoves() && currentPlayer == WHITE)
        return true;
    if(!blackInCheck() && !doesBlackHaveMoves() && currentPlayer == BLACK)
        return true;

    //Check for insufficient material

    //Check for king and knight vs king or king vs king

    if(whiteNumberOfPieces[PAWN - 1] == 0 && whiteNumberOfPieces[ROOK - 1] == 0 && 
       whiteNumberOfPieces[KNIGHT - 1] <= 1 && whiteNumberOfPieces[BISHOP - 1] == 0 &&
       whiteNumberOfPieces[QUEEN - 1] == 0 && blackNumberOfPieces[PAWN - 1] == 0 &&
       blackNumberOfPieces[ROOK - 1] == 0 && blackNumberOfPieces[KNIGHT - 1] == 0 &&
       blackNumberOfPieces[BISHOP - 1] == 0 && blackNumberOfPieces[QUEEN - 1] == 0)
            return true;
    if(whiteNumberOfPieces[PAWN - 1] == 0 && whiteNumberOfPieces[ROOK - 1] == 0 && 
       whiteNumberOfPieces[KNIGHT - 1] == 0 && whiteNumberOfPieces[BISHOP - 1] == 0 &&
       whiteNumberOfPieces[QUEEN - 1] == 0 && blackNumberOfPieces[PAWN - 1] == 0 &&
       blackNumberOfPieces[ROOK - 1] == 0 && blackNumberOfPieces[KNIGHT - 1] <= 1 &&
       blackNumberOfPieces[BISHOP - 1] == 0 && blackNumberOfPieces[QUEEN - 1] == 0)
            return true;

    //Check for king and bishop vs king

    if(whiteNumberOfPieces[PAWN - 1] == 0 && whiteNumberOfPieces[ROOK - 1] == 0 && 
       whiteNumberOfPieces[KNIGHT - 1] == 0 && whiteNumberOfPieces[BISHOP - 1] == 1 &&
       whiteNumberOfPieces[QUEEN - 1] == 0 && blackNumberOfPieces[PAWN - 1] == 0 &&
       blackNumberOfPieces[ROOK - 1] == 0 && blackNumberOfPieces[KNIGHT - 1] == 0 &&
       blackNumberOfPieces[BISHOP - 1] == 0 && blackNumberOfPieces[QUEEN - 1] == 0)
            return true;
    if(whiteNumberOfPieces[PAWN - 1] == 0 && whiteNumberOfPieces[ROOK - 1] == 0 && 
       whiteNumberOfPieces[KNIGHT - 1] == 0 && whiteNumberOfPieces[BISHOP - 1] == 0 &&
       whiteNumberOfPieces[QUEEN - 1] == 0 && blackNumberOfPieces[PAWN - 1] == 0 &&
       blackNumberOfPieces[ROOK - 1] == 0 && blackNumberOfPieces[KNIGHT - 1] == 0 &&
       blackNumberOfPieces[BISHOP - 1] == 1 && blackNumberOfPieces[QUEEN - 1] == 0)
            return true;

    //Check for king and bishop vs king and bishop of same color

    if(whiteNumberOfPieces[PAWN - 1] == 0 && whiteNumberOfPieces[ROOK - 1] == 0 && 
       whiteNumberOfPieces[KNIGHT - 1] == 0 && whiteNumberOfPieces[BISHOP - 1] == 1 &&
       whiteNumberOfPieces[QUEEN - 1] == 0 && blackNumberOfPieces[PAWN - 1] == 0 &&
       blackNumberOfPieces[ROOK - 1] == 0 && blackNumberOfPieces[KNIGHT - 1] == 0 &&
       blackNumberOfPieces[BISHOP - 1] == 1 && blackNumberOfPieces[QUEEN - 1] == 0) {

            short whiteX = -1, whiteY = -1, blackX = -1, blackY = -1;
            for(int y = 0; y < 8; ++y)
                for(int x = 0; x < 8; ++x) {

                    if(chessboard[x][y].type == BISHOP && chessboard[x][y].color == WHITE) {
                        whiteX = x;
                        whiteY = y;
                    }
                    if(chessboard[x][y].type == BISHOP && chessboard[x][y].color == BLACK) {
                        blackX = x;
                        blackY = y;
                    }
                    if(whiteX != -1 && blackX != -1)
                        break;
                }
            if((whiteX + whiteY) % 2 == (blackX + blackY) % 2)
                return true;
       }

    //Check for threefold repetition
            
    if(numberOfRepetitions == 8)
        return true;
    
    return false;
}

Uint32 timerCallback(Uint32 interval, void* param) {

    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime;
    if(currentPlayer == WHITE) {
        elapsedTime = currentTime - startTimePlayer1;
        remainingTimePlayer1 -= elapsedTime;
        startTimePlayer1 = currentTime;
    }
    else {
        elapsedTime = currentTime - startTimePlayer2;
        remainingTimePlayer2 -= elapsedTime;
        startTimePlayer2 = currentTime;
    }

    return interval;
}

void startTimer() {
    
    renderTimer(initialTime, 810, 0);
    renderTimer(initialTime, 810, 600);

    if(currentPlayer == WHITE)
        startTimePlayer1 = SDL_GetTicks();
    else
        startTimePlayer2 = SDL_GetTicks();
    
    timerID = SDL_AddTimer(1000, timerCallback, NULL);
}

void gameLoop() {

    while(gameState != QUIT) {
        
        switch(gameState) {

            case MENU:
                createMenu();
                handleMenuInput();
                break;
            case PLAYING:
                createBoard();
                while(gameState == PLAYING) {
                    handlePlayingInput();
                    renderTimer(remainingTimePlayer2, 810, 0);
                    renderTimer(remainingTimePlayer1, 810, 600);
                    SDL_RenderPresent(render);
                    SDL_Delay(1000 / 30);
                    if(whiteCheckMate() || blackCheckMate() || checkDraw() ||
                       remainingTimePlayer1 <= 0 || remainingTimePlayer2 <= 0) {
                        gameState = GAME_OVER;
                        break;
                    }
                }
                break;
            case GAME_OVER:
                while(gameState == GAME_OVER) {
                    stopEndScreen = false;
                    createEndScreen();
                    handleEndInput();
                }
                break;
        }
    }
}

bool gameIsOver() {

    if(whiteCheckMate() || blackCheckMate() || checkDraw() ||
       remainingTimePlayer1 <= 0 || remainingTimePlayer2 <= 0)
       return true;
    return false;
}