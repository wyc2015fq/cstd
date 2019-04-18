# Arduino串口数字显示 - 小灰笔记 - CSDN博客





2017年12月25日 00:15:48[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：867








       用了一下Arduino的串口功能，真好用啊！使用的时候，只需要配置一下速率就能够直接使用。如果是在软件开发阶段有这么个东西，那调试起来可就方便多了。

       简单写一个测试程序如下：



```
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int i = 0;

  Serial.println(i);
  if(i < 255)
  {
    i += 1;
  }
  else
  {
    i = 0;
  }
}
```



       直接查看串口回显结果：



![](https://img-blog.csdn.net/20171225001458400?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       其实，这个功能对我来说已然不错了。不过，Arduino IDE中还提供了图形化显示功能。具体效果如下：

![](https://img-blog.csdn.net/20171225001508702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       看来，以后这个串口功能还是值得去学习一下。之前一味地使用调试器，或许可以考虑一下串口这个廉价的手段了。



