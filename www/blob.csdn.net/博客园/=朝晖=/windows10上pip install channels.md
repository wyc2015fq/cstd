# windows10上pip install channels - =朝晖= - 博客园
# [windows10上pip install channels](https://www.cnblogs.com/dhcn/p/7131106.html)
        之前一直在MBP上做开发，在windows偶尔改一次代码，最近在windows上Pipi nstall了一次Django Channels，其中到twisted那步出现数坑
1、
- Microsoft Visual C++ 14.0 is required  
这个问题的解决方法到 http://landinghub.visualstudio.com/visual-cpp-build-tools 页面下载安装 ** Visual C++ 2015 Build Tools**
2、
- Cannot open include file: 'basetsd.h': No such file or directory  
到这个页面 下载安装 https://developer.microsoft.com/en-US/windows/downloads/windows-10-sdk
3、
- LINK : fatal error LNK1158: cannot run 'rc.exe'  
到C盘去找个文件，应该是在windows kits的某个目录里面，把系统版本对应的包含此文件的目录设置到系统path里面去

