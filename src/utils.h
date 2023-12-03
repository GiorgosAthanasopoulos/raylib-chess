#pragma once

#include <algorithm>
#include <raylib.h>
#include <stdio.h>

#include "config.h"
#include "types.h"

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
  Color other = ColorsEqual(TOP_SIDE_COLOR, W_CHARACTER_COLOR)
                    ? B_CHARACTER_COLOR
                    : W_CHARACTER_COLOR;
  chess->board = {{ROOK, TOP_SIDE_COLOR, assets->B_Rook},
                  {KNIGHT, TOP_SIDE_COLOR, assets->B_Knight},
                  {BISHOP, TOP_SIDE_COLOR, assets->B_Bishop},
                  {KING, TOP_SIDE_COLOR, assets->B_King},
                  {QUEEN, TOP_SIDE_COLOR, assets->B_Queen},
                  {BISHOP, TOP_SIDE_COLOR, assets->B_Bishop},
                  {KNIGHT, TOP_SIDE_COLOR, assets->B_Knight},
                  {ROOK, TOP_SIDE_COLOR, assets->B_Rook},

                  {PAWN, TOP_SIDE_COLOR, assets->B_Pawn},
                  {PAWN, TOP_SIDE_COLOR, assets->B_Pawn},
                  {PAWN, TOP_SIDE_COLOR, assets->B_Pawn},
                  {PAWN, TOP_SIDE_COLOR, assets->B_Pawn},
                  {PAWN, TOP_SIDE_COLOR, assets->B_Pawn},
                  {PAWN, TOP_SIDE_COLOR, assets->B_Pawn},
                  {PAWN, TOP_SIDE_COLOR, assets->B_Pawn},
                  {PAWN, TOP_SIDE_COLOR, assets->B_Pawn},

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

int CoordinatesToBoardIndex(int x, int y) { return y * BOARD_SIZE_AXIS + x; }

bool InBounds(int x, int y) {
  return x >= 0 && x < BOARD_SIZE_AXIS && y >= 0 && y < BOARD_SIZE_AXIS;
}

void WalkVerticallyUpwards(std::vector<Character> *board, int tile,
                           std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  for (int y = tileY - 1; y >= 0; --y) {
    int index = CoordinatesToBoardIndex(tileX, y);
    Character character = (*board)[index];

    if (character.type == EMPTY) {
      out->push_back(index);
    } else if (!ColorsEqual(character.color, (*board)[tile].color)) {
      out->push_back(index);
      break;
    } else {
      break;
    }
  }
}

void WalkVerticallyDownwards(std::vector<Character> *board, int tile,
                             std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;
  for (int y = tileY + 1; y < BOARD_SIZE_AXIS; ++y) {
    int index = CoordinatesToBoardIndex(tileX, y);
    Character character = (*board)[index];

    if (character.type == EMPTY) {
      out->push_back(index);
    } else if (!ColorsEqual(character.color, (*board)[tile].color)) {
      out->push_back(index);
      break;
    } else {
      break;
    }
  }
}

void WalkVertically(std::vector<Character> *board, int tile,
                    std::vector<int> *out) {
  WalkVerticallyUpwards(board, tile, out);
  WalkVerticallyDownwards(board, tile, out);
}

void WalkHorizontallyLeftwards(std::vector<Character> *board, int tile,
                               std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  for (int x = tileX - 1; x >= 0; --x) {
    int index = CoordinatesToBoardIndex(x, tileY);
    Character character = (*board)[index];

    if (character.type == EMPTY) {
      out->push_back(index);
    } else if (!ColorsEqual(character.color, (*board)[tile].color)) {
      out->push_back(index);
      break;
    } else {
      break;
    }
  }
}

void WalkHorizontallyRightwards(std::vector<Character> *board, int tile,
                                std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  for (int x = tileX + 1; x < BOARD_SIZE_AXIS; ++x) {
    int index = CoordinatesToBoardIndex(x, tileY);
    Character character = (*board)[index];

    if (character.type == EMPTY) {
      out->push_back(index);
    } else if (!ColorsEqual(character.color, (*board)[tile].color)) {
      out->push_back(index);
      break;
    } else {
      break;
    }
  }
}

void WalkHorizontally(std::vector<Character> *board, int tile,
                      std::vector<int> *out) {
  WalkHorizontallyLeftwards(board, tile, out);
  WalkHorizontallyRightwards(board, tile, out);
}

void WalkDiagonallyTopLeft(std::vector<Character> *board, int tile,
                           std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  int y = tileY - 1;
  int x = tileX - 1;
  int index = CoordinatesToBoardIndex(x, y);
  Character character = (*board)[index];
  while (character.type == EMPTY && InBounds(x, y)) {
    out->push_back(index);
    y--;
    x--;
    index = CoordinatesToBoardIndex(x, y);
    character = (*board)[index];
  }
  if (character.type != EMPTY &&
      !ColorsEqual((*board)[tile].color, character.color) && InBounds(x, y)) {
    out->push_back(index);
  }
}

void WalkDiagonallyTopRight(std::vector<Character> *board, int tile,
                            std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  int y = tileY - 1;
  int x = tileX + 1;
  int index = CoordinatesToBoardIndex(x, y);
  Character character = (*board)[index];
  while (character.type == EMPTY && InBounds(x, y)) {
    out->push_back(index);
    y--;
    x++;
    index = CoordinatesToBoardIndex(x, y);
    character = (*board)[index];
  }
  if (character.type != EMPTY &&
      !ColorsEqual((*board)[tile].color, character.color) && InBounds(x, y)) {
    out->push_back(index);
  }
}

void WalkDiagonallyBottomLeft(std::vector<Character> *board, int tile,
                              std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  // bottom left
  int y = tileY + 1;
  int x = tileX - 1;
  int index = CoordinatesToBoardIndex(x, y);
  Character character = (*board)[index];
  while (character.type == EMPTY && InBounds(x, y)) {
    out->push_back(index);
    y++;
    x--;
    index = CoordinatesToBoardIndex(x, y);
    character = (*board)[index];
  }
  if (character.type != EMPTY &&
      !ColorsEqual((*board)[tile].color, character.color) && InBounds(x, y)) {
    out->push_back(index);
  }
}

void WalkDiagonallyBottomRight(std::vector<Character> *board, int tile,
                               std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  int y = tileY + 1;
  int x = tileX + 1;
  int index = CoordinatesToBoardIndex(x, y);
  Character character = (*board)[index];
  while (character.type == EMPTY && InBounds(x, y)) {
    out->push_back(index);
    y++;
    x++;
    index = CoordinatesToBoardIndex(x, y);
    character = (*board)[index];
  }
  if (character.type != EMPTY &&
      !ColorsEqual((*board)[tile].color, character.color) && InBounds(x, y)) {
    out->push_back(index);
  }
}

void WalkDiagonally(std::vector<Character> *board, int tile,
                    std::vector<int> *out) {
  WalkDiagonallyTopLeft(board, tile, out);
  WalkDiagonallyTopRight(board, tile, out);
  WalkDiagonallyBottomLeft(board, tile, out);
  WalkDiagonallyBottomRight(board, tile, out);
}

bool IsNeighbourValid(std::vector<Character> *board, int tile, int index) {
  return (*board)[index].type == EMPTY ||
         !ColorsEqual((*board)[index].color, (*board)[tile].color);
}

void Neighbours(std::vector<Character> *board, int tile,
                std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  for (int y = tileY - 1; y <= tileY + 1; ++y) {
    for (int x = tileX - 1; x <= tileX + 1; ++x) {
      int index = CoordinatesToBoardIndex(x, y);
      if (IsNeighbourValid(board, tile, index)) {
        out->push_back(index);
      }
    }
  }
}

void L(std::vector<Character> *board, int tile, std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  int possibleMoves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                             {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

  for (int i = 0; i < 8; i++) {
    int newX = tileX + possibleMoves[i][0];
    int newY = tileY + possibleMoves[i][1];
    int index = CoordinatesToBoardIndex(newX, newY);
    Character character = (*board)[index];

    if (InBounds(newX, newY) &&
        (character.type == EMPTY ||
         !ColorsEqual(character.color, (*board)[tile].color))) {
      out->push_back(index);
    }
  }
}

void Pawn(std::vector<Character> *board, int tile, std::vector<int> *out) {
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

  // isTop check if pawn is from top or bottom spawn
  bool isTop = ColorsEqual(TOP_SIDE_COLOR, (*board)[tile].color);
  // one move forward
  int oneYForward = isTop ? tileY + 1 : tileY - 1;
  // if tileY == spawnY then allow to move 2 y's forward
  int twoYForward = isTop ? tileY + 2 : tileY - 2;
  // check for possible diagonal kills
  int diagOne = isTop ? CoordinatesToBoardIndex(tileX + 1, tileY + 1)
                      : CoordinatesToBoardIndex(tileX - 1, tileY - 1);
  int diagTwo = isTop ? CoordinatesToBoardIndex(tileX - 1, tileY + 1)
                      : CoordinatesToBoardIndex(tileX + 1, tileY - 1);

  out->push_back(CoordinatesToBoardIndex(tileX, oneYForward));
  if (isTop ? tileY == 1 : tileY == BOARD_SIZE_AXIS - 2) {
    if ((*board)[oneYForward * BOARD_SIZE_AXIS + tileX].type == EMPTY) {
      out->push_back(CoordinatesToBoardIndex(tileX, twoYForward));
    }
  }
  if (!ColorsEqual((*board)[tile].color, (*board)[diagOne].color) &&
      (*board)[diagOne].type != EMPTY) {
    out->push_back(diagOne);
  }
  if (!ColorsEqual((*board)[tile].color, (*board)[diagTwo].color) &&
      (*board)[diagTwo].type != EMPTY) {
    out->push_back(diagTwo);
  }
}

std::vector<int> GetHints(Chess *chess, int tile) {
  std::vector<int> res;
  int tileY = tile / BOARD_SIZE_AXIS;
  int tileX = tile % BOARD_SIZE_AXIS;

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
    Pawn(&chess->board, tile, &res);
    break;
  case EMPTY:
    return std::vector<int>();
    break;
  }

  return res;
}

void ResetGame(Chess *chess) {
  DefaultBoard(chess);
  chess->winner = NO_WINNER_COLOR;
  chess->turn = STARTING_COLOR;
  chess->hinting = false;
  chess->hintingTile = -1;
  chess->hintingMoves = std::vector<int>();
}

int AssertCharacterFitsInTile(Texture2D character, int scale, int tileW,
                              int tileH) {
  while (character.width > tileW || character.height > tileH) {
    scale -= 0.5;
  }
  return scale;
}

int AssertHintCircleFitsInTile(int radius, int tileW, int tileH) {
  int size = radius * 2;
  while (size > tileW || size > tileH) {
    radius--;
  }
  return radius;
}

bool HaveWinner(Color color) { return !ColorsEqual(color, NO_WINNER_COLOR); }

Color BgColorSwap(Color bg) {
  return ColorsEqual(bg, LIGHT_COLOR) ? DARK_COLOR : LIGHT_COLOR;
}

bool IsChessPiece(Character character) { return character.type != EMPTY; }

bool IsEnemy(Character a, Character b) {
  return !ColorsEqual(a.color, b.color) && a.type != EMPTY && b.type != EMPTY;
}

void CheckIfPawnMaxRanked(Character *a, int index, Assets *assets) {
  bool isTop = ColorsEqual(a->color, TOP_SIDE_COLOR);
  bool topMaxed = isTop && index > (BOARD_SIZE_AXIS * BOARD_SIZE_AXIS) - BOARD_SIZE_AXIS - 1;
  bool bottomMaxed = !isTop && index < BOARD_SIZE_AXIS;
  if (topMaxed || bottomMaxed) {
    a->type = QUEEN;
    if (ColorsEqual(a->color, W_CHARACTER_COLOR)) {
        a->texture = assets->W_Queen;
    } else {
        a->texture = assets->B_Queen;
    }
  }
}
