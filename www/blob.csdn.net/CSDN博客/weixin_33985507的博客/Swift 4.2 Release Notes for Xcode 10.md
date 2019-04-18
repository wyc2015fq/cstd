# Swift 4.2 Release Notes for Xcode 10 - weixin_33985507的博客 - CSDN博客
2018年12月03日 16:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：24
##### 概览
在`Xcode 10`中编辑`Swift`时使用`Swift 4.2`
##### `Swift Complier`
###### 新功能
- `Swift 4.2`，在某些情况下，从可选类型到泛型类型的强制转换的行为已经改变。 例如
```
struct ConditionalCast<T> {
    func cast(value: Any?) -> T? {
        return value as? T
    }
}
```
如果`value`为`nil`且类型`T`在运行时是可选类型，则返回值现在为`.some（nil）`而不是`nil`。 当使用具体类型而不是泛型类型时，此行为与转换行为一致
- `macOS Linux`的`i386`和x86_64`的C long double`类型引入`Float80` 。`Darwin`和`glibc`模块中的`tgmath`函数现在支持`Float80`以及`Float Double`。 几个`tgmath`函数已经在`BinaryFloatingPoint`和`FloatingPoint`上变得通用，因此它们可以自动用于任何符合标准的类型。
- 如果强制转换中的类型不可用或已弃用，则不再将包含强制转换的`C`宏导入`Swift`，或者在引用时生成其他诊断.这些宏仅在非常有限的情况下使用非常简单的值导入，因此这种更改不太可能影响代码
- 为了防止`Cocoa`对象不一致的哈希值，重载`NSObject`属性上的`hashValue`现在会产生编译警告，而`hash（into :)）`现在被标记为不可重载。要自定义`hashing`,需要重载`hash`属性，而不是`hash Value` ，如下：
```
class Person: NSObject {
    let name: String
 
    init(name: String) {
        self.name = name
        super.init()
    }
 
    override func isEqual(_ other: Any?) -> Bool {
        guard let other = other as? Person else { return false }
        return other.name == self.name
    }
 
    override var hash: Int {
        var hasher = Hasher()
        hasher.combine(name)
        return hasher.finalize()
    }
}
```
注意：`hashing`和`equality`相辅相成，重载`hash`,就要重载`isEqual :`,反之亦然
- 关于重叠访问的`Swift 4.1`警告现在是`Swift 4`模式中的错误。当修改变量的通用变异方法传递一个从同一变量读取的非转义闭包时，最常出现这些新错误。 之前使用警告编译的代码需要更新。
- `Swift`编译器默认使用新的编译策略，可以大大加快调试构建。 此策略允许每个编译作业处理一批文件而不是单个文件。
通过选择在调试模式下使用整个模块优化进行构建（在`-Onone`）加速调试构建的项目应该尝试新的编译样式。 为此，请为`Compilation Mode`构建设置选择`Incremental`选项，并确保项目未使用调试版本的旧整个模块设置，例如`SWIFT_WHOLE_MODULE_OPTIMIZATION = YES`或`SWIFT_OPTIMIZATION_LEVEL = -Owholemodule`。
新的编译策略在构建产品方面应该是透明的。 但是，如果它导致问题，您可以使用自定义构建设置`SWIFT_ENABLE_BATCH_MODE = NO`恢复到以前的单个文件策略。
###### 已知问题
- 如果使用在类实例上的协议或扩展中定义的`nonmutating`设置间接修改属性，则`Swift`编译器可能会在调用`getter`之后通过释放实例来错误编译它。 这可能会导致程序崩溃或在运行时出现不可预测的行为。例如：
```
protocol SomeProtocol { }
class SomeClass: SomeProtocol { }
extension SomeProtocol {
    var someNonmutatingProperty: CGPoint {
        nonmutating set { _ = self }
        get { return .zero }
    }
}
// Might be miscompiled, deallocating SomeClass() too early.
SomeClass().someNonmutatingProperty.x = 42
```
解决方案：将操作分解为多个语句，以便`get`和`set`操作在不同的语句中发生：
```
let someObject = SomeClass()
// First get the nonmutating property value.
var temp = someObject.someNonmutatingProperty
temp.x = 42
// Then modify it.
someObject.someNonmutatingProperty = temp
```
- 将具有函数类型作为参数的泛型类的`let`属性传递给另一个函数或方法可能会导致编译器崩溃
解决方案：将属性分配给局部变量，并将局部变量作为参数传递。
```
class A<B> {
    let function: (B) -> B
}
 
func takeFunction(_: (Int) -> Int) {}
 
func passFunction(from a: A<Int>) {
    // Reference to a.function as an argument here may crash
    // the compiler.
    takeFunction(a.function)
 
    // Workaround: assign to a local variable, and pass the
    // local variable instead.
    let function = a.function
    takeFunction(function)
}
```
- 数组由不同的类对象形成`AnyObject`数组时可能崩溃或编译错误
```
func f(_: [AnyObject])
f([NSObject.self, NSString.self]) // May crash or miscompile.
```
解决方案：单独将类对象分配给`AnyObject`类型的变量，然后形成这些变量的数组：
```
let myNSObject: AnyObject = NSObject.self
let myNSString: AnyObject = NSString.self
 
f([myNSObject, myNSString])
```
- 尝试将超类方法引用分配给可选函数类型的变量时，`Swift`编译器可能会崩溃
```
class X: NSObject {
    var f: ((Any?) -> Bool)?
    func setF() {
        f = super.isEqual(to:) // May crash the compiler.
    }
}
```
解决方案：首先将闭包分配给非可选变量，然后将非可选变量分配给可选变量。
```
class X: NSObject {
    var f: ((Any?) -> Bool)?
    func setF() {
        let f2: (Any?) -> Bool = super.isEqual(to:)
        // Work around compiler crash.
        f = f2
    }
}
```
- 编译可能会失败而不显示导致失败的错误。 例如，可能看到`Command CompileSwiftSources failed with a nonzero exit code`而没有伴随的失败原因
解决方案：通过添加名为`SWIFT_ENABLE_BATCH_MODE`的用户定义的`build setting`来禁用批处理模式，并将其设置为`NO`。
- 调用在协议类型的值上返回`Self`的`mutating`方法可能会导致编译器崩溃
```
protocol Example {
    mutating func test() -> Self
}
func foo(x: inout Example) {
    _ = x.test() // May crash the compiler.
}
```
解决方案：如果可能，使方法不变，或让方法返回`Void`或协议类型而不是`Self`：
```
protocol Example {
    mutating func test() -> Example // Instead of Self.
}
func foo(x: inout Example) {
    _ = x.test()
}
```
如果无法做到这一点，请在协议扩展中添加一个包装器方法，该方法调用`Self-returning`方法并返回协议类型：
```
extension Example {
    mutating func testWorkaround() -> Example {
        return test()
    }
}
func foo(x: inout Example) {
    _ = x.testWorkaround() // Invoke testWorkaround instead of test to avoid compiler crash.
}
```
###### 解决问题
- 由`Swift`调用的`__attribute __（（noescape）`注释的块参数的`Objective-C`方法，不再引发一个不正确的“关闭参数，因为`@noescape`传递给`Objective-C`已经转义”运行时错误。
- `Command Line Tools`包中附带的`Swift REPL`现在可以成功导入依赖于`Objective-C`框架的`Objective-C`框架或`Swift`框架。
- 在运行时对`AnyHashable`的通用转换得到改进。 例如，使用`init（uniqueKeysWithValues :)`初始化程序创建字典更稳定，因为您传递的密钥使用改进的转换行为隐式转换为`AnyHashable`
- 切换`@objc`枚举时，如果该值不是已识别的情况之一且`switch`没有默认情况，则程序将在运行时产生陷阱。
- `Swift REPL`现在成功导入框架。
- 在某些情况下，在协议类型的值的属性之间进行分配不再会引发进出的排他性错误。
- 可用性条件的运行时稳定性`（if #available）`得到改进。
- 与无条件协议不同，条件一致性不再意味着与任何继承协议的一致性。 这是因为如果继承的协议可以使用较少的限制性要求，则两个协议之间的最佳条件要求可能不同。 例如：
```
protocol Base {}
protocol Sub: Base {}
struct Generic<Param> {}
 
extension Generic: Base where Param: Base {}
extension Generic: Sub where Param: Sub {}
```
第一个扩展必须存在，因为第二个扩展并不意味着`Generic`与`Base`的一致性（如果确实如此，它将暗示它具有与`Sub`一致性相同的条件要求，即`Param：Sub`）。 这尤其会影响`Hashable：Equatable`和`BidirectionalCollection：Collection`等层次结构，并有助于在符合它们的类型范围内实现精度
- 现在可以使用`OS`可用性信息正确地注释`Swift`编译器生成的标头中的属性。
- 编译器现在可以正确处理`Codable`类型中的嵌套类型与`Codable`类型的属性具有相同名称的情况
- 当属性满足`@objc`协议中的要求时，`getter`和`setter`的选择器现在在所有情况下都可以从协议要求中推断出来
- 编译器现在正确地发出引用不应该可访问的私有或内部类型的类型元数据的代码，例如，因为它来自另一个模块或（在非WMO模式下）源文件。用作变通方法的`-emit-public-type-metadata-accessors`编译器标志将不再可用
- 传递上下文需要非可选值的可选值时，会显示改进的错误消息。 现在，错误消息在解决问题的不同策略之间提供了明确的选择，描述了每个策略的含义，并提供了Fix-it对话框来修复代码。例如：
```
error: value of optional type 'X?' must be unwrapped to a value
of type 'X'
f(x)
  ^
note: coalesce using '??' to provide a default when the optional value
contains 'nil'
  f(x)
    ^
     ?? <#default value#>
note: force-unwrap using '!' to abort execution if the optional value
contains 'nil'
  f(x)
    ^
     !
```
访问可选值的成员时，`Fix-it`对话框包含可选链接作为潜在解决方案：
```
error: value of optional type 'X?' must be unwrapped to refer to
member 'f' of wrapped base type 'X'
  let _: Int = x.f()
               ^
note: chain the optional using '?' to access member 'f'
only for non-'nil' base values
  let _: Int = x.f()
               ^
                ?
note: force-unwrap using '!' to abort execution if the optional
value contains 'nil'
  let _: Int = x.f()
               ^
                !
```
- `os_signpost（_：dso：log：name：signpostID :)`和`os_log（_：dso：log：type：_ :)`函数已更新为没有标记的类型参数。
- `UIEdgeInsets上`的`UIEdgeInsetsZero`属性可以使用了
- 当使用以`new`，`copy`或`mutableCopy`开头的自定义选择器将`Swift`方法暴露给`Objective-C`时，它将正确返回一个拥有的`（+1）`值，而不是自动释放的`（+0）`值。 从`Objective-C`调用，如`Objective-C ARC`所预期的那样
- `NSBundle`初始化程序`bundleForClass：`现在在从泛型类继承的类上使用时返回正确的包
- `type(of :)`函数现在提供预期结果当使用键值观察来观察对象时
##### `Swift Language`
###### 新功能
- 自定义编译时警告和错误信息通过使用`#warning(_:)` 和`#error(_:)`指令产生
```
#warning("This code is incomplete.")
#if BUILD_CONFIG && OTHER_BUILD_CONFIG
      #error("BUILD_CONFIG and OTHER_BUILD_CONFIG can't both be set")
#endif
```
- 公共类现在可以具有内部所需的初始化器。 必需的初始化程序的规则是它们必须在类可以被子类化的任何地方都可用，但以前在公共类上需要的初始化程序本身被迫公开。 这种限制是在`Swift 3`中引入公开区分之前的延续。
- 作为完全实现`SE-0054`的一部分，`ImplicitlyUnwrappedOptional <T>`现在是`Optional <T>`的不可用类型。 用`!`注释的声明具有`Optional <T>`类型。 如果涉及其中一个值的表达式无法使用`Optional <T>`类型成功编译，则会隐式解包，生成类型为`T`的值。在某些情况下，这将导致以前编译的代码需要更新.
- 新的`CaseIterable`协议，有一个静态的`allCases`属性用来描述所有的cases类型。
```
enum Suit: CaseIterable {
    case spade
    case heart
    case diamond
    case club
}
 
print(Suit.allCases)
// prints [Suit.heart, Suit.club, Suit.diamond, Suit.spade]
```
- 现在可以将各种类似函数的声明标记为`@inlinable`，使其主体可用于从其他模块进行优化。`Inlinable`函数体必须仅引用公共声明，除非引用的声明标记为`@usableFromInline`。
请注意，属性本身的存在不会强制执行内联或任何其他优化，也不会对单个模块中执行的优化产生任何影响。
###### 解决问题
- 协议一致性现在能够在与类型定义相同的文件中以扩展方式合成，允许自动合成`Hashable`，`Equatable`和`Codable`的条件一致性。 例如，如果存在一个通用包装类型，当其包装类型也是`Equatable`时，它只能是`Equatable`，则编译器可以自动构造`==`方法：
```
struct Generic<Param> {
    var property: Param
}
 
extension Generic: Equatable where Param: Equatable {}
// Automatically synthesized inside the extension:
// static func ==(lhs: Generic, rhs: Generic) -> Bool {
//     return lhs.property == rhs.property
// }
```
想要尽可能精确的代码通常不应该直接有条件地符合`Codable`，而是它的两个组成协议`Encodable`和`Decodable`，否则只能（例如）解码`Generic <Param>`如果`Param`是`Encodable`除了`Decodable`，即使可能不需要`Encodable`：
```
// Unnecessarily restrictive:
extension Generic: Codable where Param: Codable {}
 
// More precise:
extension Generic: Encodable where Param: Encodable {}
extension Generic: Decodable where Param: Decodable {}
```
最后，由于`Decodable`具有初始化程序要求，因此无法在非最终类的扩展中符合`Decodable`。 这样的类需要从协议中获得任何初始化器，这意味着它们需要在类定义中。
- 现在实现了条件一致性的运行时查询。 因此，动态转换例如`as value as? P`，其中动态类型的值有条件地符合`P`，将在满足条件要求时成功。
```
protocol P {}
 
struct DoesntConform {}
struct Conforms: P {}
 
struct Generic<Param> {}
extension Generic: P where Param: P {}
 
print(Generic<DoesntConform>() as? P) // nil
print(Generic<Conforms>() as? P)
// Optional(main.Generic<main.Conforms>())
```
##### `Swift Migrator`
###### 解决问题
- `Swift 4.2`迁移器正确地更新了对全局变量引用的成员变量引用
##### `Swift Package Manager`
###### 新功能
- `generate-xcodeproj`命令有一个新的——`watch`选项，用于监视文件系统，并在需要时自动重新生成`Xcode`项目
- 用于工具版本`4.2`的`PackageDescription`清单`API`中的`swiftLanguageVersions`属性从整数数组更改为`SwiftVersion`枚举情况的数组,例如：
```
// swift-tools-version:4.2
import PackageDescription
 
let package = Package(
    // ...
    swiftLanguageVersions: [.v3, .v4]
)
```
- 包管理器现在支持使用磁盘上的路径而不是`Git URL`来声明对包的依赖性。 这需要将软件包的工具版本更新为`4.2`。
- 方案生成逻辑得到改进并生成如下方案：一种方案包含根包的所有常规和测试目标; 每个可执行目标的一个方案，包含其依赖关系与可执行目标的依赖关系相交的测试目标
- `4.2`版本中的`PackageDescription API`支持称为`system library target`的新类型目标，它将当前系统模块包功能从包移动到目标级别
- 现在使用批处理模式编译`Swift`目标
- 包管理器为包依赖关系图中使用版本3 API的清单发出弃用通知。
###### 已知问题
- 如果包图表包含基于版本化和基于版本的要求的共享依赖项，则包管理器可能会崩溃。 考虑以下示例
```
Root package:
    PackageA@master
PackageA:
    PackageB@master
    PackageC@master
PackageB:
    PackageC@1.0.0..<2.0.0
```
根据包管理器的依赖性解析规则，这应该解决为：`PackageA @ master`，`PackageB @ master`和`PackageC @ master`。 但是，由于共享依赖项`PackageC以1.0.0 .. <2.0.0`要求引用，因此可能会导致崩溃
解决方案：对共享包依赖项使用基于版本化或基于修订版的要求。
##### `Swift Standard Library`
###### 新功能
- `Range`现在有条件地是一个`RandomAccessCollection`，当它的`Bound`可以使用`SignedInteger`步长进行`Strideable`时。 这意味着您可以在满足这些条件时使用`Range`作为集合。
`CountableRange`现在是`Range`的受约束泛型类型别名，而不是独立类型。 这意味着您仍然可以使用与以前相同的方式使用`CountableRange：`扩展它，将其作为函数的参数，或声明该类型的变量。 但您现在也可以互换使用范围和可数范围：
```
let countableRange: CountableRange<Int> = 0..<10
var range: Range = countableRange
 
func f<T: SignedInteger>(_ countableRange: CountableRange<T>) { }
f(range)
```
`ClosedRange`和`CountableClosedRange`分别与`Range`和`CountableRange`具有相同的行为。
- 标准库类型`Optional`，`Array`，`ArraySlice`，`ContiguousArray`，`Dictionary`，`DictionaryLiteral`，`Range`和`ClosedRange`现在符合`Hashable`协议，当它们的元素或绑定类型（视情况而定）符合`AnyHashable`时。 这使得合成的`Hashable`实现可用于包含这些类型的存储属性的类型。
- 标准库现在使用高质量，随机种子的通用散列函数，由新的`Hasher`结构体表示。随机种子会在每次执行程序时改变`hashValue`的结果，从而提高标准库的散列集合`Set`和`Dictionary`的可靠性。特别是，随机种子可以更好地防止（意外或故意）散列泛滥攻击。
作为随机播种的结果，集合和字典中的元素在每次执行时可能具有不同的顺序。这可能会暴露您现有代码中的一些错误，这些错误会偶然依赖于可重复排序。
此外，`Hashable`协议现在包括额外的功能要求，`hash（into :)`。新需求旨在比旧的`hashValue`属性更容易实现，并且它通常提供更好的散列。要实现`hash（into :)`，请将您在`Equatable`协议的`==`实现中比较的类型的完全相同的组件提供给提供的`Hasher：`
```
struct Foo: Hashable {
    var a: String?
    var b: [Int]
    var c: [String: Int]
 
    static func ==(lhs: Foo, rhs: Foo) -> Bool {
        return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c
    }
 
    func hash(into hasher: inout Hasher) {
        hasher.combine(a)
        hasher.combine(b)
        hasher.combine(c)
    }
}
```
`Hashable`的自动合成已更新，以生成散列`(into:)`实现。例如，上面的`==`和`hash（into :)`实现等同于编译器合成的实现，并且可以在不改变代码含义的情况下删除。
还扩展了`Synthesis`以支持从`hash（into:)`导出`hashValue`，反之亦然。因此，仅实现`hashValue`的代码在`Swift 4.2`中继续有效。这个新的编译器功能适用于所有可以实现`Hashable`的类型，包括类。
请注意，这些更改不会影响`Foundation`的散列界面。子类`NSObject`的类应该覆盖`hash`属性，就像之前一样。
在某些受控环境中，例如在运行特定测试时，选择性地禁用散列种子随机化可能会有所帮助，这样散列值和集合和字典值中元素的顺序在执行期间保持一致。可以通过定义值为1的环境变量`SWIFT_DETERMINISTIC_HASHING`来禁用散列种子随机化`.Swift`运行时在进程启动期间查看此变量，如果已定义，则使用常量值替换随机种子。
- 标准库现在有一个随机生成器`API`。 `Swift`标准库现在支持生成和使用随机数。`Swift`集合现在支持`shuffle（）`和`shuffled（）`操作以及选择`randomElement（）`。 数字类型现在可以在一个范围内生成统一值，例如：`Int.random（in：1 ... 10）`或`Double.random（in：0 .. <100）`，`Bool`提供随机的`true`或`false`方法 ：`random（）`。
`RandomNumberGenerator`协议已被引入作为驱动这些方法的随机源，并为每个平台提供默认实现。 这个实现包装了`arc4random_buf（_：_ :)`。
- `Random.default`重命名为`SystemRandomNumberGenerator`，以避免滥用并符合命名准则。
###### 解决问题
- 浮点数的`description`和`debugDescription`属性的行为使用的新算法比以前的代码更准确，速度更快。 特别是，现在保证每个`Double`值`d`的`Double（String（d））== d`。 以前这些无条件地打印固定数量的十进制数字（分别为`Double`和`15`和`17`）。 现在，它们打印的数字与生成的字符串所需的数字一样多，可以转换回原始源值，而不会更多
