# QuantLib教程（一）QuantLib的时间 - lyx的专栏 - CSDN博客





2017年05月08日 12:49:58[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：5392
所属专栏：[QuantLib的python使用教程](https://blog.csdn.net/column/details/15583.html)









        QuantLib是一个用于衍生品定价、分析分析的一个库，是用C++写的，通过SWING技术可以用Python调用。量化投资自古分P宗和Q宗，相比于各种量化回测平台，QuantLib无意识Q宗的宠儿。

        安装之类的，网上教程很多了，读者自行百度即可。安装完之后，import QuantLib，如果无误，再回来一起学习吧。

        在讨论定价的时候，期限的长短往往是一个问题，所以，时间是一个很重要的东西。

        在QuantLib中有一个Date类就是用来处理时间的。当然很多功能其实和我们常用的datetime这个库雷同，但是使用QuantLib中的Date类来定义时间的话，可以被QuantLib框架识别，所以，我们还是要学习一下。



```python
import QuantLib as ql
date = ql.Date(7, 5, 2017)#定义了2017年5月7号这样的一个date
print date
print date.dayOfMonth()#获取这个date是该月的第几天
print date.dayOfYear()#获取这个date是本年的第几天
print date.weekday()#获取date的星期数，1代表周天。大家可以自行尝试。
```

```python

```
date.weekday() == ql.Tuesday #可以用来判断这天是否是周二
date + 1 #date这天往后的一天
date - 1  # date之前一天
```python
date + ql.Period(1, ql.Months) #date之后一个月的日期
```

```python
date + ql.Period(1, ql.Weeks) #date之后一周的日期
```

```python
ql.Date(31, 3, 2015) > ql.Date(1, 3, 2015) #判断日期的先后
```


        有时候，我们可能需要获取一系列符合要求的日期天数，譬如，获取一个债券的coupon的支付日期。




```python
date1 = ql.Date(1, 1, 2015)#债券发行时间
date2 = ql.Date(1, 1, 2016)#债券结束时间
tenor = ql.Period(ql.Monthly)#付息周期
calendar = ql.UnitedStates()#时间标准
schedule = ql.Schedule(date1, date2, tenor, calendar, ql.Following,
                           ql.Following, ql.DateGeneration.Forward, False)
>>> list(schedule)
[Date(2,1,2015),
 Date(2,2,2015),
 Date(2,3,2015),
 Date(1,4,2015),
 Date(1,5,2015),
 Date(1,6,2015),
 Date(1,7,2015),
 Date(3,8,2015),
 Date(1,9,2015),
 Date(1,10,2015),
 Date(2,11,2015),
 Date(1,12,2015),
 Date(4,1,2016)]
```
        上面，我们用了一个Schedule类，来获得一系列的coupon支付日期。注意到我们用了一个calendar来制定时间的地区，这是因为节假日我们是不付coupon的，而是顺延到下一个bussiness day的，所以我们需要制定一个地区，以表明有哪些节假日。当然，好像并不支持中国地区，所以我们需要自己扩展这一部分。这是后话。




