# Arduino按键软硬件实现 - 小灰笔记 - CSDN博客





2017年12月24日 23:52:27[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：183










![](https://img-blog.csdn.net/20171224235233545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       还是老毛病，原理图软件工具就直接省掉了。上面简单手绘一个原理图，如上。Key按下接通的时候，采集点是高电平。而Key断开的时候，经过一个10K电阻后连接到0V（GND），采集点是低电平。

       电路验证在面包板上搭建，采集使用管脚6，额外再加上一个LED的显示，使用13脚。根据采集到的电平信号来驱动LED。如此，编写代码如下：



```
void setup() {
  //put your setup code here, to run once:
 pinMode(6,INPUT);
 pinMode(13,OUTPUT);
}
 
void loop() {
  //put your main code here, to run repeatedly:
  intkey_signal = 0;
 
 key_signal = digitalRead(6);
 digitalWrite(13,key_signal);
}
```



       软件编译下载后，LED常亮，如果按下按键LED则熄灭。



