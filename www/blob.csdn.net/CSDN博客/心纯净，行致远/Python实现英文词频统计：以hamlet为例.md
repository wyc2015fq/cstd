# Python实现英文词频统计：以hamlet为例 - 心纯净，行致远 - CSDN博客





2019年02月24日 23:19:04[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：46标签：[Python](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[【Python】](https://blog.csdn.net/zhanshen112/article/category/7930576)









代码和hamlet.txt需要置于同一目录

```python
#CalWordNum.py
def GetTxt():
    txt=open("hamlet.txt",'r').read()
    txt=txt.lower()
    for ch in "!'#$%&()*+,-./:'<=>?@[\\]^-‘{|}~":   #将特殊符号替换为空格
        txt=txt.replace(ch," ")
    return txt

hamlet=GetTxt()
words=hamlet.split()    #split返回列表类型
count={}    #创建字典
for word in words:
    count[word]=count.get(word,0)+1
items=list(count.items())   #转换成列表
items.sort(key=lambda x:x[-1],reverse=True)
for i in range(10):
    word,count=items[i]
    print("{0:<10}{1:>5}".format(word,count))
```

RESTART: C:/Users/QinJX/AppData/Local/Programs/Python/Python37-32/python编程学习/10CalWordNum.py

the        1143

and         966

to          762

of          669

i           629

a           546

you         544

my          514

hamlet      467

in          451



