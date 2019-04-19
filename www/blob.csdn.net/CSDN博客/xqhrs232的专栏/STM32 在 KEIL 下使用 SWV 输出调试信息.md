# STM32 在 KEIL 下使用 SWV 输出调试信息 - xqhrs232的专栏 - CSDN博客
2013年04月24日 23:13:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2038
原文地址::[http://www.360doc.com/content/10/0714/22/1090703_39061838.shtml](http://www.360doc.com/content/10/0714/22/1090703_39061838.shtml)
—— 使用STM32，个人认为 keil 还是首选
SWV 需要占用一个引脚 PB3/JTDO/TRACES WO 与 JTDO 复用，使用 SW 时，这一脚将被占用，作为 TRACES WO，一般常见仿真器直接支持
首先重写 fputc
#include <stdio.h>
int fputc(int c, FILE *f)
{
ITM_SendChar(c);
return(c);
}
就可以使用 printf 输出调试信息，但实际上我这么做失败了，初始化的时候不知道卡在了哪里，也不弄了，反正 ITM_SendChar 一样可以输出。
打开工程选项：
- project -> options for target
- "Debug"标签
- Initialization File: Keil\ARM\Boards\Keil\MCBSTM32\STLIB_Blinky\STM32DBG.ini
因为 SWV 设置只能由调试器写入
仿真器选择:
如果使用 ULINK，则选择 SWJ/Port SW
如果是 JLINK，则选择 Port SW
Trace 选择：
- Trace enable
- Core Clock 填写你捕获时CPU的频率
- ITM 使用端口 0 ，这个改不改应该影响不大
进入调试后：
View -> Serial Windows -> Debug (printf) Viewer 就看到输出信息了
最后备注一下：我山寨版的 ULINK2 失败了……但同样是山寨版的 JLINK 成功了
//======================================================
备注::
1》这样试了一下还真的是可以打印出调试消息----用printf函数-----打印到了UART#1
2》配置工程使用的是仿真环境----Options/Debug/Use Simulator/++++指定STM32DBG.ini做为初始化文件
3》其它没有什么好设置的
