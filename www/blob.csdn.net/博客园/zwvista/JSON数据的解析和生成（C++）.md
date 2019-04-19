# JSON数据的解析和生成（C++） - zwvista - 博客园
## [JSON数据的解析和生成（C++）](https://www.cnblogs.com/zwvista/p/9591853.html)
### 安装 "JSON for Modern C++"
```
$ brew tap nlohmann/json
$ brew install nlohmann_json
```
安装之后将`/usr/local/Cellar/nlohmann_json/3.1.2/include`加入 include
### 示例
```
#include <iostream>
#include <nlohmann/json.hpp>
using nlohmann::json;
using namespace std;
struct Person {
    string name;
    int age;
};
struct Persons {
    vector<Person> persons;
};
void to_json(json& j, const Person& p) {
    j = json{{"name", p.name}, {"age", p.age}};
}
void from_json(const json& j, Person& p) {
    p.name = j.at("name").get<string>();
    p.age = j.at("age").get<int>();
}
void to_json(json& j, const Persons& ps) {
    j = json{{"persons", ps.persons}};
}
void from_json(const json& j, Persons& ps) {
    ps.persons = j.at("persons").get<vector<Person>>();
}
string jsonString = R"({
  "persons" : [
    {
      "name" : "Joe",
      "age" : 12
    }
  ]
})";
int main(int argc, char *args[])
{
    json j = json::parse(jsonString);
    Persons ps = j;
    auto &p = ps.persons[0];
    cout << "name: " << p.name << endl
        << "age: " << p.age << endl;
    j = ps;
    cout << j << endl;
    cout << j.dump(2) << endl;
    return 0;
}
/*
name: Joe
age: 12
{"persons":[{"age":12,"name":"Joe"}]}
{
  "persons": [
    {
      "age": 12,
      "name": "Joe"
    }
  ]
}
*/
```
