<details open>
<summary><span style="color:tomato;font-size:16px">HTTP запросы</span></summary>
<details open>
<summary><span style="color:tomato;font-size:12px">Пользовательские запросы</span></summary>

Реализуйте программу по осуществлению различных типов HTTP-запросов к сервису httpbin.org.

Пользователь взаимодействует с программой с помощью команд: “get”, “post”, “put”, “delete”, “patch”. 

В зависимости от команды к серверу httpbin.org осуществляется запрос того или иного типа. 
Содержимое ответа сервера выводится в стандартный вывод.

Программа завершается, когда пользователь вводит команду “exit”.

</details>

<details open>
<summary><span style="color:tomato;font-size:12px">Взаимодействие с библиотеками cpr и nlohmann</span></summary>

Библиотека [cpr](https://docs.libcpr.org/advanced-usage.html#asynchronous-callbacks) позволяет делать http запросы.

Библиотека [nlohmann](https://json.nlohmann.me/integration/cmake/#supporting-both) упрощает работу с файлами json.

В соответствии с этим, конфигурируем CMake:

```cmake
cmake_minimum_required(VERSION 3.24)
project(30_4_1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_CPR_TESTS OFF)
set(CMAKE_USE_OPENSSL OFF)
# Синхронизирует библиотеки
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(30_4_1 main.cpp)

target_link_libraries(30_4_1 PRIVATE cpr::cpr)
target_link_libraries(30_4_1 PRIVATE nlohmann_json::nlohmann_json)
```

Чтобы подключить cpr и nlohmann, указываем:

```c++
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
```

</details>

---
<details open>

<summary><span style="color:tomato;font-size:12px">Комментарии к задаче</span></summary>

Для упрощения реализации, запросы post, put, patch используют лишь один payload (без пользовательского ввода).

Запросы post, put, patch выполняются одной функцией (`asyncRequest`) с конфигурацией через enum `RequestType`

Запросы get и delete вынесены в отдельные функции, по соображению семантики

</details>

---
<details>

<summary><span style="color:tomato;font-size:12px">Полезные ссылки</span></summary>

<a href="https://www.httpbin.org/" style="margin-left:16px">httpbin сервер</a>

<a href="https://docs.libcpr.org/advanced-usage.html" style="margin-left:16px">Библиотека cpr</a>

<a href="https://habr.com/ru/companies/ruvds/articles/674304/" style="margin-left:16px">Создание HTTP-запросов на C++</a>

<a href="https://www.studyplan.dev/pro-cpp/http" style="margin-left:16px">Using HTTP in Modern C++</a>

<a href="https://www.studyplan.dev/pro-cpp/json" style="margin-left:16px">Using JSON in Modern C++</a>

</details>
</details>