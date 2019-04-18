# 随便聊聊Lambda - tkokof1的专栏 - CSDN博客

2017年09月11日 21:11:52[tkokof1](https://me.csdn.net/tkokof1)阅读数：170


C#用多了,对于[Lambda](https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/statements-expressions-operators/lambda-expressions)肯定不陌生,作为创建[委托](https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/delegates/)的一种简易方式,使用上确实相当顺手,不过用的多了,难免也会踩坑,下面这个坑可能就有不少朋友踩过:

```
var count = 8;
var Func = new List<Action>(count);

for (int i = 0; i < count; ++i)
{
    Func.Add(() => { Console.WriteLine(i); });
}

foreach (var action in Func)
{
    action();
}
```

代码很简单,使用List缓存了一些委托然后执行,那么执行的结果是什么呢?一些朋友可能会脱口而出: *0 1 2 3 4 5 6 7*

可惜正确答案却是: *8 8 8 8 8 8 8 8* ! 原因在于Lambda表达式中**捕获的i是同一个引用变量!**

代码改成这样也许会更清晰一些(这里i被写在了循环外):

```
var count = 8;
var Func = new List<Action>(count);

int i = 0;
for (i = 0; i < count; ++i)
{
    Func.Add(() => { Console.WriteLine(i); });
}

foreach (var action in Func)
{
    action();
}
```

为了避免这个问题,我们需要创建不同的局部变量(这里我们通过创建多份index来缓存中间数值):

```
var count = 8;
var Func = new List<Action>(count);

for (int i = 0; i < count; ++i)
{
    int index = i;
    Func.Add(() => { Console.WriteLine(index); });
}

foreach (var action in Func)
{
    action();
}
```

实际上这个坑在C#5之后就被[填上](http://www.mindscapehq.com/blog/index.php/2012/03/18/what-else-is-new-in-c-5/)了,只是现实来看,你可能还没法用上C#5(例如Unity目前仍不支持),所以在平时使用Lambda时还是要多留下心~

OK,闲聊结束,下次再见吧~

