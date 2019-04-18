# 编程小知识之 Random接口返回值 - tkokof1的专栏 - CSDN博客

2018年11月21日 19:39:54[tkokof1](https://me.csdn.net/tkokof1)阅读数：74


> 
本文简述了 Random 接口返回值的一些相关信息

平日工作中,(伪)随机数的使用一定是避不开的,拿 C# 为例,System 命名空间下的 Random 类型一般都是我们生成(伪)随机数的第一选择:

```
var rand = new System.Random();
var val = rand.NextDouble();
```

Unity 中也在 UnityEngine 命名空间下提供了自己的 Random 类型,不过接口方法上都是静态类型:

```
var val = UnityEngine.Random.value;
```

上述的接口方法使用上似乎都很简单,但是深入一下接口细节,可能就不是所有同学都了解了,甚至还会产生混淆:

*System.Random 的 NextDouble 方法返回的(伪)随机数的范围是多少? 那么 UnityEngine.Random.value 呢?*

很多同学可能都知道答案: **0.0 ~ 1.0**

那么接着问:

*这些返回值包含 0.0 和 1.0 这两个端点吗?*

到这里可能有些同学就会有些混淆了,答案是这样的:

System.Random 的 NextDouble 的返回值范围 >= 0.0 并且 < 1.0, 即 **[0.0, 1.0)**

UnityEngine.Random.value 的返回值范围 >= 0.0 并且 <= 1.0, 即 **[0.0, 1.0]**

两者的返回值范围不同导致了混淆的产生,使用时不加注意的话自然也会导致错误.

这里将相关常用的 Random 方法以表格列出,方便记忆和查阅:
|类型|方法|返回值|
|----|----|----|
|System.Random|Next()|[0, int.MaxValue)|
|System.Random|Next(int max)|[0, max)|
|System.Random|Next(int min, int max)|[min, max)|
|System.Random|NextDouble()|[0.0, 1.0)|
|UnityEngine.Random|Range(int min, int max)|[min, max)|
|UnityEngine.Random|Range(float min, float max)|**[min, max]**|
|UnityEngine.Random|value|**[0.0f, 1.0f]**|

注意到下面一点可能有助于我们的记忆:

Random 接口方法的返回值范围**一般都是左闭右开**的,仅 **UnityEngine.Random** 的 **Range(float min, float max)** 和 **value** 两个方法的返回值范围是**全闭区间**.

#### 参考
- [System.Random Doc](https://docs.microsoft.com/en-us/dotnet/api/system.random?view=netframework-4.7.2)
- [UnityEngine.Random Doc](https://docs.unity3d.com/ScriptReference/Random.html)

