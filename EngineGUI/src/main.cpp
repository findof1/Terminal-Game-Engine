#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <unordered_map>
#include <variant>
#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_node_editor.h"
#include "imgui_stdlib.h"
#include <GLFW/glfw3.h>

#include "gameTypes.hpp"
#include "utils.hpp"
#include "game.hpp"
#include "print.hpp"
#include "serialization.hpp"

#include "editorTypes.hpp"
#include "editorUtils.hpp"
#include "editorNodeSpawner.hpp"
#include "parser.hpp"

void DrawSceneData(Node &node, SceneData &data, int drawIndex, int *uniqueId)
{
  ImGui::PushItemWidth(200);
  ImGui::InputText(("Name##" + std::to_string(drawIndex)).c_str(), &data.name);
  node.name = data.name;
  ImGui::PopItemWidth();

  ImGui::PushItemWidth(200);
  ImGui::InputText(("Prompt##" + std::to_string(drawIndex)).c_str(), &data.promptText);
  ImGui::PopItemWidth();

  ImGui::Checkbox(("Starting room?##" + std::to_string(drawIndex)).c_str(), &data.startingScene);

  if (ImGui::Button(("Add Choice##" + std::to_string(drawIndex)).c_str()))
  {
    addChoiceToSceneNode(&node, uniqueId, "Choice");
  }

  if (ImGui::Button(("Remove Choice##" + std::to_string(drawIndex)).c_str()))
  {
    removeChoiceFromSceneNode(&node);
  }
}

void DrawPromptData(Node &node, PromptData &data, int drawIndex, int *uniqueId)
{
  ImGui::PushItemWidth(200);
  ImGui::InputText(("Prompt##" + std::to_string(drawIndex)).c_str(), &data.promptText);
  ImGui::PopItemWidth();

  if (ImGui::Button(("Add Choice##" + std::to_string(drawIndex)).c_str()))
  {
    addChoiceToSceneNode(&node, uniqueId, "Choice");
  }

  if (ImGui::Button(("Remove Choice##" + std::to_string(drawIndex)).c_str()))
  {
    removeChoiceFromSceneNode(&node);
  }
}

void DrawChoiceData(Node &node, ChoiceData &data, int drawIndex, int *uniqueId)
{
  ImGui::PushItemWidth(200);
  ImGui::InputText(("Choice Text##" + std::to_string(drawIndex)).c_str(), &data.choiceText);
  node.name = data.choiceText;
  ImGui::PopItemWidth();

  if (ImGui::Button(("Add Action##" + std::to_string(drawIndex)).c_str()))
  {
    addActionToChoiceNode(&node, uniqueId, "Action");
  }

  if (node.outputs.size() > 1)
  {
    if (ImGui::Button(("Remove Action##" + std::to_string(drawIndex)).c_str()))
    {
      removeActionFromChoiceNode(&node);
    }
  }
}

void DrawActionData(Node &node, ActionData &data, int drawIndex, int *uniqueID)
{
  static std::unordered_map<int, ActionData> prevDataMap;
  auto &prevData = prevDataMap[drawIndex];

  ImGui::Checkbox(("Has Condition##" + std::to_string(drawIndex)).c_str(), &data.hasCondition);
  ImGui::Checkbox(("Sets Variable##" + std::to_string(drawIndex)).c_str(), &data.setsVariable);
  ImGui::Checkbox(("Scene Change##" + std::to_string(drawIndex)).c_str(), &data.hasSceneChange);
  ImGui::Checkbox(("Recursive Prompt##" + std::to_string(drawIndex)).c_str(), &data.hasRecursivePrompt);

  if (data.hasCondition && !prevData.hasCondition)
  {
    node.inputs.emplace_back((*uniqueID)++, "Condition", PinType::Condition, PinKind::Input);
  }
  else if (!data.hasCondition && prevData.hasCondition)
  {
    node.inputs.erase(
        std::remove_if(node.inputs.begin(), node.inputs.end(), [](const Pin &p)
                       { return p.name == "Condition"; }),
        node.inputs.end());
  }

  if (data.setsVariable && !prevData.setsVariable)
  {
    node.inputs.emplace_back((*uniqueID)++, "Target", PinType::Variable, PinKind::Input);
    node.inputs.emplace_back((*uniqueID)++, "Value", PinType::Variable, PinKind::Input);
  }
  else if (!data.setsVariable && prevData.setsVariable)
  {
    node.inputs.erase(
        std::remove_if(node.inputs.begin(), node.inputs.end(), [](const Pin &p)
                       { return p.name == "Target" || p.name == "Value"; }),
        node.inputs.end());
  }

  if (data.hasSceneChange && !prevData.hasSceneChange)
  {
    node.inputs.emplace_back((*uniqueID)++, "Scene Change", PinType::Flow, PinKind::Input);
  }
  else if (!data.hasSceneChange && prevData.hasSceneChange)
  {
    node.inputs.erase(
        std::remove_if(node.inputs.begin(), node.inputs.end(), [](const Pin &p)
                       { return p.name == "Scene Change"; }),
        node.inputs.end());
  }

  if (data.hasRecursivePrompt && !prevData.hasRecursivePrompt)
  {
    node.outputs.emplace_back((*uniqueID)++, "Recursive Prompt", PinType::Flow, PinKind::Input);
  }
  else if (!data.hasRecursivePrompt && prevData.hasRecursivePrompt)
  {
    node.outputs.erase(
        std::remove_if(node.outputs.begin(), node.outputs.end(), [](const Pin &p)
                       { return p.name == "Recursive Prompt"; }),
        node.outputs.end());
  }

  prevDataMap[drawIndex] = data;
}

void DrawConditionData(Node &node, ConditionData &data, int drawIndex)
{
  ImGui::PushItemWidth(200);
  const char *options[] = {"Equals", "Greater", "Less", "GreaterOrEqual", "LessOrEqual", "NotEqual"};
  int current = static_cast<int>(data.check);
  if (ImGui::Combo(("Check##" + std::to_string(drawIndex)).c_str(), &current, options, IM_ARRAYSIZE(options)))
    data.check = static_cast<ConditionCheck>(current);
  ImGui::PopItemWidth();
}

void DrawVariableData(Node &node, VariableData &data, int drawIndex)
{
  ImGui::Checkbox(("Global##" + std::to_string(drawIndex)).c_str(), &data.global);

  ImGui::PushItemWidth(200);
  const char *types[] = {"Bool", "Int", "Float", "String"};
  int current = data.type;
  if (ImGui::Combo(("Type##" + std::to_string(drawIndex)).c_str(), &current, types, IM_ARRAYSIZE(types)))
  {
    VariableType newType = static_cast<VariableType>(current);
    bool switchingTypes = newType != data.type;
    data.type = newType;

    if (switchingTypes)
    {
      if (newType == VariableType::BOOL)
      {
        data.value = false;
      }
      else if (newType == VariableType::FLOAT)
      {
        data.value = 0.0f;
      }
      else if (newType == VariableType::INT)
      {
        data.value = 0;
      }
      else if (newType == VariableType::STRING)
      {
        data.value = std::string();
      }
    }
  }
  ImGui::PopItemWidth();

  ImGui::PushItemWidth(200);
  ImGui::InputText(("Var Name##" + std::to_string(drawIndex)).c_str(), &data.varName);
  ImGui::PopItemWidth();
  node.name = data.varName;

  if (data.type == VariableType::STRING)
  {
    ImGui::PushItemWidth(200);
    ImGui::InputText(("Value##" + std::to_string(drawIndex)).c_str(), &std::get<std::string>(data.value));
    ImGui::PopItemWidth();
  }
  else if (data.type == VariableType::BOOL)
  {
    ImGui::PushItemWidth(200);
    ImGui::Checkbox(("Value##" + std::to_string(drawIndex)).c_str(), &std::get<bool>(data.value));
    ImGui::PopItemWidth();
  }
  else if (data.type == VariableType::INT)
  {
    ImGui::PushItemWidth(200);
    ImGui::InputInt(("Value##" + std::to_string(drawIndex)).c_str(), &std::get<int>(data.value));
    ImGui::PopItemWidth();
  }
  else if (data.type == VariableType::FLOAT)
  {
    ImGui::PushItemWidth(200);
    ImGui::InputFloat(("Value##" + std::to_string(drawIndex)).c_str(), &std::get<float>(data.value));
    ImGui::PopItemWidth();
  }
}

int main()
{
  int uniqueId = 1;

  glfwInit();
  GLFWwindow *window = glfwCreateWindow(1280, 720, "Node Editor", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoScrollWithMouse |
      ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoBringToFrontOnFocus;

  ed::EditorContext *context = ed::CreateEditor();

  bool firstFrame = true;
  std::vector<Link> links;
  std::vector<Node> nodes;
  int nextLinkId = 1;
  int selectedNode = -1;

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    const auto windowBorderSize = ImGui::GetStyle().WindowBorderSize;
    const auto windowRounding = ImGui::GetStyle().WindowRounding;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("Content", nullptr, flags);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, windowBorderSize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, windowRounding);

    ed::SetCurrentEditor(context);
    ed::Begin("Node Editor");

    int index = 0;
    for (auto &node : nodes)
    {
      ed::BeginNode(node.id);
      ImGui::Text("%s", node.name.c_str());
      ImGui::Spacing();

      for (int i = 0; i < std::max(node.inputs.size(), node.outputs.size()); i++)
      {
        bool printInput = i < node.inputs.size();
        bool printOutput = i < node.outputs.size();
        if (printInput)
        {
          auto &input = node.inputs.at(i);
          ed::BeginPin(input.id, ed::PinKind::Input);
          ImGui::Text("%s", input.name.c_str());
          ed::EndPin();
        }

        if (printInput && printOutput)
        {
          ImGui::SameLine();
          ImGui::Spacing();
          ImGui::SameLine();
        }

        if (printOutput)
        {
          auto &output = node.outputs.at(i);
          ed::BeginPin(output.id, ed::PinKind::Input);
          ImGui::Text("%s", output.name.c_str());
          ed::EndPin();
        }
      }

      ed::EndNode();
      index++;
    }

    for (auto &link : links)
    {
      ed::Link(link.id, link.startNode, link.endNode);
    }

    if (ed::BeginCreate())
    {
      ed::PinId inputPinId, outputPinId;
      if (ed::QueryNewLink(&inputPinId, &outputPinId))
      {
        if (inputPinId && outputPinId && canCreateLink(findPin(nodes, inputPinId), findPin(nodes, outputPinId)))
        {
          if (ed::AcceptNewItem())
          {
            links.push_back({ed::LinkId(nextLinkId++), inputPinId, outputPinId});

            ed::Link(links.back().id, links.back().startNode, links.back().endNode);
          }
        }
      }
    }
    ed::EndCreate();

    if (ed::BeginDelete())
    {
      ed::LinkId deletedLinkId;
      while (ed::QueryDeletedLink(&deletedLinkId))
      {
        if (ed::AcceptDeletedItem())
        {
          auto it = std::find_if(links.begin(), links.end(),
                                 [deletedLinkId](const Link &link)
                                 { return link.id == deletedLinkId; });

          if (it != links.end())
            links.erase(it);
        }
      }
    }
    ed::EndDelete();

    ed::NodeId selectedNodeId;
    ed::NodeId selected[1];

    if (ed::GetSelectedNodes(selected, 1) > 0)
    {
      selectedNodeId = selected[0];

      for (size_t i = 0; i < nodes.size(); ++i)
      {
        if (nodes[i].id == selectedNodeId)
        {
          selectedNode = static_cast<int>(i);
          break;
        }
      }
    }
    else
    {
      selectedNodeId = 0;
      selectedNode = -1;
    }

    ed::End();

    ImGui::Begin("Editor Tools");

    if (ImGui::CollapsingHeader("Spawning Nodes", ImGuiTreeNodeFlags_DefaultOpen))
    {

      if (ImGui::Button("Spawn Scene Node"))
      {
        SpawnSceneNode(nodes, &uniqueId);
      }

      if (ImGui::Button("Spawn Prompt Node"))
      {
        SpawnPromptNode(nodes, &uniqueId);
      }

      if (ImGui::Button("Spawn Choice Node"))
      {
        SpawnChoiceNode(nodes, &uniqueId);
      }

      if (ImGui::Button("Spawn Action Node"))
      {
        SpawnActionNode(nodes, &uniqueId);
      }

      if (ImGui::Button("Spawn Condition Node"))
      {
        SpawnConditionNode(nodes, &uniqueId);
      }

      if (ImGui::Button("Spawn Variable Node"))
      {
        SpawnVariableNode(nodes, &uniqueId, "Variable", true);
      }
    }

    if (ImGui::CollapsingHeader("Node Properties", ImGuiTreeNodeFlags_DefaultOpen))
    {
      if (selectedNode >= 0 && selectedNode < nodes.size())
      {
        Node &node = nodes.at(selectedNode);

        if (std::holds_alternative<SceneData>(node.data))
        {
          DrawSceneData(node, std::get<SceneData>(node.data), index, &uniqueId);
        }
        else if (std::holds_alternative<PromptData>(node.data))
        {
          DrawPromptData(node, std::get<PromptData>(node.data), index, &uniqueId);
        }
        else if (std::holds_alternative<ChoiceData>(node.data))
        {
          DrawChoiceData(node, std::get<ChoiceData>(node.data), index, &uniqueId);
        }
        else if (std::holds_alternative<ActionData>(node.data))
        {
          DrawActionData(node, std::get<ActionData>(node.data), index, &uniqueId);
        }
        else if (std::holds_alternative<ConditionData>(node.data))
        {
          DrawConditionData(node, std::get<ConditionData>(node.data), index);
        }
        else if (std::holds_alternative<VariableData>(node.data))
        {
          DrawVariableData(node, std::get<VariableData>(node.data), index);
        }
      }
    }

    ImGui::End();

    if (firstFrame)
    {
      ed::NavigateToContent(0.0f);
    }

    ed::SetCurrentEditor(nullptr);

    ImGui::PopStyleVar(2);
    ImGui::End();
    ImGui::PopStyleVar(2);

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);

    firstFrame = false;
  }

  Game game = convertGame(nodes, links);
  printGame(game);
  std::cin.get();

  std::cout << "Exporting Game" << std::endl;

  {
    std::ofstream out("game.bin", std::ios::binary);
    if (!out)
    {
      std::cout << "Error: failed to open game.bin for writing\n";
    }

    writeGame(out, game);

    if (!out)
    {
      std::cout << "Error: failed while writing game.bin\n";
    }
  }

  std::cout << "Exported Game" << std::endl;
  std::cout << "Press enter to play" << std::endl;
  std::cin.get();

  runGame(game);

  ed::DestroyEditor(context);
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}