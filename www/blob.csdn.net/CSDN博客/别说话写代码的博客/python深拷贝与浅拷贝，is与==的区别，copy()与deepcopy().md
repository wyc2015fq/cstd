# python深拷贝与浅拷贝，is与==的区别，copy()与deepcopy() - 别说话写代码的博客 - CSDN博客





2018年11月14日 21:08:57[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：832








# **1**.is与==的区别

is是比较两个引用是否指向了同一个对象

==是比较两个对象值是否相等

![](https://img-blog.csdnimg.cn/20181114204621401.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181114204704143.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

# **2.深拷贝与浅拷贝**

浅拷贝：对对象的顶层拷贝，可以理解为增加了一个引用，没有拷贝内容，即没有新生成拷贝的内存空间，两个指的是同一块 

![](https://img-blog.csdnimg.cn/20181114205234750.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

深拷贝：对一个对象所有层次的拷贝， 相当于新增加了一块内存，把对象所有内容复制一遍

![](https://img-blog.csdnimg.cn/20181114205519175.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

深入一点理解：

![](https://img-blog.csdnimg.cn/20181114205919156.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

# **3.copy与deepcopy**

等号=默认为浅拷贝

copy默认为浅拷贝，但是它对不可变和可变类型的copy不同

deepcopy是深拷贝，需要import copy

![](https://img-blog.csdnimg.cn/20181114210557811.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181114210752342.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)





