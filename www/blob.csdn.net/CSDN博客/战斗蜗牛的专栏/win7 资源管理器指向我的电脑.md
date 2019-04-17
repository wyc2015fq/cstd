# win7 资源管理器指向我的电脑 - 战斗蜗牛的专栏 - CSDN博客





2015年05月18日 16:38:35[vbskj](https://me.csdn.net/vbskj)阅读数：1210









步骤如下：

1、在Windows资源管理器上点“右键”，打开菜单；

2、在打开的菜单中，Windows资源管理器上点“右键”->“属性”；

3、将目标：%windir%/explorer.exe

修改为： %windir%/explorer.exe /e,::{20D04FE0-3AEA-1069-A2D8-08002B30309D}

4、点击“Windows资源管理器”，查看结果。




