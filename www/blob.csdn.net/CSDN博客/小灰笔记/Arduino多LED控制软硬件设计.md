# Arduino多LED控制软硬件设计 - 小灰笔记 - CSDN博客





2017年12月24日 17:17:35[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：345








       发现Arduino确实是一个了解嵌入式硬件知识以及传感器技术很好的平台，这正好能够补充我之前几年工作中的短板。

       接下来做个多LED电路的设计。没有学过什么原理图的绘制工具，估计学了我自己也买不起，简单纸上表达一下如下：

![](https://img-blog.csdn.net/20171224171613476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       按照前面的面包板搭建电路的方式，搭建电路如下：

![](https://img-blog.csdn.net/20171224171629365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       就是一个LED换成了4个而已。编写对应的测试程序如下：



```
int led_pins[4] = {13,12,11,10};

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
  int i = 0;
  for(i = 0;i < 4;i++)
  {
    digitalWrite(13 - i, HIGH);
    delay(200);
  }
  for(i = 0;i < 4;i++)
  {
    digitalWrite(13 - i, LOW);
    delay(200);
  }
}
```



       编译烧写后，软硬件工作正常。



