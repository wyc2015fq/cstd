# hibernate学习笔记之对象的三种状态 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月16日 01:08:00[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：23标签：[hibernate](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)
个人分类：[框架](https://blog.csdn.net/weixin_40247263/article/category/7511186)



![](https://img-blog.csdn.net/20180416005650432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

假设： Student 有4个属性 Id、Name、Title、BirthDate             id为自增

  1、 new Student 对象   然后 set     Name、Title、BirthDate 这三个属性的值。

   这时状态为 Transient（内存中有、session缓存中没有、没有id）

  2、save(student)   

这时状态为 Persistent（内存中有；缓存中有[session里面有个map，key为自动生成的id、value存的是对象的引用，此时会给student对象的id赋值]；数据库中有）   这里是一级缓存，close以后里面的数据就没了，如果数据经常被查看且不需要经常修改，可以使用二级缓存。

  3、close

这时状态为Detached(内存中有；缓存没有(session被断开);数据库中有；id有)

