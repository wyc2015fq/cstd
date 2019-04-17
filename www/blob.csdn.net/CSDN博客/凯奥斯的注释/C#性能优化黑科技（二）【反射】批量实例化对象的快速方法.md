# C#性能优化黑科技（二）【反射】批量实例化对象的快速方法 - 凯奥斯的注释 - CSDN博客





2017年08月24日 17:50:12[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1793
所属专栏：[C#性能优化黑科技](https://blog.csdn.net/column/details/16695.html)









在实际开发中，我们有时候会遇到这样的问题，我们知道某一个类的Type，如何实例化这个对象？

答案很简单：


`object obj = Activator.CreateInstance(type);`
但是如果是需要批量实例化对象（例如读取自定义格式的配置文件），这种方法在效率和GC上都不太理想。（Mono2.0）


我注意到System.Object有一个方法，叫做MemberwiseClone，但是它是protected的，也就是只有衍生类在内部才能访问。那我们只有用反射把它拿出来：



```
object obj = Activator.CreateInstance(type);
MethodInfo mcMi = type.GetMethod("MemberwiseClone", BindingFlags.NonPublic | BindingFlags.Instance);
List<object> list = new List<object>(100);
for (int i = 0; i < 100; i++)
{
    list.Add(mcMi.Invoke(obj, null));
}
```


减少了些许GC，效率提升了一倍多。


还有更进一步的方法，效率上可以提升很多。就是我在上一篇（[C#性能优化黑科技（一）【反射】修改属性的快速方法](http://blog.csdn.net/ecidevilin/article/details/77427806)）提到的方法CreateDelegate：



```
object obj = Activator.CreateInstance(type);
MethodInfo mcMi = typeof(object).GetMethod("MemberwiseClone", BindingFlags.NonPublic | BindingFlags.Instance);
Func<object, object> mcAct = Delegate.CreateDelegate(typeof(Func<object,object>), mcMi) as Func<object, object>;
List<object> list = new List<object>(100);
for (int i = 0; i < 100; i++)
{
    list.Add(mcAct(obj));
}
```

数据量较大的时候，效率可以提升10倍左右，因为Func<object, object>直接调用的就是MemberwiseClone这个方法，省去了MethodInfo.Invoke调用过程中参数类型获取和检查等操作。不过GC还是一样的。


