# FASTDFS  java  版本编译 - 建建的博客 - CSDN博客
2018年01月28日 17:50:11[纪建](https://me.csdn.net/u013898698)阅读数：356
  FastDFS Client目前在中央仓库是没有坐标的，为了解决这个问题，我们需要另辟蹊径。
第一种尝试：我尝试了从github上下载fastdfs-client源码并导入eclipse当中，但无法导入到eclipse当中。
第二种尝试：直接使用eclipse来下载github上的源码，这种方法证明是可行的，下面详细介绍如何操作。
1、在左侧空白处右键------>Import------->Import，如下图所示。
![](https://img-blog.csdn.net/20170411230402447?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、在弹出的对话框中展开Git，点击"Projects from Git"，然后点击"Next"，如下图所示。
![](https://img-blog.csdn.net/20170411230632885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、在弹出的对话框中点击"Clone URI"，然后点击"Next"，如下图所示。
![](https://img-blog.csdn.net/20170411230819824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4、我们会看到如下图所示的界面，我们需要向URI输入框中输入Fastdfs-Client在Github的上Git地址。
![](https://img-blog.csdn.net/20170411231036002?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      那么怎么在Github上找到git地址呢？我们在浏览器地址栏输入https://github.com/happyfish100/fastdfs-client-java并访问该网址，我们可以看到如下图所示界面，我们点击"Clone or download"右边的倒三角图标，就会下拉出一个框，在文本框中就是我们想要的git下载地址。我们复制该地址并输入到上图的URI输入框中。
![](https://img-blog.csdn.net/20170411231614765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5、我们向URI输入Git地址后，Host和Repository path会自动填充内容，我们点击"Next"。
![](https://img-blog.csdn.net/20170411232046283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
6、点击上图的"Next"之后会看到如下图所示界面，默认就是勾选的master，我们直接点击"Next"。
![](https://img-blog.csdn.net/20170411232255019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
7、默认情况下，Directory显示的是C盘下的目录，我们更改一下，更换成我们eclipse工程目录下，然后点击"Next"，如下图所示。
![](https://img-blog.csdn.net/20170411232437910?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
8、下图中我们选择"Import as general project"，表示按普通工程导入到eclipse，然后点击"Next"，如下图所示。
![](https://img-blog.csdn.net/20170411232735087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
9、不用做修改，直接点击"Finish"。
![](https://img-blog.csdn.net/20170411233009903?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
10、下面我们把通用工程转成web工程，我们在导入的fastdfs-client-java工程上右键，然后点击"Properties"，如下图所示。
![](https://img-blog.csdn.net/20170411233240149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
11、在打开的对话框中点击"Project Facets"，我们会看到右侧的信息，我们点击"Convert to faceted form.."，如下图所示。
![](https://img-blog.csdn.net/20170411233603042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
12、在下图中，我们我们不用做任何修改，直接点击"Apply"和"OK"即可。如果导入的工程是Dynamic Web工程的话，记得勾选上浅蓝色圈住的对话框。
![](https://img-blog.csdn.net/20170411233934933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
13、下面我们再把web工程转变为Maven工程，我们在工程上右键------->Configure------->Convert to Maven Project，如下图所示。
![](https://img-blog.csdn.net/20170411234553394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
14、刚生成的Maven工程是会报错的，因为它pom.xml文件中定义的jdk的版本过低（是1.5的），因此我们需要修改下jdk版本，我们打开pom.xml文件，并把jdk原来的版本1.5改为1.8（大家根据自己使用的jdk版本做相应的修改即可），如下图所示。
![](https://img-blog.csdn.net/20170411235024323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
15、修改完pom.xml文件中的jdk的版本后，我们需要重新更新下maven依赖，在工程上右键------->Maven-------->Update Project，如下图所示。
![](https://img-blog.csdn.net/20170411235314081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
16、系统默认给我们选择好了我们要更新的工程，我们直接点"OK"即可开始更新，如下图所示。
![](https://img-blog.csdn.net/20170411235512473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
17、更新maven后，工程的JRE版本自动变成1.8了，工程也不报错了，如下图所示。
![](https://img-blog.csdn.net/20170411235710817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
18、我们本地仓库中还没有fastdfs-client的包，因此我们要将该工程打包到本地maven仓库中，我们在工程上右键------->Run As------->Maven Install，如下图所示。
![](https://img-blog.csdn.net/20170412000048366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
19、打包好之后，我们到本地maven仓库看下是否已经有fastdfs-client的相关包了，我们发现已经生成好了，如下图所示。
![](https://img-blog.csdn.net/20170412000307304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
20、既然本地有了fastdfs-client的包了，那剩下的就是如何引用的问题了，这里以我的taotao-manager-web工程为例，在它的pom.xml文件中添加如下依赖。
**[html]**[view
 plain](http://blog.csdn.net/u012453843/article/details/70135826#)[copy](http://blog.csdn.net/u012453843/article/details/70135826#)
- <dependency>
- <groupId>org.csource</groupId>
- <artifactId>fastdfs-client-java</artifactId>
- <version>1.25</version>
- </dependency>
      保存并自动更新完maven依赖之后，我的taotao-manager-web工程的maven依赖中便多了一个fastdfs-client-java的文件夹，表示依赖成功。
![](https://img-blog.csdn.net/20170412000841957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1Mzg0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
