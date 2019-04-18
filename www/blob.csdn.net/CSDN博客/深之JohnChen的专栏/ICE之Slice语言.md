# ICE之Slice语言 - 深之JohnChen的专栏 - CSDN博客

2019年04月07日 17:22:46[byxdaz](https://me.csdn.net/byxdaz)阅读数：39


## Slice 语言定义

![](https://upload-images.jianshu.io/upload_images/5804015-48cbed9efec7d08d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/678/format/webp)

文档中对于slice的定义

在这里，我们提供Slice语言。 Slice（Specification Language for Ice）是一种将对象接口与其实现分离的基本抽象机制。Slice在客户端和服务器之间建立一个描述应用程序使用的类型和对象接口的合同。该描述与实现语言无关，因此客户端是否以与服务器相同的语言编写并不重要。

即使尽管Slice是一个首字母缩略词，它的发音是一个音节，像a slice of bread

Slice定义是由编译器为特定的实现语言编译的。编译器将与语言无关的定义转换为特定语言的类型定义和api。开发人员使用这些类型和API来提供应用程序功能并与Ice进行交互，用于各种实现语言的翻译算法被称为语言映射。目前，Ice定义了C ++，Java，C＃，Python，Objective-C，Ruby和PHP的语言映射。

因为Slice描述的是接口和类型(但不是实现)，它是纯粹的声明性语言;不能在Slice中编写可执行的语句。

Slice定义侧重于对象接口，这些接口支持的操作以及可能由操作引发的异常。此外，Slice还提供了对象持久化的特性。这需要相当多的支持机制；特别是，大部分的Slice内容都涉及到数据类型的定义。这是因为只有在Slice中定义了类型，才能在客户端和服务器之间交换数据。您不能在客户端和服务器之间交换任意的C ++数据，因为它会破坏Ice的语言独立性。但是，您总是可以创建一个Slice类型的定义，它对应于您想要发送的c++数据，然后可以传输Slice类型。

Slice 语言的作用如图

![](https://upload-images.jianshu.io/upload_images/5804015-e2f0c053d8c82a79.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/583/format/webp)

文档原图

## Slice 源文件 Slice Source Files

**文件命名**

包含Slice定义的文件必须以 .ice结尾。例如，clock.ice是一个有效的文件名。其他文件扩展被编译器拒绝。

对于不区分大小写的文件系统(如DOS)，可以将文件扩展名写成大写或小写，因此Clock.ICE是合法的。对于区分大小写的文件系统(如Unix)，Clock.ICE是非法的。(扩展必须是小写的。)

**文件格式**

Slice是一种自由格式的语言，因此你可以使用空格、横向和纵向制表符、换行符安排你的代码布局。Slice中语义和定义的布局没有关联。

Slice文件可以是ASCII文本文件，或者在每个文件的开头用字节顺序标记(BOM)使用utf - 8字符编码。但是，Slice标识符仅限于ASCII字母和数字;非ASCII字母只能出现在注释和字符串文字中。

**预处理**

Slice支持与c++相同的预处理指令，因此您可以使用诸如# include和宏定义之类的指令。但是，Slice许可证#只包含在文件开头的指令，在任何切片定义之前。

如果你使用# include指令，那么保护他们，以防止双重包含文件是一个好主意:

![](https://upload-images.jianshu.io/upload_images/5804015-0ccc528ec905a01d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/467/format/webp)

用#pragma也可以得到相同的结果

![](https://upload-images.jianshu.io/upload_images/5804015-2037c3788083da6f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/475/format/webp)

**检测Ice 版本**

在在Ice 3.5中，Slice编译器定义了预处理器宏__ICE_VERSION__，可以用来实现Ice版本的兼容

![](https://upload-images.jianshu.io/upload_images/5804015-e90f835b6198e17d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/691/format/webp)

**检测Slice编译器**

在Ice 3.5中，每个Slice编译器定义它自己的宏，这样您就可以为某些语言映射定制您的Slice代码。下面的宏由各自的编译器定义:

![](https://upload-images.jianshu.io/upload_images/5804015-138018fc428d9faa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/130/format/webp)

例如 .NET开发人员可能会选择避免对结构成员使用默认值，因为默认值的存在会改变结构到类的结构的c#映射:

![](https://upload-images.jianshu.io/upload_images/5804015-46a49e55a9dadf70.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/639/format/webp)

## 词法规则 Lexical Rules

**注释**(支持c和c++两种风格的注释)

![](https://upload-images.jianshu.io/upload_images/5804015-974746247440b107.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/624/format/webp)

**关键字**

Slice使用一些关键字，必须用小写字母拼写。例如，class和dictionary是关键字，必须这样拼写。这个小写规则有两个例外:Object和LocalObject是关键字，必须大写。

**标识符**

标识符以字母字符开头，后面跟着任意数量的字母字符或数字。在标识符中也允许有以下限制:

标识符不能以下划线开始或结束

标识符不能包含多个连续下划线

标识符get_account_name是合法的，但这些是非法的 _account、account_或get__account。

Slice标识符仅限于字母字符的ASCII范围，不能包含非英语字母，如Å。(支持非ascii标识符将使将Slice映射到缺少该特性支持的目标语言非常困难。)

**大小写敏感**

标识符是大小写不敏感的，但是必须保持拼写一致。例如TimeOfDay和TIMEOFDAY在同一个命名空间中是一样的。但是，Slice强制拼写一致。一旦你定义了一个标识符之后，你必须自始至终的拼写这个标识符的大小写，否则，编译器会认为这是非法的标识符。这条规则之所以存在，是因为这样就允许Slice可以映射到大小写敏感的语言也能映射到大小写不敏感的语言。

**是关键字的标识符**

你可以使用其他语言的关键字来定义Slice标示符，例如，switch可以用来作为Slice标识符，但是也是Java和 C plus plus 的关键字。Slice语言映射中针对每一种语言都定义了映射规则来处理这样的标识符。例如，Slice把switch映射为 C plus plus 的_cpp_stitch和Java的_switch。但不建议这样使用，您应该至少尝试避免常见的关键字。Slice标识符如self、import和while绝对不是一个好主意。

**转义的标识符**

可以使用Slice关键字作为标识符，通过使用反斜杠来预先确定关键字:(但也不推荐这样使用，应尽量避免)

![](https://upload-images.jianshu.io/upload_images/5804015-2d135838a0fe1938.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/573/format/webp)

**保留的标识符**

Slice将Ice以及以Ice开始的所有标识符作为保留的标识符。例如，Icecream将会被认为是非法的标识符。

同时，Slice还将以以下标识符为后缀的标识符视为保留的标识符：

Helper

Hodler

Prx

Ptr

保留它们，主要是为了防止在生成代码时发生冲突。

## 模块Modules

Slice提供模块来保持整体结构的条理性

![](https://upload-images.jianshu.io/upload_images/5804015-a064406464742112.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/448/format/webp)

**模块是强制性的，所有定义必须在模块中 Modules are Mandatory**

![](https://upload-images.jianshu.io/upload_images/5804015-285d5bfe491376ed.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/641/format/webp)

在全局范围内的定义是被禁止的，因为它们会导致一些实现语言(例如Python，它没有真正的全局作用域)的问题。

**重启模块 Reopening Modules**

模块可以重新开放:

![](https://upload-images.jianshu.io/upload_images/5804015-55450f2a94a53c99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/504/format/webp)

重新打开的模块对于更大的项目是有用的:它们允许您在多个不同的源文件中分割一个模块的内容。这样做的好处是，当开发人员对模块的一部分进行更改时，只有依赖于更改部分的文件需要重新编译(而不是必须重新编译使用该模块的所有文件)。

**模块的映射 Module Mapping**

模块映射到每个编程语言中相应的范围构造。(例如，对于c++和c#，Slice模块映射到命名空间，而对于Java，它们映射到包。)这允许您使用适当的c++使用或Java导入声明，以避免在源代码中使用过多的标识符。

**Ice 模块 The Ice Module**

Ice运行时的api，除了少数无法用Ice表达的特定语言的调用，都是在Ice模块中定义的。换句话说，大部分的Ice API实际上表示为Slice定义。这样做的好处是，Slice定义就足以为所有受支持的语言定义Ice运行时间的API。然后，各自的语言映射规则确定每个实现语言的每个Ice API的确切形状。

## 基本数据类型 Basic Types

Slice中内置了一些基本数据类型

![](https://upload-images.jianshu.io/upload_images/5804015-5f311666dac70789.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/666/format/webp)

数据类型

所有基本类型（除了byte类型）在服务器和客户端之间传递时，数据类型都根据实际情况在变化。例如：一个long类型的值在从一台little-endian机器传递到bit-endian机器时会发生位变换。类似的，string在从EBCDIC发往ASCII时也是进行翻译的，也许string的字符的尺寸也发生了变化，因为不是所有的机器都使用8位的字符。实际上，这些变化对于程序员来说都是透明的，而且会严格按照需要来变化。

**整数类形(integer types)**

Slice提供了如下的整数类型：

short，16位

int，32位

long，64位

不过，这些类型在某些机器上会映射到更宽的原生类型。需要注意的是那些无符号类型Slice并没有提供。因为无符号类型影射到那些没有原生的无符号类型的语言相当困难，例如Java。

**浮点数类型(floating-point types)**

浮点数类型遵循IEEE的规范。如果一个实现语言么有支持IEEE的浮点数格式，Ice运行时会将浮点数值转换为原生浮点数表示。

**字符串(strings)**

Slice字符串使用Unicode字符集。唯一的不能出现在字符串中的字符就是零字符。

Slice没有null字符串的概念。这是因为null字符串很难映射到不直接支持null字符串概念的语言上，例如Python。不要设计依靠一个null字符串来表示“不在那里”的语义的接口。如果你需要表示可选的串，可以通过使用类，字符串的序列，或者一个空的字符串来表示null字符串。

**布尔类型(booleans)**

布尔类型的值只能由false和true。如果语言映射时有原生布尔类型，语言映射就会使用该类型。

**Byte类型(bytes)**

Slice定义byte是一个至少8位的，在地址空间中传递时保证不会发生变化的类型。保证不发生变化就允许交换二进制数据，而这些数据不会被篆改。其他的Slice类型都会在传递过程中会被改变表达形式。

## 自定义数据类型 User-Defined Types

![](https://upload-images.jianshu.io/upload_images/5804015-96e4158461833523.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/637/format/webp)

**枚举(Enumerations)**

![](https://upload-images.jianshu.io/upload_images/5804015-43770dd4792d2d92.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/368/format/webp)

这个定义引入了一个名为水果的类型，它本身就是一种新的类型。Slice保证枚举器的值从左到右增长，默认情况下，第一个枚举器的值为0，后续枚举器的值顺序递增。

Slice枚举器符号进入封闭命名空间范围，因此以下是非法的:

![](https://upload-images.jianshu.io/upload_images/5804015-a371c94a155fef52.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/629/format/webp)

下面的示例展示了如何从不同的范围引用枚举器

![](https://upload-images.jianshu.io/upload_images/5804015-9d4b36b4e35c03b2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/495/format/webp)

Slice不允许空的枚举。

自定义枚举器值 Custom Enumerator Values 

Slice允许你为枚举器分配自定义值

![](https://upload-images.jianshu.io/upload_images/5804015-f0cee63c37f4aefa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/556/format/webp)

自定义值必须是唯一的和非负的，并且可以引用整数类型的Slice常量。如果没有为枚举器指定自定义值，那么它的值就是大于前面的枚举器的值。在上面的例子中，橙色的值为8。

枚举值的最大值与int的最大值(2（31次方）- 1)相同。

Slice不需要将自定义枚举值声明为递增顺序:

![](https://upload-images.jianshu.io/upload_images/5804015-0530283272ab9f89.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/571/format/webp)

但是注意，当声明顺序和枚举器的数值次序之间不一致时，比较操作的行为可能会因语言映射而有所不同。因此推荐在自定义枚举值时也按照递增顺序排列

****结构(Structures)****

Slice支持包含一个或多个任意类型的命名成员的结构，包括用户定义的复杂类型。例如:

![](https://upload-images.jianshu.io/upload_images/5804015-9ecaab041d8dc30c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/476/format/webp)

结构中不能出现结构定义，例如下面的代码是错误的：

![](https://upload-images.jianshu.io/upload_images/5804015-835d967f0707932b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/559/format/webp)

这条规则适用于一般情况:类型定义不能嵌套(除了用于支持嵌套的模块)。这条规则的原因是嵌套类型定义可能难以实现一些目标语言，即使是可实现的，极大地使范围解析规则复杂化。对于规范语言，例如Slice，嵌套的类型定义是不必要的——您可以始终将上面的定义写如下(这也是风格上的清洁):

![](https://upload-images.jianshu.io/upload_images/5804015-140f076744985cda.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/580/format/webp)

您可以为具有以下类型的数据成员指定一个默认值:

An integral type (byte, short, int, long)

 A floating point type (float or double) 

string 

bool

enum

![](https://upload-images.jianshu.io/upload_images/5804015-a88c53d72bf81da4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/461/format/webp)

******序列(Sequeces)******

序列是可变长度的元素集合

![](https://upload-images.jianshu.io/upload_images/5804015-104ac5ac6c376aef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/356/format/webp)

一个序列可以是空的——也就是说，它可以不包含任何元素，或者它可以将任何数量的元素都保存到平台的内存限制中。

序列可以包含本身序列的元素。这种安排允许你创建列表的列表:

![](https://upload-images.jianshu.io/upload_images/5804015-362028a1027b9c8e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/486/format/webp)

序列用于建模各种集合，如向量、列表、队列、集合、包或树。(取决于应用程序来决定顺序是否重要;通过丢弃顺序，序列充当一个集合或包。)

使用序列来定义model中的可选字段。

![](https://upload-images.jianshu.io/upload_images/5804015-8a9a3c2dea8fd5c4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/655/format/webp)

**字典(Dictionaries)**

字典是一个从键类型到值类型的映射。

例如:

![](https://upload-images.jianshu.io/upload_images/5804015-5c0fac2ce7671beb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/515/format/webp)

上面的定义创建了一个名为employeemap的字典，它达到了从employee编号到一个包含有employee详细信息的结构的映射。

字典key和value的允许类型

字典的关键类型不需要是整数类型。例如，我们可以用下面的定义来翻译一周的日子:

![](https://upload-images.jianshu.io/upload_images/5804015-b75c77e36c8924e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/542/format/webp)

服务器实现将负责初始化该映射，并与键值对monday - montag、tuesday - dienstag等进行初始化。

字典的值类型可以是任何类型的。但是，字典的关键类型仅限于以下类型之一:

Integral types (byte, short, int, long, bool) 

string

 enum 

Structures containing only data members of legal key types

其他复杂类型，如字典和浮点类型(float和double)不能用作键类型。复杂类型是不允许的，因为它们使字典的语言映射变得更加复杂，并且浮点类型是不允许的，因为跨机器边界的值的具象变化会导致定义不明确的等式语义。

## 常量 Constants and Literals

Slice 允许你用以下类型定义常量

An integral type (bool, byte, short, int, long) 

A floating point type (float or double)

string 

enum

![](https://upload-images.jianshu.io/upload_images/5804015-976899a7c012e95c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/552/format/webp)

**布尔常量**

boolean常量只能用true和false的关键词来初始化。(你不能用0和1来表示false和true。)

**整形常量**

整数常量可以用十进制、八进制或十六进制表示。

![](https://upload-images.jianshu.io/upload_images/5804015-e60f471657af1020.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/526/format/webp)

请注意，如果将byte解释为一个数字而不是一个位模式，您可能会得到不同语言的不同结果。例如，对于c++，byte映射到unsigned char，而对于Java，byte映射到byte，这是一个符号类型。

注意，后缀表示long且无符号的常量（l,L,u,U, used by C++)是非法的:

![](https://upload-images.jianshu.io/upload_images/5804015-e4e4628d2b6ad377.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/482/format/webp)

整数的值必须在其常量类型的范围内，如内置的基本类型表中所示;否则编译器将发出诊断。

**浮点型常量**

浮点常数使用 C++ 的语法，只是你不能使用 l 或 L 后缀以指示扩展的浮点常数;然而，f 和 F 则是合法的 （但被忽略）。

![](https://upload-images.jianshu.io/upload_images/5804015-e1fdd66758246b36.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/608/format/webp)

浮点常量必须在常量类型的范围内(float或double);否则，编译器会发出诊断。

**字符型常量**

字符串文本支持与c++相同的转义序列

![](https://upload-images.jianshu.io/upload_images/5804015-7e5460b9132b6a68.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/590/format/webp)

请注意，Slice没有空字符串的概念

![](https://upload-images.jianshu.io/upload_images/5804015-0c06ddf74f96460e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/453/format/webp)

Null字符串根本不存在于Slice中，在Ice平台中的任何地方都不能作为一个字符串的合法值。这个决定的原因是，在许多编程语言中都不存在null字符串。

除了C和c++之外，许多语言使用字节数组作为内部字符串表示。在这种语言中，null字符串不存在(并且很难映射)。

**常数表达式**

常数定义也可以指另一个常数。对于两个常量来说，具有相同的Slice类型并不是必需的，但是现有常量的值必须与定义的常量类型一致

![](https://upload-images.jianshu.io/upload_images/5804015-fa67551a6580b310.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/485/format/webp)

DEFAULT_SIZE常量是合法的，因为它具有与SIZE相同的类型，并且SHORT_SIZE是合法的，因为SIZE(500)的值在Slice short类型的范围内。但是，BYTE_SIZE是非法的，因为SIZE的值超出了byte类型的范围。

作者：李大烨

链接：https://www.jianshu.com/p/fd976359b5a4

来源：简书

简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。

