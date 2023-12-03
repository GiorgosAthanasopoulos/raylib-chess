#include <malloc.h>
#include <raylib.h>
#include <vector>

#include "config.h"
#include "types.h"
#include "utils.h"

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
