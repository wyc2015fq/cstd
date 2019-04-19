# STM32F042F6P6生成HID工程模拟USB鼠标 - xqhrs232的专栏 - CSDN博客
2017年08月31日 10:24:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1453
原文地址::[http://www.stm32cube.com/article/141](http://www.stm32cube.com/article/141)
相关文章
1、STM32F042F6P6-USB设备开发最小系统之键盘示例----[http://www.stm32cube.com/article/143](http://www.stm32cube.com/article/143)
就是这个板子，淘宝59可买
![image.png](http://www.stm32cube.com/ueditor/php/upload/image/20170524/1495626766299900.png)
直接上工程配置了。
![](https://iszqba.dm2301.livefilestore.com/y4mnnYsWQsj-246o-KWvCUJttFVBzYT85IkeNexymLmRXhOVVPFU_I4c7ONti2iibMRI04P821uljec6PEe-WNXVuNFH8YH4vag_vmgM8NDNtiCrfm0VgQvbZ0hOsmJvzzaOrDStFxVP3OHVEuPW3gcUhMdrWHPE4BhuhjINzRADm_2ez6mBH-gKoQkooiCKU9SJZ8qJgDfrFVdPwhF7CsbtA?width=700&height=491&cropmode=none)
打开SWD调试，开引脚映射，打开USB（由于引脚占用，USB必须引脚映射后才能开启），设置好对应按键和LED的引脚输出输入模式：
![](https://icz8ba.dm2301.livefilestore.com/y4mDr1VJUPihkAETQh6-ic12FiyxoYY1RRCJ7mRds0MEMKMbppwBRUJTyXJKWtjYgSBbFmvp6RDCPhMwgHw9Yf8FtHOl6EoEmEwb663uicIqaSOallffRhXkpVj1rWJvQ4bbEotJlg4NYzTuWg1Sq-jOF9_ayXObZqNbbxrXPCT3dCTxnInQR5J0BhNeymMbx1pcF3rgHOrayW3xkLXx0Ilmw?width=1025&height=718&cropmode=none)
设置系统时钟，这里进去后自动设置：
![](https://iczpba.dm2301.livefilestore.com/y4m5fF77_XBeHodGoxctjuG9fFVq3vBWVWz1uraYO9EKgKw9wZG2IMdFczKA2W-94GPbs_VpmfO3HrZQA5lxDY2tlPXtsgR4_T3OFbhs5O3nhBDZGZPpPP8OAN7vXQjxl7q_xgKMVj7W8sQSR6twZytk1-Ax625IN4PzvZBBW9mz29j2tTu_DI7Z7hKeJXczrE2LaHPIqxFBb0F16m3vudeBg?width=1025&height=718&cropmode=none)
设置USB模式，进入GPIO配置：
![](https://iczqba.dm2301.livefilestore.com/y4mYIthjCvYgnVm_Pqy0bokBjWWkRsFfkeYy9b2E5v6VZnO6gqjiBwcZwuoB9v3Xux9ysEJZ9cDLpKzAqVZK9DuB_0DoFHifmIbsSkHvs2syLQ2-T4tYf5Aua3VhzxCN9FMQDXoLm8rpJUIgNTtBFSFVpZQSlU1BH_GroX_T0NqNrvFNovjmAcAjwgO-iwri3S22BjMCBiEHGlzthiYO9WntA?width=1025&height=718&cropmode=none)
设置好按键和LED的输入输出模式，按键为上拉输入，当按键按下时检测为低：
![](https://icz9ba.dm2301.livefilestore.com/y4mzPs_xrz9FdxfEVK6n45aVabQqwHsLxaiKmok6sdjnbOdmrdxYP6hIIV0dm97_9a27PyNZg44OLkFpPaukB515hb8FoUPyZSbuNVOAI3ZjiE24LnRdVtpYo9fzEQMTor4_jqZbrR70MUhw1t4LKXrSXhthjBfsbCXmIrPIuAa-BYVDjb3ctEaJH4m6TtAMcorKhK6U1UvM46uqrtzbP1W1g?width=716&height=639&cropmode=none)
配置工程输出：设置工程名、工程路径、IDE：
![](https://icabba.dm2301.livefilestore.com/y4mORP8TYX8QYaFD45CcN7fiYRK6pXrDY0AY4B80vGfO0DfwqY-xqCLjtOEK3V6d_A4OAx9_fefBjQmMgbRfhAUeZfPDzUhCx2RM-dLTjfkz4IwKSyyYyc_1GV6qRGA574CN_C6ufSDGYr5oAF1iqIZtDSeai1zeH1pVU7HrbyHnTtxjYQx90BwFDJgg9KxoQhizhAArF4dqIrmj16yT3nCng?width=1025&height=718&cropmode=none)
点击生成工程，等待一会：
![](https://icacba.dm2301.livefilestore.com/y4myk-kCgpmJdy59ynP1NChxLgU8GyAvXkOyD8ZrSXooBfRdggaGEqCdHuJm8W6UJM6xxoyPBMuDvJy3S69bErAum-Xhreo23Rt1duoMH3OsPLfX79N01N6ztN60vKtDmxTU3t1nAPISuRuVNNC0D0ORbVB2C2U5sfnV95aZYlSb-3c8ygnXzEGnmbMhPQQFA-BbTST8gijxSTHginiyLMRxg?width=1025&height=718&cropmode=none)
工程生成完后可直接打开，Keil对于STM32F0/L0系列的单片机是免费授权的，大家可以不用去下盗版破解，直接到官网下载最新的Keil就可以了，新的Keil编译似乎快了不少，详情：[http://www2.keil.com/stmicroelectronics-stm32/mdk](http://www2.keil.com/stmicroelectronics-stm32/mdk)
![](https://icaaba.dm2301.livefilestore.com/y4mX0yfKnuv8qxasrwAuhFNIGXL619vxMmX3dHxWGEcA3kpsk728CnZPiaw-NhYTcYP_uzG6EHyvamJYsvVBf3hCrq2WupKg7TnVAtoq45XJyWZYUs4ZT3HOf0JYBCNAplb2yIbNhrh8SwudImNQdJWpulwr4ePCqufnlv1Jk1zt8dSXznmnYDkLpWAJgR6n1CfIj4l7GU9gTwr5ii4jSPJLw?width=1025&height=718&cropmode=none)
打开工程，编译，下载（我这里用的是ST-LINK下载），下载完成后，插上USB线到电脑上，会自动识别新设备，如下：
![](https://icadba.dm2301.livefilestore.com/y4mYDY9mXKKuHaikZREevIq_to7nuuzuhCHUhww2Pj6BIXAMzJ3tOVCVu9JXIGqecHzbHBDiiQr2te7Wz5NZlAjMNEtXMUKh-yztH3oeNsL0TrjddrjnprJDyxj2DHd3aKAROmKjwCYxAggbjS1EpHbgO-K8wgyYbT5pgWZfTrwXMfA3XY3SBErxPd8adJmcS6us1iXHknEvE39pa4GFet6mg?width=825&height=571&cropmode=none)
可以改以下文件修改设备名称等：
![](https://iszpba.dm2301.livefilestore.com/y4mpNwGQs8Cpd9RVc2f4u-fREwaSDHVCD--gIncUVWCRkKuGzdamhtJgc-QY5MzKzPtiBR27c1NQf8VIbWP0xvcANaT67zy-RW67Xam7YG-cEOMRKn5bDiRLKCvnjF3atREk8DJZjKpR41R8xC9MWrof3yHlrsjFdm3b_-6AXhWhsUDq6PB1QTv7GRXyrLz7oUtX_4bHeJE0e91GVKgg7WHJw?width=884&height=593&cropmode=none)
修改后编译、下载，电脑里检测的设备名称就已经变了：
![](https://icagba.dm2301.livefilestore.com/y4m3JJyZV9GA5y5KwEY0U8zN_Mh1x5pFEEl_BvPfOHTa99fxDleeXgF1Bk_4ZozmaoHLMsviHgcIbUlHuPJ_-4N21SLY8R8-OOuMC-2NCX4J6f3gSYsKcvnc8V-V5gLrWmo-06cSGSXyGbPlazr2vFiZLcMecb8YM54grAE0cdgS_Qntvz2xHtWhDd432Lv-Nw4rxD3nJk6ucQZKh7ALP1_lA?width=1014&height=668&cropmode=none)
下面我们来修改main.c，添加按键检测和LED灯代码，当按下按键时，灯就会亮，松开后就灭，在while(1)下添加如下代码：
 if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0)
 {
   HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
 }
 else
   HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
![](https://icahba.dm2301.livefilestore.com/y4mH1FbjeCKsq_lAC90xo150bri-dKgSpwQ9FjKjKBaMcbz9L5oWQAaQCDC_BeF1qoSexGpjoexnncHdqKQEEeCmBpSKbZ3ZF1Bve6r8XBrhSJU7xyUdBeHKNHYGgktIFE4UbEa3fDfOy7jnh28YJXXuvPdS0V4Vird8Itku-SFajI6NGB6EjRKdP0dvzEzp275Bw8Pw6S7frMqMN1gKwe8aw?width=970&height=494&cropmode=none)
然后编译，下载，重新运行之后是不是当按下按键的时候LED就亮了呢？
继续添加USB鼠标的代码，在main.c的程序头加入如下头文件：
#include "usbd_hid.h"
![](https://jmahba.dm2301.livefilestore.com/y4m_Gy-JEHfTkE4pEemYoMlaEfbuRUIrsbJsySjRjyjHvkYi5WTCxgylC-zGl2CMVcjjZQb9703HgXtU20JKyrC_1_yLY4Jj3pyJMNbF_Gwz_EhI3Sh67IUs18WpakUk5luA8wU9P-xK2Fa-PBh-gVx8ksRRkRosdG_mj1hiEOWqIVaIDdI0yBG5yRmzaClCeaH_vR2ECKXkMdObbVUsV4aQQ?width=737&height=447&cropmode=none)
在main函数中定义一个4位数组，用来存放要发送到电脑的鼠标动作信息：
  /*
 * buffer[0] - bit0: Left button 左键
 *             bit1: Right button 右键
 *             bit2: Middle button 中键
 * buffer[1] - Cursor movement X axis 水平移动
 * buffer[2] - Cursor movement Y axis 垂直移动
 * buffer[3] - Wheel vertical movement 滚轮转动
 */char buffer[4] = {0x00, 10, 10, 0};
在按键检测的判断中添加一条发送语句：
USBD_HID_SendReport(&hUsbDeviceFS, buffer, 4);
![](https://jmagba.dm2301.livefilestore.com/y4m0ynwWUsD_FIfqowJmppRmK6TGv9kHsfCM9N5V6ib10U21oxBj0pgSFlIz96ZQPdkmnL1CE7OgiyetMUYJh0seKkA_BCwewkjSScx34ufkWORaZ2ZMzvbu8Y-4b2r_9WpX5HrPA7ueDZ_ZeBccmQ8eBq9GFO7-GdI0p2OJItQibj5E39rfi8ai8U47rbN7d0kqJkfDn-jgu6Kk1rg8aebyw?width=845&height=559&cropmode=none)
再次编译下载并重新运行，USB插到电脑上后，按下按键，LED亮并且电脑上鼠标会向右下方移动。鼠标的其它参数输出请自行补充。
更多可关注smilefrog.net
