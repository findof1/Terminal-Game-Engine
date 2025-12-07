#include "game.hpp"

ENGINE_API bool compareVariables(const Variable &a, const Variable &b, ConditionCheck op)
{
  if (a.type != b.type)
    return false;

  switch (a.type)
  {
  case BOOL:
  {
    bool va = std::get<bool>(a.value);
    bool vb = std::get<bool>(b.value);
    switch (op)
    {
    case EQUALS:
      return va == vb;
    case NOT_EQUAL:
      return va != vb;
    default:
      return false;
    }
  }

  case INT:
  {
    int va = std::get<int>(a.value);
    int vb = std::get<int>(b.value);
    switch (op)
    {
    case EQUALS:
      return va == vb;
    case NOT_EQUAL:
      return va != vb;
    case GREATER:
      return va > vb;
    case LESS:
      return va < vb;
    case GREATER_OR_EQUAL:
      return va >= vb;
    case LESS_OR_EQUAL:
      return va <= vb;
    }
    break;
  }

  case FLOAT:
  {
    float va = std::get<float>(a.value);
    float vb = std::get<float>(b.value);
    switch (op)
    {
    case EQUALS:
      return va == vb;
    case NOT_EQUAL:
      return va != vb;
    case GREATER:
      return va > vb;
    case LESS:
      return va < vb;
    case GREATER_OR_EQUAL:
      return va >= vb;
    case LESS_OR_EQUAL:
      return va <= vb;
    }
    break;
  }

  case STRING:
  {
    const std::string &va = std::get<std::string>(a.value);
    const std::string &vb = std::get<std::string>(b.value);
    switch (op)
    {
    case EQUALS:
      return va == vb;
    case NOT_EQUAL:
      return va != vb;
    default:
      return false;
    }
  }
  }

  return false;
}

ENGINE_API bool evaluateCondition(const Action &action, const Game &game, const Scene &current)
{
  bool foundVar = false;
  Variable var;
  if (action.global)
  {
    auto it = game.globals.find(action.condition.target);
    if (it != game.globals.end())
    {
      var = it->second;
      foundVar = true;
    }
  }
  else
  {
    auto it = current.locals.find(action.condition.target);
    if (it != current.locals.end())
    {
      var = it->second;
      foundVar = true;
    }
  }

  if (!foundVar)
  {
    std::cout << "Variable does not exist: "
              << action.condition.target << "\n";
    return false;
  }

  return compareVariables(var, action.condition.value, action.condition.check);
}

ENGINE_API void runPrompt(Game &game, Scene &current, Prompt &prompt)
{
  const Choice &choice = doPrompt(prompt);

  for (const Action &action : choice.actions)
  {

    bool pass = true;

    if (action.hasCondition)
    {
      pass = evaluateCondition(action, game, current);
    }

    if (!pass)
      continue;

    if (!action.target.empty())
    {
      if (action.global)
        game.globals[action.target] = action.value;
      else
        current.locals[action.target] = action.value;
    }

    if (!action.sceneChange.empty())
    {
      game.currentScene = action.sceneChange;
    }

    if (action.inlinePrompt)
    {
      runPrompt(game, current, *action.inlinePrompt);
    }
  }
}

ENGINE_API void runGame(Game &game)
{
  while (true)
  {
    Scene &current = game.scenes[game.currentScene];
    runPrompt(game, current, current.prompt);

    // If the current scene has no choices, game ends
    if (game.scenes[game.currentScene].prompt.choices.empty())
    {
      clearScreen();
      std::cout << game.scenes[game.currentScene].prompt.promptText << "\n\n";
      std::cout << "Press enter to exit.";
      std::cin.get();
      break;
    }
  }
}