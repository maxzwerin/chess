#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*    LITTLE ENDIAN RANK-FILE MAPPING
      ┌────┬────┬────┬────┬────┬────┬────┬────┐
    8 │ 56 │ 57 │ 58 │ 59 │ 60 │ 61 │ 62 │ 63 │
      ├────┼────┼────┼────┼────┼────┼────┼────┤
    7 │ 48 │ 49 │ 50 │ 51 │ 52 │ 53 │ 54 │ 55 │
      ├────┼────┼────┼────┼────┼────┼────┼────┤
    6 │ 40 │ 41 │ 42 │ 43 │ 44 │ 45 │ 46 │ 47 │
      ├────┼────┼────┼────┼────┼────┼────┼────┤
    5 │ 32 │ 33 │ 34 │ 35 │ 36 │ 37 │ 38 │ 39 │
      ├────┼────┼────┼────┼────┼────┼────┼────┤
    4 │ 24 │ 25 │ 26 │ 27 │ 28 │ 29 │ 30 │ 31 │
      ├────┼────┼────┼────┼────┼────┼────┼────┤
    3 │ 16 │ 17 │ 18 │ 19 │ 20 │ 21 │ 22 │ 23 │
      ├────┼────┼────┼────┼────┼────┼────┼────┤
    2 │ 08 │ 09 │ 10 │ 11 │ 12 │ 13 │ 14 │ 15 │
      ├────┼────┼────┼────┼────┼────┼────┼────┤
    1 │ 00 │ 01 │ 02 │ 03 │ 04 │ 05 │ 06 │ 07 │
      └────┴────┴────┴────┴────┴────┴────┴────┘
        a    b    c    d    e    f    g    h   */

#define MAX_MOVES 218 // Maximum possible number of moves in any position

#define BLACK 0
#define WHITE 1

#define false 0
#define true 1

typedef uint64_t U64;

typedef struct {
    U64 whiteBB;
    U64 blackBB;
    U64 pawnBB;
    U64 knightBB;
    U64 bishopBB;
    U64 rookBB;
    U64 queenBB;
    U64 kingBB;

    int enp;      // 0-63 en passant position, -1 if null
    int castle;   // Castling rights
    int turn;     // Current turn (WHITE or BLACK)
    int fullmove; // Fullmove++ on every move
    int halfmove; // Halfmove++ on every move,
                  // Halfmove = 0 on capture/pawn push
                  // If Halfmove >= 100, draw the game
} Board;

typedef struct {
    int capture; // Captured piece
    int enp;     // Previous en passant square
    int castle;  // Previous castling rights
} Undo;

enum PIECE { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

enum PIECES {
    PAWN_B, KNIGHT_B, BISHOP_B, ROOK_B, QUEEN_B, KING_B,
    PAWN_W, KNIGHT_W, BISHOP_W, ROOK_W, QUEEN_W, KING_W
};

/* fun bit stuff to get castling rights
 * K  = 0b0001  |  k  = 0b0100
 * Q  = 0b0010  |  q  = 0b1000
 * KQ = 0b0011  |  kq = 0b1100  */
enum CASTLING {K=1, Q=2, k=4, q=8, KQ=3, kq=12};

/* from     + 6 bits | 0-63 board position
 * to       + 6 bits | 0-63 board position
 * piece    + 4 bits | piece
 * flag     + 4 bits | determines how flag is encoded
 * --------------------------------
 *           20 !! room to spare !! */
typedef uint32_t Move;

#define EMPTY_FLAG 0
#define CASTLE_FLAG 1
#define CAPTURE_FLAG 4
#define PROMO_FLAG 8
#define KNIGHT_PROMO_FLAG 8
#define ROOK_PROMO_FLAG 9
#define BISHOP_PROMO_FLAG 10
#define QUEEN_PROMO_FLAG 11

#define ENCODE_MOVE(from, to, piece, flag) \
    (((from) & 0x3f) | (((to) & 0x3f) << 6) | (((piece) & 0xf) << 12) | (((flag) & 0xf) << 16))

#define IS_PROMO(flag) ((int)((flag) & PROMO_FLAG))
#define PROMO_PT(flag) ((flag & 0x3) + KNIGHT)
#define IS_CASTLE(flag) ((flag) == CASTLE_FLAG)

#define EXTRACT_FROM(m)  (((m) >> 0)  & 0x3f)
#define EXTRACT_TO(m)    (((m) >> 6)  & 0x3f)
#define EXTRACT_PIECE(m) (((m) >> 12) & 0xf)
#define EXTRACT_FLAG(m)  (((m) >> 16) & 0xf)

#define SWITCH_SIDE(x) (x->turn ^= 1)

#define POP_LSB(bb) ({              \
    int sq = __builtin_ctzll(bb);   \
    bb &= bb - 1;                   \
    sq;                             \
})

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

#endif
