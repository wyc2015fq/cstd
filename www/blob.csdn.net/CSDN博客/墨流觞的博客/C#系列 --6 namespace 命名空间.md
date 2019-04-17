# C#系列 --6 namespace 命名空间 - 墨流觞的博客 - CSDN博客





2018年11月02日 19:45:16[墨氲](https://me.csdn.net/dss_dssssd)阅读数：173








## 表达式
- if switch
- 三元操作符 ?:
- while  do…while  for foreach

### switch:

```
static void ShowCard (int cardNumber)
{
    switch (cardNumber)
    {
    case 13:
        Console.WriteLine ("King");
        break;
    case 12:
        Console.WriteLine ("Queen");
        break;
    case 11:
        Console.WriteLine ("Jack");
        break;
    case -1:                // Joker is -1
        goto case 12;           // In this game joker counts as queen
    default:              // Executes for any other cardNumber
    Console.WriteLine (cardNumber);
        break;
    }
}
```

在每个case语句的末尾，需要明确给出程序下一步的执行情况，否则将顺序的执行下去，此时需要使用一些跳转语句。以下是一些常见的选择：
- `break` 跳出switch语句
- `goto case x` 跳到另一个case语句
- `goto default`  调到default语句
- 另外一些跳转语句声明： `return, throw, continue, goto label`

当多个case执行同一条语句时，可以顺序的将各种case罗列出来，这个和c++相同：

```
switch (cardNumber)
{
case 13:
case 12:
case 11:
    Console.WriteLine ("Face card");
    break;
default:
    Console.WriteLine ("Plain card");
    break;
}
```

`for,while, do-while`和 C++相同，主要讲以下foreach

### foreach

从头到尾迭代循环可枚举类型中的每一个元素。在C#和.NET框架中，表示元素集合或列表的大多数类型都是可枚举的，比如数组和字符串。

```
foreach (char c in "beer")
    Console.WriteLine(c);
```

## Jump statements 跳转语句
- break
- continue
- goto
- return
- throw

#### goto

goto 语句会将程序执行转移到语句块的另一个标签中。

形式：`goto statement-label;`

或者与`switch`语句一起使用，

`goto case case-constant; // (Only works with constants, not patterns)`

```
int i = 1;
startLoop:
if (i <= 5)
{
    Console.Write (i + " ");
    i++;
goto startLoop;
}
```

#### throw

抛出异常时使用

## Namespaces 命名空间

**这个地方需要知道，因为在大多数代码工程中，作者都会定义很多命名空间**

这个命名空间和C++中的差不多，

命名空间是类型(type)名称的域。类型通常被组织成层次化的命名空间，使它们更容易查找和避免冲突。比如RSA ,存在于下列的命名空间中：

`System.Security.Cryptography`

想用调用RSA类型，需要使用完整的命名空间才能引用，

比如：调用RSA的create()  方法

```
System.Security.Cryptography.RSA rsa =
    System.Security.Cryptography.RSA.Create();
```

注意：
- 命名空间独立于程序集（assemblies），程序集是部署单元，比如.exe 和 .dll文件
- 命名空间不会影响成员的可见性： public， private, protected, internal

namespace为程序块中的类型定义了一个新的命名空间，

比如：

```
namespace Outer.Middle.Inner
{
    class Class1 {}
    class Class2 {}
}
```

namespace中的点（dot  .）表示嵌套命名空间的层次结构，比如上述代码与下列代码等效：

```
namespace Outer
{
    namespace Middle
    {
        namespace Inner
        {
            class Class1 {}
            class Class2 {}
        }
    }
}
```

可以使用全限定名（fully qualified name）来引用一个类型， 即包含从外到内的namespace,

例如引用Class1需要使用：`uter.Middle.Inner.Class1`

那些没有定义在任何命名空间，据说存在于`global`的命名空间中。global namespace也包含顶级的命名空间，比如在我们例子中的`Outer`

### using 关键字

using指令会引入（import）一个命名空间，能允许你不用通过全限定名使用类型。

如下列的代码：

```
using Outer.Middle.Inner;
class Test
{
    static void Main()
    {
        Class1 c; // Don't need fully qualified name
    }
}
```

注意：在不同的命名空间里定义相同的类型名称是合法的，但不建议这样做。

## Rules Within a Namespace 命名空间中的一些规则

### Name scoping

在外部命名空间中定义的类型，可以在内部命名空间中使用，而不需要使用限定名。

比如在Inner命名空间中引用Outer中的Class1类型，不需要使用限定名。

```
namespace Outer
{
    class Class1 {}
    namespace Inner
    {
        // Class2 继承于 Class1
        class Class2 : Class1 {}
    }
}
```

引用同一命名空间内不同分支命名空间内的类型，可以使用部分限定名。

比如MyTradingCompany下有ManagementReporting和Common两个命名空间，

在ManagementReporting中引用Common中的ReportBase，可以使用部分限定名：Common.ReportBase

```
namespace MyTradingCompany
{
    namespace Common
    {
        class ReportBase {}
    }
    namespace ManagementReporting
    {
        class SalesReport : Common.ReportBase {}
    }
}
```

### Name hiding 名称屏蔽

如果在内部和外部命名空间出现相同的类型名字，内部的类型名会屏蔽掉外部的类型名。如果想引用外部的类型名，需要使用限定名来指定。

```
namespace Outer
{
    class Foo { }
    namespace Inner
    {
        class Foo { }
        class Test
        {
            //屏蔽掉了Outer.Foo
            Foo f1; // = Outer.Inner.Foo
            // 使用  Outer.Foo来指定使用外部的类型名
            Outer.Foo f2; // = Outer.Foo
        }
    }
}
```

注意：在编译时所有的类型名都会被编译为**全限定名**

### Repeated namespaces 重复的命名空间

只要命名空间中的类型名称不同，完全可以定义相同的命名空间

```
namespace Outer.Middle.Inner
{
    class Class1 {}
}
namespace Outer.Middle.Inner
{

    class Class2 {}
}
```

### Nested using directive

在命名空间中使用usig关键字后，可以在此命名空间中使用using引入的所有的类型名，而不需要使用限定名。但是在其他的命名空间，即使命名空间相同，也不能直接使用。

比如下面的例子：

```
namespace N1
{
    class Class1 {}
}
namespace N2
{
        using N1;
    class Class2 : Class1 {}
}
namespace N2
{
    class Class3 : Class1 {} // Compile-time error
}
```

### Aliasing Types and Namespaces 别名类型和名称空间

有时，引入命名空间会导致类型名冲突，比如在两个命名空间中有相同的类型名。这时，可以不用引进全部的命名空间，而只是引入特定的类型名，并赋予一个简单的别名（alias）,之后可以通过别名引用引入的类型名。

例如：

```
using PropertyInfo2 = System.Reflection.PropertyInfo;
class Program { PropertyInfo2 p; }
```



