# .Net高级技术——程序集 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [.Net高级技术——程序集](https://www.cnblogs.com/xdp-gacl/p/3525546.html)



**程序集（Assembly），可以看做是一堆相关类打一个包，相当于java中的jar包（*）。**打包的目的：程序中只引用必须的程序集，减小程序的尺寸；一些程序集内部的类不想让其他程序集调用。

我们调用的类都是位于各个程序集中，如果调用的类在没有引用的程序集中，则需要添加对那个程序集的引用，比如ConfigurationManager。

类的访问级别：private、public、protected

**访问级别约束**：**子类访问级别不能比父类高（等级观念）**，因为子类能够出席的场合父类都应该能出席，如果父类可访问级别比子类低就有了父类去不了的场合**（儿子能去的地方爹一定要能去）**

**![图一](https://images0.cnblogs.com/blog/289233/201401/182205213921.png)**

类如果不标注访问级别则是internal级别，也就是只能在程序集内部访问，如果想跨程序集访问，则需要public或者使用InternalsVisibleTo（*）

可以编写自己的程序集，新建→类库，使用这个类库的程序只要引用生成的dll即可，**如果是引用同一个解决方案中类库，则最好添加项目引用。项目不能循环引用，就是****A****引用****B****，****B****又引用****A****。**

exe、dll都是程序集，只不过exe有入口而已，exe也可以被添加引用。dll不一定是程序集(VC写的dll就不是程序集)，程序集不一定是dll（.net写的EXE也是程序集）

（*）GAC（全局程序集），数字签名（防止引用的程序集被篡改）

**所有.Net类都是定义在某个Assembly（程序集）中的，.Net基本类是定义在mscorlib.dll中**。exe也可以看做是类库，也可以引用。.net的exe也是Assembly，.net中的exe和dll的区别就是exe中包含入口函数，其他没有区别，exe也可以当成dll那样引用、也可以反编译。

GAC：全局程序集缓存。公用的Assembly放到GAC中。（*）

Assembly类是对程序集的描述（*）。

(*)AppDomain.CurrentDomain.GetAssemblies()获得程序所有的Assembly

Assembly.LoadFile()，动态从文件加载Assembly，不需要在编译的时候引用。









