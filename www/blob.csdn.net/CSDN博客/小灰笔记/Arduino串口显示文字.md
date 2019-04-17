# Arduino串口显示文字 - 小灰笔记 - CSDN博客





2017年12月25日 00:30:02[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1160








       串口在嵌入式调试的应用中的用途比我想象中的要强大不少，最简单的一点就是文字显示。这样，在调试的时候就能够实现变量实时刷新这样的类似功能。

       代码如下：



```
void setup() {
  //put your setup code here, to run once:
 Serial.begin(9600);
}
 
void loop() {
  //put your main code here, to run repeatedly:
   static unsigned char counter1 = 0;
   static unsigned char counter2 = 0;
   Serial.println("hello world");
   Serial.println("counter1");
   Serial.println(counter1);
   Serial.println("counter2");
   Serial.println(counter2);
 
   counter1 += 1;
   counter2 += 2;
}
```



       编译下载后执行效果：

![](https://img-blog.csdn.net/20171225002921858?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






