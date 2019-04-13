
# maven解决依赖冲突 - bitcarmanlee的博客 - CSDN博客


2017年05月03日 16:35:19[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：710


maven项目中，各种依赖多了以后，很容易依赖就会产生冲突。在Intellij中，我们可以快速地定位到依赖产生冲突的位置。
View->Tool Windows->Maven Projects。如果依赖有冲突，IDE中会自动显示出来。
![这里写图片描述](https://img-blog.csdn.net/20170503163327382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
IDE里面就很清楚地告诉我们，com.xiaomi.data:commons-spark这个jar包，有两个地方都有引用，而且这两个地方的版本不一致，所以最后产生了冲突！

