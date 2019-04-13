
# linux批量杀死进程 - lpty的博客 - CSDN博客

2017年11月28日 10:08:59[lpty](https://me.csdn.net/sinat_33741547)阅读数：1797



## 一、前言
一般服务器杀死进程直接用kill就可以了，但如果程序出问题，启了一大堆进程，一个个关掉那太恐怖了。
## 二、实战

```python
sudo ps aux|grep xxx|awk '{print $2}'|xargs kill -9
```

这里是批量杀死进程名包含xxx的进程，记录做个备忘。

