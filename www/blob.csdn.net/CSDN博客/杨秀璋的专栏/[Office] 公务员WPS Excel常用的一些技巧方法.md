# [Office] 公务员WPS Excel常用的一些技巧方法 - 杨秀璋的专栏 - CSDN博客





2018年01月05日 14:43:27[Eastmount](https://me.csdn.net/Eastmount)阅读数：1409








这篇文章主要是我最近工作使用WPS Excel的一些常用技巧和方法，仅仅是一篇在线笔记。当然实际操作中，你遇到问题百度经验或相关网站会提供对应的解决方法，而且它们写得更好，这篇文章更多的是结合自己使用学到的技巧。作为程序员，写了这么多年的代码，用过的Excel屈指可数，不会这些技巧也是理所当然的，因为我们需要做的是去开发这些软件，或者写代码直接导出各种各样的表格，可视化处理等等。只是现在这一年，需要学习罢了。文章没有涉及到编程知识，希望大家多多海涵，这也是我这段时间工作生活的一个映射，一年的磨炼，一年的等待，一年的成长，感谢这一年亲朋好友同事的帮助，终身受益。


  一.数据透视表功能
  二.常用小技巧
    1.设置表格统一行高
    2.表格百分比计算及除法
    3.自定义排序功能
    4.表格筛选功能
    5.固定表头
    6.设置含表头打印
    7.常见字符、函数功能
    8.查找重复项
  三.vlookup函数获取两张表数据
  四.其他高级技巧
  1.设置定时保存
    2.并排查找工作薄
    3.模糊查找
    4.隔行颜色显示
    5.固定表头
    6.直方图生成







## 一. 数据透视表功能




数据透视表是WPS做汇总信息的一个很好的功能，通常用于绘制各种报表、汇总表等。现在假设存在如下“贵州5A和4A风景区”的数据，如下图所示。
注意：数据中旅游人数、旅游收入、录入年份，纯属虚构，读者结合自己数据实验。




![](https://img-blog.csdn.net/20180103171021539)





第一步：选中需要的数据集，注意不能包含跨行或合并的数据行、数据列，点击“数据”->“数据透视表”->选择“新建工作表”->“确定”。
您可以按住Shift键滑动鼠标选中所有的数据。



![](https://img-blog.csdn.net/20180103151729849)

第二步：点击确定后如下图所示，需要在下面页面操作生成对应的汇总表。右边为对应的字段，选择所需字段对数据进行汇总。


![](https://img-blog.csdn.net/20180103152332549)

第三步：这里选择“市州”、“景点名称”、“旅游人数”和“旅游收入”等字段信息。注意需要统计的对应类别为行，对应的值为汇总的数值。


![](https://img-blog.csdn.net/20180103152746863)

注意，在右下角“值”部分，选中对应字段，可以设置求和、计数等选项，最后进行简单处理即可生成对应的汇总表，可以看到各个市州的景区情况，同时也可以透视不同年份的情况。


![](https://img-blog.csdn.net/20180103153013023)





## 二. 常用小技巧




**1.设置表格统一行高**

在某些情况下，由于Excel行数据过多时，需要设置固定的行高，那怎么实现呢？下面讲述一个简便的方法。选中需要设置统一行高的数据行，然后找到左边行号的两行间隔，拖动鼠标则可设置固定统一的行高，如下图所示。



![](https://img-blog.csdn.net/20180103155119471)




也可以右键设置行高，如下图所示：



![](https://img-blog.csdn.net/20180103155511263)

同时，在设置Excel表中，通常习惯尽可能显示一行的所有数据，让其自适应行高，这只需要选中所需行，点击左边两行之间的线条即可实现，如下图所示。



![](https://img-blog.csdn.net/20180103161559016)



**2.表格百分比计算及除法**
假设存在如下表格，现在需要统计各个市州所占人数的百分比，怎样实现比较迅速呢？在新的一列中，设置：
  当前行数据/求和数据(27107)*100




![](https://img-blog.csdn.net/20180103162440564)

然后点击如图所示选中对话框的右下角，并双击鼠标，则更新所有数据。


![](https://img-blog.csdn.net/20180103164208193)

同时，很多时候会出现需要将单位万元修改为亿元，则直接设置其D22/10000，所有数据分别除以一万即可。这也是一个小技巧。

**3.自定义排序功能**
如果是按照某一列的大小来排序还好实现，而某些表格通常会存在一些自定义的顺序，那么如何对其进行自定义排序呢？比如下面生成的透视汇总表需要按照贵州省市州固定顺序排列，即：贵\遵\六\安\毕\铜\黔东南\黔南\黔西南的顺序。



![](https://img-blog.csdn.net/20180103153013023)






首先根据需求的顺序，我们在新的一列中增加对应的数字，1表示新的排名是第一位，如下图所示：





![](https://img-blog.csdn.net/20180103165901815)

然后选中它们，点击“排序”->“自定义排序”->主要关键字选择“列E”->升序确定即可。


![](https://img-blog.csdn.net/20180103170057082)


排序成功后输出如下所示：


![](https://img-blog.csdn.net/20180103170201236)

**4.表格筛选功能**
表格筛选是一个比较常用的功能，包括颜色筛选、内容筛选和数字筛选，比如找到某种特定颜色的数据，再如找到年旅游人数大于2000万人的景点，这些在公务员中是常见的功能。



![](https://img-blog.csdn.net/20180103170754236)

![](https://img-blog.csdn.net/20180103170733850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







**5.固定表头**

固定表头功能主要是当数据较多时，当用户滑动鼠标浏览数据时，知道对应数据的字段，方便了解信息。其设置方法主要是开始栏目中的“冻结窗口”，也可以取消冻结或固定列。比如下图固定前两行数据。



![](https://img-blog.csdn.net/20180103171340836)




如下图所示，可以看到表头固定情况下预览其他数据。



![](https://img-blog.csdn.net/20180103171542655)


**6.设置含表头打印**
设置表头打印是为了当数据很多时，能够查看每页数据的表头，而不用每次都翻到第一页查看。主要方法是：在“页面布局”中点击“打印标题”，如下图所示。




![](https://img-blog.csdn.net/20180103171806927)



然后选中“顶端标题行”选择需要打印的表头即可。



![](https://img-blog.csdn.net/20180103171958076)

打印效果预览如下图所示：

![](https://img-blog.csdn.net/20180103172056284)

同时，可以在“页面布局”中设置纸张大小、纸张方向、纸张边距等。


**7.常见字符、函数功能**字符串函数主要讲解&拼接，left获取左边部分、right获取右边部分、mid获取中间内容。比如：

![](https://img-blog.csdn.net/20180103172834425)


**8.查找重复项**

查找重复项如下所示，点击“高亮重复项”。



![](https://img-blog.csdn.net/20180103173215597)

如果出现重复项目如“黄果树瀑布”，它会标记成橙色。


![](https://img-blog.csdn.net/20180103173403507)






更多知识比如设置A3打印、带格式粘贴等，希望读者自己研究。








## 三. vlookup函数获取两张表数据




VLOOKUP函数是Excel中的一个纵向查找函数，它与LOOKUP函数和HLOOKUP函数属于一类函数，在工作中都有广泛应用，例如可以用来核对数据，多个表格之间快速导入数据等函数功能。下面讲解vlookup()函数从Sheet1中快速导入Sheet2的“年旅游人数”字段。



![](https://img-blog.csdn.net/20180103175522704)




接下来使用vlookup函数：
**VLOOKUP(lookup_value, table_array, col_index_num, [range_lookup])**
表述就是VLOOKUP(查找值，查找范围，查找列数，精确匹配或者近似匹配），在我们的工作中，几乎都使用精确匹配，该项的参数一定要选择为false。否则返回值会出乎你的意料。

=VLOOKUP(A4,Sheet1!A1:H23,4,FALSE)其中A4表示对应相等字段，通过“黄果树瀑布”；Sheet1!A1:H23表示从该区域进行查找；4表示第四列数据；FALSE表示精确查找。





![](https://img-blog.csdn.net/20180103183912273)




输出结果如下所示，需要注意如果报错“N/A”，原因是两张表的第一列数据需要一致。


![](https://img-blog.csdn.net/20180103183921655)

参考：[https://jingyan.baidu.com/article/73c3ce28db4da4e50243d95c.html](https://jingyan.baidu.com/article/73c3ce28db4da4e50243d95c.html)


![](https://img-blog.csdn.net/20180103180944170)

![](https://img-blog.csdn.net/20180103181011289)







## 四. 其他高级技巧




**1.设置定时保存**
设置定时备份文件如下图所示：



![](https://img-blog.csdn.net/20180105094047282?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






**2.并排查找工作薄**

并排查找工作薄通常用于对比两张表的数据，Word和Excel都用得比较多。在“视图”中点击“并排比较”按钮，如下图所示，同时可以设置是否“同步滚动”。



![](https://img-blog.csdn.net/20180105094529666?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180105094355138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






**3.模糊查找**

快捷键Ctrl+F是查找，快捷键Ctrl+H是替换，强制换行是Alt+Enter。模糊查找主要使用“*”来进行匹配，比如“*山”查找所有包含“山”的景点，输出如下所示：




![](https://img-blog.csdn.net/20180105103935568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




其中：问号？表示在搜索目标中替代任何单个的字符，星号*表示替代任意多个连续的字符，包括空字符。







**4.数据条显示**

通过数据条显示数据方法如下，在“开始”栏目中点击“条件格式”->“数据条”，如下所示：




![](https://img-blog.csdn.net/20180105105726669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





输出如下图所示：




![](https://img-blog.csdn.net/20180105105759143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



同时可以利用箭头设置数据趋势情况，在“条件格式”->“图标集”中选择三向箭头。



![](https://img-blog.csdn.net/20180105110326431?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




**5.隔行颜色显示**

下面讲解如何隔行显示内容。在“开始”栏目中选择“条件格式”->“新建规则”如下图所示：




![](https://img-blog.csdn.net/20180105142512273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





在新建格式规则中点击“使用公式确定要设置格式的单元格”，如下图设置公式为：
**=MOD(ROW(),2)**


![](https://img-blog.csdn.net/20180105142612387?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



同时点击“格式”，设置填充效果，比如隔行颜色内容。



![](https://img-blog.csdn.net/20180105142816772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



最后输出结果如下图所示：



![](https://img-blog.csdn.net/20180105142908787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**6.直方图生成**
假设需要生成折线图如下所示：



![](https://img-blog.csdn.net/20180105143435089?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



点击“插入”->“图表”->“折线图”，如下所示即可。



![](https://img-blog.csdn.net/20180105143531096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





最后希望文章对您有所帮助，如果存在不足之处，请海涵~
(By:Eastmount 2017-01-05 中午12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))






















