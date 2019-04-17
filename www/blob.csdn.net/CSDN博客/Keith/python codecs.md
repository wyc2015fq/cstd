# python codecs - Keith - CSDN博客





2016年12月19日 15:45:44[ke1th](https://me.csdn.net/u012436149)阅读数：720








# python codecs

codecs用来读取中文文件

```python
import codecs
file_name = "file_name"
with codecs.open(file_name,mode='rb',coding='gbk') as file:
  for line in file:
    for word in line:
      print word
```

**参考文献**
[https://docs.python.org/3/library/codecs.html](https://docs.python.org/3/library/codecs.html)



