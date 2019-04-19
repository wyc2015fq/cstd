# Linux下RPM软件包的安装及卸载 - xqhrs232的专栏 - CSDN博客
2016年06月08日 17:09:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：234
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://os.51cto.com/art/201001/177866.htm](http://os.51cto.com/art/201001/177866.htm)
相关文章
1、深入讲解RPM包安装/升级/查询/卸载----[http://os.51cto.com/art/201001/176040.htm](http://os.51cto.com/art/201001/176040.htm)
2、linux下制作安装rpm包----[http://os.51cto.com/art/201001/177075.htm](http://os.51cto.com/art/201001/177075.htm)
3、、阐述Linux操作系统之rpm五种基本操作----[http://os.51cto.com/art/200912/166353.htm](http://os.51cto.com/art/200912/166353.htm)
在 Linux 操作系统下，几乎所有的软件均通过RPM 进行安装、卸载及管理等操作。RPM 的全称为Redhat Package Manager ，是由Redhat 公司提出的，用于管理Linux 下软件包的软件。Linux 安装时，除了几个核心模块以外，其余几乎所有的模块均通过RPM 完成安装。RPM 有五种操作模式，分别为：安装、卸载、升级、查询和验证。
RPM 安装操作
命令：
rpm -i 需要安装的包文件名
举例如下：
rpm -i example.rpm 安装 example.rpm 包；
rpm -iv example.rpm 安装 example.rpm 包并在安装过程中显示正在安装的文件信息；
rpm -ivh example.rpm 安装 example.rpm 包并在安装过程中显示正在安装的文件信息及安装进度；
RPM 查询操作
命令：
rpm -q …
附加查询命令：
a 查询所有已经安装的包以下两个附加命令用于查询安装包的信息；
i 显示安装包的信息；
l 显示安装包中的所有文件被安装到哪些目录下；
s 显示安装版中的所有文件状态及被安装到哪些目录下；以下两个附加命令用于指定需要查询的是安装包还是已安装后的文件；
p 查询的是安装包的信息；
f 查询的是已安装的某文件信息；
举例如下：
rpm -qa | grep tomcat4 查看 tomcat4 是否被安装；
rpm -qip example.rpm 查看 example.rpm 安装包的信息；
rpm -qif /bin/df 查看/bin/df 文件所在安装包的信息；
rpm -qlf /bin/df 查看/bin/df 文件所在安装包中的各个文件分别被安装到哪个目录下；
RPM 卸载操作
命令：
rpm -e 需要卸载的安装包
在卸载之前，通常需要使用rpm -q …命令查出需要卸载的安装包名称。
举例如下：
rpm -e tomcat4 卸载 tomcat4 软件包
RPM 升级操作
命令：
rpm -U 需要升级的包
举例如下：
rpm -Uvh example.rpm 升级 example.rpm 软件包
RPM 验证操作
命令：
rpm -V 需要验证的包
举例如下：
rpm -Vf /etc/tomcat4/tomcat4.conf
输出信息类似如下：
S.5....T c /etc/tomcat4/tomcat4.conf
其中，S 表示文件大小修改过，T 表示文件日期修改过。限于篇幅，更多的验证信息请您参考rpm 帮助文件：man rpm
RPM 的其他附加命令
--force 强制操作 如强制安装删除等；
--requires 显示该包的依赖关系；
--nodeps 忽略依赖关系并继续操作；
