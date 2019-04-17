# 深度学习中的panda - qq_30006593的博客 - CSDN博客





2017年09月30日 17:56:34[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：212








```python
import panda as pd
file = pd.read_csv(csv_path, header=None)
for i,j in file.values:
    pass
# change elements
file.loc[0,0]=0
#write
file.to_csv(csv_path,header=None,index=None)
```



