# DSP连接不上CCS3.3的问题讨论 - xiahouzuoxin - CSDN博客





2014年07月06日 16:58:42[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：6737








## 环境
- 操作系统：Win7, 64bit
- IDE：CCS V3.3
- 仿真器：SEED XDS510PLUS
- DSP型号：TMS320C6713GDP(DSP6713)

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/DSP%E8%BF%9E%E6%8E%A5%E4%B8%8D%E4%B8%8ACCS3.3%E7%9A%84%E9%97%AE%E9%A2%98%E8%AE%A8%E8%AE%BA.md#%E6%A3%80%E6%9F%A5%E6%AD%A5%E9%AA%A4)检查步骤
- 
试着按下复位按键后再点击Connect连接

- 
请检查电源供电是否正常（核心电压是否为1.2V，IO口电压是否稳定为3.3V）

使用示波器AC测试检查供电的纹波，TMS320C6713数据手册的99页有纹波的范围要求

![](https://github.com/xiahouzuoxin/notes/raw/master/images/DSP%E8%BF%9E%E6%8E%A5%E4%B8%8D%E4%B8%8ACCS3.3%E7%9A%84%E9%97%AE%E9%A2%98%E8%AE%A8%E8%AE%BA/wenbo.png)

- 
如果是用电池供电，检查电池电压，电池电压过低会造成连接失败，推荐使用电压监控芯片TPS3823-33

- 
检查上电复位是否正常（上电时有复位脉冲，上电后复位引脚为高电平），DSP6713的复位引脚有上拉电阻

- 
检查时钟输出是否正常，包括晶振输出和ECLKOUT（TMS320C6713中默认=晶振时钟/2），如果ECLKOUT管脚输出不正常，则说明芯片坏了

- 
CCS3.3工程中载入的GEL文件是否正确（如“存储器的初始化”）

- 
请确保仿真器是好的（换块好的DSP板试试），不过一般不会倒霉到是这种问题吧（一个仿真器好贵呢）

- 
检查JTAG接口的EMU0与EMU1是否有上拉电阻，NC管脚应该保持悬空

![JTAG](https://github.com/xiahouzuoxin/notes/raw/master/images/DSP%E8%BF%9E%E6%8E%A5%E4%B8%8D%E4%B8%8ACCS3.3%E7%9A%84%E9%97%AE%E9%A2%98%E8%AE%A8%E8%AE%BA/JTAG.png)

- 
检查PLL滤波管脚是否正确，滤波器是否焊接好

![PLL](https://github.com/xiahouzuoxin/notes/raw/master/images/DSP%E8%BF%9E%E6%8E%A5%E4%B8%8D%E4%B8%8ACCS3.3%E7%9A%84%E9%97%AE%E9%A2%98%E8%AE%A8%E8%AE%BA/PLL.png)

我就有一次EXCCET103的GND脚没焊好，怎么都连不上

- 
芯片相关的配置（数据手册上说明的特殊引脚）是否使用上拉/下拉电阻进行配置？TMS320C6713要进行如下的配置

![Config](https://github.com/xiahouzuoxin/notes/raw/master/images/DSP%E8%BF%9E%E6%8E%A5%E4%B8%8D%E4%B8%8ACCS3.3%E7%9A%84%E9%97%AE%E9%A2%98%E8%AE%A8%E8%AE%BA/Config.png)

另外，保留管脚RSV1~7的配置是否正确也请参考Datasheet

- 
实在没有办法，发现上面的检查都是正确的，恭喜你中奖了——赶紧换芯片吧……




