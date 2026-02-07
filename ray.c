#include "raylib.h"

#ifdef WHITE
#undef WHITE
#endif
#ifdef BLACK
#undef BLACK
#endif

#include "main.c"

#define WIDTH 800
#define HEIGHT 800



int selectedSq = -1;
int legalMap[64] = {0};

char pieceOnSquare(Board b, int sq) {
    U64 mask = 1ULL << sq;
    if (b.pawnBB   & mask) return (b.whiteBB & mask) ? 'P' : 'p';
    if (b.knightBB & mask) return (b.whiteBB & mask) ? 'N' : 'n';
    if (b.bishopBB & mask) return (b.whiteBB & mask) ? 'B' : 'b';
    if (b.rookBB   & mask) return (b.whiteBB & mask) ? 'R' : 'r';
    if (b.queenBB  & mask) return (b.whiteBB & mask) ? 'Q' : 'q';
    if (b.kingBB   & mask) return (b.whiteBB & mask) ? 'K' : 'k';
    return ' ';
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
    for (int i = 0; i < 64; i++)
        legalMap[i] = false;
}

void buildLegalMap(Move *moves, int moveCount, int fromSq) {
    clearLegal();

    for (int i = 0; i < moveCount; i++) {
        int from = moves[i].from;
        int to   = moves[i].to;

        if (from == fromSq)
            legalMap[to] = true;
    }
}

void drawBoard() {
    int tile = WIDTH / 8;

    for (int file = 0; file < 8; file++) {
        for (int rank = 0; rank < 8; rank++) {

            int isLight = (file + rank) % 2 == 0;

            Color c = isLight
                ? (Color){235,209,166,255}
                : (Color){165,117,80,255};

            DrawRectangle(file * tile, rank * tile, tile, tile, c);
        }
    }
}

void drawHighlights(int tile) {

    // Selected square
    if (selectedSq != -1) {
        int f = selectedSq % 8;
        int r = selectedSq / 8;

        int x = f * tile;
        int y = (7 - r) * tile;

        DrawRectangle(x, y, tile, tile, (Color){0,255,0,90});
    }

    // Legal moves
    for (int sq = 0; sq < 64; sq++) {
        if (!legalMap[sq]) continue;

        int f = sq % 8;
        int r = sq / 8;

        int x = f * tile;
        int y = (7 - r) * tile;

        DrawRectangle(x, y, tile, tile, (Color){255,0,0,90});
    }
}

void drawPieces(Board b, int tile) {
    for (int sq = 0; sq < 64; sq++) {

        char p = pieceOnSquare(b, sq);
        if (p == ' ') continue;

        int file = sq % 8;
        int rank = sq / 8;

        int x = file * tile + tile/3;
        int y = (7 - rank) * tile + tile/4;

        DrawText(TextFormat("%c", p), x, y, tile/2,
                 (Color){0,0,0,255});
    }
}

int main(void) {

    initMoveGen();

    Board board;
    Move moves[MAX_MOVES];

    // setFen(&board, START_FEN);
    setFen(&board, "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1");
    printChessboard(board);

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(WIDTH, HEIGHT, "");
    SetTargetFPS(60);

    int tile = WIDTH / 8;

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
                    if (moves[i].from == selectedSq &&
                        moves[i].to == sq) {

                        makeMove(&board, moves[i]);
                        printChessboard(board);
                        break;
                    }
                }

                selectedSq = -1;
                clearLegal();
            }

            else if (p != ' ') {

                selectedSq = sq;

                int moveCount = legalMoves(&board, moves);
                buildLegalMap(moves, moveCount, selectedSq);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawBoard();
        drawHighlights(tile);
        drawPieces(board, tile);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
