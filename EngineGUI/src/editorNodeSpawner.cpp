#include "editorNodeSpawner.hpp"
#include "gameTypes.hpp"
#include "editorUtils.hpp"

void addChoiceToSceneNode(Node *node, int *uniqueID, std::string name)
{
  node->outputs.emplace_back((*uniqueID)++, name, PinType::Scene, PinKind::Output);
}

void removeChoiceFromSceneNode(Node *node)
{
  if (!node->outputs.empty())
    node->outputs.pop_back();
}

size_t SpawnSceneNode(std::vector<Node> &nodes, int *uniqueID, bool startingScene)
{
  SceneData sceneData;
  sceneData.name = "Scene";
  sceneData.promptText = "Prompt";
  sceneData.startingScene = startingScene;

  size_t index = nodes.size();
  nodes.emplace_back((*uniqueID)++, sceneData.name, ImColor(128, 195, 248));
  nodes.back().type = NodeType::Simple;

  nodes.back().data = sceneData;

  nodes.back().outputs.emplace_back((*uniqueID)++, "Scene Reference", PinType::Flow, PinKind::Output);

  buildNode(&nodes.back());

  return index;
}

size_t SpawnPromptNode(std::vector<Node> &nodes, int *uniqueID)
{
  PromptData promptData;
  promptData.promptText = "Prompt";

  size_t index = nodes.size();
  nodes.emplace_back((*uniqueID)++, promptData.promptText, ImColor(128, 195, 248));
  nodes.back().type = NodeType::Simple;

  nodes.back().data = promptData;

  nodes.back().outputs.emplace_back((*uniqueID)++, "Prompt Reference", PinType::Flow, PinKind::Output);

  buildNode(&nodes.back());

  return index;
}

void addActionToChoiceNode(Node *node, int *uniqueID, std::string name)
{
  node->outputs.emplace_back((*uniqueID)++, name, PinType::Choice, PinKind::Output);
}

void removeActionFromChoiceNode(Node *node)
{
  if (!node->outputs.empty())
    node->outputs.pop_back();
}

size_t SpawnChoiceNode(std::vector<Node> &nodes, int *uniqueID)
{
  ChoiceData choiceData;
  choiceData.choiceText = "Choice 1";

  size_t index = nodes.size();
  nodes.emplace_back((*uniqueID)++, choiceData.choiceText, ImColor(200, 150, 100));
  nodes.back().type = NodeType::Simple;

  nodes.back().data = choiceData;

  nodes.back().inputs.emplace_back((*uniqueID)++, "Scene", PinType::Scene, PinKind::Input);

  addActionToChoiceNode(&nodes.back(), uniqueID, "Action 1");

  buildNode(&nodes.back());
  return index;
}

size_t SpawnVariableNode(std::vector<Node> &nodes, int *uniqueID, const std::string &varName, bool global)
{
  VariableData variableData;
  variableData.global = global;
  variableData.varName = varName;
  variableData.type = VariableType::INT;
  variableData.value = 0;

  size_t index = nodes.size();
  nodes.emplace_back((*uniqueID)++, varName, ImColor(150, 250, 150));
  nodes.back().type = NodeType::Simple;

  nodes.back().data = variableData;

  nodes.back().outputs.emplace_back((*uniqueID)++, "Value", PinType::Variable, PinKind::Output);

  buildNode(&nodes.back());
  return index;
}

size_t SpawnActionNode(std::vector<Node> &nodes, int *uniqueID)
{
  ActionData actionData;

  size_t index = nodes.size();
  nodes.emplace_back((*uniqueID)++, "Action", ImColor(250, 150, 150));
  nodes.back().type = NodeType::Simple;

  nodes.back().data = actionData;

  nodes.back().inputs.emplace_back((*uniqueID)++, "Choice", PinType::Choice, PinKind::Input);
  buildNode(&nodes.back());
  return index;
}

size_t SpawnConditionNode(std::vector<Node> &nodes, int *uniqueID)
{
  ConditionData conditionData;
  conditionData.check = ConditionCheck::EQUALS;

  size_t index = nodes.size();
  nodes.emplace_back((*uniqueID)++, "Condition", ImColor(250, 150, 150));
  nodes.back().type = NodeType::Simple;

  nodes.back().data = conditionData;

  nodes.back().inputs.emplace_back((*uniqueID)++, "A", PinType::Variable, PinKind::Input);
  nodes.back().inputs.emplace_back((*uniqueID)++, "B", PinType::Variable, PinKind::Input);

  nodes.back().outputs.emplace_back((*uniqueID)++, "Result", PinType::Condition, PinKind::Output);

  buildNode(&nodes.back());
  return index;
}