# 使用SXSSFWorkbook创建大型excel文件 - z69183787的专栏 - CSDN博客
2018年06月13日 19:54:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2160
[https://blog.csdn.net/sophie2805/article/details/79246889](https://blog.csdn.net/sophie2805/article/details/79246889)
对于大型excel文件的创建，一个关键问题就是，要确保不会内存溢出。其实，就算生成很小的excel（比如几Mb），它用掉的内存是远大于excel文件实际的size的。如果单元格还有各种格式（比如，加粗，背景标红之类的），那它占用的内存就更多了。
对于大型excel的创建且不会内存溢出的，就只有SXSSFWorkbook了。它的原理很简单，用硬盘空间换内存（就像hash map用空间换时间一样）。
SXSSFWorkbook是streaming版本的XSSFWorkbook,它只会保存最新的excel rows在内存里供查看，在此之前的excel rows都会被写入到硬盘里（Windows电脑的话，是写入到C盘根目录下的temp文件夹）。被写入到硬盘里的rows是不可见的/不可访问的。只有还保存在内存里的才可以被访问到。
至于多少行保存在内存里，其他写入硬盘，是由DEFAULT_WINDOW_SIZE决定的。代码也可以在创建SXSSFWorkbook实例时，传入一个int参数，来设定。需注意的是，int rowAccessWindowSize如果传入100，并不是指100行保存在内存里，而是说100的屏幕尺寸下可见的行数。
```
//Construct a new workbook with default row window size
    SXSSFWorkbook test1 = new SXSSFWorkbook();
    //Construct an empty workbook and specify the window for row access.
    SXSSFWorkbook test2 =new SXSSFWorkbook(int rowAccessWindowSize);
```
- 1
- 2
- 3
- 4
上个迭代，我的任务就是要解决潜在的从DB读数据写Excel溢出的问题。数据条数从几万到几十万条不等（每一行每个单元格还带格式），加上虚拟机配置很一般，就可能内存溢出。使用SXSSFWorkbook，最大生成到800Mb的Excel，都顺畅的很。当然了，实际工作中应该不会用到这么大的excel…一般的机器光是打开就可能卡死，几乎不可读。
以下是一些在虚拟机里的运行结果（均包含从DB读数据的时间）：
```
int rowAccessWindowSize = 200；
2Mb的excel, 5秒完成
70Mb的excel，2分半完成
800Mb的excel，65分钟完成
```
- 1
- 2
- 3
- 4
还有一点值得一提，当时为了生成这800Mb的excel，我愣是把C盘清出来10个G的空间，不然SXSSFWorkbook会把硬盘空间全部写满，只剩几十kb，然后程序僵死…
另外，在Windows + Eclipse开发环境下，可以更改eclipse.ini来分配更多内存空间给java程序。在Eclipse根目录下，有个eclipse.ini文件，用文本编辑器打开，找到-vmargs这一行，修改它下面的参数，保存，重启eclipse即可： 
-Xms 默认是物理内存的1/64 
-Xmx 默认是物理内存的1/4
