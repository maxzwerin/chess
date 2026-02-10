#include "typedefs.h"

void reset(Board* board) {
    board->whiteBB  = 0ULL;
    board->blackBB  = 0ULL;
    board->pawnBB   = 0ULL;
    board->knightBB = 0ULL;
    board->bishopBB = 0ULL;
    board->rookBB   = 0ULL;
    board->queenBB  = 0ULL;
    board->kingBB   = 0ULL;

    board->enp = -1;
    board->castle = 0;

    board->turn = 1;
    board->halfmove = 0;
    board->fullmove = 1;
}

void setFen(Board* board, char* fen) {
    reset(board);

    int rank = 7;
    int file = 0;

    while (*fen && *fen != ' ') {
        char c = *fen++;

        if (c == '/') {
            rank--;
            file = 0;
            continue;
        }

        if (c >= '1' && c <= '8') {
            file += c - '0';
            continue;
        }

        int square = rank * 8 + file;
        U64 mask = 1ULL << square;

        switch (c) {
            case 'P': board->pawnBB   |= mask; board->whiteBB |= mask; break;
            case 'N': board->knightBB |= mask; board->whiteBB |= mask; break;
            case 'B': board->bishopBB |= mask; board->whiteBB |= mask; break;
            case 'R': board->rookBB   |= mask; board->whiteBB |= mask; break;
            case 'Q': board->queenBB  |= mask; board->whiteBB |= mask; break;
            case 'K': board->kingBB   |= mask; board->whiteBB |= mask; break;

            case 'p': board->pawnBB   |= mask; board->blackBB |= mask; break;
            case 'n': board->knightBB |= mask; board->blackBB |= mask; break;
            case 'b': board->bishopBB |= mask; board->blackBB |= mask; break;
            case 'r': board->rookBB   |= mask; board->blackBB |= mask; break;
            case 'q': board->queenBB  |= mask; board->blackBB |= mask; break;
            case 'k': board->kingBB   |= mask; board->blackBB |= mask; break;
        }

        file++;
    }

    fen++;

    board->turn = (*fen == 'w') ? 1 : 0;
    fen += 2;

    board->castle = 0;
    if (*fen != '-') {
        while (*fen && *fen != ' ') {
            switch (*fen++) {
                case 'K': board->castle |= K; break;
                case 'Q': board->castle |= Q; break;
                case 'k': board->castle |= k; break;
                case 'q': board->castle |= q; break;
            }
        }
    } else {
        fen++;
    }
    fen++;

    if (*fen != '-') {
        int epFile = fen[0] - 'a';
        int epRank = fen[1] - '1';
        board->enp = epRank * 8 + epFile;
        fen += 2;
    } else {
        board->enp = -1;
        fen++;
    }

    if (*fen == ' ') fen++;
    board->halfmove = atoi(fen);

    while (*fen && *fen != ' ') fen++;

    if (*fen == ' ') fen++;
    board->fullmove = atoi(fen);
}

#define GET_BIT(bb, sq) ((bb) & (1ULL << (sq)))

void printChessboard(Board b) {
    for (int rank = 7; rank >= 0; rank--) {
        printf("%d  ", rank + 1);

        for (int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;
            char piece = '.';

            if (GET_BIT(b.pawnBB, sq))        
                piece = GET_BIT(b.whiteBB, sq) ? 'P' : 'p' ;
            else if (GET_BIT(b.knightBB, sq)) 
                piece = GET_BIT(b.whiteBB, sq) ? 'N' : 'n' ;
            else if (GET_BIT(b.bishopBB, sq)) 
                piece = GET_BIT(b.whiteBB, sq) ? 'B' : 'b' ;
            else if (GET_BIT(b.rookBB, sq))   
                piece = GET_BIT(b.whiteBB, sq) ? 'R' : 'r' ;
            else if (GET_BIT(b.queenBB, sq))  
                piece = GET_BIT(b.whiteBB, sq) ? 'Q' : 'q' ;
            else if (GET_BIT(b.kingBB, sq))   
                piece = GET_BIT(b.whiteBB, sq) ? 'K' : 'k' ;

            printf("%c ", piece);
        }
        printf("\n");
    }
    printf("   a b c d e f g h\n");

    printf("\nTurn: %s\n", b.turn == WHITE ? "WHITE" : "BLACK");

    printf("Castling: ");
    if (b.castle == 0) printf("-");
    if (b.castle & K)  printf("K");
    if (b.castle & Q) printf("Q");
    if (b.castle & K)  printf("k");
    if (b.castle & q) printf("q");
    printf("\n");

    if (b.enp != -1) printf("En passant: %c%d\n", 'a' + (b.enp % 8), 1 + (b.enp / 8));
    else                   printf("En passant: -\n");

    printf("Halfmove: %d\n", b.halfmove);
    printf("Fullmove: %d\n", b.fullmove);
}

void printBitboard(U64 bb) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;
            printf("%c ", (bb & (1ULL << sq)) ? '1' : '.');
        }
        printf("\n");
    }
}

void ppBB(U64 bb, int s, int p) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;
            if (sq == s) printf("%c ", p);
            else printf("%c ", (bb & (1ULL << sq)) ? '1' : '.');
        }
        printf("\n");
    }
    printf("\n");
}
