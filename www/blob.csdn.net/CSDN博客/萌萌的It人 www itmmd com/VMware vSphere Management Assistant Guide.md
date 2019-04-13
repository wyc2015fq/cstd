
# VMware vSphere Management Assistant Guide - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月18日 17:24:56[Jlins](https://me.csdn.net/dyllove98)阅读数：2625


部署vMA虚拟机
![image](http://s3.51cto.com/wyfs02/M01/53/8D/wKioL1Rq8PPB8bu_AABuYvSR1v4996.jpg)
![image](http://s3.51cto.com/wyfs02/M02/53/8F/wKiom1Rq8JPgPkOhAACI156Fq6g459.jpg)
![image](http://s3.51cto.com/wyfs02/M01/53/8D/wKioL1Rq8QqwPJlEAAEq4KDiD0Q332.jpg)
![image](http://s3.51cto.com/wyfs02/M02/53/8D/wKioL1Rq8QrBTrE5AAB-tMxaLDc983.jpg)
![image](http://s3.51cto.com/wyfs02/M00/53/8D/wKioL1Rq8QvBm26fAACSXfifkWM305.jpg)
![image](http://s3.51cto.com/wyfs02/M01/53/8D/wKioL1Rq8Qzj0BGRAAC7ojGBBmQ643.jpg)
![image](http://s3.51cto.com/wyfs02/M02/53/8D/wKioL1Rq8RDgRQlMAABu6ElkocY515.jpg)
![image](http://s3.51cto.com/wyfs02/M00/53/8D/wKioL1Rq8RDjTZgdAACRaTC8zWs370.jpg)
![image](http://s3.51cto.com/wyfs02/M01/53/8D/wKioL1Rq8RHgbuYsAAB7191mKUA987.jpg)
![image](http://s3.51cto.com/wyfs02/M02/53/8D/wKioL1Rq8RHhxuAYAACkMTeAX5s968.jpg)
点击“完成”开始部署。
vMA虚拟机设置
部署完成后，打开虚拟机电源，在控制台窗口分别设置IPv4、netmask、hostname、IPv4 gateway、dns。设置完成后，按1退出程序。接着更改vi-admin用户密码。密码要求如下：
9个字符
1个大写
1个小写
1个数字
一个$或者\#
设置完密码后，使用vi-admin账户登录系统，使用如下命令更改root密码。
|vi-admin@vma:~>|sudo passwd root|
启用vi-user用户:
|vi-admin@vma:~>|sudo passwd vi-user
|vi-admin's password:
|Changing password for vi-user.
|New password:
|Retype new password:
|Password changed.
|
向vMA添加vCenter、esxi:
|vi-admin@vma:~>|vifp addserver vcenter.xzxj.edu.cn
|Enter username for vcenter.xzxj.edu.cn:|administrator@vsphere.local
|administrator@vsphere.local@vcenter.xzxj.edu.cn's password:
|This will store username and password in credential store which is a security risk. Do you want to continue?(yes/no):|yes
|vi-admin@vma:~>|vifp listservers --long
|vcenter.xzxj.edu.cn vCenter fpauth
|vi-admin@vma:~>|vifp addserver node01.xzxj.edu.cn
|root@node01.xzxj.edu.cn's password:
|vi-admin@vma:~>|vifp addserver node02.xzxj.edu.cn
|root@node02.xzxj.edu.cn's password:
|//列出已添加服务器
|vi-admin@vma:~> vifp listservers --long
|vcenter.xzxj.edu.cn vCenter fpauth
|node01.xzxj.edu.cn ESXi fpauth
|node02.xzxj.edu.cn ESXi fpauth
|
设置健康检查脚本
此健康检查脚本名称为vmwarevSphereHealthCheck.pl，其配置文件为healthCheck.conf。建议将此脚本和配置文件存放在/home/vi-admin/bin目录下。修改文件属性如下所示：
|vi-admin@vma:~/bin>|chown vi-admin:root vmwarevSphereHealthCheck.pl
|vi-admin@vma:~/bin>|chown vi-admin:root healthCheck.conf
|
![image](http://s3.51cto.com/wyfs02/M00/53/8D/wKioL1Rq8RLT0ewhAAEA7VN9iBQ932.jpg)
接着运行vmwarevSphereHealthCheck.pl脚本，如下所示：
|vi-admin@vma:~>|vmwarevSphereHealthCheck.pl --server|vcenter.xzxj.edu.cn|--username|administrator@vsphere.local|--config|bin/healthCheck.conf|--type|vcenter
|Enter password:
|Generating VMware vSphere Health Report v5.5.0 "vmware_health_report.html" ...
|This can take a few minutes depending on the size of your environment.
|Get a cup of coffee/tea/beer and check out http://www.virtuallyghetto.com
|Wide character in print at /home/vi-admin/bin/vmwarevSphereHealthCheck.pl line 554.
|Wide character in print at /home/vi-admin/bin/vmwarevSphereHealthCheck.pl line 3582.
|Wide character in print at /home/vi-admin/bin/vmwarevSphereHealthCheck.pl line 3896.
|Start Time: 11-17-2014 21:31:00
|End Time: 11-17-2014 21:31:21
|Duration : 21 Seconds
|
运行完成后，会在当前目录下生成vmware_health_report.html文件。使用浏览器打开后可查看VMware的相关信息。如果打开后，出现乱码，将浏览器的字符集更改为Unicode即可。
参考文献:[VMware vSphere
 Management Assistant Guide](http://pubs.vmware.com/vsphere-55/index.jsp#com.vmware.vma.doc/vima_Preface.html)
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

