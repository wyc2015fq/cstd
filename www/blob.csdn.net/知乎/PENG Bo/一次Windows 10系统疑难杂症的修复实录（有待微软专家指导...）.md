# 一次Windows 10系统疑难杂症的修复实录（有待微软专家指导...） - 知乎
# 



在2018年1月7日晚上，重启某台笔记本后，启动直接绿屏，提示INACCESSIBLE BOOT DEVICE（这个绿屏的缺字有点夸张。另外是正常的win10 1709 16299，并不是预览体验版）。
![](https://pic3.zhimg.com/v2-a24d20c0746098732c8f60f59412db02_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2000' height='878'></svg>)
这台笔记本的系统恰好有一段时间没备份（正计划今天备份...），期间装了很多程序和设置，重置是不可接受的**（请大家引以为戒，如果用win10做生产力开发，一定记得经常全盘映像。尤其windows更新是个定时炸弹。现在macOS也有这种越更新bug越多的趋势）**。

因此，不向恶势力低头，不重置系统，开始尝试修复：
- 无法自动修复，也进不了安全模式（同样绿屏）。p.s. 硬盘没有开加密，没有开secure boot。
- 用PE系统启动，看到SSD内容完全正常。
- 将SSD拆出放在外置硬盘盒里启动，发现启动动画刚开始转圈的时候，SSD有读写，但后面就完全无读写，符合INACCESSIBLE BOOT DEVICE的特征：**系统能装载bootmgr，但无法装载系统分区。**
- 尝试打开 bootlog，发现记录不了任何东西（因为根本就没装载系统分区），所以无法知道是哪里的问题，真是令人无语。
- 想了想，这次开机做过不少事情，但没有改过系统配置，罪魁祸首只可能是Windows更新，搜索了一下，网上有很多Windows更新造成INACCESSIBLE BOOT DEVICE的例子，于是开始处理。
- 在修复模式可以进命令行。于是用DISM get-packages列出最近的补丁，看到当天上午和中午装了2个补丁（具体是什么忘了，总之是16299.125以后的版本号，可能是和最近intel bug有关的），于是用DISM Remove-Package将它们成功卸载，**重启后正常进入系统！**
- 于是决定再重启一遍，如果正常就备份系统。
- 重启后（可以看到windows在关机画面上又做了一些系统调整），再次绿屏INACCESSIBLE BOOT DEVICE......这就麻烦了。
- 进入修复模式命令行，DISM get-packages，发现居然又多出了1个补丁（而且安装时间是在当天下午装的？），看来这个就是真正的问题所在了，说明windows更新神经错乱了。
- 这个补丁用Remove-Package卸不掉（卸载进度条直接跳到70%+然后报错）。提示错误 0x8007371b，错误：14107，“一个或多个要求的事务成员不存在”。DISM卸载的log有3.6MB，已上传到：[withablink.com/win10/dism.log](https://link.zhihu.com/?target=http%3A//withablink.com/win10/dism.log)
- 这个补丁的名字叫做Package_for_RollupFix~31bf3856ad364e35~amd64~~16299.125.1.6，属于Security update。用get-packageinfo看到描述叫做"Fix for KB4054517"，在网上找不到信息。不过可以找到很多人说KB4054517=16299.125的问题很多。不过我记得之前升级16299.125并没有遇到什么问题。
- 没办法，尝试重新add-package重装这个补丁，成功。但是还是卸不掉。没招了。

## 请各位微软专家赐教，在这种情况下怎么看到bootlog，看到是卡在哪个内核文件/驱动/注册表的问题。

## 目前没有log，没有crash dump，不知有什么办法可以解决。如果是linux，有详细启动日志，就简单多了。


