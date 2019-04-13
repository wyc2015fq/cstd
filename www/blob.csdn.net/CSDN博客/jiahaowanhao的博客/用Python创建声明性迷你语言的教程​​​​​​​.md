
# 用Python创建声明性迷你语言的教程​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年08月19日 21:35:57[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：43


**[用Python创建声明性迷你语言的教程](http://cda.pinggu.org/view/26415.html)**
大多数程序员考虑编程时，他们都要设想用于编写应用程序的 命令式样式和技术。最受欢迎的通用编程语言（包括 Python 和其它面向对象的语言）在样式上绝大多数都是命令式的。另一方面，也有许多编程语言是 声明性样式，包括函数语言和逻辑语言，还包括通用语言和专用语言。
让我们列出几个属于各个种类的语言。许多读者已经使用过这些工具中的许多工具，但不见得考虑过它们之间的种类差别。Python、C、C++、Java、Perl、Ruby、Smalltalk、Fortran、Basic 和 xBase 都是简单的命令式编程语言。其中，一些是面向对象的，但那只是组织代码和数据的问题，而非基本编程样式的问题。使用这些语言，您 命令程序执行指令序列：把某些数据 放入（put）变量中；从变量中 获取（fetch）数据； 循环（loop）一个指令块 直到（until）满足了某些条件； 如果（if）某个命题为 true，那么就进行某些操作。所有这些语言的一个妙处在于：便于用日常生活中熟悉的比喻来考虑它们。日常生活都是由做事、选择、再做另一件事所组成的，期间或许会使用一些工具。可以简单地将运行程序的计算机想象成厨师、瓦匠或汽车司机。
诸如 Prolog、Mercury、SQL、XSLT 这样的语言、EBNF 语法和各种格式的真正配置文件，都 声明某事是这种情况，或者应用了某些约束。函数语言（比如 Haskell、ML、Dylan、Ocaml 和 Scheme）与此相似，但是它们更加强调陈述编程对象（递归、列表，等等）之间的内部（函数）关系。我们的日常生活（至少在叙事质量方面）没有提供对这些语言的编程构造的直接模拟。然而，对于那些可以用这些语言进行描述的问题来说，声明性描述 远远比命令式解决方案来得简明且不易出错。例如，请研究下面这个线性方程组：
清单 1. 线性方程式系统样本
10x + 5y - 7z + 1 = 0
17x + 5y - 10z + 3 = 0
5x - 4y + 3z - 6 = 0
这是个相当漂亮的说明对象（x、y 和 z）之间几个关系的简单表达式。在现实生活中您可能会用不同的方式求出这些答案，但是实际上用笔和纸“求解 x”很烦，而且容易出错。从调试角度来讲，用 Python 编写求解步骤或许会更糟糕。
Prolog 是与逻辑或数学关系密切的语言。使用这种语言，您只要编写您知道是正确的语句，然后让应用程序为您得出结果。语句不是按照特定的顺序构成的（和线性方程式一样，没有顺序），而且您（程序员或用户）并不知道得出的结果都采用了哪些步骤。例如：
清单 2. family.pro Prolog 样本
/* Adapted from sample at:
<http://www.engin.umd.umich.edu/CIS/course.des/cis479/prolog/>
This app can answer questions about sisterhood & love, e.g.:
\# Is alice a sister of harry?
?-sisterof( alice, harry )
\# Which of alice' sisters love wine?
?-sisterof( X, alice ), love( X, wine)
*/
sisterof( X, Y ) :- parents( X, M, F ),
female( X ),
parents( Y, M, F ).
parents( edward, victoria, albert ).
parents( harry, victoria, albert ).
parents( alice, victoria, albert ).
female( alice ).
loves( harry, wine ).
loves( alice, wine ).
它和 EBNF（扩展巴科斯范式，Extended Backus-Naur Form）语法声明并不完全一样，但是实质相似。您可以编写一些下面这样的声明：
清单 3. EBNF 样本
word    := alphanums, (wordpunct, alphanums)*, contraction?
alphanums  := [a-zA-Z0-9]+
wordpunct  := [-_]
contraction := "'", ("clock"/"d"/"ll"/"m"/"re"/"s"/"t"/"ve")
如果您遇到一个单词而想要表述其看上去 可能会是什么，而实际上又不想给出如何识别它的序列指令，上面便是个简练的方法。正则表达式与此相似（并且事实上它能够满足这种特定语法产品的需要）。
还有另一个声明性示例，请研究描述有效 XML 文档方言的文档类型声明：
清单 4. XML 文档类型声明
<!ELEMENT dissertation (chapter+)>
<!ELEMENT chapter (title, paragraph+)>
<!ELEMENT title (\#PCDATA)>
<!ELEMENT paragraph (\#PCDATA | figure)+>
<!ELEMENT figure EMPTY>
和其它示例一样，DTD 语言不包含任何有关如何识别或创建有效 XML 文档的指令。它只描述了如果文档存在，那它会是怎么样的。声明性语言采用虚拟语气。
Python 作为解释器 vs Python 作为环境
Python 库可以通过两种截然不同的方式中的一种来利用声明性语言。或许更为常用的技术是将非 Python 声明性语言作为数据来解析和处理。应用程序或库可以读入外部来源（或者是内部定义的但只用作“blob”的字符串），然后指出一组要执行的命令式步骤，这些步骤在某种形式上与那些外部声明是一致的。本质上，这些类型的库是“数据驱动的”系统；声明性语言和 Python 应用程序执行或利用其声明的操作之间有着概念和范畴差别。事实上，相当普遍的一点是，处理那些相同声明的库也被用来实现其它编程语言。
上面给出的所有示例都属于第一种技术。库 PyLog 是 Prolog 系统的 Python 实现。它读取像样本那样的 Prolog 数据文件，然后创建 Python 对象来对 Prolog 声明 建模。EBNF 样本使用专门变体 SimpleParse ，这是一个 Python 库，它将这些声明转换成可以被 mx.TextTools 所使用的状态表。 mx.TextTools 自身是 Python 的扩展库，它使用底层 C 引擎来运行存储在 Python 数据结构中的代码，但与 Python 本质上几乎没什么关系。对于这些任务而言，Python 是极佳的 粘合剂，但是粘合在一起的语言与 Python 差别很大。而且，大多数 Prolog 实现都不是用 Python 编写的，这和大多数 EBNF 解析器一样。
DTD 类似于其它示例。如果您使用象 xmlproc 这样的验证解析器，您可以利用 DTD 来验证 XML 文档的方言。但是 DTD 的语言并不是 Python 式的， xmlproc 只将它用作需要解析的数据。而且，已经用许多编程语言编写过 XML 验证解析器。XSLT 转换与此相似，也不是特定于 Python 的，而且像 ft.4xslt 这样的模块只将 Python 用作“粘合剂”。
虽然上面的方法和上面所提到的工具（我一直都在使用）都没什么 不对，但如果 Python 本身是声明性语言的话，那么它可能会更精妙，而且某些方面会表达得更清晰。如果没有其它因素的话，有助于此的库不会使程序员在编写一个应用程序时考虑是否采用两种（或更多）语言。有时，依靠 Python 的自省能力来实现“本机”声明，既简单又管用。
自省的魔力
解析器 Spark 和 PLY 让用户 用 Python 来声明 Python 值，然后使用某些魔法来让 Python 运行时环境进行解析配置。例如，让我们研究一下与前面 SimpleParse 语法等价的 PLY 语法。 Spark 类似于下面这个示例：
清单 5. PLY 样本
tokens = ('ALPHANUMS','WORDPUNCT','CONTRACTION','WHITSPACE')
t_ALPHANUMS = r"[a-zA-Z0-0]+"
t_WORDPUNCT = r"[-_]"
t_CONTRACTION = r"'(clock|d|ll|m|re|s|t|ve)"
def t_WHITESPACE(t):
r"\s+"
t.value = " "
return t
import lex
lex.lex()
lex.input(sometext)
while 1:
t = lex.token()
if not t: break
我已经在我即将出版的书籍 Text Processing in Python 中编写了有关 PLY 的内容，并且在本专栏文章中编写了有关 Spark 的内容（请参阅 参考资料以获取相应链接）。不必深入了解库的详细信息，这里您应当注意的是：正是 Python 绑定本身配置了解析（在这个示例中实际是词法分析/标记化）。 PLY 模块在 Python 环境中运行以作用于这些模式声明，因此就正好非常了解该环境。
PLY如何得知它自己做什么，这涉及到一些非常奇异的 Python 编程。起初，中级程序员会发现可以查明 globals() 和 locals() 字典的内容。如果声明样式略有差异的话就好了。例如，假想代码更类似于这样：
清单 6. 使用导入的模块名称空间
import basic_lex as _
_.tokens = ('ALPHANUMS','WORDPUNCT','CONTRACTION')
_.ALPHANUMS = r"[a-zA-Z0-0]+"
_.WORDPUNCT = r"[-_]"
_.CONTRACTION = r"'(clock|d|ll|m|re|s|t|ve)"
_.lex()
这种样式的声明性并不差，而且可以假设 basic_lex 模块包含类似下面这样的简单内容：
清单 7. basic_lex.py
def lex():
for t in tokens:
print t, '=', globals()[t]
这会产生：
% python basic_app.py
ALPHANUMS = [a-zA-Z0-0]+
WORDPUNCT = [-_]
CONTRACTION = '(clock|d|ll|m|re|s|t|ve)
PLY 设法使用堆栈帧信息插入了导入模块的名称空间。例如：
清单 8. magic_lex.py
import sys
try: raise RuntimeError
except RuntimeError:
e,b,t = sys.exc_info()
caller_dict = t.tb_frame.f_back.f_globals
def lex():
for t in caller_dict['tokens']:
print t, '=', caller_dict['t_'+t]
这产生了与 basic_app.py 样本所给输出一样的输出，但是具有使用前面 t_TOKEN 样式的声明。
实际的 PLY 模块中要比这更神奇。我们看到用模式 t_TOKEN 命名的标记实际上可以是包含了正则表达式的字符串，或是包含了正则表达式文档字符串和操作代码的函数。某些类型检查允许以下多态行为：
清单 9. polymorphic_lex
\# ...determine caller_dict using RuntimeError...
from types import *
def lex():
for t in caller_dict['tokens']:
t_obj = caller_dict['t_'+t]
if type(t_obj) is FunctionType:
print t, '=', t_obj.__doc__
else:
print t, '=', t_obj
显然，相对于用来玩玩的示例而言，真正的 PLY 模块用这些已声明的模式可以做更有趣的事，但是这些示例演示了其中所涉及的一些技术。
继承的魔力
让支持库到处插入并操作应用程序的名称空间，这会启用精妙的声明性样式。但通常，将继承结构和自省一起使用会使灵活性更佳。
模块 gnosis.xml.validity 是用来创建直接映射到 DTD 产品的类的框架。任何 gnosis.xml.validity 类 只能用符合 XML 方言有效性约束的参数进行实例化。实际上，这并不十分正确；当只存在一种明确的方式可将参数“提升”成正确类型时，模块也可从更简单的参数中推断出正确类型。
由于我已经编写了 gnosis.xml.validity 模块，所以我倾向于思考其用途自身是否有趣。但是对于本文，我只想研究创建有效性类的声明性样式。与前面的 DTD 样本相匹配的一组规则／类包括：
清单 10. gnosis.xml.validity 规则声明
from gnosis.xml.validity import *
class figure(EMPTY):   pass
class _mixedpara(Or):   _disjoins = (PCDATA, figure)
class paragraph(Some):  _type = _mixedpara
class title(PCDATA):   pass
class _paras(Some):    _type = paragraph
class chapter(Seq):    _order = (title, _paras)
class dissertation(Some): _type = chapter
您可以使用以下命令从这些声明中创建出实例：
ch1 = LiftSeq(chapter, ("1st Title","Validity is important"))
ch2 = LiftSeq(chapter, ("2nd Title","Declaration is fun"))
diss = dissertation([ch1, ch2])
print diss
请注意这些类和前面的 DTD 非常匹配。映射基本上是一一对应的；除了有必要对嵌套标记的量化和交替使用中介体之外（中介体名称用前导下划线标出来）。
还要注意的是，这些类虽然是用标准 Python 语法创建的，但它们也有不同寻常（且更简练）之处：它们没有方法或实例数据。单独定义类，以便从某框架继承类，而该框架受到单一的类属性限制。例如， <chapter> 是其它标记序列，即 <title> 后面跟着一个或多个 <paragraph> 标记。但是为确保在实例中遵守约束，我们所需做的就是用这种简单的方式来 声明chapter 类。
编写像 gnosis.xml.validity.Seq 这样的父类程序所涉及的主要“技巧”，就是在初始化期间研究 实例的 .__class__ 属性。类 chapter 自身并不进行初始化，因此调用其父类的 __init__() 方法。但是传递给父类 __init__() 的 self 是 chapter 的实例，而且 self 知道 chapter。为了举例说明这一点，下面列出了部分 gnosis.xml.validity.Seq 实现：
清单 11. 类 gnosis.xml.validity.Seq
class Seq(tuple):
def __init__(self, inittup):
if not hasattr(self.__class__, '_order'):
raise NotImplementedError, \
"Child of Abstract Class Seq must specify order"
if not isinstance(self._order, tuple):
raise ValidityError, "Seq must have tuple as order"
self.validate()
self._tag = self.__class__.__name__
一旦应用程序程序员试图创建 chapter 实例，实例化代码就检查是否用所要求的 ._order 类属性声明了 chapter ，并检查该属性是否为所需的元组对象。方法 .validate() 要做进一步的检查，以确保初始化实例所用的对象属于 ._order 中指定的相应类。
何时声明
声明性编程样式在声明约束方面 几乎一直比命令式或过程式样式更直接。当然，并非所有的编程问题都是关于约束的 － 或者说至少这并非总是自然定律。但是如果基于规则的系统（比如语法和推理系统）可以进行声明性描述，那么它们的问题就比较容易处理了。是否符合语法的命令式验证很快就会变成非常复杂难懂的所谓“意大利面条式代码”（spaghetti code），而且很难调试。模式和规则的声明仍然可以更简单。
当然，起码在 Python 中，声明规则的验证和增强总是会归结为过程式检查。但是把这种过程式检查放在进行了良好测试的库代码中比较合适。单独的应用程序应该依靠由像 Spark 或 PLY 或 gnosis.xml.validity 这样的库所提供的更简单的声明性接口。其它像 xmlproc 、 SimpleParse 或 ft.4xslt 这样的库，尽管不是 用 Python进行声明的（Python 当然适用于它们的领域），也能使用声明性样式。

