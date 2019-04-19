# 使用EditPlus和批处理删除文本重复内容行 - xqhrs232的专栏 - CSDN博客
2016年04月18日 11:53:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3693
原文地址::[http://429006.com/article/technology/3320.htm](http://429006.com/article/technology/3320.htm)
相关文章
1、
# EditPlus应用技巧----[http://wenku.baidu.com/link?url=YZKfRGMGxsXAszAcS1bqm2QwCE0CFyVzts2xegT8NOG4L1poCieY9nHax_nrPpqt8v7avU3_lx0rSMyIUfT2Umx1oAomPtR1oTdcKGgwNqq](http://wenku.baidu.com/link?url=YZKfRGMGxsXAszAcS1bqm2QwCE0CFyVzts2xegT8NOG4L1poCieY9nHax_nrPpqt8v7avU3_lx0rSMyIUfT2Umx1oAomPtR1oTdcKGgwNqq)

由于要处理一个两万多行的数据，但要删除其中内容重复的行，所以找了几个方法：
1、使用EditPlus-工具-排序，勾选“删除重复”。可惜不想把内容排序，却被自动排序了。（勾选"区分位置"，好像没用？），这个效率相当快～差不多是瞬间完成的，之所以没放弃editplus，确实它的优点有很多。
![](http://oi61.tinypic.com/2edbkti.jpg)
2、用sed替换重复内容，使用命令 -n "G;s/\n/&&/;/^\(.*\n\).*\n\1/d; s/\n//;h;P" "$(FilePath)" （这里是在editplus里的使用，注意$(FilePath)必须是带路径的文件名）。当然sed不需依赖editplus，可以单独使用的。优点：删除重复内容后，顺序结构不变。缺点：不支持UTF-8，处理大文件时（2w多行）就不行了。
3.使用一个批处理
@echo off
:: 删除重复的行，但不能保留空行
:: 对不符合变量命名规则、变量个数超过限制的文本都无法正确处理
(echo 清除重复行后的文件内容：& echo.)>str_.txt
for /f "delims=" %%i in (testText.txt) do (
if not defined %%i set %%i=A & echo %%i>>str_.txt)
:: start str_.txt
当然也不支持utf8，效率也比较慢，处理2w多行用了1分多钟，不过排序没有改变:)。
原文地址：
[http://www.fantxi.com/blog/archives/661/](http://www.fantxi.com/blog/archives/661/)
