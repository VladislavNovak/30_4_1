#include <iostream>
#include <nlohmann/json.hpp>
#include "headers/constants.h"
#include "headers/utilities.h"
#include "headers/cpr_requests.h"

using std::cout;
using std::endl;
using std::string;
using json = nlohmann::json;

int main() {
    // Создать данные для Body можно просто разобрав строку...
    // json jPayload(json::parse(R"({"one": 1, "two": 2, "three": 3})"));
    // а можно сразу создав объект
    json jPayload(json::object({ {"one", 1}, {"two", 2}, {"three", 3} }));

    while(true) {
        std::cout << "--- Main menu ---" << std::endl;
        const int command = selectMenuItem(requestTypeTitles);

        if (command == static_cast<int>(RequestType::EXIT)) {
            std::cout << "Menu/exit mode -->" << std::endl;
            break;
        }

        if (command == static_cast<int>(RequestType::GET)) {
            std::cout << "Menu/get request -->" << std::endl;
            asyncGetRequest(ROOT);
        }
        else if (command == static_cast<int>(RequestType::POST) ||
                   command == static_cast<int>(RequestType::PUT) ||
                   command == static_cast<int>(RequestType::PATCH)) {
            std::cout << "Menu/" << requestTypeTitles[command] << " request -->" << std::endl;
            asyncRequest(static_cast<RequestType>(command), ROOT, jPayload);
        }
        else if (command == static_cast<int>(RequestType::REMOVE)) {
            std::cout << "Menu/delete request -->" << std::endl;
            asyncDeleteRequest(ROOT);
        }
    }

    cout << "THE END!" << endl;

    return 0;
}
