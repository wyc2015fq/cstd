# Ubuntu10.10  3D效果系列 - 三少GG - CSDN博客
2011年09月05日 15:33:11[三少GG](https://me.csdn.net/scut1135)阅读数：1027
关于特效快捷键介绍太少了，搜了一些资料结合自己经验把它们写了出来，希望对大家能有用。如果那不对或遗漏请告诉我，以方便我更改和补充。
首先安装 CompizConfig 设置管理器
＃sudo apt-get install compizconfig-settings-manager
CompizConfig 设置管理器可能缺少某些特效，可以打开**”应用程序——软件中心“搜索compiz**-fusion-plugins-extra补丁，然后安装
任**务栏点击 -> 系统 -> 首选项 -> CompizConfig 设置管理**
选择桌面 -> 勾选 -> 旋转立方体、桌面立方体、视口切换工具这几项
在切换到特效 -> 勾选 -> 3D窗口、窗口装饰、立方体倒影与形变、动画、在屏幕上绘制火焰、水纹效果
然后，任务栏点击 -> 系统 -> 首选项 -> 外观 -> 视觉效果 -> 自定义 -> 首选项
桌面选项卡 -> 外观选桌面立方体 -> 桌面列数 4 -> 桌面行数 1
按住Ctrl+alt，再按住鼠标左键移动，3D桌面效果即出现。
Gears前面打钩，然后找到前面打过钩的桌面立方体，点击，选择Transparent Cube，将Opacity During
Rotation降到50左右，再将3D桌面调出来，立方体成半透明的了，www.linuxidc.com并且里面有三个齿轮在滚动!
点击旋转立方体->一般，把缩放调整到1.0，把速度调整到0.7，再按Ctrl+alt+向左/向右键，可以看到立方体运动时的慢动作，并且立方体小了很多。
将鼠标在桌面上的空白位置点击一下，然后转动鼠标的滚轮，出现旋转的3D桌面效果。**按住Super键(Ctrl与Alt中间的那个键)+Tab键，桌面水平排列。**
进入调整大小->动作，点击一般前面的小三角，双击“启动所有窗口的窗口拾取器”，选中Topleft，点击OK。现在将你的鼠标移动到桌面左上角，就能实现类似Mac
os X的Expose特效了。
进入CompizConfig设置管理器的主界面，点击Expo->动作，在窗口中的Expo上面双击，勾选Top
Right，然后OK。将鼠标移动到桌面的右上角，**4个桌面整齐排列在一个3D空间内(同super+E效果)，**可以将窗口从一个桌面拖动到另一个桌面，甚至放在两个桌面中间!
在CompizConfig设置主界面选择Shift Switcher->动作->Initial(All workspace)下，勾选Bottomleft。鼠标移动到桌面的左下角，出现iPhone的Cover Flow特效。可以用鼠标滚轮和方向键切换桌面。回到Shift Switcher，点击外观，在Switcher mode中选择Flip。鼠标移动到桌面的左下角，出现windows vista中的Flip3D特效。
在CompizConfig设置管理器主界面选择“在屏幕上绘制火焰”前面的复选框，现在就可以直接按住Super+shift键，然后用鼠标左键在屏幕写了。火焰的默认颜色是红色，可以直接进入“在屏幕上绘制火焰”设置项里，调整火焰颜色，或者干脆“随机”，取消火焰的时候同时按Super+Shift+C。再次回到主界面中，勾选Windows Previews，这样将鼠标移动到任务栏会出现预览窗口。
在CCSM(CompizConfig设置管理器)主界面选择移动窗口，将其设置界面中的不透明度设置到60左右，这样你移动窗口的时候就会有半透明的效果。
同样在主界面中选择General Options->Opacity Settings，点击ADD(添加)。在OpacityWindows 中输入Dock，OpacityWIndow Values输入数值80。再以同样的方式添加DropDownMenu和PopupMenu，数值都为80。输入完毕之后，不管是什么菜单，现在都是以半透明的方式显示了，包括右键菜单。
在CCSM主界面中选择Animations->CloseAnimation，在窗口中的第一个Glide2上双击，然后从CloseEffect的下拉菜单中选择Burn，这样每次关闭窗口的时候会有火焰的效果。
点击Effect Settings，找到下面的Fire，点击小三角形出现火焰选项，然后勾选其中的“随机颜色火焰”后面的复选框，火焰颜色随机。也可以去OpenAnimation里设置开启窗口时的动画特效，或者就选“随机”。
1、**给立方体设置一个背景，进入 CompizConfig -> 桌面立方体 -> Appearance -> Skydome 勾选**
**　“Background”，在 Background Images 中选择一个背景图片即可。**
2、如果要让立方体增加一个倒影，勾选 CompizConfig -> Cube Reflection即可。
Tips: 展示立方体默认快捷键是 Ctrl+Alt+鼠标左键拖动，Ctrl+Alt+方向键旋转立方体。
3、使用神灯效果，这个效果最早出现在 Mac 上。
勾选 CompizConfig -> Animations
Tips: 建议取消 CompizConfig -> 最小化效果，勾选 CompizConfig -> 渐弱窗口
4、火焰字
勾选 CompizConfig -> Paint fire on the screen即可。
可以在 CompizConfig -> Paint fire on the screen -> Fire Particle Color 这里设置火焰的颜色，勾选 Randomly Colored Fire 会写出五彩斑斓的文字。
Tips: 默认快捷键是 shift+win+鼠标左键，按 shift+win+c 是清除效果。
5、桌面展示
勾选 CompizConfig -> Expo
Tips: 默认快捷键是 win+e，鼠标移动到屏幕左上角也可以。
6、雪花效果
勾选 CompizConfig -> Snow 再到这里下载雪花材质，在 Snow -> Textures 这里添加进去即可。
7、菜单透明问题
默认菜单是透明的，可以到 CompizConfig -> General Options -> Opacity Settings 这里进行设置。删除 Window opacities 里面的那行代码就可以了。
8、3D 层叠窗口效果
勾选 CompizConfig -> 3D Windows
9、给立方体中增加一些鱼儿
勾选 CompizConfig -> Cube Atlantis
按住 Ctrl+Alt+鼠标左键拖动桌面，就能看见了。
10、屏幕保护
勾选 CompizConfig -> Screen Saver
有两种效果，一种是 Rotating cube 旋转立方体，一种是 Flying windows 飞行的窗口。
11、3D 窗口切换效果
勾选 CompizConfig -> Shift Switcher 在 Switcher mode 中可以选择两种切换效果，一种类似 Vista 的 Flip3D 切换，一种类似 Mac 的 CoverFlow 切换。
在 **Ubuntu** 中有不少便捷的快捷键，相信大伙也都非常喜欢使用这些快捷键吧。
**1.前一个后一个工作区的切换 **
如果你经常使用工作区，那你就可以用Ctrl + Alt +上/下方向键很容易地进行切换。左箭头切换到上一个工作区，右箭头切换到下一个工作区。如果你 安装 了Compiz，可以用Super + E快捷键显示所有的工作区。
**2.把当前窗口移到另一个工作区 **
快捷键Shift+ Ctrl + Alt +左/右方向键让你很容易把当前窗口移到指定的工作区。这个快捷键和上面的快捷键很好配合。如果你工作时常常打开很多窗口，但又不想看到 桌面 和任务栏挤满程序，你可以用这个快捷键把程序移到另 一个工作区，这样你的桌面就整洁多了。
**3.显示桌面 **
Ctrl + Alt + D快捷键让你很快地最小化所有窗口，看到桌面。当所有窗口都最小化后，你再按这个快捷键就可以恢复窗口原来的状态。
**4.鼠标右击的键盘快捷键 **
在大多数程序里，你可以右击显示快捷菜单。其实键盘上 Shift + F10也能达到一样的效果。
**5.重启会话以从崩溃中恢复 **
ubuntu很少会完全崩溃。但如果它真的崩溃了，你可以按下Ctrl + Alt + Backspace来重启会话，恢复的可能达90%。
**6.快速锁定屏幕 **
如果你需要离开 电脑 一会儿，可以按下Ctrl + Alt + L很快地锁定屏幕，以防有人使用
**7.反向切换窗口 **
Alt + Tab是切换窗口的快捷键。如果你再按下SHIft，你就可以反向切换窗口。这个快捷键很有用，当你Alt + Tab按得太快，错过了你想要切换的窗口，按一下shift就可以返回之前的窗口了。
**8.用方向键移动窗口 **
Alt+F7会激活移动窗口 功能 ，用方向键（上，下，左，右）就可以移到窗口了。
**9.显示隐藏的 文件 **
大多数情况下，你不需要在你的“家“目录中看到那些隐藏文件，但如果你有这个需要，你可以在Nautilus（ubuntu的文件管理器）下按Ctrl + H来显示隐藏文件。
**10.不用右击鼠标就能显示文件属性 **
想要查看文件/文件夹的一般做法是右击选择属性。现在你可以按下 Alt + Enter就能显示属性窗口了。
当然，这些只是冰山一角。如果你想要知道更多快捷键，不只是ubuntu还包括Gmail, Google Search, Firefox以及其他 软件 ，还需要大家在平时使用的时候多积累多发现。
超级键+Tab
* 打开主菜单 = Alt + F1
* 运行 = Alt + F2
* 显示桌面 = Ctrl + Alt + d
* 最小化当前窗口 = Alt + F9
* 最大化当前窗口 = Alt + F10
* 关闭当前窗口 = Alt + F4
* 截取全屏 = Print Screen
* 截取窗口 = Alt + Print Screen
*默认特殊快捷键
* 展示所有窗口程序 = F10
* 展示当前窗口最上层程序 = F11
* 展示当前窗口所有程序 = F12
* 切换窗口 = Alt + Tab
* 旋转3D桌面 = Ctrl + Alt + 左/右箭头(也可以把鼠标放在标题栏或桌面使用滚轮切换)
* 旋转3D桌面(活动窗口跟随) = Ctrl + Shift + Alt + 左/右箭头
* 手动旋转3D桌面 = Ctrl + Alt + 左键单CcCcczxcvbfdfhu击并拖拽桌面空白处
* 窗口透明/不透明 = possible with the “transset” utility or Alt + 滚轮
* 放大一次 = 超级键 + 右击
* 手动放大 = 超级键 + 滚轮向上
* 手动缩小 = 超级键 + 滚轮向下
* 移动窗口 = Alt + 左键单击
（有些快捷键可以自行设置）
