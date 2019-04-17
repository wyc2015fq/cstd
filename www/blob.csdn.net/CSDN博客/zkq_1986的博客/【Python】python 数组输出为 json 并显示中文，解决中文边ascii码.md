# 【Python】python 数组输出为 json 并显示中文，解决中文边ascii码 - zkq_1986的博客 - CSDN博客





2018年10月15日 10:06:49[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：202








```python
import json
ls=[{"name":"我暗暗","age":25},{"name":"Katie","age":33}]
with open('a.txt','w',encoding='utf-8') as f:
    f.write(json.dumps(ls,ensure_ascii=False))
```





