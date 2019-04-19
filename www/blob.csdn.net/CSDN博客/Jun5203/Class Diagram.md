# Class Diagram - Jun5203 - CSDN博客
2018年10月03日 14:52:23[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：193
所属专栏：[UML](https://blog.csdn.net/column/details/26918.html)
### 前言
类图是面向对象系统建模中最常用的图，它是定义其他图的基础。类图主要支持系统的功能需求，也就是系统要提供给最终用户的服务。
### 芝士
- 什么是类图？
类图是描述类、接口、协作以及它们之间关系的图，用来显示系统中各个类的静态结构，它描述的是一种静态关系。
- 类图的元素
- 类
类是对一组具有相同属性、操作、关系和语义的对象的描述。
类用矩形表示
![在这里插入图片描述](https://img-blog.csdn.net/20181002213220684?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 类名称（ClassName）
类名称是一个文本串，可分为简单名称和路径名称
- 属性（Attribute）
它描述了嘞在软件系统中代表的事物所具备的特性，这些特性是所有的对象所共有的。
在UML中类的属性的语法为：`【可见性】 属性名 【：类型】 【=初始值】 【{属性字符串}】`- 可见性：主要包括公有（Public）“+”、私有（Private）“—”、受保护（Protected）“#”三种。
- 属性名：单子属性名小写；多个单词的属性名，这些单词要合并，且除了第一个单词外其余单词的首字母要大写
- 类型：用来说明该属性是什么数据类型
- 初始值：设定初始值能够保护系统的完整性，防止漏掉取值或被非法的值破坏系统的完整性；为用户提供易用性
- 属性字符串：用来指定关于属性的其他信息
- 操作（Operation）
类的操作是对类的对象所能做的事务的抽象。
在UML中类操作的语法为：`【可见性】 操作名 【（参数表）】 【：返回类型】 【{属性字符串}】`- 可见性：主要包括公有（Public）“+”、私有（Private）“—”、受保护（Protected）“#”、包内共有（Package）“~”四种。
- 操作名：类似属性名
- 参数表：按顺序排列的属性定义了操作的输入
- 返回类型：返回类型是可选的，绝大部分编程语言返回类型至多一个，具体的编程语言一般要加一个关键字“void”来表示无返回值。
- 接口
- 协作
- 关系
- 依赖（Dependency）关系
![在这里插入图片描述](https://img-blog.csdn.net/20181003091357406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
举例
![在这里插入图片描述](https://img-blog.csdn.net/20181003142134742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 泛化（Generalization）关系
理解为继承关系
![在这里插入图片描述](https://img-blog.csdn.net/20181003142836465?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
举例
![在这里插入图片描述](https://img-blog.csdn.net/20181003144722337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 单重继承
一个子类有且只有一个父类
![在这里插入图片描述](https://img-blog.csdn.net/2018100314315153?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 多重继承
一个子类有两个或两个以上的父类
![在这里插入图片描述](https://img-blog.csdn.net/20181003143200303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
话说多重继承容易导致二义性，这是为什么呢？原因如下：
根据上述例子，Vehicle有V，LandVehicle和WaterVehicle从Vehicle继承了V，而AmphibiousVahicle继承了LandVehicle和WaterVehicle它们两个的V，这就存在了歧义。
- 实现（Realization）关系
![在这里插入图片描述](https://img-blog.csdn.net/2018100314344855?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 关联（Association）关系
简单的说就是如果两事物间存在连接，这些事物的类间必定存在着关联关系
![在这里插入图片描述](https://img-blog.csdn.net/20181003090120789?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181003090146552?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再举个小例子：
![在这里插入图片描述](https://img-blog.csdn.net/20181003090236783?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 普通关联：是指两个类之间有结构关系
- 聚合：整体和部分是松耦合的关系
简单理解：整体和个体可以单独存在
例：电视机和遥控器
![在这里插入图片描述](https://img-blog.csdn.net/20181003091031130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 组合 ：整体和部分是紧耦合的关系
简单理解：先有整体后有个体，二者不可分割
例：列表和节点
![在这里插入图片描述](https://img-blog.csdn.net/20181003090952727?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
纵观上述来看，这么多的关系，到底该怎么用呢？有没有什么记忆方法呢？别着急，请细细往下看~
- 关系的使用
- 能用继承来描述的时候一定要用泛化关系
- 描述接口和类的实现关系的时候用实现来描述
- 两个类之间有结构关系的时候用关联来描述
- 除此以外用依赖关系
- 类图的构成
![在这里插入图片描述](https://img-blog.csdn.net/20181003144121831?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 类图的用途
- 对系统的词汇建模
- 对简单协作建模
- 对逻辑数据库模式建模
### 附加——机房收费系统
学习完类图之后自己感觉有点迷，这是个啥呀？有毛线用？虽然自己把类图的各部分已经做了总结，也大概了解了一点“芝士”，可是要说运用到实际还是不明白，之后通过看师哥师姐的博客，自己掌握了大体思路，下面分享给大家，可能不全，大家就凑合着看吧！
![在这里插入图片描述](https://img-blog.csdn.net/20181005214757921?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上图是我根据各位大牛的博客和自己的理解画出的，但我感觉有点别扭，而且我有一个疑问就是那个操作到底指的是什么？我在一本书看的是在操作的下边还有一个职责，职责是类或其他元素的契约或义务。那这个职责和操作是一回事儿吗？不得解，也不知自己画的类图是否正确，如有这方面的大牛，还望路过的你指出~
大恩不言谢，请受小女子一拜！
### 小结
学习很容易，但是作总结确实有点麻烦哦，不过颗粒归仓也是一种对自己负责的学习态度，坚持！
如果你有什么好的建议，欢迎留言交流哈~
