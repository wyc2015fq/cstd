# 二、服务器优化（6）WSRM管理多个实例 - weixin_33985507的博客 - CSDN博客
2013年07月19日 09:51:46[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**一、多实例的资源竞争**
　　从SQL Server 2000开始，允许在一台服务器上安装多个SQL Server实例。其中只有一个可以是“默认实例”，实例的名称为MSSQLSERVER；其余的实例都必须是“命名实例”。
　　在一台服务器上安装多个SQL Server实例时，可以为不同的实例分配资源。资源调控器在这里不适用，因为资源调控器仅基于单个实例。因此，需要使用WSRM（Windows System Resource Manager）。
**二、安装WSRM**
　　从Windows Server 2008开始，WSRM就成为了Windows的内置功能。
　　安装WSRM时必须同时选择安装“Windows内部数据库”。
![203550129.png](http://blog.51cto.com/attachment/201308/203550129.png)
![203814382.png](http://blog.51cto.com/attachment/201308/203814382.png)
![204055811.png](http://blog.51cto.com/attachment/201308/204055811.png)
**三、使用WSRM**
**1. 打开WSRM**
　　在“管理工具”中打开“Windows 系统资源管理器”。
　　在“连接到计算机”对话窗口选择“此计算机”。
![204249761.png](http://blog.51cto.com/attachment/201308/204249761.png)
**2. 新建进程匹配条件**
　　依次展开“Windows 系统资源管理器”、“进程匹配条件”。在右键菜单中选择“新建进程匹配条件”。
![204638168.png](http://blog.51cto.com/attachment/201308/204638168.png)
　　新建一个名为“MSSQLSERVER”的条件。然后单击“添加按钮”添加一个规则。
![205314509.png](http://blog.51cto.com/attachment/201308/205314509.png)
　　在“添加规则”窗口，单击“选择”按钮选择一个服务。
![205315227.png](http://blog.51cto.com/attachment/201308/205315227.png)
 在“添加 已注册的服务”对话窗口，选择默认实例。
![205315214.png](http://blog.51cto.com/attachment/201308/205315214.png)
完成上述操作后，再以类似的步骤新建一个名为“SQLEXPRESS”的条件，在“添加 已注册的服务”时选择名为SQLEXPRESS的命名实例。
**3. 新建资源分配策略**
　　展开“资源分配策略” 节点，在右键菜单中选择“新建资源分配策略”。
![210445986.png](http://blog.51cto.com/attachment/201308/210445986.png)
　　新建一个名为“BalanceInstance”的策略。然后单击“添加”按钮添加一个进程匹配条件。 
![210447375.png](http://blog.51cto.com/attachment/201308/210447375.png)
　　在“添加或编辑资源分配”窗口，在“进程匹配条件”下拉列表中选择一个进程匹配条件，并为其分配资源，例如，45%的CPU。 
![210449396.png](http://blog.51cto.com/attachment/201308/210449396.png)
　　然后再添加第二个进程匹配条件。这样就把2个实例都分配了资源。如下图。 
![210451404.png](http://blog.51cto.com/attachment/201308/210451404.png)
**4. 设为管理策略**
　　在“资源分配策略”的详细列表窗格，选择上一步新建的资源分配策略，在右键菜单中选择“设置为管理策略”。 
![210453521.png](http://blog.51cto.com/attachment/201308/210453521.png)
　　设置完成之后，这个资源分配策略的右侧会显示“{管理}”的标识。 
![210456894.png](http://blog.51cto.com/attachment/201308/210456894.png)
**本文结语：**
　　资源调控器只能在某个实例内部调控工作负荷。WSRM则基于服务器，为服务器上的多个实例分配CPU和内存的百分比。
