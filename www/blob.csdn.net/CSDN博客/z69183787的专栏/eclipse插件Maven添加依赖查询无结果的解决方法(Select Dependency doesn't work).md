# eclipse插件Maven添加依赖查询无结果的解决方法(Select Dependency doesn't work) - z69183787的专栏 - CSDN博客
2014年03月27日 15:06:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13669
在eclipse中用过maven的可能都遇到过这种情况，我以前一直在search.maven里面搜索，然后添加pom信息。
今天在网上搜索时，找到了一个解决方法，在这里分享一下。
第一步，在preferences里面选择maven，选中“Download repository index updates on startup"
![](https://img-blog.csdn.net/20130930153350562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXNlYTUzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第二步，打开Maven仓库
![](https://img-blog.csdn.net/20130930153404890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXNlYTUzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第三步，在全局仓库上右键选择“Full Index Enabled"
![](https://img-blog.csdn.net/20130930153434000)
选择之后，maven会下载一个近百兆的索引文件，下载完会更新索引，需要的过程比较长，可能会卡，建议此期间不要做其他事情
![](https://img-blog.csdn.net/20130930153823828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXNlYTUzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这之后我们就可以在Select Dependency中搜索到内容了
![](https://img-blog.csdn.net/20130930153455765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXNlYTUzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

英文链接：[http://stackoverflow.com/questions/14059685/eclipse-maven-search-dependencies-doent-work](http://stackoverflow.com/questions/14059685/eclipse-maven-search-dependencies-doent-work)
