# ODP.net 及System.Data.OracleClient使用二三事 - weixin_33985507的博客 - CSDN博客
2012年11月21日 11:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
![](https://img-my.csdn.net/uploads/201211/21/1353468727_8141.png)
    最近要用到Oracle数据库连接，于是google了一下。“由于微软在.netframework4中会将System.Data.OracleClient.dll deprecated，而且就访问效率和速度而言，System.Data.OracleClient.dll与Oracle.DataAccess.dll相比，微软的确实没有oracle提供的类库有优势，所以我放弃了使用多年的System.Data.OracleClient.dll，取而代之的是odp.net。”这是网上流行的说法（参见[此篇博文](http://www.cnblogs.com/codingsilence/archive/2011/07/14/2146498.html)和[这篇文章](http://www.360doc.com/content/12/1116/15/11104439_248216066.shtml)），于是乎改弦更张，研究了一下odp.net。真是不用不知道，一用吓一跳。也许odp.net有种种优势，可是对于我的应用需求却显得晦涩难用，毫无优点可言。
    ODP.net的第一大优点：性能。这个我的应用对性能不太敏感，但是要求适应性强，比如是否安装了Oracle Client软件，安装了oracle client的不同版本都可以运行。运行时出现问题简直就是使用odp.net的噩梦，不如著名的和Oracle Client 版本不匹配。如果应用的目的环境很明确，我们还可以使用适当的odp.net版本来编译程序，可是如果环境未知呢？
    下面说说我使用odp.net的遭遇。首先，从System.Data.OracleClient迁移到Oracle.DataAccess.Client的过程还是比较愉悦的（我使用的版本是最新的2.112.1.0版本ODP），更换了一下using基本上就搞定了，对象名称基本保持了一致。由于直接使用了类似于Tnsnames.ora的连接方式（连接串如下），只要配置了服务器地址和端口以及oracle的服务实例名就可以了，所以在开发机上连接两台oracle服务器非常顺利。
![](https://img-my.csdn.net/uploads/201211/21/1353468733_4447.png)
本地试验成功后，接下来是第一台测试机，xp系统未安装oracle client，按照网上的文章，在运行程序目录中放置了以下几个dll，结果顺利连接oracle服务。
![](https://img-my.csdn.net/uploads/201211/21/1353468737_3547.png)
看来如果确认目标机器上没有Oracle软件，使用这种方法还是很不错的。不过不要小看这几个dll，总大小有130多M。
第二台测试机，windows2003，安装的Oracle 10gV2，运行程序，报错Odp和oracle client版本不匹配，同样放置上述文件，问题依旧。其他方法如设置Dll path参数，设置ORACLE_HOME及TNS_ADMIN环境变量等无一可行。不过在老外的一个网站上看到使用Assembly重定向方法，让我看到了希望的曙光，在app config文件中添加如下内容：
![](https://img-my.csdn.net/uploads/201211/21/1353468742_4038.png)
按照这样的配置方法，据老外帖子说有人成功了，可是在我的机器上改报这样的错误“Specified Slot number was not valid”，google了一下，据说是引用的目标dll版本不一致造成的，使用多种手段均未能解决，我想可能是因为我环境中是安装的Oracle的服务而不是单独的Client，如果仅仅是客户端有可能可以成功，但是我没有测试，我已经不想测试了，因为对于我这也是不可接受的！而且因为在搜索资料过程中我发现在System.Data.OracleClient的连接串中有这样的说明：Server（或者Data
 Source）项目可以指定Tnsname 或是 Oracle的服务实例名，这让我又有了改回微软的想法。
说干咱就干，开发环境中试验了一下，直接使用Oracle实例名连接居然真的成功了，看来使用微软的驱动也可以不配置Tns了，Great！继续开始在测试机上试验。首先XP上，同样使用那几个Dll，测试通过！接着2003服务器上，带着这几个dll，运行报错“System.Data.OracleClient requires Oracle client software version8.1.7 or greater.”，竟然报这样的错误，见鬼了。删掉除了“Oracle.DataAccess.dll”和“OraOps11w.dll”之外的另外三个dll，再试，OK了！^_^
看来对于我的需求来说，还是微软的驱动好用啊。以上就是我使用.net连接oracle的一些经验，希望对您有所提示。
