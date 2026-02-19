#include "gui.h"

int side = WHITE;
int selectedSq = -1;
int legalMap[64] = {0};

typedef struct {
    Texture2D tex[12];
} PieceTextures;

PieceTextures loadPieceTextures(void) {
    PieceTextures pt;

    pt.tex[PAWN_W] = LoadTexture("sprites/whitePawn.png");
    pt.tex[KNIGHT_W] = LoadTexture("sprites/whiteKnight.png");
    pt.tex[BISHOP_W] = LoadTexture("sprites/whiteBishop.png");
    pt.tex[ROOK_W] = LoadTexture("sprites/whiteRook.png");
    pt.tex[QUEEN_W] = LoadTexture("sprites/whiteQueen.png");
    pt.tex[KING_W] = LoadTexture("sprites/whiteKing.png");

    pt.tex[PAWN_B] = LoadTexture("sprites/blackPawn.png");
    pt.tex[KNIGHT_B] = LoadTexture("sprites/blackKnight.png");
    pt.tex[BISHOP_B] = LoadTexture("sprites/blackBishop.png");
    pt.tex[ROOK_B] = LoadTexture("sprites/blackRook.png");
    pt.tex[QUEEN_B] = LoadTexture("sprites/blackQueen.png");
    pt.tex[KING_B] = LoadTexture("sprites/blackKing.png");

    return pt;
}

int pieceOnSquare(Board b, int sq) {
    U64 mask = 1ULL << sq;
    if (b.pawnBB   & mask) return (b.whiteBB & mask) ? PAWN_W : PAWN_B;
    if (b.knightBB & mask) return (b.whiteBB & mask) ? KNIGHT_W : KNIGHT_B;
    if (b.bishopBB & mask) return (b.whiteBB & mask) ? BISHOP_W : BISHOP_B;
    if (b.rookBB   & mask) return (b.whiteBB & mask) ? ROOK_W : ROOK_B;
    if (b.queenBB  & mask) return (b.whiteBB & mask) ? QUEEN_W : QUEEN_B;
    if (b.kingBB   & mask) return (b.whiteBB & mask) ? KING_W : KING_B;
    return -1;
}

int mouseToSquare(int tile) {
    Vector2 m = GetMousePosition();

    int file = m.x / tile;
    int rank = m.y / tile;

    if (file < 0 || file > 7 || rank < 0 || rank > 7)
        return -1;

    int f = file;
    int r = 7 - rank;

    return f + r * 8;
}

void clearLegal() {
    for (int i = 0; i < 64; i++) legalMap[i] = false;
}

void buildLegalMap(Move *moves, int moveCount, int fromSq) {
    clearLegal();

    for (int i = 0; i < moveCount; i++) {
        int from = EXTRACT_FROM(moves[i]);
        int to = EXTRACT_TO(moves[i]);
        if (from == fromSq) legalMap[to] = true;
    }
}

void drawBoard() {
    int tile = WIDTH / 8;

    for (int file = 0; file < 8; file++) {
        for (int rank = 0; rank < 8; rank++) {

            int isLight = (file + rank) % 2 == 0;

            Color c = isLight ? rgb(235,209,166,255) : rgb(165,117,80,255);

            DrawRectangle(file * tile, rank * tile, tile, tile, c);
        }
    }
}

void drawHighlights(int tile) {
    Vector2 size = {tile, tile};
    Vector2 pos;

    if (selectedSq != -1) {
        int f = selectedSq % 8;
        int r = selectedSq / 8;
        pos.x = f * tile;
        pos.y = (7 - r) * tile;

        DrawRectangleV(pos, size, rgb(0,255,0,90));
    }

    // Legal moves
    for (int sq = 0; sq < 64; sq++) {
        if (!legalMap[sq]) continue;

        int f = sq % 8;
        int r = sq / 8;
        pos.x = (sq % 8) * tile;
        pos.y = (7 - r) * tile;

        DrawRectangleV(pos, size, rgb(255,0,0,90));
    }
}

void drawPieces(Board b, PieceTextures *pt, int tile) {
    Vector2 v2 = {0};
    for (int sq = 0; sq < 64; sq++) {

        int p = pieceOnSquare(b, sq);
        if (p == -1) continue;

        int file = sq % 8;
        int rank = sq / 8;

        v2.x = file * tile;
        v2.y = (7 - rank) * tile;

        Texture2D tex = pt->tex[p];

        double scale = (double) tile / tex.width;

        DrawTextureEx(tex, v2, 0.0f, scale, rgb(255,255,255,255));
    }
}

int main(void) {
    srand(time(NULL));
    initMoveGen();

    Board board;
    Move moves[MAX_MOVES];

    // setFen(&board, START_FEN);
    setFen(&board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(WIDTH, HEIGHT, "");
    SetTargetFPS(60);

    int tile = WIDTH / 8;

    PieceTextures pieces = loadPieceTextures();

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            int sq = mouseToSquare(tile);
            if (sq == -1) continue;

            char p = pieceOnSquare(board, sq);

            if (sq == selectedSq) {
                selectedSq = -1;
                clearLegal();
            }

            else if (selectedSq != -1 && legalMap[sq]) {

                int moveCount = legalMoves(&board, moves);

                for (int i = 0; i < moveCount; i++) {
                    if (EXTRACT_FROM(moves[i]) == selectedSq && EXTRACT_TO(moves[i]) == sq) {

                        makeMove(&board, moves[i]);
                        break;
                    }
                }

                selectedSq = -1;
                clearLegal();
            }

            else if (p != -1) {
                if (side != WHITE) continue;

                selectedSq = sq;

                int moveCount = legalMoves(&board, moves);
                buildLegalMap(moves, moveCount, selectedSq);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawBoard();
        drawHighlights(tile);
        drawPieces(board, &pieces, tile);


        EndDrawing();
    }

    for (int i = 0; i < 12; i++) UnloadTexture(pieces.tex[i]);
    CloseWindow();
    return 0;
}
