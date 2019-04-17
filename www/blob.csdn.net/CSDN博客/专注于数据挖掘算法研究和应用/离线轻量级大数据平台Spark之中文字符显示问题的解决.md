# 离线轻量级大数据平台Spark之中文字符显示问题的解决 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月23日 15:31:02[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3630








问题：spark读取文本文件转化成JavaRDD后发现中文字符显示乱码。

在spark-shell环境里执行：System.getProperty("file.encoding")，返回GB2312，和文本文件UTF-8编码不一样。

解决：在spark的Java代码中加入字符集设置即可。

//第一步：设置Java环境字符集，避免中文乱码
Properties pps=System.getProperties();  
pps.setProperty("file.encoding","UTF-8"); 



