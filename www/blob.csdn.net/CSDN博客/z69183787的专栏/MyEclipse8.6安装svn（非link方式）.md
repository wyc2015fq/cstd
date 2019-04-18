# MyEclipse8.6安装svn（非link方式） - z69183787的专栏 - CSDN博客
2013年05月08日 16:03:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1062
个人分类：[IDE-Eclipse																[版本管理-Svn](https://blog.csdn.net/z69183787/article/category/2186151)](https://blog.csdn.net/z69183787/article/category/2176833)
此方法经试验可行，暂时可以作为最佳解决方案。
本文所使用的MyEclipse版本为:8.6.1
svn的eclipse插件版本为:1.6.17，下载地址：[http://subclipse.tigris.org/servlets/ProjectDocumentList?folderID=2240](http://subclipse.tigris.org/servlets/ProjectDocumentList?folderID=2240)
这几天家里两台电脑要做项目，换来换去不方便，于是想到了用svn来管理项目就方便多了，可是用MyEclipse8.6.1怎么也装不好svn的插件。如果用MyEclipse Configuration Center来安装的话总会出错，并且还会丢失一部分功能，比如MyEclipse HTML editor就会丢失，害我重装了好几遍。突然我想到了在公司的时候装vss,可以直接把vss的插件放入MyEclipse下的dropin目录中，再打开MyEclipse就可以安装好。于是我便按此方法，将svn插件复制到dropin目录中，虽然启动时报错了，但是能正常使用，下面是操作步骤：
一、下载svn的最新Eclipse插件，下载下来是一个名为site-1.6.17.zip的压缩包，将其解压。内容如下图所示：![](http://hi.csdn.net/attachment/201102/22/3972563_12983779196RrM.png)
二、在MyEclipse的安装目录下找到dropin目录，在其中新建一文件夹，取名“svn”，当然，也可以直接把那几个文件复制过来，但是为了不混淆其它插件，还是各自放在单独的文件夹里方便。
![](http://hi.csdn.net/attachment/201102/22/3972563_1298378114uIAg.png)
三、启动MyEclipse，稍等片刻，出现如下图所示的错误，不用管他，直接点ok。
![](http://hi.csdn.net/attachment/201102/22/3972563_12983779270xeo.png)
然后在window-preferences和show view中已经可以配置svn了，而且也不会出现其它错误。
![](http://hi.csdn.net/attachment/201102/22/3972563_12983780401vYE.png)
![](http://hi.csdn.net/attachment/201102/22/3972563_1298378089xXxp.png)
![](http://hi.csdn.net/attachment/201102/22/3972563_1298378096t7f4.png)
还在为装svn且不愿使用link方式安装的朋友可以试试此方法。
