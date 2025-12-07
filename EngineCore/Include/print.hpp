#pragma once

#include "gameTypes.hpp"
#include <iostream>

ENGINE_API void printVariable(const Variable &v);
ENGINE_API void printCondition(const Condition &c);
ENGINE_API void printAction(const Action &a);
ENGINE_API void printChoice(const Choice &c);
ENGINE_API void printScene(const Scene &s);
ENGINE_API void printGame(const Game &g);