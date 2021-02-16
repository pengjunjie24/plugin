#include <iostream>
#include <fstream>
#include <iomanip>
#include <json.hpp>

using namespace std;
using json = nlohmann::json;

int main()
{
    // a way to express the empty array []
    json empty_array_explicit = json::array();

    // ways to express the empty object {}
    json empty_object_implicit = json({});
    json empty_object_explicit = json::object();

    // a way to express an _array_ of key/value pairs [["currency", "USD"], ["value", 42.99]]
    json array_not_object = { json::array({"currency", "USD"}), json::array({"value", 42.99}) };
    for (auto object : array_not_object)
    {
        cout << object << endl;
    }

    //test_array是一个数组，直接访问下标或者for循环遍历访问
    json test1_array = { "yuan", 12, "ze", 13 };
    for (auto test_value : test1_array)
    {
        cout << test_value << endl;
    }
    cout << "test1_array[0]: " << test1_array[0] << endl;//输出"yuan"

    //test2_array是一个数组里面放了一个数组。认为”yan”,12是数组里面的两个元素
    json test2_array = { {"yuan", 12}, "ze", 13 };
    cout << "test2_array[0][0]: " << test2_array[0][0] << "\n"
        << "test2_array[1]: " << test2_array[1] << endl;//分别输出"yuan"和"ze"

    //test3_array里面含有两个键值对
    json test3_array = {
        {"yan",12},
        {"ze",13}
    };
    cout << "test3_array[\"yan\"]: " << test3_array["yan"] << endl;//输出12

    //test4_array里面含有两个数组，数组里面分别有一个键值对
    json test4_array = {
        {
            {"yan",12}
        },
        {
            {"ze",13}
        }
    };
    cout << "test4_array[0][\"yan\"]: " << test4_array[0]["yan"] << endl;

    return 0;
}