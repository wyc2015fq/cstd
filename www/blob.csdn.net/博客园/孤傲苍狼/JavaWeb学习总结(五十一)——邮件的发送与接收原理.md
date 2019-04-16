# JavaWeb学习总结(五十一)——邮件的发送与接收原理 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaWeb学习总结(五十一)——邮件的发送与接收原理](https://www.cnblogs.com/xdp-gacl/p/4209586.html)



## 一、 邮件开发涉及到的一些基本概念

### 1.1、邮件服务器和电子邮箱

　　要在Internet上提供电子邮件功能，必须有专门的电子邮件服务器。例如现在Internet很多提供邮件服务的厂商：sina、sohu、163等等他们都有自己的邮件服务器。

　　这些邮件服务器类似于现实生活中的邮局，它主要负责接收用户投递过来的邮件，并把邮件投递到邮件接收者的电子邮箱中。

　　电子邮箱(E-Mail地址)的获得需要在邮件服务器上进行申请，确切地说，电子邮箱其实就是用户在邮件服务器上申请的一个账户，用户在邮件服务器上申请了一个帐号后，邮件服务器就会为这个账号分配一定的空间，用户从而可以使用这个帐号以及空间发送电子邮件和保存别人发送过来的电子邮件。

### 1.2、邮件传输协议

#### 1.2.1、SMTP协议

　　用户连接上邮件服务器之后，要想给它发送一封电子邮件，需要遵循一定的通讯规则，SMTP协议就是用来定义这种通讯规则的。因此，我们通常也把处理用户smtp请求(邮件发送请求)的服务器称之为SMTP服务器(邮件发送服务器)。

#### 1.2.2、POP3协议

　　同样，用户若想从邮件服务器管理的电子邮箱当中接收一封电子邮件话，它连上邮件服务器后，也要遵循一定的通讯格式，POP3协议就是用来定义这种通讯格式的。因此，我们通常也把处理用户pop3请求(邮件接收请求)的服务器称之为POP3服务器(邮件接收服务器)。

### 1.3、电子邮件的发送和接收过程

　　通过一张图来说明一封邮件的发送和接收过程，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/082159327033346.png)

　　简单说一下这个邮件收发过程：

　　1、[xdp@sohu.com](mailto:xdp@sohu.com) 用户写好一封Email发送到sohu的Smtp服务器。对应于上图的步骤①

　　2、sohu的Smtp服务器开始处理[xdp@sohu.com](mailto:xdp@sohu.com) 用户的请求，它会根据收件人的地址判断，当前收件人是不是自己管辖的用户，如果是，就直接将Email存放到为该收件人的分配的邮箱空间当中。sohu的Smtp服务器判断收件人地址发现，这一封Email的收件人[gacl@sina.com](mailto:gacl@sina.com) 是Sina的邮件服务器管理的，于是又将Email转发给Sina的Smtp服务器。对应于上图的步骤②

　　3、Sina的Smtp服务器开始处理sohu的Smtp服务器发送过来的Email，Sina的Smtp服务器根据收件人的地址判断，发现收件人自己管辖的用户，于是就直接将Email存放到为[gacl@sina.com](mailto:gacl@sina.com) 用户的分配的邮箱空间当中。对应于上图的步骤③。

　　4、[xdp@sohu.com](mailto:xdp@sohu.com) 用户将邮件发出去之后，就通知[gacl@sina.com](mailto:gacl@sina.com) 用户去收取。[gacl@sina.com](mailto:gacl@sina.com) 用户于是就连接上Sina的POP3服务器收取邮件，对应于上图的步骤④。

　　5、POP3服务器从[gacl@sina.com](mailto:gacl@sina.com) 用户的邮箱空间当中取出Email，对应于步骤⑤。

　　6、POP3服务器将取出来的Email发给[gacl@sina.com](mailto:gacl@sina.com) 用户，对应于步骤⑥。

## 二、使用Smtp协议发送邮件

### 2.1、Smtp协议讲解

　　使用smtp协议发送邮件给邮件服务器时规定了要做以下几件事

　　1、使用"**ehlo**"命令和连接上的smtp服务器打声招呼，例如：

　　　　 ehlo gacl

　　2、使用"**auth login**"命令登录到Smtp服务器，登录使用的用户名和密码必须经过Base64加密，例如：　　　

　　　　①、输入命令：auth login  

　　　　②、输入使用Base64加密过后的用户名：Z2FjbA==  

　　　　③、输入Base64加密过后的密码：MTIzNDU2

　　3、指明邮件的发件人和收件人

　　　　mail from:<[gacl@sohu.com](mailto:gacl@sohu.com)>

　　　　rcpt to:<[xdp_gacl@sina.cn](mailto:xdp_gacl@sina.cn)>

　　 4、编写要发送的邮件内容，邮件的编写格式是有一定的规则的，一封格式良好的邮件应该包含邮件头和邮件的主体内容。

　　　　邮件头使用下面的三个字段来指明
- - from字段用于指明邮件的发送人
- to字段用于指明邮件的收件人
- subject字段用于指明邮件的主题


　　　　邮件的内容包含了这些信息之后才是一封格式良好的邮件。

　　　　①、输入"data"命令

　　　　　　data

　　　　②、编写邮件内容

　　　　　　from:<[gacl@sohu.com](mailto:gacl@sohu.com)>　　　　----邮件头

　　　　　　to:<[xdp_gacl@sina.cn](mailto:xdp_gacl@sina.cn)> 　　　　----邮件头

　　　　　　subject:hello　　　　　　　　　　----邮件头

　　　　　　　　　　　　　　　　　　　　　　-----空行

　　　　　　hello gacl 　　　　　　　　　　　----邮件的具体内容

　　5、输入一个.告诉邮件服务器邮件内容已经写完了

　　　　.

　　6、输入quit命令断开与邮件服务器的连接

　　　　quit

　　以上的6个步骤就是Smtp协议规定的发送一封Email必须要做的事情。

### 2.2、使用Smtp协议手工发送邮件

　　在对Smtp协议有一定的了解之后，我们就可以使用Smtp协议来发送邮件了。下面演示一下使用Telnet客户端连接上搜狐的邮件服务器然后发一封Email到新浪的邮箱中去

　　为了能够对Smtp协议有一个直观的认识，这里我们不借助任何第三方邮件客户端工具，而是使用最原始的的Telnet客户端来完成邮件的发送过程，Telnet是一个Window自带的网络客户端程序，通过它可以连接上互联网上面的任意一台主机。

　　使用telnet客户端连接到搜狐的smtp服务器，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/082322350623241.png)

　　通过telnet客户端发Email，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/100955028126503.png)

　　我们登录到<[xdp_gacl@sina.cn](mailto:xdp_gacl@sina.cn)>邮箱当中，就可以收取到由<[gacl@sohu.com](mailto:gacl@sohu.com)>发送的Email了，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/101000289064347.png)

　　这就是使用Smtp协议发送邮件的过程。

## 三、使用POP3协议接收邮件

### 3.1、POP3协议讲解

　　POP3协议规定了收取邮件时要做以下几件事

①、输入用户名和密码登录到POP3服务器，用户名和密码不需要经过Base64加密

　　user username --登录邮箱的用户名
　　pass password --登录邮箱使用的密码

②、使用retr命令收取邮件
　　retr 数字 收取邮件，retr 1表示收取邮箱当中的第一封邮件，这是POP3协议里面最重要的一个命令。

　　在使用retr命令收取邮件之前，可以使用如下的两个命令查看一下邮箱里面的邮件的一些相关信息。

　　stat 

　　　　返回邮箱里面的邮件数量以及邮件占用的空间大小信息
　　list 数字 

　　　　返回某一封邮件的统计信息

③、邮件收取完成之后使用quit命令断开与POP3服务器的连接。
　　quit 断开与POP3服务器的连接

### 3.2、使用POP3协议手工接收邮件

　　收取邮件我们也不借助任何第三方客户端工具，而是使用Telnet客户端连接到POP3服务器进行收取。

　　例如：现在我的搜狐邮箱当中有这样的一封邮件，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/101335341403691.png)

　　现在我们不用使用foxmail或者outLook这样的客户端工具去收取，而是使用Telnet客户端连接到搜狐的POP3服务器去手工收取。

　　1、使用Telnet连接上搜狐的POP3服务器，使用命令：telnet pop3.sohu.com 110，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/101343235623114.gif)

　　根据POP3协议规定的邮件收取步骤来收取邮件。如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/101330536564215.png)

　　可以看到，我们POP3协议纯手工从搜狐的POP3服务器当中收取回来了一封Email，Email里面的内容都经过了Base64编码处理，下面我们写一个小程序将经过Base64编码后的邮件内容进行解码，还原回邮件的内容，代码如下：

```
1 package me.gacl.encrypt;
 2 
 3 import java.io.BufferedReader;
 4 import java.io.IOException;
 5 import java.io.InputStreamReader;
 6 
 7 import sun.misc.BASE64Decoder;
 8 import sun.misc.BASE64Encoder;
 9 
10 public class Base64Encrypt {
11 
12     public static void main(String args[]) throws IOException{
13         /*System.out.print("请输入用户名:");
14         BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
15         String userName = in.readLine();
16         System.out.print("请输入密码:");
17         String password = in.readLine();
18         BASE64Encoder encoder = new BASE64Encoder();
19         System.out.println("编码后的用户名为:" + encoder.encode(userName.getBytes()));
20         System.out.println("编码后的密码为:" + encoder.encode(password.getBytes()));*/
21         
22         BASE64Decoder decoder = new BASE64Decoder();
23         //邮件主题的Base64编码
24         String emailSubject = "=?GBK?B?08q8/rLiytQ=?=";
25         //邮件文本内容的Base64编码
26         String emailPlainContent = "vPK1pbXE08q8/reiy82y4srUo6E=";
27         //带html标签和邮件内容的Base64编码
28         String emailHtmlContent = "PFA+vPK1pbXE08q8/reiy82y4srUo6E8L1A+";
29         //将使用Base64编码过后的文本内容再使用Base64来解码
30         emailSubject = new String(decoder.decodeBuffer(emailSubject),"GBK");
31         emailPlainContent = new String(decoder.decodeBuffer(emailPlainContent),"GBK");
32         emailHtmlContent = new String(decoder.decodeBuffer(emailHtmlContent),"GBK");
33         System.out.println("邮件标题："+emailSubject);
34         System.out.println("邮件内容："+emailPlainContent);
35         System.out.println("带html标签的邮件内容："+emailHtmlContent);
36     }
37 }
```

　　运行结果如下：
![](https://images0.cnblogs.com/blog/289233/201501/101349446713015.png)

　　这就是使用POP3协议收取邮件的过程。

　　以上就是邮件的发送与接收原理的相关内容，这一篇文章主要是介绍邮件发送和接收过程中使用到的smtp协议和pop3协议。没有涉及到太多代码方面的东西，后面会具体介绍使用JavaMail来进行邮件发送。









