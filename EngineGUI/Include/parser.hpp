#pragma once

#include "editorUtils.hpp"

Variable convertVariable(const VariableData &vd);
Condition convertCondition(const Node &condNode, const std::vector<Node> &nodes, const std::vector<Link> &links);
Prompt convertPrompt(const Node &promptNode, const std::vector<Node> &nodes, const std::vector<Link> &links);
Action convertAction(const Node &actionNode, const std::vector<Node> &nodes, const std::vector<Link> &links);
Choice convertChoice(const Node &choiceNode, const std::vector<Node> &nodes, const std::vector<Link> &links);
Scene convertScene(const Node &sceneNode, const std::vector<Node> &nodes, const std::vector<Link> &links);
Game convertGame(const std::vector<Node> &nodes, const std::vector<Link> &links);