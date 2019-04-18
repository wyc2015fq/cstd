# C#性能优化黑科技（一）【反射】修改属性的快速方法 - 凯奥斯的注释 - CSDN博客





2017年08月20日 12:50:34[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2397
所属专栏：[C#性能优化黑科技](https://blog.csdn.net/column/details/16695.html)









# 当我们在撸码的时候，不可避免的要使用[反射](http://blog.csdn.net/ecidevilin/article/details/52694141)来修改[属性](http://blog.csdn.net/ecidevilin/article/details/52525080)的值。我们都知道这种反射是太阳一般的慢，如果属性类型是值类型，那么就会因为装箱拆箱产生额外的GC（[垃圾回收](http://blog.csdn.net/ecidevilin/article/details/71189108)）。那么加快速度并减少GC呢，本文将介绍一个名为“CreateDelegate”的黑科技。




设我们有这么一个类：



```
public class Test
{
	public int id { get; private set;}
}
```


那么当我们要在类型的外部修改id值的时候，该怎么办呢？~~当然是把set前面的private去掉，本文终。~~某些时候，我们因为限制而无法修改类型的内容（例如使用第三方库，再例如为了保证编程规范）， 那么我们就需要这么写：



```
//For test  
        Test a = new Test ();  
        Test b = new Test ();  
        Type type = typeof(Test);  
        PropertyInfo pi = type.GetProperty ("id");    
        pi.SetValue (a, -1, null);  
        pi.SetValue (b, -2, null);
```


SetValue方法的第三个参数是为了[索引器](http://blog.csdn.net/ecidevilin/article/details/52525080)准备的，我们先不管。


第一个参数是该类型（Test）的对象（a或b）。

第二个参数就是我们将要设置的值，类型是object。

这TM就很尴尬了，我们都知道，int类型转object会进行装箱操作，而真正赋值的时候又会进行拆箱操作，一大波GC正在袭来。

而且这个方法慢，非常慢，出了名的慢，因为在真正赋值之前，会进行一系列的类型判断。

那么，如何解决这些问题？我们就需要使用黑科技——Delegate.CreateDelegate，代码很简单：



```
var act = Delegate.CreateDelegate (typeof(Action<Test, int>), pi.GetSetMethod (true)) as Action<Test, int>;  
        act (a, 1);  
        act (b, 2);
```



stackoverflow上有更完整的解决方案（https://stackoverflow.com/questions/6158768/c-sharp-reflection-fastest-way-to-update-a-property-value/16082916#16082916）。


顺手贴上代码：



```
public static Func<S, T> BuildGetAccessor<S, T>(Expression<Func<S, T>> propertySelector)  
{  
    return propertySelector.GetPropertyInfo().GetGetMethod().CreateDelegate<Func<S, T>>();  
}  
  
public static Action<S, T> BuildSetAccessor<S, T>(Expression<Func<S, T>> propertySelector)  
{  
    return propertySelector.GetPropertyInfo().GetSetMethod().CreateDelegate<Action<S, T>>();  
}  
  
// a generic extension for CreateDelegate  
public static T CreateDelegate<T>(this MethodInfo method) where T : class  
{  
    return Delegate.CreateDelegate(typeof(T), method) as T;  
}  
  
public static PropertyInfo GetPropertyInfo<S, T>(this Expression<Func<S, T>> propertySelector)  
{  
    var body = propertySelector.Body as MemberExpression;  
    if (body == null)  
        throw new MissingMemberException("something went wrong");  
  
    return body.Member as PropertyInfo;  
}
```

```
public class Accessor<S>  
{  
    public static Accessor<S, T> Create<T>(Expression<Func<S, T>> memberSelector)  
    {  
        return new GetterSetter<T>(memberSelector);  
    }  
  
    public Accessor<S, T> Get<T>(Expression<Func<S, T>> memberSelector)  
    {  
        return Create(memberSelector);  
    }  
  
    public Accessor()  
    {  
  
    }  
  
    class GetterSetter<T> : Accessor<S, T>  
    {  
        public GetterSetter(Expression<Func<S, T>> memberSelector) : base(memberSelector)  
        {  
  
        }  
    }  
}  
  
public class Accessor<S, T> : Accessor<S>  
{  
    Func<S, T> Getter;  
    Action<S, T> Setter;  
  
    public bool IsReadable { get; private set; }  
    public bool IsWritable { get; private set; }  
    public T this[S instance]  
    {  
        get  
        {  
            if (!IsReadable)  
                throw new ArgumentException("Property get method not found.");  
  
            return Getter(instance);  
        }  
        set  
        {  
            if (!IsWritable)  
                throw new ArgumentException("Property set method not found.");  
  
            Setter(instance, value);  
        }  
    }  
  
    protected Accessor(Expression<Func<S, T>> memberSelector) //access not given to outside world  
    {  
        var prop = memberSelector.GetPropertyInfo();  
        IsReadable = prop.CanRead;  
        IsWritable = prop.CanWrite;  
        AssignDelegate(IsReadable, ref Getter, prop.GetGetMethod());  
        AssignDelegate(IsWritable, ref Setter, prop.GetSetMethod());  
    }  
  
    void AssignDelegate<K>(bool assignable, ref K assignee, MethodInfo assignor) where K : class  
    {  
        if (assignable)  
            assignee = assignor.CreateDelegate<K>();  
    }  
}
```





那么最后再来一道课后题：


如果我们只能得到类型的Type，并不能直接使用类型，那该怎么办呢？

（我已经得到了一个不算很成熟的解决方案，有缘我再拿出来讨论吧。）






