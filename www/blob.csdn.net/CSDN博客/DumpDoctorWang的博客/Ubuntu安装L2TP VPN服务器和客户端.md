# Ubuntu安装L2TP VPN服务器和客户端 - DumpDoctorWang的博客 - CSDN博客





2018年07月17日 22:00:08[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：5561








# 一、服务器端

一键安装l2tp，参考网址 [https://teddysun.com/448.html](https://teddysun.com/448.html)

```bash
wget --no-check-certificate https://raw.githubusercontent.com/teddysun/across/master/l2tp.sh
 chmod +x l2tp.sh  
./l2tp.sh
```

安装的时候IP-Range保持默认，PSK为预共享秘钥(Pre-shared Key)，用户名和密码都要设置并记住，相当于一个账号。服务器IP(Server IP)也要记住。安装过程如下图所示

![](https://img-blog.csdn.net/20180718093003630?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后再次验证ipsec（L2TP）并重启相关服务

```bash
service ipsec restart  
service xl2tpd restart  
ipsec verify
```

如果你要想对用户进行操作，可以使用如下命令(建议增加几个账户，因为一个账户只能一个设备用)：

```bash
l2tp -a 新增用户
l2tp -d 删除用户
l2tp -m 修改现有的用户的密码
l2tp -l 列出所有用户名和密码
l2tp -h 列出帮助信息
```

# 二、客户端

安装network-manager-l2tp network-manager-l2tp-gnome

```bash
sudo add-apt-repository ppa:nm-l2tp/network-manager-l2tp
sudo apt-get update
sudo apt-get install network-manager-l2tp network-manager-l2tp-gnome
```

然后打开系统设置(Settings)--->网络(Network)---->点击左下角的“+”号，出现下图

点击创建(Create)

![](https://img-blog.csdn.net/20180717214438435?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击创建(Create) 

![](https://img-blog.csdn.net/20180717214544465?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

输入你的服务器的IP、用户名、密码。点下密码框旁边的小人，选第一个，就可以输入密码

![](https://img-blog.csdn.net/20180717214746231?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击IPsec设置(IPsec Settings)，勾选“Enable IPsec tunnel to L2TP host” ，在预共享秘钥(Pre-shared key)那个框里面输入前面的预共享秘钥)

![](https://img-blog.csdn.net/2018071721482240?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后点击确定。此时网络框中已经有VPN的连接了，在左边那一栏点击“VPN Connection 1 VPN”，然后右边就会出现一个开关，打开一会儿之后，会提示VPN已经连接上，这时候就可以愉快的上网了。要关闭VPN时，在点一下右边的开关就行。

![](https://img-blog.csdn.net/20180717215331382?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 三、Windows和Android客户端

Windows10和Android手机都自带VPN客户端，可自行查找配置博客。VPN类型选择带有“L2TP/IPsec PSK”、“使用预共享密钥的L2TP/IPsec”.。然后在网关(服务器地址)输入之前的服务器IP，再输入用户名密码，点击连接，信息没问题的话等一会儿就连上了。

# 四、参考资料

在第二个链接中还讲了怎么配置ShadowSocksR，这种VPN的客户端也非常齐全。

[https://launchpad.net/~nm-l2tp/+archive/ubuntu/network-manager-l2tp?field.series_filter=xenial](https://launchpad.net/~nm-l2tp/+archive/ubuntu/network-manager-l2tp?field.series_filter=xenial)

[https://blog.csdn.net/rentian1/article/details/78864523](https://blog.csdn.net/rentian1/article/details/78864523)

[https://teddysun.com/448.html](https://teddysun.com/448.html)



