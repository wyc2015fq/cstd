# uiOne 技术 - 我相信...... - CSDN博客





2010年04月06日 11:39:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：982标签：[brew																[actor																[手机																[扩展																[数据结构																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)




uiOne是一种数据驱动的用户界面技术，是建立在BREW和BUIW之上的。TrigPlayer将TrigML的逻辑表述文件解释并映射BUIW中的控件并显示出来，并通过Actor来访问AEE层提供的各种服务，uiOne在手机上的层次关系如图10-6所示。




**图**** 10-6****：****uiOne****在手机上的层次关系**



Trig是一个或一组应用中编译好的TrigML和其他资源的混合体，Triglet是一个Trig中的可更新部分。Trig是通过TrigPlayer执行的，TrigPlayer是uiOne的应用引擎，相当于一个解释器。Actor是由C代码实现的插件，通过Actor来调用手机上BREW AEE层API，因此可以理解成应用的扩展引擎。TrigBuilder是一个开发工具来编写TrigML，运行和调试手机的待机界面应用。

Trig 实际上就是用户界面，包括运营商定制的用户界面和内容更新频道的界面。Trigs包含了本地资源的显示组合，如图10-7所示，例如编译过的TrigML文件，图像文件，文本文件等。TrigML中的资源是按树状结构组织的，每一个资源都是唯一的一个叶子节点。




**图**** 10-7****：**Trigs是包含本地资源的显示组合



Actors 提供了使用底层代码的接口，通过Actor可以访问手机平台的特性和手机上的应用，能够使用底层代码的应用逻辑。本质上也是一个BREW 应用扩展, 类似于中间件, 负责和底层的BREW API 以及其他扩展应用进行接**互, 是uiOne 应用的底层逻辑处理部分。Actor提供了3种功能：改变应用中的数据结构和变量的值，处理Trig发送给Actor的命令消息，将变化的信息通过事件发给Trig。

Triglet是Trigs中可更新的部分（图10-8），通过OTA的方式下载到手机上，可以实现手机待机界面上指定内容的自动更新。在形式上表现为一系列资源文件的目录体系, 是对uiOne 应用的动态可更新部分的描述, 是一个资源文件包, 可无线下载, 负责UI的在线更新, 并可只对部分UI 进行更新。




**图**** 10-8****：**Triglet的内容更新示意



频道升级实际上是一种准PUSH服务，由用户选择了自己喜欢的内容频道和更新周期。客户定制服务器会发送SMS到终端用户，用户手机上的更新管理器会根据内容频道的URL到内容服务器获取相关内容（Triglet），然后由资源管理器将定制的频道内容呈现给用户，系统结构如图10-9所示。




**图**** 10-9****：**uiOne中频道内容更新的体系结构](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=actor&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




