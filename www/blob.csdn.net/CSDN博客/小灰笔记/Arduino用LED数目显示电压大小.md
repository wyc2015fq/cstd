# Arduino用LED数目显示电压大小 - 小灰笔记 - CSDN博客





2017年12月24日 21:38:15[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：701








       Arduino的开发环境中AD资源的应用非常简单，甚至说比GPIO还简单。这也可以理解，GPIO具有两个方向而AD只有一个方向。开发环境以库的方式提供了AD采集的功能，只需要一个简单的函数：


`analogRead(PIN_NAME)`

例如analogRead(A3)可以读取A3管脚采集到的电压。

而另一个函数可以用LED数目来表示电压的高低，有种手机上显示几格电量的感觉。map(value, fromLow, fromHigh, toLow, toHigh)。几个参数的含义：

value: thenumber to map（AD值）

fromLow: thelower bound of the value's current range（最小AD值）

fromHigh: theupper bound of the value's current range（最大AD值）

toLow: the lowerbound of the value's target range（最低电量下的LED数目）

toHigh: theupper bound of the value's target range（最高电量下的LED数目）

由此，使用13、12、11、10四个脚驱动共阳的4个LED，A3用于采集AD。实现代码如下：



```
void setup() {
  // put your setup code here, to run once:
  int i = 0;

  for(i = 0;i<4;i++)
  {
    pinMode(13 - i,OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int ad_value_raw = 0;
  int led_level = 0;
  int i = 0;
  
  ad_value_raw = analogRead(A3);
  led_level = map(ad_value_raw,0,1023,1,4);

  for(i = 0; i < 4; i++)
  {
    if(i < led_level)
    {
      digitalWrite(13 - i,LOW);
    }
    else
    {
      digitalWrite(13 - i,HIGH);
    }
  }
}
```



       编译下载之后，程序运行正常。



