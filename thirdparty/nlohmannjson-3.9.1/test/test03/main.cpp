#include <iostream>

#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>

#include <json.hpp>

using namespace std;
using json = nlohmann::json;

//与std相适应
void test01()
{
    // create an array using push_back
    json j;
    j.push_back("foo");
    j.push_back(1);
    j.push_back(true);

    // iterate the array
    for (json::iterator it = j.begin(); it != j.end(); ++it)
    {
        std::cout << *it << '\n';
    }

    // range-based for
    for (auto element : j)
    {
        std::cout << element << '\n';
    }

    // getter/setter
    const std::string tmp = j[0];
    j[1] = 42;

    // other stuff
    j.size();     // 3 entries
    j.empty();    // false
    j.type();     // json::value_t::array
    j.clear();    // the array is empty again

    // convenience type checkers
    j.is_null();
    j.is_boolean();
    j.is_number();
    j.is_object();
    j.is_array();
    j.is_string();

    // comparison
    cout << (j == "[\"foo\", 1, true]"_json) << endl;  // true

    // create an object
    json o;
    o["foo"] = 23;
    o["bar"] = false;
    o["baz"] = 3.141;

    // special iterator member functions for objects
    for (json::iterator it = o.begin(); it != o.end(); ++it)
    {
        std::cout << it.key() << " : " << it.value() << "\n";
    }

    // find an entry
    if (o.find("foo") != o.end())
    {
        // there is an entry with key "foo"
        cout << *o.find("foo") << endl;
    }

    // or simpler using count()
    int foo_present = o.count("foo"); // 1
    int fob_present = o.count("fob"); // 0
    cout << foo_present << endl;
    cout << fob_present << endl;
    // delete an entry
    o.erase("foo");
    cout << (o.find("foo") == o.end()) << endl;
}

//把STL与json相转换
void test02()
{
    std::vector<int> c_vector{ 1, 2, 3, 4 };
    json j_vec(c_vector);
    // [1, 2, 3, 4]

    std::deque<double> c_deque{ 1.2, 2.3, 3.4, 5.6 };
    json j_deque(c_deque);
    // [1.2, 2.3, 3.4, 5.6]

    std::list<bool> c_list{ true, true, false, true };
    json j_list(c_list);
    // [true, true, false, true]

    std::forward_list<int64_t> c_flist{ 12345678909876, 23456789098765, 34567890987654, 45678909876543 };
    json j_flist(c_flist);
    // [12345678909876, 23456789098765, 34567890987654, 45678909876543]

    std::array<unsigned long, 4> c_array{ {1, 2, 3, 4} };
    json j_array(c_array);
    // [1, 2, 3, 4]

    std::set<std::string> c_set{ "one", "two", "three", "four", "one" };
    json j_set(c_set); // only one entry for "one" is used
    // ["four", "one", "three", "two"]

    std::unordered_set<std::string> c_uset{ "one", "two", "three", "four", "one" };
    json j_uset(c_uset); // only one entry for "one" is used
    // maybe ["two", "three", "four", "one"]

    std::multiset<std::string> c_mset{ "one", "two", "one", "four" };
    json j_mset(c_mset); // only one entry for "one" is used
    // maybe ["one", "two", "four"]

    std::unordered_multiset<std::string> c_umset{ "one", "two", "one", "four" };
    json j_umset(c_umset); // both entries for "one" are used
    // maybe ["one", "two", "one", "four"]
}

void test03()
{
    std::map<std::string, int> c_map{ {"one", 1}, {"two", 2}, {"three", 3} };
    json j_map(c_map);
    // {"one": 1, "three": 3, "two": 2 }

    std::unordered_map<const char*, double> c_umap{ {"one", 1.2}, {"two", 2.3}, {"three", 3.4} };
    json j_umap(c_umap);
    // {"one": 1.2, "two": 2.3, "three": 3.4}

    std::multimap<std::string, bool> c_mmap{ {"one", true}, {"two", true}, {"three", false}, {"three", true} };
    json j_mmap(c_mmap); // only one entry for key "three" is used
    // maybe {"one": true, "two": true, "three": true}

    std::unordered_multimap<std::string, bool> c_ummap{ {"one", true}, {"two", true}, {"three", false}, {"three", true} };
    json j_ummap(c_ummap); // only one entry for key "three" is used
    // maybe {"one": true, "two": true, "three": true}
}

int main()
{
    test01();
    test02();
    test03();
}