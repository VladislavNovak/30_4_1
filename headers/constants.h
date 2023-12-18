#ifndef INC_30_4_1_CONSTANTS_H
#define INC_30_4_1_CONSTANTS_H

#include <iostream>
#include <vector>

using std::string;
using std::vector;

const string ROOT = R"(https://httpbin.org)";

enum class RequestType {
    GET, POST, PUT, PATCH, REMOVE, EXIT
};

const vector<string> requestTypeTitles{"get", "post", "put", "patch", "delete", "exit"};

#endif //INC_30_4_1_CONSTANTS_H
