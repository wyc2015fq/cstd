# wampserver环境下配置phpmyadmin远程访问mysql数据库 - sxf_123456的博客 - CSDN博客
2017年11月24日 21:13:32[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：515标签：[php																[phpmyadmin																[mysql																[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=phpmyadmin&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[php](https://blog.csdn.net/sxf_123456/article/category/6883770)
## 前言
       暑假找了一个PHP软件工程师实习，实习期间由于本地服务器需要获取公司服务器上的数据库里的数据，需要远程访问公司服务器上的mysql数据库，我用的是wampserver，在网上找了一些方法都不可行，于是摸索了一下wampserver环境下配置phpmyadmin远程访问mysql数据库的方法，在这里分享一下，亲测可行。第一次写博客，望轻喷！说明一下，我本地服务器为192.168.0.127，访问地址为http://localhost/phpmyadmin/，公司服务器为192.168.0.2，假设公司服务器上的mysql数据库的用户名为username，密码为admin，也就是现在要通过http://localhost/phpmyadmin/访问192.168.0.2上的mysql数据库，具体步骤如下：
## 1.（可选）备份\wamp\apps\phpmyadmin文件夹下的config.inc.php文件和\wamp\apps\phpmyadmin\libraries文件夹下的config.default.php文件
      下面会对config.inc.php和config.default.php进行修改，因此修改之前先对这两个文件进行备份，当不再需要远程连接mysql数据库时，用备份文件替换相应文件重启wampserver服务即可还原。
## 2.修改config.inc.php文件
需要修改的内容如下：
$cfg['Servers'][$i]['auth_type'] = 'http';           //将cookie改为http
$cfg['Servers'][$i]['host'] = '192.168.0.2';             //将localhost改为所要访问的服务器，这里为192.168.0.2
$cfg['Servers'][$i]['user'] = 'username';
                //将原用户名改为所要访问服务器上的数据库的用户名
$cfg['Servers'][$i]['password'] = 'admin';              //将用户名改为对应的密码
## 3.修改config.default.php文件
需要修改的内容如下：
$cfg['PmaAbsoluteUri'] = 'http://localhost/phpmyadmin/';           //设置为访问本地mysql数据库的地址
$cfg['Servers'][$i]['host'] = '192.168.0.2';                                   //将localhost改为所要访问的服务器，这里为192.168.0.2
$cfg['Servers'][$i]['auth_type'] = 'http';                                  //将cookie改为http
$cfg['Servers'][$i]['user'] = 'username';                                                       //将原用户名改为所要访问服务器上的数据库的用户名
$cfg['Servers'][$i]['password'] = 'admin';                                                    //将密码改为对应的密码
## 4.重启wampserver服务，远程访问mysql数据库
      以上步骤完成之后，重启wampserver服务，点击WAMPSERVER的phpMyAdmin或在浏览器中输入http://localhost/phpmyadmin/，会弹出一个用户验证框，输入所要访问服务器上的数据库的用户名和密码即可进入。
## 5.注意！
a)一定要同时修改config.inc.php和config.default.php文件，尤其是较老版本的wampserver，新版本wampserver的config.inc.php文件中没有$cfg['Servers'][$i]['user']和$cfg['Servers'][$i]['password']，可以忽略；
b)必须要将$cfg['Servers'][$i]['auth_type']的值由cookie改为http，通过验证用户信息的方式远程访问mysql数据库（进入http://localhost/phpmyadmin/，会弹出一个用户验证框），否则在浏览器中输入http://localhost/phpmyadmin/之后会直接访问本地mysql数据库！
c)可将http://localhost/phpmyadmin/改为http://192.168.0.127/phpmyadmin/，即本地服务器（我这里是192.168.0.127）。
转载：http://blog.csdn.net/u010746651/article/details/47798333
