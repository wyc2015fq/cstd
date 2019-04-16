# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之57---BREW SDK 中的IDE集成环境 - 我相信...... - CSDN博客





2010年07月14日 14:59:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1200








BREW的开发可以基于任何Windows XP以及Windows 2000以上操作系统的C/C++开发，但是就集成开发环境（IDE）而言，主要采用微软的Visual Studio（可以是6.0或者.net）.BREW为Visual Studio提供了Add-in，可以方便在Visual Studio里建立项目，开发编码，以及基于模拟器的调测。

安装时，一般要先安装Visual Studio，然后在安装BREW SDK和BREW Add-in，安装成功后，在Visual Studio的工具栏中会显示如的BREW工具栏，在tools菜单中会显示相应的菜单条目（图12-2）。





    在Visual Studio的创建新项目列表中会有一个BREW应用生成向导，通过这个Wizard 可以自动生成一个BREW的项目工程。通过BREW工具栏，可以轻松的使用SDK中的工具，以及相应的交叉编译器。在Visual Studio的环境里，就可以创建所需的makefile，编译ARM平台的目标代码，对于.NET 的用户也是如此。

从BREW Mobile Platform开始，BREW准备计划将Eclipse作为另一个主要的集成开发环境。



