# Unity3D开发小贴士（二）协程（Coroutine） - 凯奥斯的注释 - CSDN博客





2016年10月22日 19:01:04[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1114
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# 在Untiy3D中，我们可以使用协程来完成一些类似于多线程的功能。但协程并不是线程，它仍然在主线程里执行，只不过是由Unity3D调用的而已（类似于Update方法）。

首先我们举个简单的例子，实现延迟销毁的功能（Destroy方法的一个重载版本实现了这个功能，我们这里只是为了方便示例）：



```
using UnityEngine;
using System.Collections;

public class DelayedDestroyTest : MonoBehaviour {
	public float delayedSeconds = 5f;
	private IEnumerator coroutineEnumerator;
	// Use this for initialization
	void Start () {
		coroutineEnumerator = DelayedDestroy ();
		StartCoroutine (coroutineEnumerator);
	}

	IEnumerator DelayedDestroy()
	{
		Debug.Log (string.Format("Game Object will be destroyed after {0} seconds", delayedSeconds));
		yield return new WaitForSeconds (delayedSeconds);
		Destroy (this.gameObject);
	}
}
```




执行DelayedDestroy方法，返回了一个IEnumerator（至于为什么要使用一个私有变量，后面会介绍到），然后以这个返回值为参数调用StartCoroutine方法，就开启了一个协程。

重点在DelayedDestroy方法里：

第一行打印了一行log，这一行没有也没关系。

第二行yield return了一个WaitForSeconds类型的实例，这就告诉了Unity，在5秒（delayedSeconds）之后，再回来执行后面的代码。

第三行执行的Destroy方法（5秒之后）。




将它挂载到Unity的一个对象上面，点击运行，默数5秒，就会发现这个对象被销毁了。




我们再举一个例子，一个渐隐（Fade）的功能：



```
using UnityEngine;
using System.Collections;

[RequireComponent(typeof(Renderer))]
public class CoroutineFadeTest : MonoBehaviour {

	// Use this for initialization
	void Start () {
		Material mat = GetComponent<Renderer> ().material;
		StartCoroutine ("Fade", mat);
	}

	IEnumerator Fade(Material mat)
	{
		Color col = mat.color;
		while (col.a > 0f)
		{
			col.a -= 0.01f;
			mat.color = col;
			yield return null;
		}
	}
}
```

我们可以在场景里面创建一个Cube，注意要把它的材质改为Sprites-Default（为了方面测试），然后挂载上CoroutineFadeTest的脚本。


点击运行，你会发现这个Cube会慢慢变透明，直到看不见。道理跟上面的相似，只不过我们使用了StartCoroutine的重载方法，参数为string类型的方法名（并且可以穿参数——mat）。




如果我们要停止协程怎么办？这就需要使用StopCoroutine方法了，在CoroutineFadeTest的类里面添加Update方法：



```
// Update is called once per frame
	void Update () {
		if (Input.GetKey (KeyCode.S)) {
			StopCoroutine ("Fade");
		}
	}
```
运行的时候按下S键，渐隐就会停止了。




那么延迟销毁的功能里该怎么写呢？貌似使用跟上面相同的方法不行呢？

网上说，StopCoroutine只能终止使用了StartCoroutine的string参数的重载版本开启的线程（有本书上也是这么说的）(⊙o⊙)…好吧，这就厉害了。

不过本文前面挖了个坑，正好现在用到了。




```
void Update () {
		if (Input.GetKey (KeyCode.S)) {
			Debug.Log ("stop");
			StopCoroutine (coroutineEnumerator);
		}
	}
```

道理就不多说了，应该很容易理解。


注意：StartCoroutine和StopCoroutine的参数类型要对应。




其实还有一种办法，使用yield break。在类里面添加一个bool值breakCoroutine，Update方法里面，当按下S的时候，设置breakCoroutine为true。然后修改DelayedDestroy方法：



```
IEnumerator DelayedDestroy()
	{
		Debug.Log (string.Format("Game Object will be destroyed after {0} seconds", delayedSeconds));
		yield return new WaitForSeconds (delayedSeconds);
		if (breakCoroutine) {
			yield break;
		}
		Destroy (this.gameObject);
	}
```


个人不太喜欢这种用法。





最后补上yield return后面可以跟的对象类型：

null：会在下一帧所有的Update方法执行之后再执行后面的代码。

WaitForEndOfFrame：会在本帧结束时且这一帧显示在屏幕上之前……


WaitForSeconds：等待sec（输入参数）秒之后……

WaitForFixedUpdate：会在下一次所有的FixedUpdate方法执行完之后……

WWW：在WWW下载完成之后……

StartCoroutine：等另一个协程执行过一次之后……


WaitUntil：当输入的参数（委托）为true的时候……

WaitWhile：当输入的参数（委托）为false的时候……

（关于委托，参考[C#语法小知识（四）委托delegate](http://blog.csdn.net/ecidevilin/article/details/52486015)和[C#语法小知识（十二）匿名方法与Lambda表达式](http://blog.csdn.net/ecidevilin/article/details/52832557)）




注：yield return返回的值跟协程方法（DelayedDestroy和Fade）的返回值并不相同。（参考[C#语法小知识（十四）迭代器（IEnumerable/IEnumerator/yield）](http://blog.csdn.net/ecidevilin/article/details/52903335)）



