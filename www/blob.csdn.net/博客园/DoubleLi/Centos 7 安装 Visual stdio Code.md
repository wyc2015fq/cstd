# Centos 7 安装 Visual stdio Code - DoubleLi - 博客园






最近微软正式发布了.net code 和asp.net code。尝试了下在linux下.net code和asp.net code使用。

具体怎么使用.net code 和asp.net code 请大家阅读大内老A写的“[通过几个Hello World感受.NET Core全新的开发体验](http://www.cnblogs.com/artech/p/net-core-hello-world.html)". 这里主要写在Centos 7 安装 Visual stdio Code。



环境参数：

操作系统版本：CentOS-7-x86_64-1511

软件版本：visual stdio code 1.2



操作步骤：

1、下载 [vscode_x86_64.rpm](https://code.visualstudio.com/b?utm_expid=101350005-21.ckupCbvGQMiML5eJsxWmxw.1&utm_referrer=https%3A%2F%2Fwww.microsoft.com%2Fnet%2Fcore)

2、在root或普通用户安装rpm。注意：早期版本只能在普通用户下安装

方法1在直接安装

方法2下载到文件目录，点右键安装或使用命令（yum install 安装）

3、验证安装成功

在命令窗口，输入“code --version”,安装成功显示

![](https://images2015.cnblogs.com/blog/119816/201607/119816-20160705221515264-42067035.jpg)



或在应用程序的安装程序中查看是否安装了Visual stdio code

![](https://images2015.cnblogs.com/blog/119816/201607/119816-20160705221700546-792665502.jpg)

4、注意使用普通用户打开vscode

在使用root用户点击launch时，无反应。在命令窗口输入“code --version”,输出
“It is recommended to start vscode as a normal user. To run as root, you must specify an alternate user data directory with the --user-data-dir argument.”

大致的意思是建议使用普通用户打开vscode,如果在root下，需要指定一个用户的数据目录。

怎样在root下打开需要再研究，目前在普通用户下，已经可以使用vscode啦

打开方式：

方式1在命令窗口输入”code"

方式2在应用程序的安装程序中可视化操作打开





可参考微软官方地址：https://code.visualstudio.com/docs/setup/linux









