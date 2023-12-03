#include <algorithm>
#include <malloc.h>
#include <raylib.h>
#include <vector>

#define DEFAULT_WINDOW_WIDTH 720
#define DEFAULT_WINDOW_HEIGHT 720
#define TARGET_FPS 60
// WARNING: BOARD_SIZE_AXIS acts more like a const rather than config field.
// Changing it will require modifications in codebase.
#define BOARD_SIZE_AXIS 8
// NOTE: Character = Chess piece
#define CHARACTER_SCALE_FACTOR 3
#define HINT_RADIUS 25 // NOTE: Hint circle radius
#define H1_FONT_SIZE 300
#define WINDOW_FLAGS FLAG_WINDOW_RESIZABLE // NOTE: separated by '|'
#define TEXT_OFFSET_SCALE                                                      \
  10 // NOTE: for maximized text scale of offset off the window borders

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
#define TOP_SIDE_COLOR                                                         \
  B_CHARACTER_COLOR // are B or W at the top side of the board?

#define EXIT_KEY KEY_NULL
#define RESTART_KEY KEY_ESCAPE

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

  Sound move;
} Assets;

typedef enum {
  BISHOP,
  KING,
  KNIGHT,
  PAWN,
  QUEEN,
  ROOK,
  EMPTY,
} CharacterType;

CharacterType PAWN_UPGRADE = QUEEN;

typedef struct {
  CharacterType type;
  Color color;
  Texture2D texture;
} Character;

typedef struct {
  int w, h;
  Assets assets;
  std::vector<Character> board;

  bool hinting;
  int hintingTile;
  std::vector<int> hintingMoves;

  Color turn;
  Color winner;
  Color top;
} Chess;

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

  assets->move = LoadSound(MOVE_SOUND_PATH);
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

  UnloadSound(assets->move);
}

bool ColorsEqual(Color a, Color b) {
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

void DefaultBoard(Chess *chess) {
  Assets *assets = &chess->assets;
  Color other = ColorsEqual(chess->top, W_CHARACTER_COLOR) ? B_CHARACTER_COLOR
                                                           : W_CHARACTER_COLOR;
  chess->board = {{ROOK, chess->top, assets->B_Rook},
                  {KNIGHT, chess->top, assets->B_Knight},
                  {BISHOP, chess->top, assets->B_Bishop},
                  {KING, chess->top, assets->B_King},
                  {QUEEN, chess->top, assets->B_Queen},
                  {BISHOP, chess->top, assets->B_Bishop},
                  {KNIGHT, chess->top, assets->B_Knight},
                  {ROOK, chess->top, assets->B_Rook},

                  {PAWN, chess->top, assets->B_Pawn},
                  {PAWN, chess->top, assets->B_Pawn},
                  {PAWN, chess->top, assets->B_Pawn},
                  {PAWN, chess->top, assets->B_Pawn},
                  {PAWN, chess->top, assets->B_Pawn},
                  {PAWN, chess->top, assets->B_Pawn},
                  {PAWN, chess->top, assets->B_Pawn},
                  {PAWN, chess->top, assets->B_Pawn},

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

                  {PAWN, other, assets->W_Pawn},
                  {PAWN, other, assets->W_Pawn},
                  {PAWN, other, assets->W_Pawn},
                  {PAWN, other, assets->W_Pawn},
                  {PAWN, other, assets->W_Pawn},
                  {PAWN, other, assets->W_Pawn},
                  {PAWN, other, assets->W_Pawn},
                  {PAWN, other, assets->W_Pawn},

                  {ROOK, other, assets->W_Rook},
                  {KNIGHT, other, assets->W_Knight},
                  {BISHOP, other, assets->W_Bishop},
                  {QUEEN, other, assets->W_Queen},
                  {KING, other, assets->W_King},
                  {BISHOP, other, assets->W_Bishop},
                  {KNIGHT, other, assets->W_Knight},
                  {ROOK, other, assets->W_Rook}};
}

bool MouseHovered(Rectangle rect, Vector2 mousePos) {
  return mousePos.x >= rect.x && mousePos.x <= rect.x + rect.width &&
         mousePos.y >= rect.y && mousePos.y <= rect.y + rect.height;
}

bool TileInHintingMoves(std::vector<int> hintingMoves, int tile) {
  return std::find(hintingMoves.begin(), hintingMoves.end(), tile) !=
         hintingMoves.end();
}

int AssertTextFitsInViewport(const char *text, int *fontSize, int w, int h) {
  int textW = MeasureText(text, *fontSize);
  while (textW > w - textW / TEXT_OFFSET_SCALE ||
         *fontSize > h - *fontSize / TEXT_OFFSET_SCALE) {
    *fontSize = *fontSize - 1;
    textW = MeasureText(text, *fontSize);
  }
  return textW;
}

void WalkVertically(std::vector<Character> *board, int tile,
                    std::vector<int> *out) {
  int tileY = tile / (BOARD_SIZE_AXIS - 1);
  int tileX = tile - tileY;

  // up
  for (int y = tileY - 1; y >= 0; --y) {
    int index = y * BOARD_SIZE_AXIS + tileX;
    Character character = (*board)[index];

    if (character.type == EMPTY) {
      out->push_back(y * BOARD_SIZE_AXIS + tileX);
    } else if (!ColorsEqual(character.color, (*board)[tile].color)) {
      out->push_back(y * BOARD_SIZE_AXIS + tileX);
      break;
    } else {
      break;
    }
  }
  // down
  for (int y = tileY + 1; y <= BOARD_SIZE_AXIS; ++y) {
    int index = y * BOARD_SIZE_AXIS + tileX;
    Character character = (*board)[index];

    if (character.type == EMPTY) {
      out->push_back(y * BOARD_SIZE_AXIS + tileX);
    } else if (!ColorsEqual(character.color, (*board)[tile].color)) {
      out->push_back(y * BOARD_SIZE_AXIS + tileX);
      break;
    } else {
      break;
    }
  }
}

void WalkHorizontally(std::vector<Character> *board, int tile,
                      std::vector<int> *out) {
  int tileY = tile / (BOARD_SIZE_AXIS - 1);
  int tileX = tile - tileY;

  // left
  for (int x = tileX - 1; x >= 0; --x) {
    int index = tileY * BOARD_SIZE_AXIS + x;
    Character character = (*board)[index];

    if (character.type == EMPTY) {
      out->push_back(tileY * BOARD_SIZE_AXIS + x);
    } else if (!ColorsEqual(character.color, (*board)[tile].color)) {
      out->push_back(tileY * BOARD_SIZE_AXIS + x);
      break;
    } else {
      break;
    }
  }
  // right
  for (int x = tileX + 1; x <= BOARD_SIZE_AXIS; ++x) {
    int index = tileY * BOARD_SIZE_AXIS + x;
    Character character = (*board)[index];

    if (character.type == EMPTY) {
      out->push_back(tileY * BOARD_SIZE_AXIS + x);
    } else if (!ColorsEqual(character.color, (*board)[tile].color)) {
      out->push_back(tileY * BOARD_SIZE_AXIS + x);
      break;
    } else {
      break;
    }
  }
}

void WalkDiagonally(std::vector<Character> *board, int tile,
                    std::vector<int> *out) {
  int tileY = tile / (BOARD_SIZE_AXIS - 1);
  int tileX = tile - tileY;

  // TODO: implement WalkDiagonally
  // top left
  // top right
  // bottom left
  // bottom right
}

bool IsNeighbourValid(std::vector<Character> *board, int tile, int index) {
  return (*board)[index].type == EMPTY ||
         !ColorsEqual((*board)[index].color, (*board)[tile].color);
}

void Neighbours(std::vector<Character> *board, int tile,
                std::vector<int> *out) {
  int tileY = tile / (BOARD_SIZE_AXIS - 1);
  int tileX = tile - tileY;

  int previousY = tileY - 1;
  int nextY = tileY + 1;

  int indices[] = {
      previousY * BOARD_SIZE_AXIS + (tileX - 1),
      previousY * BOARD_SIZE_AXIS + (tileX),
      previousY * BOARD_SIZE_AXIS + (tileX + 1),
      nextY * BOARD_SIZE_AXIS + (tileX - 1),
      nextY * BOARD_SIZE_AXIS + (tileX),
      nextY * BOARD_SIZE_AXIS + (tileX + 1),
      tileY * BOARD_SIZE_AXIS + (tileX - 1),
      tileY * BOARD_SIZE_AXIS + (tileX + 1),
  };

  for (int i = 0; i < sizeof(indices) / sizeof(int); ++i) {
    if (IsNeighbourValid(board, tile, indices[i])) {
      out->push_back(indices[i]);
    }
  }
}

void L(std::vector<Character> *board, int tile, std::vector<int> *out) {
  int tileY = tile / (BOARD_SIZE_AXIS - 1);
  int tileX = tile - tileY;

  // TODO: implement L
}

std::vector<int> GetHints(Chess *chess, int tile) {
  std::vector<int> res;
  int tileY = tile / (BOARD_SIZE_AXIS - 1);
  int tileX = tile - tileY;

  switch (chess->board[tile].type) {
  case ROOK: {
    WalkVertically(&chess->board, tile, &res);
    WalkHorizontally(&chess->board, tile, &res);
    break;
  }
  case KNIGHT:
    L(&chess->board, tile, &res);
    break;
  case BISHOP:
    WalkDiagonally(&chess->board, tile, &res);
    break;
  case KING:
    Neighbours(&chess->board, tile, &res);
    break;
  case QUEEN:
    WalkHorizontally(&chess->board, tile, &res);
    WalkVertically(&chess->board, tile, &res);
    WalkDiagonally(&chess->board, tile, &res);
    break;
  case PAWN:
    // TODO:
    // if first time moving pawn can move 2 in front, otherwise 1
    // can kill front or diagonal (1 rank)
    if (ColorsEqual(chess->board[tile].color, W_CHARACTER_COLOR)) {

    } else {
    }
    break;
  case EMPTY:
    return std::vector<int>();
    break;
  }

  return res;
}

void Draw(Chess *chess) {
  BeginDrawing();
  ClearBackground(WINDOW_BG_COLOR);

  if (ColorsEqual(chess->winner, TRANSPARENT)) {
    Color bg = FIRST_TILE_COLOR;
    float tileW = (float)chess->w / BOARD_SIZE_AXIS;
    float tileH = (float)chess->h / BOARD_SIZE_AXIS;

    for (int y = 0; y < BOARD_SIZE_AXIS; ++y) {
      for (int x = 0; x < BOARD_SIZE_AXIS; ++x) {
        float xx = x * tileW, yy = y * tileH;

        // draw tile
        Rectangle tileRec = {xx, yy, tileW, tileH};
        DrawRectangleRec(tileRec, bg);
        bg = ColorToInt(bg) == ColorToInt(LIGHT_COLOR) ? DARK_COLOR
                                                       : LIGHT_COLOR;

        int index = y * BOARD_SIZE_AXIS + x;
        Character character = chess->board[index];

        // draw characters
        if (character.type != EMPTY) {
          Vector2 characterVec = {
              xx + ((float)tileW / 2) -
                  ((float)character.texture.width * CHARACTER_SCALE_FACTOR / 2),
              yy + (float)tileH / 2 -
                  (float)character.texture.height *
                      (CHARACTER_SCALE_FACTOR + 1) / 2};

          // change color based on what tile we re drawing: enemy/friendly/hint
          Color color = character.color;
          if (chess->hinting) {
            if (index == chess->hintingTile) {
              color = HINT_COLOR;
            } else if (TileInHintingMoves(chess->hintingMoves, index) &&
                       !ColorsEqual(chess->board[chess->hintingTile].color,
                                    chess->board[index].color)) {
              color = ENEMY_COLOR;
            }
          }

          DrawTextureEx(character.texture, characterVec, 0,
                        CHARACTER_SCALE_FACTOR, color);
        } else {
          // draw hint
          if (chess->hinting) {
            if (TileInHintingMoves(chess->hintingMoves, index)) {
              DrawCircle(xx + tileW / 2, yy + tileH / 2, HINT_RADIUS,
                         HINT_COLOR);
            }
          }
        }
      }
      bg = ColorToInt(bg) == ColorToInt(LIGHT_COLOR) ? DARK_COLOR : LIGHT_COLOR;
    }
  } else {
    const char *text = ColorsEqual(chess->winner, W_CHARACTER_COLOR)
                           ? "Winner: WHITE"
                           : "Winner: BLACK";
    int fontSize = H1_FONT_SIZE;
    int textW = AssertTextFitsInViewport(text, &fontSize, chess->w, chess->h);
    DrawText(text, chess->w / 2 - textW / 2, chess->h / 2 - fontSize / 2,
             fontSize, chess->winner);
  }

  EndDrawing();
}

void ResetGame(Chess *chess) {
  DefaultBoard(chess);
  chess->winner = TRANSPARENT;
  chess->turn = STARTING_COLOR;
  chess->hinting = false;
  chess->hintingTile = -1;
  chess->hintingMoves = std::vector<int>();
}

void Update(Chess *chess) {
  float tileW = (float)chess->w / BOARD_SIZE_AXIS,
        tileH = (float)chess->h / BOARD_SIZE_AXIS;
  int clickedTile = -1;

  if (!ColorsEqual(chess->winner, TRANSPARENT)) {
    if (IsKeyPressed(RESTART_KEY)) {
      ResetGame(chess);
    }
  } else {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 mousePos = GetMousePosition();

      for (int y = 0; y < BOARD_SIZE_AXIS; ++y) {
        for (int x = 0; x < BOARD_SIZE_AXIS; ++x) {
          Rectangle tileRect = {x * tileW, y * tileH, tileW, tileH};
          if (MouseHovered(tileRect, mousePos)) {
            clickedTile = y * BOARD_SIZE_AXIS + x;
          }
        }
      }

      if (!chess->hinting) {
        // if click on a character of player turn's team: show new hints
        if (ColorsEqual(chess->board[clickedTile].color, chess->turn)) {
          chess->hinting = true;
          chess->hintingTile = clickedTile;
          chess->hintingMoves = GetHints(chess, clickedTile);
        }
      } else {
        if (clickedTile == chess->hintingTile) {
          // clicked same character
          chess->hinting = false;
        } else if (ColorsEqual(chess->board[clickedTile].color, chess->turn)) {
          //   click on other character of the same team
          chess->hintingTile = clickedTile;
          chess->hintingMoves = GetHints(chess, clickedTile);
        } else if (TileInHintingMoves(chess->hintingMoves, clickedTile)) {
          // execute hint
          if (chess->board[clickedTile].type == KING) {
            chess->winner = chess->turn;
            return;
          }
          chess->hinting = false;
          chess->board[clickedTile] = chess->board[chess->hintingTile];
          PlaySound(chess->assets.move);
          chess->board[chess->hintingTile] = {EMPTY};
          chess->turn = ColorsEqual(chess->turn, B_CHARACTER_COLOR)
                            ? W_CHARACTER_COLOR
                            : B_CHARACTER_COLOR;
          if (chess->board[clickedTile].type == PAWN) {
            if (ColorsEqual(chess->board[clickedTile].color,
                            W_CHARACTER_COLOR) &&
                clickedTile < BOARD_SIZE_AXIS) {
              chess->board[clickedTile].type = PAWN_UPGRADE;
            } else if (ColorsEqual(chess->board[clickedTile].color,
                                   B_CHARACTER_COLOR) &&
                       clickedTile >= (BOARD_SIZE_AXIS - 1) * BOARD_SIZE_AXIS) {
              chess->board[clickedTile].type = PAWN_UPGRADE;
            }
          }
        } else {
          // clicked on enemy team or empty
          chess->hinting = false;
        }
      }
    }
  }
}

int main() {
  Chess chess = {0};
  chess.w = DEFAULT_WINDOW_WIDTH, chess.h = DEFAULT_WINDOW_HEIGHT;
  chess.turn = STARTING_COLOR, chess.winner = TRANSPARENT,
  chess.top = TOP_SIDE_COLOR;
  chess.hinting = false, chess.hintingTile = -1;

  SetConfigFlags(WINDOW_FLAGS);
  InitWindow(chess.w, chess.h, WINDOW_TITLE);
  SetTargetFPS(TARGET_FPS);
  InitAudioDevice();
  SetExitKey(EXIT_KEY);

  chess.assets = (Assets){0};
  LoadAssets(&chess.assets);

  DefaultBoard(&chess);

  while (!WindowShouldClose()) {
    chess.w = GetRenderWidth(), chess.h = GetRenderHeight();
    Update(&chess);
    Draw(&chess);
  }

  UnloadAssets(&chess.assets);

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
