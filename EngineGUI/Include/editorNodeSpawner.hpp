#pragma once
#include <vector>
#include <string>
#include "editorTypes.hpp"

size_t SpawnSceneNode(std::vector<Node> &nodes, int *uniqueID, bool startingScene = false);
void addChoiceToSceneNode(Node *node, int *uniqueID, std::string name); // also works for prompt node
void removeChoiceFromSceneNode(Node *node);                             // also works for prompt node

size_t SpawnPromptNode(std::vector<Node> &nodes, int *uniqueID);

size_t SpawnChoiceNode(std::vector<Node> &nodes, int *uniqueID);
void addActionToChoiceNode(Node *node, int *uniqueID, std::string name);
void removeActionFromChoiceNode(Node *node);

size_t SpawnActionNode(std::vector<Node> &nodes, int *uniqueID);

size_t SpawnConditionNode(std::vector<Node> &nodes, int *uniqueID);

size_t SpawnVariableNode(std::vector<Node> &nodes, int *uniqueID, const std::string &varName, bool global);
void setVarNodeType(Node *node, int type);
