# 安装sql server遇到的古怪错误 - 左直拳的马桶_日用桶 - CSDN博客
2005年07月19日 19:18:00[左直拳](https://me.csdn.net/leftfist)阅读数：2367

有些错误的原因你真是做梦都想不到。
前两天我重装SQL SERVER2000，结果在安装到数据库访问组件（MDAC）时出现了错误提示：
SQL Redist
content: Command line option syntax error. Type Command /? for Help.
what is this?以前从来没有出现过这样的情况啊。我也没放在心上，按下确定，继续安装。安装完成后，数据库可以启动，也可以附加新的数据库，似乎没有什么异常。但等到调试程序时问题就来了：程序连接不上数据库！奇怪，连接串还是那个连接串，数据库文件也还是那个数据库文件，什么都未改过，好好地，怎么忽然就连不上了呢？
看来是SQLSERVER没装好。又重装了几次，还是一模一样的错误。
什么原因呢？难道是近来在网上下载安装了一些软件，中毒了？还是由于装了某著名国产财务软件的缘故搞坏了注册表？因为装了这个财务软件后，我每次启动操作系统都会给我一个警告，查了系统事件，说是SQLSERVER的一个错误，不过并没有引起什么不便，所以一直置之不理。又或者是我为了节省系统资源，禁止了一些服务，从而引起不能正常安装与使用？……
分析了又分析，操作了又操作，问题还是摆在那里，仿佛一个挥之不去的噩梦。
没办法，只好使出最后一招了。我将错误提示信息放到网上进行搜索，寻寻觅觅，终于还是在微软站点找到答案。是这样说的：
This problem may occur if all the following conditions are true: 
• You run the SQL Server 2000 SP3 Setup.bat file from a folder that contains double-byte character set (DBCS) characters in the folder name.
• An ODBC client program that uses the Sqlsrv32.dll file is running on your computer.
我英文很烂，不过还是勉强看明白了。其中第一点是说，如果你把安装文件放在一个拥有双字节的名字的文件夹里就活该倒大霉。我的SQL SERVER2000安装盘的所在文件夹起了个中文名字，而汉字正是所谓的双字节。将文件夹名字改回英文再装，问题解决了。
操作系统是简体中文的操作系统，SQL SERVER是简体中文的SQL SERVER，怎么将安装盘起了个简体中文的名字就不行了呢！
靠。
