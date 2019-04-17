# Python绘制二维曲线的日常应用 - 小灰笔记 - CSDN博客





2017年01月15日 17:48:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2135








使用Python绘制出类似Excel或者MATLAB的曲线还是比较容易就能够实现的，需要用到的额外库有两个，numpy和matplotlib。使用这两个模块实现的曲线绘制其实在一定程度上更像是MATLAB的plot功能，不过今天看了一下matplotlib网站上的信息，现在的功能更为强劲了，而且已经支持三维图像的绘制。

模块库的安装非常简单，我使用的Mac，在Mac上用pip进行了两个模块库的安装都十分顺畅。相信其他平台基本上也都这样，如果能够联网，这种安装方式是十分推荐的，确实是简单。

我用Python读取我自己日常运动的数据，数据以Numbers的方式进行统计，导出成Excel文件。为了能够读取Excel文件，我又安装了xlrd模块库。

从matplotlib的网站上抄了一小段代码简单做了一下修改，加入了数据读取以及简单的计算，代码如下：

  1 #!/usr/bin/python

  2 

  3 import numpy
as np

  4 import matplotlib.pyplot
as plt

  5 from xlrd
import open_workbook

  6 

  7 defSportLine(excel_file):

  8         days_year    = []

  9         target_km    = []

 10         records      = []

 11         sum_records  = []

 12         pct_records  = []

 13         target_pct   = []

 14 

 15         fig,axs = plt.subplots(3)

 16 

 17 for i inrange(365):

 18                 days_year.append(i)

 19 

 20 for day in days_year:

 21                 target_km.append(float(day)/365.0 *
1000.0)

 22 

 23 # read record data

 24         book  = open_workbook(excel_file)

 25         sheet = book.sheet_by_name('record')

 26         rows_num = sheet.nrows

 27         cols_num = sheet.ncols

 28 for row_num inrange(3,368):

 29 try:

 30                         records.append(float(sheet.cell(row_num,1).value))

 31 except:

 32                         records.append(0.0)

 33 

 34 # calculate sum of records

 35         sum_record =
0.0

 36 for each_record in records:

 37                 sum_record += each_record

 38                 sum_records.append(sum_record)

 39 

 40 # calculate pct of all

 41 for each_sum in sum_records:

 42                 pct_records.append(each_sum /
1000.0)

 43 

 44 # calculate target pct

 45 for day inrange(1,366):

 46                 target_pct.append(float(day)/365.0)

 47 

 48 # plot target and sum trend

 49         ax = axs[0]

 50         ax.plot(days_year,sum_records)

 51         ax.plot(days_year,target_km)

 52         ax.set_title('distance-year-km')

 53         ax.grid(True)

 54 

 55 # plot record

 56         ax = axs[1]

 57         ax.plot(days_year,records)

 58         ax.set_title('distance-day-km')

 59         ax.grid(True)

 60 

 61 # plot percentage

 62         ax = axs[2]

 63         ax.plot(days_year,pct_records)

 64         ax.plot(days_year,target_pct)

 65         ax.set_title('pct-100%')

 66         ax.grid(True)

 67         plt.show()

 68 

 69 SportLine('records.xlsx')

 70 




我的运动数据记录电子表格格式如下：

![](https://img-blog.csdn.net/20170115174840876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







程序运行，画出的曲线如下：


![](https://img-blog.csdn.net/20170115174911971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




基本差不多了，后面需要做的只有细节上的修正了。




