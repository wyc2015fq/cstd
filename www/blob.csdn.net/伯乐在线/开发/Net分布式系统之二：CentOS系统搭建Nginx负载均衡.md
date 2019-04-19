# Net分布式系统之二：CentOS系统搭建Nginx负载均衡 - 文章 - 伯乐在线
原文出处： [Andon](http://www.cnblogs.com/Andon_liu/p/5353522.html)
**一、关于CentOS系统介绍**
CentOS（Community Enterprise Operating System，中文意思是：社区企业操作系统）是Linux发行版之一，它是来自于Red Hat Enterprise Linux依照开放源代码规定释出的源代码所编译而成。基于Red Hat持续升级，和对已知BUG修复，所以CentOS更多用于搭建大型企业级服务器。目前较新版本为7.0，本文使用CentOS7 64bit进行搭建系统负载均衡。
**二、安装VMWare**
VMWare (Virtual Machine ware)是一个“虚拟PC”软件公司.它的产品可以使你在一台机器上同时运行二个或更多Windows、DOS、LINUX系统。与“多启动”系统相比，VMWare采用了完全不同的概念。多启动系统在一个时刻只能运行一个系统，在系统切换时需要重新启动机器。本文使用VMWare WorkStation12安装CentOS操作系统。详细安装步骤参考：[查看](http://jingyan.baidu.com/article/215817f78879c21edb142379.html?qq-pf-to=pcqq.group)， 百度软件下载[点击](http://rj.baidu.com/soft/detail/13808.html?ald)。
**三、安装CentOS系统**
首先需要下载CentOS安装包，下载IOS格式，本文使用CentOS7 64bit 版本，[下载](http://mirrors.aliyun.com/centos/7/isos/x86_64/CentOS-7-x86_64-DVD-1511.iso)
详细安装步骤请[点击查看](http://jingyan.baidu.com/article/a3aad71aa180e7b1fa009676.html)
注意事项：
1、分区的时候，选择EXT4格式，boot分区为400m，/分区为8G，/home分区为7G，var分区为6G，swap分区为2G
2、用户和组ID，都设为 1000
3、安装完成重启后提示License的，输入：1、2、r、yes 完成进入成功进入系统
4、ctrl+alt 鼠标切回本机电脑，不在虚拟机里
**四、安装Nginx和配置**
1、Nginx介绍
Nginx是一款轻量级的Web 服务器/反向代理服务器及电子邮件（IMAP/POP3）代理服务器，并在一个BSD-like 协议下发行。由俄罗斯的工程师Igor Sysoev研发，供俄国大型的入口网站Rambler使用。其特点是占有内存少，并发能力强，事实上nginx的并发能力确实在同类型的网页服务器中表现较好。
2、安装部署
在CentOS7通过命令行安装Nginx服务器。[](https://www.digitalocean.com/community/tutorials/how-to-install-nginx-on-centos-7)
（1）添加Nginx仓储，添加 epel repository


Shell
```
sudo yum install epel-release
```
（2）检查是否已安装过nginx


Shell
```
find -name nginx
```
（3）如果有安装则删除


Shell
```
yum remove nginx
```
（4）安装nginx


Shell
```
sudo yum install nginx
```
（5）启动nginx


Shell
```
sudo systemctl start nginx
sudo systemctl enable nginx #可用
```
（6）将nginx 设置为启动系统自动启动nginx


Shell
```
echo "/usr/local/nginx/sbin/nginx" >> /etc/rc.local
```
（7）查看本机IP，为了后续配置nginx.conf所用


Shell
```
ifconfig
```
（8）修改nginx.conf配置文件


Shell
```
cd /etc/nginx/ #定位到nginx安装目录
vim nginx.conf #通过vim打开nginx.conf配置文件进行配置
```
（9）主要配置节点，192.168.119.1:8081 和 192.168.119.1:8082 分别部署在 IIS7的两个ASP.NET MVC 开发的站点（在另一台电脑），设计软件架构解决方案后续介绍。


Shell
```
upstream 192.168.119.128{
    server 192.168.119.1:8081;
    server 192.168.119.1:8082;
}
server{
    listen   80;
    server_name  192.168.119.128;
    location /{
        proxy_pass   http://192.168.119.128;
    }
    
}
```
（10）重启nginx服务


Shell
```
service nginx restart
```
以上记录在CentOS7操作系统搭建Nginx负载均衡解决方案记录。后续简介通过Keepalived搭建Nginx负载高可用解决方案。
> 
接下来介绍通过Nginx将请求分发到各web应用处理服务。
**一、Web应用开发**
**1、asp.net mvc5开发**
（1）新建一个MVC5工程，新建一个Controller，在Index方法实现将当前时间保存到Session[“mysession”]，并写Cookies[“mycookies”]存储主机名和当前时间。


C#
```
public ActionResult Index()
        {
            if (this.HttpContext.Session["mysession"] == null)
            {
                this.HttpContext.Session["mysession"] = DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss");
            }
            this.HttpContext.Response.Cookies.Add(new HttpCookie("mycookies")
                {
                    Expires = DateTime.Now.AddDays(1),
                    Value = HttpContext.Server.MachineName + "||" + DateTime.Now.ToString()
                });
            return View();
        }
```
（2）在Controller中新增第二个方法GetSession，显示Session和Cookies的值。


C#
```
public ActionResult GetSession()
        {
            if (this.HttpContext.Session["mysession"] != null)
            {
                ViewBag.DD = this.HttpContext.Session["mysession"].ToString();
                ViewBag.SCode = this.HttpContext.Session["mysession"].GetHashCode().ToString();
                ViewBag.SID = this.HttpContext.Session.SessionID;
            }
            ViewBag.CVAL = System.Web.HttpContext.Current.Request.Cookies["mycookies"].Value;
            ViewBag.CID = System.Web.HttpContext.Current.Request.Cookies["mycookies"].Name;
            ViewBag.CDO = System.Web.HttpContext.Current.Request.Cookies["mycookies"].Domain;
            return View();
        }
```
（3）将session和cookies信息在页面显示，GetSession视图代码如下：


C#
```
@{
    ViewBag.Title = "GetSession";
}
站点：A -- GetSession
站点：A
Session Value: @ViewBag.DD
Session SCode: @ViewBag.SCode
Session ID: @ViewBag.SID
Cookies ID: @ViewBag.CID
Cookies Values: @ViewBag.CVAL
Cookies Values: @ViewBag.CDO
```
以上实现session和cookies读写，为了验证负载均衡下，每次请求处理是否保持一致，接下来重要内容，做负载均衡如何如何保持session一致，对于asp.net技术session原理此处不做介绍，网上搜索下大把。
**2、Session共享技术**
.Net平台对支持几种session存储模式：
（1）InProc 模式
session存储于当前站点在同一个进程内，修改web.config或者bin中文件更新，会导致session丢失。此模式为默认模式。
（2）aspnet state 模式
aspnet state是将session存储在状态服务中，需要启动ASP.NET State Service，能看到进程aspnet_state.exe。还需要在web.config配置此模式。
（3）SQLServer 模式
此模式需要SQL Server配置相关信息，启动代理服务、数据库账号及表，并将web.config指向数据库。
（4）第三方扩展 模式
本框架采用此模式，将session存储到其他存储，比如：Memcached、redis缓存中，达到共享session的目的。可以通过实现ASP.NET中的SessionStateStoreProviderBase这个抽象类扩展。本系统采用将session存储在redis缓存中，通过引入  RedisSessionStateProvider组件。


C#
```
Install-Package Microsoft.Web.RedisSessionStateProvider
```
**3、Nginx服务状态情况**
在centos终端输入命令service nginx status 查看情况，确保服务正常运行。
![1](http://jbcdn2.b0.upaiyun.com/2016/06/28c8edde3d61a0411511d3b1866f06364.png)
**　　4、Web站点部署**
部署两个站点分别为：
站点A：端口为8081，
站点B：端口为8082，
**![2](http://jbcdn2.b0.upaiyun.com/2016/06/665f644e43731ff9db3d341da5c827e15.jpg)**
**二、功能效果展现**
（1）浏览器访问Index方法，http://192.168.119.128/demo，显示如下：
![3](http://jbcdn2.b0.upaiyun.com/2016/06/38026ed22fc1a91d92b5d2ef93540f204.jpg)![8](http://jbcdn2.b0.upaiyun.com/2016/06/815e6212def15fe76ed27cec7a393d593.jpg)
（2）浏览器访问GetSession方法，http://192.168.119.128/demo/getsession，显示如下：
![4](http://jbcdn2.b0.upaiyun.com/2016/06/011ecee7d295c066ae68d4396215c3d04.jpg)![9](http://jbcdn2.b0.upaiyun.com/2016/06/4c0d13d3ad6cc317017872e51d01b2382.jpg)
通过以上验证，获取到的session和cookies都是一致。
