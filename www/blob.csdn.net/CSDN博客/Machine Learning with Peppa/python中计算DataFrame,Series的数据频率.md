# python中计算DataFrame,Series的数据频率 - Machine Learning with Peppa - CSDN博客





2018年07月15日 13:26:51[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1740








在pandas里面常用用value_counts确认数据出现的频率。


- Series 情况下


```python

```
- 





import numpy as np


- 





import pandas as pd


- 





from pandas import DataFrame


- 





from pandas import Series


- 





ss = Series(['Tokyo', 'Nagoya', 'Nagoya', 'Osaka', 'Tokyo', 'Tokyo'])   


- 





ss.value_counts()   #value_counts 直接用来计算series里面相同数据出现的频率



```python

```
- 





Tokyo     3


- 





Nagoya    2


- 





Osaka     1


- 





dtype: int64







- DataFrame 情况下


```xml

```
- 





import numpy as np


- 





import pandas as pd


- 





from pandas import DataFrame


- 





from pandas import Series


- 





df=DataFrame({'a':['Tokyo','Osaka','Nagoya','Osaka','Tokyo','Tokyo'],'b':['Osaka','Osaka','Osaka','Tokyo','Tokyo','Tokyo']})       #DataFrame用来输入两列数据，同时value_counts将每列中相同的数据频率计算出来


- 





print(df)



```python

```
- 





       a      b


- 





0   Tokyo  Osaka


- 





1   Osaka  Osaka


- 





2  Nagoya  Osaka


- 





3   Osaka  Tokyo


- 





4   Tokyo  Tokyo


- 





5   Tokyo  Tokyo




```python

```
- 





df.apply(pd.value_counts) 


- 





	a	b


- 





Nagoya	1	NaN           #在b列中meiynagoya，因此是用NaN 表示。


- 





Osaka	23.0


- 





Tokyo	33.0


参考：

http://ailaby.com/dataframe_value_counts/



