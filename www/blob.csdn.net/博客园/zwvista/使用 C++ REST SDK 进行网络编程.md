# 使用 C++ REST SDK 进行网络编程 - zwvista - 博客园

## [使用 C++ REST SDK 进行网络编程](https://www.cnblogs.com/zwvista/p/9581434.html)

### 安装 C++ REST SDK

```
$ brew install cpprestsdk
$ brew install boost
$ brew install libressl
```

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

将 Library Search Paths 设置为

/usr/local/Cellar/cpprestsdk/2.10.2/lib

/usr/local/Cellar/boost/1.67.0_1/lib

/usr/local/Cellar/libressl/2.7.4/lib

将 Other Linker Flags 设置为

-lcpprest -lboost_system -lboost_thread-mt -lboost_chrono-mt -lssl -lcrypto

[cpprestsdk: Undefined symbols for architecture x86_64](https://stackoverflow.com/questions/40032483/cpprestsdk-undefined-symbols-for-architecture-x86-64)

### 示例代码1

```
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

int main(int argc, char* argv[])
{
    auto fileStream = std::make_shared<ostream>();
    
    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile) {
         *fileStream = outFile;
        
         // Create http_client to send the request.
         http_client client(U("http://www.bing.com/"));
        
         // Build request URI and start the request.
         uri_builder builder(U("/search"));
         builder.append_query(U("q"), U("cpprestsdk github"));
         return client.request(methods::GET, builder.to_string());
    })
    
    // Handle response headers arriving.
    .then([=](http_response response) {
        printf("Received response status code:%u\n", response.status_code());

        // Write response body into the file.
        return response.body().read_to_end(fileStream->streambuf());
    })
    
    // Close the file stream.
    .then([=](size_t) {
        return fileStream->close();
    });
    
    // Wait for all the outstanding I/O to complete and handle any exceptions
    try {
        requestTask.wait();
    } catch (const std::exception &e) {
        printf("Error exception:%s\n", e.what());
    }

    return 0;
}
```

这段代码访问
[http://www.bing.com/search?q=cpprestsdk%20github](http://www.bing.com/search?q=cpprestsdk%20github)

将结果保存为 results.html。

### JSON : Placeholder

[JSON : Placeholder (https://jsonplaceholder.typicode.com/)](https://jsonplaceholder.typicode.com/) 是一个用于测试的 REST API 网站。

以下使用 RxJava2 + Retrofit2 调用该网站的 REST API，获取字符串以及 JSON 数据。
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

### 示例代码2

```
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <boost/algorithm/string/replace.hpp>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

using namespace std;

static void print_results(json::value const & value)
{
    if(!value.is_null()) {
        auto userId = value.at(U("userId")).as_integer();
        auto id = value.at(U("id")).as_integer();
        auto title = value.at(U("title")).as_string();
        auto body = boost::algorithm::replace_all_copy(value.at(U("body")).as_string(), "\n", "\\n");

        cout << "Post {userId = " << userId
            << ", id = " << id
            << ", title = \"" << title
            << "\", body = \"" << body
            << "\"}" << endl;
    }
}

static void json_get()
{
    http_client client(U("https://jsonplaceholder.typicode.com/"));
    // Build request URI and start the request.
    uri_builder builder(U("posts/1"));

    client
    // send the HTTP GET request asynchronous
    .request(methods::GET, builder.to_string())
    // continue when the response is available
    .then([](http_response response) -> pplx::task<json::value> {
        // if the status is OK extract the body of the response into a JSON value
        // works only when the content type is application\json
        if(response.status_code() == status_codes::OK) {
            return response.extract_json();
        }

        // return an empty JSON value
        return pplx::task_from_result(json::value());
    })
    // continue when the JSON value is available
    .then([](pplx::task<json::value> previousTask) {
        // get the JSON value from the task and display content from it
        try {
            json::value const & v = previousTask.get();
            print_results(v);
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}

static void json_post()
{
    http_client client(U("https://jsonplaceholder.typicode.com/"));
    
    json::value json_v ;
    json_v["userId"] = json::value::number(101);
    json_v["title"] = json::value::string("test title");
    json_v["body"] = json::value::string("test body");

    client
    .request(methods::POST, U("posts"), json_v)
    .then([](http_response response) -> pplx::task<string_t> {
        if(response.status_code() == status_codes::Created) {
            return response.extract_string();
        }
        return pplx::task_from_result(string_t());
    })
    .then([](pplx::task<string_t> previousTask) {
        try {
            string_t const & v = previousTask.get();
            cout << v << endl;
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}

static void json_update()
{
    http_client client(U("https://jsonplaceholder.typicode.com/"));
    
    json::value json_v ;
    json_v["userId"] = json::value::number(101);
    json_v["title"] = json::value::string("test title");
    json_v["body"] = json::value::string("test body");
    
    client
    .request(methods::PUT, U("posts/1"), json_v)
    .then([](http_response response) -> pplx::task<string_t> {
        if(response.status_code() == status_codes::OK) {
            return response.extract_string();
        }
        return pplx::task_from_result(string_t());
    })
    .then([](pplx::task<string_t> previousTask) {
        try {
            string_t const & v = previousTask.get();
            cout << v << endl;
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}

static void json_delete()
{
    http_client client(U("https://jsonplaceholder.typicode.com/"));
    
    client
    .request(methods::DEL, U("posts/1"))
    .then([](http_response response) -> pplx::task<string_t> {
        if(response.status_code() == status_codes::OK) {
            return response.extract_string();
        }
        return pplx::task_from_result(string_t());
    })
    .then([](pplx::task<string_t> previousTask) {
        try {
            string_t const & v = previousTask.get();
            cout << v << endl;
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}

int main(int argc, char* argv[])
{
    json_get();
    json_post();
    json_update();
    json_delete();

    return 0;
}
```

这段代码调用4个 REST API

然后打印所返回的 JSON 数据以及字符串的内容

```
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
{
  "body": "test body",
  "title": "test title",
  "userId": 101,
  "id": 101
}
{
  "body": "test body",
  "title": "test title",
  "userId": 101,
  "id": 1
}
{}
```


