# eclipse3.7+resin4.0集成配置小结 - =朝晖= - 博客园
# [eclipse3.7+resin4.0集成配置小结](https://www.cnblogs.com/dhcn/p/7100267.html)
1、插件不要用improve公司的了，那个太老了。直接用resin官方的，用eclipse的help->install new software功能，地址用：[http://www.caucho.com/eclipse/](http://www.caucho.com/eclipse/) 这个即可，这个地址得到是比较新的resin版本的插件。
2、剩下的Server添加配置方法就和这个教程([http://www.ibm.com/developerworks/cn/opensource/os-eclipse-tomcat/](http://www.ibm.com/developerworks/cn/opensource/os-eclipse-tomcat/))配置tomcat的步骤一样。
3、最后配置好，可能会发生tools.jar找不到而启动出错的情况。选择相关联的Web项目，右键单击项目，选择properties->[Java](http://lib.csdn.net/base/java)build path->Libraries,选择Jre System Libraries,选择edit.一层一层进去以后，到一个界面下，添加这个tools.jar即可，这个jar文件在jdk的lib目录下面。

