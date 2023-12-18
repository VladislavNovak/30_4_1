#ifndef INC_30_4_1_CPR_REQUESTS_H
#define INC_30_4_1_CPR_REQUESTS_H

#include <iostream>
#include <iomanip>
#include <map>
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
#include "constants.h"

using std::cout;
using std::endl;
using std::string;
using json = nlohmann::json;

void asyncGetRequest(const string &ROOT);

// Объединяет Post, Put, Patch
void asyncRequest(RequestType type, const string &ROOT, const json &payload);

void asyncDeleteRequest(const string &ROOT);

#endif //INC_30_4_1_CPR_REQUESTS_H
