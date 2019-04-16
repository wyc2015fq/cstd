# [CentOS Python系列] 五.阿里云部署web环境及通过IP地址访问服务器网页 - 杨秀璋的专栏 - CSDN博客





2018年02月24日 14:47:46[Eastmount](https://me.csdn.net/Eastmount)阅读数：1263
所属专栏：[CentOS云服务端之Python爬虫及数据库知识](https://blog.csdn.net/column/details/19699.html)









假设我们服务器CentOS系统已经部署好了，现在我们需要向服务器上传一个HTML主页，通过IP地址展示我们的内容，如何实现呢？本篇文章主要介绍讲述部署阿里云服务器web环境，并通过IP地址访问网页的过程。该系列文章前面都是介绍配置过程，后面还会介绍LAMP、DJango等环境配置过程，接着猜讲述Python工程的应用。基础性文章，希望对您有所帮助，同时文章中存在错误或不足之处，还请海涵~

写代码过程中往往第一步需要解决的就是配置开发环境，对于新手来说，这是非常头疼的事情，而当配置好之后或者对于老手来说，我们才能去实现自己理想的功能。同时推荐前面作者另外三个Python系列文章。




从2014年开始，作者主要写了三个Python系列文章，分别是基础知识、网络爬虫和数据分析。
- Python基础知识系列：[Pythonj基础知识学习与提升](http://blog.csdn.net/column/details/eastmount-python.html)
- Python网络爬虫系列：[Python爬虫之Selenium+Phantomjs+CasperJS](http://blog.csdn.net/column/details/eastmount-spider.html)
- Python数据分析系列：[知识图谱、web数据挖掘及NLP](http://blog.csdn.net/column/details/eastmount-kgdmnlp.html)

![这里写图片描述](https://img-blog.csdn.net/20180212160809122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161506304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161524640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

系列文章：
[[CentOS Python系列] 一.阿里云服务器安装部署及第一个Python爬虫代码实现](http://blog.csdn.net/Eastmount/article/details/79321822)
[[CentOS Python系列] 二.pscp上传下载服务器文件及phantomjs安装详解](http://blog.csdn.net/eastmount/article/details/79330430)
[[CentOS Python系列] 三.阿里云MySQL数据库开启配置及SQL语句基础知识](http://blog.csdn.net/eastmount/article/details/79334052)
[[CentOS Python系列] 四.阿里云服务器CentOS连接远程MySQL数据库及pymsql](http://blog.csdn.net/eastmount/article/details/79341409)







## 一. 安装及开启apache服务器




**1.安装httpd**

命令：yum -y install httpd



![](https://img-blog.csdn.net/20180224135016974)








**2.设置开机自动启动**

命令：chkconfig httpd on
注意，CentOS7使用命令为“systemctl enable httpd”，而CentOS6命令为“chkconfig httpd on”，更多区别推荐 "[CentOS 7 vs CentOS 6的不同](http://blog.csdn.net/zhihui1017/article/details/54890049)"。



**3.启动apache服务器**

命令：service httpd start

注意，CentOS7使用命令为“systemctl start httpd”，而CentOS6命令为“service httpd start”。


![](https://img-blog.csdn.net/20180224135442730)









**4.创建html文件**

去到/var/www/html目录下创建html文件，命令如下：
```
cd /var
ls
cd www
ls
cd html
ls
touch 20180223.html
ls
vim 20180223.html
输入 i (编辑代码insert)
输入HTML代码
保存输入 :wq! 退出
```


![](https://img-blog.csdn.net/20180224135640492)





接着输入HTML代码如下所示：

```
<html>
<head>
      <title>Putty HTML</title>
</head>
<body>
      <div align="center">
          <h1>杨秀璋个人简历</h1>
          <hr width="80%" color="red" size="2">
          <ul>
              <li>高中：贵州师大附中</li>
              <li>本科：北京理工大学-软件学院-软件工程</li>
              <li>硕士：北京理工大学-软件学院-web数据挖掘</li>
          </ul>
      </div>
</body>
</html>
```
如下图所示：




![](https://img-blog.csdn.net/20180224135753784)











## 二. iptables增加80端口号




接下来在CentOS服务器端开通HTTP端口号80，步骤如下：

1.vi /etc/sysconfig/iptables
2.加入如下代码，核心: -I INPUT -p tcp -m state --state NEW -m tcp --dport 80-j ACCEPT



```python
#Firewall configuration written by system-config-firewall
#Manual customization of this file is not recommanded.
*filter
:INPUT ACCEPT [0:0]
:FORWARD ACCEPT [0:0]
:OUTPUT ACCEPT [0:0]
-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
-A INPUT -p icmp -j ACCEPT
-A INPUT -i lo -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 3306 -j ACCEPT
-A INPUT -j REJECT --reject-with icmp-host-prohibited
-A FORWARD -j REJECT --reject-with icmp-host-prohibitedCOMMIT
```



![](https://img-blog.csdn.net/20180224140047771)

3.保存退出


4.重启防火墙：service iptables restart


![](https://img-blog.csdn.net/20180224140113481)




```
[root@iZ2ze9134z8zlqupc9t6mzZ ~]# service iptables restart
iptables: Setting chains to policy ACCEPT: filter          [  OK  ]
iptables: Flushing firewall rules:                         [  OK  ]
iptables: Unloading modules:                               [  OK  ]
iptables: Applying firewall rules:                         [  OK  ]
```

注意，开放的端口一定要添加到 REJECT 之前。同时，有的教程会让你临时关闭防火墙(如果不关闭，则只能本机访问apache服务)，service iptables stop，但作者更推荐重启防火墙即可。










## 三. 阿里云安全组开启80端口号




如果您还是无法访问如下图所示，则需要设置阿里云的安全组规则，而网上更多的资料忽略了该步骤。下面进行简单叙述：


![](https://img-blog.csdn.net/20180224141152388)





**第一步：打开阿里云服务器管理控制台页面，点击“控制台”按钮**



![](https://img-blog.csdn.net/20180224140458724)







**第二步：在弹出的页面中，选择“云服务器ECS”，然后点击我们运行中的服务器**

![](https://img-blog.csdn.net/20180224140540361)







**第三步：在弹出如下界面中点击“更多”，选择低端的“安全组配置”**

![](https://img-blog.csdn.net/20180224140622188)






**第四步：在“本实例安全组”界面选择“配置跪着”。**

![](https://img-blog.csdn.net/20180224140707567)







**第五步：点击“添加安全组规则”。**

![](https://img-blog.csdn.net/20180224140752860)





**第六步：在“添加安全组规则”中选择HTTP（80）协议，端口号为80端口，授权对象为“0.0.0.0/0”，表示允许任意公网IP登录。**


![](https://img-blog.csdn.net/20180224140854006)

设置成功如下图所示：




![](https://img-blog.csdn.net/20180224141013209)





**第七步：输入网址“39.107.105.166/20180223.html”,可以查看我们的网页如下图所示：**

![](https://img-blog.csdn.net/20180224141117125)



如果没有该网页也是提示无资源，而不是前面那样无法访问，就这样我们的阿里云web环境配置成功，如果在购买一个域名即可访问个人网站，比如：[http://www.eastmountyxz.com](http://www.eastmountyxz.com) 。




总之，希望这篇基础文章对您有所帮助，尤其是刚接触云服务器的新手，如果您是高手，还请多提意见，共同提高。祝大家新年快乐，又一年过去了，娜我们来年一起进步加油。 
( By：Eastmount CSDN 2018-02-24 中午2点[http://blog.csdn.net/Eastmount](http://blog.csdn.net/Eastmount))











