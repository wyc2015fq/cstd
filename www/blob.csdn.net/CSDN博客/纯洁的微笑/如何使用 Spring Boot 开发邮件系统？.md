# 如何使用 Spring Boot 开发邮件系统？ - 纯洁的微笑 - CSDN博客
2018年10月08日 08:08:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：629
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TCHicQEF6XKCGu6sucu82Lics9V3bIiaMnamj16K9JQkDqM5xLXpicJsAxib6vIsTh1D50E9OUQ8jz2TlXQFa4Y2KUQ/640?wx_fmt=jpeg)
本文节选自 GitChat 精品课《精通 Spring Boot 42讲》
**1. 前言**
互联网发展到现在，邮件服务已经成为互联网企业中必备功能之一，应用场景非常广泛，比较常见的有：用户注册、忘记密码、监控提醒、企业营销等。
大多数互联网企业都会将邮件发送抽取为一个独立的微服务，对外提供接口来支持各种类型的邮件发送。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKO64Ejw3ibonrqcPOUeD1kMtgaXYRd2v11k6mocNkWgoJdzdbrwNWqzDg/640?wx_fmt=png)
#### 中国的第一封电子邮件
1987 年 9 月 14 日中国第一封电子邮件是由“德国互联网之父”维纳·措恩与王运丰在当时的兵器工业部下属单位—计算机应用技术研究所（简称 ICA）发往德国卡尔斯鲁厄大学的，其内容为德文和英文双语，第一段大意如下：
> 
原文：“ Across the Great Wall we can reach every corner in the world. ”
中文大意：“ 越过长城，我们可以到达世界的每一个角落。 ”
这是中国通过北京与德国卡尔斯鲁厄大学之间的网络连接，发出的第一封电子邮件。现在看这封邮件内容，颇具深意！
### **2. 邮件协议**
发送邮件的本质是将一个人的信息传输给另外一个人，那么如何传输就需要商量好标准，这些标准就是协议。最初只有两个协议：
#### · SMTP 协议
SMTP 的全称是 “Simple Mail Transfer Protocol”，即简单邮件传输协议。它是一组用于从源地址到目的地址传输邮件的规范，通过它来控制邮件的中转方式。它的一个重要特点是它能够在传送中接力传送邮件，即邮件可以通过不同网络上的主机接力式传送。
SMTP 认证，简单地说就是要求必须在提供了账户名和密码之后才可以登录 SMTP 服务器，这就使得那些垃圾邮件的散播者无可乘之机。增加 SMTP 认证的目的是为了使用户避免受到垃圾邮件的侵扰。SMTP主要负责底层的邮件系统如何将邮件从一台机器传至另外一台机器。
#### · POP3 协议
POP3 是 Post Office Protocol 3 的简称，即邮局协议的第3个版本，它规定怎样将个人计算机连接到 Internet 的邮件服务器和下载电子邮件的电子协议。
它是因特网电子邮件的第一个离线协议标准，POP3 允许用户从服务器上把邮件存储到本地主机（即自己的计算机）上，同时删除保存在邮件服务器上的邮件。
POP 协议支持“离线”邮件处理。其具体过程是：邮件发送到服务器上，电子邮件客户端调用邮件客户机程序以连接服务器，并下载所有未阅读的电子邮件。
这种离线访问模式是一种存储转发服务，将邮件从邮件服务器端送到个人终端机器上，一般是 PC 机或 MAC。
一旦邮件发送到 PC 机或 MAC上，邮件服务器上的邮件将会被删除。但目前的 POP3 邮件服务器大都可以“只下载邮件，服务器端并不删除”，也就是改进的 POP3 协议。
**SMTP 和 POP3 是最初的两个协议，随着邮件的不断发展后来又增加了两个协议：**
#### · IMAP 协议
全称 Internet Mail Access Protocol（交互式邮件存取协议），IMAP 是斯坦福大学在 1986 年开发的研发的一种邮件获取协议，即交互式邮件存取协议，它是跟 POP3 类似邮件访问标准协议之一。
不同的是，开启了 IMAP 后，在电子邮件客户端收取的邮件仍然保留在服务器上，同时在客户端上的操作都会反馈到服务器上，如：删除邮件，标记已读等，服务器上的邮件也会做相应的动作。
所以无论从浏览器登录邮箱或者客户端软件登录邮箱，看到的邮件以及状态都是一致的。
IMAP 的一个与 POP3 的区别是：IMAP 它只下载邮件的主题，并不是把所有的邮件内容都下载下来，而是你邮箱当中还保留着邮件的副本，没有把你原邮箱中的邮件删除，你用邮件客户软件阅读邮件时才下载邮件的内容。
较好支持这两种协议的邮件客户端有：Foxmail、Outlook 等。
#### · Mime 协议
由于 SMTP 这个协议开始是基于纯 ASCⅡ文本的，在二进制文件上处理得并不好。后来开发了用来编码二进制文件的标准，如 MIME，以使其通过 SMTP 来传输。
今天，大多数 SMTP 服务器都支持 8 位 MIME 扩展，它使二进制文件的传输变得几乎和纯文本一样简单。
用一张图来看发送邮件过程中的协议使用：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOQQicyibbx3p65YNHViaNu2H4kAmkM4qQYM1AL8ibk8z7Sicf0dw18oOoJUQ/640?wx_fmt=png)
> 
**实线代表** neo@126.com 发送邮件给 itclub@aa.com；
**虚线代表** itclub@aa.com 发送邮件给 neo@126.com
### **3. 邮件发送流程**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOTB4ClXJiarWlRs8ThXu6fgIO3MqvMuXFoxpJWV6DPND9icAiaicTIvnJxw/640?wx_fmt=png)
- 
发信人在用户代理上编辑邮件，并写清楚收件人的邮箱地址；
- 
用户代理根据发信人编辑的信息，生成一封符合邮件格式的邮件；
- 
用户代理把邮件发送到发信人的邮件服务器上，邮件服务器上面有一个缓冲队列，发送到邮件服务器上面的邮件都会加入到缓冲队列中，等待邮件服务器上的 SMTP 客户端进行发送；
- 
发信人的邮件服务器使用 SMTP 协议把这封邮件发送到收件人的邮件服务器上
- 
收件人的邮件服务器收到邮件后，把这封邮件放到收件人在这个服务器上的信箱中；
- 
收件人使用用户代理来收取邮件。首先用户代理使用 POP3 协议来连接收件人所在的邮件服务器，身份验证成功后，用户代理就可以把邮件服务器上面的收件人邮箱里面的邮件读取出来，并展示给收件人。
这就是邮件发送的一个完整流程。
**4. 简单使用**
最早期的时候使用 JavaMail 相关 API 来开发，需要自己去封装消息体，代码量比较庞大；
后来 Spring 推出了 JavaMailSender 简化了邮件发送过程，JavaMailSender 提供了强大的邮件发送功能，可支持各种类型的邮件发送。
现在 Spring Boot 在 JavaMailSender 的基础上又进行了封装，就有了现在的 spring-boot-starter-mail，让邮件发送流程更加简洁和完善。
下面给大家介绍如何使用 Spring Boot 发送邮件。
#### <1> pom 包配置
引入加 spring-boot-starter-mail 依赖包：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOExcGzLibHaDQDwiaPTIDGgHQlJicfic9dib7RBiagLusXWnS5ibZ6Ua1Il9uA/640?wx_fmt=png)
#### <2> 配置文件
在 application.properties 中添加邮箱配置，不同的邮箱参数稍有不同，下面列举几个常用邮箱配置：
163 邮箱配置：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOTj2ITINq0aGsYQg45dicsL77xqaw4HlzuibzwrX2cfa3DKOzribCibZtHg/640?wx_fmt=png)
126 邮箱配置
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOxibEu1LhWpyIfuVhxBgpTdRia4gmagvoiay4jhqBZUSHu7viay64Vxmmgw/640?wx_fmt=png)
qq 邮箱配置如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKO9BUr1lyW3Npd5qJ57RL0tQmSDuldd1n2FajEBDgTMUQlMZkD24jXvw/640?wx_fmt=png)
> 
**注意：**测试时需要将 spring.mail.username 和 spring.mail.password 改成自己邮箱对应的登录名和密码，这里的密码不是邮箱的登录密码，是开启 POP3 之后设置的客户端授权密码。
这里以 126 为邮件举例，有两个地方需要邮箱中设置：
开启 POP3/SMTP 服务、IMAP/SMTP 服务
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOBIu9ibFCohm8icQNVib5ADydmK0353DmTeQ50E3FDmibBpXzPicZXygBDAg/640?wx_fmt=png)
图片下方会有 smtp 等相关信息的配置提示。
开通设置客户端授权密码
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOqUckC19gIOA8787yqb5fJNUIOXHxVy6Yggh1VicozDT98v8r9bOGWow/640?wx_fmt=png)
设置客户端授权密码一般需要手机验证码验证。
#### <3> 文本邮件发送
Spring 已经帮我们内置了 JavaMailSender，直接在项目中引用即可。我们封装一个 MailService 类来实现普通的邮件发送方法。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOibJj7d00Ay6rgzia2fN0UPLC8tYKwfXXSQWz9kN59tf4icHdY47iaLvpug/640?wx_fmt=png)
> 
文本邮件抄送使用：message.copyTo(copyTo) 来实现。
- 
from，即为邮件发送者，一般设置在配置文件中
- 
to，邮件接收者，此参数可以为数组，同时发送多人
- 
subject，邮件主题
- 
content，邮件的主体
邮件发送者 from 一般采用固定的形式写到配置文件中。
#### <4> 编写 test 类进行测试
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKO5aobdVZVh0vFLjOGIHmrsroJMS2hT6YbtBxjjyicNQuoIMjFdU4CjBA/640?wx_fmt=png)
稍微等待几秒，就可以在邮箱中找到此邮件内容了。至此一个简单的文本邮件发送就完成了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOvKywKt6x3xT3FjGClvUn4CzibUqekIVnfFicERIKh8aXnQvlt9KibWSgQ/640?wx_fmt=png)
### **5. 富文本邮件**
在日常使用的过程中，我们通常在邮件中加入图片或者附件来丰富邮件的内容，下面讲介绍如何使用 Spring Boot 来发送富文本邮件。
#### 发送 HTML 格式邮件
邮件发送支持以 HTML 语法去构建自定义的邮件格式，Spring Boot 支持使用 HTML 发送邮件。
我们在 MailService 中添加支持 HTML 邮件发送的方法.
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKO6t6GZtRdqws466XZIrgdglpicbogPc1SrfEBhRYECjSs8pDWrgvFianQ/640?wx_fmt=png)
> 
富文本邮件抄送使用：helper.addCc(cc) 来实现。
和文本邮件发送代码对比，富文本邮件发送使用 MimeMessageHelper 类。MimeMessageHelper 支持发送复杂邮件模板，支持文本、附件、HTML、图片等，接下来会一一使用到。
在测试类中构建 HTML 内容，测试发送
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOw2ZbUqXxHKILxn67acOMHa5JXsb6x1HLrEibDc2gJ4E8oyC1ECHEmbw/640?wx_fmt=png)
邮件内容大写了一段话，下面为接收到的效果：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOvO4BBvK4bEbfPVtXtszqdP4cdhgVsHZqGLcjiaSKMlDPgL1EWbbTxibQ/640?wx_fmt=png)
由此我们发现发送 HTML 邮件，就是需要拼接一段 HTML 的 String 字符串交给 MimeMessageHelper 来处理，最后由邮件客户端负责渲染显示内容。
#### 发送带附件的邮件
在 MailService 添加 sendAttachmentsMail 方法，发送带附件的邮件主要是使用 FileSystemResource 对文件进行封装，在添加到 MimeMessageHelper 中。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOFdZj2ZuEgo2rcQhbjf4REzVm4QRJQlibUiarlKCpwMb3WXBT9WIO9syA/640?wx_fmt=png)
> 
添加多个附件可以使用多条 helper.addAttachment(fileName, file)
在测试类中添加测试方法
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOZkt38z8LJo7m9IezwicEzX3qcWVKXIibc07Dqs6WK8kkFRiaicCAag97kA/640?wx_fmt=png)
> 
附件可以是图片、压缩包、Word 等任何文件，但是邮件厂商一般都会对附件大小有限制，太大的附件建议使用网盘上传后，在邮件中给出链接。
效果图如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOKr7iciaVxibs80OfpBRPVpCEdhbnOzJCvmOEYMSuGXickv6x5RD4hSnhWw/640?wx_fmt=png)
#### 发送带静态资源的邮件
邮件中的静态资源一般指图片，在 MailService 添加 sendInlineResourceMail 方法。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOrFFWTEhxIO5O6ia2V4uo7UfRky1knlGrBuRA3RFic5PFDuBk5zHsOIxw/640?wx_fmt=png)
在测试类中添加测试方法
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKO1YT0SuzehXibsSScMYxFCvibJDCB7ShwsGyoC031POrASjBvFGYxLczA/640?wx_fmt=png)
添加多个图片可以使用多条 <img src='cid:" + rscId + "' > 和helper.addInline(rscId, res) 来实现
效果图如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOLQabr2KIDCf21DpDHO2hcypHCpibicTvzIVwfQs44QOa3yUcCHGNJWqQ/640?wx_fmt=png)
以上是邮件发送的基础服务，已演示支持各种类型邮件。
### **6. 邮件系统**
如果只是想在系统中做一个邮件工具类的话，以上的内容基本就可以满足要求了。要做成一个邮件系统的话还需要考虑以下几方面：
- 
对外提供发送邮件的服务接口
- 
固定格式邮件是否考虑使用模板
- 
发送邮件时出现网络错误，是否考虑适当的重试机制
- 
邮件系统是否考虑异步化，提升服务响应时间
- 
是否开发邮件后台管理系统，开发出对应的管理软件，通过页面发送邮件，统计发送邮件成功率等数据。
- 
常见异常处理措施
#### 6.1 对外提供接口
作为一个独立的邮件系统，需要对外提供接口调用，我们以简单文本邮件为例做个演示：
首先需要定义个实例返回对象：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKO7AAwvAOGdAVFHSIAKrs5j9qzoiadMB3EMiaiaZRgJJhqMviagnuynO8gUg/640?wx_fmt=png)
默认成功的返回码为：00，返回消息为：发送成功。
创建一个 MailController 类对外提供 HTTP 请求接口。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOVMfUfEq3Qc9W7frG2PJJLcDUO8Zh020xVJ9wMjgNibOTn2EJNB3QWAw/640?wx_fmt=png)
外部请求过来时首先进行参数校验，如果参数有误返回请求；发送邮件出现异常时返回错误，正常情况下返回 00；
注意在 Service 层如果对异常信息进行了捕获的话，需要将异常信息抛到上层。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOSpIbmVJVu6rQk8a2DnWa2LFFsjDhfGZ5zKLjoqPdAABoyy7Iib4Yhzw/640?wx_fmt=png)
类似上述代码。
按照这个思路也可以提供发送带图片、带附件的邮件，同时也可以封装发送多人邮件，群发邮件等复杂情况。
#### 6.2 邮件模板
通常我们使用邮件发送服务的时候，都会有一些固定的场景，比如重置密码、注册确认等，给每个用户发送的内容可能只有小部分是变化的。
所以，很多时候我们会使用模板引擎来为各类邮件设置成模板，这样我们只需要在发送时去替换变化部分的参数即可。
我们会经常收到这样的邮件：
> 
**尊敬的 neo 用户：**
恭喜您注册成为 xxx 网的用户,同时感谢您对 xxx 的关注与支持并欢迎您使用 xx 的产品与服务。
** ...**
邮件正文只有 neo 这个用户名在变化，邮件其它内容均不变，如果每次发送邮件都需拼接 HTML 代码，程序不够优雅，并且每次邮件正文有变化都需修改代码非常不方便。
因此对于这类邮件，都建议做成邮件模板来处理，模板的本质很简单，就是在模板中替换变化的参数，转换为 HTML 字符串即可，这里以 Thymeleaf 为例来演示。
Thymeleaf 是 Spring 官方推荐的前端模板引擎，类似 Velocity、FreeMarker 等模板引擎，相较与其他的模板引擎，Thymeleaf 开箱即用的特性。
它提供标准和 Spring 标准两种方言，可以直接套用模板实现 JSTL、 OGNL 表达式效果，避免每天套模板、该 Jstl、改标签的困扰。
Thymeleaf 在有网络和无网络的环境下皆可运行，即它可以让美工在浏览器查看页面的静态效果，也可以让程序员在服务器查看带数据的动态页面效果。
**下面我们来演示使用 Thymeleaf 制作邮件模板:**
1. 添加依赖包
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKObOe4weG5DCiaSPDN8MQ5yvFe8yw3eJIq7WKgEHFEkY6RgNpBO7DxVpQ/640?wx_fmt=png)
2. 在 resorces/templates 下创建 emailTemplate.html
emailTemplate.html 文件内容即为邮件的正文内容模板。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOwDn7QrBQSydn60EYHFjFKF2U9YzYO1ibub3Kfibt602VQm0cicvCcicibRQ/640?wx_fmt=png)
我们发现上述的模板中只有 id 是一个动态的值，发送过程中会根据传入的 id 值来替换链接中的 {id}。
3. 解析模板并发送
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOCUPse2ttsyhGdzoicwqth3enKlib9gwHJaTlIkeS4wPMMXys1EJwrnzw/640?wx_fmt=png)
我们发现最后调用的还是 sendHtmlMail 的方法，邮件模板的作用只是处理 HTML 生成部分，通过 Thymeleaf 模板引擎解析固定的模板，再更具参数来动态替换其中的变量，最后通过前面的 HTML 发送的方法发送邮件。
效果图如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XNHtp78HygpjWuZf4MpcxwdEF4yGwMKOMr3apWQ9Zzgu1R69IoRTbeQyjDpS7SUbamjIOeDXQaRmZmsGIqYrwQ/640?wx_fmt=png)
点击“激活账号”跳转的链接为：http://www.ityouknow.com/register/006
#### 6.3 发送失败
因为各种原因，总会有邮件发送失败的情况，比如：邮件发送过于频繁、网络异常等。在出现这种情况的时候，我们一般会考虑重新重试发送邮件，会分为以下几个步骤来实现：
- 
接收到发送邮件请求，首先记录请求并且入库。
- 
调用邮件发送接口发送邮件，并且将发送结果记录入库。
- 
启动定时系统扫描时间段内，未发送成功并且重试次数小于3次的邮件，进行再次发送.
- 
重新发送邮件的时间，建议以 2 的次方间隔时间，比如：2、4、8、16 ...
常见的错误返回码：
- 
421 HL:ICC 该IP同时并发连接数过大，超过了网易的限制，被临时禁止连接。
- 
451 Requested mail action not taken: too much fail authentication 登录失败次数过多，被临时禁止登录。请检查密码与帐号验证设置
- 
553 authentication is required，密码配置不正确
- 
554 DT:SPM 发送的邮件内容包含了未被许可的信息，或被系统识别为垃圾邮件。请检查是否有用户发送病毒或者垃圾邮件；
- 
550 Invalid User 请求的用户不存在
- 
554 MI:STC 发件人当天内累计邮件数量超过限制，当天不再接受该发件人的投信。
如果使用一个邮箱频繁发送相同内容邮件，也会被认定为垃圾邮件，报 554 DT:SPM 错误
如果使用网易邮箱可以查看这里的提示：企业退信的常见问题？
6.4 异步发送
很多时候邮件发送并不是主业务必须关注的结果，比如通知类、提醒类的业务可以允许延时或者失败。这个时候可以采用异步的方式来发送邮件，加快主交易执行速度。
在实际项目中可以采用消息中间件 MQ 发送邮件，具体做法是创建一个邮件发送的消息队列，在业务中有需要用到邮件发送功能时，给对应消息队列按照规定参数发送一条消息，邮件系统监听此队列，当有消息过来时，处理邮件发送的逻辑。
6.5 管理后台
考虑做一个完善的邮件系统，可以设计一个独立的邮件管理后台，不但可以让系统之间调用时使用，也可以提供图形化界面让公司的运营、市场部的同事来发送邮件，查询邮件的发送进度，统计邮件发送成功率。
也可以设置一些代码钩子，统计用户点击固定链接次数，方便公司营销人员监控邮件营销转化率。
一个非常完善的邮件系统需要考虑的因素非常多，比如是否设置白名单、黑名单来做邮件接收人的过滤机制，是否给用户提供邮件退订的接口等。
因此在初期邮件发送的基本功能完成之后，再结合公司业务，快速迭代的逐步完善邮件系统，是一个推荐的做法。
### **7. 总结**
使用 Spring Boot 集成发送邮件的功能非常简单，只需要简单编码就可以实现发送普通文本邮件、带附件邮件、HTML 格式邮件、带图片邮件等。
如果需要做成一个邮件系统还需要考虑很多因素，比如：邮箱发送失败重试机制、防止邮件被识别为垃圾邮件，固定时间内发送邮件的限制等。
在微服务架构中，常常将一些基础功能下沉下来，作为独立的服务来使用，邮件系统作为平台的基础功能，特别适合做为独立的微服务来支持整个系统。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqOY2QywN4xtHKRUMpfmNxqwP0zykiak6TzmVcrxItVRkmib72DKlpB1FKv9FrcG4Pw4HGWat3GhwtQ/640?wx_fmt=jpeg)
