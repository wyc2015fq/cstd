# Unity3D开发小贴士（九）序列化和反序列化Dictionary - 凯奥斯的注释 - CSDN博客





2016年11月13日 22:12:17[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：6052
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# 我们知道在Unity里面Dictionary是不能被序列化和反序列化的，如果想要对它进行这两种操作，就需要借助于一个接口ISerializationCallbackReceiver。我们需要创建以Dictionary的派生类，并继承这个接口。



```
[System.Serializable]
public class SerializableDictionary<TKey, TValue> : Dictionary<TKey, TValue>, ISerializationCallbackReceiver
{
	[SerializeField]
	private List<TKey> _keys = new List<TKey>();
	[SerializeField]
	private List<TValue> _values = new List<TValue>();

	public void OnBeforeSerialize()
	{
		_keys.Clear();
		_values.Clear();
		_keys.Capacity = this.Count;
		_values.Capacity = this.Count;
		foreach (var kvp in this)
		{
			_keys.Add(kvp.Key);
			_values.Add(kvp.Value);
		}
	}

	public void OnAfterDeserialize()
	{
		this.Clear();
		int count = Mathf.Min(_keys.Count, _values.Count);
		for (int i = 0; i < count; ++i)
		{
			this.Add(_keys[i], _values[i]);
		}
	}
}
```

Serializable是一个表示类型可序列化的特性，相关知识可以参考[C#语法小知识（七）特性](http://blog.csdn.net/ecidevilin/article/details/52551302)和[C#语法小知识（十七）序列化与反序列化（Binary）](http://blog.csdn.net/ecidevilin/article/details/52930522)。


Unity借用了System里的这个特性，在编辑器里对类型进行序列化和反序列化，用于保存编辑器的配置，并在运行的时候读取。

SerializeField是Unity的特性，表示这个（private或者protected）成员变量是要被序列化的。


ISerializationCallbackReceiver是Unity提供的，可以在序列化/反序列化过程中监听回调的一个接口，需要实现OnBeforeSerialize和OnAfterDeserialize两个方法。可以参考Unity官方文档[ISerializationCallbackReceiver](https://docs.unity3d.com/ScriptReference/ISerializationCallbackReceiver.html)。




