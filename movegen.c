#include "typedefs.h"
#include "utils.c"

U64 KNIGHT_REF[64];
U64 KING_REF[64];
U64 ROOK_REF[64];
U64 BISHOP_REF[64];
U64 BISHOP_ATTACKS[64][512];
U64 ROOK_ATTACKS[64][4096];
U64 PAWN_ATTACKS[2][64];

int ROOK_RELEVANT_BITS[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

int BISHOP_RELEVANT_BITS[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

const U64 ROOK_MAGICS[64] = {
    0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL, 0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL,
    0xa000800080400034ULL, 0x4808020004000ULL, 0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL, 0x1002100004082ULL,
    0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL, 0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL,
    0x800080204005ULL, 0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL, 0x9020010080800200ULL, 0x813241200148449ULL,
    0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL, 0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
    0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL, 0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL,
    0x280009023410300ULL, 0xe0100040002240ULL, 0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL, 0x2000009044210200ULL,
    0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL, 0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL,
};

const U64 BISHOP_MAGICS[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL, 0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL,
    0x400492088408100ULL, 0x201c401040c0084ULL, 0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL, 0x8144042209100900ULL,
    0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL, 0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL,
    0x520040470104290ULL, 0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL, 0x188071040440a00ULL, 0x4800404002011c00ULL,
    0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL, 0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
    0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL, 0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL,
    0x111040120082000ULL, 0x6803040141280a00ULL, 0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL, 0x822088220820214ULL,
    0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL, 0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
};

U64 occupancyMask(int index, int bits, U64 attackMask) {
    U64 occ = 0ULL;

    for (int i = 0; i < bits; i++) {
        int square = __builtin_ctzll(attackMask);
        attackMask &= attackMask - 1;

        if (index & (1 << i)) occ |= (1ULL << square);
    }

    return occ;
}

U64 rayAttacks(int square, int dr, int df, U64 block) {
    U64 attacks = 0ULL;

    int r = square / 8;
    int f = square % 8;

    r += dr; f += df;

    while (r >= 0 && r <= 7 && f >= 0 && f <= 7) {
        int sq = r * 8 + f;
        U64 bit = 1ULL << sq;

        attacks |= bit;
        if (block & bit) break;

        r += dr; f += df;
    }

    return attacks;
}

U64 bishopAttacksOnTheFly(int square, U64 block) {
    return rayAttacks(square,  1,  1, block) | rayAttacks(square,  1, -1, block) | 
           rayAttacks(square, -1,  1, block) | rayAttacks(square, -1, -1, block);
}

U64 rookAttacksOnTheFly(int square, U64 block) {
    return rayAttacks(square,  1,  0, block) | rayAttacks(square, -1,  0, block) |
           rayAttacks(square,  0,  1, block) | rayAttacks(square,  0, -1, block);
}

U64 bishopMovement(int square) {
    int f, r;
    int tf = square % 8;
    int tr = square / 8;
    
    U64 movement = 0ULL;
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) movement |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) movement |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) movement |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) movement |= (1ULL << (r * 8 + f));
    
    return movement;
}

U64 rookMovement(int square) {
    int f, r;
    int tf = square % 8;
    int tr = square / 8;
    U64 movement = 0ULL;

    for (r = tr + 1; r <= 6; r++) movement |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--) movement |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++) movement |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--) movement |= (1ULL << (tr * 8 + f));

    return movement;
}

void initSliders() {
    for (int square = 0; square < 64; square++) {
        BISHOP_REF[square] = bishopMovement(square);
        ROOK_REF[square] = rookMovement(square);
        
        U64 bishopMask = BISHOP_REF[square];
        U64 rookMask = ROOK_REF[square];
        int bishopRelevantBits = BISHOP_RELEVANT_BITS[square];
        int rookRelevantBits = ROOK_RELEVANT_BITS[square];
        int bishopOccupancyVariations = 1 << bishopRelevantBits;
        int rookOccupancyVariations = 1 << rookRelevantBits;

        for (int i = 0; i < bishopOccupancyVariations; i++) {
            U64 occ = occupancyMask(i, bishopRelevantBits, bishopMask);
            U64 magic_id = occ * BISHOP_MAGICS[square] >> (64 - bishopRelevantBits);
            BISHOP_ATTACKS[square][magic_id] = bishopAttacksOnTheFly(square, occ);                
        }

        for (int i = 0; i < rookOccupancyVariations; i++) {
            U64 occ = occupancyMask(i, rookRelevantBits, rookMask);
            U64 magic_id = occ * ROOK_MAGICS[square] >> (64 - rookRelevantBits);
            ROOK_ATTACKS[square][magic_id] = rookAttacksOnTheFly(square, occ);                
        }
    }
}

void initPawnAttacks() {
    for (int sq = 0; sq < 64; sq++) {
        U64 bb = 1ULL << sq;

        PAWN_ATTACKS[WHITE][sq] = ((bb << 7) & ~FILE_H) | ((bb << 9) & ~FILE_A);
        PAWN_ATTACKS[BLACK][sq] = ((bb >> 7) & ~FILE_A) | ((bb >> 9) & ~FILE_H);
    }
}

U64 getBishopAttacks(int square, U64 occupancy) {
	occupancy &= BISHOP_REF[square];
	occupancy *=  BISHOP_MAGICS[square];
	occupancy >>= 64 - BISHOP_RELEVANT_BITS[square];
	return BISHOP_ATTACKS[square][occupancy];
}

U64 getRookAttacks(int square, U64 occ) {
	occ &= ROOK_REF[square];
	occ *=  ROOK_MAGICS[square];
	occ >>= 64 - ROOK_RELEVANT_BITS[square];
	return ROOK_ATTACKS[square][occ];
}

U64 getKnightAttacks(int sq) {
    U64 knight = 1ULL << sq; 
    U64 FILE_AB = FILE_A | FILE_B;
    U64 FILE_GH = FILE_G | FILE_H;
    U64 attacks = ((knight >> 15) & ~FILE_A)  | ((knight << 15) & ~FILE_H)  |
                  ((knight << 10) & ~FILE_AB) | ((knight >> 10) & ~FILE_GH) |
                  ((knight << 17) & ~FILE_A)  | ((knight >> 17) & ~FILE_H)  |
                  ((knight << 6)  & ~FILE_GH) | ((knight >> 6)  & ~FILE_AB);

    /* add a 64-bit mask to discard any "off-board" bits before returning */
    return attacks & 0xFFFFFFFFFFFFFFFF;
}

U64 getKingAttacks(int sq) {
    U64 king = 1ULL << sq; 
    U64 attacks = ((king >> 1) & ~FILE_H) | ((king << 1) & ~FILE_A) | 
                  ((king >> 7) & ~FILE_A) | ((king >> 9) & ~FILE_H) |
                  ((king << 7) & ~FILE_H) | ((king << 9) & ~FILE_A) |
                  (king >> 8) | (king << 8);

    return attacks & 0xFFFFFFFFFFFFFFFF;
}

U64 getPawnAttacks(int sq, Board board, int color) {
    U64 pawn = 1ULL << sq; 
    U64 attacks = 0ULL;

    if (color) attacks = ((pawn & ~FILE_A) << 7) | ((pawn & ~FILE_H) << 9);
    else attacks = (pawn >> 7 & ~FILE_A) | (pawn >> 9 & ~FILE_H);

    return attacks & 0xFFFFFFFFFFFFFFFF;
}

U64 getPawnMoves(int square, Board board, int color) {
    U64 pawn = 1ULL << square;
    U64 friendly = color ? board.whiteBB : board.blackBB;
    U64 occ = board.whiteBB | board.blackBB;

    U64 start_rank = color ? RANK_2 : RANK_7;

    U64 single = 0ULL;
    U64 dbl = 0ULL;
    U64 attacks = 0ULL;
    U64 enp = 0ULL;

    single = color ? pawn << 8 : pawn >> 8;
    single &= ~occ;

    if (pawn & start_rank && single) {
        dbl = color ? pawn << 16 : pawn >> 16;
        dbl &= ~occ;
    }

    if (board.enp >= 0) enp = 1ULL << board.enp;

    attacks = (PAWN_ATTACKS[color][square] & (occ | enp));

    attacks = attacks | single | dbl;
    return attacks & ~friendly;
}

void initKnightMovementTable(void) {
    for (int sq = 0; sq < 64; sq++) KNIGHT_REF[sq] = getKnightAttacks(sq);
}

void initKingMovementTable(void) {
    for (int sq = 0; sq < 64; sq++) KING_REF[sq] = getKingAttacks(sq);
}

void initMoveGen() {
    initKnightMovementTable();
    initKingMovementTable();
    initPawnAttacks();
    initSliders();
}

int isSquareAttacked(Board b, int sq) {
    U64 occ          = b.whiteBB | b.blackBB;
    U64 enemyOcc     = b.turn ? b.blackBB : b.whiteBB;
    U64 enemyPawns   = b.pawnBB   & enemyOcc;
    U64 enemyKnights = b.knightBB & enemyOcc;
    U64 enemyBishops = b.bishopBB & enemyOcc;
    U64 enemyRooks   = b.rookBB   & enemyOcc;
    U64 enemyQueens  = b.queenBB  & enemyOcc;
    U64 enemyKing    = b.kingBB   & enemyOcc;

    if (PAWN_ATTACKS[b.turn][sq] & enemyPawns) return 1;
    if (KNIGHT_REF[sq] & enemyKnights) return 1;
    if (KING_REF[sq] & enemyKing) return 1;

    if (getBishopAttacks(sq, occ) & (enemyBishops | enemyQueens)) return 1;
    if (getRookAttacks(sq, occ) & (enemyRooks | enemyQueens)) return 1;

    return 0;
}

void clearCastleRights(Board *board, int clear) {
    board->castle &= ~clear;
}

void pushMove(Board *board, Move move) {
    int piece = move.piece % 6;
    int turn = board->turn;
    U64 fromMask = 1ULL << move.from;

    int isEnp = move.to == board->enp && piece == PAWN;

    /* en passant management */
    if (isEnp) {
        int toSq = board->enp;
        int capturedSq = board->enp + (board->turn ? -8 : 8);
        U64 toMask = 1ULL << toSq;
        U64 capMask = 1ULL << capturedSq;

        board->pawnBB ^= fromMask | toMask | capMask;
        if (turn) {
            board->whiteBB ^= fromMask | toMask;
            board->blackBB ^= capMask;
        } else {
            board->blackBB ^= fromMask | toMask;
            board->whiteBB ^= capMask;
        }

        board->enp = -1;
        board->turn = !turn;
        return;
    }

    /* clear en passant sq */
    board->enp = -1;

    /* clear castle rights if king / rook moves */
    /* check if rook was captured later */
    if (board->castle) {
        if (move.piece == KING_W) clearCastleRights(board, KQ);
        if (move.piece == KING_B) clearCastleRights(board, kq);

        if (move.piece == ROOK_W && move.from == H1) clearCastleRights(board, K);
        if (move.piece == ROOK_W && move.from == A1) clearCastleRights(board, Q);

        if (move.piece == ROOK_B && move.from == H8) clearCastleRights(board, k);
        if (move.piece == ROOK_B && move.from == A8) clearCastleRights(board, q);
    }

    /* deal with castling */
    if (move.castle) {
        if (move.castle == K) {
            board->kingBB  &= ~(1ULL << E1); board->kingBB  |= 1ULL << G1;
            board->whiteBB &= ~(1ULL << E1); board->whiteBB |= 1ULL << G1;
            board->rookBB  &= ~(1ULL << H1); board->rookBB  |= 1ULL << F1;
            board->whiteBB &= ~(1ULL << H1); board->whiteBB |= 1ULL << F1;
        } else if (move.castle == Q) {
            board->kingBB  &= ~(1ULL << E1); board->kingBB  |= 1ULL << C1;
            board->whiteBB &= ~(1ULL << E1); board->whiteBB |= 1ULL << C1;
            board->rookBB  &= ~(1ULL << A1); board->rookBB  |= 1ULL << D1;
            board->whiteBB &= ~(1ULL << A1); board->whiteBB |= 1ULL << D1;
        } else if (move.castle == k) {
            board->kingBB  &= ~(1ULL << E8); board->kingBB  |= 1ULL << G8;
            board->blackBB &= ~(1ULL << E8); board->blackBB |= 1ULL << G8;
            board->rookBB  &= ~(1ULL << H8); board->rookBB  |= 1ULL << F8;
            board->blackBB &= ~(1ULL << H8); board->blackBB |= 1ULL << F8;
        } else if (move.castle == q) {
            board->kingBB  &= ~(1ULL << E8); board->kingBB  |= 1ULL << C8;
            board->blackBB &= ~(1ULL << E8); board->blackBB |= 1ULL << C8;
            board->rookBB  &= ~(1ULL << A8); board->rookBB  |= 1ULL << D8;
            board->blackBB &= ~(1ULL << A8); board->blackBB |= 1ULL << D8;
        }

        /* clear castle rights for color and return */
        if (turn) clearCastleRights(board, KQ);
        else clearCastleRights(board, kq);

        board->turn = !turn;
        return;
    }

    U64 *pieceThatMovedBB = (&board->pawnBB + piece);
    U64 *curColorBB = &board->blackBB - turn;
    U64 *oppColorBB = &board->whiteBB + turn;

    U64 toMask = 1ULL << move.to;

    /* pick up piece, wait to put down*/
    *pieceThatMovedBB ^= 1ULL << move.from; *curColorBB ^= 1ULL << move.from;

    /* check if piece has been captured */
    if (*oppColorBB & toMask) {
        *oppColorBB ^= toMask;

        for (int p = 0; p < 6; p++) {
            U64 *bb = (&board->pawnBB + p);
            if (*bb & toMask) {
                *bb ^= toMask; /* remove from piece BB */

                /* now check if rook was captured from starting sq.
                 * if so, clear its castle rights */
                if (p == ROOK && move.to == H1) clearCastleRights(board, K);
                if (p == ROOK && move.to == A1) clearCastleRights(board, Q);
                if (p == ROOK && move.to == H8) clearCastleRights(board, k);
                if (p == ROOK && move.to == A8) clearCastleRights(board, q);

                break;
            }
        }
    }

    /* check if promotion / put down piece */
    if (piece == PAWN && move.promo) {
        U64 promoMask = 1ULL << move.to;
        U64 *promoBB = (&board->pawnBB + move.promo % 6);
        *promoBB |= promoMask;
        *curColorBB |= promoMask;
    } else {
        *pieceThatMovedBB |= 1ULL << move.to; *curColorBB |= 1ULL << move.to;
    }

    /* set en passant if double move from pawns starting rank */
    U64 startRank = turn ? RANK_2 : RANK_7;
    if (piece == PAWN && (fromMask & startRank)) {
        if (abs(move.from - move.to) == 16) {
            board->enp = turn ? move.to - 8 : move.to + 8;
        }
    }

    /* finally switch turn */
    board->turn = !turn;
    return;
}

int validateMove(Board board, Move* move) {
    U64 kingBB = 0ULL;
    int kingSq = -1;

    if (move->castle) {
        /* does king travel over attacked squares? */
        int sq;
        if (move->castle == K) sq = F1;
        if (move->castle == Q) sq = D1;
        if (move->castle == k) sq = F8;
        if (move->castle == q) sq = D8;
        if (isSquareAttacked(board, sq)) return false;

        kingBB = board.kingBB & (board.turn ? board.whiteBB : board.blackBB);
        kingSq = __builtin_ctzll(kingBB);

        if (isSquareAttacked(board, kingSq)) return false;
    }

    Board cpy = board;
    pushMove(&cpy, *move);

    /* is king attacked after the move is made? */
    kingBB = cpy.kingBB & (cpy.turn ? cpy.blackBB : cpy.whiteBB);
    cpy.turn = !cpy.turn;
    kingSq = __builtin_ctzll(kingBB);
    if (isSquareAttacked(cpy, kingSq)) return false;

    return true;
}

int canCastle(Board *board, int right, U64 occ) {
    if (!(board->castle & right)) return 0;

    if ((right == K || right == Q) && !board->turn) return 0;
    if ((right == k || right == q) && board->turn) return 0;

    switch (right) {
        case K: if (occ & ((1ULL << F1) | (1ULL << G1))) return 0; break;
        case Q: if (occ & ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))) return 0; break;
        case k: if (occ & ((1ULL << F8) | (1ULL << G8))) return 0; break;
        case q: if (occ & ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))) return 0; break;
        default: return 0;
    }

    return 1;
}

/* generate all legal moves for the given board */
int legalMoves(Board *board, Move moves[]) {
    int length = 0;

    int turn = board->turn;
    U64 pawnBB   = board->pawnBB   & (turn ? board->whiteBB : board->blackBB);
    U64 knightBB = board->knightBB & (turn ? board->whiteBB : board->blackBB);
    U64 rookBB   = board->rookBB   & (turn ? board->whiteBB : board->blackBB);
    U64 bishopBB = board->bishopBB & (turn ? board->whiteBB : board->blackBB);
    U64 queenBB  = board->queenBB  & (turn ? board->whiteBB : board->blackBB);
    U64 kingBB   = board->kingBB   & (turn ? board->whiteBB : board->blackBB);

    U64 friendly  = turn ? board->whiteBB : board->blackBB;
    U64 occupancy = board->whiteBB | board->blackBB;
    int kingSq = __builtin_ctzll(kingBB);

    U64 attackMask = 0ULL;

    /* generate all piece movement */
    while (pawnBB) {
        int sq = __builtin_ctzll(pawnBB);
        attackMask = getPawnMoves(sq, *board, turn);

        while (attackMask) {
            int to = __builtin_ctzll(attackMask);
            U64 toBB = 1ULL << to;
            U64 isPromoting = turn ? (toBB & RANK_8) : (toBB & RANK_1);

            if (isPromoting) {
                Move q = ENCODE_MOVE(sq, to, QUEEN+turn*6, 0, PAWN+turn*6);
                Move n = ENCODE_MOVE(sq, to, KNIGHT+turn*6, 0, PAWN+turn*6);
                Move r = ENCODE_MOVE(sq, to, ROOK+turn*6, 0, PAWN+turn*6);
                Move b = ENCODE_MOVE(sq, to, BISHOP+turn*6, 0, PAWN+turn*6);

                if (validateMove(*board, &q)) moves[length++] = q;
                if (validateMove(*board, &n)) moves[length++] = n;
                if (validateMove(*board, &r)) moves[length++] = r;
                if (validateMove(*board, &b)) moves[length++] = b;
            } else {
                Move move = ENCODE_MOVE(sq, to, 0, 0, PAWN+turn*6);
                if (validateMove(*board, &move)) moves[length++] = move;
            }
            attackMask &= attackMask - 1;
        }
        pawnBB &= pawnBB - 1;
    }
    while (knightBB) {
        int sq = __builtin_ctzll(knightBB);
        attackMask = KNIGHT_REF[sq] & ~friendly;

        while (attackMask) {
            int to = __builtin_ctzll(attackMask);

            Move move = ENCODE_MOVE(sq, to, 0, 0, KNIGHT+turn*6);
            if (validateMove(*board, &move)) moves[length++] = move; 
            attackMask &= attackMask - 1;
        }
        knightBB &= knightBB - 1;
    }
    while (rookBB) {
        int sq = __builtin_ctzll(rookBB);
        attackMask = getRookAttacks(sq, occupancy) & ~friendly;

        while (attackMask) {
            int to = __builtin_ctzll(attackMask);

            Move move = ENCODE_MOVE(sq, to, 0, 0, ROOK+turn*6);
            if (validateMove(*board, &move)) moves[length++] = move;
            attackMask &= attackMask - 1;
        }
        rookBB &= rookBB - 1;
    }
    while (bishopBB) {
        int sq = __builtin_ctzll(bishopBB);
        attackMask = getBishopAttacks(sq, occupancy) & ~friendly;

        while (attackMask) {
            int to = __builtin_ctzll(attackMask);

            Move move = ENCODE_MOVE(sq, to, 0, 0, BISHOP+turn*6);
            if (validateMove(*board, &move)) moves[length++] = move;
            attackMask &= attackMask - 1;
        }
        bishopBB &= bishopBB - 1;
    }
    while (queenBB) {
        int sq = __builtin_ctzll(queenBB);
        attackMask = (getRookAttacks(sq, occupancy) | 
            getBishopAttacks(sq, occupancy)) & ~friendly;

        while (attackMask) {
            int to = __builtin_ctzll(attackMask);

            Move move = ENCODE_MOVE(sq, to, 0, 0, QUEEN+turn*6);
            if (validateMove(*board, &move)) moves[length++] = move;
            attackMask &= attackMask - 1;
        }
        queenBB &= queenBB - 1;
    }

    attackMask = KING_REF[kingSq] & ~friendly;

    while (attackMask) {
        int to = __builtin_ctzll(attackMask);
        Move move = ENCODE_MOVE(kingSq, to, 0, 0, KING+turn*6);
        if (validateMove(*board, &move)) moves[length++] = move;
        attackMask &= attackMask - 1;
    }

    int right[4] = {K, Q, k, q};
    int moveTo[4] = {G1, C1, G8, C8};
    for (int i = 0; i < 4; i++) {
        if (canCastle(board, right[i], occupancy)) {
            Move move = ENCODE_MOVE(kingSq, moveTo[i], 0, right[i], KING+turn*6);
            if (validateMove(*board, &move)) moves[length++] = move;
        }
    }

    return length;
}

void makeMove(Board *board, Move move) {
    if (validateMove(*board, &move)) {
        pushMove(board, move);
    }
}

int inCheck(Board *board, int color) {
    U64 kingBB = board->kingBB & (color ? board->whiteBB : board->blackBB);
    int kingSq = __builtin_ctzll(kingBB);

    if (isSquareAttacked(*board, kingSq)) return false;
    return true;
}

void makemove(Board *board, Move move, Undo *undo);
void undoMove(Board *board, Undo *undo);
int getTurn();
int inCheckmate();
int inDraw();
