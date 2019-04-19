# UML建模详解（1）—初识UML建模 - fanyun的博客 - CSDN博客
2016年08月10日 08:52:14[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3392
**UML建模简介：**
 UML（Unified Modeling Language的缩写），也称统一建模语言。是用来做软件建模的。不光是在软件的设计阶段用到，在软件的测试，部署阶段都要用到。用于表达软件的操作，对象等信息。
      学习UML主要是学习如何建模。它主要是利用一些图来表达信息。一幅图顶得上千言万语。所以UML的主要部分就是学会画那些图：比如用例图，类图等。
      这里，我们主要通过Rational Rose建模，当然也有其它工具可以做UML建模，比如[Visio、](http://blog.csdn.net/boboai612/article/details/8702875)PowerDesign。
**UML建模技术主要分为结构建模、动态建模和模型管理建模3个方面:**
        第1个方面是从系统的内部结构和静态角度来描述系统的，在静态视图、用例视图、实施视图和配置视图中适用，采用了类图、用例图、组件图和配置图等图形。例如类图用于描述系统中各类的内部结构(类的属性和操作)及相互间的关联、聚合和依赖等关系，包图用于描述系统的分层结构等;
        第2个方面是从系统中对象的动态行为和组成对象间的相互作用、消息传递来描述系统的，在状态机视图、活动视图和交互视图中适用，采用了状态机图、活动图、顺序图和合作图等图形，例如状态机图用于一个系统或对象从产生到结束或从构造到清除所处的一系列不同的状态;
        第3个方面描述如何将模型自身组织到高层单元，在模型管理视图中适用，采用的图形是类图。建模的工作集中在前两方面，而且并非所有图形元素都适用或需要采用。
        在嵌入式软件开发中，[面向对象技术](http://baike.so.com/doc/922652-975191.html)内在支持了对系统的抽象、分层及复用技术，能够很好地控制系统的复杂性，也逐渐广泛应用。实时UML语言是在[嵌入式开发](http://baike.so.com/doc/5567711-5782857.html)中适用的[建模语言](http://baike.so.com/doc/836661-884818.html)。现有许多功能强大UML建模工具，有些工具在引入或加强[嵌入式实时系统](http://baike.so.com/doc/7522455-7796548.html)应用领域的功能，例如Rose
 RealTime和Rhapsody。
UML建模要求：
       用UML建模时，对软件开发过程是有要求的，必须是用例驱动，以架构为中心，[迭代](http://baike.so.com/doc/5077150-5304878.html)和递增的开发，如果软件开发组织的软件开发过程不能满足这三点要求，那么UML的使用效果就会大打折扣，下面详细论述:
一、用例驱动
用例驱动意味着为系统定义的用例是整个开发过程的基础。
用例在多个核心工作流程中都发挥了作用。
（1）     用例的概念可用来表示业务流程，我们称这种用例的变体为"业务用例"。
（2）     用例模型是需求工作流程的输出结果。在这一早期流程中，需要通过用例来建立用户希望系统完成的任务的模型。这样，用例构成了一个重要的基本概念，客户和系统开发人员都必须认可这个概念。
（3）     在分析设计中，用例是在设计模型中实现的。您需要生成用例实现来说明在设计模型中如何通过对象的交互来执行用例。此模型根据设计对象来说明所实施系统的各个组成部分，以及这些部分如何通过相互作用来执行用例。
（4）     在实施阶段，设计模型就是实施的规约。由于用例是设计模型的基础，所以用例需通过设计类来实施。
（5）     在测试期间，用例是确定[测试用例](http://baike.so.com/doc/150233-158747.html)和测试过程的基础。也就是说，通过执行每一个用例来核实系统。
（6）     在项目管理过程中，用例被用来作为计划[迭代式开发](http://baike.so.com/doc/4580869-4792201.html)的基础。
（7）     在部署工作流程中，它们构成用户手册阐述内容的基础。用例也可用来确定产品[构件](http://baike.so.com/doc/2195185-2322688.html)如何排列组合。例如，客户可通过将用例进行某种组合来配置一个系统。
二、以架构为中心
使用UML建模时要以架构为中心，构架之所以重要，原因有以下几点:
1、它使您可对项目进行并保持理智的控制，应付项目中复杂多变的情况，同时保持系统的完整性。
一个复杂的系统不仅仅是其各组成部分之和，也不光是一连串没有关联关系的、很小的技巧决定。它必须依靠某种连贯统一的结构来有条理地组织那些部分，并且提供准确的规则，使系统发展过程中，其复杂程度不会膨胀，超越人类的理解力。
通过建立用于讨论设计问题的一套公共参考材料和一个公共词汇表，构架提供了增进交流和理解的手段。
2、它是大规模复用的有效基础。
通过明确阐述它们之间的主要[构件](http://baike.so.com/doc/2195185-2322688.html)和关键接口,构架为您决定重复使用提供依据，包括内部复用(确定公用的部分)和外部复用(并入现成的构件)。它还允许更大规模上的复用:构架本身的复用，用于处理同一领域中的不同功能。
3、构架还可作为项目管理的基础。
项目计划和人员配备是根据主要构件的类别组织进行的。基本的结构决策是由一个人员组成相对固定的构架小组作出的，他们不是分散的。而开发活动则被分配给若干个小组，每个小组负责开发系统的一个或若干个部分。
三、迭代和递增的开发
使用UML建模时迭代式方法一般要优于线性或瀑布式方法，其原因很多。
1、允许变更需求。需求有时会变化，这常常给项目带来麻烦，它们会导致延期交付、工期延误、客户不满意、开发人员受挫。
2、逐步集成元素。在[迭代](http://baike.so.com/doc/5077150-5304878.html)式方法中，集成可以说是连续不断的。过去在项目结束时要占到整个项目工作量的那段较长的、不确定的且棘手的时期，现分散到六至九个集成部分中，每一部分要集成的元素都比过去少得多。
3、及早降低风险。因为风险一般只有在集成阶段才能发现或得到处理。在初期迭代时，检查所有的核心工作流程，对项目使用的工具、市售软件及人员技能等许多方面进行磨合。过去认定的风险可能被证明不再是风险，而又可能出现一批新的未曾怀疑过的风险。
4、有助于组织学习和提高。团队成员有机会在整个生命周期中边做边学，各显其能。测试员可以早一些开始测试，技术文档编写员可及早开始编写，其他人也是如此。如果是非[迭代式开发](http://baike.so.com/doc/4580869-4792201.html)，这些人在初期只能制定计划或培训技能，空等着开始他们的工作。培训需求等也可在评估复审中尽早提出。
5、提高复用性。因为分部分设计或实施比起预先确定所有共性更容易确定公用部分。确定和开发可重复使用的部分并非易事。早期迭代中的[设计复审](http://baike.so.com/doc/525477-556264.html)可使构架设计师确定毋庸置疑的潜在复用部分，并在以后的迭代中开发和完善这些公用代码。
6、生成性能更强壮的产品。因为在多次[迭代](http://baike.so.com/doc/5077150-5304878.html)中您总是不断地纠正错误。在产品脱离先启阶段后的初期[迭代](http://baike.so.com/doc/5077150-5304878.html)中仍然可以发现缺陷。性能上的瓶颈可以尽早发现并处理，而不象在交付前夕，此时已来不及处理。
7、容许产品进行战术改变。例如同现有的同类产品竞争。可以决定采用抢先竞争对手一步的方法，提前发布一个功能简化的产品，或者采用其他厂商的已有技术。
8、迭代流程自身可在进行过程中得到改进和精炼。一次迭代结束时的评估不仅要从产品和进度的角度来考察项目的情况，而且还要分析组织和流程本身有什么待改进之处，以便在下次迭代中更好地完成任务。
通常在软件开发过程中，[迭代](http://baike.so.com/doc/5077150-5304878.html)在数量、持续时间和目标上都是按计划进行的。参与者的任务和职责都已确定好。对进度进行的目标评测都将记录备查。从一次[迭代](http://baike.so.com/doc/5077150-5304878.html)到下一次迭代确实会存在返工现象，但返工也是严格按规定进行的。
四、使用不当的问题
       很多企业员工在使用UML建模的过程中，只是进行了领域建模，没有进行用例建模，这样是不能最大可能地发挥UML的优势的，因为该组织的软件开发过程不是用例驱动的。
       如果软件开发组织的软件开发过程不能满足上述三点要求，那么UML的使用效果就会大打折扣。也会产生一些问题，有些组织在使用UML之后，发现前期花很长时间设计的模型到了项目的中后期和真正的开发成果相去甚远，以至于全都束之高阁了，如果产生这样的问题，就应该仔细研究一下组织的软件开发过程，是否满足上述三点要求，如果软件开发过程不满足[迭代](http://baike.so.com/doc/5077150-5304878.html)的开发，模型没有随着进度改进，这种问题就很容易出现。
       UML2.0和MDA([模型驱动](http://baike.so.com/doc/1538232-1626178.html)架构)提出了一些解决开发周期前期和后续的模型不一致问题的方法，就是通过模型的转换来完成模型的自动变更，而不是对各个抽象层次的模型全部进行修改，但MDA为大部分人所接受还需要些时日。
五、总结
       综上所述，UML建模虽然是[软件建模](http://baike.so.com/doc/5924635-6137556.html)的有利武器，也要遵循一定的规则来使用，否则就不能很好地发挥它的价值，也会事倍功半。理解UML使用的前提，并认真按照这些方法进行实施，相信会有理想的效果。
**首先启动Rational Rose，启动后如下图：![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/fa89a477db6a41d68e50e84db5f15bca.jpg)1． 启动以后点开左边的Compoent View，鼠标右键点击Compoent View，在弹出的快捷菜单中选择New->Compoent，如下图：![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/d39c3b52a80747478d92a924966fbb30.jpg)新建Compoent以后会在Compoent View里显示添加的组件，我们可以将该组件的名字修改为我们想要的名字，我将这个测试用的组件取名为TEST，如下图：![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/169dd8d64b124ae7ac8234ea81b3840d.jpg)2． 然后在TEST组件上点击鼠标右键，弹出快捷菜单，选择Open Specification…选项，对其具体属性进行设置，如图：![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/44a8926a5c8a443ca32b7e8d587f9b7e.png)3． 在弹出的Test组件设置窗口中，将Language项设置为ANSI C++，如下图：![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/9045970649674c228f58917606bcc29f.jpg)点击设置窗口的Apply按钮，然后点击OK4． 设置好以后，再用鼠标右键点击TEST组件，这时弹出的快捷菜单会有所改变，多了ANSI C++选项，选择ANSI C++ -> Open ANSI C++ Specification…选项，如下图：![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/1f1e446349b34bf0ba5b859e4ac3efa2.jpg)5． 选择该选项后，弹出ANSI C++ Specification设置窗口，将Source file root directory设置为我们需要进行类图转换的VC工程的目录。我使用D盘下的Preview工程，所以设置为该目录，如下图：**
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/e99e03eb16944d489be17d84d33bb601.jpg)
6． 然后我们点击Add Files按钮，添加我们需要进行转换的源文件。这些源文件既包括类的*.cpp实现文件也要包括相应的*.h头文件，如下图：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/35d13e0f6cd2426e91892e3c2ccb6143.jpg)
              7．添加完工程文件后，可以再Project files列表中看到添加的头文件和实现文件。我们点击确定按钮，关闭该设置窗口。右键点击TEST组件，选择ANSI C++->Reverse Engineer…选项，如下图：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/14ed43695afa4ba4ae12238a93e64bf9.jpg)
               8． 在弹出的Reverse Engineer设置窗口中选择需要转换的类或踢出不需要转换的类，结构体，如下图：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/a75f59a1d796488d92455a83b585d203.jpg)
              9． 选好要转换的类后，点击OK按钮便开始进行转换，转换成功后会提示转换工程完成：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/18cec575c95948feb39ff4a904c007b6.jpg)
              10．在转换工程成功后，再次右键点击TEST组件，选择Open Specification…选项菜单，弹出TEST组件设置窗口，将Language改为VC++，点击Apply和OK按钮，如下图：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/eb50ecf962b54e4abe53ccbeccbc5862.jpg)
              11.   再用鼠标右键点击TEST组件，弹出快捷菜单将有所变化，选择Assign To Project....菜单项，如下图：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/9dcb98b43bb64eb585f926d3c79709f8.jpg)
　　　　12.     弹出Compoent Assignment Tool对话框，右键点击左边列表VC++下的TEST项目，弹出快捷菜单，选择                  Properties选项，如图：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/61cea6cb34d147c199b3401e18f843bd.jpg)
              13．选择Properties后，弹出对话框，要对Workspace File进行设置：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/2d3b89a8cb31488086566130b7090df3.jpg)
              14．在选择VC工程文件的对话框中选择Exsting标签，然后找到需要转换的VC的工作区文件，然后点击打开
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/c0a194356bc74f01b5358b060687248e.jpg)
              15．设置了VC工作区文件后，VC工程文件会自动找到，如果出现问题说明要安装VC：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/23f6a72982f648fa93b1b56c465aae97.jpg)
　　　　16．工作区和工程文件设置完成后，点击OK，关闭各个对话框。然后再次右键点击TEST组件，在弹出的快捷菜单中选择Update Model…菜单项目，如图：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/6b3e8b7908514059bb91d9f5923f0ced.png)
              17．在弹出的Model Update Tool对话框中直接点击Finish按钮：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/5a8849916a6a40cdbfdde3d7a27c55aa.jpg)
              18．在Model Updata的过程中，可能会因为找到代码及头文件不完整的类、结果、枚举类型，这些类型由于我们在加入工程的*.cpp和*.h时候不完整，不能对这些类型进行完整Model Updata，这个时候会弹出对话框。我们可以把其剔除掉，如下图：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/505068f05a0547fd84b877408de3fa0b.jpg)
              19．将其剔除后Model Updata过程继续，完成后将在右边产生类，结构的UML图：
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](https://p-blog.csdn.net/images/p_blog_csdn_net/gamer_gerald/a6789c71d9dc4524b169f6b31ffad7f7.jpg)
rational rose2003中反向vc++工程
如果我们已经做好了一个vc++工程。我们可以通过rational rose的逆向工程自动从工程提取类图。
1，打开rational rose 2003,新建一个默认的mdl文件。
2，打开add_ins->add_ins manager 菜单，打开了 add_ins manager 窗口，确保 vc++前面的复选框被选中。
3，打开菜单 tools->visual c++->update model from code  打开向导。
4，点下一步，进入组件选择界面。
5，选择VC++前面的复选框，点击 Add Component 选择你的vc++工程的dsw文件。
6，然后点下一步。
7，点完成，等待一会儿。类图就被自动加进来了。被自动添加到：logical view / vc++ reverse engineered  /  工程名。
8，默认情况下我们在类图上，只能看到类名，看不到类的属性和方法。所以要选择一个类，点右键，在弹出的菜单上选择options->show all attributes ，然后选择 options->show all operations。然后就可以看到类的方法和属性了。
Rational Rose进行C++代码的逆向工程介绍
> Rational Rose是利用UML（统一建模语言）进行分析和设计面向对象软件系统的强大的可视化工具，可以进行项目需求分析、结构规划和生成框架代码，还可以支持从现有系统逆向转出工程代码，生成Rose模型的功能。
2004年10月，IBM推出了支持最新的UML2.0的可视化建模工具[Rational
 Software Architect](http://www-900.ibm.com/cn/software/rational/products/awdtools/architect/swarchitect/index.shtml)（见注释①） 和[IBM
 Rational Software Modeler](http://www-900.ibm.com/cn/software/rational/products/awdtools/modeler/swmodeler/index.shtml)（见注释②）。虽然它们支持在建模功能上有了更好的改进、支持了更新的标准，但是RSA的精彩功能主要是集中在对Java应用的支持，而IBM Rational Software Modeler则是主要关注系统的模型设计，如果要从结构上分析C++编写的系统的代码，[Rational
 Rose](http://www-900.cn.ibm.com/cn/software/rational/products/rose/index.shtml)还是首选的工具。
接下来的文章将会对如何利用Rational Rose 的逆向转出工程来进行系统分析进行更加详细地阐述。
一．Rational Rose逆向工程介绍
逆向工程（Reverse Engineer）就是从现有系统的代码来生成模型的功能。分析已有的代码其主要的目的就是了解代码结构和数据结构，这些对应到模型图就是类图、数据模型图和组件图（对UML各种模型图的描述见注释③），也就是通过Rational Rose的逆向工程所得到的结果。Rational Rose所支持的逆向工程功能很强大，包括的编程语言有C++, VB, VC, Java, CORBA，以及数据库DDL脚本等等，并且可以直接连接DB2, SQLServer, Oracle和Sybase等数据库导入Schema并生成数据模型。
很多大型的C++开发的产品都涉及到数据库的使用，对这种大型系统的开发，尤其是做二次开发的情况下，主要的难点就是对源码和数据库结构的分析。而利用Rose的逆向工程这一功能，就可以完成代码'类图以及数据库Schema->数据模型图的转换，解决这两大难点，可以使开发和设计人员在对这种大型系统的升级、分析和开发中，更为方便、快捷、有条理地掌握系统结构，不用再为分析庞大的系统结构而头疼。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/i/c.gif)
二．如何用Rational Rose进行C++代码分析
这一节主要介绍用户如何使用Rose的逆向工程生成UML模型，并用来进行C++代码的结构分析。
Rational Rose可以支持标准C++和Visual C++的模型到代码的转换以及逆向工程。下面将详细地说明这两种C++ project的逆向工程的步骤和具体操作。
1． ANSI C++（标准C++）逆向工程（Reverse Engineer）使用标准C++逆向工程，需要在组件图（component view）中创建一个组件（component），设置好需要进行转换的组件的信息，也就是该组件的语言、所包含的文件、文件所在的路径、文件后缀等等信息，然后Reverse Engineer就可以根据给定的信息将代码转换成类图了。
a) 右键点击组件视图（Component View），选择New->Component，创建一个新的组件
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image001.png)
b) 将component的language属性设定为ANSI C++
①选中创建的component，点击右键，选中Open Specification
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image003.png)
②在这个对话框中将该component的language设定为ANSI C++
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image005.png)
c) 配置该ANSI C++ component，设置好该component中包含的C++代码文件，并进行C++语言的详细设置
①选中该component，点击右键，选择ANSI C++->Open ANSI C++ Specification
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image007.png)
②把Source file root directory设定为你的C++源码文件所在的路径，并且将需要转换的文件添加到Project Files中，视你的需要来做其它的设定，比如：头文件扩展名等等。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image009.png)
d) 将设置好的component转换成模型图
①选中设置好的component，点击右键，选中ANSI C++->Reverse Engineer
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image011.png)
②选中需要转换的class，点击ok，一个component的逆向转换就完成了。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image013.png)
2． Visual C++ 在使用这个功能的时候，要求用户的机器上同时安装Visual Studio。Visual C++的逆向工程也需要首先创建一个component，并将该component的language属性设置为VC++。Rational Rose对VC++模型的转换是通过读取VC++ Project File的信息来实现的，用户只需要将对应的project file信息指定给component就可以了。
a) 将VC++ Project的信息赋给刚刚创建好的component。
①选择该component，点击右键，选择Assign To Project
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image015.png)
②在对话框中找到刚刚创建的component，右键点击并选择Properties。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image017.png)
③在下面对话框中选中需要进行转换的Project，点击ok，该component就与需要转换的project关联上了。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image019.png)
b) 将设置好的component转换成模型图
选中设置好的component，点击右键，Update Model…，将会弹出确认对话框，选中需要转换的class，点击finish就可以了。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image021.png)
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image023.png)
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/i/c.gif)
三．如何用Rational Rose进行数据库结构分析
利用Rational Rose可以进行关系数据库的数据模型设计并导出DDL语言脚本，相反还可以导入已有的DDL脚本生成数据模型以及连接SQLServer, DB2, Oracle等数据库并导入数据库的schema生成数据模型。
下面以SQL Server为例说明一下逆向转换工程的步骤。
a) 首先，选择Tools->Data Modeler-> Reverse Engineer
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image025.png)
b) 在进行逆向工程转换的过程中可以选择是从数据库还是DDL脚本进行转换，如果是DDL脚本转换，就直接给定DDL脚本文件位置就可以了。本例子给出的是连接SQLServer数据库将schema导入生成数据模型的过程，所以选中Database，进入下一步。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image027.png)
c) 填写数据库相关信息
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image029.png)
d) 选中所有需要转换的schema
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image031.png)
e) 选择需要将哪些数据库中的元素导入到数据模型中
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image033.png)
转换操作会自动在组件图（Component View）中添加数据库组件，并在逻辑图（Logical View）中生成以《Schema》作为命名开头的数据模型。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image035.png)
打开数据模型，可以看到从数据库中转换过来的各个表和视图。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image037.png)
从数据模型图中可以看到表的结构以及各个表和视图之间的逻辑关系。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/i/c.gif)
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/i/c.gif)
四．如何得到逆向工程的模型图
在Rational Rose中，有些模型图是不会自动生成的，很多时候这个工作需要用户手工来完成。也就是说，Rational Rose只负责生成模型，包括模型中的元素、元素的属性以及各个元素之间的关系，但是需要用户做一些额外的工作来得到视图。
首先，通过逆向工程，用户已经得到了UML模型或者数据模型的各个组件以及它们之间的关系。 下一步需要在该模型上创建一个视图，它们可以是类图（class diagram，描述系统的静态结构）或者数据模型图（Data model digram，描述关系数据结构）。然后，手动从左边的explorer中将各个元素拖进视图中，在这个过程中，各个元素之间的关联关系会自动在图中表示出来，而不需要用户再做其他工作。
例如：生成一个数据模型的数据模型图的过程
从左边将数据模型中的数据元素拖到右边的数据模型图中，表CustomerCustomerDemo和表CustomerDemographics之间的依赖关系的菱形箭头是自动生成的，无需手工操作。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image039.png)
一般来说，一个系统中所涉及的数据元素非常多，导致视图很拥挤，排版也非常困难。Rational Rose提供的自动排版功能可以很方便地帮助用户解决这个问题。
选择Format->Layout Diagram，系统会将图中的所有元素用最优方式重新排列，给用户一个清晰的视图。
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image041.png)
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image043.png)
[点击查看大图](http://www.ibm.com/developerworks/cn/rational/r-shenzj/images/image043.png)
![rational rose使用资料教程汇总 - 思想有多远 - 思想有多远……](http://www.ibm.com/i/v14/rules/blue_rule.gif)
[img]http://www.ibm.com/i/c.gif[/img]
[img]http://www.ibm.com/i/c.gif[/img]
[img]http://www.ibm.com/i/v14/icons/u_bold.gif[/img]
[**回页首**](http://www.ibm.com/developerworks/cn/rational/r-shenzj/#main)
五．总结
了解了Rational Rose的逆向工程功能，用户就可以方便地利用这一工具进行大型数据库相关的C++系统的分析和研究了。
[img]http://www.ibm.com/i/v14/rules/blue_rule.gif[/img]
[img]http://www.ibm.com/i/c.gif[/img]
[img]http://www.ibm.com/i/c.gif[/img]
注释①IBM Rational Software Architect是一个集成式设计和开发工具，使用户可以用UML模型驱动的开发方式开发出结构清晰的应用程序和服务。
②IBM Rational Software Modeler是基于UML2.0的可定制的可视化建模和设计工具，遵循它所提供的工作流程，可以使得系统设计师，系统分析师，设计人员等写出更为清晰的文档，更为有效地进行沟通和设计工作。
③UML提供的各种类型的模型描述图
用例图(Use Case Diagram)：描述系统提供的一个功能单元，主要目的是帮助开发团队以一种可视化的方式理解系统的功能需求，包括基于基本流程的"角色"（actors，也就是与系统交互的其他实体）关系，以及系统内用例之间的关系。
序列图(Sequence Diagram)：显示具体用例（或者是用例的一部分）的详细流程。
状态图(Statechart Diagram)：表示某个类所处的不同状态和该类的状态转换信息。
活动图(Activity Diagram)：表示在处理某个活动时，两个或者更多类对象之间的过程控制流。
类图(Class Diagram)：表示不同的实体（人、事物和数据）如何彼此相关；换句话说，它显示了系统的静态结构。
组件图(Component Diagram)：提供系统的物理视图。组件（Component）表示模型代码库、执行库和其它组件的信息。组件是代码的实际模块，组件图主要用户是负责控制代码和编译部署应用程序的人。有些组件是代码库，有些组件是运行组件，如执行文件或动态链接库（DLL）文件。）
部署图(Deployment View)：表示该软件系统如何部署到硬件环境中。
数据模型图(Data Model Diagram)：描述关系数据结构。
