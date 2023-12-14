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
<summary><span style="color:tomato;font-size:12px">Взаимодействие с библиотеками</span></summary>

Сначала рассмотрим окончательный вариант CMake (добавим):

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

В рамках тренировки работы с библиотеками, установим несколько.
Во-первых, это [nlohmann](https://json.nlohmann.me/integration/cmake/#supporting-both), 
которая упрощает работу с файлами json.



</details>

<details open>
<summary><span style="color:tomato;font-size:12px">INFO</span></summary>

[//]: # (<a href="" style="margin-left:16px">REF</a>)

</details>
</details>