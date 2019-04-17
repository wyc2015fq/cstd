# Python Solve UnicodeEncodeError 'gbk' / 'ascii' / 'utf8' codec can't encode character '\x??' in position ? 解决有关Python编码的错误 - Grandyang - 博客园







# [Python Solve UnicodeEncodeError 'gbk' / 'ascii' / 'utf8' codec can't encode character '\x??' in position ? 解决有关Python编码的错误](https://www.cnblogs.com/grandyang/p/6906431.html)







在Python中，处理中文字符一直是很令人头痛的问题，一言不合就乱码，而且引起乱码的原因也不尽相同，有时候是python本身默认的编码器设置的不对，有时候是使用的IDE的解码器不对，还有的时候是终端terminal的解码器不对，有时候同一份代码在Python2上正常运行，Python3上就不行了，反正产生乱码的原因很多，这里就列举一些博主遇到过的一些错误及其解决方案：



Error 1:

UnicodeEncodeError: 'gbk' codec can't encode character '\x??' in position ?: illegal multibyte sequence

这个是在Win7上用Python3编译的时候遇到的，原因是win7的python3的默认编码不是utf8，我们只需要将默认编码改为utf8就能解决这个问题，参见下面的代码：



```
import sys, io
sys.stdout = io.TextIOWrapper(sys.stdout.buffer,encoding='utf8') # Change default encoding to utf8
```



Error 2：

UnicodeDecodeError: 'ascii' codec can't decode byte 0x?? in position: ordinal not in range(128)

这个是Mac上使用Python2编译的时候遇到的，原因是也是由于python2的默认编码不是utf8，我们只需要将默认编码改为utf8就能解决这个问题，参见下面的代码：



```
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
```



像其他的产生错误的原因，比如IDE显示乱码，或者终端Terminal显示乱码，可以通过Google搜索快速的找出解决方法，这里就不赘述了。



参考资料：

[http://ajucs.com/2015/11/10/Python-character-encoding-explained.html](http://ajucs.com/2015/11/10/Python-character-encoding-explained.html)












