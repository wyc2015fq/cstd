# CodeUI Test：UIMap录制文件分析一 - weixin_33985507的博客 - CSDN博客
2014年01月20日 17:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
　　UIMap文件是Coded UI Test的核心文件，它分为三个文件。
　　　　1.UIMap.uitest
　　　　　　这是xml文件，里面描述了所有需要操作的目标控件以及进行操作的动作等。
　　　　2.UIMap.Designer.cs
　　　　　　根据UIMap.uitest文件自动生成代码的区域，每次生成录像动作的时候都会覆盖上一次的生成结果。所以我们在这个文件里面做的修改都会在下次生成代码时被替换掉。
　　　　3.UIMap.cs
　　　　　　自定义代码区域，我们将UIMap.Designer.cs中的 动作记录和断言拷贝此文件中，然后进行相应修改之后，下次根据UIMap.uitest文件生成代码的时候将不会覆盖我们自定义的部分。
　　　　本文将详细讲解UIMap.uitest文件。
　　　　一、UIMap.uitest文件结构如下：
![](https://images0.cnblogs.com/blog/140041/201401/101314395988.jpg)
**Configuration**是本机系统设置等信息。
**InitializeActions**是初始化操作动作，例如自动加载需要测试的程序。
**ExecuteActions**：录制的操作动作，需要执行的自动化UI测试。
**CleanupActions**：测试完成进行的清除动作，例如测试完成，自动关闭需要测试的程序
**Maps**：需要测试的目标程序上的控件集合和层次。
**OnErrorAction**：出现异常，自动记录日志等操作
　　　　二、在ExecuteActions中可以定义各种动作。这些动作可以组成一系列的操作，它的作用对象是Maps中的控件，以下图片中包括ErrorAction,MouseAction,TestStepMarkerAction等动作。
![](https://images0.cnblogs.com/blog/140041/201401/101323290042.jpg)
　　　　　　所有的录制动作可以参考MSDN资料[http://msdn.microsoft.com/zh-cn/library/microsoft.visualstudio.testtools.uitest.common.aspx](http://msdn.microsoft.com/zh-cn/library/microsoft.visualstudio.testtools.uitest.common.aspx)，在这里可以看到这些动作的具体说明和用法。
　　　　三、在VS2012中我们来查看UIMap.uitest文件的具体呈现
![](https://images0.cnblogs.com/blog/140041/201401/101332460043.jpg)
　　　　　　图片A部分为UIMap.uitest的Action动作管理界面，在这里可以对动作的xml进行编辑。
　　　　　　图片B部分为UIMap.uitest的控件映射识别管理界面，在这里可以删除控件和查看控件信息
　　　　　　图片C部分为UIMap.uitest的控件属性查看界面。
　　　本文VS2012，如需源码请点击[CodedUITestProject2.rar](http://files.cnblogs.com/chengxingliang/CodedUITestProject2.rar)下载。
