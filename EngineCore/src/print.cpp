#include "print.hpp"

#include "gameTypes.hpp"
#include <iostream>

ENGINE_API void printVariable(const Variable &v)
{
  switch (v.type)
  {
  case VariableType::BOOL:
    std::cout << (std::get<bool>(v.value) ? "true" : "false");
    break;
  case VariableType::INT:
    std::cout << std::get<int>(v.value);
    break;
  case VariableType::FLOAT:
    std::cout << std::get<float>(v.value);
    break;
  case VariableType::STRING:
    std::cout << std::get<std::string>(v.value);
    break;
  }
}

ENGINE_API void printCondition(const Condition &c)
{
  std::cout << "Condition:\n";
  std::cout << "  global: " << (c.global ? "true" : "false") << "\n";
  std::cout << "  target: " << c.target << "\n";
  std::cout << "  check: " << c.check << "\n";
  std::cout << "  value: ";
  printVariable(c.value);
  std::cout << "\n";
}

ENGINE_API void printAction(const Action &a)
{
  std::cout << "    Action:\n";

  if (a.hasCondition)
  {
    printCondition(a.condition);
  }

  std::cout << "    sets variable: " << (a.target.empty() ? "no" : "yes") << "\n";
  if (!a.target.empty())
  {
    std::cout << "      global: " << (a.global ? "true" : "false") << "\n";
    std::cout << "      target: " << a.target << "\n";
    std::cout << "      value: ";
    printVariable(a.value);
    std::cout << "\n";
  }

  if (!a.sceneChange.empty())
  {
    std::cout << "    scene change: " << a.sceneChange << "\n";
  }

  if (a.inlinePrompt)
  {
    std::cout << "    inline prompt: " << a.inlinePrompt->promptText << "\n";
    for (auto c : a.inlinePrompt->choices)
    {
      std::cout << "    ";
      printChoice(c);
    }
  }
}

ENGINE_API void printChoice(const Choice &c)
{
  std::cout << "  Choice: " << c.text << "\n";
  for (auto &a : c.actions)
    printAction(a);
}

ENGINE_API void printScene(const Scene &s)
{
  std::cout << "Scene: " << s.name << "\n";

  if (!s.locals.empty())
  {
    std::cout << "  Locals:\n";
    for (auto &kv : s.locals)
    {
      std::cout << "    " << kv.first << " = ";
      printVariable(kv.second);
      std::cout << "\n";
    }
  }

  std::cout << "  Prompt: " << s.prompt.promptText << "\n";

  for (auto &c : s.prompt.choices)
    printChoice(c);
}

ENGINE_API void printGame(const Game &g)
{
  std::cout << "Current scene: " << g.currentScene << "\n\n";

  if (!g.globals.empty())
  {
    std::cout << "Globals:\n";
    for (auto &kv : g.globals)
    {
      std::cout << "  " << kv.first << " = ";
      printVariable(kv.second);
      std::cout << "\n";
    }
    std::cout << "\n";
  }

  for (auto &kv : g.scenes)
  {
    printScene(kv.second);
    std::cout << "\n";
  }
}
