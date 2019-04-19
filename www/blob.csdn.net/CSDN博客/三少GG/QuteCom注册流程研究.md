# QuteCom注册流程研究 - 三少GG - CSDN博客
2012年12月15日 14:51:15[三少GG](https://me.csdn.net/scut1135)阅读数：840
最近在研究QuteCom注册流程那块怎么走，准备做相应修改以适应我们自己的应用服务器,记下了几行笔记
    发现它的注册流程如下截图所示：
![](http://ymbian.blog.51cto.com/attachment/200912/200912081260240881112.jpg)
    QuteCom 在向服务器发送三个regsiter消息之后才宣称自己注册成功：
-  第一个register 并非用于绑定自己的AOR,仅仅做Ping网络之用
-  第二个register 用于绑定自己的AOR,是正常的sip 注册消息
-  第二个407response 用于challenge 终端，要求鉴权
-  第三个register 携带鉴权信息，再次向服务器请求绑定AOR，待200 OK收到 之后，终端注册成功.
-  当然自此之后终端并非无事可作，会有专门的线程update registration expires ，以及QuteCom自定义的一些数据。
      待日后补充关于QuteCom 对信令（eXosip），多媒体(CodeC)，承载（oRtp）处理时的封装
