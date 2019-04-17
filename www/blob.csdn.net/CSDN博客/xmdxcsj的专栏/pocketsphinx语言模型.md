# pocketsphinx语言模型 - xmdxcsj的专栏 - CSDN博客





2015年01月20日 22:46:45[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1317








语言模型的训练参考官方文档

[http://cmusphinx.sourceforge.net/wiki/tutoriallm](http://cmusphinx.sourceforge.net/wiki/tutoriallm)


简单脚本如下


```
#!/bin/bash
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/:$PKG_CONFIG_PATH
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
#获取词列表
text2wfreq < all.txt | wfreq2vocab > all.vocab
#列举文本中出现的每一个n元语法。产生一个二进制文件
text2idngram -vocab all.vocab -idngram all.idngram < all.txt
#输出后缀为binlm的语言模型文件
idngram2lm -vocab_type 0 -idngram all.idngram -vocab \
     all.vocab -arpa all.arpa
#将arpa文件转化为二进制文件
sphinx_lm_convert -i all.arpa -o all.lm.DMP
```
pocketsphinx解码可以使用srilm的arpa文件，只需要做一次排序：

sphinx_lm_sort < unsorted.arpa > sorted.arpa





可以将lm文件和二进制dmp文件进行转化

sphinx_lm_convert -i model.lm -o model.dmp

sphinx_lm_convert -i model.dmp -ifmt dmp -o model.lm -ofmt arpa






