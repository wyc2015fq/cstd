# Erlang 之父学习 Elixir 语言的一周 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李鼎](http://www.jobbole.com/members/oldratlee) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
// 本文基于开源中国社区的译文稿： [用`Elixir`的一周](https://www.oschina.net/translate/a-week-with-elixir)
## 译序
作为`Erlang`之父*Joe Armstrong*，对`Erlang VM`上的新语言`Elixir`给出很精彩评论和思考。在『特定领域的专家的专业直觉』、『编程语言设计的三定律』、『管道运算符避免恶心代码』、『`Elixir`的`sigil`引出的程度语言如何定义/解释字符串』等等这些讨论上，能强烈感受到*Joe Armstrong*老黑客风范。
[自己](http://weibo.com/oldratlee)理解粗浅，而本文讨论是语言设计，且作为老一代黑客的作者计算机领域中那被我们现在不再要去理解使用的主题和思想（如`Prolog`/`DCG`、`Lisp`/宏、`sigil`、不可变闭包、语言设计的兼容性）真是信手拈来，翻译中肯定会有不少不足和不对之处，译文源码在[`GitHub`上《与Elixir相处的一周》](https://github.com/oldratlee/translations/blob/master/a-week-with-elixir/README.md)，欢迎建议（[提交Issue](https://github.com/oldratlee/translations/issues)）和指正（[Fork后提交代码](https://github.com/oldratlee/translations/fork)）！
PS：为什么要整理和审校翻译 参见 [译跋](https://github.com/oldratlee/translations/blob/master/a-week-with-elixir/translation-postscript.md)
# 与 Elixir 相处的一周
差不多一周前我开始看[`Elixir`](http://elixir-lang.org)，关于`Elixir`之前只有些模糊的了解，没打算花时间去看细节。
但在得知 *Dave Thomas *出版了 *[Programming Elixir](http://pragprog.com/book/elixir/programming-elixir)*这本书的消息后，我的想法就彻底改变了。*Dave Thomas *帮我修订过我的那本`Erlang`的书并且作为`Ruby`的倡导者做得非常出色，所以要是 *Dave *对一样东西产生了兴趣，那说明这样东西的有趣性是毫无疑问的。
*Dave*对`Elixir`很感兴趣，在他的书里这样写道：
> 
在1998年的时候，由于我是`comp.lang.misc`邮件组的忠实读者，机缘巧合得知了`Ruby`，然后下载、编译、与`Ruby`坠入爱河。 （没听过`comp.lang.misc`？那去问问你老爹吧。） 就像任何一次相爱经历一样，你很难解释原因是什么。 `Ruby`的工作方式和我心里想的灵犀默契，而且总是有足够的深度持续点燃着我的热情。
回首已经逝去15年的时光，而我无时无刻不在寻找下一个也能给出这样感觉的新『对象』。
很快我遇上了`Elixir`，由于一些原因，我没能一见钟情。 但在几个月前，和*Corey Haines*聊了一次，在如何不用那些学院派的书给大家介绍哪些有吸引力的函数式编程概念这个问题上诉了些苦。 他告诉我再去看看`Elixir`。我照做了，有了第一次看到`Ruby`时那样的感觉。
我能理解这种感觉，一种先行于逻辑的内心感性的感觉。就像我知道一件事是对的，我却并不知道我是如何和为什么知道这是对的。而对原因的解释常常在几周甚至几年后才显露出来。*Malcolm Gladwell *在他的 *[Blink: The Power of Thinking Without Thinking](https://www.amazon.com/Blink-Power-Thinking-Without/dp/0316010669/ref=sr_1_1?s=books&ie=UTF8&qid=1369995752&sr=1-1&keywords=blink)*一书中曾探讨过这个问题。一个特定领域的专家常常能瞬间感知出一些事情是否正确，但却不能解释为什么。
但得知*Dave*与`Elixir`『看对眼』时，我很想知道为什么他会这样。
无独有偶，*Simon St. Laurent*也出了本`Elixir`的书。*Simon*的 *[Introducing Erlang](http://www.amazon.com/Introducing-Erlang-Simon-St-Laurent/dp/1449331769)*一书表现不俗，我和他还通过邮件沟通过几次，还有有些熟悉的。而且*Pragmatic Press*和*O’Reilly*出版社都在争着要出版`Elixir`，我知道在`Erlang VM`上有事要发生，而我自己还没注意到。毫无疑问我Out了！
我发封邮件给*Dave*和*Simon*，他们爽快地借给我了样书，现在可以开始阅读了……谢了二位……
# 上周我下载了`Elixir`然后开始学习……
没多久我觉得就上手了。确实是个好货！有趣的是`Erlang`和`Elixir`两者在在底层一样的，对于我来说『感觉』是一样的。事实上也确实这样，两者都会被编译成`EVM`(`Erlang Virtual Machine`)指令 —— 实际上`EVM`这个叫法之前没人用，都叫成`Beam`，但为了和`JVM`区分开，我觉得是时候开始用`EVM`这个叫法了。
`Erlang`和`Elixir`为什么有相同的『语意』？这得从虚拟机底层谈起。垃圾回收行为，不共享并发机制，底层的错误处理和代码加载机制都是一致的。当然这些肯定都是一致的：他们都运行在相同的`VM`里。这也是`Scala`和`Akka`区别于`Erlang`的原因。`Scala`和`Akka`都运行在`JVM`之上，垃圾回收和代码加载机制从根本上就不一样。
你直接看到的`Elixir`是完全不同的上层语法，源自`Ruby`。看起来不那么『可怕』语法和很多附加的甜点。
`Erlang`的语法源自`Prolog`，并受到`Smalltalk`、`CSP`和函数式编程的影响很大。`Elixir`则受到`Erlang`和`Ruby`的影响很大。从`Erlang`借鉴了模式匹配（`pattern matching`）、高阶函数（`higher order function`）以及整个进程（`process`）和任其崩溃的（`let it crash`）错误处理（`error handling`）机制。从`Ruby`借鉴了`sigil`和快捷语法（`shortcut syntax`）。当然也有自创的甜点，像`|>`管道操作符（`|> pipe operator`），让人想到`Prolog`的`DCG`和`Haskell`的`Monad`（尽管相比要简单不少，更类似于`Unix`的管道操作符），还有宏的引用和反引用操作符（`macro quote and unquote operator`，对应的是`Lisp`的反引号和逗号操作符）。
> 
【译注】：
***`sigil`***是指在变量名中包含符号来表达数据类型或作用域，通常作为前缀，如`$foo`，其中`$`就是个`sigil`。 像本文中说的例子，`sigil`也可以能对常量加上字母符号，`r"abc"`，其中`r`是`sigil`，把字符串转成正则表达式。 详见[`wikipedia`词条`sigil`](https://en.wikipedia.org/wiki/Sigil_(computer_programming))
***`DCG`***（`definite clause grammar`），确定性子句语法，表达语法的一种方式，可以用于自然语言或是形式化语言，比如像`Prolog`这样逻辑编程语言。基本的`DCG`用于描述『是什么』和『有什么特性』（简单的可以认为逻辑编程程序员要做的就是给出这些描述；剩下的事是逻辑引擎会根据描述的规则生成算法，然后得出解来）。像这样：


```
sentence --> noun_phrase, verb_phrase.
noun_phrase --> det, noun.
```
不展开说明了，对于没有了解过`Prolog`/逻辑编程的同学意会一下就好，不用纠结了。详见[`wikipedia`词条`Definite clause grammar`](https://en.wikipedia.org/wiki/Definite_clause_grammar)
译文使用英文术语本身，不翻译成中文，有更好的辨识度。
`Elixir`还提供一个新的下层`AST`，取代了每个`form`都是独有表示的`Erlang AST`，`Elixir AST`有一个统一得多的表示，这使得元编程（`meta-programming`）要简单得多。
`Elixir`的实现出奇的可靠，尽管有几个地方和我预想的不一样。字符串插值（`string interpolation`）的工作方式有时候不好使（字符串插值是个很棒的想法) ：


```
IO.puts "...#{x}..."
```
对`x`求值后把`x`友好格式化的表示（`a pretty-printed representation`）插入到字符串中。但是只对简单形式的`x`可行。
这点可以通过从`Elixir`调用`Erlang`的方式很简单就能解决掉。
`IO.puts "...#{pp(x)}..."`这样就总是可行的。我只是把`pp(x)`定义成


```
def pp(x) do
    :io_lib.format("~p", [x])
    |> :lists.flatten
    |> :erlang.list_to_binary
end
```
用`Erlang`则表述成:


```
pp(X) ->
  list_to_binary(lists_flatten(li_lib:format("~p"), [X])))
```
很『显然』这和`Elixir`的版本表述是一样。当然`Elixir`的写法要更容易阅读。上面用到的`|>`操作符意思是把`io_lib:format`的结果输入到`lists:flatten`，然后再到`list_to_binary`。就像好用的老家伙`Unix`的管道符`|`。
`Elixir`打破了一些`Erlang`神圣信条 —— 在顺序结构中变量可重绑定（`re-bound`）。实际上这也是可以做到的，因为最终结果还是可以规范化成静态单赋值（`static-single-assignment`，`SSA`）的形式。尽管在顺序结构中这是可以的，但在循环结构中，一定肯定以及确定不要这么做。但这不是个问题，因为`Elixir`木有循环，只有递归。实际上`Elixir`不可能在循环中包含可变的变量（`mutable variables`），因为这样编译出来的东西在下层的`EVM`是支持不了的。顺序结构的`SSA`变量挺好的，`EVM`知道如何对其做优化。但在循环结构不行，所以`Elixir`没有这么做。关于这方面的优化甚至可以更往下挖到`LLVM`汇编器（`LLVM assembler`） —— 但又是另一个很长的故事先就此打住吧。
# 0. 编程语言设计的三定律
- 你做对的，无人为你提。
- 你做错的，有人跟你急。
- 难于理解的，你必须一而再再而三地去给人解释。
一些语言有的设计做得太好，结果大家都懒得去提，这些好的设计是正确的、是优雅的，是易于理解的。
对于错误的设计，你完了。你成了2B，如果好设计比坏设计多，你可能被原谅。你想在以后干掉这些坏设计，却因为向后兼容性或者是有些SB已经用上所有那些坏设计写上了1T行代码，结果你是改不了了。
而难以理解的部分才是真正无赖。你必须一而再再而三地解释，直到你吐血，可还是有些人永远不懂，你必须写上百邮件和数千文字来一遍又一遍地地解释这是什么意思以及为什么会如此。对于一个语言的设计者或作者来说，这是个痛苦的深渊。
下面我要说到的几件事，我认为也会落入这三类情况中。
在我开始前，我先要说，`Elixir`做对了灰常灰常多对的事情，而且远远多于错的。
关于`Elixir`有利的是，要改正它的错误还不算晚。这只能在无数代码行被写下和众多程序员开始使用它之前才能做到 —— 所以解决这些问题的时日并不多了。
# 1. 在源文件中没有版本
`XML`文件总是这样开始的：


```
<?xml version="1.0"?>
```
这点非常好。读取`XML`文件的第一行就像是听到拉赫玛尼诺夫的第三钢琴协奏曲的第一小节（【译注】：指其富有辨识度）。这是一个令人赞叹的经验。赞美`XML`设计师，愿他们的名字得到荣光，给这帮伙计颁图灵奖吧。
所有源文件中加上语言的版本是必要的。为什么呢？
早期的`Erlang`没有列表推导（`list comprehension`）。如果我们对一个新版的`Erlang`模块用一个旧的`Erlang`编译器去编译。新版的代码含有列表推导，但旧编译器并不知道列表推导，所以旧编译器会认为这是一个语法错误。
如果 **版本3**`Erlang`编译器处理这样开始的文件：


```
-version(5, 0).
```
则可以给出这样提示信息：
> 
啊～～～～咦～～～～
哦，烦炸了，我只是版本3的编译器，看不懂未来。
你刚刚给我一个版本5的程序，这说明我在地球上的寿命已过。
你将不得不杀掉我，把我卸载，然后安个版本5的新编译器。曾经玉树临风的我现在没了价值，我将不再存在。
再见吧，老朋友。
我感觉头痛。我要休息一下……
这是数据设计的第一法则：
> 
所有未来可能会改变的数据应该标记上版本号。
而 模块 ***是*** 数据。
# 2. `fun`和`def`不同
在写*Programming Erlang*一书时*Dave Thomas*问函数为什么不能输入到`shell`里。
如果模块里有这样的代码：


```
fac(0)            -> 1;
fac(N) when N > 0 -> N * fac(N-1).
```
不能直接复制到`shell`里运行，得到相同的结果。*Dave*问这是为什么，并说这样很傻。
在`Lisp`等其它语言主中这做是没问题的。*Dave*说了『这很会让人很迷惑』类似这样的话 —— 他说的对并且这确实让人迷惑了。在论坛里关于此的问题肯定有成百上千条。
我解释这个问题已经无数遍了，从黑发解释到白发，我现在头发真白了就是因为这个原因。
原因是`Erlang`的一个`bug`。
- `Erlang`的模块是一系列的 **`FORM`** 。
- `Erlang``shell`解析的是一系列 **`EXPRESSION`** 。
- 但`Erlang`的`FORM` 不是 **`EXPRESSION`** 。


```
double(X) -> 2*X.            in an Erlang module is a FORM
Double = fun(X) -> 2*X end.  in the shell is an EXPRESSION
```
上面两个是**不**同的。这小点愚蠢成了`Erlang`一个永远的痛，当时我们没有注意到，到了现在我们就只能学会和它相处。
在`Elixir`模块可以这么写


```
def triple(x) do
   3 * x;
end
```
估计很多人都会直接从编辑器复制到`shell`里直接运行，然后收到的是出错信息：


```
ex> def triple(x) do 3*x; end
** (SyntaxError) iex:66: cannot invoke def outside module
```
如果你不解决这个问题就要花后面20年的时间去解决为什么 —— 就像`Erlang`曾经所做的。
顺便说一下，修复这个问题真的真的很简单。我在`erl2`作为了尝试就解决了。`Erlang`中没法修复这个问题 (版本兼容问题)，所以我就在[`erl2`](https://github.com/joearms/erl2)解决。只需要***`erl_eval`***的小改和解析器的几个微调。
主要原因是`FORM`不是`EXPRESSION`，所以加了个关键字**`def`**。


```
Var = def fac(0) -> 1; fac(N) -> N*fac(N-1) end.
```
这就定义了一个有副作用的表达式。由于是个表达式，可以在`shell`中求值了，记住在`shell`中只能对表达式求值。
副作用指的是需要创建一个`shell:fac/1`功能（就像在模块中定义的一样）。


```
iex> double = fn(x) -> 2 * x end;
iex> def double(x) do 2*x end;
```
上面两者应该是一致的，并且**都是**定义一个名为**`Shell.double`**的函数。
做了这样的修改，妈妈再也不用担心我会白头了。
# 3. 函数名称中有个额外的点号


```
iex> f = fn(x) -> 2 * x end
#Function<erl_eval.6.17052888>
iex> f.(10)
20
```
在学校里我学会了写***`f(10)`***来调用函数而不是***`f.(10)`*** —— 这是个『真正』的函数，函数名是**`Shell.f(10)`**（一个在`shell`中定义的函数）。`shell`部分是***隐式***的，所以可以只用***`f(10)`***来调用。
如果这点你置之不理，那就等着用你生命的接下来的二十年去解释为什么吧。等着在数百论坛里的数千封邮件吧。
# 4. 发送操作符


```
Process <- Message
```
这是啥玩意？你知道从`occam-pi`转成`Elixir`有多难么。
这点让你现在在失去`occam-pi`社区路上。发送操作符就应该是**`!`**，像这样：


```
Process ! Message
```
接下来的一周，我的大脑会变成浆糊，我的神经网络要被重编程，这样我才能『看到』`<-`时才能反应成`!` —— 这点不是在说如何我思考，而是指要重编程我更深植在脊柱里无意识反应。发送操作符已经不在我大脑里，而是在我的脊柱里。我的大脑想着『发送一个消息给一个进程』并发送信号给我的手指，我的脊柱马上加上**`!`**，接着大脑要**回退删除**这个字符改成`<-`。
这是一个语法问题。让人爱恨交织的语法。如果10分制的评级标准，10代表『非常非常烂』，1代表『好吧，我可以适应』的话，这个问题我给3分。
这点会使`Occam-pi`的程序员很难转到`Elixir`，什么，只需要简单地使用**`!`**就能完成`<-`的功能？这可真是出人意料啊。相信会有很多人受到鼓舞的。
# 5. 管道运算符
这是一个很好很好的东西并且很简单就能掌握，以至于没人会给你称赞。这就是生活。
这是来自`Prolog`语言的隐性基因（`recessive gene`）：`monad`。 在`Prolog`中的基因是显而易见的, 但是在`Erlang`中确实不明显的（`Prolog`的儿子）但是又在`Elixir`（`Prolog`的儿子的儿子）中重新显现了。（【译注】：隔代遗传）
`x |> y`意味着调用了`x`然后获取了`x`的输出并且将它作为`y`的另外一个参数（第一个参数）。
所以


```
x(1, 2) |> y(a, b, c)
```
等价于下面的代码：


```
newvar = x(1, 2);
y(newvar, a, b, c);
```
这**非常**有用。假设我们要把握的是把一个变量`abc`转换为`Abc`。在`Elixir`中没有利用的函数但是还有一个功能，就是去控制一个字符串。所以我们需要现将这个变量转换为`string`，在`Erlang`中，我们可以这样写：


```
capitalize_atom(X) ->
    list_to_atom(binary_to_list(capitalize_binary(list_to_binary(atom_to_list(X))))).
```
这样的写法太惊悚了。我们还可以写成这样：


```
capitalize_atom(X) ->
    V1 = atom_to_list(X),
    V2 = list_to_binary(V1),
    V3 = capitalize_binary(V2),
    V4 = binary_to_list(V3),
    binary_to_atom(V4).
```
但是，这更糟 —— 好恶心的代码。像这样德性的代码我都不知道写过多少次了！浪费我大把的青葱岁月。
于是`|>`来了：


```
X |> atom_to_list |> list_to_binary |> capitalize_binary
  |> binary_to_list |> binary_to_atom
```
为什么我认为`|>`是隐性基因？
`Erlang`从`Prolog`中演化而来，而且`Elixir`也继承了`Erlang`。
`Prolog`有`DCG`，所以


```
foo --> a, b, c.
```
扩展后的形式：


```
foo(In, Out) :- a(In, V1), b(V1, V2), c(V2, Out).
```
这基本上是同样的想法。我们通过新加一个额外的隐藏参数把函数调用序列的输入输出串接起来了。这类似`Haskell`中`monad`用法，但做得很隐秘。
`Prolog`有`DCG`，`Erlang`没有，`Elixir`有管道操作符！
# 6. `Elixir`有`sigil`
`sigil`很棒 —— 爱之。我们应该加到`Erlang`里。
字符串是一个编程抽象。编程语言都有字符串常量，通常使用双引号包着的一串字符。就像这样的一行代码：


```
x = "a string"
```
编译器会转换成字符串的内部表示，关联上对应的语义。
在`Erlang`里


```
X = "abc"
```
表示『`X`是字符`a, b, c`的`ASCII`码值对应的整数的列表』。
但也可以选择成任何其它我们想要的含义。在`Elixir`里，`x = "abc"`代表`x`是一个`UTF8`编码二进制（`binary`）（【译注】：`binary`是`EVM`的内置类型）。通过在双引号前面加上`r`可以改变字符串含义成和`Erlang`一样：


```
X = r"...."
```
当然也可以被定义成代表编译过的正则表达式，也就是说和等价于`X = re:compile("...")` —— 基于我们确定字符串的含义，可以以不同的方式去解释（`interpret`）内容。可以写上这样的代码：


```
A = "Joe",
B = s"Hello #{A}".
```
`B`值可以是`Hello Joe` —— 这里`sigil``s`改变字符串常量解释行为，『替换变量的值并插入』。
`Elixir`在这方面做得很好，定义了很多不同的`sigil`。
`Elixir`的`sigil`语法不太一样，如下：


```
%C{.....}
```
`C`是单个字符（【译注】：`Erlang`中大写开头的是变量不是常量，`C`是单个字符，表示可以是`a`、`b`、`$`），后面跟着一对`{}`或`[]`。
`sigil`很棒。`Erlang`本可以在15年前就有这个功能，而现在也可以引入，并且不会带来向后兼容的问题。
# 7. `docstring`
大爱`docstring`。
但有个小意见。请把`docstring`放到函数定义***里面***。
`Elixir`是这样：


```
@doc """
    ...
"""
def foo do
    ...
end
```
放到函数***里面***会是这样：


```
def foo do
  @doc """
  ...
  """
end
```
否则成了『没有归属的注释』（`detached comment`）：当你编辑程序时，就可能出这样的问题。注释会与它要注释函数脱离开。
在`Erlang`里，没有办法确定注释的是下一个函数还是上一个函数，或是模块。如果注释的对象是函数那就应该放到函数***里面***而不是外面。
# 8. `defmacro`引用
爱之。在解析转换这个正确阶段所做的正确的事。这让可以让人舒舒服服得不用去知道抽象语法了。引用（`quote`）和反引用（`unquote`）为你把魔法都做好了。
这就是那种是对的事 —— 非常棒却真真儿难于解释。就像`Haskell`的`monad` —— 啊哈，`monad`真很容易解释，难怪有上千篇文章来解释它有多简单。
`Elixir`宏真是简单 —— 引用（`quote`）对应`Lisp`的反引号（`quasiquote`），反引用（`unquote`）对应`Lisp`的列表逗号操作符（`list comma operator`） —— 这就我说的简单 :-)
# 9. 额外的符号
像这样：


```
iex> lc x inlist [1, 2, 3], do: 2*x
[2, 4, 6]
```
而不是这样：


```
iex> lc x inlist [1, 2, 3] do: 2*x
** (SyntaxError) iex:3: syntax error before: do
```
列表后面额外的冒号让人迷惑。
# 10. 奇怪的空白符


```
iex> lc x inlist [1, 2, 3], do : 2*x
** (SyntaxError) iex:2: invalid token: : 2*x
```
哎呦～ 一定要是`do:`，`do :`不行。
个人认为，空白符（`whitespace`）就是空白符。在字符串里面不能随便添加。在字符串外面，为了格式化代码我可以按自己喜好添加空白，这样可以让代码更美观。
但`Elixir`不能这么做 —— 不讨我喜欢。
# 11. 闭包行为完全正确 —— 哦耶
`Elixir`的闭包（`closure`）（即`fn`表达式）和`Erlang`完全一样。
`fn`表达式有一个很好的特性：能捕获所在作用域的任何变量的当前值（换句话说：能创建不可变的闭包（`immutable closure`）），这点令人难以置信的有用。需要说一下，`JavaScript`在这点上非常错误。给一个`JavaScript`和`Elixir`的例子，方便看到这点上的差异：


```
js> a = 5;
5
js> f = function(x) { return x+a }; 
function (x){return x+a}
js> f(10)
15
js> a = 100
100
js> f(10)
110
```
啥！函数`f`被打破了。定义的`f`，开始使用；修改了变量`a`有副作用打破了函数`f`。函数式编程的好处之一就是使程序变得容易推理。如果`f(10)`的值是15，那么就应该一直是15，不应该能在其它的地方打破。
`Elixir`呢? 闭包的处理是对的：


```
iex> a = 5
5
iex> f = fn(x) -> x + a end
#Function<erl_eval.6.17052888>
iex> f.(10)
15
iex> a = 100
100
iex> f.(10)
15
```
正确的闭包只应该包含不可变数据的指针 (`Erlang`中数据正是不可变的) —— 而不是可变数据的指针。如果闭包里有指向可变数据的指针，后面修改了数据就会破坏闭包的一致性。这样的结果就是不能把程序并行化，甚至顺序执行的代码也会诡异的错误。
在传统语言里要创建合适的闭包的代价会很高，因为捕获环境里的所有变量都需要做深拷贝，但`Erlang` 和`Elixir`不用这样，数据都是不可变的。你所要做的就是引用需要的数据。内部实现是通过指针引用数据（指针对程序员是不可见的），并且不再有指针引用的数据会被垃圾回收掉。
在`shell`中可以有闭包，但不能写到模块里。
在`shell`里，如果可以这样写


```
a = 10;
f = fn(x) -> a + x end;
```
为什么不能在模块里这样写呢？


```
a = 10;
def f(x) do
   a + x
end
```
这个问题完全是可以解决的，我在`erlang2`语言实验并解决了。
# 最后
这就是我与`Elixir`的相处一周，非常兴奋的一周。
`Elixir`没有令人生畏的语法，融合了`Ruby`和`Erlang`优秀的特性。它不是`Erlang`也不是`Ruby`，有自己创新的想法。
这是门新兴的语言，但在语言的开发的同时介绍的书也同步在写了。第一本介绍`Erlang` 的书在`Erlang`被发明后7年才出现，而畅销书更是在14年后才出现。用21年的时间去等一本真正的介绍书籍实在是太长了。
*Dave*很喜欢`Elixir`，我也觉得很酷，我想我们会在使用过程中找到更多乐趣的。
像是`WhatsApp`这个应用和全世界一半手机网络的关键部分都是搭建在`Erlang`之上。当技术变得更加亲和，当新一批热衷者进入阵营，让我现在怀着非常欣喜的心情关注着后续要发生的变化。
这是篇即兴的文章。也许会有些不妥之处，欢迎大家指正。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/03/c196266f837d14e0b693f961bee37b66.jpg)![](http://www.jobbole.com/wp-content/uploads/2016/03/fa42b816950b79e3c969c637657845ad.jpg)
