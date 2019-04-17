# Hyperic Sigar API 举例 - DoubleLi - 博客园






Hyperic HQ 是什么? 

         Hyperic HQ 是一个开源的（General Public License，GPL授权）IT资源管理框架，让用户使用统一的界面来管理各种不同的IT资源的管理，Hyperic HQ 可以监控和管理：

        ★操作系统：AIX，HP/UX，Linux，Solaris，Windows，Mac OSX，FreeBSD

        ★ Web服务器：Apache，Microsoft IIS，Sun ONE Web Server

        ★ 应用服务器：BEA WebLogic，IBM WebSphere，JBoss，Apache Geronimo，Macromedia ColdFusion，Macromedia JRun，Microsoft .NET Runtime，Novell Silverstream，Tomcat，Caucho Resin ，Jetty（官网公布的文档中没有jetty，但是我发现发行包中确实有个jetty的plugin，所以也把jetty列出来了，还有待考证）

        ★ 数据库：IBM DB2，Microsoft SQL Server，MySQL，Oracle，PostgreSQL，Sybase Adaptive Server

        ★消息中间件: ActiveMQ，Weblogic MQ

        ★ 微软的产品: MS Exchange，MS ActiveDirectory，.NET

        ★ 虚拟产品: VMWare，Citrix Metaframe

        ★应用平台: LAMP，LAM-J，J2EE，MX4J

        其他：网络设备交换机，路由器，网络服务等。

 -----------------------------------------------------------

        Hyperic HQ  安装分成两个部分：Server 和Agent。Server是安装在一台服务器上作为管理服务端，Agent 分别安装在不同的需要被管理的服务器上。Angent的配置信息指向server 端。简易的结构如下：

> 
> 
                                                                  ( ---------agent

浏览器 (-----------------------------)  ( server )  (------------------agent

                                                                  ( ---------agent



       sigar全称SIGAR - System Information Gatherer And Reporter（系统信息采集报告器）。一个提供第三方开发的API。

       下载地址：[http://sourceforge.net/projects/sigar/files/sigar/1.6/](http://sourceforge.net/projects/sigar/files/sigar/1.6/)

       文档地址：[http://support.hyperic.com/display/SIGAR/Home](http://support.hyperic.com/display/SIGAR/Home)

       windows下开发需要： sigar.jar 和sigar-x86-winnt.dll

       Linux下需要：sigar.jar和对应构架下的so。可以在文档的binaries表格中看到对应关系。

       项目中lib加入sigar.jar 对应的dll文件或者so文件添加到系统目录。例如：windows 下把sigar-x86-winnt.dll添加到c:\WINDOWS\system32或者jdk的bin目录。

 例子：
- 



import java.util.Arrays;


- 



import java.util.List;


- 




- 



import org.hyperic.sigar.win32.Service;


- 



import org.hyperic.sigar.win32.Win32Exception;


- 




- 



public class ServiceStatus {


- 




- 



// 打印出服务器 服务名称 和 状态信息


- 



private static void printStatus(String name) throws Win32Exception{


- 




- 



Service service = new Service(name);


- 




- 



System.out.println(name +":" +service.getStatusString());


- 




- 



service.close();


- 



}


- 




- 



public static void main(String[] args) throws Win32Exception {


- 




- 



List service;


- 



String name;


- 




- 



if(args.length == 0){


- 



service = Service.getServiceNames();


- 



}else{


- 



service = Arrays.asList(args);


- 



}


- 




- 



for(int i=0;i<service.size();i++){


- 



printStatus((String)service.get(i));


- 



}


- 




- 



}


- 



}



          上面的代码比较简单，就是打印出服务器服务名称和状态。 结果如下：
- 



360rp:Running


- 



Alerter:Stopped


- 



ALG:Stopped


- 



AntiVirSchedulerService:Running


- 



AntiVirService:Running


- 



Apache2.2:Stopped


- 



AppMgmt:Stopped


- 



aspnet_state:Stopped


- 



………………………………



          Sigar核心包简单说明：
- 



Sigar


- 



org.hyperic.sigar Sigar - 核心库，采集信息


- 



一般情况下Sigar sigar = new Sigar(); //拿到当前系统收集器


- 



   sigar.getFileSystemList(); //通过sigar的get方法收集相关信息，这部分网上例子比较多。


- 



Other Packages


- 



org.hyperic.sigar.cmd 系统命令.


- 



org.hyperic.sigar.ptql 进程方面


- 



org.hyperic.sigar.shell Shell命令.


- 



org.hyperic.sigar.util 工具类打印、列表.


- 



org.hyperic.sigar.vmware 虚拟机vmware


- 



org.hyperic.sigar.win32 window操作系统windows32 事件、本地信息、服务、服务配置等信息。


- 





Sigar 说明和命令行


[http://support.hyperic.com/display/SIGAR/Home](http://support.hyperic.com/display/SIGAR/Home)










