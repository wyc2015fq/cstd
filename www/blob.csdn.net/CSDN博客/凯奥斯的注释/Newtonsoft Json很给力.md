# Newtonsoft.Json很给力 - 凯奥斯的注释 - CSDN博客





2017年06月07日 12:20:10[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：970
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









如题，比Unity的JsonUtility和MiniJson都给力。支持属性，支持泛型（Generic）集合（Collection）。

github地址

https://github.com/JamesNK/Newtonsoft.Json





本来是想写点什么，拖了很久也没想说来要写点儿啥。虽然Newtonsoft.Json给力，但是我们项目里并没有大范围的使用到它，所以对它的了解也不算很深~~（暂时也没打算花时间去深入了解它）~~。那么也就简单写一下我所接触到的几个点。

1、不需要[Attribute](http://blog.csdn.net/ecidevilin/article/details/52551302)修饰或实现[interface](http://blog.csdn.net/ecidevilin/article/details/52497378)。只要是public的成员变量或[Property](http://blog.csdn.net/ecidevilin/article/details/52525080)都可以被序列化。

例如Protobuf生成的数据结构，用Newtonsoft.Json序列化就很方便。

2、可以递归序列化。如果一个类里的成员变量或[Property](http://blog.csdn.net/ecidevilin/article/details/52525080)类型也是一个类，也可以被正确序列化。

3、支持List<T>和Dictionary<TKey,TVal>等[泛型](http://blog.csdn.net/ecidevilin/article/details/52832174)的[集合](http://blog.csdn.net/ecidevilin/article/details/53369809)。

4、开放了一些设置，可以对一些情况进行特殊处理，在JsonSerializerSettings这个类里。

我们项目里只用到了NullValueHandling，也就是空值处理。默认情况下，会把空值也序列化成null到Json里，有些时候我们是不需要的，所以可以设置为忽略空值（例如cjson里有个设定，就是把空值解析成lightuserdata，这种设定肯定有它的道理，但是对于我们项目来说是多余的）。

5、可以自定义某种（些）类型的转换器。

举个例子好了，我们的项目里需要在Json里把长整型long（也就是int64）转换为字符串，那么就可以自定义一个继承自JsonConverter的转换器，在读写Json的时候对长整型进行特殊处理。




这里就不贴代码了，如果有需要使用Json的同学可以去下载下来试一试，暂时不需要的，有个印象也不错。



