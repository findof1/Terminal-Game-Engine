#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <unordered_map>
#include <variant>
#include <memory>
#include "gameTypes.hpp"
#include "export.hpp"

ENGINE_API void clearInputBuffer();

ENGINE_API void clearScreen();

ENGINE_API void printPrompt(Prompt &prompt);

ENGINE_API void youDied();

// returns chosen action
ENGINE_API const Choice &doPrompt(Prompt &prompt);