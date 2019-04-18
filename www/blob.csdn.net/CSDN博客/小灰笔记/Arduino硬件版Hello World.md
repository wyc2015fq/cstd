# Arduino硬件版Hello World - 小灰笔记 - CSDN博客





2017年12月24日 17:00:18[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：236








       我并不懂硬件，刚刚接触Arduino并自己尝试动手搭建电路测试。我觉得，如果硬件最初的设计有个类似“Hello World”的功能，那么应该就是LED电路了。而配合硬件工作的软件可以做个LED闪烁之类的程序。

       手头有一块儿面包板，起初也不知道如何用，还差点把后面的胶什么的都拆了。打开一点发现里面全是金属连接片后，觉得这应该就是一个配合使用的整体。

![](https://img-blog.csdn.net/20171224165709379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       我的面包板跟上面这个一样，如果按照这个分成行列的话。中心沟槽两边可以看成是两块对称隔离的功能分区，便于两列是电源接入部分，正负两列的各孔分别连接导通。中间两块数行5列的区域，每一行的5列是连接导通的。不同行之间，并不连接导通。

       基于上面的信息，搭建一个LED电路如下：

![](https://img-blog.csdn.net/20171224165727698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       看不太清，略作解释：LED正端连接Arduino的13脚，负端先连一个1K的电阻然后接地。LED的两个管脚在不同的两行。

       程序实现：13脚设置为GPIO的输出，然后延迟一定时间输出翻转。正好Arduino中有相应的例子程序，具体如下：






```
/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
```

程序编译烧写后，LED按照预期闪烁。






