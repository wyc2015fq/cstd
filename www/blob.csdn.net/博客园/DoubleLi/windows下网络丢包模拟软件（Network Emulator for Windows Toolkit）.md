# windows下网络丢包模拟软件（Network Emulator for Windows Toolkit） - DoubleLi - 博客园






最近公司有一个直播的测试项目，需要模拟各种网络环境下的直播状态，最后找到一款这样的软件（如果有遇到更好的软件，望和网友多多交流）

介绍一款windows下的网络模拟器，可以模拟各种丢包或延迟的网络（Network Emulator for Windows Toolkit）

下载地址：https://blog.mrpol.nl/2010/01/14/network-emulator-toolkit/

这里简单介绍一下使用方法：

软件界面：



首先，我们在正常网络下ping一下我们的路由器：



可以看到没有任何丢包：

接下来我们使用Network Emulator for Windows Toolkit，创建一个过滤器：configuration---》new filter，点击add，如下：



接着，创建一个新的连接configuration ---》 new link ，创建好如下：



接着右键创建好的连接，设置上行和下载的丢包率，在这里我们做如下设置：



我们将上行和下载的丢包率都设置为0.03，确定后，点击工具栏上的三角符号开始，再次ping路由：



此时，我们看到已经开始出现丢包了。

至于其他的网络模拟，还需要研究。

from:https://blog.csdn.net/lyhDream/article/details/49404041









