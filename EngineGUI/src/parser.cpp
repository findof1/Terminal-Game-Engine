#include "parser.hpp"

Variable convertVariable(const VariableData &vd)
{
  Variable v;
  v.type = vd.type;
  v.value = vd.value;
  return v;
}

Condition convertCondition(const Node &condNode, const std::vector<Node> &nodes, const std::vector<Link> &links)
{
  auto &cd = std::get<ConditionData>(condNode.data);

  Condition c;
  c.check = cd.check;

  for (auto &pin : condNode.inputs)
  {
    const Node *linked = findLinkedNodeOf(nodes, links, pin.id);
    if (!linked)
      continue;

    if (std::holds_alternative<VariableData>(linked->data) && pin.name == "A")
    {
      auto &vd = std::get<VariableData>(linked->data);
      c.global = vd.global;
      c.target = vd.varName;
    }

    if (std::holds_alternative<VariableData>(linked->data) && pin.name == "B")
    {
      auto &vd = std::get<VariableData>(linked->data);
      c.value = convertVariable(vd);
    }
  }

  return c;
}

Prompt convertPrompt(const Node &promptNode, const std::vector<Node> &nodes, const std::vector<Link> &links)
{
  auto &pd = std::get<PromptData>(promptNode.data);

  Prompt p;
  p.promptText = pd.promptText;

  for (auto &pin : promptNode.outputs)
  {
    const Node *linked = findLinkedNodeOf(nodes, links, pin.id);
    if (!linked)
      continue;
    if (std::holds_alternative<ChoiceData>(linked->data))
    {
      p.choices.push_back(convertChoice(*linked, nodes, links));
    }
  }

  return p;
}

Action convertAction(const Node &actionNode, const std::vector<Node> &nodes, const std::vector<Link> &links)
{
  auto &ad = std::get<ActionData>(actionNode.data);

  Action a;

  if (ad.hasCondition)
  {
    for (auto &pin : actionNode.inputs)
    {
      const Node *linked = findLinkedNodeOf(nodes, links, pin.id);
      if (!linked)
        continue;
      if (std::holds_alternative<ConditionData>(linked->data))
      {
        a.condition = convertCondition(*linked, nodes, links);
        a.hasCondition = true;
      }
    }
  }

  if (ad.setsVariable)
  {

    for (auto &pin : actionNode.inputs)
    {
      const Node *linked = findLinkedNodeOf(nodes, links, pin.id);
      if (!linked)
        continue;

      if (std::holds_alternative<VariableData>(linked->data) && pin.name == "Target")
      {
        auto &vd = std::get<VariableData>(linked->data);
        a.target = vd.varName;
        a.global = vd.global;
      }

      if (std::holds_alternative<VariableData>(linked->data) && pin.name == "Value")
      {
        auto &vd = std::get<VariableData>(linked->data);
        a.value = convertVariable(vd);
      }
    }
  }

  if (ad.hasSceneChange)
  {
    for (auto &pin : actionNode.inputs)
    {
      const Node *linked = findLinkedNodeOf(nodes, links, pin.id);
      if (!linked)
        continue;

      if (std::holds_alternative<SceneData>(linked->data))
      {
        auto &sd = std::get<SceneData>(linked->data);
        a.sceneChange = sd.name;
      }
    }
  }

  if (ad.hasRecursivePrompt)
  {
    for (auto &pin : actionNode.outputs)
    {
      const Node *linked = findLinkedNodeOf(nodes, links, pin.id);
      if (!linked)
        continue;

      if (std::holds_alternative<PromptData>(linked->data))
      {
        a.inlinePrompt = std::make_shared<Prompt>(convertPrompt(*linked, nodes, links));
      }
    }
  }

  return a;
}

Choice convertChoice(const Node &choiceNode, const std::vector<Node> &nodes, const std::vector<Link> &links)
{
  auto &cd = std::get<ChoiceData>(choiceNode.data);

  Choice c;
  c.text = cd.choiceText;

  for (auto &pin : choiceNode.outputs)
  {
    const Node *linked = findLinkedNodeOf(nodes, links, pin.id);
    if (!linked)
      continue;

    if (std::holds_alternative<ActionData>(linked->data))
    {
      c.actions.push_back(convertAction(*linked, nodes, links));
    }
  }

  return c;
}

Scene convertScene(const Node &sceneNode, const std::vector<Node> &nodes, const std::vector<Link> &links)
{
  auto &sd = std::get<SceneData>(sceneNode.data);

  Scene s;
  s.name = sd.name;
  s.prompt.promptText = sd.promptText;

  for (auto &pin : sceneNode.outputs)
  {
    const Node *linked = findLinkedNodeOf(nodes, links, pin.id);
    if (!linked)
      continue;

    if (std::holds_alternative<VariableData>(linked->data))
    {
      auto &vd = std::get<VariableData>(linked->data);
      if (!vd.global)
        s.locals[vd.varName] = convertVariable(vd);
    }

    if (std::holds_alternative<ChoiceData>(linked->data))
    {
      s.prompt.choices.push_back(convertChoice(*linked, nodes, links));
    }
  }

  return s;
}

Game convertGame(const std::vector<Node> &nodes, const std::vector<Link> &links)
{
  Game g;

  for (auto &n : nodes)
  {
    if (std::holds_alternative<SceneData>(n.data))
    {
      Scene s = convertScene(n, nodes, links);
      if (std::get<SceneData>(n.data).startingScene)
      {
        g.currentScene = s.name;
      }
      g.scenes[s.name] = std::move(s);
    }

    if (std::holds_alternative<VariableData>(n.data))
    {
      auto &vd = std::get<VariableData>(n.data);
      if (vd.global)
        g.globals[vd.varName] = convertVariable(vd);
    }
  }

  return g;
}
