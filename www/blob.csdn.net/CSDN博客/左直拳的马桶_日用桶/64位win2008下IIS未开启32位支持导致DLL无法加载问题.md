# 64位win2008下IIS未开启32位支持导致DLL无法加载问题 - 左直拳的马桶_日用桶 - CSDN博客
2015年03月03日 14:03:37[左直拳](https://me.csdn.net/leftfist)阅读数：4865
部署一个WEB项目，在本机、本地服务器都没有问题，但部署到远程服务器以后，提示有个DLL无法加载：
Server Error in '/' Application.
Could not load file or assembly 'Common.Component.Repository' or one of its dependencies. An attempt was made to load a program with an incorrect format.
首先肯定的是，系统声称无法加载的DLL千真万确存在
其次从本机、本地服务器都没有问题推测，似乎最有可能的原因是该远程服务器的环境有问题。但是，远程服务器是装了.NET4.5.2的，并且跑其他WEB项目都没问题。WEB项目所对应的应用程序池也是4.0，排除选了2.0的原因。
后来该WEB项目的开发者在IIS的应用程序池上开启了32位支持，问题解决。原来，远程服务器的是64位的WIN SERVER 2008。
开启32位支持的方法是：
选中WEB项目对应的应用程序池，右键，高级设置，Enable 32-bit Application = True。
网上类似文章
[http://goxia.maytide.net/read.php/1541.htm](http://goxia.maytide.net/read.php/1541.htm)
