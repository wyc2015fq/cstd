# Java Nashorn--Part 6 - weixin_33985507的博客 - CSDN博客
2017年02月17日 06:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
### Nashorn 的 JavaScript 语言的扩展
正如我们所讨论的，Nashorn 是一个完全符合 ECMAScript 5.1 的实现。然而除此之外，Nashorn 还实现了很多 JavaScript 语言的语法扩展，开发人员使用起来更加方便和容易。这些扩展被 JavaScript 开发者所熟悉，并且相当多的人重复扩展的 JavaScript 的 Mozilla 的方言版本，让我们来看看几个最常见的有用的扩展。
### Foreach 循环
标准的 JavaScript 并没有等同于 Java 语言中的 foreach 循环，但是
Nashorn 实现了 Mozilla 方言版本的 foreach 循环体：
```
var jsEngs = [ "Nashorn", "Rhino", "V8", "IonMonkey", "Nitro" ];
for each (js in jsEngs) {
    print(js);
}
```
### 单一功能的表达式
Nashorn 还支持另一个小的语法强化，如果一个函数（命名的或匿名的）只包含一个表达式，那么括号和返回语句可以省略。在下面的例子中，cube() 和cube2() 是完全等效的，但 cube() 通常是不合法的
JavaScript 语法：
```
function cube(x) x*x*x;
function cube2(x) {
    return x*x*x;
}
print(cube(3));
print(cube2(3));
```
### 支持多个 catch 子句
JavaScript 只支持简单的 try、catch 和 throw 的异常处理。
> 
注意：
在 JavaScrip t中，所有的异常都是 unchecked 类型的异常。
标准的 JavaScrip t只允许一个 catch 子句，不支持处理不同类型异常的 catch 子句。幸运的是，已经有一个现有的 Mozilla 的语法扩展提供这个功能，Nashorn 具有很好的支持，如下所示：
```
function fnThatMightThrow() {
    if (Math.random() < 0.5) {
        throw new TypeError();
    } else {
        throw new Error();
    }
}
try {
    fnThatMightThrow();
} catch (e if e instanceof TypeError) {
    print("Caught TypeError");
} catch (e) {
    print("Caught some other error");
}
```
### 具体实现细节
我们以前讨论过，Nashorn 直接编译 JavaScript 程序生成 JVM 字节码，然后就像其他类一样运行它们。例如，lambda 表达式可以作为
JavaScript 函数简单明了的表现，以及它们之间的简单的操作性。
让我们看下以前的例子，来看一下是如何用函数作为 Java 接口的匿名实现的。
```
jjs> var clz = Java.type("java.util.concurrent.Callable");
jjs> var obj = new clz(function () { print("Foo"); } );
jjs> print(obj);
jdk.nashorn.javaadapters.java.util.concurrent.Callable@290dbf45
```
这意味着实现 Callable 接口的具体实现的 JavaScript 对象的类型为jdk.nashorn.javaadapters.java.util.concurrent.Callable，这个类并不是 Nashorn 附带的，反而，Nashorn 实现不管什么样的接口，动态转换成字节码，只要求维护原始的名字作为包结构的一部分，看起来更加具有可读性。
> 
注意：
动态代码生成是 Nashorn 必备的部分，Nashorn 编译所有所有的JavaScript 代码生成 Java 字节码，但从来不会解释代码。
由于 Nashorn 是完全遵从 ECMAScript 文档规范，所以有些时候实现的能力就会有所受限，像下面的例子，考虑打印出的对象是什么：
```
jjs> var obj = {foo:"bar",cat:2};
jjs> print(obj);
[object Object]
```
ECMAScript 说明要求输出的结果为 [object Object]，而不允许提供更多有用的细节（例如，obj 里所有元素的属性和值）。
