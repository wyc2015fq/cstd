# Unity3D开发小贴士（十一）ToLua协同程序 - 凯奥斯的注释 - CSDN博客





2016年11月16日 21:47:14[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2183
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# [Unity3D开发小贴士（二）协程（Coroutine）](http://blog.csdn.net/ecidevilin/article/details/52893833)中介绍了在Unity3D中使用协同程序（C#），[Lua语法小贴士（八）协同程序](http://blog.csdn.net/ecidevilin/article/details/53001188)中介绍了使用Lua的协同程序。Lua的协同程序功能相对有限，所以ToLua（参考[Unity3D开发小贴士（三）愉快的使用Lua开发](http://blog.csdn.net/ecidevilin/article/details/53025444)）帮我们对Lua的协同程序进行了扩展。

具体的文件是ToLua/Lua/System/coroutine.lua。

它对Lua原生的coroutine进行了扩展。




coroutine.start(f, ...)

输入f为函数，...为f所需的参数，返回协程的句柄。与原生的coroutine.create方法相比，为协程增加了一个定时器，使得协程在当前帧的LateUpdate里执行。



coroutine.wait(t, co, ...)
t为秒数，co为协同程序句柄，...为等待结束继续执行时所需要的参数。


coroutine.step(t, co, ...)
t为帧数，co为协同程序句柄，...为等待结束继续执行时所需要的参数。


coroutine.www(www, co)
www为WWW的对象，co为协同程序句柄（为什么没有参数？？？），会在WWW下载完毕之后继续协程。


coroutine.stop(co)
co为协同程序。会停止协同程序相关的计时器（wait\step\www）。
示例：



```
local co
co = coroutine.start(function()
    print("coroutine")
    coroutine.wait(1,co)
    print("wait")
end)
```





