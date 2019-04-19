# C#由变量捕获引起对闭包的思考 - 文章 - 伯乐在线
原文出处： [Recluse_Xpy](http://www.cnblogs.com/CreateMyself/p/5573643.html)
# 前言
偶尔翻翻书籍看看原理性的东西确实有点枯燥，之前有看到园中有位园友说到3-6年工作经验的人应该了解的.NET知识，其中就有一点是关于C#中的闭包，其实早之前在看书时（之前根本不知道C#中还有闭包这一说）看到对于闭包的内容篇幅很少而且介绍的例子一看就懂（最终也就是有个印象而已），反正工作又用不到来让你去实现闭包，于是乎自己心存侥幸心理，这两天心血来潮再次翻了翻书想仔细研究一番（或许是出于内心的惶恐吧，工作几年竟然不知道闭包，就算知道而且仅止于了解，你是在自欺欺人么），紧接着就查了下资料来研究研究这个东西，若有错误之处，请指出。
## 话题
首先来我们来看看委托的演变进化史，有人问了，本节的主题不是【C#由变量捕获引起对闭包的思考】？哦，看的还仔细，是的，咱能别着急么，又有人问了，你之前不是写过有关委托的详细介绍么，哦，看来还是我的粉丝知道的还挺多，但是这个介绍侧重点不同啦，废话少来，进入主题才是真理。
### C#1.0之delegate
我们今天知道过来一个列表可以通过lamda如where或者predicate来实现，而在C#1.0中我们必须来写一个方法来实现predicate的逻辑，紧接着创建委托实例是通过指定的方法名来创建。我们来创建一个帮助类（ListUtil），如下：


C#
```
/// 
    /// 操作list帮助类
    /// 
    static class ListUtil
    {
        /// 
        /// 创建一个predicate
        /// 
        public static IList Filter(IList source, Predicate predicate)
        {
            List ret = new List();
            foreach (T item in source)
            {
                if (predicate(item))
                {
                    ret.Add(item);
                }
            }
            return ret;
        }
        /// 
        ///遍历列表并在控制台上进行打印
        /// 
        public static void Dump(IList list)
        {
            foreach (T item in list)
            {
                Console.WriteLine(item);
            }
            Console.ReadKey();
        }
    }
```
同时给出一个测试数据：


C#
```
static class SampleData
    {
        public static readonly IListstring> Words =
            new Liststring> { "the", "quick", "brown", "fox", "jumped",
             "over", "the", "lazy", "dog" }.AsReadOnly();    
    }
```
现在我们要做的是返回其长度小等于4的字符串并打印，给出长度小于4的方法：


C#
```
static bool MatchFourLettersOrFewer(string item)
        {
            return item.Length 4;
        }
```
下面我们在控制台调用上述方法来进行过滤：


C#
```
Predicatestring> predicate = new Predicatestring>(MatchFourLettersOrFewer);
            IListstring> shortWords = ListUtil.Filter(SampleData.Words, predicate);
            ListUtil.Dump(shortWords);
```
结果打印如下：
![](http://jbcdn2.b0.upaiyun.com/2016/06/6c9b6ffea75b037c92f177615976f96b.png)
上述一切都是so easy!当我们利用委托来实现时只是简单的进行一次调用不会多次用到，为了精简代码，此时匿名方法出现在C# 2.0.
### C#2.0之delegate
上述在控制台进行调用方法我们稍作修改即可达到同样效果，如下：


C#
```
Predicatestring> predicate = 
                delegate(string item) 
                {
                    return item.Length 4;
                };
            IListstring> shortWords = ListUtil.Filter(SampleData.Words, predicate);
            ListUtil.Dump(shortWords);
```
好了，到了这里貌似有点浪费篇幅，到这里我们反观上述代码，对于predicate中过滤数据长度都是硬编码，缺少点什么，我们首先要讲的是闭包，那对于闭包需要的可以基本概括为：闭包是函数与其引用环境组合而成的实体（来源于：你必须知道的.NET）。我们可以将其理解为函数与上下文的综合。我们需要来通过手动输入过滤数据的长度来给出一个上下文。我们给出一个过滤长度的类（VariableLengthMather）：


C#
```
public class VariableLengthMatcher
    {
        int maxLength;
        /// 
        /// 将手动输入的数据进行传递
        /// 
        /// 
        public VariableLengthMatcher(int maxLength)
        {
            this.maxLength = maxLength;
        }
        /// 
        /// 类似于匿名方法
        /// 
        public bool Match(string item)
        {
            return item.Length  maxLength;
        }
    }
```
下面我们来进行手动输入调用以此来过滤数据：


C#
```
Console.Write("Maximum length of string to include? ");
            int maxLength = int.Parse(Console.ReadLine());
            VariableLengthMatcher matcher = new VariableLengthMatcher(maxLength);
            Predicatestring> predicate = matcher.Match;
            IListstring> shortWords = ListUtil.Filter(SampleData.Words, predicate);
            ListUtil.Dump(shortWords);
```
演示如下：
![](http://jbcdn2.b0.upaiyun.com/2016/06/e7056f7ac1501494eeb5ce0ced26504f.gif)
接着我们将上述控制台代码进行如下改造：


C#
```
Console.Write("Maximum length of string to include? ");
            int maxLength = int.Parse(Console.ReadLine());
            VariableLengthMatcher matcher = new VariableLengthMatcher(maxLength);
            Predicatestring> predicate = matcher.Match;
            IListstring> shortWords = ListUtil.Filter(SampleData.Words, predicate);
            ListUtil.Dump(shortWords);
            Console.WriteLine("Now for words with ");
            maxLength = 5;
            shortWords = ListUtil.Filter(SampleData.Words, predicate);
            ListUtil.Dump(shortWords);
```
我们只是将maxLength值改变了下，再次进行打印，演示结果如下：
![](http://jbcdn2.b0.upaiyun.com/2016/06/a174169dfcce359fb85dc70d7e708749.gif)
### C#3.0之delegate
为了更好的演示代码，我们利用C#3.0中lamda表达式来进行演示，我们继续接着上述来讲，当我们将maxLength修改为5时，此时过滤的数据和4一样，此时我们利用匿名方法或者lamda表达式同样进行如上演示，如下。


C#
```
Console.Write("Maximum length of string to include? ");
            int maxLength = int.Parse(Console.ReadLine());
            Predicatestring> predicate = item => item.Length  maxLength;
            IListstring> shortWords = ListUtil.Filter(SampleData.Words, predicate);
            ListUtil.Dump(shortWords);
            Console.WriteLine("Now for words with ");
            maxLength = 5;
            shortWords = ListUtil.Filter(SampleData.Words, predicate);
            ListUtil.Dump(shortWords);
```
看看演示结果：
![](http://jbcdn2.b0.upaiyun.com/2016/06/473d2908bf9bdd230df187a6a3f34b94.gif)
从上述演示结果可以看出此时的maxLength为5，当然打印过滤的结果则不一样，这个时候就得说到第一个话题【变量捕获】。在C# 2.0和3.0中的匿名方法和lambda表达式都能捕获到本地变量。
那么问题来了，什么是变量捕获呢？我们怎么去理解呢？
我们接下来利用lambda表达式再来看一个例子：


C#
```
var name = "cnblogs";
            Func capture = () => name;
            name = "xpy0928";
            Print(capture);
```


C#
```
static void Print(Funcstring> capture)
        {
            Console.WriteLine(capture());
            Console.ReadKey();
        }
```
那么打印的结果将会是什么呢？cnblogs？xpy0928？
![](http://jbcdn2.b0.upaiyun.com/2016/06/683e18a17400b7babbbeeccbc6107f31.png)
name被捕获，当本地变量发生改变时lambda也同样作出对应的改变（因lambda会延迟执行），所以会输出xpy0928。那么编译器到底做了什么才使得输出xpy0928呢？编译器内部将上述代码进行了大致如下转换。


C#
```
public class Capture
    {
        public string name;
        public string printName()
        {
            return this.name;
        }
    }
```


C#
```
var capture = new Capture();
    capture.name = "cnblogs";
    capture.name = "xpy0928";
    Print(capture.printName);
```
到了这里想必我们能够理解了捕获变量的意义lambda始终指向当前对象中的name值即对象中的引用始终存在于lamda中。
接下来就要说到闭包，在此之前一直在讨论变量捕获，因为闭包产生的源头就是变量捕获。（个人理解，若有错误请指正）。
变量捕获的结果就是编译器将产生一个对象并将该局部变量提升为实例变量从而达到延长局部变量的生命周期，存储到的这个对象叫做所谓的闭包。
上述这句话又是什么意思？我们再来看一个例子：


C#
```
Listint>> funcs = new Listint>>();
            for (int j = 0; j 10; j++)
                funcs.Add(() => j);
            foreach (Funcint> func in funcs)
                Console.WriteLine(func());
            Console.ReadKey();
```
有人说上述例子就是闭包，对，是闭包且结果返回10个10，恩完事！不能就这样吧，我们还得解释清楚。我们一句一句来看。


C#
```
funcs.Add(() => j);
```
()=>j代表什么意思，来我们来看看之前lambda表示式的六部进化曲：
![](http://jbcdn2.b0.upaiyun.com/2016/06/2a4f3fef7bbd272be621a13a73a67a07.png)
实例化一个匿名委托并返回j值，注意这里说()=>j是返回变量j的当前值而非返回值j。返回的匿名委托为一个匿名类并访问此类中的属性j（为什么说返回的匿名委托为一个匿名类，请看此链接：[http://www.cnblogs.com/jujusharp/archive/2011/08/04/C-Sharp-And-Closure.html](http://www.cnblogs.com/jujusharp/archive/2011/08/04/C-Sharp-And-Closure.html)） 。好说完这里，我们再来解释为什么打印10个10？
此时创建的每个匿名委托都捕获了这个变量j，所以每个匿名委托即匿名类保持了对字段j的引用，当for循环完毕时即10时此时字段值全变为10，直到j不被匿名委托所引用，j才被会垃圾回收器回收。
我们再来看看对上述进行修改：


C#
```
Listint>> funcs = new Listint>>();
            for (int j = 0; j 10; j++)
            {
                int tempJ = j;
                funcs.Add(() => tempJ);
            }
            foreach (Funcint> func in funcs)
                Console.WriteLine(func());
            Console.ReadKey();
```
很明显将输出0-9因为此时创建了一个临时变量tempJ，当每次进行迭代时匿名委托即lambda捕获的是不同的tempJ，所以此时能按照我们预期所输出。
我们再来看一种情况：


C#
```
for (int j = 0; j 10; j++)
            {
                Funcint> fun = () => j;
                Console.WriteLine(fun());
            }
```
此时还是正常输出0-9，因为此时lambda表达式每次迭代完立即执行，而不像第一个例子直到延迟到循环到10才开始进行所有的lambda执行。
## 总结
闭包概念：闭包允许你将一些行为封装，将它像一个对象一样传来递去，而且它依然能够访问到原来第一次声明时的上下文。这样可以使控制结构、逻辑操作等从调用细节中分离出来。
作用：（1）利于代码精简。（2）利于函数编程。（3）代码安全。
参考资料：
C# in Depth：[http://csharpindepth.com/Articles/Chapter5/Closures.aspx](http://csharpindepth.com/Articles/Chapter5/Closures.aspx)
Variable Capture in C# with Anonymous Delegates：[http://www.digitallycreated.net/Blog/34/variable-capture-in-c%23-with-anonymous-delegates](http://www.digitallycreated.net/Blog/34/variable-capture-in-c%23-with-anonymous-delegates)
Understanding Variable Capturing in C#：[https://blogs.msdn.microsoft.com/matt/2008/03/01/understanding-variable-capturing-in-c/](https://blogs.msdn.microsoft.com/matt/2008/03/01/understanding-variable-capturing-in-c/)
C#与闭包：[http://www.cnblogs.com/jujusharp/archive/2011/08/04/C-Sharp-And-Closure.html](http://www.cnblogs.com/jujusharp/archive/2011/08/04/C-Sharp-And-Closure.html)
