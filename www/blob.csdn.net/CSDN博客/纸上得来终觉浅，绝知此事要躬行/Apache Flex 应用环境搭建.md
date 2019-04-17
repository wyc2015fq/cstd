# Apache Flex 应用环境搭建 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年05月20日 14:47:36[boonya](https://me.csdn.net/boonya)阅读数：5939








### 1.Flash Builder插件下载

Flash Builder：[http://www.adobe.com/products/flash-builder.html](http://www.adobe.com/products/flash-builder.html)



或者您可以在：[https://creative.adobe.com/products/flash-builder](https://creative.adobe.com/products/flash-builder)



注：Adobe官网上下载比较慢，你可以找一些网盘资源进行下载(64bit下载：[http://adobe-flash-builder-64-bit.updatestar.com/](http://adobe-flash-builder-64-bit.updatestar.com/)或者[http://www.jb51.net/softs/103858.html](http://www.jb51.net/softs/103858.html))。

推荐下载：Flash Builder 4.5  [http://pan.baidu.com/share/link?shareid=142476&uk=2198762756](http://pan.baidu.com/share/link?shareid=142476&uk=2198762756)[](http://adobe-flex-builder.en.softonic.com/)

下载参考：[http://www.bidepan.com/430](http://www.bidepan.com/430)



### 2.Flash Player 插件下载

Flash Player：[http://www.adobe.com/support/flashplayer/downloads.html](http://www.adobe.com/support/flashplayer/downloads.html)



注：下载并安装 Flash Player 最新的 Debugger 调试版，安装时关闭所有浏览器。



### 3.AIR (Adobe Integrated Runtime)下载

Adobe Integrated Runtime：[http://get.adobe.com/cn/air/](http://get.adobe.com/cn/air/)



注：下载并安装最新的 AIR 虚拟机。

### 4.Apache Flex SDK下载

Apache Flex 官网：[http://flex.apache.org/](http://flex.apache.org/)



Apache Flex SDK installer：[http://flex.apache.org/installer.html](http://flex.apache.org/installer.html)



### 5.安装Flash Builder的Eclipse插件

1、关掉eclipse EE.(从EE安装目录,可以将除plugins和features目录之外的全部删除）。

2、进入flash builder安装目录,dropins下，我这里是D:\InstallPrograms\FlashBuilder\Adobe Flash Builder 4\dropins,建立一个文本文件eclipse.link,内容只包括：path=eclipse解压目录.

注意：路径用/斜杠；如我这里就是path=D:/DEVELOPER_WORKSPACE_AJX/eclipse

3、找到Flash builder安装路径下configuration目录中将org.eclipse.update目录删掉.

![](https://img-blog.csdn.net/20140520144655640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4、cmd运行FlashBuilder.exe -clean等一会儿会弹出Flash Builder的使用许可，点击“接受”，然后你会看到如下界面。

![](https://img-blog.csdn.net/20140520144554515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




至此，Flash Builder的Eclipse插件安装成功。

**注：前期准备安装Eclipse和JDK（均为必要条件）,注意必须下载支持的Eclipse版本（Flash Builder 4.7对应的Eclipse版本是3.7和4.2）。**

### 6.Eclipse安装Flash Builder插件




Flash Builder 4.5 支持将其作为插件安装到现有 Eclipse 实例中。具体操作如下：
- 完成上述安装过程。
- 导航至 Flash Builder 的安装位置，打开实用程序文件夹。
- 运行可执行文件 Adobe Flash Builder 4.5 Plug-in Utility.exe。
- 选择语言并单击“确定”。
- 根据提示选择 Flash Builder 安装位置。
- 选择要将 Flash Builder 4.5 插入到其中的 Eclipse 文件夹，然后单击“下一步”。（注意：Eclipse 副本必须是 3.6.1 或更高版本（32 位），且必须包含名为“dropins”的文件夹）。
- 查看预安装摘要，然后单击“安装”。
- 在安装过程中，建议您编辑 Eclipse 实例的 eclipse.ini 文件，使其包含以下设置：
-vmargs -Xms256m -Xmx512m -XX:MaxPermSize=256m -XX:PermSize=64m 
**          注：5和6看起来比较乱，5这种方式对于只开发Flex项目是足够的，但是要在Eclipse中开发Flex项目就需要选择6这种方式；重点是6，这种方式安装适合Eclipse，所以大多数时候采用Flash Builder 作为Eclipse插件而使用。**

**         首先完成6操作，然后选择“Windows”>“Preferences”>"Flash Builder">选择对应的SDK。此SDK你可以选择Apache Flex SDK 也可以选择Flash Builder的SDK，最重要的是Flash Builder是选择SDK的桥梁。**





