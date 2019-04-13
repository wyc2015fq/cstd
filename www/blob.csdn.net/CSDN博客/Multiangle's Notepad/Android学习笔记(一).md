
# Android学习笔记(一) - Multiangle's Notepad - CSDN博客


2016年03月06日 20:11:58[multiangle](https://me.csdn.net/u014595019)阅读数：982标签：[android																](https://so.csdn.net/so/search/s.do?q=android&t=blog)个人分类：[零零碎碎																](https://blog.csdn.net/u014595019/article/category/2176549)



最近准备开始做一个app项目，虽然我负责的是后台服务器部分，但是想了想也有必要对android有一定的了解，加上对安卓耳闻已久，不过一直没有做过，就趁此机会学习下吧。
由于我之前已经装了IDEA,所以一开始理所当然的想用IDEA来开发，按照的是这篇[文章](http://www.tuicool.com/articles/a2MNna)，结果折腾了半天，一开始用的Gradle，死活运行不了，后来发现是Gradle的问题，于是建立Module项目，这会虽然能跑了但是由于版本太低，功能太少，没有新下的Android Studio方便，于是便转型AS开发了。。。反正这两个使用习惯几乎一样，没什么转型问题。
言归正传，当刚建立项目的时候我是一脸懵逼的，这么多文件夹，这么多文件，你让我从哪里开始？
又折腾了一下午，在不停的懵逼-查资料/问同学的循环中，总算大概了解了android项目的结构。总的框架是这样
![这里写图片描述](https://img-blog.csdn.net/20160306194041378)
其中manifests中的AndroidManifest.xml是一个提纲式的文件，其中包含了所有主体内容的名字。例如该app的标签，名字，修饰风格，主体执行程序等
![这里写图片描述](https://img-blog.csdn.net/20160306194726416)
而MainActivity则可以认为是app主体程序的入口部分。整个程序由此而起。当然，如果在AndroidManifest.xml中加以修改，那么也可以由其他类作为入口。
![这里写图片描述](https://img-blog.csdn.net/20160306195405841)
可以看到，MainActivity 继承自 AppCompatActivity, 需要对onCreate函数进行重载，用以显示layout文件夹中的布局。
```python
setContentView(R
```
```python
.layout
```
```python
.activity
```
```python
_main)
```
```python
;
```
该函数负责显示布局（MVC中的V），而接下来的代码则负责对模块的控制（MVC中的C）。从上面可以看到，activity_main.xml才是布局文件的入口。回过头去看activity_main.xml，会发现虽然layout中有两个xml文件，其实content_main.xml是嵌套在activity_main.xml之中的
![这里写图片描述](https://img-blog.csdn.net/20160306200342439)
再来看res。在res文件夹中，有各种资源文件。其中layout是布局文件，之前已经提过了，menu_main.xml有什么用暂且不是很清楚，mipmap可能是用来放图的，在value文件夹下， colors.xml用来定义部件需要的颜色，strings.xml定义app中需要用到的字符串，styles.xml定义Theme。把这些放在value下而不是直接写在程序中，主要是为了提高复用性。此外，对于复杂的程序，这样做也可以减少修改的难度。当有外观上的改进需求时，可以很快搞定，不用再去程序中一个一个修改

