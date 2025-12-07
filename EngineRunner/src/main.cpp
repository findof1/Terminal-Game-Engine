#include <iostream>
#include <fstream>
#include "serialization.hpp"
#include "gameTypes.hpp"
#include "game.hpp"

int main()
{
  Game game;

  std::ifstream in("game.bin", std::ios::binary);
  if (!in)
  {
    std::cout << "Failed to open game.bin\n";
    return 1;
  }

  readGame(in, game);

  if (!in)
  {
    std::cout << "Failed to read game data\n";
    return 1;
  }

  runGame(game);
}