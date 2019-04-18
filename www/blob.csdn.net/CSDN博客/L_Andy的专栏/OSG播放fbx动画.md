# OSG播放fbx动画 - L_Andy的专栏 - CSDN博客

2015年11月11日 10:14:39[卡哥](https://me.csdn.net/L_Andy)阅读数：2024



刚刚学会怎样播放fbx动画，总结几点：

1.3dmax导出的fbx坐标系可能与osg不同，需要做个旋转rotate(90,vec3(1,0,0))

2.导出的dds图片需要做个翻转

3.参考osganmitionviewer中的AnimationManagerFinder找到动画管理器

4.autodesk自带的fbx例子可以加载却播放不了，不知原因。

5.同时播放多个fbx,注意通过finder找到动画管理器，然后setUpdateCallback

6.编译fbx需要fbx sdk,设置好3rdParty目录

注意用最新的fbx源码，兼容性好
struct ImageFlip:NodeVisitor
{
apply(geode){apply(geode.state);}
for(getNumDrawables)
apply(state);
{for(getNumTexAttrList)}
tex2d=getTexAttr(stateAttr:Texture);
tex.getImage(0).flipVertical();
}
AnimationManagerFinder
group=read(.fbx);
group.accept(finder);
manager=finder._am;
group.setUpdate(manager);
manager.play();
group.accept(imageFlip);

url:[http://greatverve.cnblogs.com/archive/2013/05/08/osg-fbx.html](http://greatverve.cnblogs.com/archive/2013/05/08/osg-fbx.html)
参考：

win7 32bit, vs2010, OpenSceneGraph-3.0.1

1.先安装[fbx SDK 版本号：2012.1](http://images.autodesk.com/adsk/files/fbx20121_fbxsdk_vs2010_win.exe)

2.在配置CMake时，如下：(注意lib使用xxx-md.lib和xxx-mdd.lib)

FBX_INCLUDE_DIR            C:/Program Files/Autodesk/FBX/FbxSdk/2012.1/include

FBX_LIBRARY                    C:/Program Files/Autodesk/FBX/FbxSdk/2012.1/lib/vs2010/x86/fbxsdk-2012.1-md.lib

FBX_LIBRARY_DEBUG       C:/Program Files/Autodesk/FBX/FbxSdk/2012.1/lib/vs2010/x86/fbxsdk-2012.1-mdd.lib

3.点击“Configure”，完成后进入到下一步。

4.点击“Generate”，完成后进入到下一步。

5.运行“OpenSceneGraph.sln”，在"解决方案"中选择“生成”。

## 一、文件准备

　　1、准备OpenSceneGraph源代码及编译包

　　请参照：[点击打开链接](http://blog.csdn.net/l_andy/article/details/40511781)

　　2、准备fbx插件编译的SDK

　　笔者使用的是OSG3.0.1,[Autodesk](http://usa.autodesk.com/adsk/servlet/pc/item?siteID=123112&id=10775847)的相关网站提供了fbx的最新SDK的下载地址，最新已经更新到2013.3版本。笔者也用这个版本编译过，但是2013.3版本的SDK和OSG3.0.1不是很匹配，Autodesk对这个版本的SDK做了很大的改动，包括改动了文件名和文件夹名。于是，笔者将之前下载好的fbx20121_fbxsdk_vs2010_win.exe(VS2010使用版本)安装包，上传到了百度网盘。下载地址:[点击打开链接](http://pan.baidu.com/s/1o6znHwQ)，笔者后来升级版本，2013.3版本的SDK和OSG3.2.1能匹配。

## 二、安装fbx编译的SDK

　　安装过程没有任何难度，直接下一步就行。但是请您记住安装包的安装位置。

![](http://images.cnitblog.com/blog/417822/201302/27101434-cbf800961f304f9f9a22cc3013e03aeb.jpg)

## 三、CMake生成fbx插件编译项目

　　这里只需要再设置fbx插件的include、lib路径（前提是您之前已经编译过了OSG3.0.1）：

![](http://images.cnitblog.com/blog/417822/201302/27102457-5e4e400bf5174d2e8a2e67c98602470c.jpg)

　　具体方法在这里不再赘述。

## 四、VS2010编译插件

　　如下图所示，用VS2010打开osgdb_fbx.vcxproj（因为笔者之前编译过一遍项目，您看到的文件数和笔者可能不一样）

![](http://images.cnitblog.com/blog/417822/201302/27104245-4a43441e1b314531b8ebaa279280edce.jpg)

　　用VS2010的批生产工具编译工程：

![](http://images.cnitblog.com/blog/417822/201302/27104527-6cbb3b4ebd36477b8de0d3ed7f95f38f.jpg)

　　出现错误：.../openscenegraph-3.0.1\src\osgplugins\fbx\ReaderWriterFBX.h(19): fatal error C1189: #error :  Wrong FBX SDK version

　　解决方法：按照如下方法修改ReaderWriterFBX.h文件即可

![](http://images.cnitblog.com/blog/417822/201302/27105421-07e782ce818444088afbb87d1234764c.jpg)

　　重新编译后成功编译：

![](http://images.cnitblog.com/blog/417822/201302/27110526-ce18df405fdf4d06aad8891ea32c9c25.jpg)

　　在如图所示的文件夹中，如果可以看到fbx插件，即可表示编译成功。您现在可以用osgviewer.exe打开fbx格式的文件了。

![](http://images.cnitblog.com/blog/417822/201302/27110926-766fe019a5b248059b74349b906a016b.jpg)

**五、补充说明**

1）如果已经成功编译了相应版本的OpenSceneGraph。（注意core已经编译成功）。那么直接用CMake生成fbx插件程序，运行插件程序生成插件即可。

2）如果生成的插件只有相应的动态链接库.dll,没有相应的静态链接.lib那么按如下方式进行修改：

![](https://img-blog.csdn.net/20151111100910065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20151111100914548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20151111100918580?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20151111100922183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

重新运行程序即可。祝您成功。


