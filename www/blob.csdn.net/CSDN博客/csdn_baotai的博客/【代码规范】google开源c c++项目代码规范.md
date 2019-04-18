# 【代码规范】google开源c\c++项目代码规范 - csdn_baotai的博客 - CSDN博客

2018年05月14日 00:45:58[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：2691


# google开源 C / C ++项目代码规范

# 1.头文件

每通常一个  `.cc` 文件都有一个对应的  `.h` 文件。也有一些常见例外，如单元测试代码和只包含  `main()` 函数的  `.cc` 文件。

正确使用头文件可令代码在可读性，文件大小和性能上大为改观。

下面的规则将引导你规避使用头文件时的各种陷阱。

## 1.1。自包含的头文件

TIP

头文件应该能够自给自足（自包含的，也就是可以作为第一个头文件被引入），以  `.h` 结尾。至于用来插入文本的文件，说到底它们并不是头文件，所以以应  `.inc` 结尾不允许。出分离  `-inl.h` 头文件的做法。

所有头文件要能够自给自足。换言之，用户和重构工具不需要为特别场合而包含额外的头文件。详言之，一个头文件要有  [1.2。#define保护](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/headers/#define-guard)，统计包含它所需要的其它头文件，也不要求定义任何特别的符号。

不过有一个例外，即一个文件并不是自足的，而是作为文本插入到代码某处。或者，文件内容实际上是其它头文件的特定平台（特定于平台）扩展部分，这些文件就要用  `.inc` 文件扩展名。

如果  `.h` 文件声明了一个模板或内联函数，同时也在该文件加以定义。有用凡是到这些的  `.cc` 文件，就得统统包含该头文件，否则程序可能会在构建中链接失败。不要把这些定义放到分离的  `-inl.h` 文件里（译者注：过去该规范曾提倡把定义放到-inl.h里过）。

有个例外：如果某函数模板为所有相关模板参数显式实例化，或本身就是某类的一个私有成员，它就那么定义只能在实例化该模板的  `.cc` 文件里。

## 1.2。#define保护

TIP

所有头文件都应该使用  `#define` 来防止头文件被多重包含，命名格式当是：  `<PROJECT>_<PATH>_<FILE>_H_` 。

为保证唯一性，头文件的命名应该基于所有项目源代码树的全路径。例如，项目  `foo` 中的头文件  `foo/src/bar/baz.h` 可按如下方式保护：

#ifndef FOO_BAR_BAZ_H_ #define FOO_BAR_BAZ_H_ ... #endif // FOO_BAR_BAZ_H_

## 1.3。前置声明

TIP

尽可能地避免使用前置声明。使用  `#include` 所有游戏需要的头文件即可。

**定义：**

> 所谓「前置声明」（forward declaration）是类，函数和模板的纯粹声明，没伴随着其定义。

**优点：**

> - 前置声明能够节省编译时间，的多余  `#include` 会迫使compile-器展开更多的文件，处理更多的输入。
- 前置声明能够节省不必要的重新编译的时间。  `#include` 使代码因为头文件中无关的改动而被重新编译多次。

**缺点：**

> - 
前置声明隐藏了依赖关系，头文件改动时，用户的代码会跳过必要的重新编译过程。

- 
前置声明可能会被库的后续更改所破坏。前置声明函数或模板有时会妨碍头文件开发者变动其API。例如扩大形参类型，加个自带默认参数的模板形参等等。

- 
前置声明来自命名空间  `std::` 的符号时，其行为未定义。

- 
很难判断什么时候该用前置声明，时候什么用该  `#include` 极端情况下，用前置声明代替。  `includes` 甚至都会暗暗地改变代码的含义：

> 

// bh：struct B {}; struct D ：B {}// good_user.cc： ＃包括“BH”void f （B * ）; void f （void * ）; void test （D * x ）{ f （x ）; } //调用f（B *）

> 如果  `#include` 被  `B` 状语从句：  `D` 的前置声明替代，  `test()` 就会调用  `f(void*)` 。
- 前置声明了`include` 不少来自头文件的符号时，就会比单单一行的  冗长。
- 仅仅为了能前置声明而重构代码（比如用指针成员代替对象成员）会使代码变得更慢更复杂。

**结论：**

> - 尽量避免前置声明那些定义在其他项目中的实体。
- 函数：总是使用  `#include`。
- 类模板：优先使用  `#include`。

至于什么时候包含头文件，参见  [1.5。#include的路径及顺序](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/headers/#name-and-order-of-includes)  。

## 1.4。内联函数

TIP

只有当函数只有10行甚至更少时才将其定义为内联函数。

**定义：**

> 当函数被声明为内联函数之后，编译器会将其内联展开，而不是按通常的函数调用机制进行调用。

**优点：**

> 只要内联的函数体小小，内联该函数可以令目标代码更加高效。对于存取函数以及其它函数体比较短，性​​能关键的函数，鼓励使用内联。

**缺点：**

> 滥用内联将导致程序变得更慢。内联可能使目标代码量或增或减，这取决于内联函数的大小。内联非常短小的存取函数通常会减少代码大小，但内联一个相当大的函数将戏剧性的增加代码大小。现代处理器由于更好的利用了指令缓存，小巧的代码往往执行更快。

**结论：**

> 
一个较为合理的经验准则是，不要内联超过10行的函数。谨谨对待析构函数，析构函数往往比其表面看起来要更长，因为有隐含的成员和基类析构函数被调用！

另一个实用的经验准则：内联那些包含循环或  `switch` 语句的函数常常是得不偿失（除非在大多数情况下，这些循环或  `switch` 语句从不被执行）。

有些函数即使声明为内联的也不一定会被编译器内联，这点很重要; 比如虚函数和递归函数就不会被正常内联。通常，递归函数不应该声明成内联函数。（YuleFox注：递归调用堆栈的展开并不像循环那么简单，比如递进层数在编译时可能是未知的，大多数编译器都不支持内联递归函数）。虚函数内联的主要原因是想把它的函数体放在类定义内，为了图个方便，抑或是当作文件描述其行为，比如精短的存取函数。

## 1.5。  `#include` 的路径及顺序

TIP

使用标准的头文件包含顺序可增强可读性，避免隐藏依赖：相关头文件，C库，C ++库，其他库的  .h，本项目内的  .h。

项目内部文件应按照项目源代码目录树结构排列，避免使用UNIX特殊的快捷目录  `.`（当前目录）或  `..` （上级目录）。例如，  `google-awesome-project/src/base/logging.h` 应该按如下方式包含：

#include “base / logging.h”

又如，  `dir/foo.cc` 或  `dir/foo_test.cc` 的主要作用英文的英文实现或测试  `dir2/foo2.h` 的功能，  `foo.cc` 中包含头文件的次序如下：

> - `dir2/foo2.h` （优先位置，详情如下）
- C系统文件
- C ++系统文件
- 库其他的  `.h` 文件
- 项目本。内  `.h` 文件

优先这种顺序的排序保证当  `dir2/foo2.h` 遗漏某些必要的库时，  `dir/foo.cc` 或  `dir/foo_test.cc` 的构建会立刻中止。因此这一条规则保证维护这些文件的人们首先看到构建中止的消息而不是维护其他包的人们。

`dir/foo.cc` 和  `dir2/foo2.h` 通常位于同一目录下（如  `base/basictypes_unittest.cc` 和  `base/basictypes.h`），但也可放在不同目录下。

按字母顺序分别对每种类型的头文件进行二次排序是不错的主意。注意较老的代码可不符合这条规则，要在方便的时候改正它们。

您所依赖的符号（符号）被哪些头文件所定义，您就应该包含（包括）哪些头文件，[前置声明](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/headers/#forward-declarations)  （向前声明）情况除外。您比如要用到  `bar.h` 中的某个符号，哪怕您所包含的  `foo.h` 已经包含了  `bar.h`，也照样得包含  `bar.h`，除非`foo.h` 有明确  说明它会自动向您提供  `bar.h` 中符号。不过，凡是cc文件所对应的「相关头文件」已经包含的，就不用再重复包含进其cc文件里面了，就像  `foo.cc` 只包含  `foo.h`就够了，不用再管后者所包含的其它内容。

举#include “foo / public / fooserver.h”//优先位置

#include “foo / public / bar.h”例如，  `google-awesome-project/src/foo/internal/fooserver.cc` 包含次序如下：

# 4.函数

## 4.1。参数顺序

**总述**

函数的参数顺序为：输入参数在先，后跟输出参数。

**说明**

C / C ++中的函数参数或者是函数的输入，或者是函数的输出，或兼而有之。输入参数通常是值参或  `const` 引用，输出参数或输入/输出参数则一般为非  `const` 指针。在排列参数顺序时，将所有的输入参数置于输出参数之前。特别要注意，在加入新参数时不要因为它们是新参数就置于参数列表最后，而是仍然要按照前述的规则，即将新的输入参数也置于输出参数之前。

这并非一个硬性规定。输入/输出参数（通常是类或结构体）让这个问题变得复杂。并且，有时候为了其他函数保持一致，你可能不得不不所有变通。

## 4.2。编写简短函数

**总述**

我们倾向于编写简短，凝练的函数。

**说明**

我们承认长函数有时是合理的，因此并不硬限制函数的长度。如果函数超过40行，可以思索一下能不能在不影响程序结构的前提下对其进行分割。

即使一个长函数现在工作的非常好，一旦有人对其修改，有可能出现新的问题，甚至导致难以发现的错误。使函数尽量简短，以便于他在他人阅读和修改代码。

在处理代码时，你可能会发现复杂的长函数。不要害怕修改现有代码：如果证实这些代码使用/调试起来很困难，或者你只需​​要使用其中的一小段代码，考虑将其分割为更加简短并易于管理的若干函数。

## 4.3。引用参数

**总述**

所有按引用传递的参数必须加上  `const`。

**定义**

在C语言中，如果函数需要修改变量的值，参数必须为指针，如  。在C ++中，函数还可以声明为引用参数：  。`int foo(int *pval)``int foo(int &val)`

**优点**

引用定义参数可以防止出现  `(*pval)++` 这样丑陋的代码。引用参数对于拷贝构造函数这样的应用也是必需的。同时也更明确地不接受空指针。

**缺点**

容易引起误解，因为引用在语法上是值变量却拥有指针的语义。

**结论**

函数参数列表中，所有引用参数都必须是  `const`：

void Foo （const string ＆in ，string * out ）;

事实上这在Google Code是一个硬性约定：输入参数是值参或  `const` 引用，输出参数为指针。输入参数可以是  `const` 指针，但决不能是非  `const` 引用参数，除非特殊要求，比如  `swap()`。

有时候，在输入形参中用针指   比   更明智。比如：`const T*``const T&`
- 可能会传递空指针。
- 函数要把指针或对地址的引用赋值给输入形参。

总而言之，大多时候输入形参往往是  。用若   则说明输入侧另有处理。所以若要使用  ，则应给出相应的理由，否则会使读者感到迷惑。`const T&``const T*``const T*`

## 4.4。函数重载

**总述**

若要使用函数重载，则必须能让读者一看调用点就胸有成竹，而不用花心思猜测调用的重载函数到底是哪一种。这一规则也适用于构造函数。

**定义**

你可以编写一个参数类型为   的函数，然后用另一个参数类型为   的函数对其进行重载：`const string&``const char*`

class MyClass { public ：void Analyze （const string ＆text ）; void 分析（const char * text ，size_t textlen ）; };

**优点**

通过重载参数不同的同名函数，可以令代码更直观。模板化代码需要重载，这同时也能为使用者带来便利。

**缺点**

如果函数单靠不同的参数类型而重载（acgtyrant注：这意味着参数数量不变），读者就得十分熟悉C ++五花八门的匹配规则，以了解匹配过程具体到底如何。另外，如果派生类只重载了某个函数的部分变体，继承语义就容易令人困惑。

**结论**

如果打算重载一个函数，可以试试改在函数名里加参数信息。例如，用  `AppendString()`和  `AppendInt()` 等，而不是一口气重载多个  `Append()`。如果重载函数的目的是为了支持不同数量的同一类型参数，则优先考虑使用  `std::vector` 以便使用者可以用  [列表初始化](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/others/#braced-initializer-list)指定参数。

## 4.5。缺省参数

**总述**

只允许在非虚函数中使用缺省参数，且必须保证缺省参数的值始终一致。参数缺省与  [函数重载](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/others/#function-overloading)  遵循同样的规则。一般情况下建议使用函数重载，尤其是在缺省函数带来的可读性提升不能弥补下文中所提到的缺点的情况下。

**优点**

有些函数一般情况下使用默认参数，但有时需要又使用非默认的参数。缺省参数为这样的情形提供了便利，使程序员不需要为了极少的例外情况编写大量的函数。和函数重载相比，缺省参数的语法更简洁明了，减少了大量的样板代码，也更好地区别了“必要参数”和“可选参数”。

**缺点**

缺省参数实际上是函数重载语义的另一种实现方式，因此所有  [不应当使用函数重载的理由](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/others/#function-overloading)  也都适用于缺省参数。

虚函数调用的缺省参数取决于目标对象的静态类型，此时无法保证给定函数的所有重载声明的都是同样的缺省参数。

缺省参数是在每个调用点都要进行重新求值的，这会造成生成的代码迅速膨胀。作为读者，一般来说也更希望缺省的参数在声明时就已经被固定了，而不是在每次调用时都可能会有不同的取值。

缺省参数会干扰函数指针，导致函数签名与调用点的签名不一致。而函数重载不会导致这样的问题。

**结论**

对于虚函数，不允许使用缺省参数，因为在虚函数中缺省参数不一定能正常工作。如果在每个调用点缺省参数的值都有可能不同，在这种情况下缺省函数也不允许使用。（例如，不要写像   这样的代码。）`void f(int n = counter++);`

在其他情况下，如果缺省参数对可读性的提升远远超过了以上提及的缺点的话，可以使用缺省参数。如果仍有疑惑，就使用函数重载。

## 4.6。函数返回类型后置语法

**总述**

只有在常规写法（返回类型前置）不便于书写或不便于阅读时使用返回类型后置语法。

**定义**

C ++现在允许两种不同的函数声明方式。以往的写法是将返回类型置于函数名之前。例如：

int foo （int x ）;

C ++ 11引入了这一新的形式。现在可以在函数名前使用  `auto` 关键字，在参数列表之后后置返回类型。例如：

auto foo （int x ）- > int ;

后置返回类型为函数作用域。对于像  `int` 这样简单的类型，两种写法没有区别。但对于复杂的情况，例如类域中的类型声明或者以函数参数的形式书写的类型，写法的不同会造成区别。

**优点**

后置返回类型是显式地指定  [Lambda表达式](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/others/#lambda-expressions)  的返回值的唯一方式。某些情况下，编译器可以自动推导出Lambda表达式的返回类型，但并不是在所有的情况下都能实现。即使编译器能够自动推导，显式地指定返回类型也能让读者更明了。

有时在已经出现了的函数参数列表之后指定返回类型，能够让书写更简单，也更易读，尤其是在返回类型依赖于模板参数时。例如：

template < class T ，class U > auto add （T t ，U u ）- > decltype （t + u ）;

对比下面的例子：

template < class T ，class U > decltype （declval < T ＆> （）+ declval < U ＆gt ;）add （T t ，U u ）;

**缺点**

后置返回类型相对来说是非常新的语法，而且在C和Java中都没有相似的写法，因此可能对读者来说比较陌生。

在已有的代码中有大量的函数声明，你不可能把它们都用新的语法重写一遍。因此实际的做法只能是使用旧的语法或者新旧混用。在这种情况下，只使用一种版本是相对来说更规整的形式。

**结论**

在大部分情况下，应当继续使用以往的函数声明写法，即将返回类型置于函数名前。只有在必要的时候（如Lambda表达式）或者使用后置语法能够简化书写并且提高易读性的时候才使用新的返回类型后置语法。但是后一种情况一般来说是很少见的，大部分时候都出现在相当复杂的模板代码中，而多数情况下不鼓励写这样  [复杂的模板代码](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/others/#template-metaprogramming)。

# 7.命名约定

最重要的一致性规则是命名管理。命名的风格能让我们在不需要去查找类型声明的条件下快速地了解某个名字代表的含义：类型，变量，函数，常量，宏，等等，甚至。我们大脑中的模式匹配引擎非常依赖这些命名规则。

命名规则具有一定随意性，但相比按个人喜好命名，一致性更重要，所以无论你认为它们是否重要，规则总归是规则。

## 7.1。通用命名规则

**总述**

函数命名，变量命名，文件命名要有描述性; 少用缩写。

**说明**

尽可能使用描述性的命名，别心疼空间，毕竟相比之下让代码易于新读者理解更重要。不要用只有项目开发者能理解的缩写，也不要通过砍掉几个字母来缩写单词。

int price_count_reader ; //无缩写int num_errors ; //“num”是一个常见的写法int num_dns_connections ; //人人都知道“DNS”是什么

int n ; //毫无意义。int nerr ; //含糊不清的缩写。int n_comp_conns ; //含糊不清的缩写。int wgc_connections ; //只有贵团队知道是什么意思 int pc_reader ; //“pc”有太多可能的解释了。int cstmr_id ; //删减了若干字母。

注意，一些特定的广为人知的缩写是允许的，例如用  `i` 表示迭代变量和用  `T` 表示模板参数。

模板参数的命名应当遵循对应的分类：类型模板参数应当遵循  [类型命名](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#type-names)  的规则，而非类型模板应当  [遵循变量命名](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#variable-names)  的规则。

## 7.2。文件命名

**总述**

文件名要全部小写，可以包含下划线（`_`）或连`-`字符（），依照项目的约定。如果没有约定，那么“ `_`” 更好。

**说明**

可接受的文件命名示例：
- `my_useful_class.cc`
- `my-useful-class.cc`
- `myusefulclass.cc`
- `myusefulclass_test.cc` //  `_unittest` 状语从句：  `_regtest` 已弃用。

C ++文件要以  `.cc` 结尾，头文件以  `.h` 结尾。专门插入文本的文件则以  `.inc` 结尾，参见  [头文件自足](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/headers/#self-contained-headers)。

不要使用已经存在于  `/usr/include` 下的文件名（Yang.Y注：即编译器搜索系统头文件的路径），如  `db.h`。

通常应尽量让文件名更加明确。  `http_server_logs.h` 就比  `logs.h` 要好。定义类时文件名一般成对出现，如  `foo_bar.h` 和  `foo_bar.cc`，对应于类  `FooBar`。

联内必须函数放在  `.h` 文件中。如果内联函数比较短，就直接放在  `.h` 中。

## 7.3。类型命名

总述

类型名称的每个单词首字母均大写，不包含下划线：  `MyExcitingClass`，  `MyExcitingEnum`。

说明

所有类型命名 - 类，结构体，类型定义（`typedef`），枚举，类型模板参数 - 均使用相同约定，即以大写字母开始，每个单词首字母均大写，不包含下划线。例如：

//类和结构体类UrlTable { ... class UrlTableTester { ... struct UrlTableProperties { ...//类型定义typedef hash_map < UrlTableProperties * ，string > PropertiesMap ;//使用别名使用PropertiesMap = hash_map < UrlTableProperties * ，string > ;//枚举enum UrlTableErrors { ...

## 7.4。变量命名

总述

变量（包括函数参数）和数据成员名一律小写，单词之间用下划线连接。类的成员变量以下划线结尾，但结构体的就不用，如：  `a_local_variable`，  `a_struct_data_member`，  `a_class_data_member_`。

说明

### 普通变量命名

举例：

字符串table_name ; //好 - 用下划线。字符串表名; //好 - 全小写。字符串tableName ; //差 - 混合大小写

### 类数据成员

不管是静态的还是非静态的，类数据成员都可以和普通变量一样，但要接下划线。

类TableInfo { ... private ：string table_name_ ; //好 - 后加下划线。字符串tablename_ ; //好。静态池< TableInfo > * pool_ ; //好。};

### 结构体变量

不管是静态的还是非静态的，结构体数据成员都可以和普通变量一样，不用像类那样接下划线：

struct UrlTableProperties { string name ; int num_entries ; 静态池< UrlTableProperties > * 池; };

结构体与类的使用讨论，参考  [结构体与类](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/classes/#structs-vs-classes)。

## 7.5。常量命名

总述

声明为  `constexpr` 或  `const` 的变量，或在程序运行期间其值始始保持不变的，命名时以“k”开头，大小写混合。例如：

const int kDaysInAWeek = ;

说明

所有具有静态存储类型的变量（例如静态变量或全局变量，参见  [存储类型](http://en.cppreference.com/w/cpp/language/storage_duration#Storage_duration)）都应当以此方式命名。对于其他存储类型的变量，如自动变量等，这条规则是可选的。如果不采用这条规则，就按照一般的变量命名规则。

## 7.7。函数命名

总述

常规函数使用大小写混合，取值和设值函数则要求与变量名匹配：  `MyExcitingFunction()`，  `MyExcitingMethod()`，  `my_exciting_member_variable()`，  `set_my_exciting_member_variable()`。

说明

一般来说，函数名的每个单词首字母大写（即“驼峰变量名”或“帕斯卡变量名”），没有下划线。对于首字母缩写的单词，更倾向于将它们视作一个单词进行首字母大写（例如，写作  `StartRpc()` 而非  `StartRPC()`）。

AddTableEntry （）DeleteUrl （）OpenFileOrDie （）

（同样的命名规则同时适用于类作用域和命名空间作用域的常量，因为它们是作为API的一部分暴露对外的，因此应当让它们看起来像是一个函数，因为在这时，它们实际上是一个对象而非函数的这一事实对外不过是一个无关紧要的实现细节。）

取值和设值函数的命名与变量一致。一般来说它们的名称与实际的成员变量对应，但并不强制要求。例如   与  。`int count()``void set_count(int count)`

## 7.7。命名空间命名

总述

命名空间以小写字母命名。最高级命名空间的名字取决于项目名称。要注意避免嵌套命名空间的名字之间和常见的顶级命名空间的名字之间发生冲突。

顶级命名空间的名称应当是项目名或者是该命名空间中的代码所属的团队的名字。命名空间中的代码，应当存放于和命名空间的名字匹配的文件夹或其子文件夹中。

注意  [不使用缩写作为名称](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#general-naming-rules)  的规则同样适用于命名空间。命名空间中的代码极少需要涉及命名空间的名称，因此没有必要在命名空间中使用缩写。

要避免嵌套的命名空间与常见的顶级命名空间发生名称冲突。由于名称查找规则的存在，命名空间之间的冲突完全有可能导致编译失败。尤其是，不要创建嵌套的  `std` 命名空间。建议使用更独特的项目标识符（`websearch::index`，  `websearch::index_util`）而非常见的极易发生冲突的名称（比如  `websearch::util`）。

对于  `internal` 命名空间，要当心加入到同一  `internal` 命名空间的代码之间发生冲突（由于内部维护人员通常来自同一团队，因此常有可能导致冲突）。在这种情况下，请使用文件名以使内部名称独一无二（例如对于  `frobber.h`，使用  `websearch::index::frobber_internal`）。

## 7.8。枚举命名

总述

的枚举命名应当状语从句：  [常量](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#constant-names)  或  [宏](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#macro-names)  harmony和谐：  `kEnumName` 或是  `ENUM_NAME`。

说明

的单独枚举值应该优先采用  [常量](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#constant-names)  的命名方式。但  [宏](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#macro-names)  方式的命名也。可以接受。枚举名  `UrlTableErrors` （以及  `AlternateUrlTableErrors`）是类型，所以要用大小写混合的方式。

enum UrlTableErrors { kOK = ，kErrorOutOfMemory ，kErrorMalformedInput ，}; 枚举AlternateUrlTableErrors { OK = ，OUT_OF_MEMORY = ，MALFORMED_INPUT = ，};

2009年1月之前，我们一直建议采用  [宏](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#macro-names)  的方式命名枚举值。由于枚举值和宏之间的命名冲突，直接导致了很多问题。由此，这里改为优先选择常量风格的命名方式。新代码应该尽可能优先使用常量风格。但是老代码没必要切换到常量风格，除非宏风格确实会产生编译期问题。

## 7.9。宏命名

**总述**

你并不打算  [使用宏](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/others/#preprocessor-macros)，对吧？如果你一定要用，像这样命名：  `MY_MACRO_THAT_SCARES_SMALL_CHILDREN`。

**说明**

参考  [预处理宏](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/others/#preprocessor-macros) ; 通常  *不应该*  使用宏。如果不得不使用，其命名像枚举命名一样全部大写，使用下划线：

#define ROUND（x）... #define PI_ROUNDED 3.0

## 7.10。命名规则的特例

**总述**

如果你命名的实体与已有C / C ++实体相似，可参考现有命名策略。

`bigopen()`：函数名，参照  `open()` 的形式

`uint`： `typedef`

`bigpos`：  `struct` 或  `class`，参照  `pos` 的形式

`sparse_hash_map`：STL型实体; 参照STL命名约定

`LONGLONG_MAX`：常量，如同 `INT_MAX`

# 8.注意

注释虽然写起来很痛苦，但对保证代码可读性至关重要。下面的规则描述了如何注释以及在哪儿注释。当然也要记住：注释固然很重要，但最好的代码应当本身就是文档。有意义的类型名和变量名，要远胜过要用注释解释的含糊不清的名字。

你写的注释是给代码读者看的，也就是下一个需要理解你的代码的人。所以慷慨些吧，下一个读者可能就是你！

## 8.1。注释风格

总述

使用  `//` 或  ，统一就好。`/* */`

说明

`//` 或   都可以; 但  *更*  常用。要在如何注释及注释风格上确保统一。`/* */``//`

## 8.2。文件注释

总述

在每一个文件开头加入版权公告。

文件注释描述了该文件的内容。如果一个文件只声明，或实现或测试了一个对象，并且这个对象已经在它的声明处进行了详细的注释，那么就没有必要再加上文件注释。除此之外的其他文件都需要文件注释。

说明

### 法律公告和作者信息

每个文件都应该包含许可证引用。为项目选择合适的许可证版本（比如，Apache 2.0，BSD，LGPL，GPL）

如果你对原始作者的文件做了重大修改，请考虑删除原作者信息。

### 文件内容

如果一个  `.h` 文件声明了多个概念，则文件注释应当对文件的内容做一个大致的说明，同时说明各个概念之间的联系。一个一到两行的文件注释就足够了，对于每个概念的详细文档应当放在各个概念中，而不是文件注释中。

不要在  `.h` 和  `.cc` 之间复制注释，这样的注释偏离了注释的实际意义。

## 8.3。类注释

总述

每个类的定义都要附带一份注释，描述类的功能和用法，除非它的功能相当明显。

//遍历GargantuanTable的内容。//示例：// GargantuanTableIterator * iter = table-> NewIterator（）; // it for（iter-> Seek（“foo”）;！iter-> done（）; iter-> Next（））{ // process（iter-> key（），iter-> value（））; //} //删除它; 类GargantuanTableIterator { ... };

说明

类注释应当为读者理解如何使用与何时使用类提供足够的信息，同时应当提醒读者在正确使用此类时应当考虑的因素。如果类有任何同步前提，请用文档说明。如果该类的实例可被多线程访问，要特别注意文档说明多线程环境下相关的规则和常量使用。

如果你想用一小段代码演示这个类的基本用法或通常用法，放在类注释里也非常合适。

如果类的声明和定义分开了（例如分别放在了  `.h` 和  `.cc` 文件中），此时，描述类用法的注释应当和接口定义放在一起，描述类的操作和实现的注释应当和实现放在一起。

## 8.4。函数注释

总述

函数声明处的注释描述函数功能; 定义处的注释描述函数实现。

说明

### 函数声明

基本上每个函数声明处前都应当加上注释，描述函数的功能和用途。只有在函数的功能简单而明显时才能省略这些注释（例如，简单的取值和设值函数）。注释使用叙述式（“打开文件”）而非指令式（“打开文件”）; 注释只是为了描述函数，而不是命令函数做什么。通常，注释不会描述函数如何工作。那是函数定义部分的事情。

函数声明处注释的内容：
- 函数的输入输出。
- 对类成员函数而言：函数调用期间对象是否需要保持引用参数，是否会释放这些参数。
- 函数是否分配了必须由调用者释放的空间。
- 参数是否可以为空指针。
- 是否存在函数使用上的性能隐患。
- 如果函数是可重入的，其同步提提是什么？

举例如下：

//返回此表的迭代器。当迭代器完成时，它是客户端的责任//并且一旦创建迭代器的GargantuanTable对象被删除，它就不能使用迭代器。// //迭代器最初位于表的开始位置。// //此方法等同于：// Iterator * iter = table-> NewIterator（）; // iter-> Seek（“”）; //返回iter; //如果您要立即寻找到返回的迭代器中的其他位置，则使用NewIterator（）会更快，并避免额外的查找。Iterator * GetIterator （）const;

但也要避免罗罗嗦嗦，或者对显着易见的内容进行说明。下面的注释就没有必要加上“否则返回false”，因为已经暗含其中了：

//如果表不能包含更多条目，则返回true。bool IsTableFull （）;

注释函数重载时，注释的重点应该是函数中被重载的部分，而不是简单的重复被重载的函数的注释。多数情况下，函数重载不需要额外的文档，因此也没有必要加上注释。

注释构造/析构函数，切记读代码的人知道构造/析构函数的所有功能，所以“销毁这一对象”这样的注释是没有意义的。你应该注意的是注意构造函数对参数做了什么（例如，是否取得指针所有权）以及析构函数清理了什么。如果都是些无关紧要的内容，直接省掉注释。析构函数前没有注释是很正常的。

### 函数定义

如果函数的实现过程中用到了很巧妙的方式，那么在函数定义处应当加上解释性的注释。例如，你所使用的编程技巧，实现的大致步骤，或解释如此实现的理由。举个例子，你可以说明为什么函数的前半部分要加锁而后半部分不需要。

*不要*  从  `.h` 文件或其他地方的函数声明处直接复制注释。简要重述函数功能是可以的，但注释重点要放在如何实现上。

## 8.5。变量注释

**总述**

通常变量名本身足以很好说明变量用途。某些情况下，也需要额外的注释说明。

**说明**

### 类数据成员

每个类数据成员（也叫实例变量或成员变量）都应该用注释说明用途。如果有非变量的参数（例如特殊值，数据成员之间的关系，生命周期等）不能够使用类型与变量名明确表达，则应当加上注释。然而，如果变量类型与变量名已经足够描述一个变量，那么就不需要加上注释。

特别地，如果变量可以接受  `NULL` 或  `-1` 等警戒值，须加以说明。比如：

private ：//用于限制检查表访问。-1意味着//我们还不知道表中有多少个条目。int num_total_entries_ ;

### 全局变量

和数据成员一样，所有全局变量也要注释说明含义及用途，以及作为全局变量的原因。比如：

//在此回归测试中我们经历的测试用例的总数。const int kNumTestCases = ;

## 8.6。实现注释

总述

对于代码中巧妙的，晦涩的，有趣的，重要的地方加以注释。

说明

### 代码前注释

巧妙或复杂的代码段前要加注释。比如：

//将结果除以2，考虑到x //包含来自add的进位。for （int i = ; i < result - > size （）; i ++ ）{ x = （x << ）+ （* result ）[ i ]; （* 结果）[ i ] = x >> ; x ＆= ; }

### 行注释

比较隐晦的地方要在行尾加入注释。在行尾空两格进行注释。比如：

//如果我们有足够的内存，也可以对数据部分进行mmap。mmap_budget = max < int64 > （，mmap_budget - index_ - > length （））; if （mmap_budget > = data_size_ && ！MmapData （mmap_chunk_bytes ，mlock ））return ; //错误已经记录。

注意，这里用了两段注释分别描述这段代码的作用，并提示函数返回错误已经被记入日志。

如果你需要连续进行多行注释，可以使之对齐获得更好的可读性：

DoSomething （）; //在这里发表评论，以便评论排成一行。DoSomethingElseThatIsLonger （）; //代码和注释之间有两个空格。{ //允许打开一个新的作用域时，在注释之前的一个空格// //因此注释与下面的注释和代码一起排列。DoSomethingElse （）; //通常在行注释之前有两个空格。} std :: vector < string > list { //支撑列表中的注释描述下一个元素... “First item” ，// ..并且应该适当地对齐。“第二项” }; 做一点事（）; / *对于尾部块注释，一个空间可以。* /

### 函数参数注释

如果函数参数的意义不明显，考虑用下面的方式进行弥补：
- 如果参数是一个字面常量，并且这一常量在多处函数调用中被使用，用以推断它们一致，你应该用一个常量名让这个约定变得更明显，并且保证这一约定不会被打破。
- 考虑更改函数的签名，让某个  `bool` 类型的参数变为  `enum` 类型，这样可以让这个参数的值表达其意义。
- 如果某个函数有多个配置选项，你可以考虑定义一个类或结构体以保存所有的选项，并传入类或结构体的实例。这样的方法有许多优点，例如这样的选项可以在调用处用变量名引用，这样就能清晰地表明其意义。同时也减少了函数参数的数量，使得函数调用更易读也易写。除此之外，以这样的方式，如果你使用其他的选项，就无需对调用点进行更改。
- 用具名变量代替大段而复杂的嵌套表达式。
- 万不得已时，才考虑在调用点用注释阐明参数的意义。

比如下面的示例的对比：

//这些论据是什么？const DecimalNumber product = CalculateProduct （values ，，false ，nullptr ）;

和

ProductOptions 选项; 选项。set_precision_decimals （）; 选项。set_use_cache （ProductOptions :: kDontUseCache ）; const DecimalNumber product = CalculateProduct （values ，options ，/ * completion_callback = * / nullptr ）;

哪个更清晰一目了然。

### 不允许的行为

不要描述显而易见的现象，  *永远不要*  用自然语言翻译代码作为注释，除非即使对深入理解C ++的读者来说代码的行为都是不明显的。要假设读代码的人C ++水平比你高，即便他/她可能不知道你的用意：

你所提供的注释应当解释代码  *为什么*  要这么做和代码的目的，或者最好是让代码自文档化。

比较这样的注释：

//在矢量中查找元素。< - 差：这太明显了！自动ITER = STD :: 找到（v 。开始（），v 。端（），元素）; 如果（ITER =！v 。端（））{ 过程（元件）; }

和这样的注释：

//处理“元素”，除非它已经被处理。自动ITER = STD :: 找到（v 。开始（），v 。端（），元素）; 如果（ITER =！v 。端（））{ 过程（元件）; }

自文档化的代码根本就不需要注释。上面例子中的注释对下面的代码来说就是毫无必要的：

if （！IsAlreadyProcessed （element ））{ Process （element ）; }

## 8.8。标点，拼写和语法

总述

注意标点，拼写和语法; 写的好的注释比差的要易读的多。

说明

注释的通常写法是包含正确大小写和结尾句号的完整叙述性语句。大多数情况下，完整的句子比句子片段可读性更高。短一点的注释，比如代码行尾注释，可以随意点，但依然要注意风格的一致性。

虽然被别人指出该用分号时却用了逗号多少有些尴尬，但清晰易读的代码还是很重要的。正确的标点，拼写和语法对此会有很大帮助。

## 8.8。TODO注释

**总述**

对那些临时的，短期的解决方案，或已经够好，但仍不完美的代码使用  `TODO` 注释。

`TODO` 注意要使用全大写的字符串  `TODO`，在随后的圆括号里写上你的名字，邮件地址，bug ID，或其它身份标识和与这一  `TODO` 相关的问题。主要目的是让添加注释的人（也是可以请求提供更多细节的人）可根据规范的  `TODO` 格式进行查找。添加  `TODO` 注释并不意味着你要自己来修正，因此当你加上带有姓名的时候  `TODO` ，一般都是写上自己的名字。

// TODO（kl@gmail.com）：这里使用“*”作为连接运算符。// TODO（Zeke）将其改为使用关系。// TODO（错误12345）：删除“最后访问者”功能

如果加  `TODO` 是为了在“将来某一天做某事”，可以附上一个非常明确的时间“Fix by November 2005”），或者一个明确的事项（“所有客户端都可以处理XML响应时删除此代码。”） 。

## 8.9。弃用注释

**总述**

通过弃用注释（`DEPRECATED` 评论）以标记某接口点已弃用。

您可以写上包含全大写的  `DEPRECATED` 注释，以标记某接口为弃用状态。注释可以放在接口声明前，或者同一行。

在`DEPRECATED` 一词后，在  括号中留下您的名字，邮箱地址以及其他身份标识。

弃用注释应当包涵简短而清晰的指引，以帮助其他人修复其调用点。在C ++中，你可以将一个弃用函数改造成一个内联函数，这一函数将调用新的接口。

`DEPRECATED` 仅仅标记接口为并  不允许大家不约而同地弃用，您还得亲自主动修正调用点（callsites），或是找个帮手。

修正好的代码应该不会再涉及弃用接口点了，着实改用新接口点。如果您不知从何下手，可以找标记弃用注释的当事人一起商量。

# 9.格式

每个人都可能有自己的代码风格和格式，但如果一个项目中的所有人都遵循同一风格的话，这个项目就能更顺利地进行。每个人未必能同意下述的每一处格式规则，而且其中的不少规则需要一定时间的适应，但整个项目服从统一的编程风格是很重要的，只有这样才能让所有人轻松地阅读和理解代码。

为了帮助你正确的格式化代码，我们写了一个  [emacs配置文件](https://raw.githubusercontent.com/google/styleguide/gh-pages/google-c-style.el)。

## 9.1。行长度

总述

每一行代码字符数不超过80。

我们也认识到这条规则是有争议的，但很多已有代码都遵照这一规则，因此我们感觉一致性更重要。

优点

提倡该原则的人认为强迫他们调整编辑器窗口大小是很野蛮的行为。很多人同时并排开几个代码窗口，根本没有多余的空间拉伸窗口。大家都把窗口最大尺寸加以限定，并且80列宽是传统标准。那么为什么要改变呢？

缺点

反对该原则的人则认为更宽的代码行更易阅读。80列的限制是上个世纪60年代的大型机的古板缺陷; 现代设备具有更宽的显示屏，可以很轻松地显示更多代码。

结论

80个字符是最大值。

如果无法在不伤害易读性的条件下进行断行，那么注释行可以超过80个字符，这样可以方便复制粘贴。例如，带有命令示例或URL的行可以超过80个字符。

长所有游戏的路径  `#include` 语句可以超出80列。

[文件头保护](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/headers/#define-guard)  可以无视该原则。

## 9.2。非ASCII字符

总述

尽量不使用非ASCII字符，使用时必须使用UTF-8编码。

说明

即使是英文，也不应将用户界面的文本硬编码到源代码中，因此非ASCII字符应当很少被用到。特殊情况下可以适当包含此类字符。例如，代码分析外部数据文件时，可以适当硬编码数据文件中作为分隔符的非ASCII字符串; 更常见的是（不需要本地化的）单元测试代码可能包含非ASCII字符串。此类情况下，应使用UTF-8编码，因为很多工具都可以理解和处理UTF-8编码。

十六进制编码也可以，能增强可读性的情况下尤其鼓鼓 - 比如  `"\xEF\xBB\xBF"`，或者更简洁地写作  `u8"\uFEFF"`，在Unicode中是  *零宽度无间断*  的间隔符号，如果不用十六进制直接放在UTF -8格式的源文件中，是看不到的。

（Yang.Y注：  `"\xEF\xBB\xBF"` 通常用作带编码标记的UTF-8）

使用  `u8` 前缀把带  `uXXXX` 转义序列的字符串字面值编码成UTF-8。不要用在本身就带UTF-8字符的字符串字面值上，因为如果编译器不把源代码识别成UTF-8，输出就会出错。

别用C ++ 11的  `char16_t` 和  `char32_t`，它们和UTF-8文本没有关系，  `wchar_t` 同理，除非你写的代码要调用Windows API，后者广泛使用了  `wchar_t`。

## 9.3。空格还是制表位

**总述**

只使用空格，每次缩进2个空格。

**说明**

我们使用空格缩进。不要在代码中使用制表符。你应该设置编辑器将制表符转为空格。

## 9.4。函数声明与定义

**总述**

返回类型和函数名在同一行，参数也尽量放在同一行，如果放不下就对形参分行，分行方式与  [函数调用](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/formatting/#function-calls)  一致。

**说明**

函数看上去像这样：

ReturnType ClassName :: FunctionName （Type par_name1 ，Type par_name2 ）{ DoSomething （）; ... }

如果同一行文本太多，放不下所有参数：

ReturnType ClassName :: ReallyLongFunctionName （类型par_name1 ，类型par_name2 ，类型par_name3 ）{ DoSomething （）; ... }

甚至连第一个参数都放不下：

ReturnType LongClassName :: ReallyReallyReallyLongFunctionName （Type par_name1 ，// 4 space indent Type par_name2 ，Type par_name3 ）{ DoSomething （）; // 2空格缩进... }

注意以下几点：
- 使用好的参数名。
- 只有参数未被使用或者其用途非常明显时，才能省略参数名。
- 如果返回类型和函数名在一行放不下，分行。
- 如果返回类型与函数声明或定义分行了，不要缩进。
- 左圆括号总是和函数名在同一行。
- 函数名和左圆括号间永远没有空格。
- 圆括号与参数间没有空格。
- 左大括号总在最后一个参数同一行的末尾处，不另起新行。
- 右大括号总是单独位于函数最后一行，或者与左大括号同一行。
- 右圆括号和左大括号间总是有一个空格。
- 所有形参应尽可能对齐。
- 缺省缩进为2个空格。
- 换行后的参数保持4个空格的缩进。

未被使用的参数，或者根据上下文很容易看出其用途的参数，可以省略参数名：

类美孚{ 公共：美孚（富&& ）; Foo （const Foo ＆）; Foo ＆operator = （Foo && ）; Foo ＆operator = （const Foo ＆）; };

未被使用的参数如果其用途不明显的话，在函数定义处将参数名注释起来：

class Shape { public ：virtual void Rotate （double radians ）= ; };classCircle:publicShape{public:voidRotate(doubleradians)override;};voidCircle::Rotate(double/*radians*/){}

// 差 - 如果将来有人要实现, 很难猜出变量的作用.voidCircle::Rotate(double){}

属性, 和展开为属性的宏, 写在函数声明或定义的最前面, 即返回类型之前:

MUST_USE_RESULTboolIsOK();

## 9.5. Lambda 表达式

**总述**

Lambda 表达式对形参和函数体的格式化和其他函数一致; 捕获列表同理, 表项用逗号隔开.

**说明**

若用引用捕获, 在变量名和 `&` 之间不留空格.

intx=;autoadd_to_x=[&x](intn){x+=n;};

短 lambda 就写得和内联函数一样.

std::set<int>blacklist={,,};std::vector<int>digits={,,,,,,};digits.erase(std::remove_if(digits.begin(),digits.end(),[&黑名单]（诠释我）{ 返回黑名单。找到（i ）！= 黑名单。end （）; }），数字。end （））;

## 9.6。函数调用

**总述**

要么一行写完函数调用，要么在圆括号里对参数分行，要么参数另起一行且缩进四格。如果没有其它顾虑的话，尽可能精简行数，比如把多个参数适当地放在同一行里。

**说明**

函数调用遵循如下形式：

bool retval = DoSomething （argument1 ，argument2 ，argument3 ）;

如果同一行放不下，可断为多行，后面每一行都和第一个实参对齐，左圆括号后和右圆括号前不要留空格：

bool retval = DoSomething （averyveryveryverylongargument1 ，argument2 ，argument3 ）;

参数也可以放在次行，缩进四格：

如果（...）{ ... ... if （...）{ DoSomething （argument1 ，argument2 ，// 4空格缩进argument3 ，argument4 ）; }

把多个参数放在同一行以减少函数调用所需的行数，除非影响到可读性。有人认为把每个参数都独立成行，不仅更好读，而且方便编辑参数。不过，比起所谓的参数编辑，我们更看重可读性，且后者比较好办：

如果一些参数本身就是略复杂的表达式，且降低了可读性，那么可以直接创建临时变量描述该表达式，并传递给函数：

intmy_heuristic=scores[x]*y+bases[x];boolretval=DoSomething(my_heuristic,x,y,z);

或者放着不管, 补充上注释：

boolretval=DoSomething(scores[x]*y+bases[x],// Score heuristic.x,y,z);

如果某参数独立成行, 对可读性更有帮助的话, 那也可以如此做. 参数的格式处理应当以可读性而非其他作为最重要的原则.

此外, 如果一系列参数本身就有一定的结构, 可以酌情地按其结构来决定参数格式：

//通过3x3矩阵转换小部件。my_widget 。变换（x1 ，x2 ，x3 ，y1 ，y2 ，y3 ，z1 ，z2 ，z3 ）;

## 9.7。列表初始化格式

总述

您平时怎么格式化函数调用，就怎么格式化  [列表初始化](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/others/#braced-initializer-list)。

说明

如果列表初始化伴随着名字，比如类型或变量名，格式化时将将名称视图函数调用名，  {}视图函数调用的括号。如果没有名字，就视作名字长度为零。

//一行列表初始化示范。返回{ foo ，bar }; functioncall （{ foo ，bar }）; pair < int ，int > p { foo ，bar };//当不得不行时。SomeFunction （{ “在{” } 之前假定一个零长度的名字，//假设在{前有长度为零的名字。some_other_function_parameter ）; SomeType 变量{ some ，other ，values ，{ “假设在{” } 之前有一个零长度的名字，//假设在{前有长度为零的名字。SomeOtherType { “非常长的字符串需要周围的中断。” ，//非常长的字符串，前后都需要断行。一些，其它的值}，SomeOtherType { “略短字符串” ，//稍短的字符串。一些，其它，值}}; SomeType 变量{ “这太长了，无法将所有内容放在一行中” }; //字符串过长，因此无法放在同一行。MyType m = { //注意了，您可以在{前断行。superlongvariablename1 ，superlongvariablename2 ，{ short ，interior ，list }，{ interiorwrappinglist ，interiorwrappinglist2 }};

## 9.9。条件语句

总述

倾向于不在圆括号内使用空格。关键字  `if` 状语从句：  `else` 另起一行。

说明

对基本条件语句有两种可以接受的格式。一种在圆括号和条件之间有空格，另一种没有。

最常见的是没有空格的格式。哪一种都可以，最重要的是  *保持一致*。如果你是在修改一个文件，参考当前已有格式。如果是写新的代码，请参考目录下或项目中的其它文件。还在犹豫的话，就不要加空格了。

if （condition ）{ //圆括号里没有空格。... // 2空格缩进。} else if （...）{ // else与if的右括号同一行。... } 其他{ ... }

如果你更喜欢在圆括号内部加空格：

if （condition ）{ //圆括号与空格紧邻 - 不常见... // 2空格缩进。} else { // else与if的右括号同一行。... }

所有注意下情况  `if` 状语从句：左圆括号间都有个空格。右圆括号和左大括号之间也要有个空格：

if （condition ）//差 -  IF后面没空格。if （condition ）{ //差 -  {前面没空格。if （condition ）{ //变本加厉地差。

if （condition ）{ //好 -  IF和{都与空格紧邻。

如果能增强可读性，简短的条件语句允许写在同一行。当只有简单语句并且没有使用  `else` [主语]时使用：

if （x == kFoo ）返回新的Foo （）; if （x == kBar ）返回新的Bar （）;

如果有语句  `else` 分支则不允许：

//不允许 - 当有ELSE分支时如块块写在同一行if （x ）DoThis （）; 其他DoThat （）;

通常，单行语句不需要使用大括号，如果你喜欢用也没问题; 复杂的条件或循环语句用大括号可读性会更好。有也。项目一些要求  `if` 必须总是使用大括号：

如果（条件）DoSomething （）; // 2空格缩进。if （condition ）{ DoSomething （）; // 2空格缩进。}

但如果语句中  `if-else` 某个分支使用了大括号的话，其它分支也必须使用：

//不可以这样子 - 如果有大括号ELSE却没有。if （condition ）{ foo ; } else bar ;//不可以这样子 -  ELSE有大括号IF却没有。如果（条件）foo ; else { bar ; }

//只要其中一个分支用了大括号，两个分支都要用上大括号。if （condition ）{ foo ; } else { bar ; }

## 9.9。循环和开关选择语句

**总述**

`switch` 语句可以使用大括号分段，以表明cases之间不是连在一起的。在单语句循环里，括号可用可不用。循环空应行业释义体育使用  `{}` 或  `continue`。

**说明**

`switch` 语句中的  `case` 块可以使用大括号也可以不用，取决于你的个人喜好。如果用的话，要按照下文所述的方法。

如果有不满足  `case` 条件的枚举值，  `switch` 应该`default` 总是包含一个  匹配（如果有输入值没有case去处理，编译器将给出警告）。如果  `default` 应该永远执行不到，简单的加条  `assert`：

switch （var ）{ case ：{ // 2空格缩进... // 4空格缩进break ; } 案例：{ ... break ; } default ：{ assert （false ）; } }

在单语句循环里，括号可用可不用：

for （int i = ; i < kSomeNumber ; ++ i ）printf （“我爱你\ n ” ）;for （int i = ; i < kSomeNumber ; ++ i ）{ printf （“我拿回来\ n ” ）; }

空循环体应使用  `{}` 或  `continue`，而不是一个简单的分号。

while （condition ）{ //反复循环直到条件失效。} for （int i = ; i < kSomeNumber ; ++ i ）{} //可 - 空循环体。同时（条件）继续; //可 -  contunue表明没有逻辑。

while （condition ）; //差 - 看起来仅仅只是while / loop的部分之一。

## 9.10。指针和引用表达式

**总述**

句点或箭头前后不要有空格。指针/地址操作符（）之后不能有空格。`*, &`

**说明**

下面是指针和引用表达式的正确使用范例：

x = * p ; p = ＆x ; x = r 。y ; x = r - > y ;

注意：
- 在访问成员时，句点或箭头前后没有空格。
- 操作指针符  `*` 或  `&` 后没有空格。

在声明指针变量或参数时，星号与类型或变量名紧挨都可以：

//好，空格前置。char * c ; const string ＆str ;//好，空格后置。char * c ; const string ＆str ;

int x ，* y ; //不允许 - 在多重声明中不能使用＆或* char * c ; //差 -  *两边都有空格const string ＆str ; //差 - ＆两边都有空格。

在单个文件内要保持风格一致，所以，如果是修改现有文件，要遵照该文件的风格。

## 9.11。布尔表达式

**总述**

如果一个布尔表达式超过  [标准行宽](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/formatting/#line-length)，断行方式要统一一下。

**说明**

下例中，逻辑与（`&&`）操作符总位于行尾：

if （this_one_thing > this_other_thing && a_third_thing == a_fourth_thing && yet_another && last_one ）{ ... }

注意，上例的逻辑与（`&&`）操作符均位于行尾。这个格式在Google里很常见，虽然把所有操作符放在开头也可以。可以考虑额外插入圆括号，合理使用的话对增强可读性是很有帮助的。此外，直接用符号形式的操作符，比如  `&&` 和  `~`，不要用词语形式的  `and` 和  `compl`。

## 9.12。函数返回值

**总述**

在不要  `return` 表达式里加上非必须的圆括号。

**说明**

在只有写   要加上括号的时候才在   里使用括号。`x = expr``return expr;`

返回结果; //返回很简单，没有圆括号。//可以用圆括号把复杂表达式圈起来，改善可读性。return （some_long_condition && another_condition ）;

回报（价值）; //毕竟您从从来不会写var =（value）; 返回（结果）; // return可不是函数！

## 9.13。变量及数组初始化

**总述**

用  `=`，  `()` 状语从句：  `{}` 均可。

**说明**

您可以用  `=`，  `()` 和  `{}`，以下的例子都是正确的：

int x = ; int x （）; int x { }; 字符串名称（“Some Name” ）; 字符串名称= “某个名称” ; 字符串名称{ “Some Name” };

请小心务必列表初始化  `{...}` 用  `std::initializer_list` 构造函数初始化出的类型。非空列表初始化就会优先调用  `std::initializer_list`，不过空列表初始化除外，后者原则上会调用默认构造函数。为了强制禁用  `std::initializer_list` 构造函数，请改用括号。

矢量< INT > v （，）; //内容为100个1的向量。矢量< INT > v { ，}; //内容为100和1的向量。

此外，列表初始化不允许整型类型的四舍五入，这可以用来避免一些类型上的编程失误。

int pi （3.14 ）; //好 -  pi == 3. int pi { 3.14 }; //编译错误：缩略转换。

## 9.14。预处理指令

**总述**

预处理指令不要缩进，从行首开始。

**说明**

即使预处理指令位于缩进代码块中，指令也应从行首开始。

//好 - 指令从行首开始if （lopsided_score ）{ #if DISASTER_PENDING       //正确 - 从行首开始DropEverything （）; ＃if NOTIFY                //非必要 - ＃后跟空格NotifyClient （）; ＃endif #endif BackToNormal （）; }

//差-指令缩进如果（lopsided_score ）{ 的#if DISASTER_PENDING   //差- “的#if”应该放在行开头DropEverything （）; #endif                 //差 - “#endif”不要缩进BackToNormal （）; }

## 9.15。类格式

**总述**

访问控制块的声明依次序是  `public:`，  `protected:`，  `private:`，每个都缩进1个空格。

**说明**

类声明（下面的代码中缺少注释，参考  [类注释](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/comments/#class-comments)）的基本格式如下：

class MyClass ：public OtherClass { public ：//注意有一个空格的缩进MyClass （）; //标准的两空格缩进显式MyClass （int var ）; 〜MyClass的（）{}void SomeFunction （）; void SomeFunctionThatDoesNothing （）{ }void set_some_var （int var ）{ some_var_ = var ; } int some_var （）const { return some_var_ ; }private ：bool SomeInternalFunction （）;int some_var_ ; int some_other_var_ ; };

注意事项：
- 所有基类名应在80列限制下尽量与子类名放在同一行。
- 关键词  `public:`，  `protected:`，  `private:` 要缩进1个空格。
- 除第一个关键词（一般是  `public`）外，其他关键词前要空一行。如果类比较小的话也可以不空。
- 这些关键词后不要保留空行。
- `public` 放在最前面，然后是  `protected`，最后是  `private`。
- 声明关于顺序的规则请参考  [声明顺序](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/classes/#declaration-order)  一节。

## 9.16。构造函数初始值列表

**总述**

构造函数初始化列表放在同一行或按四格缩进并排多行。

**说明**

下面两种初始值列表方式都可以接受：

//如果所有变量能放在同一行：MyClass :: MyClass （int var ）：some_var_ （var ）{ DoSomething （）; }//如果不能放在同一行，//必须置于冒号后，并缩进4个空格MyClass :: MyClass （int var ）：some_var_ （var ），some_other_var_ （var + ）{ DoSomething （）; }//如果初始化列表需要置于多行，将每个成员放在单独的一行//并逐行对齐MyClass :: MyClass （int var ）：some_var_ （var ），// 4空格缩进some_other_var_ （var + ）{ //列队DoSomething （）; }//右大括号}可以和左大括号{放在同一行//如果这样做合适的话MyClass :: MyClass （int var ）：some_var_ （var ）{}

## 9.17。命名空间格式化

**总述**

命名空间内容不缩进。

**说明**

[命名空间](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/scoping/#namespaces)  不要增加额外的缩进层次，例如：

命名空间{void foo （）{ //正确。命名空间内没有额外的缩进。... }} //命名空间

不要在命名空间内缩进：

命名空间{//错，缩进多余了。void foo （）{ ... }} //命名空间

声明嵌套命名空间时，每个命名空间都独立成行。

namespace foo { namespace bar {

## 9.19。水平留白

**总述**

水平留白的使用根据在代码中的位置决定。永远不要在行尾添加没意义的留白。

**说明**

### 通用

void f （bool b ）{ //左大括号前总是有空格。... int i = ; //分号前不加空格。//列表初始化中大括号内的空格是可选的。//如果加了空格，那么两边都要加。int x [] = { }; int x [] = { };//继承与初始列表中的冒号前后恒有空格。class Foo ：public Bar { public ：//对于单行函数的实现，在大括号内加上空格//然后是函数实现Foo （int b ）：Bar （），baz_ （b ）{} //大括号里面是空的话，不加空格。void Reset （）{ baz_ = ; } //用括号把大括号与实现分开。...

添加冗余的留白会给其他人编辑时造成额外负担。因此，行尾不要留空格。如果确定一行代码已经修改完毕，将多余的空格去掉; 或者在专门清理空格时去掉（尤其是在没有其他人在处理这件事的时候）。（Yang.Y注：现在大部分代码编辑器稍加设置后，都支持自动删除行首/行尾空格，如果不支持，考虑换一款编辑器或IDE）

### 循环和条件语句

if （b ）{ // if条件语句和循环语句关键字后均有空格。} else { // else前后有空格。} while （test ）{} //圆括号内部不紧邻空格。switch （i ）{ for （int i = ; i < ; ++ i ）{ switch （i ）{ //循环和条件语句的圆括号里可以与空格紧邻。如果（测试）{ //圆括号，但这很少见。总之要一致。for （inti = ; 我< ; ++ i ）{ for （; i < ; ++ i ）{ //循环里内; 后恒有空格，; 前可以加个空格。switch （i ）{ case ：// switch case的冒号前无空格。... 案例：打破; //如果冒号有代码，加个空格。

### 操作符

//赋值运算符前后总是有空格。x = ;//其它二元操作符也前后恒有空格，不过对于表达式的子式可以不加空格。//圆括号内部没有紧邻空格。v = w * x + y / z ; v = w * x + y / z ; v = w * （x + z ）;//在参数和一元操作符之间不加空格。X = - ; ++ x ; 如果（x && ！y ）...

### 模板和转换

//尖括号（<和>）不与空格紧邻，<前没有空格，>和（之间也没有矢量< 字符串> X ; ý = 的static_cast < 炭*> （X ）;//在类型与指针操作符之间留空格也可以，但要保持一致。vector < char *> x ;

## 9.19。垂直留白

**总述**

垂直留白越少越好。

**说明**

这不仅仅是规则而是原则问题了：不在仅万不得已，不要使用空行。尤其是：两个函数定义之间的空行不要超过2行，函数体首尾不要留空行，函数体中也不要随意添加空行。

基本原则是：同一屏可以显示的代码越多，越容易理解程序的控制流程。当然，过密集的代码块和过于疏松的代码块同样难看，这取决于你的判断。但通常是垂直留白越少越好。

下面的规则可以让加入的空行更有效：有点可读性。

摘录自谷歌开源项目开源指南。

http://zh-google-styleguide.readthedocs.io/en/latest/

