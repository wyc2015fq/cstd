# 更改AD中已有的Exchange邮件组织名 - weixin_33985507的博客 - CSDN博客
2009年11月14日 15:39:47[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
    我们都知道，在Exchange安装之前，需要对AD进行一些准备工作，也就是做Schema的扩展。无所谓是在DC还是在Exchange服务器上，安装所使用的账户只要在AD的Schema Admins这个组里就可以。不过有些时候事情真的不是那么顺的，我这次就碰上了。
    按道理说公司以前没有做过建过Exchange服务器，AD里是不会有相关信息的。但是今天我在扩展Schema的时候报了这样一个错，看下图：
![](http://bisheng.blog.51cto.com/attachment/200911/200911141258184444846.jpg)
    上图说已存在名为“First Organization”的组织，而我要配置的名为“Tiens”的组织就无法配置，并且所有后续配置也全部停止。
    “First Organization”，我记得好像是03还是07的Exchange上默认的就是这个名字，看来以前可能是做过测试，至少做过AD的Schema扩展。当然，现在没有必要去追究这个，现在的首要任务就是要将这个已有的名字改过来。
    这里需要用到一个工具——“Microsoft Exchange Server LegacyDN Utility”，可以在微软的网站上下载到：[http://www.microsoft.com/downloads/details.aspx?familyid=5ef7786b-a699-4aad-b104-bf9de3f473e5&displaylang=en](http://www.microsoft.com/downloads/details.aspx?familyid=5ef7786b-a699-4aad-b104-bf9de3f473e5&displaylang=en)
    这个工具还能修改一些其他由Exchange在AD中生成的数据，有兴趣的朋友自己研究研究吧。这里，我主要通过截图来展示一下如何通过它来修改组织名。
    把这个工具下载后解压到Exchange服务器上，可以看到三个文件，下图中第4个是日志文件，运行以后才生成的。
![](http://bisheng.blog.51cto.com/attachment/200911/200911141258184460702.jpg)
双击它运行，会出现提示信息，和身份验证信息。
![](http://bisheng.blog.51cto.com/attachment/200911/200911141258184478334.jpg)
![](http://bisheng.blog.51cto.com/attachment/200911/200911141258184490837.jpg)
    打开以后，我们选中一条记录，在下面的三个配置框内就会自动显示对应的值。但是仔细一看，所有“change”的按钮都是灰色的，左下角也写了个“read only mode”
![](http://bisheng.blog.51cto.com/attachment/200911/200911141258184502406.jpg)
    查了查手册，原来双击运行，默认是以只读方式打开，可能是考虑到安全问题吧。如果需要以可写方式打开，我们需要通过命令行运行，并且要附加运行参数。
![](http://bisheng.blog.51cto.com/attachment/200911/200911141258184517062.jpg)
    再次打开以后，可以看到左下角写着“启用强行写入|读写模式”，并且所有“change”的按钮也都变成可用了。这时再按照我们的需要修改对应的值就可以了。
![](http://bisheng.blog.51cto.com/attachment/200911/200911141258184527525.jpg)
    再次运行Schema扩展的命令，一切OK了。
    这里声明一下，我是在确认AD中没有任何Exchange服务器的情况下，进行的组织名的变更，所以组织名变更不会对现在的Exchange系统造成任何的影响。
    但是，如果AD中已经部署了Exchange组织，甚至是投入到正式业务环境中的情况下。。一定一定要小心行事，更改组织名可能产生很多问题。即使真的有必要，也要参照TechNet Library里面有关Modify的章节做好相应的配置准备。
    工具仅仅只是工具，必要性、影响程度、风险性都是必须由人来控制的。
