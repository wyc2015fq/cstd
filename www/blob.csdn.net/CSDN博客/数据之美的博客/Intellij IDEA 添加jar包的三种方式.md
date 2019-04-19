# Intellij IDEA 添加jar包的三种方式 - 数据之美的博客 - CSDN博客
2018年12月25日 11:21:13[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：76
**一.直接复制：(不推荐)**
方法：直接将硬盘上的jar包复制粘贴到项目的lib目录下即可。
![](https://img-blog.csdnimg.cn/20181225112113470)
**注意：**
1.对于导入的eclipse项目，该方式添加的jar包没有任何反应，用make编译项目会报错
2.对于在idea中创建的项目，该方式添加jar包，编译不报错，但是打开Structure会有"...jar ... is missing ..."的提示，需要点击"Fix"或红灯泡解决。
![](https://img-blog.csdnimg.cn/20181225112113488)
![](https://img-blog.csdnimg.cn/20181225112113506)
**二.通过Modules的Dependencies添加：(推荐)**
1.打开 File -> Project Structure （Ctrl + Shift + Alt + S）
![](https://img-blog.csdnimg.cn/20181225112113526)
2.单击 Modules -> Dependencies -> "+" -> "Jars or directories"
![](https://img-blog.csdnimg.cn/20181225112113543)
![](https://img-blog.csdnimg.cn/20181225112113561)
3.选择硬盘上的jar包
4.Apply -> OK
**三.通过Libraries添加：(强烈推荐)**
1.打开 File -> Project Structure （Ctrl + Shift + Alt + S）
![](https://img-blog.csdnimg.cn/20181225112113526)
对于新导入的项目，会自动导入lib目录下的相关jar包，我们需要将这些自动添加进来的jar包删除，如图：
![](https://img-blog.csdnimg.cn/20181225112113599)
![](https://img-blog.csdnimg.cn/20181225112113617)
![](https://img-blog.csdnimg.cn/20181225112113633)
2.单击 Libraries  -> "+" -> "Java" -> 选择我们导入的项目主目录，点击OK
![](https://img-blog.csdnimg.cn/20181225112113656)
![](https://img-blog.csdnimg.cn/20181225112113672)
![](https://img-blog.csdnimg.cn/20181225112113690)
注意：在弹出的方框中点击“Cancel”,取消将其添加到Module中
![](https://img-blog.csdnimg.cn/20181225112113709)
此时，lib目录创建成功，删除目录中添加进来的多余内容，重新添加需要的jar包
![](https://img-blog.csdnimg.cn/20181225112113729)
![](https://img-blog.csdnimg.cn/20181225112113746)
![](https://img-blog.csdnimg.cn/20181225112113760)
![](https://img-blog.csdnimg.cn/20181225112113781)
到此，Libraries创建成功。
3.回到Modules菜单，选中之前导入的项目，点击“Dependencies”,点击“+” -> “Library”,将刚才创建成功的Library目录加入
![](https://img-blog.csdnimg.cn/20181225112113800)
![](https://img-blog.csdnimg.cn/20181225112113819)
![](https://img-blog.csdnimg.cn/20181225112113838)
4.将不是项目lib目录下的其他jar包（如：tomcat相关jar包），也通过该方式加入
![](https://img-blog.csdnimg.cn/20181225112113856)
![](https://img-blog.csdnimg.cn/20181225112113873)
到此，所有jar包添加成功！
**注意：**
1.图标的区别：
   一种是添加项目lib目录下的jar包的图标，像文件夹；
   一种是在其他目录添加的jar包，像柱状图。
2.以后要添加新的jar包，只要在Library目录中之前创建的项目lib目录下添加进去即可
