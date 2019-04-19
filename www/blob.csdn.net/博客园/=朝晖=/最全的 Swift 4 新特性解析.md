# 最全的 Swift 4 新特性解析 - =朝晖= - 博客园
# [最全的 Swift 4 新特性解析](https://www.cnblogs.com/dhcn/p/7597536.html)
转自： http://www.jianshu.com/p/f35514ae9c1a
WWDC 2017 带来了很多惊喜。Swift 4 也伴随着 Xcode 9 测试版来到了我们的面前，很多强大的新特性非常值得我们期待在正式项目中去使用它。因为 Swift 4 是开源的，如果你关注 `swift-evolution` 这个项目的话，就应该已经提前了解到它的新特性了。本文参考了 WWDC 2017 以及各种资料，从语法、字符串、标准库、构建过程等方面，把 Swift 4 的这些新特性一一列举出来做介绍和分析，让他们毫无保留地展现在你眼前。
# 一、语法改进
## extension 中可以访问 private 的属性
考虑以下代码：
```
struct Date: Equatable, Comparable {
    private let secondsSinceReferenceDate: Double
    static func ==(lhs: Date, rhs: Date) -> Bool {
        return lhs.secondsSinceReferenceDate == rhs.secondsSinceReferenceDate
    }
    static func <(lhs: Date, rhs: Date) -> Bool {
        return lhs.secondsSinceReferenceDate < rhs.secondsSinceReferenceDate
    }
}
```
上面代码定义了一个 Date 结构体，并实现 Equatable 和 Comparable 协议。为了让代码更清晰，可读性更好，一般会把对协议的实现放在单独的 extension 中，这也是一种非常符合 Swift 风格的写法，如下：
```
struct Date {
    private let secondsSinceReferenceDate: Double
}
extension Date: Equatable {
    static func ==(lhs: Date, rhs: Date) -> Bool {
        return lhs.secondsSinceReferenceDate == rhs.secondsSinceReferenceDate
    }
}
extension Date: Comparable {
    static func <(lhs: Date, rhs: Date) -> Bool {
        return lhs.secondsSinceReferenceDate < rhs.secondsSinceReferenceDate
    }
}
```
但是在 Swift 3 中，编译就报错了，因为 extension 中无法获取到 secondsSinceReferenceDate 属性，因为它是 `private` 的。于是在 Swift 3 中，必须把 private 改为 fileprivate。
```
struct Date {
    fileprivate let secondsSinceReferenceDate: Double
}
...
```
但是如果用 fileprivate，属性的作用域就会比我们需要的更大，可能会不小心造成属性的滥用。
在 Swift 4 中，private 的属性的作用域扩大到了 extension 中，并且被限定在了 struct 和 extension 内部，这样就不需要再改成 fileprivate 了，这是最好的结果。
## 类型和协议的组合类型
考虑以下代码：
```
protocol Shakeable {
    func shake()
}
extension UIButton: Shakeable { /* ... */ }
extension UISlider: Shakeable { /* ... */ }
func shakeEm(controls: [???]) {
    for control in controls where control.state.isEnabled {
    }
    control.shake()
}
```
在 Swift 3 中，这里的 `???` 应该写什么呢？如果写 `UIControl`，那么 `control.shake()` 就会报错；如果写 `Shakeable`，那么 `control.state.isEnabled` 就会报错。其实我们也可以这样写：
```
func shakeEm(controls: [UIControl]) {
    for control in controls where control.isEnabled {
        if control is Shakeable {
            (control as! Shakeable).shake()
        }
    }
}
```
这样写虽然可以跑通了，但是很丑陋。
在 Swift 4 中，可以把类型和协议用 `&` 组合在一起作为一个类型使用，就可以像下面这样写了：
```
protocol Shakeable {
    func shake()
}
extension UIButton: Shakeable { /* ... */ }
extension UISlider: Shakeable { /* ... */ }
func shakeEm(controls: [UIControl & Shakeable]) {
    for control in controls where control.state.isEnabled {
        control.shake()
    }
}
```
把它声明为了 `UIControl & Shakeable` 类型。OK，圆满解决。
PS:
这个代码例子是 WWDC 2017 的 PPT 中的，上面的代码有点问题，control.state.isEnabled 这句代码中，state 是没有 isEnabled 这个属性的，改为 `control.isEnabled` 就可以了。看来苹果的工程师做 PPT 有时候还是不太严谨。
另外，iOS SDK 中的 API 也用这个特性做了优化，例如：
```
// Objective-C API
@interface NSCandidateListTouchBarItem<CandidateType> : NSTouchBarItem
@property (nullable, weak) NSView <NSTextInputClient> *client;
@end
```
这个 API 的 Objective-C 版本是没有问题的，可以知道 client 属性既是一个 NSView，又符合 NSTextInputClient 协议。然而它对应的 Swift 3 版本为：
```
class NSCandidateListTouchBarItem<CandidateType: AnyObject> : NSTouchBarItem {
    var client: NSView?
}
```
仅仅是一个 NSView 类型 /(ㄒoㄒ)/~~
在 Swift 4 中，这类 API 做了优化，改成了：
```
class NSCandidateListTouchBarItem<CandidateType: AnyObject> : NSTouchBarItem {
    var client: (NSView & NSTextInputClient)?
}
```
这样类型的声明就更加严谨了。
## Associated Type 可以追加 Where 约束语句
在 Swift 4 中可以在 associatedtype 后面声明的类型后追加 where 语句
`associatedtype Element where <xxx>`
看下面是 Swift 4 标准库中 Sequence 中 Element 的声明：
```
protocol Sequence {
    associatedtype Element where Self.Element == Self.Iterator.Element
    // ...
}
```
它限定了 Sequence 中 Element 这个类型必须和 Iterator.Element 的类型一致。
通过 where 语句可以对类型添加更多的约束，使其更严谨，避免在使用这个类型时做多余的类型判断。
## 新的 Key Paths 语法
先来看看 Swift 3 中 Key Paths 的写法：
```
@objcMembers class Kid: NSObject {
    dynamic var nickname: String = ""
    dynamic var age: Double = 0.0
    dynamic var friends: [Kid] = []
}
var ben = Kid(nickname: "Benji", age: 5.5)
let kidsNameKeyPath = #keyPath(Kid.nickname)
let name = ben.valueForKeyPath(kidsNameKeyPath)
ben.setValue("Ben", forKeyPath: kidsNameKeyPath)
```
Swift 4 中创建一个 KeyPath 用 `` 作为开头：
`\Kid.nickname`
当编译器可以推导出类型时，可以省略基础类型部分：
`\.nickname`
上面的代码在 Swift 4 中就可以这样写：
```
struct Kid {
    var nickname: String = ""
    var age: Double = 0.0
    var friends: [Kid] = []
}
var ben = Kid(nickname: "Benji", age: 8, friends: [])
let name = ben[keyPath: \Kid.nickname]
ben[keyPath: \Kid.nickname] = "BigBen"
```
相比 Swift 3，Swift 4 的 Key Paths 具有以下优势：
- 类型可以定义为 class、struct
- 定义类型时无需加上 @objcMembers、dynamic 等关键字
- 性能更好
- 类型安全和类型推断，例如 `ben.valueForKeyPath(kidsNameKeyPath)` 返回的类型是 Any，`ben[keyPath: \Kid.nickname]` 直接返回 String 类型
- 可以在所有值类型上使用
## 下标支持泛型
有时候会写一些数据容器，Swift 支持通过下标来读写容器中的数据，但是如果容器类中的数据类型定义为泛型，以前的下标语法就只能返回 Any，在取出值后需要用 `as?` 来转换类型。Swift 4 定义下标也可以使用泛型了。
```
struct GenericDictionary<Key: Hashable, Value> {
    private var data: [Key: Value]
    init(data: [Key: Value]) {
        self.data = data
    }
    subscript<T>(key: Key) -> T? {
        return data[key] as? T
    }
}
let dictionary = GenericDictionary(data: ["Name": "Xiaoming"])
let name: String? = dictionary["Name"] // 不需要再写 as? String
```
# 二、字符串
## Unicode 字符串在计算 count 时的正确性改善
在 Unicode 中，有些字符是由几个其它字符组成的，比如 `é` 这个字符，它可以用 `\u{E9}` 来表示，也可以用 e 字符和上面一撇字符组合在一起表示 `\u{65}\u{301}`。
考虑以下代码：
```
var family = "👩"
family += "\u{200D}👩"
family += "\u{200D}👧" 
family += "\u{200D}👦"
print(family)
print(family.characters.count)
```
这个 family 是一个由多个字符组合成的字符，打印出来的结果为 👩‍👩‍👧‍👦。上面的代码在 Swift 3 中打印的 count 数是 4，在 Swift 4 中打印出的 count 是 1。
## 更快的字符处理速度
Swift 4 的字符串优化了底层实现，对于英语、法语、德语、西班牙语的处理速度提高了 3.5 倍。
![](https://upload-images.jianshu.io/upload_images/2564398-fe89f80dfd39f1b2.png?imageMogr2/auto-orient/strip%7CimageView2/2)
对于简体中文、日语的处理速度提高了 2.5 倍。
![](https://upload-images.jianshu.io/upload_images/2564398-19f4322146c2103d.png?imageMogr2/auto-orient/strip%7CimageView2/2)
## 去掉 characters
Swift 3 中的 String 需要通过 characters 去调用的属性方法，在 Swift 4 中可以通过 String 对象本身直接调用，例如：
```
let values = "one,two,three..."
var i = values.characters.startIndex
while let comma = values.characters[i...<values.characters.endIndex].index(of: ",") {
    if values.characters[i..<comma] == "two" {
        print("found it!")
    }
    i = values.characters.index(after: comma)
}
```
Swift 4 可以把上面代码中的所有的 characters 都去掉，修改如下：
```
let values = "one,two,three..."
var i = values.startIndex
while let comma = values[i...<values.endIndex].index(of: ",") {
    if values[i..<comma] == "two" {
        print("found it!")
    }
    i = values.index(after: comma)
}
```
## One-sided Slicing
Swift 4 新增了一个语法糖 `...` 可以对字符串进行单侧边界取子串。
Swift 3:
```
let values = "abcdefg"
let startSlicingIndex = values.index(values.startIndex, offsetBy: 3)
let subvalues = values[startSlicingIndex..<values.endIndex]
// defg
```
Swift 4:
```
let values = "abcdefg"
let startSlicingIndex = values.index(values.startIndex, offsetBy: 3)
let subvalues = values[startSlicingIndex...] // One-sided Slicing
// defg
```
## String 当做 Collection 来用
Swift 4 中 String 可以当做 Collection 来用，并不是因为 String 实现了 Collection 协议，而是 String 本身增加了很多 Collection 协议中的方法，使得 String 在使用时看上去就是个 Collection。例如：
翻转字符串：
```
let abc: String = "abc"
print(String(abc.reversed()))
// cba
```
遍历字符：
```
let abc: String = "abc"
for c in abc {
    print(c)
}
/*
a
b
c
*/
```
Map、Filter、Reduce:
```
// map
let abc: String = "abc"
_ = abc.map {
    print($0.description)
}
// filter
let filtered = abc.filter { $0 == "b" }
// reduce
let result = abc.reduce("1") { (result, c) -> String in
    print(result)
    print(c)
    return result + String(c)
}
print(result)
```
## Substring
![](https://upload-images.jianshu.io/upload_images/2564398-eebe9aa29e6661ee.png?imageMogr2/auto-orient/strip%7CimageView2/2)
在 Swift 中，String 的背后有个 Owner Object 来跟踪和管理这个 String，String 对象在内存中的存储由内存其实地址、字符数、指向 Owner Object 指针组成。Owner Object 指针指向 Owner Object 对象，Owner Object 对象持有 String Buffer。当对 String 做取子字符串操作时，子字符串的 Owner Object 指针会和原字符串指向同一个对象，因此子字符串的 Owner Object 会持有原 String 的 Buffer。当原字符串销毁时，由于原字符串的 Buffer 被子字符串的 Owner Object 持有了，原字符串 Buffer 并不会释放，造成极大的内存浪费。
在 Swift 4 中，做取子串操作的结果是一个 Substring 类型，它无法直接赋值给需要 String 类型的地方。必须用 String(<substring>) 包一层，系统会通过复制创建出一个新的字符串对象，这样原字符串在销毁时，原字符串的 Buffer 就可以完全释放了。
```
let big = downloadHugeString()
let small = extractTinyString(from: big)
mainView.titleLabel.text = small // Swift 4 编译报错
mainView.titleLabel.text = String(small) // 编译通过
```
## 多行字符串字面量
Swift 3 中写很长的字符串只能写在一行。
```
func tellJoke(name: String, character: Character) {
    let punchline = name.filter { $0 != character }
    let n = name.count - punchline.count
    let joke = "Q: Why does \(name) have \(n) \(character)'s in their name?\nA: I don't know, why does \(name) have \(n) \(character)'s in their name?\nQ: Because otherwise they'd be called \(punchline)."
    print(joke)
}
tellJoke(name: "Edward Woodward", character: "d")
```
字符串中间有换行只能通过添加 `\n` 字符来代表换行。
Swift 4 可以把字符串写在一对 `"""` 中，这样字符串就可以写成多行。
```
func tellJoke(name: String, character: Character) {
    let punchline = name.filter { $0 != character }
    let n = name.count - punchline.count
    let joke = """
        Q: Why does \(name) have \(n) \(character)'s in their name?
        A: I don't know, why does \(name) have \(n) \(character)'s in their name?
        Q: Because otherwise they'd be called \(punchline).
        """
    print(joke)
}
tellJoke(name: "Edward Woodward", character: "d")
```
# 三、Swift 标准库
## Encoding and Decoding
当需要将一个对象持久化时，需要把这个对象序列化，往常的做法是实现 NSCoding 协议，写过的人应该都知道实现 NSCoding 协议的代码写起来很痛苦，尤其是当属性非常多的时候。几年前有一个工具能自动生成 Objective-C 的实现 NSCoding 协议代码，当时用着还不错，但后来这个工具已经没有人维护很久了，而且不支持 Swift。
Swift 4 中引入了 `Codable` 帮我们解决了这个问题。
```
struct Language: Codable {
    var name: String
    var version: Int
}
```
我们想将这个 Language 对象的实例持久化，只需要让 Language 符合 Codable 协议即可，Language 中不用写别的代码。符合了 Codable 协议以后，可以选择把对象 encode 成 JSON 或者 PropertyList。
Encode 操作如下：
```
let swift = Language(name: "Swift", version: 4)
if let encoded = try? JSONEncoder().encode(swift) {
    // 把 encoded 保存起来
}
```
Decode 操作如下：
```
if let decoded = try? JSONDecoder().decode(Language.self, from: encoded) {
    print(decoded.name)
}
```
## Sequence 改进
Swift 3:
```
protocol Sequence {
    associatedtype Iterator: IteratorProtocol
    func makeIterator() -> Iterator
}
```
Swift 4:
```
protocol Sequence {
    associatedtype Element
    associatedtype Iterator: IteratorProtocol where Iterator.Element == Element
    func makeIterator() -> Iterator
}
```
由于 Swift 4 中的 associatedtype 支持追加 where 语句，所以 Sequence 做了这样的改进。
Swift 4 中获取 Sequence 的元素类型可以不用 Iterator.Element，而是直接取 Element。
SubSequence 也做了修改：
```
protocol Sequence {
    associatedtype SubSequence: Sequence 
        where SubSequence.SubSequence == SubSequence,
              SubSequence.Element == Element
}
```
通过 where 语句的限定，保证了类型正确，避免在使用 Sequence 时做一些不必要的类型判断。
Collection 也有一些类似的修改。
## Protocol-oriented integers
整数类型符合的协议有修改，新增了 `FixedWidthInteger` 等协议，具体的协议继承关系如下：
```
+-------------+   +-------------+
        +------>+   Numeric   |   | Comparable  |
        |       |   (+,-,*)   |   | (==,<,>,...)|
        |       +------------++   +---+---------+
        |                     ^       ^
+-------+------------+        |       |
|    SignedNumeric   |      +-+-------+-----------+
|     (unary -)      |      |    BinaryInteger    |
+------+-------------+      |(words,%,bitwise,...)|
       ^                    ++---+-----+----------+
       |         +-----------^   ^     ^---------------+
       |         |               |                     |
+------+---------++    +---------+---------------+  +--+----------------+
|  SignedInteger  |    |  FixedWidthInteger      |  |  UnsignedInteger  |
|                 |    |(endianness,overflow,...)|  |                   |
+---------------+-+    +-+--------------------+--+  +-+-----------------+
                ^        ^                    ^       ^
                |        |                    |       |
                |        |                    |       |
               ++--------+-+                +-+-------+-+
               |Int family |-+              |UInt family|-+
               +-----------+ |              +-----------+ |
                 +-----------+                +-----------+
```
## Dictionary and Set enhancements
这里简单列一下 Dictionary 和 Set 增强了哪些功能：
- 通过 Sequence 来初始化
- 可以包含重复的 Key
- Filter 的结果的类型和原类型一致
- Dictionary 的 mapValues 方法
- Dictionary 的默认值
- Dictionary 可以分组
- Dictionary 可以翻转
## NSNumber bridging and Numeric types
```
let n = NSNumber(value: 999)
let v = n as? UInt8 // Swift 4: nil, Swift 3: 231
```
在 Swift 4 中，把一个值为 999 的 NSNumber 转换为 UInt8 后，能正确的返回 `nil`，而在 Swift 3 中会不可预料的返回 231。
## MutableCollection.swapAt(*:*:)
MutableCollection 现在有了一个新方法 swapAt(*:*:) 用来交换两个位置的值，例如：
```
var mutableArray = [1, 2, 3, 4]
mutableArray.swapAt(1, 2)
print(mutableArray)
// 打印结果：[1, 3, 2, 4]
```
# 四、构建过程改进
## New Build System
Xcode 9 引入了 New Build System，可在 Xcode 9 的 `File -> Project Settings...` 中选择开启。
![](https://upload-images.jianshu.io/upload_images/2564398-5a2566e214b350c5.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2)
## 预编译 Bridging Headers 文件
对于 Swift 和 Objective-C 混合的项目，Swift 调用 Objective-C 时，需要建立一个 Bridging Headers 文件，然后把 Swift 要调用的 Objective-C 类的头文件都写在里面，编译器会读取 Bridging Headers 中的头文件，然后生成一个庞大的 Swift 文件，文件内容是这些头文件内的 API 的 Swift 版本。然后编译器会在编译每一个 Swift 文件时，都要编译一遍这个庞大的 Swift 文件的内容。
有了预编译 Bridging Headers 以后，编译器会在预编译阶段把 Bridging Headers 编译一次，然后插入到每个 Swift 文件中，这样就大大提高了编译速度。
苹果宣称 Xcode 9 和 Swift 4 对于 Swift 和 Objective-C 混合编译的速度提高了 40%。
## Indexing 可以在编译的同时进行
用 Swift 开发项目时，近几个版本的 Xcode 进行 Indexing 的速度慢的令人发指。Xcode 9 和 Swift 4 在这方面做了优化，可以在编译的同时进行 Indexing，一般编译结束后 Indexing 也会同时完成。
## COW Existential Containers
Swift 中有个东西叫 Existential Containers，它用来保存未知类型的值，它的内部是一个 Inline value buffer，如果 Inline value buffer 中的值占用空间很大时，这个值会被分配在堆上，然而在堆上分配内存是一个性能比较慢的操作。
Swift 4 中为了优化性能引入了 COW Existential Containers，这里的 COW 就代表 "Copy-On-Write"，当存在多个相同的值时，他们会共用 buffer 上的空间，直到某个值被修改时，这个被修改的值才会被拷贝一份并分配内存空间。
## 移除未调用的协议实现
```
struct Date {
    private let secondsSinceReferenceDate: Double
}
extension Date: Equatable {
    static func ==(lhs: Date, rhs: Date) -> Bool {
        return lhs.secondsSinceReferenceDate == rhs.secondsSinceReferenceDate
    }
}
extension Date: Comparable {
    static func <(lhs: Date, rhs: Date) -> Bool {
        return lhs.secondsSinceReferenceDate < rhs.secondsSinceReferenceDate
    }
}
```
看上面例子，Date 实现了 Equatable 和 Comparable 协议。编译时如果编译器发现没有任何地方调用了对 Date 进行大小比较的方法，编译器会移除 Comparable 协议的实现，来达到减小包大小的目的。
## 减少隐式 @objc 自动推断
在项目中想把 Swift 写的 API 暴露给 Objective-C 调用，需要增加 @objc。在 Swift 3 中，编译器会在很多地方为我们隐式的加上 @objc，例如当一个类继承于 NSObject，那么这个类的所有方法都会被隐式的加上 @objc。
```
class MyClass: NSObject {
    func print() { ... } // 包含隐式的 @objc
    func show() { ... } // 包含隐式的 @objc
}
```
这样很多并不需要暴露给 Objective-C 也被加上了 @objc。大量 @objc 会导致二进制文件大小的增加。
在 Swift 4 中，隐式 @objc 自动推断只会发生在很少的当必须要使用 @objc 的情况，比如：
- 复写父类的 Objective-C 方法
- 符合一个 Objective-C 的协议
其它大多数地方必须手工显示的加上 @objc。
减少了隐式 @objc 自动推断后，Apple Music app 的包大小减少了 5.7%。
# 五、 Exclusive Access to Memory
在遍历一个 Collection 的时候可以去修改每一个元素的值，但是在遍历时如果去添加或删除一个元素就可能会引起 Crash。
例如为 MutableCollection 扩展一个 modifyEach 方法来修改每个元素的值，代码如下：
```
extension MutableCollection {
    mutating func modifyEach(_ body: (inout Element) -> ()) {
        for index in self.indices {
            body(&self[index])
        }
    }
}
```
假如在调用 modifyEach 时去删除元素：
```
var numbers = [1, 2, 3]
numbers.modifyEach { element in
    element *= 2
    numbers.removeAll()
}
```
就会在运行时 Crash。Swift 4 中引入了 Exclusive Access to Memory，使得这个错误可以在编译时被检查出来。
# 六、 兼容性
Xcode 9 中同时集成了 Swift 3.2 和 Swift 4。
- Swift 3.2 完全兼容 Swift 3.1，并会在过时的语法或函数上报告警告。
- Swift 3.2 具有 Swift 4 的一些写法，但是性能不如 Swift 4。
- Swift 3.2 和 Swift 4 可以混合编译，可以指定一部分模块用 Swift 3.2 编译，一部分用 Swift 4 编译。
- 迁移到 Swift 4 后能获得 Swift 4 所有的新特性，并且性能比 Swift 3.2 好。
总结：当 Xcode 正式版发布后，现有的 Swift 代码可以直接升级到 Swift 3.2 而不用做任何改动，后续可以再迁移到 Swift 4。或者直接迁移到 Swift 4 也可以，Swift 4 相比 Swift 3 的 API 变化还是不大的，很多第三方库都可以直接用 Swift 4 编译。Swift 1 到 2 和 Swift 2 到 3 的迁移的痛苦在 3 到 4 的迁移上已经大大改善了。
# 七、参考资料
- WWDC 2017 Session 402 《What's New in Swift》
- WWDC 2017 Session 212 《What's New in Foundation》
- WWDC 2017 Session 102 《Platforms State of the Union》
- 《Swift Language Programming (Swift 4.0)》
- [https://github.com/apple/swift-evolution](https://github.com/apple/swift-evolution)
- [https://github.com/ole/whats-new-in-swift-4](https://github.com/ole/whats-new-in-swift-4)
- [https://www.raywenderlich.com/163857/whats-new-swift-4](https://www.raywenderlich.com/163857/whats-new-swift-4)
- [https://www.hackingwithswift.com/swift4](https://www.hackingwithswift.com/swift4)
作者：刘铎Derek
链接：http://www.jianshu.com/p/f35514ae9c1a
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

