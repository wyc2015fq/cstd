# J2EE—JAF和JavaMail - Big Smile - CSDN博客
2017年02月14日 20:52:59[王啸tr1912](https://me.csdn.net/tr1912)阅读数：532
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        今天终于是J2EE的最后两个规范了，也是最有联系的两个规范，让我们一起来看看他们吧。
# 一、JAF
        JAF是一个专用的数据处理框架，它用于封装数据，并为应用程序提供访问和操作数据的接口。JAF的主要作用在于让java应用程序知道如何对一个数据源进行查看、编辑和打印等操作。
        Mail API 的所有版本都需要 JavaBeans Activation Framework 来支持任意数据块的输入及相应处理。功能似乎不多，但目前许多浏览器和邮件工具中都能找到这种基本的 MIME 型支持。activation.jar 文件就是JAF的框架jar包。
        对于通过JAF封装的数据，应用程序通过JAF提供的接口可以完成如下功能：
        1、访问数据源中的数据.
        2、获知数据源的数据类型.
        3、获知可对数据进行的各种操作.
        4、用户对数据执行某种操作时，自动创建执行该操作的软件部件的实例对象.
        javaMail API可以利用JAF从某种数据源中读取数据和获知数据的MIME类型，并用这些数据生成MIME消息中的消息体 和消息类型。
# 二、JavaMail
         JavaMail，顾名思义，提供给开发者处理电子邮件相关的编程接口。它是Sun发布的用来处理email的API。它可以方便地执行一些常用的邮件传输。我们可以基于JavaMail开发出类似于Microsoft Outlook的应用程序。
         JavaMail是SUN提供给开发人员在应用程序中实现邮件发送和接收功能而提供的一套标准开发类库，支持常用的邮件协议，如SMTP、POP3、IMAP，开发人员使用JavaMail编写邮件程序时，无需考虑底层的通信细节(Socket)，JavaMail也提供了能够创建出各种复杂MIME格式的邮件内容的API。使用JavaMail，我们可以实现类似OutLook、FoxMail的软件。虽然JavaMail(仅支持JDK4及以上)也是Java的API之一，但是却没有直接加入到JDK中，所以我们需要另行下载。另外，JavaMail依赖JAF(JavaBeans
 Activation Framework)，JAF在Java6之后已经合并到JDK中，而JDK5之前需要另外下载JAF的类库。
# 三、思考
        从JavaMail中的需要和API通信方式上来理解，其实我们最后这两个模式，其实是有很紧密的联系的，JavaMail是通过JAF来进行数据的接收和发送的，这里体现了JAF的数据处理的作用，JavaMail把信息的请求抛给JAF，然后再把接收的需求給JAF，那么JAF就是一个Mail的对外的接收和发送的一个处理机制。在使用JAF来处理数据的时候要注意的是MIME类型的数据是非常重要的。
