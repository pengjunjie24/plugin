#include "../json_config.h"

#include <json.hpp>
#include <fstream>
#include <iostream>
using namespace std;
using json = nlohmann::json;

int main()
{
    std::string configPath = CMAKE_CURRENT_SOURCE_DIR;
    configPath += "/test04/test_input.json";

    json testjson; // 创建 json 对象
    ifstream jfile(configPath.c_str());

    jfile >> testjson; // 以文件流形式读取 json 文件
    float pi = testjson.at("pi");
    bool happy = testjson.at("happy");

    printf("pi: %f, happy: %d\n", pi, happy);
    return 0;
}