# ReactiveX 学习笔记（24）使用 RxCpp + C++ REST SDK 调用 REST API - zwvista - 博客园
## [ReactiveX 学习笔记（24）使用 RxCpp + C++ REST SDK 调用 REST API](https://www.cnblogs.com/zwvista/p/9607303.html)
### JSON : Placeholder
[JSON : Placeholder (https://jsonplaceholder.typicode.com/)](https://jsonplaceholder.typicode.com/) 是一个用于测试的 REST API 网站。
以下使用 Task API/Rx.NET + Json.NET 调用该网站的 REST API，获取字符串以及 JSON 数据。
- GET /posts/1
- GET /posts
- POST /posts
- PUT /posts/1
- DELETE /posts/1
所有 GET API 都返回JSON数据，格式（JSON-Schema）如下：
```
{
  "type":"object",
  "properties": {
    "userId": {"type" : "integer"},
    "id": {"type" : "integer"},
    "title": {"type" : "string"},
    "body": {"type" : "string"}
  }
}
```
### 安装 C++ REST SDK
```
$ brew install cpprestsdk
$ brew install boost
$ brew install libressl
```
### 安装 "JSON for Modern C++"
```
$ brew tap nlohmann/json
$ brew install nlohmann_json
```
### 下载 RxCpp
`$ git clone --recursive https://github.com/ReactiveX/RxCpp.git`
### 创建工程
打开 Xcode，File / New / Project...
在向导的第1页选 macOS / Command Line Tool
在向导的第2页语言选 C++，Product Name 填上任意名称
在向导的第3页选择任意文件夹，点击 Create 创建工程。
### 配置工程
将 System Header Search Paths 设置为
/usr/local/Cellar/cpprestsdk/2.10.2/include
/usr/local/Cellar/boost/1.67.0_1/include
/usr/local/Cellar/libressl/2.7.4/include
/usr/local/Cellar/nlohmann_json/3.1.2/include
RxCpp安装文件夹/Rx/v2/src
将 Library Search Paths 设置为
/usr/local/Cellar/cpprestsdk/2.10.2/lib
/usr/local/Cellar/boost/1.67.0_1/lib
/usr/local/Cellar/libressl/2.7.4/lib
将 Other Linker Flags 设置为
-lcpprest -lboost_system -lboost_thread-mt -lboost_chrono-mt -lssl -lcrypto
[cpprestsdk: Undefined symbols for architecture x86_64](https://stackoverflow.com/questions/40032483/cpprestsdk-undefined-symbols-for-architecture-x86-64)
### Post
在工程中添加 post.hpp，内容如下
```
#ifndef Post_hpp
#define Post_hpp
#include <iostream>
#include <nlohmann/json.hpp>
using nlohmann::json;
struct Post {
    int userId;
    int id;
    std::string title;
    std::string body;
};
void to_json(json& j, const Post& p);
void from_json(const json& j, Post& p);
std::ostream& operator<<(std::ostream& out, const Post& p);
#endif /* Post_hpp */
```
在工程中添加 post.cpp，内容如下
```
#include "Post.hpp"
#include <boost/algorithm/string/replace.hpp>
using namespace std;
void to_json(json& j, const Post& p) {
    j = json{{"userId", p.userId}, {"id", p.id}, {"title", p.title}, {"body", p.body}};
}
void from_json(const json& j, Post& p) {
    p.userId = j.at("userId").get<int>();
    p.id = j.at("id").get<int>();
    p.title = j.at("title").get<string>();
    p.body = boost::algorithm::replace_all_copy(j.at("body").get<string>(), "\n", "\\n");
}
std::ostream& operator<<(std::ostream& out, const Post& p) {
    cout << "Post {userId = " << p.userId
        << ", id = " << p.id
        << ", title = \"" << p.title
        << "\", body = \"" << p.body
        << "\"}";
    return out;
}
```
### RestApi
在工程中添加 RestApi.hpp，内容如下
```
#ifndef RestApi_hpp
#define RestApi_hpp
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
using namespace utility;                    // Common utilities like string conversions
//using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
#include "rxcpp/rx.hpp"
namespace Rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
}
using namespace Rx;
#include <nlohmann/json.hpp>
using nlohmann::json;
template<class T>
struct RestApi {
    http_client client;
    
    RestApi(const uri &base_uri) : client(base_uri) {}
    
    observable<string_t> getString(const string_t &path_query_fragment) {
        return observable<>::create<string_t>(
        [&](subscriber<string_t> s){
            client
            .request(methods::GET, path_query_fragment)
            .then([](http_response response) -> pplx::task<string_t> {
                return response.extract_string();
            })
            .then([&](pplx::task<string_t> previousTask) {
                try {
                    string_t const & v = previousTask.get();
                    s.on_next(v);
                    s.on_completed();
                } catch (http_exception const & e) {
                    s.on_error(std::current_exception());
                }
            })
            .wait();
        });
    }
    
    observable<T> getObject(const string_t &path_query_fragment) {
        return observable<>::create<T>(
        [&](subscriber<T> s){
            client
            .request(methods::GET, path_query_fragment)
            .then([](http_response response) -> pplx::task<string_t> {
                return response.extract_string();
            })
            .then([&](pplx::task<string_t> previousTask) {
                try {
                    string_t const & v = previousTask.get();
                    json j = json::parse(v);
                    T t = j;
                    s.on_next(t);
                    s.on_completed();
                } catch (http_exception const & e) {
                    s.on_error(std::current_exception());
                }
            })
            .wait();
        });
    }
    
    observable<T> getArray(const string_t &path_query_fragment) {
        return observable<>::create<T>(
        [&](subscriber<T> s){
            client
            .request(methods::GET, path_query_fragment)
            .then([](http_response response) -> pplx::task<string_t> {
                return response.extract_string();
            })
            .then([&](pplx::task<string_t> previousTask) {
                try {
                    string_t const & v = previousTask.get();
                    json j = json::parse(v);
                    std::vector<T> vec = j;
                    for(const auto& t : vec)
                        s.on_next(t);
                    s.on_completed();
                } catch (http_exception const & e) {
                    s.on_error(std::current_exception());
                }
            })
            .wait();
        });
    }
    observable<string_t> createObject(const string_t& url, const T& obj) {
        return observable<>::create<string_t>(
        [&](subscriber<string_t> s){
            json j = obj;
            client
            .request(methods::POST, url, j.dump(), U("application/json"))
            .then([](http_response response) -> pplx::task<string_t> {
                return response.extract_string();
            })
            .then([&](pplx::task<string_t> previousTask) {
                try {
                    string_t const & v = previousTask.get();
                    s.on_next(v);
                    s.on_completed();
                } catch (http_exception const & e) {
                    s.on_error(std::current_exception());
                }
            })
            .wait();
        });
    }
    
    observable<string_t> updateObject(const string_t& url, const T& obj) {
        return observable<>::create<string_t>(
        [&](subscriber<string_t> s){
            json j = obj;
            client
            .request(methods::PUT, url, j.dump(), U("application/json"))
            .then([](http_response response) -> pplx::task<string_t> {
                return response.extract_string();
            })
            .then([&](pplx::task<string_t> previousTask) {
                try {
                    string_t const & v = previousTask.get();
                    s.on_next(v);
                    s.on_completed();
                } catch (http_exception const & e) {
                    s.on_error(std::current_exception());
                }
            })
            .wait();
        });
    }
    
    observable<string_t> deleteObject(const string_t& url) {
        return observable<>::create<string_t>(
        [&](subscriber<string_t> s){
            client
            .request(methods::DEL, url)
            .then([](http_response response) -> pplx::task<string_t> {
                return response.extract_string();
            })
            .then([&](pplx::task<string_t> previousTask) {
                try {
                    string_t const & v = previousTask.get();
                    s.on_next(v);
                    s.on_completed();
                } catch (http_exception const & e) {
                    s.on_error(std::current_exception());
                }
            })
            .wait();
        });
    }
};
#endif /* RestApi_hpp */
```
### main
将 main.cpp 的内容改为
```
#include "Post.hpp"
#include "RestApi.hpp"
using namespace std;
int main(int argc, char* argv[])
{
    RestApi<Post> api(U("https://jsonplaceholder.typicode.com/"));
    api.getString(U("posts/1")).subscribe([](const string_t& v){cout << v << endl;});
    api.getObject(U("posts/1")).subscribe([](const Post& v){cout << v << endl;});
    api.getArray(U("posts")).take(2).subscribe([](const Post& v){cout << v << endl;});
    Post o;
    o.id = 0;
    o.userId = 101;
    o.title = U("test title");
    o.body = U("test body");
    api.createObject(U("posts"), o).subscribe([](string_t v){cout << v << endl;});
    api.updateObject(U("posts/1"), o).subscribe([](string_t v){cout << v << endl;});
    api.deleteObject(U("posts/1")).subscribe([](string_t v){cout << v << endl;});
    return 0;
}
```
### 输出结果
```
{
  "userId": 1,
  "id": 1,
  "title": "sunt aut facere repellat provident occaecati excepturi optio reprehenderit",
  "body": "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"
}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
Post {userId = 1, id = 2, title = "qui est esse", body = "est rerum tempore vitae\nsequi sint nihil reprehenderit dolor beatae ea dolores neque\nfugiat blanditiis voluptate porro vel nihil molestiae ut reiciendis\nqui aperiam non debitis possimus qui neque nisi nulla"}
{
  "body": "test body",
  "id": 101,
  "title": "test title",
  "userId": 101
}
{
  "body": "test body",
  "id": 1,
  "title": "test title",
  "userId": 101
}
{}
```
