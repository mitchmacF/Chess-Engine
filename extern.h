/* Global Data Structures */
extern struct Lookup_tables *tbls;
extern struct Move_list *mv_list;
extern struct Board *bd;
extern struct Board *undo_bd;
extern char board[64]; 
extern char *move_notation[64];

/* Move Generation Help */
U64 FileAttacks(Square sq, U64 board_state, U64 side);
U64 DiagonalAttacks(Square sq, U64 board_state, U64 side);
U64 AntiDiagonalAttacks(Square sq, U64 board_state, U64 side);
U64 RankAttacks(Square sq, U64 side, U64 opp_side);
extern U64 getPosition(Square sq);
U64 flip(U64 x);
U64 bitScanForward(U64 bb);

/* Move Generation */
extern U64 generate_king_moves(U64 current_king_location, U64 all_pieces, U64 side);
extern U64 generate_castling_moves(U64 current_king_location, U64 all_pieces, U64 side);
extern U64 generate_knight_moves(U64 current_knight_location, U64 all_pieces, U64 side);
extern U64 generate_white_pawn_moves(U64 white_pawn_location, U64 all_pieces, U64 side);
extern U64 generate_black_pawn_moves(U64 black_pawn_location, U64 all_pieces, U64 side);
extern U64 generate_queen_moves(U64 queen_location, U64 all_pieces, U64 side);
extern U64 generate_bishop_moves(U64 bishop_location, U64 all_pieces, U64 side);
extern U64 generate_rook_moves(U64 rook_location, U64 all_pieces, U64 side);
extern U64 enPas();
extern bool attacked(U64 piece_location, U64 board_state, U64 side);
extern void generateAllMoves();
void generateWhiteMoves();
void generateBlackMoves();
extern bool make_move(Move mv);
extern void update_bb(Piece pc, U64 to, U64 from);
extern U64 Pop(U64 bb);
extern void undo_move(struct Board *to, struct Board *from);

/* IO */
extern void printState();
extern void parseFEN(char *FEN);
extern void printboard(U64 n);
extern void printMoveList();
extern void printCharBoard();
extern void fillCharBoard();
extern void printMove(Move mv);

/* Initialization */
extern void init_all();
extern void initState();
extern void copy(struct Board *to, struct Board *from);
