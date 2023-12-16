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

enum class RequestType { GET, POST, PUT, PATCH, REMOVE, EXIT };
vector<string> requestTypeTitles{"get", "post", "put", "patch", "delete", "exit"};

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

void asyncGetRequest(const string &ROOT) {
    string METHOD = R"(/get)";
    cpr::Url URL{ROOT + METHOD};

    // Позволяет программе дождаться выполнения запроса
    bool isRun = true;

    // Используем функцию обратного вызова.
    auto callback{[&isRun](const cpr::Response &r) {
        if (r.status_code == 200) {
            cout << "Request completed in " << r.elapsed << endl;

            // Посмотрим заголовки
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
    cout << "Parallel calculate: " << (5 + 5) << endl;

    while (isRun) {}
}

// Объединяет Post, Put, Patch
void asyncRequest(RequestType type, const string &ROOT, const json &payload) {
    // Добавление header с контентом равным application/json позволить получить в ответе json
    cpr::Header Header{{"content-type", "application/json"}};
    cpr::Body Body(payload.dump());

    string METHOD{R"(/)" + requestTypeTitles[static_cast<int>(type)]};
    cout << "Method path: " << METHOD << endl;
    cpr::Url URL{ROOT + METHOD};

    bool isRun = true;

    auto callback{[&isRun](const cpr::Response &r) {
        if (r.status_code == 200) {
            cout << "Request completed in " << r.elapsed << endl;
            // Можем распечатать все данные
            // cout << r.text << endl;
            // Разбираем тело ответа. После этого сможем получать доступ к каждому элементу json
            json jDoc(json::parse(r.text));

            try {
                // Можем извлечь конкретное поле и, например, итерироваться по переданному ранее payload
                auto data(jDoc.at("json").get<std::map<string, int>>());
                for (const auto &i : data) {
                    cout << std::setw(10) << std::left << i.first << ": " << i.second << endl;
                }
            }
            catch (json::exception &e) { cout << "Error " << e.what() << endl; }
        }
        else if (r.status_code == 0) { std::cerr << r.error.message << endl; }
        else { cout << "Something wrong with status: " << r.status_code << endl; }

        isRun = false;
    }};

    cout << "Loading..." << endl;
    if (type == RequestType::POST) {
        cpr::PostCallback(callback, URL, Header, Body);
    }
    else if (type == RequestType::PUT) {
        cpr::PutCallback(callback, URL, Header, Body);
    }
    else if (type == RequestType::PATCH) {
        cpr::PatchCallback(callback, URL, Header, Body);
    }

    // Пример асинхронности. Действия выполняются параллельно http запросу
    cout << "Parallel calculate: " << (5 + 5) << endl;

    while (isRun) {}
}

void asyncDeleteRequest(const string &ROOT) {
    string METHOD = R"(/delete)";
    cpr::Url URL{ROOT + METHOD};

    bool isRun = true;

    auto callback{[&isRun](const cpr::Response &r) {
        if (r.status_code == 200) {
            cout << "Request completed in " << r.elapsed << endl;
            cout << "DELETE OK" << endl;
            cout << r.text << endl;
        }
        else if (r.status_code == 0) { std::cerr << r.error.message << endl; }
        else { cout << "Something wrong with status: " << r.status_code << endl; }

        isRun = false;
    }};

    cpr::DeleteCallback(callback, URL);

    while (isRun) {}
}

int main() {
    const string ROOT = R"(https://httpbin.org)";

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
