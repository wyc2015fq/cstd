# SQL Server与SQL Server Express的区别 - xqhrs232的专栏 - CSDN博客
2017年10月18日 09:10:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6603
原文地址::[http://www.cnblogs.com/jerry01/articles/1614859.html](http://www.cnblogs.com/jerry01/articles/1614859.html)
转自：[http://blog.csdn.net/Helling/archive/2009/10/19/4696705.aspx](http://blog.csdn.net/Helling/archive/2009/10/19/4696705.aspx)
SQL Server Express 2005（以下简称 SQLExpress） 是由微软公司开发的 SQL Server 2005（以下简称 SQL2005）的缩减版，这个版本是免费的，它继承了 SQL Server 2005 的多数功能与特性，如：安全性设置、自定义函数和过程、Transact-SQL、SQL、CLR 等，还免费提供了和它配套的管理软件 SQL Server Management Studio Express 。
    SQLExpress 有几个主要的限制：
    1. 仅允许本地连接。
    2. 数据库文件的最大尺寸为4GB，此限制只对数据文件（后缀名为 mdf），日志文件（后缀名为 ldf）不受此限。 
    3. 只使用一个CPU来运算，不能充分利用多CPU服务器的性能。 
    4. 可使用的记忆体量最高只有1GB。 
    5. 没有 SQL Agent，若要做排程服务必须自己编写。
    因此它是 SQL Server 产品系列中面对低端的产品，是面对桌面型应用，或者小型的内部网络应用的。
    所谓面对桌面型应用的软件，是指单用户在一台电脑就能完成操作的软件，强调操作指令和数据处理一体化，不考虑多用户共享权限和并发处理问题。与此相对的是客户/服务器（或浏览器/服务器）模式软件，SQL2005 就是 客户/服务器应用模式中的服务器端软件，SQL2005 只能管理附加（Attach）到服务器实例内的数据库文件（mdf文件），而此数据库文件也唯一归属于该SQL2005实例，不能被其它程序调用（当然，也不能被客户程序直接调用），必须由SQL2005实例执行分离（Dettach）该数据库文件后，才能由其它程序调用。
    为了适应桌面应用，SQLExpress 比 SQL2005 新增了一项标志性的功能：不需将外置的数据库附加到 SQLExpress 服务器中，就能够直接调用，即在连接数据库语句中增加了 AttachDBFilename 选项。有此功能后，我们用 Microsoft Visual Studio 设计桌面型应用程序时，使用Access数据库或SQLExpress数据库的方式几乎相同，由于SQLExpress数据库的功能更多、扩展性更强，而且是嵌入到 Visual Studio 软件中的默认数据库，相关帮助文档更多，在编程中使用SQLExpress数据库甚至比Access更方便。唯一缺点是包含SQLExpress的软件尺寸较大，与使用Access数据库的软件相比，尺寸约大40兆，但现在的电脑硬盘容量有上百G，几十兆已不是问题。
    从桌面数据库的发展过程来看，Foxpro 曾经很受软件开发者喜爱，后来微软公司开发的 VB、C# 等编程语言主推Access做桌面数据库，使得 Foxpro 逐渐衰落。当今 Internet 网络快速普及，纯桌面数据管理软件已经落伍，取而代之的是带有网络功能的客户/服务器（C/S）软件，或浏览器/服务器（B/S）软件，与之对应编程语言就是 Visual Studio 。使用 Visual Studio ＋ SQLExpress 编写的面对桌面的数据管理软件，能够很容易升迁到客户/服务器应用（甚至就是二者兼备），有此推断，SQLExpress
 也将逐渐替代 Access 。
    目前，网络服务器和虚拟主机服务商提供的数据库产品主要就是 SQL Server 2000－2008 和 Access ，几乎没有 SQLExpress 。这是因为 SQLExpress 只能使用1GB内存，和一个CPU，在多用户共享应用时，随着用户增多，效能下降的很快，这个缺点与Access类似（也可能比Access强点有限）。由于SQLExpress和SQL2005管理的是同一种数据库文件，如果软件编程得当，就应该二者兼备。因此，在有SQL2005的服务器上，就没有必要再应用SQLExpress了。作为特例，在小型企业内部网中，由于SQL2005的购置费较高，也可使用SQLExpress作为客户
 端/服务器系统中的服务器端软件。
    如果要编写兼容SQLExpress和SQL2005的软件，在编程时只需将连接数据库字符串抽离出来， 用自定义函数来替代（该函数的返回值等于“连接数据库字符串”），就可以了。
    我公司编写的“利和信息管理系统”软件，是面对桌面应用、客户/服务器应用，及浏览器/服务器应用于一身的软件，管理本机数据的后台数据库就是SQLExpress ，同时也支持调用Web网站SQL2005数据库。

