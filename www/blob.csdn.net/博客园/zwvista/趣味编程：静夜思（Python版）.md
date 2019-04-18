# 趣味编程：静夜思（Python版） - zwvista - 博客园

## [趣味编程：静夜思（Python版）](https://www.cnblogs.com/zwvista/p/9506278.html)

```
from itertools import groupby

def verticalWriting(txt, offset):
    l = lambda x: x[0] % offset
    for (_, g) in groupby(sorted(enumerate(txt), key=l), key=l):
        print('|'.join(reversed(list(map(lambda x: x[1], g)))))

verticalWriting(u"床前明月光疑是地上霜举头望明月低头思故乡", 5)

'''
低|举|疑|床
头|头|是|前
思|望|地|明
故|明|上|月
乡|月|霜|光
'''
```


