# UML是什么？ - Jun5203 - CSDN博客
2018年10月23日 11:38:48[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：188
统一建模语言（Unified Modeling Language——UML）是一种面向对象的建模语言，它可以实现大型复杂系统各种成分描述的可视化、说明并构造系统模型，以及建立各种所需的文档，是一种定义良好、易于表达、功能强大且普遍适用的建模语言。
**UML基本内容**
- 一
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028165740694.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_27,color_FFFFFF,t_70)
- 二
- ![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104163927246.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
**UML基本内容详述**
**（1）视图**
视图是表达系统的某一方面特征的UML建模元素的子集；试图并不是图，它是由一个或多个图组成的对系统某个角度的抽象。
1）用例视图（核心视图）
强调从用户的角度看到的或需要的系统功能。
2）逻辑视图
该视图用于描述系统内实现的逻辑功能，展现系统的静态或结构组成及特征。
3）组件视图
该视图从系统实现的角度来描述模型对象间的关系。
4）配置视图
该视图用于说明系统的物理配置。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028170237708.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_27,color_FFFFFF,t_70)
**（2）图表**
图表是描述视图内容的图。
1）用例图
用于描述外部项与系统提供的使用事件之间的联系。一个使用事件是系统提供的功能的具体描述，是系统分析人员从用户角度描述系统的功能，是功能与功能之间以及功能与用户之间的关系。使用事件定义了系统的功能需求。
简单理解：用来描述系统的功能。
2）类图
用于描述系统的静态结构。类可以用不同方式连接，主要包括联合、依赖、独立和包装。一个系统一般有多张类图，一个类可在不同的视图中出现。
3）对象图
用于表述系统在某个时刻的静态结构。对象图也可作为协作图的一部分，说明一组对象之间的动态协作关系。
对象图与类图的区别：对象图表示的是类中的许多对象实例，而不是类本身。
4）状态图
用于说明类中的对象可能具有的状态，以及由时间引起的状态的改变。
简单理解：描述了系统元素的状态条件和响应。
5）顺序图（时序图）
用于描述对象间的动态协作关系。表达了对象间发行消息的时序，同时也表达出对象间的相互作用，以及当系统执行到某个特定位置时可能会发生的事。
简单理解：按时间顺序描述系统元素间的交互。
6）协作图
按照时间和空间顺序描述系统元素间的交互和它们之间的关系。
7）活动图
用于描述系统活动的流程。活动图由活动状态组成，它包含将完成的活动的说明。当一个动作完成时，激发一个明确的事件并转到一个新的状态。它可以描述并行执行的活动。另外，它还包括了当动作部分完成时收到或发出的消息的说明。
简单理解：本质上是流程图，描述系统的执行顺序。
8）组件图
用于描述组件代码的物理结构。它建立了一个从逻辑视图到物理视图的映射。同时，它还描述了组件的依赖关系，可以用来分析一个组件的变化对另一个组件所产生的影响。
9）配置图
用于描述系统中软件和硬件的物理结构。
我从不同的角度对UML的各种图进行了分类，下面分享给大家
- 一
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028170550389.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_27,color_FFFFFF,t_70)
- 二
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104163431629.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 三
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104163457698.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
**（3）模型元素**
模型元素代表面向对象中的类、对象、接口、消息和关系等概念。UML中的模型元素包括事物和事物之间的联系，常见的联系包括关联关系、依赖关系、泛化关系、实现关系和聚合关系。
**（4）基本机制**
UML的基本机制表现为各种图标上的附加信息，用于描述那些模型元素无法表达的内容。
1）修饰
通过特定的修饰把一些语义加到模型元素上。
2）注释
UML提供增加注释的方式以表达那些模型元素无法表示的信息。
3）说明
用于增加无法正式在图中表示的元素实例的附加说明，可以由文本的形式对图中相应部分的责任和权限加以说明。
**使用Rational Rose建模**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028171228385.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_27,color_FFFFFF,t_70)
UML的作用就是从静态和动态方面用模型图来从各个角度全面描述要开发的系统，为人们提供一套功能强大的捕获信息技术要点的工具。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104163958859.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
