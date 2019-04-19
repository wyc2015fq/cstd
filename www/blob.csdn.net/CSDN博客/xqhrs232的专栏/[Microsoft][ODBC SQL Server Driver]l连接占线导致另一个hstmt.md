# [Microsoft][ODBC SQL Server Driver]l连接占线导致另一个hstmt - xqhrs232的专栏 - CSDN博客
2018年08月07日 17:45:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：107
原文地址::[https://blog.csdn.net/cb168/article/details/7693131](https://blog.csdn.net/cb168/article/details/7693131)
不要隨隨便便 在 SELECT、UPDATA、INSERT 、DELETE 的execsql;
OPEN、ACTIVE：=TRUE 前 加Prepare;特别是这几个组合，要少加。
这第五点这个方法能减少90%出现的机率。原因是DELPHI运行太快了导致了你数据
库前一个PREPARE没有释放时又产生一个，SQL Server ODBC driver 只允许一
个活动的hstmt,记住这个问题产生的原因是因为DELPHI 运行太快了。太快的东西
，太好的，太优秀的（DELPHI）和不好的东西（SQL）会产生矛盾。
六、这一点很重要，尽量采用专业的服务器+SISC 硬盘+大内存，不要随随便便用一台机器
装一下NT就是服务器了，很多问题正是它引起的（事实上微软让hstmt尽快释放，而是你的
服务器它不行。碰到这个问题时，当你整理一下硬盘，机率少一点，但不久就会现次出现
屏蔽了存储过程控件的Prepare正常！
