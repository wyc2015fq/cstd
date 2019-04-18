# Unity3D开发小贴士（八）tolua库 - 凯奥斯的注释 - CSDN博客





2016年11月12日 23:11:05[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2033
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









ToLua插件自带了一个tolua库，这个库可以让我们更灵活的使用Lua。

tolua库里包含以下方法：


|gettime|获取系统时间|
|----|----|
|typename|获取对象的类型名称|
|setpeer|设置替身|
|getpeer|获取替身|
|getfunction|获取函数|
|initset|初始化set“访问器”|
|initget|初始化get“访问器”|
|int64|生成一个int64对象|
|uint64|生成一个uint64对象|





setpeer在[Unity3D开发小贴士（七）Lua里扩展C#对象](http://blog.csdn.net/ecidevilin/article/details/53134392)使用过，实际上相当于为userdata设置了一个lua代理（proxy）——table，所有调用userdata的方法都会调用table的同名函数，访问或者设置userdata的属性时也会访问或设置table的变量。
initset和initget也在[Unity3D开发小贴士（七）Lua里扩展C#对象](http://blog.csdn.net/ecidevilin/article/details/53134392)使用过，这两个方法是为一个table的成员变量设置get和set“访问器”。它们实际上都是返回了一个表，暂命名为gettable和settable，当访问table里的变量的时候，会调用gettable里的同名函数（如果有），而设置table的里的变量的时候，会调用settable里的同名函数（如果有）。当然这些同名函数可以由你自己实现。

int64和uint64这两个方法相当于调用了int64库和uint64库的new方法。Lua5.3已经支持了这两种数据类型，而对于旧版本的Lua来讲，这两个方法实际上返回的是userdata，不过这两种userdata可以像正常的数一样加减乘除、取余、取相反数、乘方，还有比较和打印，此外还提供了tostring和equals函数，以及（不怎么好用的）tonum2函数。tonum2会返回两个数，第二个数是右移32位的值，第一个数是剩下的值（&0xFFFFFFFF）。



