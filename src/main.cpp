#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>

void get_type(std::string command) {
  std::unordered_set<std::string> valid_commands = {
    "echo",
    "exit",
    "type",
  };
  if (valid_commands.contains(command)) {
    std::cout << command << " is a shell builtin" << std::endl;
  } else {
    // List out paths
    std::string paths = std::getenv("PATH");
    std::string path;
    std::istringstream stream(paths);
    while (std::getline(stream, path, ':')) {
      std::filesystem::path full_path = path + "/" + command;
      // std::cout << full_path << std::endl;
      if (std::filesystem::exists(full_path) && access(full_path.c_str(), X_OK) == 0) {
        std::cout << command << " is " << full_path.string() << std::endl;
        return;
      }
    }

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
