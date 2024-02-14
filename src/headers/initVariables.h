enum chessPiece {

    EMPTY,            //0
    PAWN,             //1
    ROOK,             //2
    KNIGHT,           //3
    BISHOP,           //4
    QUEEN,            //5
    KING              //6
};

enum chessColor {

    WHITE,            //0
    BLACK,            //1
    EMPTYCOLOR        //2
};

enum gameState {

    MENU,             //0
    PLAYING,          //1
    GAME_OVER,        //2
    QUIT             //3
};

typedef struct {

    enum chessPiece type;
    enum chessColor color;
    bool hasMoved;
} piece;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const Uint32 initialTime = 600 * 1000;        //Number of seconds each player has
piece chessboard[8][8];
enum chessColor currentPlayer = WHITE;
enum gameState gameState = MENU;
enum chessPiece whiteNumberOfPieces[6] = {8, 2, 2, 2, 1, 1};
enum chessPiece blackNumberOfPieces[6] = {8, 2, 2, 2, 1, 1};
short whiteKingX, whiteKingY, blackKingX, blackKingY;
short lastMoveFromX, lastMoveFromY, lastMovePieceType;
short lastWhiteFromX, lastWhiteFromY, lastBlackFromX, lastBlackFromY, numberOfRepetitions;
short lastWhiteToX, lastWhiteToY, lastBlackToX, lastBlackToY;
bool threefoldRepetition, okToStart, stopEndScreen;
Uint32 startTimePlayer1, startTimePlayer2;
int remainingTimePlayer1, remainingTimePlayer2;

SDL_Surface *blackPieces[6], *whitePieces[6];
SDL_Texture* blackTextures[6];
SDL_Texture* whiteTextures[6];
TTF_Font *font = NULL;

SDL_Window *window = NULL;
SDL_Renderer* render = NULL;
SDL_Event event;
SDL_TimerID timerID;