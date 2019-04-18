# windows server 2012 AD 活动目录部署系列（三）加入域并创建域用户 - 知乎
# 



**1、加入域**

创建计算机账号就是把成员服务器和用户使用的客户机加入域，这些计算机加入 域时会在 Active Directory 中创建计算机账号。

以 Berlin 为例为大家介绍如何把计算机加入域，首先要确保 Berlin 已经使用了192.168.11.1 作为自己的 DNS 服务器，否则 Berlin 无法利用 DNS 定位域控制器。



![](https://pic1.zhimg.com/v2-04e1de612f1dc9f01466c2e923d61198_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='479' height='525'></svg>)






右键点击“计算机”，打开‘属性’对话框，再点击“更改”，



![](https://pic3.zhimg.com/v2-2274f8c6e1f9588c2db9157f187d61da_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='486' height='590'></svg>)






选择隶属于“域”，输入要加入的域名：[http://adtest.com](https://link.zhihu.com/?target=http%3A//adtest.com)。



![](https://pic2.zhimg.com/v2-71b68e7a2475d8fa5de60f84bc11af51_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='391' height='453'></svg>)






点击“确定”后，会弹出输入域管理员凭证的对话框，



![](https://pic2.zhimg.com/v2-e8b56498b5b3ef819340399aeb631321_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='439' height='295'></svg>)






点击“确定”，提示“欢迎加入域”，然后按提示重启计算机。



![](https://pic4.zhimg.com/v2-65350c403da05931b979b1c736c44343_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='487' height='586'></svg>)









这时在域控制器 Florence 上打开 Active Direc tory 用户和计算机，如下图所示，我们发现 Berlin 的计算机账号已经被创建出 来了。



![](https://pic3.zhimg.com/v2-b0beed6ec921533c0297aabd78543c02_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='768' height='537'></svg>)



至此，计算机加入域已成功！




**2、创建用户账号**

计算机加入域后，我们需要为企业内的员工在 Active Directory 中创建关 联的用户账号。首先我们应该在 Active Directory 中利用组织单位展示出企业的管理架构，如下图所示，我们为大家演示一下如何创建一个组织单位。打开 Active Directory 用户和计算机，选择新建组织单位。



![](https://pic3.zhimg.com/v2-bc59986bee4b3f7569aab239e15cbfb6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='770' height='536'></svg>)





![](https://pic1.zhimg.com/v2-8ae816c020733ded4c40f20d265c844c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='524' height='438'></svg>)






点击“确定”，完成组织单位的创建，接着在该组织单位下创建用户，



![](https://pic2.zhimg.com/v2-b16b8a45642ab476bcc9a21991cd4729_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='769' height='539'></svg>)






输入帐号和姓名，



![](https://pic2.zhimg.com/v2-8e8ad29ae18b3006004baecc9f6f7629_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='524' height='438'></svg>)






输入密码（密码有一定的复杂度：由字母、数字、符号三种字符组成并超过八位数），

并在下面选择“密码永不过期”，这步很重要，



![](https://pic2.zhimg.com/v2-183e16d3c66b5f0038f8e937d0e3ba6d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='524' height='438'></svg>)





![](https://pic2.zhimg.com/v2-2a07a7e48f5a8fd46b7d34345ba9fc91_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='524' height='438'></svg>)






完成用户添加，如下图所示，



![](https://pic3.zhimg.com/v2-2155c22802f5110319d203c9bd495a96_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='768' height='537'></svg>)






目前为止，我们已经创建了一个域，也在域中创建了计算机账号和用户账号。我们在下篇博文中将介绍域用户权限分配问题。


