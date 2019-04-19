# 一篇文章帮你彻底了解 Swift 3.1 的新内容 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月13日 08:39:22[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：439
> 
本文翻译自 [What’s New in Swift 3.1?](https://www.raywenderlich.com/156352/whats-new-in-swift-3-1)
![](https://ww1.sinaimg.cn/large/006tNc79gy1fe3r7pr2snj30dw0dwmy1.jpg)
本文来自简书，原文地址:[http://www.jianshu.com/p/bb0bd1881a35](http://www.jianshu.com/p/bb0bd1881a35)
好消息好消息：Xcode 8.3 和 Swift 3.1 正式版的发布包含了等待已久的 [Swift 包管理器](https://github.com/apple/swift-package-manager) 功能，以及对语言本身的改进。
如果你没有一直密切关注 [Swift Evolution Process](https://github.com/apple/swift-evolution) ，那这篇文章就是为你准备的！
本文会强调 Swift 3.1 中最重大的变动，这将对我们的代码产生重大影响。来吧！:]
## 开始
Swift 3.1 和 Swift 3.0 是源码兼容的，所以如果已经使用 Edit\Convert\To Current Swift Syntax… 将项目迁移到了 Swift 3.0 的话，新功能将不会破坏我们的代码。不过，苹果在 Xcode 8.3 中已经抛弃了对 Swift 2.3 的支持。所以如果还没有从 Swift 2.3 迁移过来，现在要抓紧做了！
下面会有类似 [SE-0001] 的标签链接。这些是 Swift Evolution 的提案号。我列出了每个提案的链接，以便查看某变动完整的细节信息。我建议在 playground 上试一下我们讨论的功能，以便对于所有变动都有更好的理解。
所以，启动 Xcode，选择 File\New\Playground…. 选择 iOS 平台，随便取个名字。一边读一边在 playground 中尝试每个功能。
> 
注意：如果需要简单回顾一下 Swift 3.0 重点，看看这篇文章 [What’s
 New in Swift 3](https://www.raywenderlich.com/135655/whats-new-swift-3) 。
## 语言改进
首先，看看这次发布中的语言改进，包括数字类型的可失败初始化方法（failable initializers）、新的序列函数等等。
### 可失败数值转换初始化方法
Swift 3.1 为所有数字类型 (Int, Int8, Int16, Int32, Int64, UInt, UInt8, UInt16, UInt32, UInt64, Float, Float80, Double) 实现了可失败初始化方法，要么完全成功、不损失精度，要么返回 nil [ [SE-0080](https://github.com/apple/swift-evolution/blob/master/proposals/0080-failable-numeric-initializers.md) ]。
这个功能很实用，例如，以安全、可还原（recoverable）的方式转换来自外部的非确切类型数据。举个实际的例子，我们可能会这样处理一个学生的 JSON 数组：
```
class Student {
  let name: String
  let grade: Int
  init?(json: [String: Any]) {
    guard let name = json["name"] as? String,
          let gradeString = json["grade"] as? String,
          let gradeDouble = Double(gradeString),
          let grade = Int(exactly: gradeDouble)  // <-- 这里是 3.1 的功能
    else {
        return nil
    }
    self.name = name
    self.grade = grade
  }
}
func makeStudents(with data: Data) -> [Student] {
  guard let json = try? JSONSerialization.jsonObject(with: data, options: .allowFragments),
        let jsonArray = json as? [[String: Any]] else {
    return []
  }
  return jsonArray.flatMap(Student.init)
}
let rawStudents = "[{\"name\":\"Ray\", \"grade\":\"5.0\"}, {\"name\":\"Matt\", \"grade\":\"6\"},
                    {\"name\":\"Chris\", \"grade\":\"6.33\"}, {\"name\":\"Cosmin\", \"grade\":\"7\"}, 
                    {\"name\":\"Steven\", \"grade\":\"7.5\"}]"
let data = rawStudents.data(using: .utf8)!
let students = makeStudents(with: data)
dump(students) // [(name: "Ray", grade: 5), (name: "Matt", grade: 6), (name: "Cosmin", grade: 7)]
```
我们在 Student 类的指定可失败初始化方法中用可失败构造器将 grade 属性从 Double 转换为 Int，就像这样：
`let grade = Int(exactly: gradeDouble)`
如果 `gradeDouble` 是小数，例如 6.33，就会失败。如果可以用 Int 来表示，例如 6.0，就会成功。
> 
注意：还有一种可替代的设计是使用抛出错误的初始化方法而不是可失败的。社区选择了可失败的，因为它更好、更符合工程学设计。
### 新的序列函数
Swift 3.1 为标准库的 Sequence 协议增加了两个新函数，用于数据过滤：prefix(while:) 和 drop(while:) [ [SE-0045](https://github.com/apple/swift-evolution/blob/master/proposals/0045-scan-takewhile-dropwhile.md) ]。
考虑斐波那契无限数列：
```
let fibonacci = sequence(state: (0, 1)) {
  (state: inout (Int, Int)) -> Int? in
  defer {state = (state.1, state.0 + state.1)}
  return state.0
}
```
在 Swift 3.0 中，我们只需指定迭代次数（iteration count）即可遍历斐波那契数列：
```
// Swift 3.0
for number in fibonacci.prefix(10) {
  print(number)  // 0 1 1 2 3 5 8 13 21 34
}
```
Swift 3.1 允许我们使用 prefix(while:) 和 drop(while:) 来获取位于序列两个给定值之间所有的元素，像这样：
```
// Swift 3.1
let interval = fibonacci.prefix(while: {$0 < 1000}).drop(while: {$0 < 100})
for element in interval {
  print(element) // 144 233 377 610 987
}
```
`prefix(while:)` 返回满足某 predicate 的最长子序列。从序列的开头开始，并且在第一个从给定闭包中返回
 false 的元素处停下。
`drop(while:)` 做相反的操作：从第一个在闭包中返回 false 的元素开始，直到序列的结束，返回此子序列。
> 
注意：这种情况下可以使用尾随闭包（trailing closure）：
`let interval = fibonacci.prefix{$0 < 1000}.drop{$0 < 100}`
### Concrete Constrained Extensions
Swift 3.1 允许我们扩展具有 concrete type constraint 的泛型。之前不能扩展这样的类型，因为约束必须是一个协议。看一个例子。
例如，Ruby On Rails 提供了一个非常实用的方法 `isBlank` 用于检查用户输入。在 Swift
 3.0 中我们会将其实现为 String 扩展中的计算属性：
```
// Swift 3.0
extension String {
  var isBlank: Bool {
    return trimmingCharacters(in: .whitespaces).isEmpty
  }
}
let abc = " "
let def = "x"
abc.isBlank // true
def.isBlank // false
```
如果想要 string 可选值 也能用 `isBlank` 计算属性，在
 Swift 3.0 中要这么做：
```
// Swift 3.0
protocol StringProvider {
  var string: String {get}
}
extension String: StringProvider {
  var string: String {
    return self
  }
}
extension Optional where Wrapped: StringProvider {
  var isBlank: Bool {
    return self?.string.isBlank ?? true
  }
}
let foo: String? = nil
let bar: String? = "  "
let baz: String? = "x"
foo.isBlank // true
bar.isBlank // true
baz.isBlank // false
```
我们创建了一个 `StringProvider` 协议供 `String` 采用。当拆包类型是
 StringProvider 的时候使用它扩展 `Optional`，添加 `isBlank` 方法。
Swift 3.1 可以不用这样的协议来扩展 concrete type：
```
// Swift 3.1
extension Optional where Wrapped == String {
  var isBlank: Bool {
    return self?.isBlank ?? true
  }
}
```
与之前相同的功能，但是代码更少！
### 泛型嵌套
Swift 3.1 让我们可以混合使用泛型和类型嵌套。练习一下，看看这个（不是很难的）例子。如果某个 raywenderlich.com 的团队领导想要在博客上发一篇文章，他会找专门的开发者团队来处理这个问题，以保证文章的质量：
```
class Team<T> {
  enum TeamType {
    case swift
    case iOS
    case macOS
  }
  class BlogPost<T> {
    enum BlogPostType {
      case tutorial
      case article
    }
    let title: T
    let type: BlogPostType
    let category: TeamType
    let publishDate: Date
    init(title: T, type: BlogPostType, category: TeamType, publishDate: Date) {
      self.title = title
      self.type = type
      self.category = category
      self.publishDate = publishDate
    }
  }
  let type: TeamType
  let author: T
  let teamLead: T
  let blogPost: BlogPost<T>
  init(type: TeamType, author: T, teamLead: T, blogPost: BlogPost<T>) {
    self.type = type
    self.author = author
    self.teamLead = teamLead
    self.blogPost = blogPost
  }
}
```
我们把内部类 `BlogPost` 嵌套在对应的外部类 `Team` 中，这两个类都是泛型。目前团队在寻找已发布的教程和文章时需要这样做：
```
Team(type: .swift, author: "Cosmin Pupăză", teamLead: "Ray Fix", 
     blogPost: Team.BlogPost(title: "Pattern Matching", type: .tutorial, 
     category: .swift, publishDate: Date()))
Team(type: .swift, author: "Cosmin Pupăză", teamLead: "Ray Fix", 
     blogPost: Team.BlogPost(title: "What's New in Swift 3.1?", type: .article, 
     category: .swift, publishDate: Date()))
```
但实际上可以简化这里的代码。如果嵌套的内部类型用了外部的泛型，它就默认继承了父类的类型。因此我们不需要声明，只要这样写就可以了：
```
class Team<T> {
  // 本来的代码
  class BlogPost {
    // 本来的代码
  }  
  // 本来的代码
  let blogPost: BlogPost
  init(type: TeamType, author: T, teamLead: T, blogPost: BlogPost) {
    // 本来的代码   
  }
}
```
> 
注意：如果想学习 Swift 中的泛型，读一读这篇最近更新的教程 [getting
 started with Swift generics](https://www.raywenderlich.com/154371/swift-generics-tutorial-getting-started) 。
### Swift 版本可用性
我们可以使用 Swift 版本的 `#if swift(>= N)`静态构造器，像这样：
```
// Swift 3.0
#if swift(>=3.1)
  func intVersion(number: Double) -> Int? {
    return Int(exactly: number)
  }
#elseif swift(>=3.0)
  func intVersion(number: Double) -> Int {
    return Int(number)
  }
#endif
```
然而在使用 Swift 标准库这样的东西时，这种方法有一个很大的缺点。它需要为每个旧语言版本编译标准库。因为如果要使用 Swift 3.0 的行为，则需要使用针对该版本编译的标准库。如果使用 3.1 版本的标准库，就根本没有正确的代码。
所以，Swift 3.1 扩展了 `@available` 属性 [ [SE-0141](https://github.com/apple/swift-evolution/blob/master/proposals/0141-available-by-swift-version.md) ]：
```
// Swift 3.1
@available(swift 3.1)
func intVersion(number: Double) -> Int? {
  return Int(exactly: number)
}
@available(swift, introduced: 3.0, obsoleted: 3.1)
func intVersion(number: Double) -> Int {
  return Int(number)
}
```
这个新功能与 intVersion 方法相同。但是，它只允许像标准库这样的库被编译一次。编译器随后只要选择与对应版本兼容的功能即可。
> 
注意：如果想学习 Swift 的 availability attributes，看看这篇教程 [availability
 attributes in Swift](https://www.raywenderlich.com/139077/availability-attributes-swift)。
### 将非逃逸闭包转换为逃逸闭包
Swift 3.0 把函数的闭包参数改为默认非逃逸 [SE-0103]。但是，该提案当时并没有被完全实现。在 Swift 3.1 中，可以使用新的辅助函数 withoutActuallyEscaping() 临时转换非逃逸闭包。
为什么要这么做？虽然可能永远都不会用到，但还是考虑一下提案提到的这个例子。
```
func perform(_ f: () -> Void, simultaneouslyWith g: () -> Void,
             on queue: DispatchQueue) {
  withoutActuallyEscaping(f) { escapableF in     // 1
    withoutActuallyEscaping(g) { escapableG in
      queue.async(execute: escapableF)           // 2
      queue.async(execute: escapableG)     
      queue.sync(flags: .barrier) {}             // 3
    }                                            // 4
  }
}
```
此函数同时运行两个闭包，然后在两个都完成之后返回。
- f 和 g 一开始是非逃逸的，并被转换为 escapableF 和 escapableG。
- 调用 async(execute:) 需要逃逸闭包。很幸运，我们已经搞定了。
- 运行 sync(flags: .barrier)，以确保 async(execute:) 被全部完成了、之后不会再调用闭包。
- Scope 限制了 escapableF 和 escapableG 的使用。
如果不临时转换为逃逸闭包，就会是一个 bug。标准库未来应该能够检测到这种错误的调用。
## Swift 包管理器的更新
啊啊啊啊，期待已久的 Swift 包管理器终于更新了！
### Editable Packages
Swift 3.1 在 [Swift 包管理器](https://github.com/apple/swift-package-manager) 中新增了 Editable Packages 概念
 [ [SE-0082](https://github.com/apple/swift-package-manager/blob/master/Documentation/Usage.md#editable-packages) ]。
`swift package edit` 命令可以将现有包转换为可编辑的。可编辑的包将会替换 dependency
 graph 中的规范包。使用 `—end-edit` 命令将包管理器还原回规范解析的包。
### Version Pinning
Swift 3.1 在特定版本的 [Swift 包管理器](https://github.com/apple/swift-package-manager) 中新增了 version pinning 概念
 [ [SE-0145](https://github.com/apple/swift-package-manager/blob/master/Documentation/Usage.md#package-pinning) ]。 `pin` 命令会像这样固定一个或多个依赖：
```
$ swift package pin --all      // pin 所有依赖
$ swift package pin Foo        // 把 Foo pin 在当前解析版本 
$ swift package pin Foo --version 1.2.3  // 把 Foo pin 在 1.2.3
```
使用 `unpin` 命令恢复到以前的包版本：
```
$ swift package unpin —all
$ swift package unpin Foo
```
包管理器在 Package.pins 中存储每个包的活跃版本 pin 信息。如果文件不存在，包管理器则会按照包 manifest 中指定的要求自动创建该文件，这是 automatic pinning 过程的一部分。
### 其它
`swift package reset` 命令将包重置为干净状态，不会检出当前的依赖关系或 build
 artifact。
还有，使用 `swift test --parallel` 命令并行执行测试。
## 杂项
Swift 3.1 还有一些并不属于以上某个分类的改动：
### 多重返回函数
返回两次的 C 函数（如 vfork 和 setjmp ）将无法继续使用。它们以可笑的方式改变了程序的控制流程。所以 Swift 社区已经决定禁止使用它们，现在直接会导致编译时错误。
### 禁用 Auto-Linking
[Swift 包管理器](https://github.com/apple/swift-package-manager) 禁用了 C 语言 target module maps 的 auto-linking 功能：
```
// Swift 3.0
module MyCLib {
    header “foo.h"
    link “MyCLib"
    export *
}
// Swift 3.1
module MyCLib {
    header “foo.h”
    export *
}
```
## 下一步？
Swift 3.1 优化了一些 Swift 3.0 的功能，为将在今年推出的 Swift 4.0 中捆绑的重大改动做准备。包括对泛型的巨大改进，正则表达式，更符合工程学的字符串设计等等。
如果你喜欢挑战，可以看看 [Swift standard library diffs](https://developer.apple.com/reference/swift?changes=latest_minor) ，或者查看官方的 [Swift
 CHANGELOG](https://github.com/apple/swift/blob/master/CHANGELOG.md) ，可以在其中阅读有关所有改动的更多信息。或者可以用它来了解 Swift 4.0 中的内容！
如果好奇 Swift 4 及更高版本中的改动，可以去 [Swift Evolution proposals](https://github.com/apple/swift-evolution/tree/master/proposals) ，可以看到现在提案的内容。如果真的很热爱
 Swift，为什么不就目前的提案提出意见，甚至自己提出提案呢？:]
所以你现在喜不喜欢 Swift 3.1？欢迎在下方评论！
