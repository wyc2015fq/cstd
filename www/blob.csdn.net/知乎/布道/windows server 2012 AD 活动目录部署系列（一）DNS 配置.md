# windows server 2012 AD 活动目录部署系列（一）DNS 配置 - 知乎
# 



前言

本系列将介绍在windows server 2012 下 AD的详细部署文档，首先我们需要做以下准备工作：

1、导入五台备用的虚拟机

2、准备好虚拟机后，计算机名分别命名如下：DNS、Florence、Firenze、Berlin、Perth。

一般情况下， 域中有三种计算机，一种是域控制器，域控制器上存储着 Active Directory； 一种是成员服务器，负责提供邮件，数据库，DHCP等服务；还有一种是工作站， 是用户使用的客户机。我们准备搭建一个基本的域环境，拓扑如下图所示，

Florence 是域控制器，Berlin 是成员服务器，Perth 是工作站。 



![](https://pic3.zhimg.com/v2-90afcd1cea622ea6ddd3e9e5d7779ef6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='576' height='555'></svg>)






部署一个域大致要做下列工作：

**1 DNS****前期准备**

**2****创建域控制器**

**3****创建计算机账号**

**4****创建用户账号**

**一 DNS前期准备**

DNS 服务器对域来说是不可或缺的，一方面，域中的计算机使用 DNS 域名，DNS 需要为域中的计算机提供域名解析服务；另外一个重要的原因是域中的计算 机需要利用 DNS 提供的 SRV 记录来定位域控制器，因此我们在创建域之前需 要先做好 DNS 的准备工作。那么究竟由哪台计算机来负责做 DNS 服务器呢？ 一般工程师有两种选择，要么使用域控制器来做 DNS 服务器，要么使用一台单独的 DNS 服务器。这里使用一台独立的计算机来充当 DNS 服务器，这台 DNS 服务器不但为域提供解析服务，也为公司其他的业务提供 DNS 解析支持， 大家可以根据具体的网络环境来选择 DNS 服务器。




**1、添加“DNS服务器”角色，如下图所示：**



![](https://pic1.zhimg.com/v2-8dd9456ce324fe958701a5b4617b7690_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='728'></svg>)





![](https://pic2.zhimg.com/v2-b4c656bc76cf5c4e3e7db8c034e6cca5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='567'></svg>)





![](https://pic4.zhimg.com/v2-ee0719b0522cb129a4122085cb65d91f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='567'></svg>)





![](https://pic3.zhimg.com/v2-3f3327de778f83d89e878691e9173426_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='567'></svg>)





![](https://pic3.zhimg.com/v2-41442051d1ec6c6d8cd536988569ab9e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='801' height='569'></svg>)





![](https://pic1.zhimg.com/v2-8bf663580fd82d27d192960f81eb37b0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='567'></svg>)





![](https://pic2.zhimg.com/v2-52a5f7bdf9595f717f70aff3d59a11d9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='567'></svg>)





![](https://pic3.zhimg.com/v2-822a07bf2e194ac1b741889c9ff2785a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='567'></svg>)





![](https://pic4.zhimg.com/v2-c04a2d58a95b19fc39d74b4f3ed7226f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='567'></svg>)





![](https://pic4.zhimg.com/v2-f415dd88c34df02b28241793e6a01373_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='567'></svg>)



至此，DNS角色添加成功后，重启计算机。




**2、创建区域并允许动态更新**

首先我们要在 DNS 服务器上创建出一个区域，区域的名称和域名相同，域内计 算机的 DNS记录都创建在这个区域中。我们在 DNS服务器上打开DNS管理器， 如下图所示，右键单击正向查找区域，选择新建一个区域，出现新建区域向导后， 点击下一步继续。 



![](https://pic4.zhimg.com/v2-dd0d9a0bac14e383b617d6780d970d93_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='768'></svg>)





![](https://pic4.zhimg.com/v2-032fbc569e0742bb3499d864d329be47_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='596' height='455'></svg>)





![](https://pic2.zhimg.com/v2-bfa2370162bf2bffa4e526ada94ead55_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='596' height='455'></svg>)![](https://pic2.zhimg.com/v2-b247fe3caf74f390f718814d8122598d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='596' height='455'></svg>)





![](https://pic1.zhimg.com/v2-5a8d0c6fe2afb8ea936a0898e1293b10_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='596' height='455'></svg>)





![](https://pic3.zhimg.com/v2-7eb1a0a8f365545c2d568ac69e4231f6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='596' height='455'></svg>)





![](https://pic3.zhimg.com/v2-35d6191663e9e2714a22133462247586_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='596' height='455'></svg>)






**3、检查NS和SOA记录**

区域创建完成后，一定要检查一下区域的 NS 记录和 SOA 记录。NS 记录描述了有多少个 DNS 服务器可以解析这个区域，SOA 记录描述了哪个 DNS 服务器是区域 的主服务器。如果 NS 记录和 SOA 记录出错，域的创建过程中就无法向 DNS 区域中写入应有的记录。打开 DNS 管理器，在[http://adtest.com](https://link.zhihu.com/?target=http%3A//adtest.com)区域中检查 ns 记录，如下图所示，我们发现 ns 记录不是一个有效的完全合格 域名，我们需要对它进行修改。



![](https://pic1.zhimg.com/v2-06cdcded9f348ebdd863bea47e6ec18c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='768'></svg>)





![](https://pic3.zhimg.com/v2-ab6311394730bc72f03ef1f6190c30ce_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='479' height='525'></svg>)





![](https://pic4.zhimg.com/v2-53d6482f0d656cd1f22ed05a9343131b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='632' height='472'></svg>)





![](https://pic4.zhimg.com/v2-6895ece841d67192c27ff80ce16eff17_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='479' height='525'></svg>)





![](https://pic2.zhimg.com/v2-5a73f06e2f43c548e5f306ecec49191d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='768'></svg>)





![](https://pic1.zhimg.com/v2-1f5a4295bc72305010623b70a1e50b90_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='479' height='525'></svg>)





![](https://pic4.zhimg.com/v2-8c4ffde5d1d8338ed70f7b446a39ea03_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='768'></svg>)





![](https://pic1.zhimg.com/v2-fdd58397ef46b5bbd1e42d5e363423d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='728'></svg>)



至此，DNS 准备工作完成，我们接下来可以部署域了。


