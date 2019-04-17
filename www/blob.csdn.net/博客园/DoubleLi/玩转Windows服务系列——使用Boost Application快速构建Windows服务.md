# 玩转Windows服务系列——使用Boost.Application快速构建Windows服务 - DoubleLi - 博客园






[玩转Windows服务系列——创建Windows服务](http://www.cnblogs.com/hbccdf/p/3477644.html)一文中，介绍了如何快速使用VS构建一个Windows服务。[Debug、Release版本的注册和卸载，及其原理](http://www.cnblogs.com/hbccdf/p/3486565.html)和[服务运行、停止流程浅析](http://www.cnblogs.com/hbccdf/p/3489917.html)分别介绍了Windows服务的注册、卸载、运行、停止的简单流程。那么今天就来介绍一下如何使用开源库[Boost.Application](https://github.com/retf/Boost.Application)来快速构建一个Windows服务。

# Boost.Application简介

[Boost.Application](https://github.com/retf/Boost.Application) 是一个开源的C++库，主要用于构建跨平台的服务，包括Windows、Unix、Linux、MaxOS等。没错，这个库可以用来做跨平台的服务。Boost.Application 使用开源协议 [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt)。

Boost.Application 目前还不是Boost官方的库，想要使用它，需要到[Github](https://github.com/retf/Boost.Application)上下载最新代码，当前版本为0.4.12。

# 用Boost.Application构建Windows服务

第一步，用VS创建一个控制台程序。

![控制台程序](https://images2015.cnblogs.com/blog/367940/201510/367940-20151018194353429-570601887.jpg)

第二步，配置Boost.Application以及Boost的include路径和lib路径。

第三步，将Boost.Application目录下的README.md文件中的示例代码拷贝到 myservice.cpp 中并覆盖原来所有的代码。

第四部，将编译选项设置为多字节

![多字节](https://images2015.cnblogs.com/blog/367940/201510/367940-20151018194413319-1632331806.jpg)

第五步，编译并运行程序

![运行程序](https://images2015.cnblogs.com/blog/367940/201510/367940-20151018194416163-316805333.jpg)

由于这时候还没有注册服务，所以以控制台的方式运行起来就会报错了。

第六步，将程序注册为Windows服务，在命令行中执行如下命令：

```
sc create myservice binpath= "D:\Code\C++\CommonCode\Win32\Debug\myservice.exe" type= own start= demand displayname= "service test"
```

sc命令的详细使用方法可以参考[玩转Windows服务系列——命令行管理Windows服务](http://www.cnblogs.com/hbccdf/p/managewindowsservicewithcmd.html)。

在Windows服务管理器中可以看到刚刚创建的服务：

![myservice](https://images2015.cnblogs.com/blog/367940/201510/367940-20151018194419726-1832329792.jpg)

第七步，启动服务，然后停止服务，在服务所在目录，可以看到产生了一个log.txt文件，

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

Start Log...
-----------------------------
---------- Arg List ---------
-----------------------------
myservice
-----------------------------
0, running...
1, running...
2, running...
Stoping my application...

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

这样，一个完整的Windows服务就算完成了。

# 注册服务程序

在Boost.Application 的 “Boost.Application\example\setup”目录下，有一个用于注册卸载服务的示例代码。

将此示例代码编译为service_setup_ex.exe程序后，就可以使用此程序进行服务的注册和卸载了。

安装服务，支持设置程序路径、服务名字、显示名字、描述、启动模式、依赖服务等，如下：

```
service_setup_ex.exe -i --name="My Service" --path="c:\myservice\service.exe"
service_setup_ex.exe -i --name="My Service" --path="c:\myservice\service.exe" --display="My Service"
service_setup_ex.exe -i --name="My Service" --path="c:\myservice\service.exe" --display="My Service" --description "Service Description"
service_setup_ex.exe -i --name="My Service" --path="c:\myservice\service.exe" --user=".\Renato Tegon Forti" --pass="x1x1x1"
service_setup_ex.exe -i --name="My Service" --path="c:\myservice\service.exe" --start="manaul" --depends="service1\service2\service3"
```

检测服务：

```
service_setup_ex.exe -c --name="My Service"
```

卸载服务：

```
service_setup_ex.exe -u --name="My Service" --path="c:\myservice\service.exe"
```

# 参考资料

[Boost.Application](https://github.com/retf/Boost.Application)

[Build a Server Application using Application Library](http://www.codeproject.com/Articles/756866/Build-a-Server-Application-using-Application-Libra)

# 系列链接

[玩转Windows服务系列——创建Windows服务](http://www.cnblogs.com/hbccdf/p/3477644.html)

[玩转Windows服务系列——Debug、Release版本的注册和卸载，及其原理](http://www.cnblogs.com/hbccdf/p/3486565.html)

[玩转Windows服务系列——无COM接口Windows服务启动失败原因及解决方案](http://www.cnblogs.com/hbccdf/p/3488391.html)

[玩转Windows服务系列——服务运行、停止流程浅析](http://www.cnblogs.com/hbccdf/p/3489917.html)

[玩转Windows服务系列——Windows服务小技巧](http://www.cnblogs.com/hbccdf/p/3491641.html)

[玩转Windows服务系列——命令行管理Windows服务](http://www.cnblogs.com/hbccdf/p/managewindowsservicewithcmd.html)

[玩转Windows服务系列——Windows服务启动超时时间](http://www.cnblogs.com/hbccdf/p/service_start_over_time.html)

[玩转Windows服务系列——使用Boost.Application快速构建Windows服务](http://www.cnblogs.com/hbccdf/p/build_windows_service_using_boost_application_library.html)

[玩转Windows服务系列——给Windows服务添加COM接口](http://www.cnblogs.com/hbccdf/p/add_com_interface_to_windows_service.html)



from:https://www.cnblogs.com/hbccdf/p/build_windows_service_using_boost_application_library.html









