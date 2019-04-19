# 在C++中实现Python的切片 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Halal](http://www.jobbole.com/members/deanyanpeng) 翻译，[Daetalus](http://www.jobbole.com/members/daetalus) 校稿。未经许可，禁止转载！
英文出处：[ericniebler](http://ericniebler.com/2014/12/07/a-slice-of-python-in-c/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
### **在C++中实现Python切片**
本文描述了一个最近包含在我的[Range-v3](https://github.com/ericniebler/range-v3)库中的巧妙方法：一个具有精炼语法的类似Python切片的工具。从功能的角度来看，这并没有什么惊天动地的，但在库设计中，却是一个有趣的小案例，同时，它也很好地说明了我的库设计理念。
### **Python切片**
在Python中，切分容器，也即是创建一个连续子域的视图，它使用一个非常简洁的语法，比如：

Python
```
>>> letters = ['a', 'b', 'c', 'd', 'e', 'f', 'g']
>>> letters
['a', 'b', 'c', 'd', 'e', 'f', 'g']
>>> # access a subrange with a slice operation
>>> letters[2:5]
['c', 'd', 'e']
>>> # replace some values
>>> letters[2:5] = ['C', 'D', 'E']
>>> letters
['a', 'b', 'C', 'D', 'E', 'f', 'g']
```
第5行，我们使用语法letters[2,5)得到列表letters在半开区间[2,5)中的元素。简洁明了。第8行，我们使用切片赋值，最终改变了列表letters。如此证明Python切片具有引用的语义。
Python切片能做到的还不止这些。你可以省略偏移量，让Python使用智能缺省：

Python
```
>>> # A missing first offset means "from the beginning"
>>> letters[:5]
['a','b','C', 'D', 'E']
>>> # A missing end offset means "to the end"
>>> letters[5:]
['f','g']
```
你甚至可以使用负的偏移量从末尾切片：

Python
```
>>> # Take the last two elements:
>>> letters[-2:]
```
这都很酷很方便。
### **Range-v3中C++实现的旧式切片**
我的range-v3库已经实现切片操作很久了，但它不是那么强大，语法也不太酷：

C++
```
using namespace ranges;
auto letters = view::iota('a','g');
std::cout << letters << 'n';
// prints: {a,b,c,d,e,f,g}
std::cout << (letters | view::slice(2,5)) << 'n';
// prints: {c,d,e}
```
上面代码中，view:iota 是一个生成从’a’到’g’（包含’g’）的字符视图，view::slice是一个从偏移量2到偏移量5（不包含5）的元素视图。与Python的切片相比，这种切片是轻量、非持有的。
这种语法本身并不糟糕，但肯定没有Python的有趣。而且view:slice不接受负的偏移量以实现从末尾切片。所以无论如何也并不强大。
### **Range-v3中C++实现的新式切片**
首先，我想找到一个创建切片的精炼形式，所以我借鉴了[array_view提议](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4177.html)，此提议有一个真正的智能语法来索引多维数组。下面是提议中给出的一个例子：

C++
```
char a[3][1][4] {{{'H', 'i'}}};
auto av = array_view<char, 3>{a};
// the following assertions hold:
assert((av.bounds() == bounds<3>{3, 1, 4}));
assert((av[{0, 0, 0}] == 'H'));
```
第1-2行声明了一个3-D字符数组，然后创建它的3-D视图。第5行是奇迹发生的地方，它使用一个看起来有点陌生的语法av[{0,0,0}]访问在位置(0,0,0)的元素。这究竟是什么呢？
其实很简单：对统一初始化语法的一个新的使用而已。考虑如下类型：

C++
```
struct indices
{
    std::size_t i, j, k;
};
struct my_array_view
{
    double & operator[](indices x);
};
```
现在我便可以使用av[{0,0,0}]语法来索引my_array_view对象，多么简洁！
我意识到我可以使用这个技巧，给大家提供一个超级短小精悍的域切片语法。


```
using namespace ranges;
auto letters = view::iota('a','g');
std::cout << letters << 'n';
// prints: {a,b,c,d,e,f,g}
std::cout << letters[{2,5}] << 'n';
// prints: {c,d,e}
```
嘿，这还不错！
### **从末尾切片—一个困境**
那还不够充分，我想要方便的从末尾切片的功能。但是从库的设计观点来看，这里变得有点趣味。并不是所有的域类型（range types）支持从末尾切片。要明白我的意思，你可以考虑一个从istream读入的ints域。这是一个输出域。直到得到它你才能知道它的末尾，也就意味着直到N个元素传递给它你才能知道N个元素中的最后一个。
换句话说，下面的代码明显不合理：

C++
```
using namespace ranges;
// An input range of ints read from cin
auto ints = istream<int>(std::cin);
// I'm sorry, I can't do that, Dave:
std::cout << ints[{0,-2}] << 'n';
```
istream返回的输入流在编译阶段知道它不能从末尾切片。但是偏移量是正值还是负值却是一个运行时的属性，因此在编译阶段不能检测。这将造成运行时出错，呃。
更糟糕的是，关于哪一类域可以接受负的偏移量的规则出奇的微妙。考虑上面的代码的这种变化：

C++
```
using namespace ranges;
// Take the first 10 ints read from cin:
auto ints = istream<int>(std::cin) | view::take(10);
// This should work! It should take the first 8 ints:
std::cout << ints[{0,-2}] << 'n';
```
这里例子中，我们已经从输入流中取出10个整数。Ints域依然是一个输入域，但是是一个具有确定大小的输入域。现在我们可以从末尾开始切片了，因为我们知道它的末尾的位置。
如果我们有一个已知的域，我们总是可以从末尾切片，尽管我们不知道末尾在哪（例如：一个null结尾的字符串），通过计算序列的长度，然后从前面开始推进distance减N个元素（虽然这并不总是最有效的方法来实现这一点）。
如果域是无穷大的，你永远不要指定一个负的偏移量，永远，永远不要。
还可以变得更加微妙：如果两个偏移量都是负的，或者两个偏移量都是非负的，则所得到的切片在O(1)内得到它的大小。否则，它只能在被切片的域知道自身大小的情况下得到它的大小。当O(1)大小的一个域是类型系统的一部分时，便开启各种优化。如果我们直到运行时才知道偏移的符号，我们不能再返回一个标榜自己为已知大小的类型。
我的意思是，对于什么时候可以从末尾进行切片的规则是微妙的，过于微妙以至于直到运行时才会报告错误，而且这样做使得宝贵的优化变得一文不值。
### **从末尾切片—一种方法**
我想出的一个解决办法是，使用一个无条件的断言来禁止负的偏移。但是在你对我感到气愤前，请稍等一下。我添加了一个替代语法来表示一个从末尾开始的偏移。看看这个：

C++
```
using namespace ranges;
auto letters = view::iota('a','g');
std::cout << letters << 'n';
// prints: {a,b,c,d,e,f,g}
std::cout << letters[{2,end-2}] << 'n';
// prints: {c,d,e}
```
我们用end-2表示距离末尾的第二个元素，替代使用一个负的偏移，这里的end是什么呢？它是一个你可以调用的end函数，用来获得一个迭代的结尾（想想std::end），只有在我的库中它不是一个函数，而是一个函数对象。（了解更多我为什么选择把begin和end当做全局的函数对象，而不是简单的函数，可以看下我的这篇[定制设计的要点](http://ericniebler.com/2014/10/21/customization-point-design-in-c11-and-beyond/)）。由于end 是一个对象，我便可以定义一个重载运算符operator-，这个运算符的左边是end，右边是一个整型数据。这样返回的某种类型的对象可以使得从末尾开始的偏移成为类型系统的一部分。

C++
```
struct from_end { int i; };
from_end operator-( decltype(ranges::end), int i )
{
    assert(i >= 0); // No funny business, please
    return {i};
}
```
现在我可以在我的域类型上定义一个重载运算符operator[]，运算符接收一个std:pair<int,from_end>:

C++
```
struct my_range
{
    // callable as rng[{2,end-2}]
    slice_view<my_range>
    operator[](std::pair<int, from_end> p)
    {
        // ... slicing happens here
    }
};
```
瞧！现在得到一个从末尾开始的一个切片，我可以用一个简短、可读的语法和编译时的类型检测，而且不会因此错过任何优化机会。
### **好的，但是**
这样很好，但是像”rng[{2,-2}]”这样的代码仍然会编译，并且在运行时出错。怎样让这种情况变好点呢？现在的差异是传递一个负偏移去切片总是造成一个运行时的错误。即使设想域类型支持负偏移操作，但这么做也不可能成功满足你的愿望。用户很快会发现这不是正确的处理方法。
如果我们允许负的偏移有时有效，有时无效，这样会使接口变得更加危险。用户可能在尝试中得到正确结果，然后便错误地得出这样总是有效的结论。他们在部署应用之后将很难发现他们的错误。
它引出了我的库的设计哲学：
> 
I can’t keep people from writing bad code. But I’m guilty of collusion if I make it easy.
我不能阻止人们写出糟糕的代码，但是如果我使他们很容易做成那样的事情，我就是同谋。
关于这个问题的一个推论：
> 
If you can’t make something succeed consistently, it’s better to make it fail consistently.
如果你不能使得某些事情一直成功，那最后让它一直失败。
希望你能喜欢这个库设计中的小案例的学习。
### **致谢**
感谢钱德勒-卡鲁斯引导我注意Python切片操作的简洁、出色。
### **脚注：**
在C++容器中，索引操作只允许随机存取容器，即元素可以在O(1)内存取。这里，我允许用户使用一个类似索引的符号进行域的切片，尽管可能是一个O(N)的操作。我现在还不确定切片与索引是否显著不同，所以我的决定不一定正确。欢迎大家给出自己的想法。
