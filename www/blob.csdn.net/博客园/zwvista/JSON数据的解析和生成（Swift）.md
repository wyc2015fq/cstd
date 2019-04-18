# JSON数据的解析和生成（Swift） - zwvista - 博客园

## [JSON数据的解析和生成（Swift）](https://www.cnblogs.com/zwvista/p/9519776.html)

### Codable

```
public typealias Codable = Decodable & Encodable
public protocol Decodable {}
public protocol Encodable {}
```

Swift 对象与 JSON 数据相互转换的关键是实现 Codable 接口。

### 示例

```
import Foundation

struct Persons : Codable {
    let persons: [Person]
}

struct Person : Codable {
    let name: String
    let age: Int
    // only necessary when we want to customize the keys
    enum CodingKeys : String, CodingKey {
        case name = "name"
        case age = "age"
    }
}

let jsonString = """
{
  "persons" : [
    {
      "name" : "Joe",
      "age" : 12
    }
  ]
}
"""
let jsonData = jsonString.data(using: .utf8)!
let decoder = JSONDecoder()
let o = try! decoder.decode(Persons.self, from: jsonData)
print(o) // Persons(persons: [__lldb_expr_65.Person(name: "Joe", age: 12)])

let encoder = JSONEncoder()
//encoder.outputFormatting = .prettyPrinted
let data = try! encoder.encode(o)
print(String(data: data, encoding: .utf8)!) // {"persons":[{"name":"Joe","age":12}]}
```

### 参考链接

[Ultimate Guide to JSON Parsing with Swift 4](https://benscheirman.com/2017/06/swift-json/)
[http://www.jsoncafe.com/](http://www.jsoncafe.com/) 可以根据 JSON 数据自动生成实现 Codable 的 Swift 类。


