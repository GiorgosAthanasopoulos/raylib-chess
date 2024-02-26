#include <raylib.h>

#include "config.h"
#include "utils.h"

void Draw(Chess *chess) {
  BeginDrawing();
  ClearBackground(WINDOW_BG_COLOR);

  if (HaveWinner(chess->winner)) {
    const char *text = ColorsEqual(chess->winner, W_CHARACTER_COLOR)
                           ? "Winner: WHITE"
                           : "Winner: BLACK";
    int fontSize = H1_FONT_SIZE;
    int textW = AssertTextFitsInViewport(text, &fontSize, chess->w, chess->h);
    DrawText(text, chess->w / 2 - textW / 2, chess->h / 2 - fontSize / 2,
             fontSize, chess->winner);
    EndDrawing();
    return;
  }

  Color bg = FIRST_TILE_COLOR;
  float tileW = (float)chess->w / BOARD_SIZE_AXIS;
  float tileH = (float)chess->h / BOARD_SIZE_AXIS;

  for (int y = 0; y < BOARD_SIZE_AXIS; ++y) {
    for (int x = 0; x < BOARD_SIZE_AXIS; ++x) {
      float xx = x * tileW, yy = y * tileH;

      // draw tile
      Rectangle tileRec = {xx, yy, tileW, tileH};
      DrawRectangleRec(tileRec, bg);
      bg = BgColorSwap(bg);

      int index = CoordinatesToBoardIndex(x, y);
      Character character = chess->board[index];

      // draw characters
      if (IsChessPiece(character)) {
        Vector2 characterVec = {
            xx + ((float)tileW / 2) -
                ((float)character.texture.width * CHARACTER_SCALE_FACTOR / 2),
            yy + (float)tileH / 2 -
                (float)character.texture.height * (CHARACTER_SCALE_FACTOR + 1) /
                    2};

        // change color based on what tile we re drawing: enemy/friendly/hint
        Color color = character.color;
        if (chess->hinting) {
          if (index == chess->hintingTile) {
            color = HINT_COLOR;
          } else if (TileInHintingMoves(chess->hintingMoves, index) &&
                     IsEnemy(chess->board[chess->hintingTile],
                             chess->board[index])) {
            color = ENEMY_COLOR;
          }
        }

        int scale = AssertCharacterFitsInTile(
            character.texture, CHARACTER_SCALE_FACTOR, tileW, tileH);
        DrawTextureEx(character.texture, characterVec, 0, scale, color);
      } else {
        // draw hint
        if (chess->hinting) {
          if (TileInHintingMoves(chess->hintingMoves, index)) {
            int radius = AssertHintCircleFitsInTile(HINT_RADIUS, tileW, tileH);
            DrawCircle(xx + tileW / 2, yy + tileH / 2, radius, HINT_COLOR);
          }
        }
      }
    }
    bg = BgColorSwap(bg);
  }

  EndDrawing();
}

void Update(Chess *chess) {
  chess->w = GetRenderWidth(), chess->h = GetRenderHeight();
  float tileW = (float)chess->w / BOARD_SIZE_AXIS,
        tileH = (float)chess->h / BOARD_SIZE_AXIS;
  int clickedTile = -1;

  if (HaveWinner(chess->winner)) {
    if (IsKeyPressed(RESTART_KEY)) {
      ResetGame(chess);
    }
    return;
  }

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
        CheckIfPawnMaxRanked(&chess->board[clickedTile], clickedTile,
                             &chess->assets);
      } else {
        // clicked on enemy team or empty
        chess->hinting = false;
      }
    }
  }
}

int main() {
  // Init game state
  Chess chess = {0};
  Image favicon = LoadImage(WIN_FAVICON_PATH);

  chess.w = DEFAULT_WINDOW_WIDTH, chess.h = DEFAULT_WINDOW_HEIGHT;
  chess.turn = STARTING_COLOR, chess.winner = NO_WINNER_COLOR,
  chess.hinting = false, chess.hintingTile = -1;

  // Init raylib
  SetConfigFlags(WINDOW_FLAGS);
  InitWindow(chess.w, chess.h, WINDOW_TITLE);
  SetWindowIcon(favicon);
  SetTargetFPS(TARGET_FPS);
  InitAudioDevice();
  SetExitKey(EXIT_KEY);

  // Load assets
  chess.assets = {0};
  LoadAssets(&chess.assets);

  // generating board
  DefaultBoard(&chess);

  // main game loop
  while (!WindowShouldClose()) {
    Update(&chess);
    Draw(&chess);
  }

  // free assets when closing
  UnloadAssets(&chess.assets);
  UnloadImage(favicon);

  // close raylib
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
