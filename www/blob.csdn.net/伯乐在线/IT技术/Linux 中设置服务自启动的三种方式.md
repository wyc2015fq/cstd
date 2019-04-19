# Linux 中设置服务自启动的三种方式 - 文章 - 伯乐在线
原文出处： [Nerxious](http://www.cnblogs.com/nerxious/archive/2013/01/18/2866548.html)
有时候我们需要Linux系统在开机的时候自动加载某些脚本或系统服务主要用三种方式进行这一操作：


```
ln -s                       在/etc/rc.d/rc*.d目录中建立/etc/init.d/服务的软链接(*代表0～6七个运行级别之一）
chkonfig                命令行运行级别设置
ntsysv                   伪图形运行级别设置
```
**注意：**1.这三种方式主要用于以redhat为基础的发行版
2.如果还不知道运行级别是什么，那么最好先看看相关资料再实验
#### **第一种方式：ln -s 建立启动软连接**
在Linux中有7种运行级别（可在*/etc/inittab*文件设置），每种运行级别分别对应着*/etc/rc.d/rc[0~6].d*这7个目录
![](http://jbcdn2.b0.upaiyun.com/2016/11/b48fbd731379141fe2703f249867dd12.png)
**Tips：***/etc/rc[0~6].d*其实是*/etc/rc.d/rc[0~6].d*的软连接，主要是为了保持和Unix的兼容性才做此策
这7个目录中，每个目录分别存放着对应运行级别加载时需要关闭或启动的服务
由详细信息可以知道，其实每个脚本文件都对应着*/etc/init.d/*目录下具体的服务
K开头的脚本文件代表运行级别加载时需要关闭的，S开头的代表需要执行
![](http://jbcdn2.b0.upaiyun.com/2016/11/0edf056ea91d632f1c0315f2e0453a05.png)
因此，当我们需要开机启动自己的脚本时，只需要将可执行脚本丢在*/etc/init.d*目录下，然后在*/etc/rc.d/rc*.d*中建立软链接即可

```
[root@localhost ~]# ln -s /etc/init.d/sshd /etc/rc.d/rc3.d/S100ssh
```
此处sshd是具体服务的脚本文件，S100ssh是其软链接，S开头代表加载时自启动
如果需要在多个运行级别下设置自启动，则需建立多个软链接
这种方式比较繁琐，适用于自定义的服务脚本
如果系统中已经存在某些服务（比如安装apache时就会有httpd服务项），可以使用下面的两种方式
#### **第二种方式：chkconfig**
![](http://jbcdn2.b0.upaiyun.com/2016/11/5fbc08b5238622c20e0d2393a2d32957.png)
如果需要自启动某些服务，只需使用*chkconfig 服务名 on*即可，若想关闭，*将on改为off*
在默认情况下，*chkconfig*会自启动*2345*这四个级别，如果想自定义可以加上*–level*选项
![](http://jbcdn2.b0.upaiyun.com/2016/11/6f553d765cbd1df56762d5c87f4dab2e.png)
上面我们先将sshd服务的所有启动级别关闭，然后使用*–level*选项启动自定义级别
**Tips：***–list*选项可查看指定服务的启动状态，*chkconfig*不带任何选项则查看所有服务状态
#### **第三种方式：ntsysv 伪图形**
*ntsysv*和*chkconfig*其实是一样的，只不过加上了图形而已
启动*ntsysv*有两种方式，一是直接在命令行中输入*ntsysv*，二是使用*setup*命令，然后选择系统服务
![](http://jbcdn2.b0.upaiyun.com/2016/11/75d6f167d4e330ec2a2b34aa0614f62b.png)
默认情况下，当前运行级别为多少，在*ntsysv*中设置的启动服务的级别便是多少
比如，我当前的运行级别是3,那么我在伪图形界面中选择启动服务后，它的运行级别也会是3
如果想自定义运行级别可使用*ntsysv –level*方式
![](http://jbcdn2.b0.upaiyun.com/2016/11/0c38cde21c128f324cbfcde0400c2b7e.png)
以上三种操作需要保证服务脚本文件可执行，并且要有root权限
其中，第一种方式多用于自定义脚本，第二、三种多用于系统已存在的服务
比如ftp、samba、ssh、httpd等等
并且，要做相关设置需要弄清楚运行级别的问题
**Tips：**如果想手动启动某服务，传统的方式是 */etc/init.d 服务名 start*
实际上还可以这样，*service 服务名 start*
![](http://jbcdn2.b0.upaiyun.com/2016/11/bd5c8af61cf032a7d9f86e7aa15291ee.png)
