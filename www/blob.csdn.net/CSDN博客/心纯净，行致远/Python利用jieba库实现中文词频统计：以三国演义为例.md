# Python利用jieba库实现中文词频统计：以三国演义为例 - 心纯净，行致远 - CSDN博客





2019年02月24日 23:50:45[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：172








### 词频统计

```python
#CalThreeKingdoms.py
import jieba
txt=open("threekingdoms.txt",'r',encoding="utf-8").read()
words=jieba.lcut(txt)   #jieba库函数
count={}    #创建字典
for word in words:
    if len(word)==1:
        continue
    else:
        count[word]=count.get(word,0)+1
items=list(count.items())   #转换成列表
items.sort(key=lambda x:x[-1],reverse=True)
for i in range(15):
    word,count=items[i]
    print("{0:<10}{1:>5}".format(word,count))
```

RESTART: C:/Users/QinJX/AppData/Local/Programs/Python/Python37-32/python编程学习/CalThreeKingdoms.py

Building prefix dict from the default dictionary ...

Loading model from cache C:\Users\QinJX\AppData\Local\Temp\jieba.cache

Loading model cost 1.109 seconds.

Prefix dict has been built succesfully.

曹操          953

孔明          836

将军          772

却说          656

玄德          585

关公          510

丞相          491

二人          469

不可          440

荆州          425

玄德曰         390

孔明曰         390

不能          384

如此          378

张飞          358

### 以人物出场顺序的话：

```python
#CalThreeKingdomsV2.py
import jieba
excludes = {"将军","却说","荆州","二人","不可","于是","今日",\
            "不能","如此","主公","商议","如何","不敢","魏兵",\
            "军士","左右","军马","引兵","大喜","次日","天下","东吴"}
txt = open("threekingdoms.txt", "r", encoding='utf-8').read()
words  = jieba.lcut(txt)
counts = {}
for word in words:
    if len(word) == 1:
          continue
    elif word == "诸葛亮" or word == "孔明曰":
        rword = "孔明"
    elif word == "关公" or word == "云长":
        rword = "关羽"
    elif word == "玄德" or word == "玄德曰":
         rword = "刘备"
    elif word == "孟德" or word == "丞相":
         rword = "曹操"
    else:
         rword = word
    counts[rword] = counts.get(rword,0) + 1
for word in excludes:
    del counts[word]
items = list(counts.items())
items.sort(key=lambda x:x[1], reverse=True) 
for i in range(7):
    word, count = items[i]
    print ("{0:<10}{1:>5}".format(word, count))
```

RESTART: C:/Users/QinJX/AppData/Local/Programs/Python/Python37-32/python编程学习/CalThreeKingdomsV2.py

Building prefix dict from the default dictionary ...

Loading model from cache C:\Users\QinJX\AppData\Local\Temp\jieba.cache

Loading model cost 1.139 seconds.

Prefix dict has been built succesfully.

曹操         1451

孔明         1383

刘备         1252

关羽          784

张飞          358

吕布          300

赵云          278



