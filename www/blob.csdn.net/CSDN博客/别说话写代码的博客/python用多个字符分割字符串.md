# python用多个字符分割字符串 - 别说话写代码的博客 - CSDN博客





2019年03月20日 20:50:27[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：26








用re包，多个字符全部放入[ ]即可，比如要把1+2-3按照+ - 分隔。就re.split("[+-]")

```python
import re
s = "1+2-3"
a=re.split("[+-]",s)
print(a)
```

![](https://img-blog.csdnimg.cn/2019032020500297.png)



