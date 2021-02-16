#include <iostream>
#include <fstream>
#include <iomanip>
#include <json.hpp>

using namespace std;
using json = nlohmann::json;

int main()
{
    cout << "Json test" << endl;

    // create an empty structure (null)
    json outputjson;
    // add a number that is stored as double (note the implicit conversion of j to an object)
    outputjson["pi"] = 3.141;
    // add a Boolean that is stored as bool
    outputjson["happy"] = true;
    // add a string that is stored as std::string
    outputjson["name"] = "Niels";
    // add another null object by passing nullptr
    outputjson["nothing"] = nullptr;
    // add an object inside the object
    outputjson["answer"]["everything"] = 42;
    // add an array that is stored as std::vector (using an initializer list)
    outputjson["list"] = { 1, 0, 2 };
    // add another object (using an initializer list of pairs)
    outputjson["object"] = { {"currency", "USD"}, {"value", 42.99} };
    cout << "outputjson: " << setw(4) << outputjson << endl;

    string outputfilename = "xdfile.json";
    ofstream outFile(outputfilename);
    outFile << setw(4) << outputjson << endl;

    json j2 = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {
            {"everything", 42}
        }},
        {"list", {1, 0, 2}},
        {"object", {
            {"currency", "USD"},
            {"value", 42.99}
        }}
    };
    cout << "j2: " << j2 << endl;

    return 0;
}