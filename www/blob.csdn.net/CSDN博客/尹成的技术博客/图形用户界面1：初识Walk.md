
# 图形用户界面1：初识Walk - 尹成的技术博客 - CSDN博客

2018年11月16日 09:33:10[尹成](https://me.csdn.net/yincheng01)阅读数：100个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**GO语言的图形界面库Walk简介**
GO没有原生的界面库，所以不能直接用来写GUI界面。但最近互联网上已经涌现出不少成熟、好用的第三方界面库。使用它们，就同样可以写出同C\#、C++的界面。而且效率还更胜一筹。
关于Walk界面库（官方介绍）：Walk是一个写给Golang的Window应用程序库套件，它主要用于桌面GUI的开发，但也有更多的东西。
\#\#\#安装Walk
在浏览这部分前，请确定已经配置安装完毕go语言的环境。如果没有，可以参考go官网的Install帮助。另外，注意：这个Walk库只能运行在Go 1.1.x及以上。
要安装Walk很简单，只要运行命令：`go get github.com/lxn/walk`
等待命令执行完毕，这时候检查GOPATH的src与pkg下，是否有符合自己计算机环境的walk目录。（我的电脑的CPU架构为AMD64，则会在`%GOPATH%\windows_amd64\github.com\lxn`下出现一个walk文件夹与编译完成的a文件，另外在`%GOPATH%\src\github.com\lxn`下也会出现walk的源代码）
**导入依赖**
`import (
	//引入walk包
	"github.com/lxn/walk"
	//declarative包下包含大量控件，加一个点：使用包内的成员时可以省略包名
	."github.com/lxn/walk/declarative"
	"strings"
)`**官方的入门Demo**
`func main() {
	//声明两个文本域控件
	var inTE, outTE *walk.TextEdit
	//配置主窗口，并运行起来
	MainWindow{
		//窗口标题
		Title:   "尖叫的Demo",
		//可拉伸的最小尺寸
		MinSize: Size{600, 400},
		//主布局：垂直布局
		Layout:  VBox{},
		//窗口中的所有控件
		Children: []Widget{
			//水平分割器（水平小布局）
			HSplitter{
				//局部水平排列的控件们
				Children: []Widget{
					//文本输入框
					TextEdit{
						//绑定到inTE变量
						AssignTo: &inTE},
					//文本输出框
					TextEdit{
						AssignTo: &outTE,
						//只读的文本框
						ReadOnly: true},
				},
			},
			//普通按钮
			PushButton{
				//按钮文本
				Text: "动动我试试",
				//响应函数
				OnClicked: func() {
					inputStr := inTE.Text()
					outputStr := strings.ToUpper(inputStr)
					outTE.SetText(outputStr)
				},
			},
		},
	}.Run()
}`**配置文件**
命名为xxx.exe.manifest，放置在程序包的根目录下
xxx.exe即为程序编译形成的可执行程序名称
该配置文件作用是运行时向Windows操作系统寻求一些支持和依赖，内容可以无需修改
`<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" xmlns:asmv3="urn:schemas-microsoft-com:asm.v3">
	<assemblyIdentity version="1.0.0.0" processorArchitecture="*" name="SomeFunkyNameHere" type="win32"/>
	<dependency>
		<dependentAssembly>
			<assemblyIdentity type="win32" name="Microsoft.Windows.Common-Controls" version="6.0.0.0" processorArchitecture="*" publicKeyToken="6595b64144ccf1df" language="*"/>
		</dependentAssembly>
	</dependency>
	<asmv3:application>
		<asmv3:windowsSettings xmlns="http://schemas.microsoft.com/SMI/2005/WindowsSettings">
			<dpiAware>true</dpiAware>
		</asmv3:windowsSettings>
	</asmv3:application>
</assembly>`**运行Hello程序**
go get xxx的顺利执行需要预先安装好Git版本控制工具，下载链接：[https://git-scm.com/downloads](https://git-scm.com/downloads)
安装walk包
`go get github.com/lxn/walk`拷贝类库首页（[https://github.com/lxn/walk）的Hello程序代码到自己的go文件](https://github.com/lxn/walk%EF%BC%89%E7%9A%84Hello%E7%A8%8B%E5%BA%8F%E4%BB%A3%E7%A0%81%E5%88%B0%E8%87%AA%E5%B7%B1%E7%9A%84go%E6%96%87%E4%BB%B6)
从类库代码包的examples下的任意应用程序文件夹下拷贝一个系统依赖配置文件（xxx.exe.manifest）到自己的go文件所在的目录下，重命名为hello.exe.manifest，后续可执行程序的名字就必须叫hello.exe
编译形成hello.exe，-ldflags="-H windowsgui"代表屏蔽控制终端（控制终端可以在测试阶段查看调试信息）
`go build -o hello.exe -ldflags="-H windowsgui"`执行hello.exe
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

