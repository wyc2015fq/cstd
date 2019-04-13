
# Linux 修改SSH 默认端口 22，防止被破解密码 - 阳光岛主 - CSDN博客

2019年01月25日 11:39:53[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：195


Linux/Unix 系统，很多人使用SSH + 密码来登陆服务器，默认 22端口，这样会有被暴力破解密码的危险（除非密码足够复杂且长度很长），因此最好修改SSH默认的22端口为其它随机端口号。
为了保险起见，推荐先添加一个SSH 随机端口号并添加对应的防火墙规则，然后用这个新的端口连接服务器试试，如果没问题了，我们再删除默认的22端口。这样做的好处，就是因为如果新修改的端口号无法连接，可以仍然使用默认的22端口登录，否则，可能无法连接或出现一些问题后会导致你无法通过SSH连接到服务器，那就惨了。
本文原文来自米扑博客：[Linux 修改SSH 默认端口 22，防止被破解密码](https://blog.mimvp.com/article/28602.html)
**本文实例的系统环境**
阿里云 CentOS 7.4
**一、修改配置文件**
1. 修改配置文件 /etc/ssh/sshd_config
vim /etc/ssh/sshd_config
修改
```python
#Port 22
#ListenAddress 0.0.0.0
#ListenAddress ::
```
为
```python
Port 22
Port 23456
#ListenAddress 0.0.0.0
#ListenAddress ::
```
如上，取消注释 Port 22，并在其下方增加一行 Port 23456
说明：
SSH默认监听端口是22，如果不强制说明，”Port 22”注释或不注释，都会默认开放端口22远程登录
上面取消注释并保留了22端口，防止可能各种权限和配置问题，导致22端口不能访问，那就尴尬了
增加了一行 Port 23456 端口，就是为了来替换默认端口22的，大家在修改端口时候最好挑10000~65535之间的端口号，10000以下容易被系统或一些特殊软件占用，或是以后新安装的应用可能占用该端口，所以10000以下的端口号都不要使用。
**二、使其 sshd 配置生效**
1、执行如下命令，使 sshd 配置修改后生效
1）CentOS 7.x 以上系统，执行命令
systemctl restart sshd.service
2）CentOS 6.x 以下系统，执行命令
/etc/init.d/sshd restart
2、生效后，使用新端口号登录
ssh root@47.106.126.167 -p 23456
\# ssh root@47.106.126.167 -p 23456
root@47.106.126.167's password:
输入密码即可成功登录
注：此时 22 和 23456 两个端口都可以成功登录ssh
3、确认新端口可登录，注释掉22端口
vim /etc/ssh/sshd_config
注释掉端口22，最后的配置内容如下
```python
#Port 22
Port 23456
#ListenAddress 0.0.0.0
#ListenAddress ::
```
最后，别忘了修改配置文件后，使其生效
1）CentOS 7.x 以上系统，执行命令
systemctl restart sshd.service
2）CentOS 6.x 以下系统，执行命令
/etc/init.d/sshd restart
**三、防火墙允许新端口号**
阿里云使用 CentOS 7 以前的版本并开启默认防火墙 iptables 时，应注意 iptables 默认不拦截访问
如果您配置了 iptables 规则，需要执行命令允许新端口：
`iptables -A INPUT -p tcp --dport 23456 -j ACCEPT`
然后执行重启防火墙命令
service iptables restart
**说明**：
CentOS 7 以后版本默认安装 Firewalld
首先，查看防火墙是否开启了 23456 端口号
firewall-cmd --permanent --query-port=23456/tcp
若打印结果为 no 表示没有开放 23456 端口号，那么添加允许新端口号，运行命令
firewall-cmd --permanent --add-port=23456/tcp
返回结果为 success 即表示放行 TCP 23456 端口号
接着，重新加载防火墙策略，使配置生效
firewall-cmd --reload
最后，再次查询是否开放了 23456 端口号
firewall-cmd --permanent --add-port=23456/tcp
若开放了新端口号，此时会打印 yes
**四、策略组允许新端口号**
阿里云、腾讯云等很多云服务器商都有安全组策略，若防火墙开放了新端口号，但安全组没有开放新端口号，也是无法ssh登录的，因此还需要在安全组开放新端口号
登录[ECS 管理控制台](https://ecs.console.aliyun.com/#/home)，找到该实例，选择**网络和安全组**
在**安全组规则**页面，单击**添加安全组规则，**根据实际的使用场景来定义安全规则，允许新配置的远程端口进行连接。
关于如何设置安全组参见[添加安全组规则](https://help.aliyun.com/document_detail/25471.html)


