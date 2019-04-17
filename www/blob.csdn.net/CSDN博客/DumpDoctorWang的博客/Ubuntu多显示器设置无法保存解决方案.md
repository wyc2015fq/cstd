# Ubuntu多显示器设置无法保存解决方案 - DumpDoctorWang的博客 - CSDN博客





2018年07月22日 16:19:23[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：664








近来新增了一块显示器，于是我也是拥有两块显示器的人了，其中一块显示器需要设置，我就使用nvidia-settings来设置，但是设置好之后重启，双显示器设置失效。经过一番折腾，终于找到了正确的设置方法。

1、依次打开“系统设置(System Settings)” ---> "显示(Display)"，进入如下界面

![](https://img-blog.csdn.net/20180723165105119?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 拖动黄色长方形(写着“AOC Intl 23“”)或者粉色长方形(写着"AOC intl 22"")，可以改变两个显示器的布局。左边或者上边那个是主显示器，右边或者下边那个是副显示器。
- 灰色区域下面的左边一栏是显示器的参数；右边是通用参数；
- 显示器参数：点击黄色长方形或者粉色长方形，可以设置对应显示器的参数。比如，我点击黄色长方形(AOC Intl 23“)，下面的参数就是”AOC Intl 23”“的参数；”AOC Intl 23”“后面有个开关选项，用来设置是否启用本显示器；Resolution-->分辨率；Rotation-->旋转；Scale for menu and title bars是设置启动器栏和字体的大小。
- 通用参数Launcher placement。Launcher placement是设置启动器(桌面左边那一栏)在哪个显示器上显示，如果选择“All displays”，则会在两个显示器上显示启动栏，点击哪个启动器栏上的应用，就会在哪个显示器上打开该应用，如果还是在主显示器上打开，就把该应用拖到副显示器上，下次打开该应用就是在副显示器上了。这时，主显示器和副显示器的设置就不那么重要了。
- 通用参数Stiky edges。Stiky edges是在鼠标跨越两个显示器时，是否会被“粘住”，读者可自行测试。Scale all window contents to matche是设置窗口的缩放基准。
- 镜像显示(Mirror displays)是设置两个显示器是否显示相同的内容。

以上的内容设置完之后，点击应用(Apply)，然后会黑屏，之后会弹出一个框让你确认是否保存当前显示设置

![](https://img-blog.csdn.net/20180723171815368?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

若显示正常，点击保留此配置(Keep This Configuration)保存当前设置就行。

**通过修改xorg.conf来修改显示器设置会出现奇怪的bug。**



