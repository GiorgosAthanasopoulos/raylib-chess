#pragma once

// == NUMBER SETTINGS ==
#define DEFAULT_WINDOW_WIDTH 720
#define DEFAULT_WINDOW_HEIGHT 720
#define TARGET_FPS 60
// NOTE: BOARD_SIZE_AXIS acts more like a const rather than config field.
// Changing it will require modifications in codebase.
#define BOARD_SIZE_AXIS 8
// NOTE: Character = Chess piece
#define CHARACTER_SCALE_FACTOR 3
#define HINT_RADIUS 25 // NOTE: Hint circle radius
#define H1_FONT_SIZE 300
#define WINDOW_FLAGS                                                           \
  FLAG_WINDOW_RESIZABLE // NOTE: window flags separated by '|'
#define TEXT_OFFSET_SCALE                                                      \
  10 // NOTE: for maximized text scale of offset off the window borders

// == STRING/PATH RELATED SETTINGS ==
#define WINDOW_TITLE "Chess"
#define ASSETS_PATH "./assets/"
// NOTE: B stands for Black (Dark variant) and W for White (Light variant)
#define B_BISHOP_TEXTURE_PATH ASSETS_PATH "B_Bishop.png"
#define B_KING_TEXTURE_PATH ASSETS_PATH "B_King.png"
#define B_KNIGHT_TEXTURE_PATH ASSETS_PATH "B_Knight.png"
#define B_PAWN_TEXTURE_PATH ASSETS_PATH "B_Pawn.png"
#define B_QUEEN_TEXTURE_PATH ASSETS_PATH "B_Queen.png"
#define B_ROOK_TEXTURE_PATH ASSETS_PATH "B_Rook.png"
#define W_BISHOP_TEXTURE_PATH ASSETS_PATH "W_Bishop.png"
#define W_KING_TEXTURE_PATH ASSETS_PATH "W_King.png"
#define W_KNIGHT_TEXTURE_PATH ASSETS_PATH "W_Knight.png"
#define W_PAWN_TEXTURE_PATH ASSETS_PATH "W_Pawn.png"
#define W_QUEEN_TEXTURE_PATH ASSETS_PATH "W_Queen.png"
#define W_ROOK_TEXTURE_PATH ASSETS_PATH "W_Rook.png"
#define MOVE_SOUND_PATH ASSETS_PATH "move.wav"
#define WIN_FAVICON_PATH ASSETS_PATH "favicon.png"
#define B_WINNER_TEXT "Winner: Black"
#define W_WINNER_TEXT "Winner: White"

// == COLOR RELATED SETTINGS ==
#define WINDOW_BG_COLOR BLACK
// NOTE: Choose what color to make board(specify light/dark color)
#define LIGHT_COLOR WHITE
#define DARK_COLOR BLACK
// NOTE: What color should the board start with? Dark or light variant?
#define FIRST_TILE_COLOR DARK_COLOR
#define B_CHARACTER_COLOR GRAY
#define W_CHARACTER_COLOR BEIGE
// NOTE: who takes the turn first
#define STARTING_COLOR B_CHARACTER_COLOR
#define HINT_COLOR GREEN
#define ENEMY_COLOR RED
#define TRANSPARENT                                                            \
  (Color) { 0, 0, 0, 0 }
#define NO_WINNER_COLOR TRANSPARENT
#define TOP_SIDE_COLOR                                                         \
  B_CHARACTER_COLOR // are B or W at the top side of the board?

// == KEY RELATED SETTINGS ==
#define EXIT_KEY KEY_NULL
#define RESTART_KEY KEY_ESCAPE
