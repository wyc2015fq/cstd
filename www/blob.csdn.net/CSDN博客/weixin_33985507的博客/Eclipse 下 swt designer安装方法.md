# Eclipse 下 swt designer安装方法 - weixin_33985507的博客 - CSDN博客
2012年07月26日 11:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
First ------首先下载swt-3.7.2-win32-win32-x86
 进入 http://www.eclipse.org/swt/ 下载
进入页面后 有一个小段的标题为Releases
可以通过点击下面的相关超链接
找到相关文件 进行下载
放到 eclipse下的plugins文件夹下（整个文件夹都放进去哦）
然后 自己建个工程
在Eclipse IDE的Package Explorer视图中单击右键，在弹出的快捷菜单中选择命令选择“Existing Projects into Workspace”选择“Select archive file”，单击“Browse”按钮，定位到之前下载的SWT工具包，此时该对话框的项目列表中会显示一个org.eclipse.swt项目，选定该项目并单击“Finish”按钮。打开项目的构建路径配置窗口，在该对话框中选择“Projects”选项卡在该页面中，单击“Add”按钮，在弹出的项目列表中选择“org.eclipse.swt”，单击“OK”按钮完成构建路径配置。注意org.eclipse.swt中包含SWT的源代码压缩文件，通过将Eclipse SWT项目加入到项目的“依赖项目”列表中，可以方便地查看这些源代码以及相应的Java Doc文件。
这个工程就这么留着哦~~~
Second------下面安装 swt Designer
[http://www.swt-designer.com/](http://www.swt-designer.com/) 这个网址已经不存在了，点击下面的网址查看相对应的Eclipse的版本 对应的网址
[https://developers.google.com/java-dev-tools/download-wbpro?hl=zh-CN](https://developers.google.com/java-dev-tools/download-wbpro?hl=zh-CN)
![](https://img-my.csdn.net/uploads/201206/09/1339214543_1050.jpg)
然后，打开eclipse
在help菜单里有
选择  install new software
![](https://img-my.csdn.net/uploads/201206/09/1339214696_7880.jpg)
进入install new software 后，点击 Add选项 在弹出对话框中输入网址
![](https://img-my.csdn.net/uploads/201206/09/1339214773_7288.jpg)
![](https://img-my.csdn.net/uploads/201206/09/1339214788_2008.jpg)
由于我已经装过了，所以显示Duplicate location 如果没装过，直接ok就好了
然后然你选择安装项目，怕麻烦的话就 select all~
然后finish就进行在线安装了，在安装过程中可能会出现 unsign 的警告，继续安装即可，安装之后会提示重启eclipse，重启后，就可以使用了。
在新建工程中选择 other
然后然你选择工程
![](https://img-my.csdn.net/uploads/201206/09/1339214971_5790.jpg)
 选择swt就这个就可以了。
之后再那个SWT/JFace那个工程里在建个项目就好了，选择dialog啥的都可以
![](https://img-my.csdn.net/uploads/201206/09/1339237677_6722.png)
工程建完后，把刚才那个留着的工程引入到这个工程里，就可以使用了(好像不用引用也成)！！
![](https://img-my.csdn.net/uploads/201206/09/1339239374_4627.jpg)
和Qt,VC之类的都差不多了哦~~
