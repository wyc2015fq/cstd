# 设计模式学习笔记——Chain of Responsibility职责链模式 - 左直拳的马桶_日用桶 - CSDN博客
2017年02月05日 15:25:28[左直拳](https://me.csdn.net/leftfist)阅读数：397
重点在链。一条链，如果本节点处理不了，则传递给下一个节点处理。
![这里写图片描述](https://img-blog.csdn.net/20170205130256258?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
关键是如何传给下一个节点？
主要是由本节点决定传给哪一个节点。
```java
public class Client
{
    public static void main(String args[])
    {
        Leader objDirector,objManager,objGeneralManager,objViceGeneralManager;
        //构造节点实例
        objDirector=new Director("王明");
        objManager=new Manager("赵强");
        objGeneralManager=new GeneralManager("李波");
        objViceGeneralManager=new ViceGeneralManager("肖红");
        //设置下一个节点是谁
        objDirector.setSuccessor(objManager);
        objManager.setSuccessor(objViceGeneralManager);
        objViceGeneralManager.setSuccessor(objGeneralManager);
        LeaveRequest lr1=new LeaveRequest("张三",2);
        objDirector.handleRequest(lr1);
        LeaveRequest lr2=new LeaveRequest("李四",5);
        objDirector.handleRequest(lr2);
        LeaveRequest lr3=new LeaveRequest("王五",15);
        objDirector.handleRequest(lr3);
        LeaveRequest lr4=new LeaveRequest("赵六",45);
        objDirector.handleRequest(lr4);         
    }
}
```
