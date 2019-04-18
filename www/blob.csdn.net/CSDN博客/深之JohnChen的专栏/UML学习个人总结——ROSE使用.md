# UML学习个人总结——ROSE使用 - 深之JohnChen的专栏 - CSDN博客

2009年08月16日 23:30:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：8254


**Rational Rose****使用**

一、几种UML工具汇总

目前市场上UML工具比较多，我们将列出比较有影响力的UML工具。

◆Rational Rose: 如果不提及由Rational软件公司开发的Rational Rose建模工具，那就无需考虑UML工具的完整性。Rational Rose（Rose代表“Rational Object-oriented Software Engineering”）对UML来说，是一款可视化的建模工具。它有不同的版本来满足不同的需求。

Rational Rose提供上面我们谈到所有的特征。除此之外，Rational Rose也可以支持在同样的环境下进行数据模型的设计。Rational Rose更有趣的特征就是能够将UML中的图作为网页和图片发布。这就使得你能够在不安装Rational Rose的情况下分享你的应用设计。

◆Together Control Center:由美国的Borland 公司开发的Together Control Center（源于Togethersoft）是一款可视化的UML建模工具。Together Control Center支持UML图、MVC建模、正向工程技术和自动更新工程技术，以及双向工程技术，并且可以集成到比如IBM WebSphere Studio的集成开发环境。它不但支持文档编制，并且可以支持协作建模环境。Together Control Center的另一个特征是pattern repository。pattern repository使得经常使用的图和设计形式能够在建模中重新使用。它还支持Rational软件统一开发过程和极限编程方法等。

◆Poseidon:源于Gentleware的Poseidon在ArgoUML开源软件中有其坚固的根基。作为开源的ArgoUML建模工具是一款实用的工具，包含全部UML特征的并且可以免费获得。Gentleware已经采取措施使得ArgoUML成为一款很好的建模工具。使用Poseidon不同的格调来满足不同的需求。

Poseidon通过使用单一用途的插件来支持正向技术和自动更新技术以及文档编制。Gentleware并没有忘记它的开源的特性，因此，为个人软件开发者免费提供UML Community Edition 1.5的Poseidon。

二、Rational Rose工具介绍与使用

Rational Rose 是一种面向对象的统一建模语言软件设计工具，用于可视化建模和公司级水平软件应用的组件构造。就像一个戏剧导演设计一个剧本一样，一个软件设计师使用Rational Rose，以演员（数字）、使用拖放式符号的程序表中的有用的案例元素（椭圆）、目标（矩形）和消息/关系（箭头）设计个种类，来创造（模型）一个应用的框架。当程序表被创建时，Rational Rose记录下这个程序表然后以设计师选择的C++, Visual Basic, Java, Oracle8, CORBA或者数据定义语言（Data Definition Language）来产生代码。Rational Rose的两个受欢迎的特征是它的提供反复式发展和来回旅程工程的能力。Rational Rose允许设计师利用反复发展（有时也叫进化式发展），因为在各个进程中新的应用能够被创建，通过把一个反复的输出变成下一个反复的输入。（这和瀑布式发展形成对比，在瀑布式发展中，在一个用户开始尝试之前整个工程被从头到尾的完成。）然后,当开发者开始理解组件之间是如何相互作用和在设计中进行调整时,Rational Rose能够通过回溯和更新模型的其余部分来保证代码的一致性,从而展现出被称为"来回旅程工程"的能力.Rational Rose是可扩展的,可以使用刻下载附加项和第三方应用软件.它支持COM/DCOM (ActiveX), JavaBeans, 和 Corba组件标准.

Rational Rose界面图：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/1633860629214888750.JPG)

Rose支持的模型图

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/2633860629215670000.JPG)

三、使用案例

网上选课系统主要包括如下功能：管理员通过管理界面进入，建立本学期要开的各种课程、将课程信息保存在数据库里并可以对课程进行改动和删除。学生通过客户机浏览器根据学号和密码进入选课界面，在这里学生可以进行三种操作：查询已选课程、选课以及付费。同样，通过业务层，这些操作结果存入数据库中。

详细的文档从[http://download.csdn.net/source/1580613](http://download.csdn.net/source/1580613)下载。



