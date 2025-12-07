#include "utils.hpp"

ENGINE_API void clearInputBuffer()
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

ENGINE_API void clearScreen()
{
  std::system("cls");
}

ENGINE_API void printPrompt(Prompt &prompt)
{
  std::cout << prompt.promptText << std::endl
            << std::endl;

  for (int i = 0; i < prompt.choices.size(); i++)
  {
    std::cout << std::to_string(i + 1) << ": " << prompt.choices[i].text << std::endl;
  }
}

ENGINE_API void youDied()
{
  clearScreen();
  std::cout << "You died :(" << std::endl
            << std::endl;
  std::cout << "Press enter to exit the game" << std::endl;
  std::cin.get();
  exit(EXIT_SUCCESS);
}

// returns chosen action
ENGINE_API const Choice &doPrompt(Prompt &prompt)
{
  while (true)
  {
    clearScreen();
    printPrompt(prompt);
    std::cout << "\n"
              << "Which Option?" << "\n"
              << "> ";

    int option;
    if (!(std::cin >> option))
    {
      std::cout << "Invalid input!" << std::endl;
      clearInputBuffer();
      continue;
    }

    if (option < 1 || option > prompt.choices.size())
    {
      std::cout << "Invalid input!" << std::endl;
      clearInputBuffer();
      continue;
    }
    clearInputBuffer();

    return prompt.choices[option - 1];
  }
}