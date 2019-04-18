# 网页Web上调用本地应用程序（.exe） - 深之JohnChen的专栏 - CSDN博客

2017年02月15日 23:28:53[byxdaz](https://me.csdn.net/byxdaz)阅读数：21585


﻿﻿

一、web调用本地应用程序的思路

这个方法主要思路是利用自定义URL Protocol来调用应用程序。浏览器在解析到自定义URL Protocol之后，会寻找注册表，然后通过注册表启动相应的程序，然后启动改程序，传入参数。这样就可以在WEB页面调到你的程序了。比如在浏览器地址栏输入“tencent://message/?uin=88888888&Site=JooIT.com&Menu=yes”就会出现一个QQ对话框。

1、先往注册表写信息。如下：

```
Windows Registry Editor Version 5.00
[HKEY_CLASSES_ROOT\Diff]
"URL Protocol"="这里是小程序的安装路径"
@="DiffProtocol"
[HKEY_CLASSES_ROOT\Diff\DefaultIcon]
@="这里是小程序的安装路径,1"
[HKEY_CLASSES_ROOT\Diff\shell]
[HKEY_CLASSES_ROOT\Diff\shell\open]
[HKEY_CLASSES_ROOT\Diff\shell\open\command]
@="\"这里是小程序的安装路径\" \"%1\""
```

将上面的内容保存为.reg文件运行即可。

2、web页面调用。在网页直接一个超链接，协议://参数就可以了。比如

```
<a href=" Diff://">
   alt="点击这里">  
 </a>
```

`<a href=" Diff://hello">SIMU</a>`

需要指出的是：在上面例子中注册表中代码【@="\"这里是小程序的安装路径\"
 \"%1\""】中的%1表示传入参数，就是对应这此处的Diff://hello，解析之后就可以得到参数hello了。

二、完整实例。

1、xxxClient.reg文件

```
Windows RegistryEditor Version 5.00
 
[HKEY_CLASSES_ROOT\xxxClient]
@="xxxClientProtocol"
"URLProtocol"=""
 
[HKEY_CLASSES_ROOT\xxxClient\DefaultIcon]
@="C:\\ xxx_Client\\client.bat,1"
 
[HKEY_CLASSES_ROOT\xxxClient\shell]
 
[HKEY_CLASSES_ROOT\xxxClient\shell\open]
 
[HKEY_CLASSES_ROOT\xxxClient\shell\open\command]
@="\"C:\\xxx_Client\\client.bat\" \"%1\""
```

2、client.bat文件

```
@echo off
 
rem设置当前目录为C:\ xxx _Client\
setcurrent_dir=C:\ xxx _Client\ 
 
pushd%current_dir% 
 
call C:\ xxx _Client\xxx _Client.exe
```

3、web页面文件

```
<!DOCTYPEHTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<htmlxmlns="http://www.w3.org/1999/xhtml">
 <head>
     <meta http-equiv="Content-Type"content="text/html; charset=utf-8" />
 </head>
   <body>
       <div>
           <a href="xxxClient://">
          打开客户端软件
           </a>
       </div>
   </body>
</html>
```

三、注意问题：

1、如何动态生成注册表信息，可以在应用程序中写代码操作注册表，写入上面的URL Protocol注册表格式信息。

2、路径问题，如果应用程序包含了很多文件，就涉及到工作路径问题，web调用应用程序，当时的工作路径是的web运行的工作路径。可以通过bat或应用程序来切换路径。

以bat为例，建立一个批处理文件，里面切换路径，注册注册表信息时，[HKEY_CLASSES_ROOT\Diff\shell\open\command]使用bat路径。

xxxClient.bat文件

@echo off

rem设置当前目录为C:\
 xxx_Client\

setcurrent_dir=C:\ xxx_Client\

pushd%current_dir%

call C:\ xxx_Client\xxx_Client.exe

3、如何获取参数？

MFC Dialog程序的参数值：

首先需要获取传入的参数，在控制台程序中我们都知道main()函数的参数argv里带入了传入的参数，而在MFC程序中则需要在InitInstance()中进行命令行参数解析。

    CCommandLineInfo   cmdInfo;   

    ParseCommandLine(cmdInfo);

为了给对话框传入待显示的参数，加入了一个SetDisplayInfo方法。

    CHelloWorldDlg dlg;

    dlg.SetDisplayInfo(cmdInfo.m_strFileName);//设置待显示的信息

void CHelloWorldDlg::SetDisplayInfo(CString& strInfo)

{
this->m_strInfo = strInfo;

}

最后在OnInitDialog函数中进行参数解析

//解析传入的完整地址,e.g "helloworld://hello world/"
int pos = m_strInfo.Find("//");//找到分隔符
    m_strInfo = m_strInfo.Mid(pos+2);//取到传入的参数
    m_strInfo.Delete(m_strInfo.GetLength()-1);//去掉最后的'/'
    m_edit_info.SetWindowText(m_strInfo);

[代码实例](http://download.csdn.net/detail/byxdaz/9755434)

参考资料：

1，[Registering
 an Application to a URL Protocol](http://msdn.microsoft.com/zh-cn/library/aa767914(en-us,VS.85).aspx)

