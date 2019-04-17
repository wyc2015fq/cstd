# j2me程序发布到手机 - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年10月22日 10:18:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2106标签：[j2me																[手机																[游戏](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)





a.配置程序(.jad)文件：
      —Required：MicroEidtion configuration 要和手机支持相符合,现在大部分都是1.1版；
      —Midlets：Name--游戏名称，Icon--游戏图表，Class--游戏主入口(就)是MidLet所在的类；
      —配置上面两点后保存；
    b.发布程序：
      —右击应用，选择"J2ME->Create Package"生成(.jar)和(.jad)文件，在应用目录"deployed"文件下；
      —智能手机拷贝两个文件，运行.jad安装；
      —非智能手机直接拷贝.jar文件运行；





参考：[http://blog.csdn.net/jiezigege/archive/2008/09/17/2941127.aspx](http://blog.csdn.net/jiezigege/archive/2008/09/17/2941127.aspx)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=j2me&t=blog)




