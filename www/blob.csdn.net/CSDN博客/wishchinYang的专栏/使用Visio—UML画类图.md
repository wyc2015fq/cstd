# 使用Visio—UML画类图 - wishchinYang的专栏 - CSDN博客
2015年07月06日 11:25:53[wishchin](https://me.csdn.net/wishchin)阅读数：36652
在一个VS工程中，由于类的个数较多，而参数描述不是特别清晰。若此工程的生命周期较长，则有必要对工程进行完整分析，给出完整的文档。需要画出类图，并对每个成员进行详细描述。
**一、画出类图**
        在VIsio中，新建软件—UML模型图；在UML静态结构下，列出了各种静态结构；
        在模型资源管理器中，右键顶层包，选择 新建——类，可建立一个空类，在图表中显示顶层包；
        双击建立好的类，或者右键-属性，可以对类进行编辑；
        选择类别 第二行——特性，便可以添加类成员；
![](https://img-blog.csdn.net/20150706105627220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20150706105812538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         Visio的预定义类型包含了C++、VB、C#等基本类型，而自己设计的类和结构体有时包含了其他类型，这就需要自己定义数据类型，并在UML设置特定显示。
**二、建立自定义数据类型**
          选择界面选项：UML——包，显示UML包的属性；
          选择新建，在 包 这个属性栏自己命名 自己声明的包空间：比如Eigen
          建立完成后，在模型资源管理器 里面显示出了自己建立的包空间
![](https://img-blog.csdn.net/20150706110900658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
           右键包空间名，可建立新的数据类型，
![](https://img-blog.csdn.net/20150706111428102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         选择界面UML选项：UML——选项， UML文档标签，设定使用时显示的数据选项
![](https://img-blog.csdn.net/20150706111710627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**三、建立成员变量**
         建立好自定义数据类型后，可以添加自定义的数据成员：
![](https://img-blog.csdn.net/20150706112043424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后，在UML显示的类图为，
![](https://img-blog.csdn.net/20150706112239895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         其中成员前面的 +为属性为Public； -为属性Private； #为属性protect。
