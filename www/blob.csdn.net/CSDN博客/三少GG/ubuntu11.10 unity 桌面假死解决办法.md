# ubuntu11.10 unity 桌面假死解决办法 - 三少GG - CSDN博客
2011年12月13日 13:13:22[三少GG](https://me.csdn.net/scut1135)阅读数：2199
前言：
最好安装 CCSM（CompizConfig设置管理器） 和 蓝色立方体“fusion icon”图标
+++++++++++++++++++++++++++++
1.如果CCSM（CompizConfig设置管理器）仍能工作，勾选“窗口装饰”（这时窗口的边框和标题栏会恢复正常工作），
勾选“放置窗口”，勾选“移动窗口”，勾选“调整窗口大小”；
2.如果上方的任务栏仍能正常工作，鼠标右键点击桌面任务栏右上角的蓝色立方体“fusion icon”图标，
选择“Reload Window Manager”（重载窗口管理器），桌面会刷新一下，桌面的窗口就恢复正常了。
3.如果遇到桌面崩溃无法操作的情况，可以按“Ctrl
 Alt F1“切换到字符终端下，输入用户名和密码登录，
输入命令 killall gnome-session，这时系统会自动结束图形界面，并重新回到重新登录的图形界面。
4. **如果重新登录之后桌面仍不正常，或者Compiz配置发生混乱，或者登录时选择了Ubuntu登录后却像是选择了User Defined Session的界面一样，这时可以按下列步骤恢复:（1）按“Ctrl Alt F1“切换到字符终端下，输入用户名和密码登录；（2）输入以下命令删除出错的Compiz配置文件相关目录：rm -rf .gconf/apps/compiz*rm -rf .cache/compizconfig-1rm -rf .config/compiz-1rm -rf .compiz（3）按“Alt F7“切换回图形界面下，大概需要等一分钟，图形界面就能恢复正常（不需要重新登录）；**
（4）如果等的时间实在太长，确认是“假死”状态，可以按“方法四”用命令 killall gnome-session重新登录；
（5）登录后，这时compiz是默认的状态，先右键点击上方任务栏右上角的蓝色“fusion-icon”标志，在菜单中的“Compiz Options”里点击勾选上“Indirect Rendering”，这样可以最大限度地避免出现窗口无边框无标题栏的情况
如果您觉得实在太烦人了，感觉实在没办法在Unity下设置3D桌面的话，**就放弃Unity桌面吧**。
注销用户，重新登录时选择“Cairo-Dock (with Gnome and effects)”也可启用3D桌面的，方法类似。
[http://woshao.com/article/1a5a7306235711e19f9d000c29fa3b3a/](http://woshao.com/article/1a5a7306235711e19f9d000c29fa3b3a/)
