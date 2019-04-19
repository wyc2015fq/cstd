# USB OTG插入检测识别 - xqhrs232的专栏 - CSDN博客
2016年10月11日 09:40:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：634
个人分类：[各种电子元器件/硬件技术](https://blog.csdn.net/xqhrs232/article/category/6300755)
原文地址::[http://blog.csdn.net/uranus_wm/article/details/9838847](http://blog.csdn.net/uranus_wm/article/details/9838847)
相关文章
1、[USB OTG ID 检测原理](http://blog.csdn.net/liuxd3000/article/details/18216003)----[http://blog.csdn.net/liuxd3000/article/details/18216003](http://blog.csdn.net/liuxd3000/article/details/18216003)
2、USB OTG原理+ ID 检测原理----[http://bbs.elecfans.com/jishu_469277_1_1.html](http://bbs.elecfans.com/jishu_469277_1_1.html)
3、[OTG中的ID脚风波释疑](http://blog.csdn.net/evilcode/article/details/6551959)----[http://blog.csdn.net/evilcode/article/details/6551959](http://blog.csdn.net/evilcode/article/details/6551959)
版权声明：本文为博主原创文章，未经博主允许不得转载。
转载请标注原文地址：[http://blog.csdn.net/uranus_wm/article/details/9838847](http://blog.csdn.net/uranus_wm/article/details/9838847)
一 USB引脚一般四根线，定义如下：
![](https://img-blog.csdn.net/20130813131528984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXJhbnVzX3dt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
为支持OTG功能，mini/micro usb接口扩展了一个ID引脚（第4脚）
A设备端ID脚接地，则初始状态为Host，例如PC和支持OTG设备做主设备时
B设备端ID脚悬空，默认上拉为高电平，则初始状态为Device，例如U盘和支持OTG设备做从设备时
1. 移动设备如需支持OTG，内部ID引脚需要默认上拉为高
2. VBUS是输入输出双向引脚。对于Device是power supply输入脚；对于Host需要串一个5V电源输出给device。
二 USB插入检测过程，看下面一个简单电路，Samsung Exynos4412平台：
![](https://img-blog.csdn.net/20130808182016062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdXJhbnVzX3dt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
电路描述：CON1为micro usb连接器，VBUS连接系统XuotgVBUS脚，同时提供一个中断输出脚到XEINT28
同时外部一个DC5V经过控制芯片也输出到XuotgVBUS，作为host时需要用到给外部供电
ID脚上拉1.8V作为Host/Slave检测，同时提供另一个中断输入脚到XEINT29
该设备支持OTG，下面说下设备的发现过程：
作为从设备插入PC端口时：
1.  系统检测到VBUS上的XEINT28上升沿触发中断，因为PC端会有一个5V从VBUS给过来，进入中断处理函数进一步确认ID脚状态，ID脚为低则状态错误，ID脚为高表示设备应该切换到从设备模式
2. 通知usb gadget使能vbus，按照device模式使能PHY。gadget在probe时注册了一个SPI软中断IRQ_USB_HSOTG，用于响应数据接收
3. 开启usb clk，使能PHY，此时外部5V电源供给系统XuotgVBUS，gadget收到IRQ_USB_HSOTG中断要求重启OTG core
4. USB DP（高速设备为DP，低速设备为DM）上产生一个高电平脉冲，此时PC识别到一个USB设备插入，windows会提示用户
5. 后续就是SETUP，GET DISCRIPTOR的过程
作为主设备发现设备插入时：
1. 系统检测到ID脚上XEINT29下降沿触发中断（实际是插入的usb公口第四脚直接连接到第五脚地上面），进入中断处理，切换到主设备模式
2. 关中断，使能DC5V给VBUS上电，唤醒ehci与ohci
3. usb core在内核初始化时注册了一个名为khubd的内核线程，由khubd监控port event。（实际过程我理解是从设别由VUBS供电后，会在DP或DM上产生一个高电平脉冲
ehci在接收到脉冲信号后识别到设备插入，仅仅是理解，这一点未验证）
3. khubd获取port，speed后交给ehci，接下来就是usb的SETUP，GET DISCRIPTOR过程
