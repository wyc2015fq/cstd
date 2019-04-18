# c# - javalzy - 博客园
# [c#](https://www.cnblogs.com/javalzy/p/4332367.html)
**1.  C# 类型基础**
http://www.cnblogs.com/JimmyZhang/archive/2008/01/31/1059383.html
**2.  C#中的反射原理及应用**
http://blog.163.com/xuanmingzhiyou@yeah/blog/static/1424776762011612115124188/
3. Unity3D中的多线程及使用多线程
http://www.unitymanual.com/3821.html
4. Unity3D协同程序（Coroutine）
http://blog.csdn.net/kongbu0622/article/details/8775037
5. Render的作用？描述MeshRender和SkinnedMeshRender的关系与不同 
Skinned Mesh原理解析和一个最简单的实现示例
http://blog.csdn.net/n5/article/details/3105872
### 【精】从游戏脚本语言说起，剖析Mono搭建的脚本基础
http://www.open-open.com/news/view/1e69f47
foreach vs For 
foreach 语句为[数组](http://baike.baidu.com/view/209670.htm)或对象集合中的每个元素重复一个嵌入语句组。foreach 语句用于循环访问集合以获取所需信息，但不应用于更改集合内容以避免产生不可预知的副作用。
oreach效率比for低主要分2个角度说。
2个地方，一个是.net 1.1之前的对值类型的装箱，一个是每次调用GetEnumator方法的函数调用带来的时间消耗，单一次都不消耗时间，但经过大量循环放大后，时间消耗比较明显。
.net 1.1之后的版本，foreach对值类型已经不装箱，不慢了，因为有了yield关键字。
但函数调用带来的堆栈创建内存分配则不可避免。
绝对意义上，for比foreach快，但从.net 1.1之后，这个差距缩小到多一层函数调用而已，不是特别严格的地方，还是用foreach好一点。因为foreach不止可以访问一个数组或List这样循环时能确定长度的集合，也可以访问可迭代的类型，对于一些不需要最开始就确定长度的，这样甚至效率更高，因为不需要在循环开始之前就准备好要循环的数据，而是每次foreach循环获取下一个数据。

