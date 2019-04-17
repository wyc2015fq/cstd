# STM32F0308开发环境的选择--CooCox CoIDE篇 - DoubleLi - 博客园






STM32的开发环境有很多总，官方手册也提供了IAR Embedded Workbench、MDK-ARM和TrueSTUDIO这3种。今天我试用了[CooCox CoIDE](http://www.coocox.org/Index.html)，是免费的集成开发环境，同TI公司提供的Code Composer Studio CCS一样，基于Eclipse和GCC的全功能集成开发环境（IDE），两者的界面都差不多，用惯了CCSv5的话，就很容易上手了。下面开始我的试用。
**1.**下载并安装CooCox CoIDE：**CooCox CoIDE V1.7.5**      9/29/2013更新
下载地址：[http://www.coocox.org/CoIDE/CoIDE_Updates.htm](http://www.coocox.org/CoIDE/CoIDE_Updates.htm)，下面是界面：
![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115akccizgakx4dcq8l.png)

**2.**首先你需要自己下载安装编译交叉工具链[gcc-arm-none-eabi-4_7-2013q3-20130916-win32.exe](http://www.coocox.org/CoIDE/Compiler_Settings.html)。下载地址：[https://launchpad.net/gcc-arm-embedded/+download](https://launchpad.net/gcc-arm-embedded/+download)
![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115zyoyja0b02q8rd39.png)
安装完之后的目录：![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115benwtbdb6toxddyp.png)

**3.**配置CoIDE的编译器，选择Project->Select Toolchain Path
![](http://bbs.eeworld.com.cn/attachments/month_1311/201311156gr38qnvjgtdtp32.png)
选择你先前安装工具链的目录：例如我的是D:\\Program Files (x86)\\GNU Tools ARM Embedded\\4.7 2013q3\\bin
![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115re7kpj80jli6utrj.png)

**4.**接下来开始创建工程了，一步步按照下面操作，简单点击下就行了，省了好多事儿！![](http://bbs.eeworld.com.cn/images/smilies/default/lol.gif)
![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115kuwv5mw9q4hmooz7.png)

![](http://bbs.eeworld.com.cn/attachments/month_1311/201311156yygc3zx0wz10pga.png)

![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115xfnsvp59jlijve6o.png)
按照我下面的图打上勾，我第一个测试的GPIO控制
![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115p0fblrs769w3tvxi.png)

接下来点击Build:
![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115al5a7utfddo7egkg.png)
啊哦！怎么会有warning呢?好吧，先解决下。当然找度娘啦！![](http://bbs.eeworld.com.cn/images/smilies/default/hug.gif)
![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115pj2kmhkza5xzucvm.png)
找到出现警告的原因后，解决办法如下。（个人方法，仅供参考）![](http://bbs.eeworld.com.cn/images/smilies/default/sweat.gif)
![](http://bbs.eeworld.com.cn/attachments/month_1311/201311153g4mfgxy5fpbdvuf.png)

![](http://bbs.eeworld.com.cn/attachments/month_1311/201311151yeul9flpwjn0uv2.png)
再来编译下，没有警告了。
![](http://bbs.eeworld.com.cn/attachments/month_1311/201311155iuemydlhytzjgwa.png)


**5.**接下来就是编写程序了[code]#include \"stm32f0xx.h\"
#include \"stm32f0xx_rcc.h\"
#include \"stm32f0xx_gpio.h\"

GPIO_InitTypeDef GPIO_InitStructure;

int main(void)
{
        uint32_t times;
  /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* Configure PC8 and PC9 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  while (1)
  {
    /* Set PC8 and PC9 */
    GPIO_SetBits(GPIOC, GPIO_Pin_8);
        /* Delay some time */
        for(times = 0; times < 500000; times++);
    GPIO_SetBits(GPIOC, GPIO_Pin_9);
        for(times = 0; times < 500000; times++);

        GPIO_ResetBits(GPIOC, GPIO_Pin_8);
        for(times = 0; times < 500000; times++);
        GPIO_ResetBits(GPIOC, GPIO_Pin_9);
        for(times = 0; times < 500000; times++);
  }
}
[/code][code]
[/code]
**6.**你也可以查看官方提供的例程
![](http://bbs.eeworld.com.cn/attachments/month_1311/20131115mhpivwfmlxsiadla.png)


**7.**总结：有人问这么多的开发环境，该怎么选择呢？其实你大可以每款软件都尝试用一下，根据自己的口味来选择。比如人家分享的心得，在Ubuntu下开发STM32，其实也是基于Eclipse和GCC，这就涉及到了开源了。我喜欢开源。过两天有时间也配置下Ubuntu13.10下的STM32F0的开发环境，图文并茂哦！![](http://bbs.eeworld.com.cn/images/smilies/default/loveliness.gif)









