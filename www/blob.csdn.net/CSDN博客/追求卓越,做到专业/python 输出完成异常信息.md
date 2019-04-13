
# python 输出完成异常信息 - 追求卓越,做到专业 - CSDN博客


2019年01月09日 14:03:23[Waldenz](https://me.csdn.net/enter89)阅读数：74


```python
import traceback
try:
    1/0
exception:
   traceback.print_exc()
```
traceback.print_exc() 直接打印异常
traceback.format_exc()返回字符串
print_exc() 还可以接受file参数直接写入到一个文件
还可以将信息写入到文件
traceback.print_exc(file=open(‘error.txt’,’a+’))

