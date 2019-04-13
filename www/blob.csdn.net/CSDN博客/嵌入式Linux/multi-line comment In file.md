
# multi-line comment In file - 嵌入式Linux - CSDN博客

2015年01月05日 17:30:18[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1326


写注释引起的编译问题：

```python
//s3c_gpio_cfgpin(pin, GTP_INT_CFG); \
```

//s3c_gpio_cfgpin(pin, GTP_INT_CFG); \

如果向上面那样写注释的话，就会引起问题，GCC，警告，有可能编译不通过，我遇到的问题就是编译不通过。


