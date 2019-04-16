# 基于Java Socket实现的SMTP邮件客户端 - 全面支持SSL, TLS - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年03月15日 23:38:08[gloomyfish](https://me.csdn.net/jia20003)阅读数：6696








**协议完成有：**

**- 最小化完成了RFC821， RFC822， 简单的邮件发送协议，邮件格式化协议**

**- 最小化完成RFC4648 - Base64编码协议**

**- 完成扩展协议[MS-XLOGIN]**

**- 基于SSLSocket完成的支持 SSL， TLS等加密SMTP传输协议**

****

**基于Swing程序界面：**

![](http://hi.csdn.net/attachment/201203/15/0_1331823180Lj8x.gif)


![](http://hi.csdn.net/attachment/201203/15/0_1331823388sIr8.gif)


**支持操作如下：**

**- 设置 SMTP Server, 端口， 用户名和密码**

**- 编辑简单纯文本E-Mail内容，发送E-Mail**




**项目文件列表：**

![](http://hi.csdn.net/attachment/201203/15/0_13318240605s23.gif)


**基于Swing的Model和View分离原则完成编码。JDK 1.6上面通过测试。**

**在QQ 邮箱, GMail上测试通过！**

**QQ SMTP Server - smtp.qq.com**

**GMail SMTP Server - bfnh20009@gmail.com**




**程序关键点：**

**- Swing开发，CardLayout， Custom UI， Layout技巧**

**- Swing开发中如何实现MVC**

**- SMTP协议，Socket编程技巧**

**- SSL, TLS简单原理知识**




**源代码下载地址：**

[**http://download.csdn.net/detail/jia20003/4144771**](http://download.csdn.net/detail/jia20003/4144771)




