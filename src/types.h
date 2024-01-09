#pragma once

#include <raylib.h>
#include <utility>
#include <vector>

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
} Chess;
