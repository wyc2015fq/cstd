# Python基础(二)自定义函数 - _天枢 - 博客园
## [Python基础(二)自定义函数](https://www.cnblogs.com/yhleng/p/7568220.html)
2017-09-21 13:49 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7568220)
**1、判断字符串，内容是否为数字**
**我们用python；xlrd读Excel内容时，本来只是输入的整数字，经常读出来的是float类型**
**我们需要自动转成整型，意思就是说，读出来的和我们输入的一样，但是，我们不能直接用int转换，原因呢，我们无法确定读的每个单元格的值都是数字**
**那么我们自己，定认一个函数如下：**
```
def isNumeric(value):
    try:
        tv = float(value)
        return int(tv)
    except ValueError:
        return value
```
**2、获取当前系统时间**
```
import time
```
```
#获取当前系统时间
    def getCurTime(self):
        curTimeStr = time.strftime('%Y-%m-%d_%H-%M-%S',time.localtime()).decode('utf-8')
        return curTimeStr
```
**3、**
