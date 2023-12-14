#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using json = nlohmann::json;

enum class Menu { EXIT, GET, POST, PUT, REMOVE, PATCH };

int selectMenuItem(const std::vector<std::string> &list, const std::string &msg = "Choose between options") {
    std::cout << msg << " (";
    for (const auto &item : list) { std::cout << item << ((item != list[list.size() - 1]) ? "|" : "):"); }

    while (true) {
        std::string userInput;
        std::getline(std::cin >> std::ws, userInput);

        for (int i = 0; i < list.size(); ++i) { if (list[i] == userInput) return i; }

        std::cout << "Error. Try again:";
    }
}

void getAsyncRequest(const string &ROOT) {
    string METHOD = R"(/get)";
    cpr::Url URL{ROOT + METHOD};

    // Позволяет программе дождаться выполнения запроса
    bool isRun = true;

    // Используем функцию обратного вызова.
    auto callback{[&isRun](const cpr::Response &r) {
        if (r.status_code == 200) {
            cout << "Get-request completed in " << r.elapsed << endl;
            cout << "--- Headers ---" << endl;
            for (const auto &[key, value] : r.header) {
                cout << std::setw(36) << std::left << key << ": " << value << endl;
            }

            cout << "--- Text ---" << endl;
            json jDoc(json::parse(r.text));
            cout << jDoc.dump(2) << endl;

            // Как пример использования вызова через оператор at
            try {
                auto data{jDoc.at("origin").get<string>()};
                cout << "ORIGIN: " << data << endl;
            }
            catch (json::exception &e) { cout << "Trouble" << endl;}
        }
        else if (r.status_code == 0) { std::cerr << r.error.message << endl; }
        else { cout << "Something wrong with status: " << r.status_code << endl; }

        // Ожидание можно завершать
        isRun = false;
    }};

    cout << "Loading..." << endl;
    cpr::GetCallback(callback, URL);

    // Действия выполняются параллельно http запросу
    cout << "Calculate: " << (5 + 5) << endl;

    while (isRun) {}
}

void postRequest(const string &ROOT) {
    string METHOD = R"(/post)";
    cpr::Url URL{ROOT + METHOD};

    cpr::Header Header{{"content-type", "application/json"}};

    // Создать данные для Body можно просто разобрав строку...
    // json jPayload(json::parse(R"({"one": 1, "two": 2, "three": 3})"));
    // а можно сразу создав объект
    json jPayload(json::object({ {"one", 1}, {"two", 2}, {"three", 3} }));
    cpr::Body Body(jPayload.dump());

    // Добавление header с контентом равным application/json позволить получить в ответе json
    cpr::Response r(cpr::Post(URL, Header, Body));

    if (r.status_code == 200) {
        json jDoc(json::parse(r.text));

        // Можем распечатать все данные
        // cout << r.text << endl;

        try {
            // Извлечем то, что передавали в Body(Payload)
            auto data(jDoc.at("json").get<std::map<string, int>>());
            for (const auto &i : data) {
                cout << std::setw(10) << std::left << i.first << ": " << i.second << endl;
            }
        }
        catch (json::exception &e) { cout << "Error " << e.what() << endl; }
    }
    else if (r.status_code == 0) { std::cerr << r.error.message << endl; }
    else { cout << "Something wrong with status: " << r.status_code << endl; }
}

int main() {
    vector<string> menu = { "exit", "get", "post", "put", "delete", "patch" };
    const string ROOT = R"(https://httpbin.org)";





    while(true) {
        std::cout << "--- Main menu ---" << std::endl;
        const int command = selectMenuItem(menu);

        if (command == static_cast<int>(Menu::EXIT)) {
            std::cout << "Menu/exit mode -->" << std::endl;
            break;
        }

        if (command == static_cast<int>(Menu::GET)) {
            std::cout << "Menu/get request -->" << std::endl;
            getAsyncRequest(ROOT);
        }
        else if (command == static_cast<int>(Menu::POST)) {
            std::cout << "Menu/post request -->" << std::endl;
            postRequest(ROOT);
        }
        else if (command == static_cast<int>(Menu::PATCH)) {
            std::cout << "Menu/patch request -->" << std::endl;
        }
        else if (command == static_cast<int>(Menu::PUT)) {
            std::cout << "Menu/put request -->" << std::endl;
        }
        else if (command == static_cast<int>(Menu::REMOVE)) {
            std::cout << "Menu/remove request -->" << std::endl;
        }
    }

    cout << "THE END!" << endl;

    return 0;
}
