#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>

std::string get_command_path(std::string command) {
  std::string path_var = std::getenv("PATH");
  std::string path_split;
  std::istringstream path_stream(path_var);
  while (std::getline(path_stream, path_split, ':')) {
    std::string full_path = path_split + "/" + command;
    if (access(full_path.c_str(), X_OK) == 0) {
      return full_path;
    }
  }
  return "";
}

void get_type(std::string command) {
  std::unordered_set<std::string> valid_commands = {
    "echo",
    "exit",
    "type",
    "pwd",
  };
  if (valid_commands.contains(command)) {
    std::cout << command << " is a shell builtin" << std::endl;
  } else {
    std::string full_path = get_command_path(command);
    if (full_path != "") {
      std::cout << command << " is " << full_path << std::endl;
    } else {
      std::cout << command << ": not found" << std::endl;
    }
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
    } else if (command.substr(0, 3) == "pwd") {
      std::cout << std::filesystem::current_path().string() << std::endl;
    } else if (command.substr(0, 3) == "cd ") {
      std::string path = command.substr(3);
      if (std::filesystem::is_directory(path)) {
        std::filesystem::current_path(path);
      } else {
        std::cout << "cd: " << path << ": No such file or directory" << std::endl;
      }
    } else {
      if (get_command_path(command.substr(0, command.find(' '))) != "") {
        std::system(command.c_str());
      } else {
        std::cout << command << ": command not found" << std::endl;
      }
    }
  }
}
