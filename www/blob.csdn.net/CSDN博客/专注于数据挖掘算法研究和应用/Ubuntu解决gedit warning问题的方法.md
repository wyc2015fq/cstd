# Ubuntu解决gedit warning问题的方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年12月09日 08:52:21[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3394








1.问题：Ubuntu11.04用sudo gedit打开文件编辑保存后会出现如下提示：

  (gedit:26742): Gtk-WARNING **: Attempting to store changes into `/root/.local/share/recently-used.xbel', but failed: 创建文件“/root/.local/share/recently-used.xbel.D2BO6V”失败：没有那个文件或目录

(gedit:26742): Gtk-WARNING **: Attempting to set the permissions of `/root/.local/share/recently-used.xbel', but failed: 没有那个文件或目录




2.在终端执行命令：sudo mkdir -p /root/.local/share 即可解决。具体原因未知，似乎是和root权限有关。





