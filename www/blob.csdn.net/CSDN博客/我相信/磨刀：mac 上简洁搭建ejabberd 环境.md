# 磨刀：mac 上简洁搭建ejabberd 环境 - 我相信...... - CSDN博客





2015年04月18日 16:36:42[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2437








和app-get，或者yum类似， 这里使用的是mac上的软件管理工具port。

先磨刀，安装port的方法呢？ 访问如下网站：

[https://www.macports.org/install.php](https://www.macports.org/install.php)


选择自己的mac 软件版本，下载即可安装port了。




port安装成功后，继续磨刀，ejabberd 是erlang编写的，再装一个erlang的运行环境：

$sudo port install erlang

静等结果，安装完成后，检验一下

$erl




Erlang/OTP 17 [erts-6.4] [source] [64-bit] [smp:8:8] [async-threads:10] [hipe] [kernel-poll:false]

Eshell V6.4  (abort with ^G)

1> 





erlang安装成功后，就可以安装ejabberd了：

$sudo port install ejabberd


安装完成后，第一次启动前做如下操作：



$cd /opt/local/etc/ejabberd/

$cp ejabberd.yml.sample ejabberd.yml

$cp ejabberdctl.cfg.sample ejabberdctl.cfg

$cp inetrc.sample inetrc




至此，可以启动了：


$ejabberdctl start




接下来就是按需修改配置，开始实战了。












