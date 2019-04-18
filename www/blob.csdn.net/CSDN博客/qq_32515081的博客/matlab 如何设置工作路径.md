# matlab 如何设置工作路径 - qq_32515081的博客 - CSDN博客
2017年12月22日 16:14:31[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：1390
**4、如何设置工作路径**
​所谓工作路径就是matlab运行时的工作目录，matlab包含很多工具箱，用户在使用时也会产生很多文件，工作路径就是matlab使用这些工具，函数，文件的途径。
4.1、查看搜索路径：path
在命令窗口中输入path，就能查看matlab的搜索路径（比如，输入一条代码，程序会在这些目录中依次搜索是否为变量，是否为函数，M文件等）。
>> path
4.2、​当前工作目录：cd
输入命令cd,可以查看当前工作目录。
>>cd
4.3、修改工作路径：userpath('F:\matlab\work')
路径可以自己定义。
>>userpath('F:\matlab\work')
​4.4、保存路径修改：savepath
改完默认路径后需要保存一下。
>>savepath
4.5、也可以用工具操作：pathtool
在命令窗口中输入pathtool，打开工具，在修改完成后，要保存。
