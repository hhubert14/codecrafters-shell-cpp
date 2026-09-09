#include <iostream>
#include <string>
#include <unordered_set>

void get_type(std::string command) {
  std::unordered_set<std::string> valid_commands = {
    "echo",
    "exit",
    "type",
  };
  if (valid_commands.contains(command)) {
    std::cout << command << " is a shell builtin" << std::endl;
  } else {
    std::cout << command << ": not found" << std::endl;
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    std::string command;
    std::getline(std::cin, command);

    if (command == "exit") {
      break;
    } else if (command.substr(0, 5) == "echo ") {
      std::cout << command.substr(5) << std::endl;
    } else if (command.substr(0, 5) == "type ") {
      get_type(command.substr(5));
    } else {
      std::cout << command << ": command not found" << std::endl;
    }
  }
}
