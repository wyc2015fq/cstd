# UML建模详解（3）—使用Rational Rose 画九种常用架构图 - fanyun的博客 - CSDN博客
2016年08月11日 09:05:03[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：17457
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**UML****是一种建模语言，是系统建模的标准。我们之所以建模是因为大规模的系统设计时相当复杂的，当系统比较复杂时就会涉及到以下这几个问题：**
      (1).开发人员如何与用户进行沟通来了解系统的需求?
      (2).开发人员之间如何沟通以确保各个部门能够无缝地协作？
      (3).UML是我们在构造个特种模型时首选的一种建模工具，去描述系统的需求和设计，在对复杂的工程进行建模时，系统可由单一的图形来描述，但是单一图形不可能包含一个大系统所需的所有信息，更不可能描述系统的整体结构功能，这样一来就几乎不能用单一图形来建模了，而UML能从不同的角度去描述系统,如何处理好UML复杂系统建模？
** 使用RationalRose创建用例(Use Case)如图：**
![](https://img-blog.csdn.net/20160607102001900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      (1). 右击browser框中的Use CaseView包，弹出快捷菜单；
      (2).选择New——Use Case项；
      (3).输入用例的名字；(如出错，可用Rename命令更改)
      (4).如果文档窗口不可见，选择屏幕上方的View——Documentation菜单；
      (5).在browser框中选中所需用例；
      (6).将光标置于文档框中，输入相应文档。
       通过上面的学习我们了解了使用RationalRose创建图的具体操作步骤，其他几种图的创建方法类似，记住相应的单词就可以了。
      (1).用例图：Use CaseDiagram
      (2). 类图：Class Diagram
      (3). 对象图：ObjectDiagram
      (4). 状态图：StateDiagram
      顺序图：Sequence Diagram
      (1).协作图：Collaboration Diagram
      (2).组件图：Component Diagram
      (3).部署图：Deployment Diagram
      (4.)活动图：Activity Diagram
**      如何用RationalRose提供的模型去画一张用例图：**
**1.用例图**
步骤： 
(1).双击browser框中的Use CaseView包中的Main条目，打开主用例图；
(2).单击选中browser框中的执行者，并将其拖到主用例图中；
(3). 重复第2)步，直到完成所需的工作为止；
(4).单击选中browser框中的用例，并将其拖到主用例图中；
(5).重复第4)步，直到完成所需的工作为止；
(6).在工具条中选择单向关联(UnidirectionalAssociation)图标；
(7).单击一个执行者，并拖到相应的用例上；或单击一个用例，并拖到相应的执行者上。
例图：
![](https://img-blog.csdn.net/20160608143250353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160607103619548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160607103839829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160607104050803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.类图：**
** 类图中主要说一下如何创建类的创建属性和方法**
 类图的工具类中的图标：
![](https://img-blog.csdn.net/20160613140156254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**(1).创建一个最基本的类（含有类的名称即可）(2).右击刚刚创建好的类选择 NewAttribute即可创建类的属性(3).右击刚刚创建好的类选择 New Operation即可创建类的方法**
如图：
![](https://img-blog.csdn.net/20160607104339925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
例图：
![](https://img-blog.csdn.net/20160607104550826?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**3.对象图：**
与创建类图相似，其中的一个区别是在对象名的下面要有下划线，且对象名的表示方法由三种
 (1).一般方法
![](https://img-blog.csdn.net/20160607104644411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(2).只有类名，冒号和下划线
**![](https://img-blog.csdn.net/20160607105021474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
说明建立的模型适用于该类的所有实例，这种表示方式称作匿名对象
(3).只有对象名和下划线
![](https://img-blog.csdn.net/20160607104914068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
隐藏了属性
**4.****状态图**
(1).创建状态(States)
·        在工具条中选择State图标；
·        在状态图中单击要放置状态的位置；
·        输入状态的名字。(如出错，可用Rename命令更改)
(2).创建状态转换(StateTransitions)
·        在工具条中选择StateTransitions图标；
·        单击起始状态，并拖至下一个状态；
·        输入状态转换的名字。(如出错，可用Rename命令更改)
(3).创建起始状态
·        在工具条中选择Start图标；
·        在状态图中单击要放置起始状态的位置；
·        用状态转换线进行连接。
 (4).创建结束状态
·        在工具条中选择Stop图标；
·        在状态图中单击要放置结束状态的位置；
用状态转换线进行连接。
例图：
![](https://img-blog.csdn.net/20160607105458774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**5.****顺序图**
创建对象和信息
·        双击顺序图名称，打开顺序图；
·        将browser框UseCaseView包中的执行者拖入图中；
·        选择工具条中的“Object”图标；
·        单击图中放置对象的位置，并输入相应的名字；
·        重复3——4步；
·        选择工具条中的“ObjectMessage”图标；
·        从信息发出者拖至信息接收者；
·        输入信息的名字；
·        重复6——8步。
例图：
![](https://img-blog.csdn.net/20160608134244954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**6.****协作图**
采取将顺序图转换为合作图的方法
双击顺序图名称，打开顺序图；
·        选择屏幕上方的菜单Browser——CreateCollaboration Diagram
·        调整图中的对象和信息，使其美观。
![](https://img-blog.csdn.net/20160607105705763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
例图：
![](https://img-blog.csdn.net/20160607105759296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**7.组件图**
创建Main构件图(ComponentDiagram)
·        双击Main构件图中的University包，打开图形；
·        在工具条中选择Component图标；
·        单击图中某一位置，放置构件；
·        输入构件名称。
** 8.部署图**
双击Browser框中的部署图(DeploymentDiagram)；
.   选择工具条中的Processor图标，并单击图中某一位置；
·   输入结点的名字；
·   选择工具条中的Connection图标；
·   点击某一结点，拖至另一结点；
·   选择工具条中的Text图标；
·   在相应结点下写上文字。
下图是书中的一个例图：
![](https://img-blog.csdn.net/20160607105940235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**9.活动图**
- 创建活动(Activity)
- 创建决策点(Decision Points)
- 创建同步条(Synchronization Bar)
- 创建起始活动和终止活动
例图：网购下单的活动图：
![](https://img-blog.csdn.net/20160608142959481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
