# 【Regex】初试Python下的中文正则表达式 - CD's Coding - CSDN博客





2016年07月05日 12:25:20[糖果天王](https://me.csdn.net/okcd00)阅读数：2017








# 0x00 前言

真的是第一次在Python上做正则表达式呀……走了好多弯路，表示自己实在是笨的不行，难得实现了个小Demo，做点备忘好了。 

简要概括一下就是：英文正则亲和度蛮高的，随意；中文，**转成Unicode**再说话！

# 0x01 参考页面
- [正则表达式 - 教程](http://www.runoob.com/regexp/regexp-tutorial.html)
- 正则表达式 - [简介](http://www.runoob.com/regexp/regexp-intro.html)
- 正则表达式 - [语法](http://www.runoob.com/regexp/regexp-syntax.html)
- 正则表达式 - [元字符](http://www.runoob.com/regexp/regexp-metachar.html)
- 正则表达式 - [运算符优先级](http://www.runoob.com/regexp/regexp-operator.html)
- 正则表达式 - [匹配规则](http://www.runoob.com/regexp/regexp-rule.html)
- 正则表达式 - [实例](http://www.runoob.com/regexp/regexp-example.html)

- [正则表达式在线测试](http://tool.oschina.net/regex/#)
- 在程序里调试太麻烦了，有个在线的工具实时尝试还是相当不错的

- [30分钟掌握正则表达式](http://www.jb51.net/tools/zhengze.html)
- 这个真的不错的，基本上入门都靠的它
- 剧透一下30分钟学不完的 23333


# 0x02 正则步骤
- 转成Unicode 
- 个人是通过decode转成unicode编码来保证编码一致性的，感觉笨笨的，如果有什么好方法希望大神们能务必教教我~ ^_^

- 拍脑袋自己想个正则表达式
- 在站长工具之类的地方验证正则效果
- 非获取（**关键**） 
- `(?:pattern)` 匹配 pattern 但不获取匹配结果，也就是说这是一个非获取匹配，不进行存储供以后使用。这在使用 “或” 字符 (`|`) 来组合一个模式的各个部分是很有用。例如，`'industr(?:y|ies)` 就是一个比 `'industry|industries'` 更简略的表达式。
- 单单从结论上讲，java可以直接跑通的正则表达式，在python里re.compile(xxx)时，可以试着在每个左括号的右边加上非获取符号（`?:`）

- 输出的时候随意encode成自己需要的编码

# 0x03 示例代码

```python
#coding=utf8

import sys
import re, os
import string

Pnumberic    = re.compile(u"[\\d,\\.]+(?:\\d|万|亿)元");
Ppercent     = re.compile(u"[\\d,\\.]+%");
Pzero        = re.compile(u"(?:[^\\d])0(?:[^\\d])");
PDatetime    = re.compile(u"(?:(?:[0-9]{4})[-/\\.年])(?:第[一二三四]季度)?[末初前后]?(?:(?:[01]?[0-9])[-/\\.月])?(?:[0-3]?[0-9]{1}[日])?");

def LR(s):
    return '{'.decode('utf-8') + s + '}'.decode('utf-8')

if __name__ == "__main__" :

    each = u"听说这个公司的净资产为369.38亿元（截至2015年9月30日），然后随便放个12450元呀，2016年7月5日，12.34%和2.5%的百分比什么的试试看。"
    print each
    print 
    if Pnumberic.findall(each):
        #print Pnumberic.search(each).group()
        for pn in Pnumberic.findall(each):
            print pn    
            each = each.replace(pn,LR(pn))
    print 
    if Ppercent.findall(each):
        #print Ppercent.search(each).group()
        for pp in Ppercent.findall(each):
            print pp
            each = each.replace(pp,LR(pp))
    print
    if PDatetime.findall(each):
        #print PDatetime.search(each).group()
        for dt in PDatetime.findall(each):
            print dt
            each = each.replace(dt,LR(dt))
    print
    print each
    # f.write(each.encode('utf-8')+'\n')
```

输出：

```python
听说这个公司的净资产为369.38亿元（截至2015年9月30日），然后随便放个12450元呀，2016年7月5日，12.34%和2.5%的百分比什么的试试看。

369.38亿元
12450元

12.34%
2.5%

2015年9月30日
2016年7月5日

听说这个公司的净资产为{369.38亿元}（截至{2015年9月30日}），然后随便放个{12450元}呀，{2016年7月5日}，{12.34%}和{2.5%}的百分比什么的试试看。
```



