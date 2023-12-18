#include "cpr_requests.h"

void asyncGetRequest(const string &root) {
    string METHOD = R"(/get)";
    cpr::Url URL{root + METHOD};

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
void asyncRequest(RequestType type, const string &root, const json &payload) {
    // Добавление header с контентом равным application/json позволить получить в ответе json
    cpr::Header Header{{"content-type", "application/json"}};
    cpr::Body Body(payload.dump());

    string METHOD{R"(/)" + requestTypeTitles[static_cast<int>(type)]};
    cout << "Method path: " << METHOD << endl;
    cpr::Url URL{root + METHOD};

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

void asyncDeleteRequest(const string &root) {
    string METHOD = R"(/delete)";
    cpr::Url URL{root + METHOD};

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