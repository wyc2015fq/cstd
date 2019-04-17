# 海康sdk运行java环境 - zj360202的专栏 - CSDN博客





2017年03月30日 16:37:14[zj360202](https://me.csdn.net/zj360202)阅读数：4955








如果系统是64位的就是64位的sdk，如果是32位就用32位的，和是什么系统没有关系，只是系统位数有关

我这边是win7 64位的

环境有intellij和netbeans，都调试成功了




## 非常抱歉，以前调试有一些问题，突然就过了，后面再次调试也不OK，其实最大得问题是库少了，下面说的是将5个库移动到项目目录，其实应该将库目录下面的所有dll移动过去，我记得应该要6个dll就够了，不过忘记除了下面的5个是哪个一个了，建议全部移动过去，然后再删除试试




### 下载海康sdk就自己去官网上面下载然后解压，我的解压路径是:F:\appsoft\CH-HCNetSDK(Windows64)V5.2.3.3_build20160623

懒人下载：[http://download.csdn.net/detail/zj360202/9799116](http://download.csdn.net/detail/zj360202/9799116)  intellij版本

一.intellij

1.新建一个项目hik

2.将F:\appsoft\CH-HCNetSDK(Windows64)V5.2.3.3_build20160623\Demo示例\4- Java 开发示例\ClientDemo-NetBeansPro\src下面的ClientDemo全部得到的hik项目的src下载

3.将F:\appsoft\CH-HCNetSDK(Windows64)V5.2.3.3_build20160623\库文件  下面的HCNetSDKCom文件夹，AudioRender.dll，HCCore.dll，HCNetSDK.dll，PlayCtrl.dll和SuperRender.dll复制到hik项目根目录下面

4.在hik项目下面建立lib文件夹，将jna.jar，examples.jar放入lib下面，然后双右这两个jar，选中add as library

5.然后选中ClientDemo.java，右击选择运行，就完成了

目录结构

![](https://img-blog.csdn.net/20170330163352785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





运行结果：

![](https://img-blog.csdn.net/20170330163501378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





二.netbeans

1.打开netbeans

2.选择打开项目-->选中F:\appsoft\CH-HCNetSDK(Windows64)V5.2.3.3_build20160623\Demo示例\4- Java 开发示例\ClientDemo-NetBeansPro   然后点击"打开"

3.在上述ClientDemo-NetBeansPro目录同级，如果没有lib就建立一个，然后将F:\appsoft\CH-HCNetSDK(Windows64)V5.2.3.3_build20160623\库文件  下面的HCNetSDKCom文件夹，AudioRender.dll，HCCore.dll，HCNetSDK.dll，PlayCtrl.dll和SuperRender.dll复制到刚才的lib目录下面

3.右击项目。选中属性-->运行-->工作目录-->设置成F:\appsoft\CH-HCNetSDK(Windows64)V5.2.3.3_build20160623\Demo示例\4- Java 开发示例\lib

4.就可以直接运行了，效果和上面是一样的



