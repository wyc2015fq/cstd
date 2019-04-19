# WAMP+Drupal 环境搭建痛苦回忆录！ - 三少GG - CSDN博客
2011年02月06日 22:37:00[三少GG](https://me.csdn.net/scut1135)阅读数：2015
**怀疑及意外收获**：
1.配置Apache httpd.conf时 不可直接拷贝，检查“”符号！！！例如 PHPIniDir “D:/php”未改导致Apache启动不了~~~
2.Mysql干净卸载。。例如：删除默认隐藏文件 C:/ProgramData/MySQL，注册表等
3.文件夹及配置文件的读写权限管理！没想到windows下也要类似linux下更改只读属性及管理员取得所有权（右键）
4.MySQL安装时出现“终结点映射器中没有更多的终结点可用”警告！
解决：这是因为在安装MySQL的某一步中勾选了“Add firewall exception for this port”，但是服务器的Windows的防火墙又没有开启，所以会有这个提示，一般情况下点击OK就好了，不会产生什么系统错误。
一般情况下，windows系统都是默认开启防火墙的，现在很多的简化系统都将防火墙默认关闭了，很多服务也不全了，所以，建议大家还是用原版的或者没有精简过的系统好些。。
5.Mysql 常用GUI： 1.mysql-gui-tools-5.0-r17-win32   2.phpMyAdmin-3.3.9-all-languages
6.搜安装方法时要加上win7的字样，操作系统会造成影响！！！！！！！！！
**原因! 罪魁祸首：**
**1. **
**localhost VS 127.0.0.1** 参考下面文章~ Win7下两者居然不可互换。。。自己辛苦从testdb中验证得到，参见[http://blog.csdn.net/scut1135/archive/2010/12/06/6057217.aspx](http://blog.csdn.net/scut1135/archive/2010/12/06/6057217.aspx) 其中“
$dbhost = '127.0.0.1';           //或者"localhost"
”如果未设置host则不可互换！ 否则前者可以，后者不可！ 配置Apache时需要填写localhost~
  因为大多PHP程序安装的默认数据库地址localhost，每次都要去修改成127.0.0.1觉得有点麻烦，既然知道了原理就好办了，我们可以通过修改HOSTS文件来达到我们的目的。首先用记事本打开HOSTS文件“C:/WINDOWS/system32/drivers/etc/hosts”，打开后如图所示在其中加上一条“127.0.0.1 localhost”并保存。如果你是WIN7系统，可以参考下[WIN7下修改HOSTS文件](http://www.xxzh.org/computer/Windows7-hosts.html)这篇文章。  **进行完上面的操作以后我们就可以直接用localhost连接mysql服务器了**。我不知道有没有人碰过这种问题，还是只有WIN7系统才有这种情况，反正我就是通过这种方法解决的。
2.成功打开页面！不再是pages not found
不确定原因。。。在纠结多日之后准备全部卸载重装Xampp，不料最后一次重启后居然成功。猜测原因是更改了php配置 可能性最大。
一个是确认了extension_dir="D:/BBS/PHP/ext";另一个是更改session.save_path= "D:/BBS/phpsession";(自己创建的，貌似没用到) 或者是拷贝了php5ts.dll到C:/windows C:/windows/system32 或者拷贝了php.ini ?
**后记：**1.耗时两天 大过年的实在。。。
2.安装过程等还是看英文原文件！尽量不看杂七杂八的中文翻译~~~安装包里面都有详细的安装过程！比如Drupal的Document...
