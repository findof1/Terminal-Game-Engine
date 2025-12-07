#include "editorUtils.hpp"

const Node *findNodeFromPin(const std::vector<Node> &nodes, ed::PinId pinId)
{
  for (auto &node : nodes)
  {
    for (auto &pin : node.inputs)
      if (pin.id == pinId)
        return &node;
    for (auto &pin : node.outputs)
      if (pin.id == pinId)
        return &node;
  }
  return nullptr;
}

std::vector<const Link *> findLinksFromPin(const std::vector<Link> &links, ed::PinId pin)
{
  std::vector<const Link *> out;
  for (auto &l : links)
  {
    if (l.startNode == pin || l.endNode == pin)
      out.push_back(&l);
  }
  return out;
}

const Node *findLinkedNodeOf(const std::vector<Node> &nodes, const std::vector<Link> &links, ed::PinId pin)
{
  for (auto *link : findLinksFromPin(links, pin))
  {
    ed::PinId other = (link->startNode == pin)
                          ? link->endNode
                          : link->startNode;
    return findNodeFromPin(nodes, other);
  }
  return nullptr;
}

Pin *findPin(std::vector<Node> &nodes, ed::PinId id)
{
  if (!id)
    return nullptr;

  for (auto &node : nodes)
  {
    for (auto &pin : node.inputs)
      if (pin.id == id)
        return &pin;

    for (auto &pin : node.outputs)
      if (pin.id == id)
        return &pin;
  }

  return nullptr;
}

bool isPinLinked(const std::vector<Link> &links, ed::PinId id)
{
  if (!id)
    return false;

  for (auto &link : links)
    if (link.startNode == id || link.endNode == id)
      return true;

  return false;
}

bool canCreateLink(Pin *a, Pin *b)
{
  if (!a)
  {
    // std::cout << "Link rejected: first pin missing\n";
    return false;
  }

  if (!b)
  {
    // std::cout << "Link rejected: second pin missing\n";
    return false;
  }

  if (a == b)
  {
    // std::cout << "Link rejected: same pin\n";
    return false;
  }

  if (a->kind == b->kind)
  {
    // std::cout << "Link rejected: both pins are same kind (both input or both output)\n";
    return false;
  }

  if (a->type != b->type)
  {
    // std::cout << "Link rejected: mismatched types\n";
    return false;
  }

  if (a->node == b->node)
  {
    // std::cout << "Link rejected: pins belong to same node\n";
    return false;
  }

  return true;
}

void buildNode(Node *node)
{
  for (auto &input : node->inputs)
  {
    input.node = node;
    input.kind = PinKind::Input;
  }

  for (auto &output : node->outputs)
  {
    output.node = node;
    output.kind = PinKind::Output;
  }
}
