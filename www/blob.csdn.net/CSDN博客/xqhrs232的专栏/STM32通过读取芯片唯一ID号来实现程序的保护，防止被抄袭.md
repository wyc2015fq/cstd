# STM32通过读取芯片唯一ID号来实现程序的保护，防止被抄袭 - xqhrs232的专栏 - CSDN博客
2018年05月02日 11:04:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：379
原文地址::[http://www.51hei.com/bbs/dpj-42683-1.html](http://www.51hei.com/bbs/dpj-42683-1.html)
相关文章
1、
## STM32实验连载之四：读取芯片唯一ID号
 ----[http://blog.sina.com.cn/s/blog_b1ed84120101fmlo.html](http://blog.sina.com.cn/s/blog_b1ed84120101fmlo.html)
2、[【STM32F4】读取芯片ID和芯片Flash Size](http://www.cnblogs.com/zyqgold/p/3378993.html)----[https://www.cnblogs.com/zyqgold/p/3378993.html](https://www.cnblogs.com/zyqgold/p/3378993.html)
3、STM32读芯片ID号----[https://download.csdn.net/download/jonahan001/5342889](https://download.csdn.net/download/jonahan001/5342889)
4、如何读取STM32的唯一ID----[https://blog.csdn.net/u012388993/article/details/71425063](https://blog.csdn.net/u012388993/article/details/71425063)
5、[【STM32F4】读取芯片ID和芯片Flash Size](http://www.cnblogs.com/zyqgold/p/3378993.html)----[https://www.cnblogs.com/zyqgold/p/3378993.html](https://www.cnblogs.com/zyqgold/p/3378993.html)
大家好！我叫肖亚平，从小热爱上了电子技术，读书时阴差阳错的选择了电子这方面的专业，学习电子技术、一直到今年毕业。对于我的理解来说，学校里面学到的技术不是全部实用，但是必须有用，所以一直奋斗在前线。我对学习总结出一句话“压力不是有人努力，而是比你牛X N倍的人依然在努力”
STM32通过读取芯片唯一ID号来实现程序的保护，防止被抄袭。STM32芯片通过STlink可以把程序读出来。只要在相同的芯片上面，就可以跑起来，那么如何才能让一个程序只能够在一个芯片上正常运行呢？
经过一个下午的时间，总结出这个方法。通过读取ID号，然后在修改HEX文件来保护自己的程序不被抄袭，相对于开发成本中等的产品，还是有用的。但是，有一点麻烦。我用的是STM32F103ZET6芯片。下面将介绍详细的步骤和方法。
一．获取ID码
- 工欲善其事，必先利其器，准备好工具，需要一个STlink及下载软件，一个可以正常运行的硬件。
- uint16_t temp[12];   //存放芯片ID的临时变量
- uint16_t aa[12] = {0xee,0x01,0x02,0x03,0x04,
-                0x05,0x06,0x07,0x08,0x09,
-                0x10,0x11};//预置ID号，在HEX文中修改。
- 
- aa这个数组用于存放预置ID号
复制代码
- /******************************************************************************
- 函数名称：读取芯片唯一ID码
- 创建时间：2015-08-11
- 修改时间：2015-08-11
- 备    注：
- ******************************************************************************/
- 
- void Get_ChipID(void)
- {
-     u32 temp0,temp1,temp2;
- temp0 = *(__IO u32*)(0x1FFFF7E8);    //产品唯一身份标识寄存器（96位）
-     temp1 = *(__IO u32*)(0x1FFFF7EC);
- temp2 = *(__IO u32*)(0x1FFFF7F0);
- 
- //ID码地址： 0x1FFFF7E8   0x1FFFF7EC  0x1FFFF7F0 ，只需要读取这个地址中的数据就可以了。
- 
-     temp[0] = (u8)(temp0 & 0x000000FF);
-     temp[1] = (u8)((temp0 & 0x0000FF00)>>8);
-     temp[2] = (u8)((temp0 & 0x00FF0000)>>16);
-     temp[3] = (u8)((temp0 & 0xFF000000)>>24);
-     temp[4] = (u8)(temp1 & 0x000000FF);
-     temp[5] = (u8)((temp1 & 0x0000FF00)>>8);
-     temp[6] = (u8)((temp1 & 0x00FF0000)>>16);
-     temp[7] = (u8)((temp1 & 0xFF000000)>>24);
-     temp[8] = (u8)(temp2 & 0x000000FF);
-     temp[9] = (u8)((temp2 & 0x0000FF00)>>8);
-     temp[10] = (u8)((temp2 & 0x00FF0000)>>16);
-     temp[11] = (u8)((temp2 & 0xFF000000)>>24);         
- }
复制代码
- 通过void Get_ChipID(void)就可以得到ID码了，这个ID码可以用串口输出，也可以用STM32 ST-LINK Utility读出来。这里，两种方法我都会讲到。
- 
- 方法一：通过串口输出得到ID码。
- 
- /****************************************************************************************************************************
- 函数名称：串口2初始化配置
- 创建时间：2015-08-11
- 修改时间：2015-08-11
- 备    注：
- *****************************************************************************************************************************/
- void USART2_Config(void)
- {
- GPIO_InitTypeDef   GPIO_InitStructure;  //定义GPIO类型结构体
- USART_InitTypeDef  USART_InitStructure; //定义串口类型结构体
- RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //配置GPIOA时钟，并使能时钟。
- RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//配置USART2的时钟，并使能时钟
- GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;       //配置PA.02作为TXD
- GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //配置成推挽输出
- GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO时钟为50MHZ
- GPIO_Init(GPIOA, &GPIO_InitStructure);           //初始化GPIO
- GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  //配置PA.03作为RXD
- GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //配置GPIO输入浮空
- GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化GPIO
- USART_InitStructure.USART_BaudRate = 115200;  //配置串口波特率为115200
- USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据位
- USART_InitStructure.USART_StopBits = USART_StopBits_1; //1位停止位
- USART_InitStructure.USART_Parity = USART_Parity_No; //
- USART_InitStructure.USART_HardwareFlowControl = T_HardwareFlowControl_None; //硬件流程控制
- USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //启动发送和接收
- USART_Init(USART2, &USART_InitStructure); //初始化串口2
- 
- USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //使能串口2接收中断 
- USART_Cmd(USART2, ENABLE); //使能外部中断
- }
复制代码
- /****************************************************************************************************************************
- 函数名称：USART2发送数据函数
- 创建时间：2015-08-11
- 修改时间：2015-08-11
- 备    注：
- *****************************************************************************************************************************/
- void Usart2_SendData(uint16_t uiSendDataNumber,uint16_t * uiData)
- {
-   static uint16_t uiTempData = 0; //发送数据临时变量
- for(uiTempData = uiSendDataNumber;uiTempData > 0;uiTempData--) //数据的个数
- {
- USART_SendData(USART2, *uiData++); //调用发送函数
- //           uiData++; //发送数据的地址加1，切换到下一个要发送数据的地址。
- while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);//发送缓冲区空状态标志位。只有当缓冲区为空时，才发送下一个数据。
-   }
- }
复制代码
串口配置好后，就可以调用Usart2_SendData(12,temp);函数，就能在串口助手看到ID码了。
![](http://c.51hei.com/d/forum/201601/21/145551chh700ynpls3bbne.png)
- 芯片唯一ID码：32 FF DA 05 43 41 38 36 30 71 02 43   96位
复制代码
方法二：通过STlink得到ID码。
1、打开STM32 ST-LINK Utility，确保STlink与硬件连接正常。
2、在Address中输入“0x1FFFF7E8”芯片ID码的首地址。输入完成后，STlink自动读取ID码。
![](http://c.51hei.com/d/forum/201601/21/145551od4zya7aid9g9aka.png)
3、确定后，自动读取ID码
![](http://c.51hei.com/d/forum/201601/21/145551mo1ez0jfhjcggeor.png)
但是，用STlink读出来的只有64位，少了32位，这个“0x1FFFF7F0”地址中读取失败，我也不知道是什么原因，可能是厂家不允许下载器访问。
|地  址|0x1FFFF7E8|0x1FFFF7EC|0x1FFFF7F0|
|----|----|----|----|
|ID数据|05 DA FF 32|36 38 41 43|空（不允许读）|
我们把上面ID码整理一下：32 FF DA 05 43 41 38 36，由于在内存中是小端存储方式。所以是这样的。
通过两种方法，把我们想要的ID码得到后。就可以进行最关键的一步，修改HEX文件。
二.修改HEX文件加密
1、打开STM32 ST-LINK Utility，打开HEX文件。
![](http://c.51hei.com/d/forum/201601/21/145552udeidwzyis7w9wd9.png)
- uint16_t  aa[12] = {0xee,0x01,0x02,0x03,0x04,
-                0x05,0x06,0x07,0x08,0x09,
-                0x10,0x11};//预置ID号，在HEX文中修改。
复制代码
现在就打HEX文件打开后。找到上面定义的数组，这个是关键所在，我花了一个下午时间，才搞清楚。
[size=9.0000pt]2、找到自定义ID在地址的位置后，开始修改。
![](http://c.51hei.com/d/forum/201601/21/145552a6e36d267r676d6w.png)
3、把数组“aa[12]” 里面的内容改成与芯片的实际ID码一致。如果程序被读出来，在另外一片相同的芯片上面也运行不起来。因为设置的ID与芯片实际ID不一致。
![](http://c.51hei.com/d/forum/201601/21/145552zd15qqddfxddpdqy.png)
4、现在如果下载，会提示你文件被修改，需要保存。我们保存成HEX文件到桌面。
![](http://c.51hei.com/d/forum/201601/21/145552a2g3im33jgig3z25.png)
5、重新打开刚才保存的HEX文件。现在可以看到，修改的ID已经在HEX文件里面了。
![](http://c.51hei.com/d/forum/201601/21/145553qjf0aja37c3fskv3.png)
6、下载程序到单片机。
![](http://c.51hei.com/d/forum/201601/21/145553j8b0b098rtpir08r.png)
7、完成下载。到这儿就基本完成了。
8、看到这儿，有的人会问，为什么不在编译时直接放在数组里，还要在HEX文件中修改，这样做的目的是方便批量生产。
![](http://c.51hei.com/d/forum/201601/21/145553jncjhb36v3nobz3d.png)
- 在程序中，我做了一个简单的判断。如下
-   Get_ChipID();  //先读出芯片实际ID          
-   for(i=0;i<12;i++)
-         {
-                         if(temp == aa) //判断设置的ID与实际ID是不是一致。如果一致。LED灯不亮
-                         {
-                                 ;
-                         }
-                         else
-                         {
-                                  GPIO_ResetBits(GPIOF, GPIO_Pin_6);
-             }
-   }
复制代码

