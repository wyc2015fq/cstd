# 关于word插入特殊符号不显示的问题及解决 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月24日 10:37:46[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5496








1、问题：在编辑数学公式时，需要用到特殊符号，word2007版本以上，可以直接插入公式，但这种公式无法直接复制，所以笔者多数情况下就直接插入特殊符号。最近发现插入特殊符号后不显示、没反应。




2、解决：关闭文档结构图就可以正常显示插入的特殊符号。




百度经验有说：

http://jingyan.baidu.com/article/eae07827b244ad1fed548545.html


但原因不知，网上还提供了下面的说法，供参考：

一般有有两种方法，对应两种原因，其一，office文件设置更改，其二当前状态打开了“文档结构视图”。

尝试如下解决手段：

1、[运行]——输入cmd——[确定]

2、输入cd C:\program files\microsoft Office\office11\addins 完成后按[enter]键

3、输入regsvr32 /u SYMINPUT.DLL 完成后再按下[Enter]键

4、输入regsvr32 SYMINPUT.DLL 完成后再按下[Enter]键

打开Word文档，重新插入符号，问题解决。




