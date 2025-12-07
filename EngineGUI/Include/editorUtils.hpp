#pragma once
#include <vector>
#include "editorTypes.hpp"

const Node *findNodeFromPin(const std::vector<Node> &nodes, ed::PinId pinId);
std::vector<const Link *> findLinksFromPin(const std::vector<Link> &links, ed::PinId pin);
const Node *findLinkedNodeOf(const std::vector<Node> &nodes, const std::vector<Link> &links, ed::PinId pin);
Pin *findPin(std::vector<Node> &nodes, ed::PinId id);
bool isPinLinked(const std::vector<Link> &links, ed::PinId id);
bool canCreateLink(Pin *a, Pin *b);
void buildNode(Node *node);