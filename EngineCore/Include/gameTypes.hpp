#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <unordered_map>
#include <variant>
#include <memory>
#include "export.hpp"

struct Prompt;

enum VariableType
{
  BOOL,
  INT,
  FLOAT,
  STRING,

};

struct Variable
{
  VariableType type;
  std::variant<int, bool, float, std::string> value;
};

enum ConditionCheck
{
  EQUALS,
  GREATER,
  LESS,
  GREATER_OR_EQUAL,
  LESS_OR_EQUAL,
  NOT_EQUAL
};

struct Condition
{
  bool global;
  std::string target = "";
  ConditionCheck check;
  Variable value;
};

struct Action
{
  // optional condition for the action to happen
  bool hasCondition = false;
  Condition condition;

  // whether or not target is a global variable
  bool global;
  std::string target = "";
  Variable value;

  // optional scene change
  std::string sceneChange = "";

  std::shared_ptr<Prompt> inlinePrompt;
};

struct Choice
{
  std::string text;
  std::vector<Action> actions;
};

struct Prompt
{
  std::string promptText;
  std::vector<Choice> choices;
};

struct Scene
{
  std::string name;
  std::unordered_map<std::string, Variable> locals;
  Prompt prompt;
};

struct Game
{
  std::string currentScene;
  std::unordered_map<std::string, Scene> scenes;
  std::unordered_map<std::string, Variable> globals;
};