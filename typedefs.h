#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef uint64_t U64;

typedef struct {
    U64 whiteBB;   // White pieces
    U64 blackBB;   // Black pieces
    U64 pawnBB;    // Pawns
    U64 knightBB;  // Knights
    U64 bishopBB;  // Bishops
    U64 rookBB;    // Rooks
    U64 queenBB;   // Queens
    U64 kingBB;    // Kings

    int enp;       // 0-63 en passant position, -1 if null
    int castle;    // Castling rights
    int turn;      // Current turn (WHITE or BLACK)
    int fullmove;  // Fullmove++ on every move
    int halfmove;  // Halfmove++ on every move, 
                   // Halfmove = 0 on any capture/pawn push
                   // If Halfmove >= 100, game ends in a draw
} Board;

typedef struct {
    int from;
    int to;
    int promo;
    int castle;
    int piece;
} Move;

#define ENCODE_MOVE(FROM, TO, PROMO, CASTLE, PIECE) {.from=FROM, .to=TO, .promo=PROMO, .castle=CASTLE, .piece=PIECE}

#define RANK_1 0x00000000000000ffULL
#define RANK_2 0x000000000000ff00ULL
#define RANK_3 0x0000000000ff0000ULL
#define RANK_4 0x00000000ff000000ULL
#define RANK_5 0x000000ff00000000ULL
#define RANK_6 0x0000ff0000000000ULL
#define RANK_7 0x00ff000000000000ULL
#define RANK_8 0xff00000000000000ULL

#define FILE_A 0x0101010101010101ULL
#define FILE_B 0x0202020202020202ULL
#define FILE_C 0x0404040404040404ULL
#define FILE_D 0x0808080808080808ULL
#define FILE_E 0x1010101010101010ULL
#define FILE_F 0x2020202020202020ULL
#define FILE_G 0x4040404040404040ULL
#define FILE_H 0x8080808080808080ULL

#define BLACK 0
#define WHITE 1

#define false 0
#define true 1

#define MAX_MOVES 218

enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

enum PIECES {
    PAWN_B, KNIGHT_B, BISHOP_B, ROOK_B, QUEEN_B, KING_B,
    PAWN_W, KNIGHT_W, BISHOP_W, ROOK_W, QUEEN_W, KING_W
};

enum CASTLING {K=1, Q=2, k=4, q=8};

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum SQUARES { 
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
};
