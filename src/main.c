#include <malloc.h>
#include <memory.h>
#include <raylib.h>

#define DEFAULT_WINDOW_WIDTH 720
#define DEFAULT_WINDOW_HEIGHT 720
#define WINDOW_TITLE "Chess"
#define TARGET_FPS 60
// WARNING: BOARD_SIZE_AXIS acts more like a const rather than config field.
// Changing it will require modifications in codebase.
#define BOARD_SIZE_AXIS 8
#define FIRST_TILE_COLOR WHITE

#define ASSETS "./assets/"

#define B_BISHOP_TEXTURE_PATH ASSETS "B_Bishop.png"
#define B_KING_TEXTURE_PATH ASSETS "B_King.png"
#define B_KNIGHT_TEXTURE_PATH ASSETS "B_Knight.png"
#define B_PAWN_TEXTURE_PATH ASSETS "B_Pawn.png"
#define B_QUEEN_TEXTURE_PATH ASSETS "B_Queen.png"
#define B_ROOK_TEXTURE_PATH ASSETS "B_Rook.png"

#define W_BISHOP_TEXTURE_PATH ASSETS "W_Bishop.png"
#define W_KING_TEXTURE_PATH ASSETS "W_King.png"
#define W_KNIGHT_TEXTURE_PATH ASSETS "W_Knight.png"
#define W_PAWN_TEXTURE_PATH ASSETS "W_Pawn.png"
#define W_QUEEN_TEXTURE_PATH ASSETS "W_Queen.png"
#define W_ROOK_TEXTURE_PATH ASSETS "W_Rook.png"

#define CHARACTER_SCALE_FACTOR 3
#define B_CHARACTER_COLOR GRAY
#define W_CHARACTER_COLOR BEIGE

#define HINT_COLOR GREEN // TODO:

typedef struct {
  Texture2D B_Bishop;
  Texture2D B_King;
  Texture2D B_Knight;
  Texture2D B_Pawn;
  Texture2D B_Queen;
  Texture2D B_Rook;

  Texture2D W_Bishop;
  Texture2D W_King;
  Texture2D W_Knight;
  Texture2D W_Pawn;
  Texture2D W_Queen;
  Texture2D W_Rook;
} Assets;

typedef enum {
  BISHOP,
  KING,
  KNIGHT,
  PAWN,
  QUEEN,
  ROOK,
  EMPTY,
  HINT,
} CharacterType;

typedef struct {
  CharacterType type;
  Color color;
  Texture2D texture;
} Character;

void LoadAssets(Assets *assets) {
  assets->B_Bishop = LoadTexture(B_BISHOP_TEXTURE_PATH);
  assets->B_King = LoadTexture(B_KING_TEXTURE_PATH);
  assets->B_Knight = LoadTexture(B_KNIGHT_TEXTURE_PATH);
  assets->B_Pawn = LoadTexture(B_PAWN_TEXTURE_PATH);
  assets->B_Queen = LoadTexture(B_QUEEN_TEXTURE_PATH);
  assets->B_Rook = LoadTexture(B_ROOK_TEXTURE_PATH);

  assets->W_Bishop = LoadTexture(W_BISHOP_TEXTURE_PATH);
  assets->W_King = LoadTexture(W_KING_TEXTURE_PATH);
  assets->W_Knight = LoadTexture(W_KNIGHT_TEXTURE_PATH);
  assets->W_Pawn = LoadTexture(W_PAWN_TEXTURE_PATH);
  assets->W_Queen = LoadTexture(W_QUEEN_TEXTURE_PATH);
  assets->W_Rook = LoadTexture(W_ROOK_TEXTURE_PATH);
}

void UnloadAssets(Assets *assets) {
  UnloadTexture(assets->B_Bishop);
  UnloadTexture(assets->B_King);
  UnloadTexture(assets->B_Knight);
  UnloadTexture(assets->B_Pawn);
  UnloadTexture(assets->B_Queen);
  UnloadTexture(assets->B_Rook);

  UnloadTexture(assets->W_Bishop);
  UnloadTexture(assets->W_King);
  UnloadTexture(assets->W_Knight);
  UnloadTexture(assets->W_Pawn);
  UnloadTexture(assets->W_Queen);
  UnloadTexture(assets->W_Rook);
}

void Draw(int w, int h, Character *board) {
  BeginDrawing();
  ClearBackground(BLACK); // we just want to clear the bg, dont care about
                          // modularizing the bg

  Color bg = FIRST_TILE_COLOR;
  int tileW = w / BOARD_SIZE_AXIS;
  int tileH = h / BOARD_SIZE_AXIS;

  for (int y = 0; y < BOARD_SIZE_AXIS; ++y) {
    for (int x = 0; x < BOARD_SIZE_AXIS; ++x) {
      int xx = x * tileW, yy = y * tileH;

      // draw tile
      Rectangle tileRec = {xx, yy, tileW, tileH};
      DrawRectangleRec(tileRec, bg);
      bg = ColorToInt(bg) == ColorToInt(WHITE) ? BLACK : WHITE;

      int index = BOARD_SIZE_AXIS * y + x;
      Character character = board[index];

      // draw characters
      if (character.type != EMPTY && character.type != HINT) {
        Vector2 characterVec = {
            xx + ((float)tileW / 2) -
                ((float)character.texture.width * CHARACTER_SCALE_FACTOR / 2),
            yy + (float)tileH / 2 -
                (float)character.texture.height * (CHARACTER_SCALE_FACTOR + 1) /
                    2};
        DrawTextureEx(character.texture, characterVec, 0,
                      CHARACTER_SCALE_FACTOR, character.color);
      }
    }
    bg = ColorToInt(bg) == ColorToInt(WHITE) ? BLACK : WHITE;
  }

  EndDrawing();
}

void Update(int w, int h, Character *board) {
  // TODO: check if user clicks on tile,
  // TODO: if he does then get available position's tiles
  // TODO: highlight the returned tiles

  // TODO: if a user clicks on a tile twice hide hints
  // TODO: if a user clicks on a different tile while hinting remove old
  // hints and render new ones
}

int main() {
  int w = DEFAULT_WINDOW_WIDTH, h = DEFAULT_WINDOW_HEIGHT;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(w, h, WINDOW_TITLE);
  SetTargetFPS(TARGET_FPS);

  Assets assets = {0};
  LoadAssets(&assets);

  Character board[BOARD_SIZE_AXIS * BOARD_SIZE_AXIS] = {
      {ROOK, B_CHARACTER_COLOR, assets.B_Rook},
      {KNIGHT, B_CHARACTER_COLOR, assets.B_Knight},
      {BISHOP, B_CHARACTER_COLOR, assets.B_Bishop},
      {KING, B_CHARACTER_COLOR, assets.B_King},
      {QUEEN, B_CHARACTER_COLOR, assets.B_Queen},
      {BISHOP, B_CHARACTER_COLOR, assets.B_Bishop},
      {KNIGHT, B_CHARACTER_COLOR, assets.B_Knight},
      {ROOK, B_CHARACTER_COLOR, assets.B_Rook},

      {PAWN, B_CHARACTER_COLOR, assets.B_Pawn},
      {PAWN, B_CHARACTER_COLOR, assets.B_Pawn},
      {PAWN, B_CHARACTER_COLOR, assets.B_Pawn},
      {PAWN, B_CHARACTER_COLOR, assets.B_Pawn},
      {PAWN, B_CHARACTER_COLOR, assets.B_Pawn},
      {PAWN, B_CHARACTER_COLOR, assets.B_Pawn},
      {PAWN, B_CHARACTER_COLOR, assets.B_Pawn},
      {PAWN, B_CHARACTER_COLOR, assets.B_Pawn},

      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},

      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},

      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},

      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},
      {EMPTY},

      {PAWN, W_CHARACTER_COLOR, assets.W_Pawn},
      {PAWN, W_CHARACTER_COLOR, assets.W_Pawn},
      {PAWN, W_CHARACTER_COLOR, assets.W_Pawn},
      {PAWN, W_CHARACTER_COLOR, assets.W_Pawn},
      {PAWN, W_CHARACTER_COLOR, assets.W_Pawn},
      {PAWN, W_CHARACTER_COLOR, assets.W_Pawn},
      {PAWN, W_CHARACTER_COLOR, assets.W_Pawn},
      {PAWN, W_CHARACTER_COLOR, assets.W_Pawn},

      {ROOK, W_CHARACTER_COLOR, assets.W_Rook},
      {KNIGHT, W_CHARACTER_COLOR, assets.W_Knight},
      {BISHOP, W_CHARACTER_COLOR, assets.W_Bishop},
      {QUEEN, W_CHARACTER_COLOR, assets.W_Queen},
      {KING, W_CHARACTER_COLOR, assets.W_King},
      {BISHOP, W_CHARACTER_COLOR, assets.W_Bishop},
      {KNIGHT, W_CHARACTER_COLOR, assets.W_Knight},
      {ROOK, W_CHARACTER_COLOR, assets.W_Rook}};

  while (!WindowShouldClose()) {
    w = GetRenderWidth(), h = GetRenderHeight();
    Update(w, h, (Character *)&board);
    Draw(w, h, (Character *)&board);
  }

  UnloadAssets(&assets);

  CloseWindow();
  return 0;
}
