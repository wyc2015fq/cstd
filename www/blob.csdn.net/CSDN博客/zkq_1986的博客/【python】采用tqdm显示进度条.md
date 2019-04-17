# 【python】采用tqdm显示进度条 - zkq_1986的博客 - CSDN博客





2018年12月27日 10:57:23[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：45








```python
from tqdm import tqdm

for i in tqdm(range(100)):
    for j in range(100000):
        s='sd'+'s'
```

输出：

100%|██████████| 100/100 [00:00<00:00, 116.38it/s]



