# 基于Visual Studio Code搭建Python开发环境 - zhusongziye的博客 - CSDN博客





2018年01月15日 22:16:28[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：15409










## 安装Visual Studio Code




### 官网下载Visual Studio Code


下载路径：https://code.visualstudio.com/Download，注意系统类型和版本；

![](https://img-blog.csdn.net/20180115221634848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






注：本作者的实例都是基于windows10 64位+Python3.6.4+VSCodeSetup-x64-1.19.1版本；




### 安装VSCode


1.双击下载的exe安装包，点击“下一步”；

![](https://img-blog.csdn.net/20180115221655462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



2.选择同意协议，点击“下一步”

![](https://img-blog.csdn.net/20180115221706772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



3.设置安装路径，点击“下一步”

![](https://img-blog.csdn.net/20180115221717085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



4.选择开始菜单文件夹，此处默认，点击“下一步”

![](https://img-blog.csdn.net/20180115221729734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



5.设置其它快捷方式，此处根据个人需要而定，点击“下一步”

![](https://img-blog.csdn.net/20180115221743097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



6.一切设置好，准备就绪后，点击“安装”

![](https://img-blog.csdn.net/20180115221755512?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



7.进入安装进度界面

![](https://img-blog.csdn.net/20180115221808543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



8.点击“完成”完成VSCode的安装，并打开VSCode

![](https://img-blog.csdn.net/20180115221820245?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20180115221834378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 配置与验证


1.输入 install Python或者点击工具和语言，在左侧列表中选择Python进行安装

![](https://img-blog.csdn.net/20180115221856026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



2.新建一个文件夹，里面创建一个XXX.py文件，使用vscode左上角文件->打开文件，打开XXX.py文件，并输入



if __name__=="__main__":print("hello vscode")

3.点击“调试”，选择Python并执行调试，此时在当前文件夹中会生成一个.vscode文件夹，其中有一个launch.json文件，记录了一些Debug的设置。

![](https://img-blog.csdn.net/20180115221915297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



4.打开launch.json文件

![](https://img-blog.csdn.net/20180115221931966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



5.编辑launch.json文件，增加图中红框中内容并保存

![](https://img-blog.csdn.net/20180115221948738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



6.按F5调试程序，可以看到控制台上的输出，到这里关于Visual Studio Code编写Python的环境就已经搭建好了

![](https://img-blog.csdn.net/20180115222011814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 问题汇集


1.安装Python后，打开文件提示没有安装pylint，根据提示点击安装pylint，安装pylint后，提示更新安装pip，根据提示命令安装即可

![](https://img-blog.csdn.net/20180115222036945?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180115222055104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

















