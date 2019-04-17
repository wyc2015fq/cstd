# 韦根协议-C编程实现 - V__KING__的专栏 - CSDN博客





2018年07月17日 17:18:13[v__king__](https://me.csdn.net/V__KING__)阅读数：1386








## 一. 韦根协议介绍

### 定义

Wiegand（韦根）协议是由摩托罗拉公司制定的一种通讯协议，它适用于涉及门禁控制系统的读卡器和卡片的许多特性，其协议并没有定义通讯的波特率、也没有定义数据长度韦根格式主要定义是数据传输方式：Data0和Data1两根数据线分别传输0和1，现在应用最多的是26bit，34bit，36bit，44bit等等，其中标准26-bit 格式是一个开放式的格式，任何人都可以购买某一特定格式的HID卡，并且这些特定格式的种类是公开可选的，26-Bit格式就是一个广泛使用的工业标准，并且对所有HID的用户开放，现在几乎所有的门禁控制系统都接受26-Bit格式的标准。

### 时序

根数据输出由二根线组成，分别是DATA0 和 DATA1 ；二根线分别为‘0’或‘1’输出。 

输出‘0’时：DATA0线上出现负脉冲； 

输出‘1’时：DATA1线上出现负脉冲； 

负脉冲宽度TP=100微秒；周期TW=1600微秒；
### 韦根26输出格式：

bit0为bit1~bit12的偶校验[1] 

bit1~bit24为3字节卡号信息 

bit25为bit13~bit24的奇校验
> 
[1]通常专门设置一个奇偶校验位，用它使这组代码中“1”的个数为奇数或偶数


## 二. 韦根协议编程

### 韦根协议接收

韦根的数据线：一个专门发送0， 一个专门发送1。用两个IO端口来接收韦根数据，两个IO都用中断方式来接收数据。代码入下：

```
volatile long reader = 0;
volatile int readerCount = 0;
long sign = 0xff0000;

void readerOne(void) 
{
  readerCount++;
  reader = reader << 1;
  reader |= 1;
}

void readerZero(void)
{
  readerCount++;
  reader = reader << 1;
}

void setup()
{
  Serial.begin(9600);
  attachInterrupt(3, readerZero, RISING);
  attachInterrupt(2, readerOne, RISING); 
  delay(10);
  reader = 0;
  readerCount = 0; 
}

void loop()
{
  if(readerCount >=26)
  {
    Serial.print(" Reader:");
    Serial.print(reader);
    Serial.println("");
    reader = reader & 0x1fffffe;
    reader = reader >> 1;

    long hid = reader & sign;
    hid = hid >> 16;
    hid = hid * 256 * 256;
    sign = sign >> 8;

    long pid1 = reader & sign;
    pid1 = pid1 >> 8;
    pid1 = pid1 * 256;
    sign = sign >> 8;

    long pid2 = reader & sign;
    sign = 0xff0000;

    long value = hid + pid1 + pid2;
    Serial.println( value);

    reader = 0, readerCount = 0;
    hid = 0, pid1 = 0, pid2 = 0, value = 0;
  }
}
```

### 韦根协议26发送
- 发送 偶校验位bit0
- 发送bit1~bit24 总共3字节卡号信息
- 发送bit25为bit13~bit24的奇校验

> 
容易理解代码：[http://blog.sina.com.cn/s/blog_4bb1137401010426.html](http://blog.sina.com.cn/s/blog_4bb1137401010426.html)

  奇偶校验算法：[https://www.cnblogs.com/Yogurshine/p/3949723.html](https://www.cnblogs.com/Yogurshine/p/3949723.html)

  百度：[https://baike.baidu.com/item/%E9%9F%A6%E6%A0%B9%E5%8D%8F%E8%AE%AE/9914978?fr=aladdin](https://baike.baidu.com/item/%E9%9F%A6%E6%A0%B9%E5%8D%8F%E8%AE%AE/9914978?fr=aladdin)
```
void Send_Weigand26(unsigned char *str)
{
    unsigned char one_num = 0;
    unsigned char even = 0;
    unsigned char odd = 0;
    unsigned char check_temp,i;
    check_temp = *str;
    for(i = 0;i < 8;i++)
    {
        if(check_temp & 0x01)
            one_num++;
        check_temp >>= 1;
    }
    check_temp = *(str + 1);
    for(i = 0;i < 4;i++)
    {
        if(check_temp & 0x80)
            one_num++;
        check_temp <<= 1;
    }
    if(one_num % 2 )
        even = 1;
    else
        even = 0;
    one_num = 0;
    check_temp = *(str + 1);
    for(i = 0;i < 4;i++)
    {
        if(check_temp & 0x01)
            one_num++;
        check_temp >>= 1;
    }
    check_temp = *(str + 2);
    for(i = 0;i < 8;i++)
    {
        if(check_temp & 0x01)
            one_num++;
        check_temp >>= 1;
    }
    if(one_num % 2 )
        odd = 0;
    else
        odd = 1;
    one_num = 0;
    WG_DATA0 = 1;
    WG_DATA1 = 1;
    Delay_1ms(2);
    if(even)
    {
        WG_DATA1 = 0;                   
        Delay_50us(8);
        WG_DATA1 = 1;
    }
    else
    {           
        WG_DATA0 = 0;                  
        Delay_50us(8);
        WG_DATA0 = 1;
    }
    Delay_1ms(2);                       
    for(i = 0;i < 24;i++)
    {
        WG_DATA0 = 1;
        WG_DATA1 = 1;
        if(str[0] & 0x80)
        {
            WG_DATA1 = 0;
            Delay_50us(8);
            WG_DATA1 = 1;
        }
        else
        {
            WG_DATA0 = 0;
            Delay_50us(8);
            WG_DATA0 = 1;
        }
        (*(long*)&str[0]) <<= 1;
        Delay_1ms(2);              
    }
    WG_DATA0 = 1;
    WG_DATA1 = 1;
    if(odd)
    {
        WG_DATA1 = 0;
        Delay_50us(8);
        WG_DATA1 = 1;
    }
    else
    {           
        WG_DATA0 = 0;
        Delay_50us(8);
        WG_DATA0 = 1;
    }
    Delay_1ms(2);  
}

void Send_Weigand34(unsigned char *str)
{
    unsigned char one_num = 0;
    unsigned char even = 0;
    unsigned char odd = 0;
    unsigned char check_temp,i;
    check_temp = *str;
    for(i = 0;i < 8;i++)
    {
        if(check_temp & 0×01)
            one_num++;
        check_temp >>= 1;
    }
    check_temp = *(str + 1);
    for(i = 0;i < 8;i++)
    {
        if(check_temp & 0×01)
            one_num++;
        check_temp >>= 1;
    }
    if(one_num % 2 )
        even = 1;
    else
        even = 0;
    one_num = 0;
    check_temp = *(str + 2);
    for(i = 0;i < 8;i++)
    {
        if(check_temp & 0×01)
            one_num++;
        check_temp >>= 1;
    }
    check_temp = *(str + 3);
    for(i = 0;i < 8;i++)
    {
        if(check_temp & 0×01)
            one_num++;
        check_temp >>= 1;
    }
    if(one_num % 2 )
        odd = 0;
    else
        odd = 1;
    one_num = 0;
    WG_DATA0 = 1;
    WG_DATA1 = 1;
    Delay_1ms(2);
    if(even)
    {
        WG_DATA1 = 0;                   
        Delay_50us(8);
        WG_DATA1 = 1;
    }
    else
    {           
        WG_DATA0 = 0;                  
        Delay_50us(8);
        WG_DATA0 = 1;
    }
    Delay_1ms(2);                       
    for(i = 0;i < 32;i++)
    {
        WG_DATA0 = 1;
        WG_DATA1 = 1;
        if(str[0] & 0×80)
        {
            WG_DATA1 = 0;
            Delay_50us(8);
            WG_DATA1 = 1;
        }
        else
        {
            WG_DATA0 = 0;
            Delay_50us(8);
            WG_DATA0 = 1;
        }
        (*(long*)&str[0]) <<= 1;
        Delay_1ms(2);              
    }
    WG_DATA0 = 1;
    WG_DATA1 = 1;
    if(odd)
    {
        WG_DATA1 = 0;
        Delay_50us(8);
        WG_DATA1 = 1;
    }
    else
    {           
        WG_DATA0 = 0;
        Delay_50us(8);
        WG_DATA0 = 1;
    }
    Delay_1ms(2);  
}
```







