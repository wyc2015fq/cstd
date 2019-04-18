# Windows Server 2012 从入门到精通系列 之 DHCP服务器 - 知乎
# 



DHCP（Dynamic Hoat Configure Protocol）动态主机配置协议简称,首先我们先了解DHCP服务器的作用所在。当企业计算机数量较多时，例如BENET公司中有300台计算机，如果要使用静态IP地址，那么网络管理员的工作量可见而知，那么如何解决此类问题呢？那么就需要一台能够自动给客户机分配IP地址的服务器,这台服务器就是DHCP服务器啦,有人会认为DHCP只会为客户机分配IP地址和子网掩码,那你大错特错了,DHCP的作用是为网络中计算机自动分配TCP/IP参数的协议。






![](https://pic4.zhimg.com/v2-7c7c01feb715515c54d6fd5b71828adb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='387' height='241'></svg>)






DHCP的特点：安全、可靠、简单。




**使用DHCP的好处：**
- 减少管理员的工作量
- 避免输入错误的可能
- 避免IP冲突
- 提高了IP地址的利用率
- 方便客户端的配置


**DHCP的分配方式：**

自动分配

分配到一个IP地址后永久使用

手动分配

由DHCP服务器管理员专门指定IP地址

动态分配




使用完后释放该IP，供其它客户机使用




当了解了DHCP的作用后,我们再了解一下DHCP服务器和客户机的工作原理:

**客户机想DHCP申请IP的步骤分为4个环节:**
![](https://pic3.zhimg.com/v2-66876c773062dc9763d088e877dd4b32_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='617' height='165'></svg>)






1.客户机想DHCP请求IP地址(DHCPCISCOVER)






![](https://pic4.zhimg.com/v2-8e2df3f9903d9f97780873b9ec7c1eaf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='721' height='196'></svg>)






2.服务器响应(DHCPOFFER)






![](https://pic4.zhimg.com/v2-a7df449ac8411943cb08bdc22997e99b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='706' height='220'></svg>)






3.客户机选择IP地址(DHCPREQUEST)






![](https://pic3.zhimg.com/v2-24e5700a553b4029c7f79a5ee8f1fc2a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='705' height='248'></svg>)






4.服务器确定租约(DHCPPACK)






![](https://pic2.zhimg.com/v2-29f122d204a9a66673603b1074b17f99_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='707' height='196'></svg>)






但要记住这4个步骤不管是客户机还是服务器它们之间的通信都是以广播的形式来进行的。







**接下来我们再来了解IP租约更新:**客户机从DHCP申请的IP地址是有租约限制的,默认是8天到期,到期后要续租的,那么这两者之间如何进行IP的续租工作呢?1.当DHCP客户机租期达50％时,重新更新租约，客户机发送DHCPRequest包 

2.如果当DHCP客户机租期达50％时,没有联系上DHCP时,那么要等到当租约达到87.5%时,进入重新申请状态，客户机发送DHCPDiscover包3.如果当租约达到87.5%时,还没有联系上DHCP,那么就会到期满后重新进行DHCP的申请过程.

4.客户端也可以使用ipconfig /release命令,客户机的TCP/IP通信联络停止,IP地址:0.0.0.0

子网掩码:0.0.0.0.服务器可以将释放的IP分配给其他客户机.然后使用ipconfig /renew命令向DHCP服务器发送DHCPRequest包,如果DHCP服务器没有响应，客户机将继续使用当前的配置.**客户端重启**发送带有IP地址的DHCP Request请求包
![](https://pic4.zhimg.com/v2-95e1f091c9188b63b83dc5933ad9a2b7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='707' height='157'></svg>)
IP地址没有分配使用，发送DHCP ACK确认信息客户端继续使用重启前的IP地址
![](https://pic4.zhimg.com/v2-223c05655e6a06221dadc9ce149ad12f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='709' height='159'></svg>)
IP地址分配到其他客户机使用发送DHCP Nack否认信息
![](https://pic4.zhimg.com/v2-85656662114315c8da7a83688fd29f17_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='710' height='173'></svg>)
客户机从新发送DHCP Discover
![](https://pic1.zhimg.com/v2-580364ceccf828296c4d060e3f27a6e8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='707' height='181'></svg>)
**DHCP****租约的生成过程**

1.         DHCP客户机发送完DHCPDiscover消息后,会等待DHCP Offer消息,如果未能接收到DHCPOffer,它就会重试4次（相隔2,4,8,16s,加上一个0到1000ms之间的随机时间数）

2.         如果DHCP客户机经过努力仍未获得任何有效服务器的IP地址,将使用169.254.0.1～169.254.255.254这一保留地址中的一个IP地址（这也是帮助我们排错的一个根据）




3.         每隔5分钟，该客户机都继续尝试发现一个DHCP服务器，如果有某个DHCP服务器成为可用，客户机将接受到合法的IP地址

**IP租约释放**

ipconfig /release命令进行释放动态获得的IP地址

ipconfig /renew命令进行更新动态获得的IP地址

客户机的TCP/IP通信联络停止

IP地址：0.0.0.0

子网掩码：0.0.0.0

目标地址：255.255.255.255




服务器可以将释放的IP分配给其它的计算机




**那么我们现在就开始配置DHCP服务器:**

1.DHCP服务器的要求:




a)静态IP地址，子网掩码和其他的TCP/IP参数(192.168.6.1) 



![](https://pic1.zhimg.com/v2-2cfb873929b821c7267d9130a537c574_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='478' height='524'></svg>)



2.开始安装配置DHCP服务器：

a) 打开服务器管理器，添加角色



![](https://pic3.zhimg.com/v2-cc7bc3fa7069c7f5be1b6bf694f0fd2e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



b) 添加DHCP服务器

1.选择基于角色和功能的安装



![](https://pic3.zhimg.com/v2-2ee12e1fda20cb77855156cfc1bac182_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



2.选择服务器



![](https://pic4.zhimg.com/v2-52e0b8addfa4dcffa0d9e25e75a9d5cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



3.选择DHCP服务器角色
![](https://pic2.zhimg.com/v2-cec048c13117a700167e53a0836b4f49_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)





![](https://pic1.zhimg.com/v2-1c9c4b1d4f1d937b9a1ddf720ab12b94_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)





![](https://pic2.zhimg.com/v2-e366680d88f1fd4e3ea8a17319842209_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)





![](https://pic2.zhimg.com/v2-eec7ef70b4b27f15ff2559792af2cde9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



3）配置管理DHCP服务器

打开DHCP服务器控制台：



![](https://pic2.zhimg.com/v2-414f4136fdb3579bd5877236fc924d69_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



**授权DHCP服务器:**

授权（AD中才有）是一种安全的预防措施,避免未经授权的DHCP服务器在网络中运行 



![](https://pic4.zhimg.com/v2-653653ab4606c0a7f4cf9d30a4b434f7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



**配置作用域:**




a)作用域是一段IP地址的范围
b)作用域必须在租给客户机IP地址之前创建
c)每个DHCP服务器中至少应有一个作用域 ，为一个网段分配IP地址
d)为多个网段分配IP地址，就需要创建多个作用域 






![](https://pic1.zhimg.com/v2-070ad2327a6fb6f7f37fe04584ff43a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='695' height='225'></svg>)






**新建作用域/激活作用域**

²  在DHCP控制台新建作用域



![](https://pic2.zhimg.com/v2-d064b596754772bc36edd0605f0221a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



























²  输入作用域名称



![](https://pic3.zhimg.com/v2-b5660d50318c6bc677e86f23da17b972_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



²  输入IP地址范围



![](https://pic2.zhimg.com/v2-7af1010190464fe66086f251d22eef4d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



²  添加排除



![](https://pic2.zhimg.com/v2-000132c09b4aa545a0ad155ea06c8e81_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



²  租用期限



![](https://pic4.zhimg.com/v2-9eaff5bdd0743a217f9f68c8964af523_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



²  配置DHCP选项



![](https://pic4.zhimg.com/v2-6287fa0d9e184efb223ad1062ee8bb33_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



²  完成新建作用域



![](https://pic3.zhimg.com/v2-12ef325adf105ed03a4802c3dbf6ff9a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



²  激活作用域



![](https://pic3.zhimg.com/v2-c06e0aa4cd298f89c436b8184f9882a2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)





![](https://pic3.zhimg.com/v2-22f3800b76f6065e286d1711deb5182e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



²  查看DHCP服务器地址池和地址租约选项



![](https://pic3.zhimg.com/v2-a5c18e25b2293288244d7826fea314c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)





![](https://pic4.zhimg.com/v2-e467b4479d499f30061b56b8bf6f8953_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



**配置客户机：**

打开一台windows7客户机，网卡设置为自动获得，查看是否能从DHCP服务器地址池中获得IP信息，并从DHCP服务器的地址租约选项中查看变化。



![](https://pic3.zhimg.com/v2-67f2de9daad9b2da8b4926822da8f9ae_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='411' height='429'></svg>)





![](https://pic2.zhimg.com/v2-e9334fa35fccd5404529ce4cf5db57ad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)





![](https://pic3.zhimg.com/v2-d208b09185ed8ce1248fcc219cb3d37e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)






**配置服务器选项:**

服务器选项

在该服务器上所有作用域中生效

作用域选项

在该作用域中生效

保留选项

对保留的客户机生效

优先级

服务器选项→作用域选项→保留选项

服务器选项:提供客户机一些可选项,例如:默认网关的IP地址,DNS服务器的IP地址,对该服务器所有的作用域的客户机生效。
例如:给所有客户机配置了DNS地址202.106.0.20



![](https://pic2.zhimg.com/v2-6097b08fa65019123ef97418aa4f5951_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)





![](https://pic4.zhimg.com/v2-a56a7b8767875dc50e5cba4606945c8f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



作用域选项:提供客户机一些可选项,例如:默认网关的IP地址,DNS服务器的IP地址,但只对本作用域的客户机有效。例如:给本网段配置了默认网关地址192.168.6.1。



![](https://pic1.zhimg.com/v2-f71feb2edf7f250e1b777e27655ff734_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)





![](https://pic1.zhimg.com/v2-534a305588d1e02b606570ddf4fcd644_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



配置客户端保留:

客户端保留可以确保特定的客户机或者指定的服务器总是获得同一IP地址,新建保留-保留选项-仅对本台客户机生效(需要IP和MAC的绑定) 



![](https://pic3.zhimg.com/v2-ba9dd163f66b0c2d4dbe60e8394c718e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)





![](https://pic4.zhimg.com/v2-5c23758536897d25cf6a1e31d57ebdef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)






DHCP的类级别



![](https://pic3.zhimg.com/v2-4904d3759ce6596bdbb57b5c4f7b2bd6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='606' height='416'></svg>)



希望在所有的作用域中的操作系统是WIN7的客户端和WINXP的客户端从DHCP得到的配置不一样，如何去实现？

1. 在所有WINXP客户端去创建类别



![](https://pic4.zhimg.com/v2-e7cbdeecf023d68bb62d971d4475217f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='436' height='254'></svg>)





![](https://pic2.zhimg.com/v2-d2398b4ce024b9b5127aa4d0bfc1ed05_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='674' height='485'></svg>)



2. 然后在DHCP服务器对该类进行



![](https://pic1.zhimg.com/v2-70ee4916c5a95055d8f16e9fd058ae3c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='293' height='438'></svg>)





![](https://pic2.zhimg.com/v2-3ac7afdd00c65c2479b11556229e50a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='553' height='312'></svg>)





![](https://pic3.zhimg.com/v2-cc79de31ff7954e951bb8f6593e05f7e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='438' height='448'></svg>)





![](https://pic3.zhimg.com/v2-c2eab5e14c24c7a364fbcb9396f72e12_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='552' height='310'></svg>)





![](https://pic4.zhimg.com/v2-2c6222ae6d92b7f9bff1180aa10dd993_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='300' height='325'></svg>)





![](https://pic2.zhimg.com/v2-9dbb66dab306542f07d71fd70d9ecacd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='404' height='445'></svg>)



维护DHCP服务器:

在做完所有配置,为了防止DHCP服务器由于硬盘发生错误而导致数据库丢失的情况发生，需要备份DHCP数据库，备份可分为联机备份和脱机备份。

DHCP服务器的数据库默认情况下就会自动去做备份,当目前使用的数据库有一些问题,它会自动恢复数据库，但此操作仅限于服务器系统是正常运行的状态，也就是联机备份，联机备份文件存放到c:\windows\system32\dhcp\backup目录下，默认24小时系统自动进行一次联机备份，也可以使用手动进行备份。



![](https://pic3.zhimg.com/v2-6a2ded4bd5713ca3f75c35fb499585aa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='662' height='291'></svg>)



备份和还原DHCP数据库

案例:将一台DHCP服务器（源服务器）的数据库移动到另一台DHCP服务器（目标服务器）上.

步骤:

1.在源服务器上备份DHCP数据库

2.停止DHCP服务器（这将防止该服务器在备份了数据库后向客户端指派新的地址租约）

3.禁用DHCP服务（防止DHCP服务器在数据库转移之后启动）

4.将备份文件夹从源服务器拷贝到目标服务器

5.在目标服务器上还原DHCP数据库

删除DHCP组件



![](https://pic4.zhimg.com/v2-e3574a0070af03147ccee002e20c14c7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1276' height='622'></svg>)



注意:

如果有时连整个DHCP服务器都损坏了，此还原就不可用了，那么如何解决？

要使用手动对DHCP数据库进行脱机备份，使用NTBACKUP工具对c:\windows\system32\dhcp\backup文件夹对备份即可。

安装DHCP服务后会出现两个关于DHCP的组

DHCP Users group:具有查看DHCP权限

DHCP Administrators group:具有管理DHCP权限

具有授权DHCP服务器的权力给有Enterprise Admins（企业管理员组）成员才能实现


