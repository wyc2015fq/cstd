
# OAD 空中升级 - 嵌入式Linux - CSDN博客

2015年04月13日 14:33:44[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4393


[http://www.deyisupport.com/question_answer/wireless_connectivity/bluetooth/f/103/p/69222/172351.aspx\#172351](http://www.deyisupport.com/question_answer/wireless_connectivity/bluetooth/f/103/p/69222/172351.aspx#172351)﻿﻿
第二十三节  OAD空中升级
通过仿真器更新程序或者通过USB更新固件那都是一般人都可以实现的操作，但是要想实现OAD空中升级，这还是需要一定的技术能力。这一节我就带着大家完善这一能力。
OAD：  on air download，指空中下载模式。当我们的产品发布以后，有时需要对固件进行升级，OAD是升级方式中的一种。
配置BIM（Boot Image Manger）
打开IAR，打开BLE-CC254x-1.4.0下的工程C:\TexasInstruments\BLE-CC254x-1.4.0\Projects\ble\util\BIM\cc254x\BIM.eww，然后编译，下载到开发板中。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/1614.1.jpg)
配置Image A
打开OADExample工程，配置工程，添加如下几个宏
FEATURE_OAD_BIM
HAL_IMAGE_A
FEATURE_OAD
OAD_KEEP_NV_PAGES
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/4857.2.jpg)
(1)  打开simplePeripheral.c找到宏定义DEFAULT_ENABLE_UPDATE_REQUEST，将其改为FLASE。否则会影响BLEDevice Monitor对其进行空中升级，到时可以再改回来。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/4075.3.jpg)
(2)  IAR左侧导航中找到Profile文件夹，点击右键添加oad_target.c和oad_target.h两个文件，二文件位于C:\TexasInstruments\BLE-CC254x-1.4.0\Projects\ble\Profiles\OAD中。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/0677.4.jpg)
(3)  在IAR导航中找到HAL→Target→MT254xboard→Driver，右键添加文件hal_crc.c，该文件位于C:\TexasInstruments\BLE-CC254x-1.4.0\Components\hal\target\MT254xboard中。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/0576.5.jpg)
(4)  在刚才的simplePeripheral.c文件中，找到simplePeripheral_Init()函数，在里面添加OADTarget_AddService()函数。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/5873.6.jpg)
(5)  在simplePeripheral.c的前面，添加引用OAD的头文件，OAD.h和OAD_target.h。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/0285.7.jpg)
(6)  点击Project→Option…，或直接按Alt+F7，选择BuildActions，在Post-buildcommand line中添加：
"$PROJ_DIR$\..\..\common\[cc2540](http://www.ti.com.cn/product/cn/cc2540)\cc254x_ubl_pp.bat""$PROJ_DIR$" "![](http://bbs.21ic.com/static/image/smiley/default/titter.gif)rodUBL""$PROJ_DIR$\CC2540-OAD-ImgA\Exe\OADExample"
注意，各双引号之间只有一个空格。注意图中红框标的部分，Image_A是和之前第1步对应的。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/3173.8.jpg)
(7)  点击Project→Option…，或直接按Alt+F7，选择Linker，选择Config，Linker Configurationfile中勾选Override default，添加C:\TexasInstruments\BLE-CC254x-1.4.0\Projects\ble\common\[cc2540](http://www.ti.com.cn/product/cn/cc2540)\cc254x_f256_imgA.xcl。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/4807.9.jpg)
(8)  点击Project→Option…，或直接按Alt+F7，选择Linker，选择Extra Output。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/3583.10.jpg)
(9)  Extra Option添加Hex文件的输出。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/6712.11.jpg)
(10)  点击OK，编译下载，如果出现如下错误，是因为我们使用的是IAR8051 8.30版本，如果你使用的是IAR8.20版本就没有这问题。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/2086.12.jpg)
(11)  这个问题是因为使用了虚拟寄存器导致的，我们找到如下文件。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/0435.13.jpg)
(12)  对文件的115行进行如下修改，将虚拟寄存器注释掉。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/8037.14.jpg)
(13)  编译后，可以看到我们生成的文件
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/2816.15.jpg)
(14)  我们将hex文件叠加到BIM后面
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/1781.16.jpg)
这样ImageA就成功烧录进开发板了。

配置Image B
(1)  方法如Image A，其它要注意的几个地方：点击Project→Option…，或直接按Alt+F7，选择C/C++Compiler，选择Preprocessor，将Defined symbols中的HAL_IMAGE_A改成B。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/5722.17.jpg)
(2)  点击Project→Option…，或直接按Alt+F7，选择Linker，选择Config，将Linker Configuration file中的文件改为B。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/6064.18.jpg)
(3)  保存后编译，同样的可以看到生成的文件。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/1346.19.jpg)
(4)  为了区分A和B镜像，我们将最终结果改名。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/0638.20.jpg)


空中升级
有了bin文件就可以进行空中升级了，打开BLE Device Monitor(没有安装的需要安装)，打开后软件会自动扫描设备，如图，我们扫描到了开发板。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/6523.21.jpg)
(2)  连接后，打开OAD
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/4137.22.jpg)
(3)  点击file，选择Progame（OAD），选择生成的bin文件，可以看到我们当前运行的是A固件
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/2311.23.jpg)
(4)  点击start，当进行到100%，就完成了固件空中升级。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/2311.24.jpg)
(5)  升级后再次打开OAD选项，可以看到选择运行的固件已经是B版本的了。
![](http://www.deyisupport.com/resized-image.ashx/__size/550x0/__key/communityserver-discussions-components-files/103/3660.25.jpg)
注意：
要把 BLE协议栈BLE-CC254x-1.4.0 安装在C盘，在其他盘符下没有生成bin文件。
﻿﻿

