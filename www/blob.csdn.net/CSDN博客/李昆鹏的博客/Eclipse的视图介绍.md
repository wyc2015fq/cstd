
# Eclipse的视图介绍 - 李昆鹏的博客 - CSDN博客


2018年03月07日 19:52:50[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1959


------------------------------**Eclipse视图介绍**--------------------------------------------
Eclipse的组成
整体叫做视图，每一个小玩意叫做视图。
在eclipse中要想爽一下，必须先要有项目（工程）。
1.   视图，视窗，当前视图
视图由视窗组成
![](https://img-blog.csdn.net/20180307194729706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
调用视窗 –--》window （窗口）---》Show View（展示视窗）--》选择要调用的窗口
![](https://img-blog.csdn.net/20180307194736817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Consele就是控制台
Ant项目构建
Other其他的
---------------------------------
2.   视图就是某些视窗的组合
3.   视图还原
在视图操作中，我有可能有误操作，这个时候怎么办？
Window ---》Reset Perspective重置视图
![](https://img-blog.csdn.net/20180307194748284?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.   Package Explorer 包资源管理器（重点）
![](https://img-blog.csdn.net/20180307194758794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
a.该视图中显示当前工作空间中的所有以及项目中的文件。
但不显示项目下bin目录中的运行文件。
b.可以直接在该视图中创建项目，包，类，接口等。
5.Outline大纲视窗
![](https://img-blog.csdn.net/20180307194806393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
a.   该视图中显示当前代码视图中源文件中定义的类或者接口。
b.   当代码过长时，查找代码中的某一个类或者成员，在该视图中是最方便的。
c.   在代码中出现多次的名称要统一更改，也可以在该视窗中完成。
同时其他代码如果使用了该变量，那么也会一并改变。
-------------------------java项目创建---------------------------
1.   使用eclipse
第一种方法
![](https://img-blog.csdn.net/201803071948128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第二种方法
![](https://img-blog.csdn.net/20180307194820952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第三种方法
![](https://img-blog.csdn.net/20180307194826473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
创建项目，项目的名字全部小写
![](https://img-blog.csdn.net/20180307194839353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180307194850942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
工程中文件夹介绍
![](https://img-blog.csdn.net/20180307194858617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Navigator视窗显示的内容跟项目中的文件夹一模一样
![](https://img-blog.csdn.net/20180307194905209?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180307194913479?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

------------------------创建包和类---------------------
**包的名字都是小写，****src****为资源包的意思，我们创建的包都是在资源包里面**
![](https://img-blog.csdn.net/20180307194925453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**创建类的时候先用鼠标选中包。**
![](https://img-blog.csdn.net/20180307194934429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
--------------------关联源代码-------------------------------------------
**Java.Lang****下的类可以直接使用**
**按住****Ctrl****，用鼠标点击要查看的类**
![](https://img-blog.csdn.net/20180307195001456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**如果没有关联源代码会出现以下情况**
![](https://img-blog.csdn.net/20180307195011701?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击这个关联源代码
![](https://img-blog.csdn.net/20180307195015791?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们一般选择压缩包形式的源码
![](https://img-blog.csdn.net/20180307195020763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择源码压缩包
![](https://img-blog.csdn.net/20180307195024910?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击关联OK
![](https://img-blog.csdn.net/20180307195032167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-------------------------console控制台视图----------------------
![](https://img-blog.csdn.net/20180307195042514?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
a.   用于显示程序的运行结果，相当于DOS命令。
b.   如果有多个控制台同时运行。比如：服务器端和客服端。
可以通过控制台上的小屏幕按钮进行控制台的切换。
c.   通过点击小方框，进行控制台的强制停止，相当于DOS中的Ctrl + C。
--------------------------TypeHierarchy层次视窗---------------------------
a.   用于显示类之间的层次（继承）关系。
b.   在当前类上，按F4就可以切换到该窗口，并查看层次关系。
查看源码，当前类上F3，或者Ctrl + 鼠标点击。
![](https://img-blog.csdn.net/20180307195057973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
----------------------------工作空间基本配置-------------------------------------
注意：在工作空间的设置对于该空间中的所有项目都有效。
更换了工作空间，需要重新设置。
1.   配置java的编译和运行环境版本
![](https://img-blog.csdn.net/20180307195106280?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
编译级别的选择
![](https://img-blog.csdn.net/20180307195110848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
运行环境级别
![](https://img-blog.csdn.net/20180307195116171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所有的软件都是高版本兼容低版本
Jdk1.6编译的class文件肯定能在jdk1.7上运行。
Jdk1.7编译的class文件不一定能在jdk1.6上运行。
**增加运行环境**
![](https://img-blog.csdn.net/20180307195123892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Standard VM代表标准JVM
![](https://img-blog.csdn.net/20180307195128123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择JRE文件
![](https://img-blog.csdn.net/20180307195133543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Windows – preference– java
|--compiler设置5.0以上，要和运行环境版本一致
|--installed JREs： 可以指定多个jre环境，但是仅有一个是当前版本的
要和编译器一致，或者高于编译器版本。
**如果采用高版本编译，用低版本****jre****运行，会出现异常。**
**----------------------****调整字体****----------------------------------------**
**Windows—perferenec–General – Appearance – Color and Fonts – Basic—Text Font—Edit**
**右边选择编辑即可**
**字体选择****Courier New****比较标准，个人爱好**
![](https://img-blog.csdn.net/20180307195147675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择字体
![](https://img-blog.csdn.net/20180307195154749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-------------------------**注释模板配置**---------------------------
取消默认模板
在首选项设置板面中
Windows—Perferenec --Java—CodeStyle – Code Templates
右边选择Code，将不希望出现默认注释的代码块中的注释去掉
新建模板注释
在首选项设置板面中
Windows—Perferenec --Java—CodeStyle – Code Templates –Comments—Types
选择Edit编辑注释模板.
![](https://img-blog.csdn.net/20180307195202522?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**选择****Methods****是设置方法模板注释。**
![](https://img-blog.csdn.net/20180307195220638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


