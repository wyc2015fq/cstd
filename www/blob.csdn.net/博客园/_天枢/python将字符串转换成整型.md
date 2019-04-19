# python将字符串转换成整型 - _天枢 - 博客园
## [python将字符串转换成整型](https://www.cnblogs.com/yhleng/p/8623386.html)
2018-03-22 14:07 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8623386)
**将字符串转换成，整型，从字面理解很容易让人误会。**
**比如，要把这个"abcabc"转换成整型，臣妾做不到啊。除成转成ascii。**
**我们所说字符串转成整型是这样的。**
```
**s = "12"s = "12.12"**
```
我们自己写一个函数，来实现这个转换。
思想：python提供转换成整型用int()这关键字，那么，如果我这样**int('12') int(12.12)**这样是可以转成整型的。但是如果**int('abc') int('12.12') 会报类型错误**
**那么我们就利用无法转换的情况，会报类型错误来写这个函数。**
**实现：如果可以转换成，int型直接反回转换后的值，如果无法转换，反回False**
```
def to_int(str):
    try:
        int(str)
        return int(str)
    except ValueError: #报类型错误，说明不是整型的
        try:
            float(str) #用这个来验证，是不是浮点字符串
            return int(float(str))
        except ValueError:  #如果报错，说明即不是浮点，也不是int字符串。   是一个真正的字符串
            return False
```
调用：
```
print to_int('str')
print to_int('str123')
print to_int('12.12')
print to_int('234')
print to_int('12#$%%')
```
结果：
```
D:\Python27\python.exe D:/HttpRunnerManager-master/HttpRunnerManager-master/test.py
False
False
12
234
False
```
**python学习3群：563227894**
