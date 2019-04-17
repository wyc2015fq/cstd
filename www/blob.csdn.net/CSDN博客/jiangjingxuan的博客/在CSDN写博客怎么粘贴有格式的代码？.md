# 在CSDN写博客怎么粘贴有格式的代码？ - jiangjingxuan的博客 - CSDN博客





2019年02月22日 10:29:08[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：39








**本文为原创，转载请注明：[https://blog.csdn.net/jiangjingxuan/article/details/87873324](https://blog.csdn.net/jiangjingxuan/article/details/87873324)**

1、在CSDN写博客的富文本编辑器上方选择</>，如下图所示

![](https://img-blog.csdnimg.cn/20190222102051717.png)



2、在弹出的"插入代码"窗口中粘贴相关的代码，然后再右上角的下拉列表中，选择语言类型（本例选Python）。

![](https://img-blog.csdnimg.cn/20190222102834332.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

3、点“确定”后，返回编辑器，即可看到有格式的代码。

```python
import cPickle
import gzip
import numpy as np
import scipy.misc

def load_data():
    f = gzip.open('./data/mnist.pkl.gz', 'rb')
    training_data, validation_data, test_data = cPickle.load(f)
    f.close()
    return (training_data, validation_data, test_data)
```





