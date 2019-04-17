# 02.1 知识图谱工具 Protege的下载安装与使用 - 博客堂 - CSDN博客





2019年03月01日 11:27:19[最小森林](https://me.csdn.net/u012052268)阅读数：188








![在这里插入图片描述](https://img-blog.csdnimg.cn/20190226172013859.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

# Protege的下载安装与使用

> 
本文是王昊奋老师《知识图谱》系列笔记第2.1篇。





### 文章目录
- [Protege的下载安装与使用](#Protege_4)
- [0 相关介绍](#0__11)
- [0.0 Protege相关介绍](#00_Protege_20)
- [0.1 Protégé的特点](#01_Protg_29)
- [0.2 Protégé的用途](#02_Protg_37)
- [1 Protege的下载](#1_Protege_44)
- [1.1 准备工作：安装配置JAVA环境](#11_JAVA_46)
- [1.2 官网下载](#12__52)
- [2 Protege的安装](#2_Protege_66)
- [2.1 直接解压缩](#21__68)
- [2.2 双击运行](#22__74)
- [3 Protege的使用实例-创建类和实体](#3_Protege_81)
- [3.1 新建本体（一个体系）](#31__83)
- [3.2 创建类（Class）](#32_Class_105)
- [3.3 构建类之间的关系](#33__136)
- [3.4 （重点）创建“关系”properties](#34_properties_150)
- [3.5 （重点）设置实体的属性](#35__168)
- [3.6 水到渠成，添加实体](#36__178)
- [4 Protege本体保存、可视化、推理](#4_Protege_197)
- [4.1 Protege本体保存](#41_Protege_198)
- [4.2 Protege图谱可视化](#42_Protege_207)
- [4.3 Protege推理](#43_Protege_218)
- [4.4 怎么保存Protege 5.x中HermiT推理机reasoner的推理结果？](#44_Protege_5xHermiTreasoner_229)




## 0 相关介绍

通过前面两篇文章的介绍，我们对知识图谱，其相关概念，以及语义网技术栈中的RDF，RDFS/OWL有了一定的了解。

然而，之前我们都是在介绍一些概念性的东西。实践才出真知，理论掌握得再好，不能解决实际问题也只是纸上谈兵。

因此，理论结合实践，让读者能够从无到有构建一个领域知识图谱，并在其上搭建一个基于知识图谱的问答小程序。demo比较简单，问答实现是基于模板匹配和正则表达式，整个流程是为了让读者对知识图谱及其相关应用有个直观的认识。

本文作为实践篇第一篇文章，首先介绍我们使用的数据、数据来源和数据获取方法；其次，基于数据内部关系，介绍如何以自顶向下的方式构建本体结构。

### 0.0 Protege相关介绍

Protégé软件是斯坦福大学医学院生物信息研究中心基于Java语言开发的本体编辑和知识获取软件，或者说是本体开发工具，也是基于知识的编辑器，属于开放源代码软件。

这个软件主要用于语义网中本体的构建，是语义网中本体构建的核心开发工具，目前最新版本是5.5.0（截止到2019-02-26）。

Protégé提供了本体概念类，关系，属性和实例的构建，并且屏蔽了具体的本体描述语言，用户只需在概念层次上进行领域本体模型的构建。

### 0.1 Protégé的特点
- Protégé是一组自由开源的工具软件，用于构建域模型与基于知识的本体化应用程序。
- Protégé提供了大量的知识模型架构与动作，用于创建、可视化、操纵各种表现形式的本体。
- 可以通过用户定制实现域-友好(领域相关)的支持，用于创建知识模型并填充数据。
- Protégé可以通过两种方式进行扩展：插件和基于java的API。
- 相比与其他的本体构建工具而言，Protégé最大的好处在于支持中文，在插件上，用Graphviz可实现中文关系的显示。

### 0.2 Protégé的用途

protégé可用于：
- 类模拟（Class modeling)：protégé提供了一个图形化用户界面来模拟类（领域概念）和它们的属性及关系。
- 实例编辑（Instance editing）：从这些类中，protégé自动产生交互式的形式，全用户或领域专家进入的有效实例成为可能。
- 模型处理（Model processing)：protégé有一个插件库，可以定义语义、解答询问以及定义逻辑行为。
- 模型交换（Model exchange)：最终的模型（类和实例）能以各种各样的格式被装载和保存，包括XML、UML和资源描述框架RDF.

## 1 Protege的下载

### 1.1 准备工作：安装配置JAVA环境

由于Protege基于JAVA，所以要安装配置JAVA环境，至少需要一个JRE。 安装配置过程可以参考我的另外一篇文章：

[《java安装:jdk1.7的安装》](https://blog.csdn.net/u012052268/article/details/44680389)：[https://blog.csdn.net/u012052268/article/details/44680389](https://blog.csdn.net/u012052268/article/details/44680389)

### 1.2 官网下载

protege属于开放源代码软件，开源软件不需要破解，直接去官网上下载稳定本即可：[Protege桌面版官网](https://protege.stanford.edu/products.php#desktop-protege)：
[https://protege.stanford.edu/products.php#desktop-protege](https://protege.stanford.edu/products.php#desktop-protege)

打开后界面如下，点击右下角下载按钮；

弹出注册页面 不用理会。

等待下载好。注意，protege软件下载需要外网，且下载很慢，博主下载了一个小时才下载好，把他上传到了CSDN 有需要的自行下载：[《Protege-5.5.0-beta-9 截止到2019-02-27的最新版本》](https://download.csdn.net/download/u012052268/10978113)[https://download.csdn.net/download/u012052268/10978113](https://download.csdn.net/download/u012052268/10978113)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190226195621405.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

## 2 Protege的安装

### 2.1 直接解压缩

哈哈 不知道什么时候，Protege变成了绿色免安装版本，下载好了直接解压即可（网络上其他博客 还要点击NEXT安装，有点过时了）：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227102146974.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

### 2.2 双击运行

直接双击上图中的 **Protege.exe** 运行程序。打开之后界面如下图所示：这是一个新建本体的页面。

![image](https://img-blog.csdn.net/20170811155143041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3g5NDMwMjQyNTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 3 Protege的使用实例-创建类和实体

### 3.1 新建本体（一个体系）

2.2节所示的图片就是新建本体的页面。

也可以选择file -> new 来新建，点击之后，与上图一致。

（或者open一个别人建立的本体。）

打开protege，看到和下图类似的界面。在Ontology IRI中填写我们新建本体资源的IRI。读者**一定**要先填写自己的符合标准的IRI。 这个后面建实体关系都会用到。

![image](https://pic2.zhimg.com/80/v2-4492b603368188c449800a83431a1f29_hd.jpg)

和下面：
![](http://ww1.sinaimg.cn/large/006qDjsOgy1g1hbi3baolj30wx0lb0wx.jpg)
比如我们设置的hr知识图谱：

```
@prefix: <http://www.hr_kg_qa.com#> .
```

### 3.2 创建类（Class）
- 第一步：找到“Classes"选项

我们创建类（Class）需要用到“Classes"选项卡，

protege工具展现的面板还是比较多的，首先找到Classes面板吧，就像这样：
![image](http://images.cnitblog.com/blog/595533/201402/201744338104933.png)
如果没有找到的话，在菜单中“Window->Tags->Classes”。
- **第二步：创建类与子类**

在classes页面，邮右键点击owl:Thing ，选择add subclasses；

在出现的对话框中 的name 输入类的名字，点击确定。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227104028146.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

此外，还可以右键点击一个类（人物），创建它的子类（禅师），如下图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227105038742.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

同理可以创建出男人、女人、寺庙、学校等类别。

点击左侧的Thing，这个是所有类的最高级的父类，我们建立的所有类都是它的子孙，

这时候Thing上面的三个小图标中的第一个就会亮起，它是创建子类的快捷按钮，第二个是创建兄弟类，第三个是删除选中的类。

单击，输入类名就可以建立一个新的类了。这样我们新建一堆的类

### 3.3 构建类之间的关系

点击classes页面中的某一个类，在右面的describtion可以观察和修改 这个类的属性，比如它的父类是啥，他的互斥是啥。

我们可以利用这个功能设置类之间的关系。

比如：人物和地点 是完全不同的事物，他们之间具有排他性（owl:DishointWith），我们可以这样设置：
- 选中“人物”类，在右侧Describtion部分点击DishointWith 的加号。见下图左边
- 在弹出的窗口展开所以类（Thing）选则“地点”，确定；这样任务和地点就有互斥关系了。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227105804759.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

### 3.4 （重点）创建“关系”properties

“关系”properties 是三元组中间的一环，没有“关系”，试题之间无法连接起来。所以目前最重要的是要创建“关系”properties 连接起不同的实体。

具体方法如下图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/201902271130112.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

有了“关系” 还要进一步设定 “关系”的**约束**。  比如对于关系“曾住”来说，  他的主体（domain）只能是一个人名！  而他的客体（对象）只能是一个地点！ 具体设置方式如下图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227113222566.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

同理可以设置“父亲”、“前身”“开山之祖”、“相识”等关系。

其中设置“相识”的时候，可以点击symmetric（对称性）。 即A相识与B  那么B相识与A。  这一点特别重要，在推理的时候，免去我们人工很多设置。

### 3.5 （重点）设置实体的属性

实体不仅仅 有相互之间的关系相连接，还有属于自己的一些属性，  比如：对于禅师来说，会有法号，法号必须是string类型。

我们切换到"Data properties"，我们在该界面创建类的属性，即，数据属性。其定义方法和对象属性类似，除了没有这么丰富的描述属性特性的词汇。其实不难理解，这些描述特性的词汇是传递、对称、反对称、自反等，表明其必定有指向其他资源或自身的边，而我们之前提到过，数据属性相当于树的叶子节点，只有入度，而没有出度。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227114220933.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

同理，可以设置“别名”、“籍贯”、“生日”等实体属性

### 3.6 水到渠成，添加实体

前面分别定义了“类别”、“关系”、“实体的属性”以及他们的约束条件。下面只需要按照之前的约束填写一个个的实体即可。
- 第一步创建实体

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227115124199.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)
- 第二一步创建实体之间的关系

同时在创建实体的过程中就可以顺便填写上试题之间的关系，即三元组。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227115251410.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

（此处注意，关系名 和实体名都要手动填写，没有系统提示；如果有错别字，无法填写成功）- 第三步：填写实体的属性。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227130003137.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

## 4 Protege本体保存、可视化、推理

### 4.1 Protege本体保存
- 在菜单中选择File -> sava
- 弹出的页面格式选择：“RDF/XML Syntax” 并点击确定
- 在新出现的页面中输入保存本体的名字比如：“test苏轼”，保存文件类型选择“OWL File”并点击保存

同时，这个保存后的文件可以共别人打开，发开方式为：File -> Open

### 4.2 Protege图谱可视化

protege也支持以可视化的方式来展示本体结构。我们点击"Window"选项，在"Tabs"中选择"OntoGraf"，然后"Entities"旁边就多了一个标签页。在右侧窗口中移动元素，可以很直观地观察本体之间的关系。

在菜单中选择 Windows -> Tabs -> OntoGraf

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227131141413.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

注意，默认的一些实体是隐藏的，在图中单击加号 即可显示出来。

### 4.3 Protege推理

Protege是具有强大推理插件的（hermiT），比如苏轼相识B，那么根据对称性，B应该自动相识苏轼。 这样推理就可以展现出强大人脉网络。

具体操作如下图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227131532211.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

点击之后，所有的推理规则会存储到本体中，比如下图，点击裘文德，会出现他是男人 这样一条新的三元组。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227131737932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIwNTIyNjg=,size_16,color_FFFFFF,t_70)

### 4.4 怎么保存Protege 5.x中HermiT推理机reasoner的推理结果？

我们在使用Protégé5.5，我想将推理器的结果保存到本体文件中。当我停止推理时，所有结果都消失了。如何保存结果？

[怎么保存Protege 5.x中HermiT推理机reasoner的推理结果？](https://blog.csdn.net/u012052268/article/details/88736783)[https://blog.csdn.net/u012052268/article/details/88736783](https://blog.csdn.net/u012052268/article/details/88736783)






