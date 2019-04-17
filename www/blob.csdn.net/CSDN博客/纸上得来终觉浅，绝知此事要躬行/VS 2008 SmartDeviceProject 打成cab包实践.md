# VS 2008 SmartDeviceProject 打成cab包实践 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年02月12日 13:48:48[boonya](https://me.csdn.net/boonya)阅读数：4407








**         按照如下所述步骤进行打包， 打包不成功时请检查：**

**             （1）程序所在路径是否含有中文；**

**            （2）新建cabProject Manufacturer属性是否是中文；**

**            （3）快捷方式是否为中文。**

### 1、创建CAB项目

1、打开Vs 2008 ,打开项目的解决方案，若不是解决方案调整为解决方案视图。

![](https://img-blog.csdn.net/20140212124951203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2、检查解决方案是否已移除您所创建的"SmartDeviceProject"，确保解决方案下存在智能设备应用程序。

![](https://img-blog.csdn.net/20140212124925062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3、在解决方案上【添加】->【新建项目】在弹出窗口中找到【安装和部署】选择创建【智能设备CAB项目】。

![](https://img-blog.csdn.net/20140212125502390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


4、修改项目名称【SmartDeviceCab1】为【ReleaseWtmsCab】，点击【确定】按钮进行项目创建。

![](https://img-blog.csdn.net/20140212125937859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5、查看项目创建路径在解决方案上是否成功创建。

![](https://img-blog.csdn.net/20140212130132781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 2、添加智能设备项目

   选中【ReleaseWtmsCab】项目，右键【视图】->【文件系统】，将文件系统视图界面打开。选择【应用程序文件夹】，右键【添加】-【项目输出】。

![](https://img-blog.csdn.net/20140212131029781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


选择需要打包的智能设备程序，如【SmartDeviceProjectWtms】，选择输出类型为【主输出】，配置输出项目为【Release Any CPU】，然后【确定】。

![](https://img-blog.csdn.net/20140212131534906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 3、添加快捷方式

选择创建的主输出文件，右键选择【创建主输出来自SmartDeviceProjectWtms（Release Any CPU）的快捷方式】，修改快捷方式名称为【SmartWtms】.

![](https://img-blog.csdn.net/20140212132115187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


鼠标选择【目标计算机上的文件系统】，右键【添加特殊文件夹】，选择创建【Programs文件夹(O)】或【Start Menu 文件夹(S)】（目的：指定快捷图标出现的文件夹路径）。这里创建【Start Menu 文件夹(S)】，将快捷图标拖到Start Menu 文件夹即可。

![](https://img-blog.csdn.net/20140212132805875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 4、添加注册表

   选中【SmartWtmsCab】项目，右键【视图】->【注册表】，在【HKEY_CURRENT_USER】-【新建键】“LICENSE”,设置键的字符串值【mycompany】；

![](https://img-blog.csdn.net/20140212133711359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


点击【mycompany】，将其属性值设置为【Scengine】(这个可以随意)。


![](https://img-blog.csdn.net/20140212134026515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注意：创建完成了之后点击VS编辑器【全部保存】按钮将所有操作保存起来。

### 5、生成CAB包

 你可以点击【SmartWtmsCab】，右键属性，修改打包成cab包的名字。

![](https://img-blog.csdn.net/20140212140106265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


选中项目【SmartWtmsCab】，右键【生成】即可，注意这里可能会生成失败，发现右下角属性【Manufacturer】的值为中文，将此值改为“Scengine”，或其他英文字符，在进行打包。

![](https://img-blog.csdn.net/20140212134639875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


至此，整个CAB程序打包完成，如果你还不行那就看下这篇文章：http://blog.csdn.net/cwinliu/article/details/5537866可能对你有些帮助。

### 6、打包不全添加配置文件和本地数据库

如图所示：

![](https://img-blog.csdn.net/20140213132307109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**        注意：如果你打包成功安装成功但是配置找不到URI可能与你打包未完全有关，你需要加入你的配置的txt或sdf文件才能运行（升级程序包不需要加入sdf文件）；主输出方式没有将这两个文件加入。**










