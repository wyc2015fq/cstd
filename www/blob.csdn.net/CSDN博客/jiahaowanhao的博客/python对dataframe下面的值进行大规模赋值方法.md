
# python对dataframe下面的值进行大规模赋值方法 - jiahaowanhao的博客 - CSDN博客


2018年06月12日 13:52:09[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：375


[python对dataframe下面的值进行大规模赋值方法](http://cda.pinggu.org/view/25805.html)
今天小编就为大家分享一篇python 对dataframe下面的值进行大规模赋值方法，具有很好的参考价值，希望对大家有所帮助。一起跟随小编过来看看吧
假设我们有一个数据集，列名叫status下面有100万的数据，其中包装 “HUMAN_REFUSE”,”SYS_REFUSE”,”HUMAN_AGREE”,”SYS_APPROVING”,”REVIEWING”, “HUMAN_CANCEL”,”SYS_AGREE”,”SYS_CANCEL” 这些字段，我现在想将这些字段分别赋值，并且新创造一列，列名为申请结果。
我们使用map函数即可：
df.申请结果 = df.申请结果.map({
"HUMAN_REFUSE":"人工拒绝",
"SYS_REFUSE":"系统拒绝",
"HUMAN_AGREE":"人工通过",
"SYS_APPROVING":"系统审核中",
"REVIEWING":"人工复核",
"HUMAN_CANCEL":"人工取消",
"SYS_AGREE":"系统通过",
"SYS_CANCEL":"系统取消"})
以上这篇python 对dataframe下面的值进行大规模赋值方法就是小编分享给大家的全部内容了

