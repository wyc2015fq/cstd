# SQL SERVER 2008过了评估期 - 左直拳的马桶_日用桶 - CSDN博客
2012年05月24日 17:02:36[左直拳](https://me.csdn.net/leftfist)阅读数：1875
SQL SERVER 2008过了评估期之后，MS SQLSERVER服务不能启动。最简单的办法是将系统日期往回改，这样子就可以启动。
但对于数据库来讲，日期不对，弊端很多吧，敢用吗？
网上解决这种问题的文章很多，就是拿一个有效的产品序列号（所谓的产品密钥）输进去。方法如下：
1、开始 - 所有程序 - Microsoft Sql Server 2008 - 配置工具 - SQL SERVER安装中心
2、点维护
3、版本升级
4、输入产品序列号
从网上获得一个：JD8Y6-HQG69-P9H84-XDTPG-34MBB（仅作试验用途）
有的安装包默认自带了产品系列号，可以在安装包里面找到。
\X86\DefaultSetup.ini（32位）
\X64\DefaultSetup.ini（64位）
OK，可以启动实例。
但好像Sql Server ManageMent Studio不能启动，还是提示评估期过期。娘的，要改注册表：
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SQL Server\100\ConfigurationState 裡的 CommonFiles 值，改成 3
然后再升级一次。
此前接触一个交易系统，是另外一家公司开发的，服务器上的数据库也是他们安装。前阵子因为各种原因项目中止，服务器上的数据库居然就过了评估期了！好几台服务器啊，全部齐刷刷地一起过期，也太蹊跷了吧。并且有效日期就是到项目中止日那天截止！
应当是这家公司的设置。
可是他们怎么办到的呢？想不明白。难道还可以指定过期日期的？
