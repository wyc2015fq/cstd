# 设计模式学习笔记——Observer观察者模式 - 左直拳的马桶_日用桶 - CSDN博客
2017年02月17日 08:02:19[左直拳](https://me.csdn.net/leftfist)阅读数：450
观察者模式里面有两个东西：观察者（Observer）和目标（Subject）。当目标发生变化的时候，观察者随之起舞，也作出相应的变化。此为观察者模式。
这是怎么做到的？主要是目标里面存有一份观察者的名单，当目标变化时，可以按照名单逐一通知，于是观察者们做出自己的响应。
![这里写图片描述](https://img-blog.csdn.net/20170217075926447?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
public class Client
{
    public static void main(String a[])
    {
        MySubject subject=new Cat();
        MyObserver obs1,obs2,obs3;
        obs1=new Mouse();
        obs2=new Mouse();
        obs3=new Dog();
        subject.attach(obs1);
        subject.attach(obs2);
        subject.attach(obs3);
        MyObserver obs4;
        obs4=new Pig();
        subject.attach(obs4);
        subject.cry();      
    }
}
```
