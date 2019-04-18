# Swift 4 新特性 - zwvista - 博客园

## [Swift 4 新特性](https://www.cnblogs.com/zwvista/p/7136866.html)

### 多行字符串

```
/// 多行字符串用三引号括起来
let quotation = """
The White Rabbit put on his spectacles.  "Where shall I begin,
please your Majesty?" he asked.
 
"Begin at the beginning," the King said gravely, "and go on
till you come to the end; then stop."
"""
/// 多行字符串内三引号需要转义
let threeDoubleQuotes = """
Escaping the first quote \"""
Escaping all three quotes \"\"\"
"""
/// 打头和结尾的三引号所占的行不计入多行字符串
let singleLineString = "These are the same."
let multilineString = """
These are the same.
"""
/// 开头和结尾都是空行的多行字符串
"""
 
This string starts with a line feed.
It also ends with a line feed.
 
"""
/// 结尾的三引号之前的空格不计入多行字符串
func generateQuotation() -> String {
    let quotation = """
        The White Rabbit put on his spectacles.  "Where shall I begin,
        please your Majesty?" he asked.
 
        "Begin at the beginning," the King said gravely, "and go on
        till you come to the end; then stop."
        """
    return quotation
}
print(quotation == generateQuotation())
// Prints "true"
```

Swift语言中的多行字符串可以说是目前设计得最好的多行字符串。它是社区共同努力的成果。
- 打头和结尾的三引号所占的行不计入多行字符串
- 结尾的三引号之前的空格不计入多行字符串

以上这两个特性使得多行字符串的形状在代码中能够维持，所见即所得，拷贝剪切粘贴都很方便。

### 协议的组合

```
protocol Named {
    var name: String { get }
}
protocol Aged {
    var age: Int { get }
}
struct Person: Named, Aged {
    var name: String
    var age: Int
}
// Named & Aged 表示两个协议的组合
// celebrator 必须同时满足 Named 和 Aged 协议
// 即 celebrator 同时具有 name 和 age 属性
func wishHappyBirthday(to celebrator: Named & Aged) {
    print("Happy birthday, \(celebrator.name), you're \(celebrator.age)!")
}
let birthdayPerson = Person(name: "Malcolm", age: 21)
wishHappyBirthday(to: birthdayPerson)
// Prints "Happy birthday, Malcolm, you're 21!"
```

协议的组合弥补了Swift语言的缺陷。

因为这是一个Objective-C语言中存在但是其后继者Swift语言中却不存在的特性。

Swift 4: Protocol1 & Protocol2

Objective-C: id<Protocol1, Protocol2>

### 键路径表达式（Key path expression）

\type name . path

用于动态属性。

```
struct SomeStructure {
    var someValue: Int
}
 
let s = SomeStructure(someValue: 12)
let pathToProperty = \SomeStructure.someValue
 
let value = s[keyPath: pathToProperty]
// value is 12

let greetings = ["hello", "hola", "bonjour", "안녕"]
let myGreeting = greetings[keyPath: \[String].[1]]
// myGreeting is 'hola'
```

### 可迭代的枚举

符合 CaseIterable 协议的枚举可以迭代。

```
enum Beverage: CaseIterable {
    case coffee, tea, juice
}
let numberOfChoices = Beverage.allCases.count
print("\(numberOfChoices) beverages available")
// Prints "3 beverages available"
```

```
for beverage in Beverage.allCases {
    print(beverage)
}
// coffee
// tea
// juice
```


