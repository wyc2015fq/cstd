# windows server 2012 AD 活动目录部署系列（二）创建域控制器 - 知乎
# 



**二 创建域控制器**

上一篇我们进行了DNS配置，有了 DNS 的支持，我们现在可以开始创建域控制器了，域控制器是域中的第一台服务器，域控制器上存储着 Active Directory，可以说，域控制器就是域的灵魂。我们准备在 Florence 上创建域控制器，首先检查 Florence 网卡的 TCP /IP 属性，注意，Florence 应该使用 192.168.11.1 作为自己的 DNS 服务器。 因为我们刚刚在 192.168.11.1 上创建了 [http://adtest.com](https://link.zhihu.com/?target=http%3A//adtest.com) 区域。



**![](https://pic4.zhimg.com/v2-5363b1fe940fd868b952424fdc6f304b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='479' height='525'></svg>)**






**1、添加“AD域服务”角色**

首先，我们需要添加域服务角色，此过程类似于“DNS服务”角色添加，这里不再重复讲，

如有需要，请参照 [http://blog.csdn.net/ronsarah/article/details/9423759](https://link.zhihu.com/?target=http%3A//blog.csdn.net/ronsarah/article/details/9423759)




**2、添加新域（提升域控制器）**

添加好域服务角色后，打开服务器管理器，会看右上角一栏中的旗帜图标下有一个黄色的警告，点击后会弹出如下图所示，



![](https://pic2.zhimg.com/v2-9c8c3186e0c00653d0dfee03af36f501_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='728'></svg>)






然后点击“将此服务器提升为域控制器”，打开域服务配置向导，如下图所示，我们选择“添加新林”，这个选项是什么意思呢？我们虽然只是简单地创建了一个域，但其实从逻辑上讲是创建了一个域林。因为域一定要隶属于域树，域树一定要隶属于域林。因为我们实际上是创建了一个域林，虽然这个域林内只有一棵域树，域树内只有一个树根。输入域名：[http://adtest.com](https://link.zhihu.com/?target=http%3A//adtest.com)。 



![](https://pic2.zhimg.com/v2-10b1f425180f3a3f921da02f7cd868e5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='775' height='567'></svg>)






去掉“DNS服务器”的勾选项，输入还原模式密码（此密码在本系列后续中的‘恢复域控制器’会用到），



![](https://pic4.zhimg.com/v2-3d034d0ba8490ab2c3fd45064f9f5a4f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='775' height='567'></svg>)





![](https://pic3.zhimg.com/v2-ad7171e9e7fa37a7b3a4789b02cd1e96_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='775' height='567'></svg>)





![](https://pic2.zhimg.com/v2-1137cfd3603939cdd4892fe962c7c2d1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='775' height='567'></svg>)





![](https://pic1.zhimg.com/v2-914c86ac29ae9a80a4aff9bf9e9cacb4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='775' height='567'></svg>)





![](https://pic3.zhimg.com/v2-b79fbb480d58b366c95b59bd9cdf77d6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='775' height='567'></svg>)






安装完，重启 Florence 后我们发现已经可以用域管理员的身份登录了，[http://adtest.com](https://link.zhihu.com/?target=http%3A//adtest.com) 域 已经被成功创建了。



![](https://pic4.zhimg.com/v2-2038bb0ef0f2377c184bf79bd948638b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1022' height='743'></svg>)






检查DNS服务器，我们发现DNS区域中已经自动创建了很多记录，这些记录的作用以后我们再来分析，现在大家只要注意检查一下创建域时有没有把这些记录创建出来，如果没有那就有问题了。




![](https://pic1.zhimg.com/v2-df99c8dd979abfe99171d55916ef3150_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='728'></svg>)






至此，我们完成了域控制器的创建，[http://adtest.com](https://link.zhihu.com/?target=http%3A//adtest.com) 域诞生了！


