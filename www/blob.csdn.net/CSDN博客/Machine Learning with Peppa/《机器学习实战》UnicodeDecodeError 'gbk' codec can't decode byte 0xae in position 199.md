# 《机器学习实战》UnicodeDecodeError: 'gbk' codec can't decode byte 0xae in position 199 - Machine Learning with Peppa - CSDN博客





2018年07月06日 21:20:27[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：671








```python
File "C:/Users/Administrator/Machine-Learning/Naive_Bayes/__main__.py", line 31, in <module>
    Spam_filter('email')

  File "C:\Users\Administrator\Machine-Learning\Naive_Bayes\Mail_filter.py", line 37, in Spam_filter
    wordList = text_parser(open(filename + '/ham/%d.txt' % i).read())

UnicodeDecodeError: 'gbk' codec can't decode byte 0xae in position 199: illegal multibyte sequence
```

今天做贝叶斯这一章的时候出了这个问题，Python3版本的GBK不太跳BUG，所以我特意上网查了一下，发现是文档里的“？”符号出错，作者应该打成了非英语系的符号。根据decode byte 0xae in position 199看出来好像是文件中某个字节不能解码，经过好心人的查找，是HAM的第23个文件出问题了。

## 解决方法：

## 打开email\ham\23.txt，第三行，找到SciFinance ?，把?替换成空格即可。











