# UnicodeEncodeError: 'ascii' codec can't encode characters in position 0-1: ordinal not in range(128) - 别说话写代码的博客 - CSDN博客





2018年08月18日 20:38:09[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：157








```python
from functools import partial
import pickle
pickle.load = partial(pickle.load, encoding="latin1")
pickle.Unpickler = partial(pickle.Unpickler, encoding="latin1")
```





