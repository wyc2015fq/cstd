# QT中layoutstretch属性简析 - fanyun的博客 - CSDN博客
2017年10月20日 18:13:02[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：845
      相对一个horizontalLayout中的两个QTextEdit控件按2:3的比例扩大
具体方法：
     把horizontalLayout中的layoutstretch属性设置为2,3即可，layoutstretch属性的意思是horizontalLayout里面的两个控件按2：3的宽度显示。
同理如果horizontalLayout里面放了三个控件，layoutstretch里面的初始值为0,0,0，意思是三个控件的比例是1：1：1。
      其他控件中的layoutstretch也是这个意思。
