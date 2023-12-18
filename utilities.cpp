#include "utilities.h"

int selectMenuItem(const std::vector<std::string> &list, const std::string &msg) {
    std::cout << msg << " (";
    for (const auto &item : list) { std::cout << item << ((item != list[list.size() - 1]) ? "|" : "):"); }

    while (true) {
        std::string userInput;
        std::getline(std::cin >> std::ws, userInput);

        for (int i = 0; i < list.size(); ++i) { if (list[i] == userInput) return i; }

        std::cout << "Error. Try again:";
    }
}

