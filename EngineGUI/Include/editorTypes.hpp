#pragma once
#include <string>
#include <vector>
#include <variant>
#include "imgui.h"
#include "imgui_node_editor.h"
#include "gameTypes.hpp"
namespace ed = ax::NodeEditor;

enum class PinType
{
  Flow,
  Scene,
  Choice,
  Action,
  Variable,
  Condition
};
enum class PinKind
{
  Output,
  Input
};
enum class NodeType
{
  Simple
};

struct Node;

struct Pin
{
  ed::PinId id;
  Node *node;
  std::string name;
  PinType type;
  PinKind kind;

  Pin(int id, const std::string &name, PinType type, PinKind kind) : id(id), name(name), type(type), kind(kind) {}
};

struct SceneData
{
  std::string name;
  std::string promptText;
  bool startingScene;
};
struct PromptData
{
  std::string promptText;
};
struct ChoiceData
{
  std::string choiceText;
};
struct ActionData
{
  bool hasCondition = false, setsVariable = false, hasSceneChange = false, hasRecursivePrompt = false;
};
struct ConditionData
{
  ConditionCheck check;
};
struct VariableData
{
  bool global;
  VariableType type;
  std::string varName;
  std::variant<int, bool, float, std::string> value;
};

struct Node
{
  ed::NodeId id;
  std::string name;
  std::vector<Pin> inputs;
  std::vector<Pin> outputs;
  ImColor color;
  NodeType type;
  ImVec2 size;

  std::variant<SceneData, PromptData, ChoiceData, ActionData, ConditionData, VariableData> data;

  Node(int id, const std::string &name, ImColor color = ImColor(255, 255, 255)) : id(id), name(name), color(color) {}
};

struct Link
{
  ed::LinkId id;
  ed::PinId startNode;
  ed::PinId endNode;
  ImColor color;

  Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId)
      : id(id), startNode(startPinId), endNode(endPinId) {}
};
