# 高效 Windows 工作环境 && Java 开发环境搭建 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [theboboy](http://www.jobbole.com/members/theboboy) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
首先，感谢那篇《[高效 MacBook 工作环境配置](http://blog.jobbole.com/89013/)》，让我能在第一次使用 MacBook 时顺利的适应。
时过境迁，换了公司之后，重新换回Windows，新机器，新系统。由于Windows中没有“绿色/免费/汉化/Cxxxk”的“市场”应用，决定基于Windows环境写一篇工作中常用的软件整理。因为是做Java的，所以会把编程和Java相关的软件也带上，非Java猿可忽略这一块。
随着科学技术的提高，真正提高的并不是人类本身，而是各式各样的工具，正如诺基亚的那句广告“科技以懒人为本”。Windows不同于OSX的就是软件/应用数量非常多，因此本文中推荐的不一定是最好的，具体要看个人的使用习惯。也希望有个人觉得好的软件，同类或者补充的，能反馈给作者。
工欲善其事，必先利其器。作者认为任何方面的工具都没有最好，只有最适合，只要利用得好，就是好工具。也不希望有什么vim和emacs之争，完全没必要把自己的想法强加于别人头上。
那么进入正题
## 一、硬件
首先，现在内存白菜价，对于普通用户来说，8G够用，对于我们猿类，应该是8G起步了吧。除此之外，作者建议使用机械键盘，并且推荐使用国产轴的，一是因为国产轴的相对便宜，并且够用，二是当买的人多了之后，国产的各个厂商一定会开始打价格战，届时一些大学生也就可以消费得起了，想当初作者大学时天天看机械键盘，就是买不起，唉。作者推荐使用青轴，虽然声音大，但是确实舒服。另外，假如有大家的U盘是新买的，建议格式化成exFAT格式，兼容Windows 7以上的系统和OSX。
最后，虽然Windows 10免费，还是建议大家抵住诱惑，使用Windos 7。PS：推荐一个Windows 7不激活，直接重装Windows10激活的方法：[不从Win7/Win8.1升级，直接全新安装并激活Win10方法](http://www.ithome.com/html/win10/172955.htm)。
## 二、系统
首先，Windows 7、8、8.1、10，先关闭烦人的UAC。具体步骤为：开始 -> 控制面板 -> 查看方式：大图标 ->用户账户 ->更改用户账户控制设置 ->从不通知 ->确定。
![](http://img.blog.csdn.net/20160526161654519)
![](http://img.blog.csdn.net/20160526162001848)
![](http://img.blog.csdn.net/20160526162007863)
现在市面上的笔记本大部分都是768p的，很坑，所以尽量让自己的工作空间变大，有一个普遍的做法是任务栏显示为小图标，具体方法为：任务栏上右键 -> 属性 ->使用小图标。
![](http://img.blog.csdn.net/20160526162058052)
![](http://img.blog.csdn.net/20160526162101958)
##  三、字体
上面的前三张截图是同事那里的，下面两张是本机的，明显本机的字体要更清晰，这是由于使用了MacType代替Windows做字体渲染。
具体介绍和下载请戳[MacType – 美化 Windows字体的神器！(让字体渲染显示效果比苹果电脑更清晰炫丽)](http://www.iplaysoft.com/mactype.html)。
需要注意的是，网上大部分给的版本都是v2013.1231.0，但是这个版本在一些地方会存在缺字问题，文字显示成一个小点。通过下载[MacType 1.2013.1231.0/1.2016.0216 Dev](http://down.tech.sina.com.cn/page/54565.html)版本，将里面的MacType.dll和MacType64.dll覆盖到MacType的安装目录中重启即可。
![](http://img.blog.csdn.net/20160526162107271)
PS：关于这个WinRAR的皮肤，大家请自行搜索DTU Theme。
如果读者和作者是同（猿）类，并且也做Web开发，相信用的浏览器一定是Chrome或则同内核的浏览器，因为这个浏览器的版本相当多，某些版本不会使用MacType渲染导致局部字体不和谐，解决方法请戳[解决MacType 在 Chrome 43+ 下缺字/粗细不一问题](http://www.jianshu.com/p/fdaa696a8eb4)。
## 四、浏览器
正如上面所说，作者用的是Chrome内核的浏览器，作者觉得这种浏览器主要的优势不是因为他的开发商牛，而是因为开放，拥有各种各样好用的插件。
当然，读者会想到，插件的网站无法访问。作者是个和谐的攻城狮，平常走（不）平（爬）路（墙），因此用了国产的基于Chorme的浏览器，几乎试过了所有的国产浏览器，经过无数次安装卸载，最终选用了360极速浏览器。这里推荐几款作者使用的插件。
- Scroll To Top Button 中文版，不是所有的网站都良心的在底部放置了返回顶部的按钮，所以使用了这个工具，它会在网页右侧生成一个返回顶部的按钮，当然也可以自定义位置。
![](http://img.blog.csdn.net/20160526162115411)
- VerticalTabs，标签开太多之后，就用这个来快速的导航。
![](http://img.blog.csdn.net/20160526162120114)
对于其它的工具，使用率不高，而且与“效率”的关系不大，因此不做介绍了。
## 五、应用偏系统
1、我们猿类，尤其是Java猿类，遇到没遇到过的Exception或者Error怎么办呢？当然是上网搜，可是爬虫这么发达，一些无良网站爬了很多知名网站的博客，还弄了一堆的广告，强烈鄙视这种行为。因此向大家推荐一款去广告的应用，ADSafe净网大师_强力有效的广告拦截器，是国产的，强烈支持！
![](http://img.blog.csdn.net/20160526162125786)
另外，这款应用已经有了IOS和[Android](http://lib.csdn.net/base/15)版本，Android版本觉得很好，IOS版之前内测时用的效果不是很好，希望这款应用可以越做越好。这种良心软件我们不支持它支持谁呢？
2、用惯了chrome，大家是不是也希望Windows自带的资源管理器（文件管理器）也能有那种tab呢？确实有，而且开发商也是我们中国的。作者突然想，其实中国的技术不是不强，而之所以一些应用做的没有某些国外的好是因为非技术问题，大家想想为什么Linux出现在芬兰，或许就和作者想到一起了。言归正传，这款软件是[Clover给资源管理器加上类似Chrome的标签](http://cn.ejie.me/)。
![](http://img.blog.csdn.net/20160526162129943)
需要注意的是这款应用pin到任务栏上图标为默认的三叶草图标，与Windows默认的资源管理器图标区别很大，所以推荐使用一个网友修改的图标替换三叶草图标。[http://ejie.me/images/Windows_Explorer_Clover.ico](http://ejie.me/images/Windows_Explorer_Clover.ico)，快捷方式的路径在C:\Users\你的用户名\AppData\Roaming\Microsoft\InternetExplorer\Quick Launch\User Pinned\TaskBar\Clover.lnk，替换完成之后重启资源管理器或者注销重新登录即可。
PS：快捷方式所在的文件夹是隐藏的，用下面的方法显示：
![](http://img.blog.csdn.net/20160526162134553)
![](http://img.blog.csdn.net/20160526162139052)
3、联调的那些天、上线前的几天、甚至是平时开发中，大家是不是忙到没有时间的概念，想不起来吃饭、喝水、甚至是上厕所。
有一方面因素是因为Windows上的时钟太小了，位置又太偏了，目测是五环外。这里推荐一个时钟工具[AtomicAlarmClock](http://drive-software.com/atomicalarmclock.html)。
![](http://img.blog.csdn.net/20160526162143818)
这款工具还自带了一些简单却实用的工具，比如提醒我们喝水的闹钟。
![](http://img.blog.csdn.net/20160526162148006)
![](http://img.blog.csdn.net/20160526162152755)
希望这个工具可以帮助我们猿类健康工作，健康生活。[下载地址请戳这里](http://pan.baidu.com/s/1hqtATDE)。
4、Windows下切换程序有两种方法，一种是Alt+Tab，另一种是Win+Tab。两种方式如下：
![](http://img.blog.csdn.net/20160526162157239)
Alt+Tab
![](http://img.blog.csdn.net/20160526162203333)
Win+Tab
但是作者按Alt+Tab是这样的：
![](http://img.blog.csdn.net/20160526162210710)
有木有觉得比上面的两种都直观？这款软件是[VistaSwitcher – SpeedUp Windows Alt-Tab](http://www.ntwind.com/software/vistaswitcher.html)。不只是有切换应用的功能，还有应用内部打开的多个文件之间切换功能，快捷键是Alt+~，就是Tab上面的那个键，效果如下：
![](http://img.blog.csdn.net/20160526162219476)
5、大家打开一款应用最常用的方式是什么呢？pin到任务栏 or Win+R or 其它快捷方式？这里给大家推荐一个快速启动器，可以快速启动你的应用。
![](http://img.blog.csdn.net/20160526162228536)
按Alt+空格就可以在任意位置调出这个框，输入后自动匹配应用，应用位置可以自行添加，这款启动器是[Launchy](http://www.launchy.net/)，截图和官网的不太一样是因为用了其它自带的皮肤。
不过默认的版本无法支持拼音快捷搜索，不过好在中国民间高手多，具体说明请戳[Launchy设置及中文修改版下载](http://www.aojgame.com/soft/446.html)。链接中的下载链接进不去，请大家自行搜索[Launchy设置及中文修改版下载](http://cn.bing.com/search?q=Launchy%E8%AE%BE%E7%BD%AE%E5%8F%8A%E4%B8%AD%E6%96%87%E4%BF%AE%E6%94%B9%E7%89%88%E4%B8%8B%E8%BD%BD)。
PS：平时使用必应搜索，所以搜索链接贴的是必应的，要是有人问百度一下你会死啊，请大家替我回答。
6、作为猿类，O2O带给大家最大的好处就是不用出门也能吃饭了吧。于是在工位上边看视频变吃饭也算是常态了，但是这样不好！！！即便是在座位上吃饭，也强烈建议大家下楼走走，晒太阳，吹风，和小伙伴们聊天吹牛侃大山，不要整天在办公室坐着！！！回头说一下吃饭，吃饭时往往是一只手操作电脑，另一只手拿着食物，这时鼠标和键盘的切换就不方便了，所以推荐大家一款鼠标手势软件，用国产Chrome内核的想必并不陌生，但是这款适用于任何操作界面。这款软件同样是我们中国人开发的，是个独立开发者，这里向他致敬！软件主页为[WGestures全局鼠标手势](http://www.yingdev.com/projects/wgestures)，具体操作请见主页的视频。
![](http://img.blog.csdn.net/20160526162233898)
默认带了全局的，文件管理器的，cmd的，图片查看的，下面那个是自定义的，很方便。
## 六、编程相关
1、大部分人线上的服务器环境应该都是Linux，大家对Linux的命令一定比Windows的更熟悉。Windows下某些参数必须要用双引号一直是作者的心头之痛，于是找到了一个在Windows中可以使用Linux命令的方法——[Cygwin,Get that Linuxfeeling – on Windows](https://cygwin.com/)。于是就有了各种爽。
![](http://img.blog.csdn.net/20160526162238740)
这个软件使用的是在线下载安装方式，网上也有离线的包，但是有点旧。用在线安装的方式下载比较慢，希望大家有点耐心，安装过程中有一个步骤是选择源的，建议大家选择中国的。
![](http://img.blog.csdn.net/20160526162245492)
2、猿们面试时是否遇到过一些Linux的面试题是关于“在一个文件夹下看哪些文件中包含一些关键字”这种问题，相信很多人都是用了grep。在这里不否认grep的强大以及为我们带来的好处，这里给大家介绍速度比grep快的工具[The Silver Searcher](http://blog.kowalczyk.info/software/the-silver-searcher-for-windows.html)。大家可以直接下载[exe版本](https://kjkpub.s3.amazonaws.com/software/the_silver_searcher/rel/0.29.1-1641/ag.exe)并直接放到path中，或者直接放到cygwin安装目录的bin文件夹下，因为之前这个文件夹已经在path中。
![](http://img.blog.csdn.net/20160526162250351)
命令很简单ag后边加具体要查找的内容，同样支持正则，最大的好处是比grep和ack要快。
3、上面两个介绍的都是命令相关的，想必大家看截图就要吐槽cmd吧，这货最大化之后贴着屏幕左侧，不到半屏。估计有不少人自己开了putty，用SecureCRT连本机，也比cmd好用。这里不折腾，推荐一款替代cmd的工具，先看截图吧。
![](http://img.blog.csdn.net/20160526162256615)
用这个来代替cmd非常好，默认的界面无需更改就很友好，官网请戳[Portable console emulator for Windows](http://cmder.net/)。
这款软件唯一的缺点是中文支持的不是太好，这里可以参考一位网友的[解决方案](https://blog.phpgao.com/cmder.html)。
PS：同时也推荐另一款类似的工具ConsoleZ，因为类似没有使用，详情可以查看这位网友的博客[windows下使用体验更好的控制台——ConsoleZ](http://www.cnblogs.com/fanyong/p/4036233.html)，在此感谢。
4、不知道大家是否发现了上面截图cmder和cmd的字体区别，个人喜欢[Microsoft Yahei Mono](http://download.csdn.net/detail/acvay/8517889)字体，这个可以还是看个人喜好的，个人觉得编程来说，只要是等宽字体就OK。
![](http://img.blog.csdn.net/20160526162505662)
5、由于Github上创建的项目必须是public的，想创建private项目要收费，Bitbucket在国内的访问速度又慢，而且有限制，所以想自己搭一套可以多人一起开发git项目的仓库。目前看来网盘类是最适合的，可以把网盘的同步文件夹设置成origin。因为种种原因，最终选择了[坚果云](https://www.jianguoyun.com/)。值得一题的是官网的QA页面就有相关的配置，链接在此：[你的github-通过坚果云管理您的代码](http://blog.jianguoyun.com/?p=321)。主界面中包含了一些个人信息，这里就截了一张官网的介绍，请见谅。
![](http://img.blog.csdn.net/20160526162511725)
6、大家都用什么git工具？是命令？还是eclipse或者IDEA的插件？还是TortoiseGit？这里向大家推荐一个软件SourceTree，不过官网的下载是无进度的，大家懂的。大家可以搜索。用过很多版本的，发现1.6版本的是最好用的，于是找了一个CSDN的[下载链接](http://download.csdn.net/detail/yeeeeeeeeee/8463839)。由于不是作者共享的，是否评论看个人。由于个人的截图有项目信息，懒得打码，这里使用了一个网友的截图，原图和介绍在[这里](http://blog.csdn.net/collonn/article/details/39259227)。
![](http://img.blog.csdn.net/20160526162517949)
另外，SourceTree自带的比较工具虽然比git diff好用，但还是不方便进行合并，这里推荐一个网友介绍的使用Beyond Compare作为SourceTree的外部比较工具，有冲突时可以点击外部差异比对（中文版）或者External Diff（英文版），打开Beyond Compare进行修复。具体信息请参考[sourcetree修改difftool 为 Beyond Compare 的方法](http://blog.csdn.net/w6611415/article/details/19755513)，该方法不足的是当出现冲突时，只能点击按钮才能用Beyond Compare，右键打开外部合并工具无效，但是第一种够用了。Beyond Compare借用了官方的一张图，由于软件不是免费的，请各位自行[搜索](http://cn.bing.com/search?q=beyond+compare)。
![](http://img.blog.csdn.net/20160526162525246)
## 七、Java相关
1、首先是数据库，相信绝大多数人用的是[MySQL](http://lib.csdn.net/base/14)。这里推荐大家的本地MySQL使用5.5版本，因为5.6以上Windows版本的安装方式包括界面变了，不能像之前一样安装后打开配置工具。
PS：如果忘记了打开配置工具，该工具默认在MySQL安装路径下的bin目录下，名为MySQLInstanceConfig.exe
![](http://img.blog.csdn.net/20160526162535319)
![](http://img.blog.csdn.net/20160526162541621)
配置的时候这里别忘了选这个，字符集是UTF-8。
装完MySQL Server，自然要有一个客户端，个人不推荐直接用命令行，因为不直观。这里只推荐一个作者常用的[SQLyog – Mostcomplete and easy to use MySQL GUI](https://www.webyog.com/product/sqlyog)。
![](http://img.blog.csdn.net/20160526162558497)
PS：提供一个CSDN的大牛共享的版本，请戳[这里](http://download.csdn.net/detail/shel_lee/7209183)。
当然数据库的GUI工具还有Navicat，用的人也很多，但是作者觉得SQLyog更好用，所以推荐了这个。除此之外，还有一个国产的也很好，免费并且支持多数据库，[Database.NET -直覺式多重資料庫系統管理](http://fishcodelib.com/database.htm)，提供一张官网的截图，也很好用。
![](http://img.blog.csdn.net/20160526162604731)
大家用Java的ORM和数据库时实体类都是用生成工具生成的吧？作者一直没有找到太适合的生成简洁代码的工具，于是乎，就用了个土办法，写存储过程简单实现字段的转换，分享给有代码简洁癖的猿。
下面这个是匈牙利命名法（some_columns）转驼峰（SomeClass or someFields）的函数：

MySQL
```
createdefiner=`root`@`localhost` function `to_camel`(src varchar(255), lowercaseboolean) returns varchar(255) charset utf8
begin
    declare temp varchar(255);
    declare buffer varchar(255);
    declare i int;
    declare lasti int;
    declare len int;
    set temp = concat(src, '_');
    set buffer = '';
    set i = 1;
    set lasti = 1;
    set len = length(temp);
    while i <= len do
        if substring(temp, i, 1) = '_' then
    set temp =concat(substring(temp, 1, lasti - 1), upper(substring(temp, lasti, 1)),substring(temp, lasti + 1));
    set buffer =concat(buffer, substring(temp, lasti, i - lasti));
    set lasti = i + 1;
        end if;
        set i = i + 1;
    end while;
    if lowercase then
        set buffer =concat(lower(substring(buffer, 1, 1)), substring(buffer, 2));
    end if;
    return buffer;
    end$ $
delimiter;
```
下面这个是db_type转JavaType的函数：

MySQL
```
createdefiner=`root`@`localhost` function `to_java_type`(sqltype varchar(255))returns varchar(255) charset utf8
begin
    declare javatype varchar(255);
    set javatype = '';
    case sqltype
        when 'bigint' then
            set javatype = 'double';
        when 'binary' then
            set javatype = 'int';
        when 'bit' then
            set javatype = 'bit';
        when 'blob' then
            set javatype = 'byte[]';
        when 'bool' then
            set javatype = 'boolean';
        when 'boolean' then
            set javatype = 'boolean';
        when 'char' then
            set javatype = 'String';
        when 'date' then
            set javatype = 'Date';
        when 'datetime' then
            set javatype = 'Date';
        when 'decimal' then
            set javatype = 'double';
        when 'double' then
            set javatype = 'double';
        when 'enum' then
            set javatype = 'Object';
        when 'float' then
            set javatype = 'float';
        when 'int' then
            set javatype = 'int';
        when 'longblog' then
            set javatype = 'byte[]';
        when 'longtext' then
            set javatype = 'String';
        when 'mediumblob' then
            set javatype = 'byte[]';
        when 'mediumint' then
            set javatype = 'int';
        when 'mediumtext' then
            set javatype = 'String';
        when 'numeric' then
            set javatype = 'double';
        when 'real' then
            set javatype = 'boolean';
        when 'set' then
            set javatype = 'Object';
        when 'smallint' then
            set javatype = 'int';
        when 'text' then
            set javatype = 'String';
        when 'time' then
            set javatype = 'Date';
        when 'timestamp' then
            set javatype = 'Date';
        when 'tinyblob' then
            set javatype = 'byte[]';
        when 'tinyint' then
            set javatype = 'int';
        when 'tinytext' then
            set javatype = 'String';
        when 'varbinary' then
            set javatype = 'int';
        when 'varchar' then
            set javatype = 'String';
        when 'year' then
            set javatype = 'Date';
    end case;
    if javatype = '' then
        set javatype = 'Object';
    end if;
    return javatype;
    end$ $
delimiter;
```
注意这里将tinyint、smallint、int都作为Integer，这样Java端好处理一些。
下面这个是将表转换成Java类（仅含熟悉）的存储过程，getters和setters需要自己生成：


```
createdefiner=`root`@`localhost` procedure `table_to_class`(tablename varchar(255))
begin
        select concat('public class ',to_camel(tablename, false), ' implements java.io.Serializable {')
        union all
        select concat('\tprivate ',to_java_type(data_type), ' ', to_camel(column_name, true), ';') frominformation_schema.columns where table_name = tablename
        union all
        select '}';
    end$ $
delimiter;
```
效果如下：
![](http://img.blog.csdn.net/20160526162609966)
PS：这里的JavaType用的是基本类型，因为不是用它生成实体，如果有需要改成包装类型需要自己改一下，很容易。
2、Java开发工具之争的两方一直是eclipse和IDEA，之前是eclipse和NetBeans，不建议使用vim或者emacs开发Java，IDE就是方便。作者使用的是[Eclipse IDE for](http://www.eclipse.org/downloads/packages/eclipse-ide-java-ee-developers/mars2)[Java EE](http://lib.csdn.net/base/17) Developers 4.5.2Mars 2版本。
![](http://img.blog.csdn.net/20160526162616544)
这里推荐大家装的几个插件。
（1）[findbugs](http://findbugs.sourceforge.net/)，代码静态分析工具，这里只能提供部分截图，还请见谅。
![](http://img.blog.csdn.net/20160526162622913)
PS：findbugs3.x只支持 JDK7及以上、Eclipse3.6及以上版本。具体操作请参考[代码检查工具系列——FindBugs](http://blog.csdn.net/wenwen091100304/article/details/50891065)和[Findbugs 缺陷详解与英文代号的对照表](http://blog.csdn.net/kuyuyingzi/article/details/38352723)。
（2）[Open Explorer](https://github.com/samsonw/OpenExplorer)，如下图所示，在工具栏增加了个按钮，可以快捷用Explorer打开选中文件所在路径，如果用的是Myeclipse，可以直接无视。
![](http://img.blog.csdn.net/20160526162630944)
（3）[JD-Eclipse](http://jd.benow.ca/)，别误会，不是京东，而是Java Decompiler。大家应该知道了，反编译的。
![](http://img.blog.csdn.net/20160526162637560)
（4）[JRebelfor Eclipse](http://marketplace.eclipse.org/content/jrebel-eclipse)，可以实现热部署。
![](http://img.blog.csdn.net/20160526162644451)
PS：用法和下载地址请参考[Eclipse/IntelliJIDEA添加JRebel 6.4.4以实现Tomcat热部署](https://blog.kuoruan.com/62.html)。
（5）[Zookeeper Browser](http://www.massedynamic.org/mediawiki/index.php?title=Eclipse_Plug-in_for_ZooKeeper)，个人觉得比ZooInspector好用，主要是不用来回切换窗口。
![](http://img.blog.csdn.net/20160526162649404)
PS：由于目前项目主页存在问题，具体请参考[Zookeeper Browser插件 for Eclipse](http://www.xing-rong.com/zookeeper-browser%E6%8F%92%E4%BB%B6-eclipse/)。
3、Java服务器猿大部分都是半个前端+半个DBA+半个运维。涉及到前端时，无论是eclipse还是Myeclipse都显得力不从心，所以这里推荐一个前端的编辑器[Sublime Text 3](http://www.sublimetext.com/3)，当然也可以编辑后端代码，只是没必要。不好意思，这里截图还是用的官网截图。
![](http://img.blog.csdn.net/20160526162655514)
不过引入新工具之后，自然要增加学习成本，不过呢，我们可以把所有的快捷键改成eclipse的快捷键，这样在很大程度上增加了工作效率。这里推荐几个插件，也算得上是必装的。
（1）[Package Control](https://packagecontrol.io/installation)，相当于eclipse的marketplace或者Maven的repository。
（2）装好Package Control并重启sumlime之后，按Ctrl+Shift+P呼出菜单。
![](http://img.blog.csdn.net/20160526162704413)
找到Package Control: Install Package，按回车稍等片刻。分别安装EncodingHelper、IMESupport、PackageResourseViewer，安装完成之后重新启动Sublime Text。
![](http://img.blog.csdn.net/20160526162711054)
还是按Ctrl+Shift+P呼出菜单，选择PackageResourseViewer: Open Resource。
![](http://img.blog.csdn.net/20160526162716648)
回车后找到Default。
![](http://img.blog.csdn.net/20160526162722116)
选择Default (Windows).sublime-keymap，这个就是默认的快捷键配置文件，强调一下，这个文件改动后立即生效，操作要慎重。
![](http://img.blog.csdn.net/20160526162732632)
这里参考了一个大神的[配置](https://github.com/ufologist/sync-software/blob/master/development/Sublime%20Text/Data/Packages/User/Default%20(Windows).sublime-keymap)，注意一点是不可以用这个直接覆盖，覆盖后会连Ctrl+Z这个快捷键都无法使用了，别问我是怎么知道的。
正确的做法是把“[”“]”之间的那些JSONObject拷贝到默认配置文件结尾的“]”前。
![](http://img.blog.csdn.net/20160526162739140)
### 八、总结
Windows下软件众多，上面这些工具也许不是同类中最好的，但却是最适合作者的，不同人用的工具也是不同的， 希望网友们可以提出一些不同的观点，如果大家觉得合适，会在本文及时更新。同时，文章中用到的链接如果有原作者不允许的，请告知，会在第一时间内取消链接。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/eac008a5ad6c8763c7b5d84ac3560bae.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/d1883dd90740e55db921de7eb1e42048.jpg)
