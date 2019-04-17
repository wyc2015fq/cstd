# 规模数据导入高效方式︱将数据快速读入R—readr和readxl包 - 素质云笔记/Recorder... - CSDN博客





2017年01月06日 10:17:41[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3128








> 
本文由雪晴数据网负责翻译整理，原文请参考New packages for reading data into R — fast作者David 

  Smith。转载请注明原文链接[http://www.xueqing.tv/cms/article/102](http://www.xueqing.tv/cms/article/102)

昨天在新电脑使用xlsx包的时候，因为加载rJava十分不悦…于是用了readxl，不要太方便，于是转一篇过来备用着。以后读入都用你了~


Hadley Wickham 和 RStudio团队写了一些新的R包，这些包对于每个需要在R中读入数据的人来说都是非常有用的。readr包提供了一些在R中读入文本数据的函数。readxl包提供了一些在R中读入Excel电子表格数据的函数。它们的读取速度远远超过你目前正在用的一些函数。

readr包提供了若干函数在R中读取数据。我们通常会用R中的read.table家族函数来完成我们的数据读入任务。这里，readr包提供了许多替代函数。它们增加了额外的一些功能并且速度快很多。

首先，read_table几乎代替了read.table。下面通过读取一个包含400万行的数据来比较它们的区别。点击这里下载该数据。

注1：在演示之前简单说下我电脑的配置：win7,64位操作系统，8G内存，CPU A6双核。电脑配置不行，原文给出的实验时间甩了我好几条街。但不管怎样，在现有的条件下效率确实提高了很多。原文用时见末尾链接。

注2：如果读取中文数据出现乱码，在编辑器设置下字符编码为”UTF-8”

```
system.time(read_table("C:\\Users\\a\\Desktop\\biggerfile.txt",
                       col_names=c("DAY","MONTH","YEAR","TEMP")))

system.time(read.table("C:\\Users\\a\\Desktop\\biggerfile.txt",
                       col.names=c("DAY","MONTH","YEAR","TEMP")))
```

这些命令看上去非常相似，但是read.table花的时间是50.62秒，而read_table完成相同的任务只花了2.76秒。这是因为read_table把数据当做是固定格式的文件，并且使用C++快速处理数据。

R中的基础包utils也有读取固定宽度数据的函数，下面的示例就能体现出readr的亮点：

```
system.time(read_fwf("C:\\Users\\a\\Desktop\\biggerfile.txt", 
                      fwf_widths(c(3,15,16,12),
                      col_names=c("DAY","MONTH","YEAR","TEMP"))))

system.time(read.fwf("C:\\Users\\a\\Desktop\\biggerfile.txt", 
                     c(3,15,16,12),
                     col.ames=c("DAY","MONTH","YEAR","TEMP")))
```

readr包的read_fwf函数用时3.97秒，而标准的read.fwf函数耗时1372秒。

readr包中的其它函数包括：read_csv读取逗号分隔的数据（欧洲用的是read_csv2函数），read_tsv读取制表符分隔数据，read_lines函数从文件中逐行读取数据（非常适合复杂的后期处理）。它还可以读取多种格式的日期时间列，智能的将文本数据读取为字符串（不再需要设置strings.as.factors=FALSE）。

> 
对于Excel格式的数据，这里有readxl包。这个包提供的函数可以读取.xls和.xlsx格式的Excel工作表。虽然这里没有演示read_execl函数的使用，但是它跟readr中的函数一样都是基于C++库的，因此读取速度应该也很快。最重要的是，它没有任何的外部依赖，因此你可以在任意平台上用它来读取数据—不要求安装了Excel。


readr包已发布在CRAN上，readxl可以从github安装。



