#pragma once

#include "gameTypes.hpp"
#include "utils.hpp"
#include "export.hpp"

ENGINE_API bool compareVariables(const Variable &a, const Variable &b, ConditionCheck op);
ENGINE_API bool evaluateCondition(const Action &action, const Game &game, const Scene &current);
ENGINE_API void runGame(Game &game);