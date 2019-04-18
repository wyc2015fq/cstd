# Ice手册，第39章IceGrid 第5节使用部署 - gauss的专栏 - CSDN博客
2014年07月14日 00:47:42[gauss](https://me.csdn.net/mathlmx)阅读数：388
个人分类：[服务器架构与网络编程](https://blog.csdn.net/mathlmx/article/category/944322)
表时间：2008-05-01浏览量：11646评论数：0挑错数：0
Internet Communications Engine (Ice) 是现代面向对象的中间件，它支持C++, C#, Java, Python, Ruby, PHP, 和 Visual Basic开发语言。 世界各地已经有许多公司在他们的关键项目上使用Ice。Ice简单易学，并且提供了强大的网络基础设施和为技术应用提供了许多特性。Ice相比SOAP和CORBA这类太慢太复杂的技术有太多亮点。Ice是一个自由软件，所有代码都可用，是在GNU General
 Public License (GPL)下发布的。 对于希望使用不带源码的Ice的软件需要遵守一个商业许可（Commercial licenses，http://www.zeroc.com/licensing.html） 译者Blog： http://blog.chaoskey.com
本文來源于:[第39章IceGrid 第5节使用部署(翻译) | 夢想混沌](http://blog.chaoskey.com/2008/05/01/105/)
# 39.5 部署使用
在本节，我们研究如何使用IceGrid的部署设施扩展我们这个示例程序的能力。
## 39.5.1 架构
这个被调整过的程序构架由一个单独的IceGrid节点构成。，它负责我们这个编码服务，而这个服务运行在一个名为ComputeServer的机器上。[图39.5](http://article.yeeyan.org/view/chaoskey/7194/#119143) 显示了一个客户端通过它间接代理的初始调用，并且激活IceGrid使调用成为可能的任务。![](http://www.zeroc.com/doc/Ice-3.2.1/manual/images/IceGrid.40.5.1.jpg)
图39.5. 部署ripper应用的构架
对比[39.4.1章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.4.html#76913)的构架， 我们不需要花时间手动启动我们的服务。在这个修改后的程序，客户端的寻址请求提示注册器（Registry）查询节点的服务器状态并且根据需要启动它。一旦这个服务启动成功, 这个寻址请求完成，并且随后客户端直接和服务端通信。
## 39.5. 描述
我们可以利用icegridadmin命令行工具部署我们的程序 (见 [Section 39.21.1章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.21.html#80700)), 但是首先我们必须在XML中定义我们的描述。 这个描述是非常简短的:
<icegrid>
<applicationname="Ripper">
<nodename="Node1">
<serverid="EncoderServer"
exe="/opt/ripper/bin/server"
activation="on‑demand">
<adaptername="EncoderAdapter"
id="EncoderAdapter"
register‑process="true"
endpoints="tcp"/>
</server>
</node>
</application>
</icegrid>
对于IceGrid的用途而言, 我们已经把我们的application命名为Ripper. 他包括一个单独的server：EncodeServer，被分配到这个节点Node1[1](http://article.yeeyan.org/view/chaoskey/7194/#366). 这个服务的exe属性是包含路径名的可执行程序,并且activation属性是on‑demand
 ，表明是根据需要来激活这个服务。
这个对象适配器的描述更有趣。就如我们所看到那样，它的name和id属性都指定为EncoderAdapter. name的值反映的是这个适配器（adapter）在服务进程中的名字 (即, createObjectAdapter所传递的参数)，这就是他被用作配置的用途,而在注册器（Registry）中，id的值唯一的标识了这个适配器（adapter ），并且被用在间接代理中.
 这两个名字并不要求一定一样，如果我们忽略这个id属性，IceGrid将会结合server的名字和adapter的名字组成一个唯一值，产生如下的标识：
EncoderServer.EncoderAdapter
register-process属性使得在需要的时候IceGrid能方便的shutdown服务成为可能。(见
[Section 32.17.6章节](http://article.yeeyan.org/view/chaoskey/7194/Adv_server.33.17.html#99397)), 当endpoints为adapter定义了一个或多个endpoint. 具体解释[39.4.4.章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.4.html#77885), 这些endpoints不需要有固定的端口.
见[39.16章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.16.html#94076) 有详细的使用XML定义的描述.
## 39.5.3 配置 Registry 和 Node
在[Section 39.4.2章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.4.html#77721), 我们建立了一个给Registry使用的目录
/opt/ripper/registry.根据我们的用途，这个节点（ node ） 也需要一个子目录， 所以我们建立
/opt/ripper/node. 其次，这些目录必须在启动Registry和Node前存在.
我们还需要建立一个Ice配置文件来保存 Registry和Node需要的属性. 这个文件 
/opt/ripper/config 包含如下属性:
# Registry属性
IceGrid.Registry.Client.Endpoints=tcp -p 4061
IceGrid.Registry.Server.Endpoints=tcp
IceGrid.Registry.Internal.Endpoints=tcp
IceGrid.Registry.AdminPermissionsVerifier=IceGrid/NullPermissionsVerifier
IceGrid.Registry.Data=/opt/ripper/registry
# Node属性
IceGrid.Node.Endpoints=tcp
IceGrid.Node.Name=Node1
IceGrid.Node.Data=/opt/ripper/node
IceGrid.Node.CollocateRegistry=1
Ice.Default.Locator=IceGrid/Locator:tcp -p 4061
这个Registry和Node能够共享这些文件. 实际上，通过定义IceGrid.Node.CollocateRegistry=1, 我们已经表示了Registry 和Node 应该运行在相同的进程里.
[39.4.2章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.4.html#77721) 描述了Registry属性. 一个差别是这里我们不再定义IceGrid.Registry.DynamicRegistration. 通过忽略这个属性,我们强制这个Rgistry拒绝对象适配器（object adapters）的没有被部署的注册.
这个Node属性的具体解释如下:
• IceGrid.Node.Endpoints
这个属性指定了Node的endpoints. 没有固定端口的要求.
• IceGrid.Node.Name
这个属性定义了这个节点的唯一名字. 这个值必须和上面的[39.5.2章节](http://article.yeeyan.org/view/chaoskey/7194/#119152)的XML描述一致。
• IceGrid.Node.Data
这个属性指定了Node的数据存放目录.
• Ice.Default.Locator
这个属性是为了icegridadmin工具性定义的. 如果Registry不包括在这个节点中，那么这个Node也将需要这个属性.
[39.4.3章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.4.html#78496) 提供了关于这个设置的更多信息.
## 39.5.4 配置 Server
Server的配置完全靠描述文件来配置. 在部署过程中，这个节点为没个服务建立一个子目录树， 在这个目录树的内部，这个节点建立了一个配置文件，这个文件包含了这个服务的描述。作为一个实例，在[39.5.２章节](http://article.yeeyan.org/view/chaoskey/7194/#119152) 中的适配器（adapter）的描述产生了的Ｓｅｒｖｅｒ配置文件，有如下属性:
# Serve　配置
Ice.ServerId=EncoderServer
Ice.ProgramName=EncoderServer
# Object adapter EncoderAdapter
EncoderAdapter.Endpoints=tcp
EncoderAdapter.AdapterId=EncoderAdapter
EncoderAdapter.RegisterProcess=1
Ice.Default.Locator=IceGrid/Locator:default -p 4061
正如你所看到的，IceGrid从描述文件（类似于我们上面[39.4.4章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.4.html#77885)建立的）生成的这个配置文件 ,有两个附加属性:
• EncoderAdapter.RegisterProcess
这个属性被建立，是因为这个适配器定义了register-process属性. 这个属性许可 IceGrid Node 可优雅的关闭服务。
• Ice.ServerId
这个属性设置了Server的id，并且当任何一个服务的对象适配器（object adapters）定义了RegisterProcess,这个属性就是必须的 . 见 [32.17.6章节](http://article.yeeyan.org/view/chaoskey/7194/Adv_server.33.17.html#99397) 有更多的信息.
使用为Ripper应用我们建立的目录结构,这个为了EncoderServer的配置文件有如下的名字:
/opt/ripper/node/servers/EncoderServer/config/config
注意，这个文件不应该被直接编辑，因为任何的改都将会由于下一次Node产生这个文件而丢失.正确的方法是将需要添加到这个文件的属性包含在server的xml的描述文件中.举个例子，我们能够修改server描述文件，添加一个Ice.Trace.Network=1属性，如下：
<icegrid>
<applicationname="Ripper">
<nodename="Node1">
<serverid="EncoderServer"
exe="/opt/ripper/bin/server"
activation="on‑demand">
<adaptername="EncoderAdapter"
id="EncoderAdapter"
register‑process="true"
endpoints="tcp"/>
<propertyname="Ice.Trace.Network"
value="1"/>
</server>
</node>
</application>
</icegrid>
当一个节点激活一个服务是，他通过server位置的配置文件来使用--Ice.Config这个命令行参数。 如果用户手动通过命令行启动一个服务，你必须执行带上这个参数.
## 39.5.5 启动IceGrid
现在一个配置文件已经写好，并且相关的的目录结构以及被预先准备，我们准备开始启动Registry和Node. 使用一个Registry和Node在一起的情况，我们只需要使用一个命令启动:
$ icegridnode --Ice.Config=/opt/ripper/config
附加的命令行操作可以被支持，并且许可这个节点运行的 Windows服务或Unix守护进程. [39.20.2章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.20.html#80685) 中有更多的信息.
## 39.5.6 部署这个应用
在Registry启动并运行的情况下,现在是该我们部署我们这个应用的时候了. 像我们的客户端，这个icegridadmin工具也需要定义Ice.Default.Locator属性. 我们能够通过如下的命令行启动这个工具:
$ icegridadmin --Ice.Config=/opt/ripper/config
在我们确认可以连上Registry后，icegridadmin提供了一个命令行提示符，在这里我们可以部署我们的应用。 假定我们的xml描述存储在
/opt/ripper/app.xml,这个部署命令显示如下:
>>> application add "/opt/ripper/app.xml"
下一步，确认这个应用已经被部署：
>>> application list
Ripper
你能够通过这个命令启动这个服务：
>>> server start EncoderServer
最后，你能获取对象适配器的当前endpoints:
>>> adapter endpoints EncoderAdapter
如果你想进一步使用icegridadmin,你可以使用help命令 和 看[39.21.1章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.21.html#80700).
## 39.5.7 回顾
我们已经部署了我们第一个IceGrid应用,但是你可能会质疑它是否值得深入. 至少在早期阶段，我们已经获得如下好处:
• 在启动客户端前，我们不再需要手动启动encoder服务,因为IceGrid节点自动开始，如果在客户端需要的时候没有被激活.如果服务端由于某些原因被终止，比如IceGrid激活或服务程序错误，这个节点会重启它而不需要我们去干预.
• 我们能够使用IceGrid的一中管理工具远程管理应用. 具有远程修改应用，启动和停止服务，并且检查你配置的各个方面也是一大优势.
诚然，我们还没有取得很大进展，但我们的提高Ripper的性能的目标要好过只限于运行在一台单独的机器上的其它方案。我们的客户端现在有能力轻易代理运行在其它计算机上的encoding任务,但是我们还没有完成我们实际需要的并发的需要。举个例子,如果Client建立了好几个encoder 并且同时从多个线程使用它们，这个编码的性能可能会比单纯的直接在客户端编码还更坏, 作为远程计算机可能会在任务切换间处理一个密集的任务，从而抓取放缓。
## 39.5.8 添加节点
在我们的环境中添加一些节点将许可分散编码在多台计算机服务上加载。使用到目前为止我们所了解到的技术，让我们考虑添加一个节点的影响， 这个影响出现在我们的xml描述文件，配置，客户端程序。
### 描述
一个节点的添加，主要体现在复制粘贴:
<icegrid>
<applicationname="Ripper">
<nodename="Node1">
<serverid="EncoderServer1"
exe="/opt/ripper/bin/server"
activation="on‑demand">
<adaptername="EncoderAdapter"
register‑process="true"
endpoints="tcp"/>
</server>
</node>
<nodename="Node2">
<serverid="EncoderServer2"
exe="/opt/ripper/bin/server"
activation="on‑demand">
<adaptername="EncoderAdapter"
register‑process="true"
endpoints="tcp"/>
</server>
</node>
</application>
</icegrid>
注意，我们现在有两个Node元素而不是一个. 你可能临时简单的使用Host名字作为Node的名字。然而，通常那不是好主意。举个例子，你可能会在一个计算机上运行好几个IceGrid Node（比如测试的时候）。简单的，你可能不得不对一些点重命名Host 或者 迁移一个节点到其它的主机上。但是，除非你也重命名这个节点，你有一个含有主机名（可能过时了）的节点，当这个节点实际没有运行在这个主机上，会导致一些问题。很明显，这相比使用一个抽象的名字（比如Node1），是一个混乱糟糕的配置.
除了新的节点元素，注意Server的表示必须是唯一的。然而 这个适配器（adapter）的名字还是可以是EncoderAdapter，因为这个名字仅仅用于本地用途的服务进程呢个。 实际上为每个适配器取不同的名字会使服务的执行更复杂，因为在建立这个适配器时，它将会需要去发现这个名字是否可用.
我们已经把id属性从我们的适配器描述中移除了，IceGrid的默认的值就足以满足我们的目的 （见 [39.16.1章节](http://article.yeeyan.org/view/chaoskey/7194/IceGrid.40.16.html#80336)).
### 配置
为我们的那个结合了Registry-Node的进程，我们继续使用我们上面（[39.5.3章节](http://article.yeeyan.org/view/chaoskey/7194/#119177)）建立的配置文件。我们需要为了Node2分割配置文件，首先是为IceGrid.Node.Name定义一个不同的名字。然后，我们也不能有两个节点都配置了IceGrid.Node.CollocateRegistry=1
 ，因为只能有一个主Registry，所以我们必须移除这个属性:
IceGrid.Node.Endpoints=tcp
IceGrid.Node.Name=Node2
IceGrid.Node.Data=/opt/ripper/node
Ice.Default.Locator=IceGrid/Locator:tcp -h registryhost -p 4061
我们假定在计算机主机Nodes2上 ，/opt/ripper/node 指向本地文件系统目录，并且没有共享一个卷（volume），因为两个节点不能共享相同的数据目录。
我们也能修改本地代理到包含运行了Registry的主机地址。
### 重部署
保持好这新的描述文件后，我们需要重新部署这个应用。使用icegridadmin ,有如下命令：
$ icegridadmin --Ice.Config=/opt/ripper/config
>>> application update "/opt/ripper/app.xml"
### Client
我们加了一个新节点，但我们还是需要修改我们Client来利用它. 现在我们的Client可以代表的编码任务是2个MP3EncoderFactory对象中的一个。客户端选择一个用作间接代理:
• factory@EncoderServer1.EncoderAdapter
• factory@EncoderServer2.EncoderAdapter
为了在两工厂间分发，客户端可以使用随机数决定那个factory接受下一个任务：
stringadapter;
if((rand()%2)==0)
adapter="EncoderServer1.EncoderAdapter"$$
else
adapter="EncoderServer2.EncoderAdapter"$$
Ice::ObjectPrxproxy=
communicator‑>stringToProxy("factory@"+adapter);
Ripper::MP3EncoderFactoryPrxfactory=
Ripper::MP3EncoderFactoryPrx::checkedCast(proxy);
Ripper::MP3EncoderPrxencoder=factory‑>createEncoder();
这个设计中有几个缺点:
• 客户端必须在每次新的计算机添加或移除的时候修改，因为他知道所有的适配器标识.
• 客户端不能智能分发加载； 它会把重载的计算机当成空闲来分配任务。
我们将会在随后的章节里描述更好的方案.
[1](http://article.yeeyan.org/view/chaoskey/7194/#wwfootnote_inline_366)
因为典型情况下一个计算机只运行一个Node进程，你可能会临时把它的主机名作为Node的名字(比如 
ComputeServerNode).然而，一旦你迁移这个Node到其他的机器上时，这个命名的转换将会成为问题 .
