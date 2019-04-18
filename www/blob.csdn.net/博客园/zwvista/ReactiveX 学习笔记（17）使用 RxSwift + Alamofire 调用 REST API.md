# ReactiveX 学习笔记（17）使用 RxSwift + Alamofire 调用 REST API - zwvista - 博客园

## [ReactiveX 学习笔记（17）使用 RxSwift + Alamofire 调用 REST API](https://www.cnblogs.com/zwvista/p/9521239.html)

### JSON : Placeholder

[JSON : Placeholder (https://jsonplaceholder.typicode.com/)](https://jsonplaceholder.typicode.com/) 是一个用于测试的 REST API 网站。

以下使用 RxSwift + Alamofire 调用该网站的 REST API，获取字符串以及 JSON 数据。
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

### 创建工程

打开 Xcode，File / New / Project..

在 New Project 向导的第1页，选 iOS / Single View App

在向导的第2页填上 Product Name: RxExample

在向导的第3页选择任意文件夹点击 Create 按钮创建工程

关闭所创建的工程

### 配置 Pods

在工程所在文件夹下创建 Podfile 文件，内容如下：

```
workspace 'RxExample'

use_frameworks!

target 'RxExample' do
project 'RxExample'
pod 'CodableAlamofire'
pod 'RxAlamofire'
end
```

打开终端在工程所在文件夹下执行 pod install 命令。

```
$ cd RxExample
$ pod install
...
Installing Alamofire (4.7.3)
Installing CodableAlamofire (1.1.0)
Installing RxAlamofire (4.2.0)
Installing RxSwift (4.2.0)
...
```

用 Xcode 打开 RxExample.xcworkspace

### RxCodableAlamofire

在 RxExample 工程中添加 RxCodableAlamofire.swift 文件，内容如下：

```
import Foundation
import RxSwift
import Alamofire
import RxAlamofire
import CodableAlamofire

class RxCodableAlamofire {
    public static func object<T: Decodable>(_ method: Alamofire.HTTPMethod, _ url: URLConvertible, parameters: [String: Any]? = nil, encoding: ParameterEncoding = URLEncoding.default, headers: [String: String]? = nil, queue: DispatchQueue? = nil, keyPath: String? = nil, mapToObject object: T? = nil, decoder: JSONDecoder = JSONDecoder()) -> Observable<T> {
        return SessionManager.default.rx.object(method, url, parameters: parameters, encoding: encoding, headers: headers, queue: queue, keyPath: keyPath, mapToObject: object, decoder: decoder)
    }
}

extension Reactive where Base: SessionManager {
    public func object<T: Decodable>(_ method: Alamofire.HTTPMethod, _ url: URLConvertible, parameters: [String: Any]? = nil, encoding: ParameterEncoding = URLEncoding.default, headers: [String: String]? = nil, queue: DispatchQueue? = nil, keyPath: String? = nil, mapToObject object: T? = nil, decoder: JSONDecoder = JSONDecoder()) -> Observable<T> {
        return request(method, url, parameters: parameters, encoding: encoding, headers: headers).flatMap { $0.rx.object(queue: queue, keyPath: keyPath, mapToObject: object, decoder: decoder) }
    }
}

extension Reactive where Base: DataRequest {
    public func object<T: Decodable>(queue: DispatchQueue? = nil, keyPath: String? = nil, mapToObject object: T? = nil, decoder: JSONDecoder = JSONDecoder()) -> Observable<T> {
        return result(queue: queue, responseSerializer: DataRequest.DecodableObjectSerializer(keyPath, decoder))
    }
}
```

在 Pods 工程中找到并打开 DataRequest+Decodable.swift （Pods/CodableAlamofire）

将其中的 DecodableObjectSerializer 的可见性从 private 改为 public。(提示是否 unlock 时按下 Unlock)

### RestApi

在 RxExample 工程中添加 RestApi.swift 文件，内容如下：

```
import Foundation
import Alamofire
import RxSwift
import RxAlamofire

// https://stackoverflow.com/questions/27855319/post-request-with-a-simple-string-in-body-with-alamofire
class StringEncoding: ParameterEncoding {
    let body: String;
    
    public init(body: String) {
        self.body = body
    }
    
    public func encode(_ urlRequest: URLRequestConvertible, with parameters: Parameters?) throws -> URLRequest {
        var request = try urlRequest.asURLRequest()
        request.httpBody = body.data(using: .utf8, allowLossyConversion: false)
        return request
    }
}

extension Encodable {
    
    public func toJSONString(prettyPrint: Bool) throws -> String? {
        let encoder = JSONEncoder()
        if prettyPrint { encoder.outputFormatting = .prettyPrinted }
        let data = try! encoder.encode(self)
        let jsonString = String(data: data, encoding: .utf8)
        return jsonString
    }
    
}

class RestApi {
    static func getObject<T: Decodable>(url: String, keyPath: String? = nil) -> Observable<T> {
        return RxCodableAlamofire.object(.get, url, keyPath: keyPath)
    }
    static func getArray<T: Decodable>(url: String, keyPath: String? = nil) -> Observable<[T]> {
        return RxCodableAlamofire.object(.get, url, keyPath: keyPath)
    }
    static func update(url: String, body: String) -> Observable<String> {
        return RxAlamofire.string(.put, url, encoding: StringEncoding(body: body))
    }
    static func create(url: String, body: String) -> Observable<String> {
        return RxAlamofire.string(.post, url, encoding: StringEncoding(body: body))
    }
    static func delete(url: String) -> Observable<String> {
        return RxAlamofire.string(.delete, url)
    }
    static func getString(url: String) -> Observable<String> {
        return RxAlamofire.string(.get, url)
    }
}
```

### Post

在 RxExample 工程中添加 Post.swift 文件，内容如下：

```
import Foundation
import RxSwift

struct Post : Codable {
    let userId: Int
    let id: Int
    let title: String
    let body: String
    var description: String {
        return "Post {userId = \(userId), id = \(id), title = \"\(title)\", body = \"\(body.replacingOccurrences(of: "\n", with: "\\n"))\"}";
    }
    
    static let url = "https://jsonplaceholder.typicode.com/"

    static func getPostAsString() -> Observable<String> {
        return RestApi.getString(url: "\(url)posts/1")
    }
    static func getPostAsJson() -> Observable<Post> {
        return RestApi.getObject(url: "\(url)posts/1")
    }
    static func getPosts(n: Int) -> Observable<Post> {
        return RestApi.getArray(url: "\(url)posts").flatMap { Observable.from($0) }.take(n)
    }
    static func createPost() -> Observable<String> {
        let post = Post(userId: 101, id: 0, title: "test title", body: "test body")
        return RestApi.create(url: "\(url)posts", body: try! post.toJSONString(prettyPrint: false)!)
    }
    static func updatePost() -> Observable<String> {
        let post = Post(userId: 101, id: 1, title: "test title", body: "test body")
        return RestApi.update(url: "\(url)posts/1", body: try! post.toJSONString(prettyPrint: false)!)
    }
    static func deletePost() -> Observable<String> {
        return RestApi.delete(url: "\(url)posts/1")
    }
}
```
- getPostAsString 方法取出第1个Post，返回字符串
- getPostAsJson 方法取出第1个Post，返回Post对象
- getPosts 方法取出前n个Post，返回n个Post对象
- createPost 方法创建1个Post，返回字符串
- updatePost 方法更新第1个Post，返回字符串
- deletePost 方法删除第1个Post，返回字符串

### AppDelegate

在 AppDelegate.swift 中添加 RxSwift 的引用，然后在 AppDelegate 类中添加 DisposeBag 类型的实例。

```
import RxSwift

class AppDelegate {
// ...
let disposeBag = DisposeBag()
// ...
}
```

在 AppDelegate 类的 applicationDidFinishLaunching 方法中添加以下代码

```
Post.getPostAsString().do(onNext: { print($0) }).subscribe().disposed(by: disposeBag)
Post.getPostAsJson().do(onNext: { print($0.description) }).subscribe().disposed(by: disposeBag)
Post.getPosts(n: 2).do(onNext: { print($0.description) }).subscribe().disposed(by: disposeBag)
Post.createPost().do(onNext: { print($0) }).subscribe().disposed(by: disposeBag)
Post.updatePost().do(onNext: { print($0) }).subscribe().disposed(by: disposeBag)
Post.deletePost().do(onNext: { print($0) }).subscribe().disposed(by: disposeBag)
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
Post {userId = 1, id = 2, title = "qui est esse", body = "est rerum tempore vitae\nsequi sint nihil reprehenderit dolor beatae ea dolores neque\nfugiat blanditiis voluptate porro vel nihil molestiae ut reiciendis\nqui aperiam non debitis possimus qui neque nisi nulla"}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
{}
{
  "{\"body\":\"test body\",\"id\":0,\"title\":\"test title\",\"userId\":101}": "",
  "id": 101
}
{
  "id": 1
}
```


