# MapReduce基础开发context.write注意new text("")多出一列的问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月19日 10:31:23[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5205
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、问题描述：在MapReduce中代码中，Map输出context.write(okey,new text(""))，Reduce也是context.write(okey,new text(""))输出；

       查询字符长度，发现多了一位，如果用|字符隔开，发现最后一个字段会多出一位。




2、查核发现，new Text("")会增加一个空列（列分隔符是\t），导致多出一个隐形字符的长度；

       注意如果输出context.write(oKey,oValue)，就是key和value都带字符，在key和value中间也会有一个空格字符存在。




3、处理：在Reduce输出时用null替代new text("")，如context.write(oKey,null)，注意Map中不能有null，如context.write(oKey,null)，这样在传递给reduce时会出现空值错误。





