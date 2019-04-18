# foreach, 用还是不用,这是一个问题~ - tkokof1的专栏 - CSDN博客

2016年12月17日 11:55:25[tkokof1](https://me.csdn.net/tkokof1)阅读数：1150


  接触过C#循环的朋友，想来对foreach应该不会陌生，相比一般的for循环方式，foreach显得更加优雅简洁，Unity支持C#脚本，平日使用中数组列表什么的自然也会遇到不少，想来foreach定然大有用武之地呀！

  可惜网上大家的共识却是：不要用foreach！

  WTF？

  原因其实也简单，就是为了避免[GC](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))，因为foreach会“偷偷”申请内存，使用过度的话自然会引发系统的垃圾收集！

  有鉴于此，建议大家平日尽量限制使用foreach，转而使用for之类的循环控制语法，尤其注意一下Update（或者说频繁调用的函数）中的foreach使用，不小心的话确实会导致频繁GC~

  OK，基础知识普及完毕，接下来让我们再细致看下（基于Unity5.3.3f1）：  

  1. foreach真的会申请内存吗？申请多少内存？（或者叫GC Alloc）

  简单写个测试，Profiler一下就明了了~


```
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ForeachTest : MonoBehaviour 
{
    int[] m_array = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    ArrayList m_arrayList = new ArrayList { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    List<int> m_list = new List<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    void Update()
    {
        ForeachList();
    }

    void ForeachArray()
    {
        // foreach array
        foreach (var element in m_array)
        {
            Func(element);
        }
    }

    void ForeachArrayList()
    {
        // foreach array list
        foreach (var element in m_arrayList)
        {
            Func((int)element);
        }
    }

    void ForeachList()
    {
        // foreach list
        foreach (var element in m_list)
        {
            Func(element);
        }
    }

    void Func(int element)
    {
    }
	
}
```

![](https://img-blog.csdn.net/20161217115309928)

  可以看到，foreach一个List确实会产生内存申请，大小为40字节~

  2. 为什么foreach会申请内存呢？

  说到这个问题，我们便需要进一步的认识一下foreach了，相比传统的for，foreach其实是C#的一种[语法糖](https://zh.wikipedia.org/wiki/%E8%AF%AD%E6%B3%95%E7%B3%96)，还拿上面的测试程序举例，foreach一个List最后会被C#翻译为大概下面这种形式：


```
using (List<int>.Enumerator enumerator = list.GetEnumerator())
{
    while (enumerator.MoveNext())
    {
        int current = enumerator.Current;
        this.Func(current);
    }
}
```

  初看上去似乎没有什么申请内存的地方，但是注意到这里using的使用，其最后会通过IDisposable接口调用Dispose，但是由于List的Enumerator是个值类型，转换为IDisposable接口会导致[装箱操作](https://msdn.microsoft.com/zh-cn/library/yz2be5wk.aspx)，继而便引发了内存申请~

  使用[ILSpy](http://ilspy.net/)看下生成的IL便更加一目了然了：

![](https://img-blog.csdn.net/20161217115325741)

  3. foreach List会触发GC Alloc，那么其他类型的列表类型是不是一样呢？

   首先看下原生数组：

```
void ForeachArray()
{
    // foreach array
    foreach (var element in m_array)
    {
        Func(element);
    }
}
```

![](https://img-blog.csdn.net/20161217115315761)

  竟然没有产生GC Alloc？看下转换后的代码：


```
// ForeachTest
private void ForeachArray()
{
	int[] array = this.m_array;
	for (int i = 0; i < array.Length; i++)
	{
		int element = array[i];
		this.Func(element);
	}
}
```

  看来C#对于原生数组的foreach形式做了优化，使用了传统的for来遍历数组，自然便不会申请额外的内存了~

  再来试下ArrayList~


```
void ForeachArrayList()
{
	// foreach array list
	foreach (var element in m_arrayList)
	{
		Func((int)element);
	}
}
```

![](https://img-blog.csdn.net/20161217115320663)

  看来同List一样，也会产生40字节的GC Alloc，同样的看下转换后的代码：

```
// ForeachTest
private void ForeachArrayList()
{
	using (IEnumerator enumerator = this.m_arrayList.GetEnumerator())
	{
		while (enumerator.MoveNext())
		{
			object current = enumerator.get_Current();
			this.Func((int)current);
		}
	}
}
```

  形式上与foreach List如出一辙，但是值得指出的是，这里产生内存申请的地方与foreach List是不同的，foreach List如上面所说，是由于装箱操作而引起的GC Alloc，但是foreach ArrayList则是由于GetEnumerator，因为ArrayList的Enumerator 是引用类型，创建时自然会在堆上分配（也就是产生了内存分配），后面虽然也会尝试转换为IDisposable接口来调用Dispose，但是因为是引用类型间的转换，并不会引发Box~

  IL代码最能说明问题：

![](https://img-blog.csdn.net/20161217115330975)

  4. 真的不能再使用foreach了吗？

  诚然，foreach会产生内存申请，但是相对而言GC Alloc的大小还是相对有限的（上面看到是40B），所以只要不是频繁调用，这点消耗还是能够接受的；再者，如果你使用的是原生数组，那么便不用担心了，随意使用foreach即可，因为就像上面看到的那样，foreach原生数组并不会产生GC Alloc；最后，其实新版的C#早已修复了foreach会产生额外内存申请的问题，只是由于Unity内含的Mono版本较早，没有修复该问题罢了，如果你想痛快的在Unity中使用foreach，可以看看[这里](https://forum.unity3d.com/threads/upgraded-c-compiler-on-5-3-5p8.417363/)和[这里](https://bitbucket.org/alexzzzz/unity-c-5.0-and-6.0-integration/src)~

  OK，没想简单的一个foreach也讲了这么多东西，其中的知识其实网上早已有了很多优秀的解释，知乎上的一篇相关[问答](https://www.zhihu.com/question/30334270)想来应该是个不错的起点，有兴趣的朋友可以仔细看看~

  好了，下次再见吧~

