# 随便再聊一点点Coroutine（确实只是一点点~） - tkokof1的专栏 - CSDN博客

2016年06月22日 21:25:39[tkokof1](https://me.csdn.net/tkokof1)阅读数：666标签：[C#																[Game																[Unity																[Coroutine																[Script](https://so.csdn.net/so/search/s.do?q=Script&t=blog)](https://so.csdn.net/so/search/s.do?q=Coroutine&t=blog)](https://so.csdn.net/so/search/s.do?q=Unity&t=blog)](https://so.csdn.net/so/search/s.do?q=Game&t=blog)](https://so.csdn.net/so/search/s.do?q=C#&t=blog)
个人分类：[随性																[游戏](https://blog.csdn.net/tkokof1/article/category/642710)](https://blog.csdn.net/tkokof1/article/category/642708)


  之前写过一点Coroutine相关的东西（[这里](http://blog.csdn.net/tkokof1/article/details/11842673)和[这里](http://blog.csdn.net/tkokof1/article/details/12834939)），大致讲了些自己关于Unity协程的理解，自己在平日的工作中也确实用到了不少相关的知识，遂而引发了一个比较细节或者说微妙（subtle）的思考：StartCoroutine中的协程是否会立即执行？

  考虑以下代码：


```
IEnumerator TestCoroutine()
{
	Debug.LogError("Before");

	yield return null;

	Debug.LogError("After");
}

void StartCoroutineAuto()
{
	StartCoroutine(TestCoroutine());
	Debug.Log("End");
}
```

  这段代码所输出的序列是：”Before, End, After” 还是 “End, Before, After” 呢？

  结果很容易验证，正确的答案应该是前者，即”Before, End, After”，这种结果我一度认为可能是Unity本身StartCoroutine的实现问题，感觉上即便StartCoroutine中的协程是延迟执行的，好像也不存在多大的问题~

  后来自己在这个问题上踩了坑，才发现这种立即执行协程的实现方式其实是经过考虑的，因为只有这种方式才能准确的实现协程逻辑：

  考虑以下代码：


```
IEnumerator TestCoroutineV2()
{
	yield return new WaitForSeconds(1); // 1
	Debug.LogError("1 second elapsed ?"); // 2
}
```

  如果1处的代码是延迟执行的，那么程序运行到2时，实际的使用时间其实并不是一秒钟，而是有差不多一帧的延迟，在此这一帧的延迟也许并不紧要，但在其他情况下便不见得如此了~

  假设你正在使用协程编写一个倒计时程序，你的代码可能是这样的：

```
IEnumerator TestCoroutineV3(float leftTime)
{
	while (leftTime > 0)
	{
		// some logic here

		yield return new WaitForSeconds(1); // 1
		leftTime -= 1; // 2
	}
}
```


  如果1处是延迟执行的，那么每一遍循环，逻辑在2处就有大约一帧的延迟，如果leftTime有60秒的话，实际的倒计时可能要花60 + 60 * （1 / fps）秒钟，对于一般30帧的游戏来讲，就是62秒钟，如果leftTime有数百秒的话，那么倒计时与真实时间的差距可能是分钟级的！

  延迟执行的协程会导致逻辑执行步调不如预期，于是相关问题便产生了，并且可能越积越大！这就是为什么我们需要立即执行协程的原因（之一）~

  OK，以上便是那一点点想聊的东西~


