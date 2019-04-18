# svn查看指定release的revision号 - z69183787的专栏 - CSDN博客
2015年01月16日 13:15:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1859
假设每个release都tag了，可以运行svn info /path/to/svn/tag，返回如下信息:
**[plain]**[view
 plain](http://blog.csdn.net/flying881114/article/details/16858819#)[copy](http://blog.csdn.net/flying881114/article/details/16858819#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75695/fork)
- Path: [path]  
- URL: [url]  
- Revision: [当前最新的revision号]  
- Node Kind: directory  
- Last Changed Author: [author]  
- Last Changed Rev: [指定path最后一次更改的revision号] <- 这个就是我们想要的信息  
- Last Changed Date: YYYY-MM-DD hh:mm:ss  
举个例子，假设要得到Apache commons math的release 2.0的revision号
**[plain]**[view
 plain](http://blog.csdn.net/flying881114/article/details/16858819#)[copy](http://blog.csdn.net/flying881114/article/details/16858819#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75695/fork)
- svn info https://svn.apache.org/repos/asf/commons/proper/math/tags/MATH_2_0  
- 
- Path: MATH_2_0  
- URL: https://svn.apache.org/repos/asf/commons/proper/math/tags/MATH_2_0  
- Repository Root: https://svn.apache.org/repos/asf  
- Repository UUID: 13f79535-47bb-0310-9956-ffa450edef68  
- Revision: 1544028  
- Node Kind: directory  
- Last Changed Author: niallp  
- Last Changed Rev: 909837  
- Last Changed Date: 2010-02-13 23:18:38 +0800   
可以看到，对应的revision是909837
