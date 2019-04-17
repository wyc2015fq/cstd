# Unity3D开发小贴士（三）愉快的使用Lua开发 - 凯奥斯的注释 - CSDN博客





2016年11月03日 21:38:28[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4025
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# Unity3D不支持脚本动态更新，所以我们需要使用Lua来完成这个工作。本文介绍如何使用ToLua和CodeIDE来开发可用于U3D的Lua脚本。

首先我们需要下载[ToLua](https://github.com/topameng/tolua)。

新建一个项目，将tolua-master目录下的Assets复制到项目中去。

在Unity编辑器里添加一个对象，对象上添加LuaClient脚本，然后我们可以执行Lua/Main.lua中的脚本。

如何编写lua脚本可能会是一个大问题，因为我之前做过Cocos2d-x的开发，所以觉得[Code IDE](http://cocostudio.download.appget.cn//cocos-code-ide/1.2.0/cocos-code-ide-mac64-1.2.0.dmg)还不错~~，不过就是需要下载[Cocos2d-x 3.x](http://www.cocos.com/download)~~。

（Code IDE吃枣药丸，所以备份一个[百度网盘](http://pan.baidu.com/s/1mhCmI4g)，包括Mac64、Win32和Win64。望天！希望百度网盘千秋万代永久免费）

打开Code IDE的偏好设置，设置cocos2d-x的目录和模拟器的目录。

![](https://img-blog.csdn.net/20161103210009846)





然后新建一个Cocos Lua Project

![](https://img-blog.csdn.net/20161103211134732)


在Location设置到Unity项目的Assets目录下。

你需要先将Main.lua移动到其他地方，然后把Lua目录删掉，当项目创建之后，再把Main.lua移动回来。




新建的工程会有很多cocos2d-x的东西，大部分我们都不需要，只有src/cocos/cocos2d目录下的bitExtend和functions是通用并且有用的，其他的都可以删掉，包括cocos2dx 3.x的Library。最后的工程浏览器应该是这样的：

![](https://img-blog.csdn.net/20161103211739509)


打开工程属性，添加ToLua的引用：

![](https://img-blog.csdn.net/20161103211931790)


只需要在Main.lua里面添加：



```
require("functions.functions")
require("functions.bitExtend")
require("tolua")
```




便可以使用functions和bitExtend以及tolua里的方法。

示例：



```
dump(bit._d2b(3))
dump(Vector2.New(2,1))
```

如果你需要在Lua里面调用C#的类，需要在Editor/CustomSetting.cs里添加这种类型，然后在编辑器的菜单栏里点击Lua->Gen LuaWrap + Binder。


例如：


`print(UnityEngine.Application.dataPath)`









