# 用string.format()解决可空类型变量的格式化 - Big Smile - CSDN博客
2016年12月06日 16:20:26[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2112
        故事的起因总是那么的突然，这不，人事系统又要改需求了，这次是要把人事代理合同书下面的甲方签字的时间改为档案的转入时间，这么小的一个问题处理起来应该相当的容易吧。
        但是看到代码之后才发现没有那么简单，因为里面的变量虽然可以从ViewModel中获得，但是其中的格式转换确实是一个难题，我们首先来看数据库中的数据格式是什么：
![](https://img-blog.csdn.net/20161206154125165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)            我们需要的是什么格式呢？             ![](https://img-blog.csdn.net/20161206154330673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
  于是有了今天的故事。
# 一、普通的转换方法
         日期的格式化我们都不陌生，因为我们的机房还有学生系统都用过很多遍了，且都换了三种语言了，所以肯定也不是很难，在C#中的常用的格式化的方法肯定是直接tostring（format） 就可以办到，例如：
`string nowTime = DateTime.Now.ToString("yyyy年M月d日");`
还有就是分段输出也是可以的，以DateTime类型为例，用其中的年，月，日的变量分别输出：
```
DateTime asd = new DateTime();
asd = DateTime.Now;      //时间暂定为现在
string str = asd.Year.ToString();     //取年
str += asd.Month.ToString();          //取月拼接
str += asd.Day.ToString();            //取日拼接
```
以上方法都是最常用的方法，但是今天的这个情况不适合以上的方法，因为点出来的方法并没有这些东西，并且在Tostring里面写东西是会报错的，就是下面的错，于是有了这篇博客的精华，也就是解决的方法。
![](https://img-blog.csdn.net/20161206155611688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20161206155617816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、调查深入
        因为直接转换不行，且类型是DateTime?类型的，所以，我就想着去改变类型为DateTime，看到了Model里面是这么定义的：
![](https://img-blog.csdn.net/20161206160122245?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        看字面意思就能猜出来是可以为Null的类型，因为如果直接写DateTime  dt = null；是会报错的，由于DateTime是值类型，所以不能为Null，就和Int类型为空必须是0二不是null是一个道理的。
        于是把所有的可空类型都改为了DateTime的不可空的类型之后整个程序就报错了，因为页面上的数据有没有填写全的情况，所以在填写整个页面的时候也需要加载Model里面的数据，就出现了空数据的获取赋值时候的错误。
        没有办法，只能还原成可空类型了。
# 三、老朋友上场
        于是小编开始在网上游荡寻找，终于功夫不负有心人，终于被我找到了，原来用String.Format()就可以办到，这个函数可以说是很不陌生，因为在学习VB.NET转换时间格式的时候就曾经用过这个函数，只是因为实验了一个上午都没有成功就放弃了，因为ToString这个函数用起来比较简单，直接写通用的格式就可以，所以没有仔细研究，正好研究一下。
    这里我们要把时间格式转换为xxxx年xx月xx日，所以代码要这样写：
`string.Format("{0:yyyy}" + "年" + "{0:MM}" + "月" + "{0:dd}" + "日", enFileWages.intotime)`
     其中intotime是获得的DateTime?类型的时间。如果为空的话，结果就是“  年  月   日”。先给大家结果，下面来说这里的知识（这里只介绍日期类型的，剩下的自行百度）：
string.format（）可用的参数，用法参照上面的语句：
|Specifier|Type|Example|Example Output|
|----|----|----|----|
|dd|Day|{0:dd}|10|
|ddd|Day name|{0:ddd}|Tue|
|dddd|Full day name|{0:dddd}|Tuesday|
|f, ff, ...|Second fractions|{0:fff}|932|
|gg, ...|Era|{0:gg}|A.D.|
|hh|2 digit hour|{0:hh}|10|
|HH|2 digit hour, 24hr format|{0:HH}|22|
|mm|Minute 00-59|{0:mm}|38|
|MM|Month 01-12|{0:MM}|12|
|MMM|Month abbreviation|{0:MMM}|Dec|
|MMMM|Full month name|{0:MMMM}|December|
|ss|Seconds 00-59|{0:ss}|46|
|tt|AM or PM|{0:tt}|PM|
|yy|Year, 2 digits|{0:yy}|02|
|yyyy|Year|{0:yyyy}|2002|
|zz|Timezone offset, 2 digits|{0:zz}|-05|
|zzz|Full timezone offset|{0:zzz}|-05:00|
|:|Separator|{0:hh:mm:ss}|10:43:20|
|/|Separator|{0:dd/MM/yyyy}|10/12/2002|
参考的博客链接：[http://www.cnblogs.com/tangjian/archive/2009/03/28/1423897.html](http://www.cnblogs.com/tangjian/archive/2009/03/28/1423897.html)
        探索到这里就结束了，我们的功能也就实现了，其实都是很简单的东西，只是由于没有这样用过，导致了一个小时的学习的时间，对于现在的我们来说也是很值得的。如有问题，还希望各位读者指正。
