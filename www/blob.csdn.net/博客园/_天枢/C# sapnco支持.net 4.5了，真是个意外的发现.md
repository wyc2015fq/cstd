# C# sapnco支持.net 4.5了，真是个意外的发现 - _天枢 - 博客园
## [C# sapnco支持.net 4.5了，真是个意外的发现](https://www.cnblogs.com/yhleng/p/7404427.html)
2017-08-21 14:59 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7404427)
意外篇：
                   需要用C#写一个RFC直连的类库，需要引用sapnco.dll   sapnco_utils.dll两个文件
          之前都是从网上下载的sapnco3.0，引用开发，在win10机器上使用没有问题，然而开发好的类库，在win8系统上给其它工具使用
           一直报错，提示我找不到sapnco.dll，这个问题让我抓狂了好久。我已经将sapnco  sapnco_utils 和我一类型文件，一起打包过去了。
           怎么就提示我找不到sapnco呢。
           于是就怀疑是不是版本的问题。但是这已经是我从官网上下载的最新的了。怎么解决，怎么解决，抓狂中………………
           难道要放弃，后来一个偶然发现，所有问题迎刃而解。激动有木有。……。…。。
          －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
                   项目上在使用SAP公司的其它工具就是SAP TAO ；这是做sap自动化测试的工具，无意间在这个工具目录下发现了sapnco.dll  sapnco_utils.dll这两个文件
           当时有点小激动，为什么呢，因为这个工具在N多个系统上都可以使用。于是就果断的偿试，使用这个工具带的两个dll.
           果然，写好的类库，在用其它工具去掉果然问题解决了……（当时其中还有插曲）…………
　　　 使用sap tao工具提供的sapnco.dll ; sapnco_utils.dll     进行开发的时候，按照之前惯例，一样改成.net 2.0，结果悲剧了，提示我找不到
              using SAP.Middleware.Connector;命名空间…………
            当时有点蒙，这是个什么情况，不是官方说，最新的只支持.net 2.0吗。
            果断，改回.net 4.5问题解决。有点不可思义，只能规于sap公司为自己产品提供便利……
![](https://images2017.cnblogs.com/blog/1149221/201708/1149221-20170821145532136-1421989616.png)
NCO4.0下载地址：[http://download.csdn.net/download/xzdylyh/10183729](http://download.csdn.net/download/xzdylyh/10183729)
 希望对遇到同样问题的有所帮助。
