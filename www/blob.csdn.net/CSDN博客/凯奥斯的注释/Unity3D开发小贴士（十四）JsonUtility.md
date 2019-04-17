# Unity3D开发小贴士（十四）JsonUtility - 凯奥斯的注释 - CSDN博客





2016年11月28日 22:10:23[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：6979
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# Json是现在非常常用的数据格式，因为.Net的版本问题，所有没有很方便的方法可以直接在Unity里面使用C#官方的Json库，于是Unity3D自己提供了自己的一套Json工具——JsonUtility。

参考下面的示例：



```
using UnityEngine;
using System.Collections;

public class TestSer
{
	public int i;
	public string str;
}

public class TestDeser
{
	public string str;
	public float i;
}
public class TestDeser2
{
	public string st;
	public float f;
}

public class NewBehaviourScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
		TestSer ts = new TestSer();
		ts.i = 10;
		ts.str = "aaaa";
		string json = JsonUtility.ToJson (ts);
		Debug.Log (json); 
		TestDeser td = JsonUtility.FromJson<TestDeser> (json);
		Debug.Log (td.str);
		Debug.Log (td.i);
		TestDeser2 td2 = JsonUtility.FromJson<TestDeser2> (json);
		Debug.Log (td2.st);
		Debug.Log (td2.f);
	}
}
```




可以看出，JsonUtility使用起来很方便，而且没什么~~节操~~约束。

JsonUtility支持数组，并且支持可序列化的自定义类型，也就是需要添加System.Serializable特性（关于特性，参考[C#语法小知识（七）特性](http://blog.csdn.net/ecidevilin/article/details/52551302)）。

例如：



```
[System.Serializable]
public struct TestObj
{
	public string ttt;
}

public class TestSer2
{
	public int i;
	public string str;
	public TestObj obj;
	public int[] arr;
	public GameObject go;
}
```

序列化：


```
TestSer2 ts2 = new TestSer2();
		ts2.i = 10;
		ts2.str = "aaaa";
		ts2.obj = new TestObj ();
		ts2.obj.ttt = "tttt";
		ts2.arr = new int[3];
		ts2.arr [0] = 1;
		ts2.arr [1] = 1;
		ts2.arr [2] = 1;
		ts2.go = new GameObject ("go");
		string json = JsonUtility.ToJson (ts2);
```

打印出来的结果：

`{"i":10,"str":"aaaa","obj":{"ttt":"tttt"},"arr":[1,1,1],"go":{"instanceID":-12766}}`
毫无疑问，JsonUtility使用了C#反射（参考[C#语法小知识（十）反射](http://blog.csdn.net/ecidevilin/article/details/52694141)），所以效率上会差一点。效率方面，肯定还是微软自家的Binary和XML库会稍微好一点。（详情参考[C#语法小知识（十六）序列化与反序列化（XML）](http://blog.csdn.net/ecidevilin/article/details/52925232)和[C#语法小知识（十七）序列化与反序列化（Binary）](http://blog.csdn.net/ecidevilin/article/details/52930522)）











