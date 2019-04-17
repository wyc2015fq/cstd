# linux 系统安装配置 zabbix服务(源码安装) - DoubleLi - 博客园






**简介：**



 zabbix是一个基于WEB界面的提供分布式系统监视以及网络监视功能的企业级的开源解决方案。

 zabbix能监视各种网络参数，保证服务器系统的安全运营；并提供灵活的通知机制以让系统管理员快速定位/解决存在的各种问题。

 zabbix由2部分构成，zabbix server与可选组件zabbix agent。zabbix server可以通过SNMP，zabbix agent，ping，端口监视等方法提供对远程服务器/网络状态的监视，数据收集等功能。









**标注：**



(zabbix必须依赖LAMP或LNMP平台运行，可以参考以下教程配置 LNMP平台,LNMP有安装顺序之分，一般按照LNMP名称顺序进行安装软件。)



**Linux+Apache+Mysql+PHP = LAMP**

**Linux+Nginx+Mysql+PHP = LNMP**



配置nginx参考此教程：

**[http://www.cnblogs.com/zoulongbin/p/6253568.html](http://www.cnblogs.com/zoulongbin/p/6253568.html)**



配置mysql请参考此教程：

**[http://www.cnblogs.com/zoulongbin/p/6287167.html](http://www.cnblogs.com/zoulongbin/p/6287167.html)**



配置php请参考此教程：

**[http://www.cnblogs.com/zoulongbin/p/6379272.html](http://www.cnblogs.com/zoulongbin/p/6379272.html)**



配置网络yum源参考此教程：

**[http://www.cnblogs.com/zoulongbin/p/5773330.html](http://www.cnblogs.com/zoulongbin/p/5773330.html)**



Zabbix安装包官网下载地址：

**[http://www.zabbix.com/download](http://www.zabbix.com/download)**







**提示：**Linux软件编译安装都需要依赖两个安装包  **gcc   gcc-c++  make    可使用 yum -y  install  **gcc   gcc-c++ make  在线安装。****





**标注：**zabbix安装包存放路径在 /usr/local/src 目录下，请先自行关闭linux服务器防火墙  /etc/init.d/iptables stop







1、  mysql创建zabbix数据库和用户。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208193919166-1445389133.png)









2、把zabbix安装包里面的数据库导入msqyl创建的zabiix数据库。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208193936682-1331297354.png)









3、添加zabbix启动加载mysql共享库libmysqlclient.so.18。(这步很重要，如果没配置zabbix启动会报错)



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208195131635-1162357498.png)











4、创建zabbix用户和用户组，并把zabbix用户添加到zabbix组里，在线yum安装net-snmp-devel依赖包。



[root@ssticentos65 ~]# **groupadd zabbix　**　　　　　　　　　　　　　　　　　　　#创建zabbix用户组



[root@ssticentos65 ~]# **useradd zabbix -g zabbix -s  /sbin/nologin**                  #创建zabbix用户，并把zabbix用户拉进zabbix用户组，禁止zabbix用户登录系统                



[root@ssticentos65 ~]# **yum -y install net-snmp-devel**                                   #在线yum安装net-snmp-devel依赖包











5、编译安装zabbix服务。



./configure --prefix=/usr/local/zabbix --enable-server --enable-agent --with-net-snmp --with-libcurl --enable-proxy --with-mysql=/usr/local/mysql/bin/mysql_config

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208194028963-1999488382.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208194043416-688087869.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208194109838-2125966594.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208194115182-1588096423.png)











6、添加zabbix服务对应的端口。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208194152510-2036066003.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208194158244-736020554.png)











7、修改zabbix配置文件。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208194215932-1977343567.png)



DBHost=127.0.0.1              #服务器主机

DBName=zabbix               #数据库名称

DBUser=zabbix                #数据库用户名

DBPassword=123456      #数据库密码

#ListenIP=0.0.0.0            #监听地址，此选项默认注释，默认0.0.0.0可以监听任何IP地址，不要设置成127.0.0.1，否则会导致10051端口异常

AlertScriptsPath=/usr/local/zabbix/share/zabbix/alertscripts #zabbix      #运行脚本存放目录

:wq!                     #保存退出



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208194232276-882657691.png)



#ListenIP=0.0.0.0            #监听地址，此选项默认注释，默认0.0.0.0可以监听任何IP地址，不要设置成127.0.0.1，否则会导致10051端口异常



Hostname=VMredhat6.5   #填写zabbix服务器计算机名称



Include=/usr/local/zabbix/etc/zabbix_agentd.conf.d/



UnsafeUserParameters=1                      #启用自定义key



:wq!                                      #保存退出









8、添加zabbix开机启动脚本。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208192642322-288683794.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208192604447-1926417767.png)









9、配置zabbix服务web站点。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208192534244-1005134249.png)









10、修改php配置文件参数。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208192738729-671760623.png)



post_max_size =16M

max_execution_time =300

max_input_time =300

:wq!                     #保存退出



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170209215612166-893376135.png)



request_terminate_timeout = 300

:wq!                    #保存退出





![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208192903026-1207316301.png)











11、在zabbix服务器打开浏览器输入 http://127.0.0.1/zabbix/setup.php安装web



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095200588-397437004.png)







12、如果执行下一步提示如下错误。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095238322-1329938099.png)







**解决办法：**



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095316197-2074484845.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095337260-208057749.png)









13、显示全部OK才能执行下一步操作。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095407197-1623657349.png)









14、按照下图配置。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095425588-438876168.png)









15、一直点击下一步直到完成。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095452807-42701703.png)



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095442557-1484207379.png)





![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095516401-1761400617.png)



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095530197-732652370.png)





![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095542369-2128950691.png)













**标注：**



打开zabbix网页部分地方如果会显示乱码，如下图所示



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210095707822-1033169235.png)





**解决办法：**



造成这个问题主要因为zabbix的web端中文库不完整，需要把中文字库加上即可。



(1)、windows操作系统 ----  控制面板 ---- 字体 ---- 自行选择一种字体复制出来再上传到zabbix服务器上。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208193440338-1790402318.png)







(2)、把它拷贝到zabbix的web端的fonts目录下 /usr/local/nginx/html/zabbix/fonts/，并且把TTF后缀改为ttf，修改zabbix的web端/include/defines.inc.php。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170208193457838-1737966021.png)





define('ZBX_FONT_NAME', 'DejaVuSans');

把原来的字库名修改成上传的中文字库名 simhei

define('ZBX_FONT_NAME', 'simhei');



define('ZBX_GRAPH_FONT_NAME',   'DejaVuSans');

把原来的字库名修改成上传的中文字库名 simhei

define('ZBX_GRAPH_FONT_NAME',   'simhei');

















** 其它功能操作：zabbix网页修改中文版及黑色主题。**



1、点击个人信息维护。

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210120403119-1282403835.png)



2、语言修改成中文，主题修改成黑色主题。

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210120411197-511309452.png)



3、效果。

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170210120420979-1829797351.png)

















**感谢此博客提供参考    http://www.osyunwei.com/archives/7891.html**









