# WINCE下配置GPRS的详细过程----带图显示过程 - xqhrs232的专栏 - CSDN博客
2013年01月25日 16:54:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：947
原文地址::[http://www.chainway.cn/Services/service-gprs.html](http://www.chainway.cn/Services/service-gprs.html)
相关网帖
### 1、移动GPRS详细参数设置----[http://www.zh1861.com/Info_5/54.htm](http://www.zh1861.com/Info_5/54.htm)

设备出厂时，已经进行GPRS初始设置。如果用户自己进行GPRS设置，则须进行如下设置：
执行操作：开始菜单—>设置-->网络和拨号连接（如图1-1），双击新建连接（如图1-2），输入连接名称如“GPRS”并且选择拨号连接,点击“下一步(N)”进入属性界面（如图1-3）。
![](http://www.chainway.cn/Services/images/support_pic02.jpg)
图1-1
![](http://www.chainway.cn/Services/images/support_pic03.jpg)
图1-2
![](http://www.chainway.cn/Services/images/support_pic04.jpg)
图1-3
在图1-3界面，点击“配置（C）”按钮进行端口设置，设置波特率为“115200”、数据位为“8”、奇偶校验为“无”、停止位为“1”、流控制为“无”（如图1-4）。点击“拨号选项”标签，在最下方的“可插入拨号串中的特殊调制解调命令”编辑框中输入“+CGDCONT=1,"IP","CMNET"”（如图1-5，不带外围双引号），设置完后点击界面右上角的“OK”按钮，界面将跳转到图1-3所示界面。
**注意：GPRS设置中，图1-5所进行的拨号选项设置"CMNET"是依据“中国移动”设置信息进行填写，如需使用其他网络运行商，请联系运营商获取相应GPRS设置信息及拨号信息。**
![](http://www.chainway.cn/Services/images/support_pic05.jpg)
图1-4
![](http://www.chainway.cn/Services/images/support_pic06.jpg)
图1-5
在图1-3界面，点击“下一步(N)”进入电话号码设置界面，根据当地信息填写“国家/地区代码”和“区号”信息（中国大陆地区的填写内容见图1-6中所示）。在电话号码中输入“*99***1#”(如图1-6，不带外围双引号)，设置完成后点击“完成”。
![](http://www.chainway.cn/Services/images/support_pic07.jpg)
图1-6
双击新建的“GPRS”连接，打开拨号连接界面（如图1-7），点击“拨号属性”按钮进入拨号属性界面（如图1-8），点击“编辑”按钮进入编辑拨号模式界面人，把“拨打本地电话时拨”“拨打长途电话时拨”“拨打国际电话时拨”三个编辑框中的数据全部改为“G”（如图1-9，不带外围双引号），设置完成后点击“OK”。
![](http://www.chainway.cn/Services/images/support_pic08.jpg)
图1-7
![](http://www.chainway.cn/Services/images/support_pic09.jpg)
图1-8
![](http://www.chainway.cn/Services/images/support_pic10.jpg)
图1-9
到此GPRS拨号连接设置完成。把SIM卡插入手持机的2号卡槽中（如图1-10的蓝色方框处）。
![](http://www.chainway.cn/Services/images/support_pic11.jpg)
图1-10
然后双击“网络和拨号”连接中刚才设置好的名为“GPRS”的连接，见图1-11。
![](http://www.chainway.cn/Services/images/support_pic12.jpg)
图1-11
