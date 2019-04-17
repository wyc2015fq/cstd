# python cookbook 2.5 字符串搜索和替换 - 别说话写代码的博客 - CSDN博客





2019年03月30日 14:48:19[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：33
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#若想在字符串中搜索和匹配指定的文本模式。对简单的字面模式，直接使用str.replace()方法即可
text = 'yeah, but no, but yeah, but no, but yeah'
print(text.replace('yeah','yep'))
```

![](https://img-blog.csdnimg.cn/20190330144613944.png)

```
#对于负责的模式，请使用re模块中的sub()函数。为了说明这个，假设你想将形式为11/27/2012的日期
#改成2012-11-27.例如
text = 'today is 11/27/2012,pycon starts 3/13/2013.'
import re
print(re.sub(r'(\d+)/(\d+)/(\d+)',r'\3-\1-\2',text))
```

![](https://img-blog.csdnimg.cn/20190330144631939.png)

```
#sub函数的第一个参数是被匹配的模式，第二个参数是替换模式。反斜杠数字比如\3指向前面模式的捕获组号
#如果打算用相同的模式做多次替换，考虑先编译它来提升性能。比如：
import re
datepat = re.compile(r'(\d+)/(\d+)/(\d+)')
print(datepat.sub(r'\3-\1-\2',text))
```

![](https://img-blog.csdnimg.cn/20190330144654971.png)

```
#对于更加复杂的替换，可以传递一个替换回调函数来代替，比如：
from calendar import month_abbr
def change_date(m):
    mon_name = month_abbr[int(m.group(1))]
    return '{}-{}-{}'.format(m.group(2),mon_name,m.group(3))
print(datepat.sub(change_date,text))
```

![](https://img-blog.csdnimg.cn/20190330144715718.png)

```
#一个替换毁掉函数的参数是一个match对象，也就是match()或者find()返回的对象。使用group()方法来提取特定的匹配部分。
#回调函数最后返回替换字符串
#如果除了替换后的结果外，你还想知道有多少替换发生了，可以使用re.subn()来代替
newtext,n = datepat.subn(r'\3-\1-\2',text)
print(newtext)
print(n)  #返回替换了多少次
```

![](https://img-blog.csdnimg.cn/2019033014474077.png)





