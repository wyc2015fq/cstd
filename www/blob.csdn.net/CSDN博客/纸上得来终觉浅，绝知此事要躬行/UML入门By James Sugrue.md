# UML入门By James Sugrue - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月14日 13:44:40[boonya](https://me.csdn.net/boonya)阅读数：431标签：[软件设计																[UML](https://so.csdn.net/so/search/s.do?q=UML&t=blog)](https://so.csdn.net/so/search/s.do?q=软件设计&t=blog)
个人分类：[软件工程](https://blog.csdn.net/boonya/article/category/1731877)








原文地址：[https://dzone.com/refcardz/getting-started-uml](https://dzone.com/refcardz/getting-started-uml)


![](https://img-blog.csdn.net/20170214134349684)


本文采用Google翻译，有些地方数据类型被翻译成了中文，建议中英文对照一起看。


第1节

## 关于UML


统一建模语言是由对象管理组管理和创建的软件系统规范的一组规则和符号。
符号提供了一组图形元素来为系统的各部分建模。

此Refcard概述了UML的关键元素，为您在设计软件时提供有用的桌面参考。

**UML工具**有许多UML工具可用，商业和开源，以帮助您记录您的设计。独立工具，插件和UML编辑器可用于大多数IDE。

### 图类型

UML 2由13种不同类型的图组成，如下面分类中的规范所定义的。





第2节

## 结构图



### 类图

类图描述了系统中类的静态结构，并说明了类之间的属性，操作和关系。

建模类

一个类的表示有三个隔间。

![图1](https://dzone.com/storage/rc-covers/5015-thumb.png)

**图1：**类表示

从上到下包括：
- 包含类名称以及构造型的名称，它提供有关此类的信息。
构造型的示例包括“interface”，“abstract”或“controller”。
- **属性**列出了格式为name：type的类属性，可以使用格式名：type = value提供初始值
- **操作**以格式方法（参数）列出类的方法：返回类型。

操作和属性的可见性注释如下：+ public，＃protected， - private，〜package

![热提示](https://dzone.com/storage/rc-covers/5016-thumb.png)**接口**接口名称和操作是

![数字](https://dzone.com/storage/rc-covers/5017-thumb.png)通常用斜体表示。

**RelationshipDescription依赖**关系“...使用...”一个弱的，通常是临时的关系，说明一个类在某个点使用另一个类。

![类](https://dzone.com/storage/rc-covers/5018-thumb.png)

**图2：** ClassA对ClassB具有依赖性

关联“...有...”强于依赖关系，实线关系表示该类随时间保留对另一个类的引用。

![类](https://dzone.com/storage/rc-covers/5019-thumb.png)

**图3：**与ClassB关联的ClassA

聚合“...拥有...”比关联更具体，这表示类是容器或其他类的集合。
包含的类没有对容器的生命周期依赖，所以当容器被销毁时，内容不是。这使用空心菱​​形描绘。

![类](https://dzone.com/storage/rc-covers/5020-thumb.png)

**图4：**公司包含员工

组合“...是...的一部分”比聚合更具体，这表示类之间的强生命周期依赖，所以当容器被销毁时，内容也是如此。这使用实心菱形描绘。

![类](https://dzone.com/storage/rc-covers/5021-thumb.png)

**图5：** StatusBar是Window的一部分

泛化“...是...”也称为继承，这表示子类型是超类型的更具体类型。
这在关系的一般侧上使用空心三角形来描绘。

![类](https://dzone.com/storage/rc-covers/5022-thumb.png)

**图6：**福特是一种更具体的车型

关联类

有时，类之间存在更复杂的关系，其中第三类包含关联信息。

![图7](https://dzone.com/storage/rc-covers/5023-thumb.png)

**图7：**帐户将银行与个人关联

注释关系

对于所有上述关系，可以表示方向和多重性，以及关系的注释。
方向用箭头表示，其可以是双向的。

以下示例显示了ClassA和ClassB之间的多重关联，并向链接提供了别名。

![图8](https://dzone.com/storage/rc-covers/5024-thumb.png)

**图8：**注释类关系

![热提示](https://dzone.com/storage/rc-covers/5025-thumb.png)注释注释或注释用于所有UML图。它们用于保存图表的有用信息，例如说明或代码示例，并且可以链接到图表中的实体。

### 对象图

对象图提供关于特定时间点的类的实例之间的关系的信息。
正如你所期望的，此图使用类图中的一些元素。

通常，使用不带分隔符的简单矩形以及带有下划线的文本的InstanceName：Class格式对对象实例建模

![图9](https://dzone.com/storage/rc-covers/5026-thumb.png)

**图9：**一个简单的对象图

对象元素还可以具有用于在特定时间对属性的状态建模的额外信息，如在上述示例中的myAccount的情况。

### 组件图

组件图用于说明系统的组件如何以比类图更高的抽象级别连接在一起。
组件可以由一个或多个类来建模。

组件在具有<< component >>分类器和可选组件图标的矩形中建模：

![图10](https://dzone.com/storage/rc-covers/5027-thumb.png)

**图10：**单个组件的UML表示

组装连接器

当一个组件需要使用另一个组件提供的服务时，可以使用组件连接器。

![图11](https://dzone.com/storage/rc-covers/5028-thumb.png)

**图11：** AccountManagement取决于CreditChecker服务

使用球和插座符号，所需或提供的接口如下所示

![图12](https://dzone.com/storage/rc-covers/5029-thumb.png)

**图12：**所需和提供的接口notationPort连接器

端口允许您对暴露于外部世界的功能建模，将特定功能所需的和提供的接口分组在一起。
这在显示嵌套组件时特别有用。

![图13](https://dzone.com/storage/rc-covers/5030-thumb.png)

图**13：**显示端口使用的嵌套组件图

### 复合结构图

复合结构图显示了一个类的内部结构以及可能的协作。

复合结构图中的主要实体是部件，端口，连接器，协作以及分类器。

部分

表示包含实例所拥有的一个或多个实例。
这是使用所有类或组件中的简单矩形来说明的。也可以对部件之间的关系进行建模。

![图14](https://dzone.com/storage/rc-covers/5031-thumb.png)

**图14：**具有正方形和线作为其结构一部分的图类

端口

表示结构的外部可见部分。它们在所属结构的边界处显示为命名的矩形。在组件图中，端口可以指定所需的和提供的服务。

连接器

连接器将实体绑定在一起，允许它们在运行时交互。
实线通常在部件之间绘制。名称和类型信息使用名称：classname格式添加到连接器。还可以在连接器上注释多重性。

![图15](https://dzone.com/storage/rc-covers/5032-thumb.png)

**图15：**两个部件之间的连接器

合作

表示可以一起使用以实现某些特定功能的一组角色。
使用虚线椭圆对协作进行建模。

![图16：](https://dzone.com/storage/rc-covers/5033-thumb.png)

**图16：**多个实体之间的协作

![热提示](https://dzone.com/storage/rc-covers/5034-thumb.png)使用协作建模模式有时协作将是模式的实现。在这种情况下，协作用模式标记，每个部分与其在问题中的角色的描述链接。

![图16](https://dzone.com/storage/rc-covers/5035-thumb.png)

### 部署图

部署图在现实世界设置中建模系统的运行时体系结构。
它们显示如何将软件实体部署到物理硬件节点和设备上。

这些实体之间的关联链接表示节点之间的通信并且可以包括多重性。

**实体描述**![图16：](https://dzone.com/storage/rc-covers/5036-thumb.png)硬件或软件元素显示为3D框形状。节点可以有许多刻板，由右上角的适当图标指示。通过提供带下划线的“名称：节点类型”符号，实例与节点不同。![图17：](https://dzone.com/storage/rc-covers/5037-thumb.png)工件是软件开发的任何产品，包括源代码，二进制文件或文档。它使用右上角的文档图标描述。

![图18](https://dzone.com/storage/rc-covers/5038-thumb.png)

**图17：**部署图示例

### 包装图

软件包图显示了软件包的组织，其中的元素提供了将应用于类的命名空间的可视化。
软件包图通常用于组织和提供类图的高级概述。

除了标准依赖关系，有两种特定类型的关系用于软件包图。
两者都使用具有适当的原型（导入或合并）的标准虚线依赖性来描绘。
- **包导入**用于指示**导入**命名空间将包的成员的名称添加到其自己的命名空间。这表示包可以访问另一个包中的元素。未标记的依赖项被视为导入。
- **包合并**用于指示两个包的内容以类似于泛化关系的方式组合。

![图19](https://dzone.com/storage/rc-covers/5039-thumb.png)

**图18：**包合并示例







第3节

## 行为图



### 用例图

用例图是一种有用的高级通信工具，用于表示系统的要求。
该图显示了用户和其他外部实体与正在开发的系统的交互。

图形元素**实体描述**![图20](https://dzone.com/storage/rc-covers/5040-thumb.png)执行者表示系统中的外部实体，并且可以是人，硬件或其他系统。使用棍子图绘制演员。泛化关系可以用于表示更具体类型的actors，如在示例中。![图21](https://dzone.com/storage/rc-covers/5041-thumb.png)用例表示可以与外部角色交互或与其他用例相关的功能单元。用例用带有用例名称的椭圆表示。BoundaryUse用例包含在系统边界内，使用简单的矩形描述。
外部实体不能放置在系统边界内。图形元素**符号描述**包括![图23](https://dzone.com/storage/rc-covers/5042-thumb.png)说明基本用例可以包括另一个用例，这意味着所包含的用例行为被插入到基本用例的行为中.Extend![图24](https://dzone.com/storage/rc-covers/5043-thumb.png)说明特定用例在一些备选流程中向基本用例提供附加功能。这可以被读取意味着它不需要完成基本用例的目标。一般化当有一个常见的用例提供可以由更专门的用例使用的基本功能时使用。

![热提示](https://dzone.com/storage/rc-covers/5044-thumb.png)**多重性**与正常关系一样，所有用例关系都可以包括多重性注释。

![图23](https://dzone.com/storage/rc-covers/5045-thumb.png)

**图19：**一个简单的用例图

记录用例

在每个用例后面应该有一些文本描述它。
以下是用例定义中的典型部分：

**SectionDescriptionName和Description**用例应该有动词名称，并有简短的描述。**要求**这可以是到外部正式规范的链接，或者这个用例将满足的要求的内部列表。**约束**适用于此用例**执行**的前置和后置条件。**场景**在执行用例期间发生的事件流。通常，这从一个正路径开始，引用多个替代流。

### 活动图

活动图捕获程序的流程，包括主要操作和决策点。
这些图表对记录业务流程很有用。

![图20](https://dzone.com/storage/rc-covers/5046-thumb.png)

**图20：**活动图

图形元素部分**描述**操作![图1.1](https://dzone.com/storage/rc-covers/5047-thumb.png)表示程序流中的一个步骤，使用圆角矩形说明![图1.2](https://dzone.com/storage/rc-covers/5048-thumb.png)操作约束链接到具有格式为“stereotype >> {constraint}起始节点”的文本的注释中的操作![图1.3](https://dzone.com/storage/rc-covers/5049-thumb.png)起始节点用于表示流开始的位置。这使用单个back spot.Activity最终节点来说明![图1.4](https://dzone.com/storage/rc-covers/5050-thumb.png)表示activity.Flow最终节点内所有控制流的结束![图1.5](https://dzone.com/storage/rc-covers/5051-thumb.png)表示单个流的结束。控制流![图1.6](https://dzone.com/storage/rc-covers/5052-thumb.png)表示从一个操作到下一个操作的控制流，如带有arrowhead.Object Flow的实线![图1.7](https://dzone.com/storage/rc-covers/5053-thumb.png)如果在活动之间传递对象，则可以在活动之间添加对象的表示。还可以通过在控制流的任一侧添加表示对象的正方形来表示对象流![图1.8](https://dzone.com/storage/rc-covers/5054-thumb.png)带注释的菱形用于表示控制流中的决策。这也可以用于合并流。决策节点将具有记录的条件，在获取该路径之前需要满足该条件![图1.9](https://dzone.com/storage/rc-covers/5055-thumb.png)使用水平或垂直条表示，叉节点说明并发线程的开始。相同的符号可以用于并发线程的加入。分区![图1.10](https://dzone.com/storage/rc-covers/5056-thumb.png)可以在活动图中使用泳道来说明由不同actor执行的活动![图1.11](https://dzone.com/storage/rc-covers/5057-thumb.png)区域用于将某些活动组合在一起。将原型应用于该区域以识别其是迭代的还是并行的。使用虚线圆角矩形来说明区域。

### 状态机图

状态机图用于描述响应事件的单个对象的生命周期的状态转换。
状态机图以与活动图类似的方式建模。

**实体描述**状态![图2.1](https://dzone.com/storage/rc-covers/5058-thumb.png)状态模型分析器的行为的时间模型。它使用圆角矩形![图2.2](https://dzone.com/storage/rc-covers/5059-thumb.png)表示此状态机的开始执行。插图使用填充圆![图2.3](https://dzone.com/storage/rc-covers/5060-thumb.png)在可能在比初始状态晚的阶段进入状态机的情况下，可以使用这种情况。说明使用空圆。最终状态![图2.4](https://dzone.com/storage/rc-covers/5061-thumb.png)表示状态机执行的结束。使用包含黑点的圆表示。退出点![图2.5](https://dzone.com/storage/rc-covers/5062-thumb.png)表示状态机的最终状态的替代端点。说明使用与X.Transition的一个圆![图2.6](https://dzone.com/storage/rc-covers/5063-thumb.png)表示为带有箭头的线。转换说明了状态之间的移动。它们可以用Trigger [Guard] / Effect注释。状态也可以有自转移，对迭代行为有用![图2.7](https://dzone.com/storage/rc-covers/5064-thumb.png)状态也可以用任何数量的触发/效果对注释，这在状态具有多个转换时是有用的。嵌套状态![图2.8](https://dzone.com/storage/rc-covers/5065-thumb.png)状态本身可以包含内部状态机图![图2.9](https://dzone.com/storage/rc-covers/5066-thumb.png)使用菱形来说明决策，其中多个过渡离开选择元素。状态连接![图2.10](https://dzone.com/storage/rc-covers/5067-thumb.png)连接用于合并来自不同状态的多个转换。使用填充圆圈示出结点。终止状态![图2.11](https://dzone.com/storage/rc-covers/5068-thumb.png)表示状态机的流程已结束，使用XHistory状态进行说明![图2.12](https://dzone.com/storage/rc-covers/5069-thumb.png)历史状态可用于对状态存储器建模，其中状态从上次的状态恢复。这是使用带有H inside.Concurrent Region的圆圈绘制的![图2.13](https://dzone.com/storage/rc-covers/5070-thumb.png)状态可以具有并发执行的多个子状态，其使用虚线建模以分离平行轨道。叉和合并（参见活动图）用于分割/合并转换。

![热提示](https://dzone.com/storage/rc-covers/5071-thumb.png)**转换：触发器，保护，效果**触发器引起转换，这通常是条件的变化。保护是在转换可以执行之前必须求值为真的条件。
效果是将在该对象上调用的操作。







第4节

## 互动图



交互图是处理建模系统上的控制流的行为图的子集。

### 序列图

序列图描述实体如何交互，包括用于交互的消息。
所有消息按执行顺序描述。

除了类和用例外，序列图是用于建模软件系统的最常用的图。

生命线对象

序列图由多条生命线组成。每个实体都有自己的列。元素在列的顶部建模，生命线用虚线继续。以下是生命线对象的选项，最后三个是最具体的。

**部分描述**演员![图3.1](https://dzone.com/storage/rc-covers/5072-thumb.png)参与者代表系统中的外部实体。它们可以是人，硬件或其他系统。演员使用棍子图画。普通救生绳![图3.2](https://dzone.com/storage/rc-covers/5073-thumb.png)表示序列图中的单个实体，显示为矩形。它可以有一个名称，构造型或可以是一个实例（使用实例：类）边界![图3.3](https://dzone.com/storage/rc-covers/5074-thumb.png)边界元素通常在系统的边缘，例如用户界面或处理外部系统的后端逻辑。控制![图3.4](https://dzone.com/storage/rc-covers/5075-thumb.png)控制器元素管理场景的信息流。行为和业务规则通常由这些对象管理。实体![图3.5](https://dzone.com/storage/rc-covers/5076-thumb.png)实体通常是负责保存数据或信息的元素。它们可以被认为是bean或模型对象。

![热提示](https://dzone.com/storage/rc-covers/5077-thumb.png)**Swimlanes** Swimlanes可用于将序列图分解成逻辑层。泳道可以包含任何数量的生命线。

消息

序列图的核心是在建模的对象之间传递的消息。
消息通常是messagename（参数）的形式。

沿生命线的一个薄矩形说明了对象消息的执行生命周期。

可以在两个方向上发送消息，并且可以在到接收者的途中跳过越过其他生命线。

**实体**描述同步![图4.1](https://dzone.com/storage/rc-covers/5078-thumb.png)在末端有一个实心箭头的消息。如果消息是返回消息，它将显示为虚线而不是solid.Asynchronous![图4.2](https://dzone.com/storage/rc-covers/5079-thumb.png)结束时带有线箭头的消息。如果消息是返回消息，它将显示为虚线而不是solid.Lost![图4.3](https://dzone.com/storage/rc-covers/5080-thumb.png)丢失的消息是被发送到非预期的接收器，或发送到未在图中建模的对象的消息。此消息的目的地是一个黑点![图4.4](https://dzone.com/storage/rc-covers/5081-thumb.png)找到的消息是从未知发件人或未在图中建模的对象到达的消息。未知部分被建模为一个黑点.Self MessageA自身消息通常是一个递归调用，或者调用属于同一对象的另一个方法。

![](https://dzone.com/storage/rc-covers/5082-thumb.png)**管理对象生命周期**对象不需要都显示在序列图的顶部。当发送消息以创建对象时，元素的生命线可以在消息结束时开始。
要终止生命线，只需在虚线的末端使用X.

片段

片段是在给定特定条件下执行的逻辑部分。
这些片段可以是许多不同类型。

**EntityDescriptionalt**模型if then else块**opt**模型切换语句**break**用于替代事件序列**par**并发块**seg**在继续**严格**之前以任何顺序处理的消息集在继续**neg**之前以严格顺序处理消息临时**关键的**无效消息集section**ignore**没有兴趣的消息**考虑**相反的忽略。**assert**如果断言是无效的**循环，**则不会显示循环片段

![图21](https://dzone.com/storage/rc-covers/5083-thumb.png)

**图21：**序列图片段

### 通信图

也称为协作图，通信图类似于序列图，除了它们是以自由形式而不是生命线来定义的。
该图的焦点是边界，控制和实体类型之间的对象关系。

参与者之间的消息被编号以提供排序信息。

![图22](https://dzone.com/storage/rc-covers/5084-thumb.png)

**图22：**简单的通信图

### 交互概述图

交互概述图是一种形式的活动图，其中每个节点是到另一种类型的交互图的链接。
这提供了一种有用的方法来提供系统中关键图的高级概述或索引。

![图23](https://dzone.com/storage/rc-covers/5085-thumb.png)

**图23：**交互概述图










