
# android实现类似于支付宝余额快速闪动的效果 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年02月09日 16:57:58[Jlins](https://me.csdn.net/dyllove98)阅读数：2672



## 效果如下：
**此图片不会动，但实际上是会快速跳动的。**
**之前看到有支付宝的效果非常牛逼。就是进去看到余额呼噜噜的直接上蹿下跳到具体数字，效果帅，但不知道怎么实现，最近终于知道了。**
![image](http://images.cnitblog.com/blog/287866/201502/091406558234810.png)
## 思路：
首先经常用到倒计时操作。这个类就是 CountDownTimer.Android自带的。继承此类。无非就是跳动的 increase和intervel
增加的数字和间隔！你懂。继续。。然后让他快速从一定数字跳动到具体的总额数字，无废话了。。看Demo看效果
## 代码：
直接调用执行：
privatevoidinitAllViews() {
        myDanceTimer =newDanceWageTimer(DanceWageTimer.getTotalExecuteTime(myMoney, 100), 100, tvMoney, myMoney);
        myDanceTimer.start();
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/*** @Title getTotalExecuteTime
     * @Description 得到总共执行的时间
     *@paramtotalWage
     *@return*/publicstaticintgetTotalExecuteTime(floattotalWage,intinterval) {intwage = getIntegerOfWage(totalWage);intstartNum = getStartNum(totalWage);intincreased = getIncreased(startNum);intresult = (wage - startNum) / increased * interval;returnresult;
    }![复制代码](http://common.cnblogs.com/images/copycode.gif)
Demo：下载地址：[http://pan.baidu.com/s/1mgkPnHQ](http://pan.baidu.com/s/1mgkPnHQ)[猛击这里](http://pan.baidu.com/s/1mgkPnHQ)

参考资料：


## android学习笔记（60）使用Bundle在Activity之间文换教接
## android学习笔记（59） android开发启动和关闭activity

## [android学习笔记（58）Activity学习过程](http://www.itmmd.com/201502/597.html)
## 更多
## [android开发](http://www.itmmd.com/)
## 教程



