
# Eclipse、NetBeans、IntelliJ - 阳光岛主 - CSDN博客

2013年07月23日 22:19:41[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：6897



Eclipse作为开发Java的IDE，从出现到现在已经有几年的时间了，其丰富的特性，操作容易和在提高生产力方面收到了广大开发者的青睐。当然一提到Java IDE，人们不禁会提到其他Java IDE像NetBeans、IntelliJ等，那么这些Java IDE之间到底存在着哪些异同呢？
因此在Java开发的空间里，对这些主要的IDEs做一个全面的比较是很有必要的，通过比较我们找出这些主要的IDEs在四个通常的开发领域：Swing、JSP/Struts、JavaServer Faces、J2EE/EJB3.0中哪个IDE表现的更出色。本文就从这四个通常的开发领域来比较Eclipse、NetBeans、IntelliJ。
三个IDEs中，Eclipse是唯一个存在多个版本，从基本的版本到预先打包好的带额外的开源插件（像EasyEclipse）和开源/商业结合的（像MyEclipse）。

**NetBeans 5.5**
首先，来看NetBeans 5.5，NetBeans 5.5的基本信息如下表：
Vendor：Sun Microsystems
Website：[https://netbeans.org/](https://netbeans.org/)
Price：Free/Open source
Distribution：Base + EnterprisePack + Visual Web Pack

**安装：**
NetBeans 5.5，包括它所带的包（像带有UML/BPEL/SOA的EnterprisePack和开发JSF的 Visual Web Pack ）都既可以作为ZIP文件的形式来下载也可以跨平台进行安装。
在Windows平台下，安装以后会与操作系统无缝的集成起来，包括桌面的快捷方式和增加安装/卸载控制面板。
在Linux平台下，NetBeans 5.5仅仅安装在一个指定的目录下并且在GNOME或者KDE桌面上创建了一个启动的图标，但是，在Linux平台下，NetBeans 5.5不能以RPM或者一个.deb文件形式打包在一起，也不能提供一个标准的知识库，这些知识库可以使Linux用户按照其他的应用来安装NetBeans5.5。

**一般特性：**
早期的NetBeans版本在开发Swing上速度非常慢，而且界面很不美观，因此并没有收到众多人士的欢迎，自从5的版本以后，NetBeans开发小组已经彻底的改变了这种状况，NetBeans5.5和JDK 1.6的结合为用户提供了更为出色的用户体验，特别在Windows平台下（Linux仍然有一些UI上的小问题，预计在NetBeans6.0的版本这些问题会得以改善。在5.5的版本中缺少对本地GTK的外观支持。）
在Windows系统中，NetBeans 使用起来非常的方便，像收缩/隐藏/切换等功能，面板/编辑器等配置起来也非常的容易，另外，NetBeans的菜单布局也非常有逻辑，使用起来很方便，大多数的一般功能很容易上手，一直以来，NetBeans的稳定性和性能也十分出色。
基本的Java编辑器也是相当好的，虽然在代码完成方面与其他IDEs相比还逊色一些，但是毕竟还是可以使用的，NetBeans也提供重构的功能（特别是最常用的重命名/移除特性），在一些测试中，它不总是能够进行上下文感知（context-aware），例如在Struts工程中重命名Action类所在的包，相应的在struts-config.xml中就不能及时的做到同步更新，但是在重构JSFbacking beans和更新在faces-config.xml中的实体方面表现的很好，同时在同步更新J2EE 1.4sessions beans和ejb-jar-xml的实体方面表现的也很好。

**Swing开发：**
NetBeans包括一系列的特性来开发Swing应用程序，像根据实际文本的位置来进行自动排列，它的强有力的尺寸恢复功能和固定功能使得UI设计器是任何平台的任何语言都不能比拟的。NetBeans5.5还包括一些新的有助于提高生产力的特性，像自动进行国际化（控制每个窗体/对话框/面板到ResourceBundle包括所有的国际化字符串），使用自定义的窗体/面板（只要他们在你的工程中至少编译过一次）。
对Swing开发者来说，使用NetBeans可以充分的利用NetBeans的强有力的RCP（Rich Client  Platform）向导来创建新的，成熟的Swing应用。

**JSP/Struts开发：**
NetBeans带有非常好的JSP编辑器，该编辑器包括我们期望的基本的特性，基本的向导和创建一个新的Struts工程。内嵌的Tomcat同期使得开发和调试JSP应用程序都很容易，下图为NetBeansweb应用向导：
NetBeans web应用向导自动配置web.xml文件和struts-config.xml文件，增加对Tiles和Validator的支持，另外，NetBeans还在struts-config.xml文件中提供了内容菜单选项，用向导的方式来增加ActionForms,Actions和Forwards。提供了非可视化的编辑器来显示在应用程序中的页面导航功能，提供对Tiles 和Validators配置的完全支持。
NetBeans 5.5对Struts支持的非常好，但并不像发布文档说的那样完美，非可视化的编辑器可以用来编辑JSP或者HTML页面，对JSP编辑器缺乏对嵌入的JavaScript的支持，JSP编辑器也只带有一个基本的CSS编辑器（当然安装上Visual WebPack是更好的）。这些都有待于提高。下如为NetBeans对Struts的支持：

**JSF开发：**
NetBeans对JSF的支持就像对Struts支持的一样，提供了一个基本的向导来创建一个工程，包括类库，配置所有要求的文件等，甚至在JSP编辑器中提供了对backing beans属性的代码补全功能。同时包括在faces-config.xml文件中对导航规则的向导。
NetBeans Visual Web Pack提供了一个交互式的解决方案通过从Sun JavaStudio Creator直接移植到NetBeans的特性来构建JSF应用，这包括用来构建web页面的一个Matisse样式的GUI构造器，并带有丰富的JSF控件，这些JSF控件扩展了基本的JSF RI集，同时也支持数据绑定（直接依靠数据库查询甚至通过JPA/Hibernate）。

尽管Visual Web Pack有一些优点，但是Visual Web Pack也有一些缺点：
1． 缺少对页面模板的支持（虽然可以保存一个页面作为一个模板并且作为一个标准使用，但是模板的变化不是级连的），对Facelets的支持或者来自MyFaces对Tiles的支持在这里将是很有用处的。
2． 缺乏对ApacheMyFaces的支持（存在着潜在的问题，当使用MyFaces RI而不是Sun的规范像JBoss部署到一个应用服务器）
3． 不灵活的backingbeans自动产生功能。
另一个主要的特性是支持Facelets，在许多JSF社区中，Facelets被认为是JSP的替代品，尤其是JBoss Seam已经开放了它的所有文档。
虽然NetBeans一再宣称下一个发布版本的不足，但是对JSF开发来说，Visual Web Pack是最能提高生产力的工具。

**企业开发：**
NetBeans 5.5是第一个支持JPA和EJB 3.0的IDE，在自动产生JPA实体类和注释（包括persistence.xml文件的正确设置，甚至基本的连接信息）可以节省大量的手工编写代码的时间，自动产生的JPA代码的质量非常高并且可以立即使用，第一次使用Matisse开发一个Swing应用和通过JPA查询/更新一个数据库所达到的水平是我使用PowerBuilder多年达到的水平。
对纯粹的企业开发来讲，NetBeans提供了对J2EE 1.4和Java EE 5的支持，特别对J2EE 1.4的项目来说，EJB向导产生了所有要求的代码（包括business/home/remote接口）。
总之，NetBeans考虑了与J2EE 1.4相关的冗长的代码，同时提供了对Java EE 5的封装；NetBeans对部署到GlassFish/Sun Application Server和JBoss上的企业应用提供了官方的支持，而且更新站点为WebLogic和 WebSphere提供了额外的插件。


**[IntelliJ IDEA](http://www.jetbrains.com/idea/)**
首先，IntelliJ IDEA 6.0.4的基本信息如下表
Vendor:JetBrains
Website:[http://www.jetbrains.com/idea/](http://www.jetbrains.com/idea/)
Price:$499.00 ($299.00 upgrade)

**安装：**
在Windows下，IDEA提供了exe形式的安装文件
在Linux下，IDEA的安装是更为简单，包括一个TAR.GZ文件，解压该文件，手动改变到bin子目录下执行./idea.sh。如果不能正常的执行，可能是因为你没有正确的设置$JDK_HOME环境变量，设置$JDK_HOME在.bashrc文件中：
exportJDK_HOME=/home/jacek/Devhttp://java.chinaitlab.com/JDK/jdk1.6.0_01
exportJAVA_HOME=/home/jacek/Devhttp://java.chinaitlab.com/JDK/jdk1.6.0_01
在Linux版本中，JetBrains并没有提供标准的.deb或者RPM文件来进行安装，因此，安装以后没有桌面快捷方式或者创建K Menu实体，更糟糕的是，双击bin/idea.sh文件也不能启动（返回的错误是没有设置JAVA_HOME，虽然已经正确的设置了），所以必须的手动地转到命令行执行./idea.sh以正常启动，这点在三个IDEs中是最差的。

**一般特性：**
与NetBeans一样，IDEA提供了灵活的布局，在各种配置中截短/锁定/浮动 面板的功能，但它没有NetBeans（Windows系统下）那样流畅和可配置。
相对IDEA杰出的编辑器来讲，这些只是一些小问题，IDEA的编辑器是非常快的，具有对敏感内容的颜色凸显，提示和弹出建议框的功能，使用IDEA的时间越长，编程的效率就会越高。下图为一个简单的IDEA编辑器和高级的颜色/语法突出显示：

**JSP/Struts开发：**
IDEA可以自动的建立配置文件（包括Tiles 和Validator），下载所有需要的Struts类库，它也提供了一个Struts助手，提供图形化的编辑器和向导来进行Struts配置，包括tiles-config.xml 和validation.xml文件。
IDEA没有JSP/HTML页面的可视化编辑器，其规则的IDEAJSP/HTML编辑器即使没有一个可视化的组件也是非常好看的，支持嵌入的JavaScript，特别是能够处理DHTML或者AJAX代码，JavaScript编辑器也支持指定的浏览器（IE,Mozilla和 Opera）和流行的AJAX矿见像Dojo, Bindows和 Prototype。

**JSF 开发**
对JSF 开发的支持应该比其他的IDE更好，提供了更成熟的JSF功能，包括对faces-config.xml进行可视化编辑，代码自动装配，在JSP页面管理 bean的属性，新的管理bean的向导，可视化编辑器建立Converters, RenderKits和Validators。Eclipse还计划发布可视化的JSP/JSF编辑器。

**企业开发：**
Eclipse并没有对JPA提供官方的支持，但它提供了一个叫Dali的孵化器项目来连接到发布的功能，目前它处于开发的早期阶段，虽然能够从一个数据库中产生实体，但是它产生的代码没用NetBeans产生的好（例如没有任何自动创建的指定的查询），如果你没有从事JPA开发而正在使用Hibernate，MyEclipse是一个不错的选择。
对J2EE开发，Eclipse支持创建EJB和EAR模块，为了避免J2EE的复杂性，Eclipse专注通过XDoclet来产生EJB，这无疑是一个很好的解决方案，减少了企业应用的复杂性。MyEclipse扩展了这些功能通过提供session beans，消息驱动beans向导和容器管理持久化beans，同时也被XDoclet驱动。
目前，Eclipse和MyEclipse都没有支持Java EE 5，在将来的版本中应该会提供对Java EE 5的支持。

**总结：**
对不同的IDE，每个人有着不同的见解，同时每个IDE也有着不同的特性和优点，根据开发的不同需要可能会选择不同的IDE。
在Java开发的每个方面，每个IDE都能胜任工作，然而根据你所做的不同开发如Swing开发，web开发，或者企业开发，我们会选择不同的IDE。
**Swing开发：**
如果你专门从事Swing开发，NetBeans无疑是最好的选择，如果你没有选择NetBeans而是选择了Eclipse，建议使用MyEclipse 和它的Matisse4Eclipse构造器。IDEA由于对JGoodies Forms的支持，所以在开发Swing方面与前两者相比并不占有优势。
**JSP/Struts开发：**
开发JSP/Struts首选的是IDEA，接下来是MyEclipse，然后是NetBeans，由于缺少对内建Struts的支持，Eclipse的基本版本没有竞争优势。
**JSF开发：**
在开发JSF方面，这三个IDE可以说都是很好的选择，但是IDEA可能会更好一点，接下来是Eclipse/MyEclipse，最后是NetBeans。
**企业开发：**
由于NetBeans对JPA的支持和它产生代码的质量以及对持久化单元合适的设置，所以在企业开发方面，NetBeans是首选，接下来是IDEA，由于Eclipse的Dali项目的限制，Eclipse可能是最后选择的。如果放弃标准的JPA，从事Hibernate开发，MyEclipse是最好的选择。
IDEA由于对J2EE 和 Java EE 5支持的很好，所以很适合进行企业开发，Eclipse缺少对Java EE 5的支持，应该是最后的选择。


**参考推荐：**
[Eclipse、NetBeans、IntelliJ](http://www.360doc.com/content/10/0809/13/2020166_44735761.shtml)
[Eclipse NetBeans IntelliJ的实用对比](http://wenku.baidu.com/view/4eeeaf8ad0d233d4b14e6979.html)
[ECLIPSE、NETBEANS、INTELLIJ对比](http://www.blogjava.net/sutao/articles/136375.html)



