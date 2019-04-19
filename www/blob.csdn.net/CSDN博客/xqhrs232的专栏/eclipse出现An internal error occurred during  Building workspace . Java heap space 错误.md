# eclipse出现An internal error occurred during: "Building workspace". Java heap space 错误 - xqhrs232的专栏 - CSDN博客
2018年11月28日 13:53:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：39
原文地址::[https://blog.csdn.net/dengpeng1994/article/details/53635399](https://blog.csdn.net/dengpeng1994/article/details/53635399)
出现这个错误，eclipse 会卡死，以及自动退出
解决方案 
工程根目录 找到项目中.project文件
删除这两处
  第一处：
 <buildCommand>
         <name>org.eclipse.wst.jsdt.core.javascriptValidator</name>
         <arguments>
         </arguments>
<ildCommand>
第二处：
 <nature>org.eclipse.wst.jsdt.core.jsNature</nature> 
--------------------- 
作者：大帅哥程序员 
来源：CSDN 
原文：https://blog.csdn.net/dengpeng1994/article/details/53635399 
版权声明：本文为博主原创文章，转载请附上博文链接！
