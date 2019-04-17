# python cookbook 2.10 在正则式中使用unicode - 别说话写代码的博客 - CSDN博客





2019年04月12日 10:01:17[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：15
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#你正在使用正则表达式处理文本，但是关注的是unicode字符串处理
#默认re模块已经对一些unicode字符类有了基本支持。\\d已经匹配任意的unicode数字字符了：
import re
num=re.compile('\d+')  #ascii 数字
print(num.match('123'))
print(num.match('\u0661\u0662\u0663'))
```

![](https://img-blog.csdnimg.cn/20190412100025674.png)

```
#若想在模式中包含指定的unicode字符，可以使用unicode字符对应的转义序列（比如\uFFF,\UFFFFFFF）
#下面是一个匹配几个不同阿拉伯编码页面中所有字符的正则表达式：
arabic=re.compile('[\u0600-\u06ff\u0750-\u077f\u08a0-\u08ff]+')
```

```
#当执行匹配和搜索操作时候，最好先标准化并且清理所有文本为标准化格式，同样要考虑忽略大小写匹配
#和大小写转换的行为
pat=re.compile('stra\u00dfe',re.IGNORECASE)
s='straße'
print(pat.match(s))  #匹配到了
print(pat.match(s.upper()))   #没匹配到
print(s.upper())
```

![](https://img-blog.csdnimg.cn/20190412100048885.png)

忠告：混合使用unicode和正则表达式最好考虑安装第三方正则式库，他们会为unicode大小写转换和其他大量有趣特性提供全面支持，包括模糊匹配。



