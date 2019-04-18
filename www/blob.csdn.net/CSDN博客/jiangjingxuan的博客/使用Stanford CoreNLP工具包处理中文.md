# 使用Stanford CoreNLP工具包处理中文 - jiangjingxuan的博客 - CSDN博客





2017年02月07日 09:17:02[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2677








这几天刚刚接触自然语言处理，使用了Stanford CoreNLP工具。但毕竟是第一次用，所以遇到很多问题，现将解决方案记下（容易百度到的错误就不记了）。

其中用Stanford CoreNLP集合工具来处理中文的方法网上很少有说明，官方Demo也没有说明，特在此记下。

Stanford 提供了很多NLP工具，默认均直接支持英文，部分需要model支持中文。这是这些工具官方列表地址：http://nlp.stanford.edu/software/

部分工具列表如图：

![](http://blog.csdn.net/u014749291/article/details/51152007)![](https://img-blog.csdn.net/20160414161918368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中第一个Stanford CoreNLP工具可以说是其它工具的集合，功能相对强大。假如你只需要进行分词，则使用Stanford Word Segmenter工具即可。该工具支持中文。而且下载的文件中也有适用于中文的Demo，比较容易理解。但是如果你想使用集合工具Stanford CoreNLP处理中文，则网上相关资料很少，下载的Demo文件中只有处理英文的。

下面介绍怎么用集合工具Stanford CoreNLP处理中文：

注意：需要java8，怎么配合eclipse使用，自己百度，目前需要安装一个更新。

1、你需要下载Stanford CoreNLP，我下载的是3.6.0：http://stanfordnlp.github.io/CoreNLP/index.html#download

2、你需要下载中文支持包：stanford-chinese-corenlp-models.jar，我下载的也是3.6.0：http://stanfordnlp.github.io/CoreNLP/index.html#download

3、下载完毕后，Stanford CoreNLP需要解压，然后将stanford-chinese-corenlp-models.jar放到解压文件夹里，将文件夹中的所有.jar加入你建立的工程中。

![](https://img-blog.csdn.net/20160414162112103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后[测试](http://lib.csdn.net/base/softwaretest)代码与英文的Demo不同，代码很简单

public class test_demo {
    public static void main(String[] args) {
String props="StanfordCoreNLP-chinese.properties";
StanfordCoreNLP pipeline = new StanfordCoreNLP(props);
    Annotation annotation;
    //if  data from file
    //annotation = new Annotation(IOUtils.slurpFileNoExceptions(file));
     annotation = new Annotation("这家酒店很好，我很喜欢。");


    pipeline.annotate(annotation);
    pipeline.prettyPrint(annotation, System.out);
}
}


主要就是标红的代码。这个是stanford-chinese-corenlp-models.jar中的一个文件，可以用解压软件打开查看一下，里边默认定义了使用哪些工具，以及相应的设置，可以自己根据需要修改，箭头指示的为默认支持的工具，分词、分句、词性标注、命名实体识别、语法分析等等，下边是每个工具对应的参数设置，这样就可以用这一强大工具来处理中文了。

![](http://blog.csdn.net/u014749291/article/details/51152007)![](https://img-blog.csdn.net/20160414162246216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


结果截图：

![](http://blog.csdn.net/u014749291/article/details/51152007)![](https://img-blog.csdn.net/20160414161938978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20160414161950868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](http://blog.csdn.net/u014749291/article/details/51152007)

![](http://blog.csdn.net/u014749291/article/details/51152007)

![](https://img-blog.csdn.net/20160414162007244?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


