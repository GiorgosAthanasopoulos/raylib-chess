#pragma once

#include <raylib.h>
#include <algorithm>

#include "types.h"
#include "config.h"

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

void ResetGame(Chess *chess) {
  DefaultBoard(chess);
  chess->winner = TRANSPARENT;
  chess->turn = STARTING_COLOR;
  chess->hinting = false;
  chess->hintingTile = -1;
  chess->hintingMoves = std::vector<int>();
}
