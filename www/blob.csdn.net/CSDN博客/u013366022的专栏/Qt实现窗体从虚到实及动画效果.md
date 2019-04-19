# Qt实现窗体从虚到实及动画效果 - u013366022的专栏 - CSDN博客
2016年08月26日 17:04:20[slitaz](https://me.csdn.net/u013366022)阅读数：1608

一，实现界面从虚变实
方法1：使用QWidget类提供的公有方法voidsetWindowOpacity(qreallevel);（参数为double类型）
     a，使用此方法应该在窗体show之后在调用，否则可能会出现窗体会闪一下实体窗口，然后才会从虚到实变化。
     b，调用此方法时窗体不能设置Qt::FramelessWindowHint风格，否则该方法将不生效
方法2：使用QPropertyAnimation类实现
QPropertyAnimation*animation=newQPropertyAnimation(myWidget,"windowOpacity");//创建QPropertyAnimation对象
animation->setDuration(1000)；//设置持续时间animation->setStartValue(0);//设置起始透明度 0透明  1不透明animation->setEndValue(1);//设置结束透明度animation->start();//开始动画效果二，实现界面动画效果1，效果使myWidget从左上角移动至（250， 250）QPropertyAnimation *animation = new QPropertyAnimation(myWidget, "geometry");animation->setDuration(10000);animation->setStartValue(QRect(0, 0, 100, 30));animation->setEndValue(QRect(250, 250, 100, 30));animation->start();2，使用setEasingCurve设置相关属性可以进行其他曲率线性移动
QPropertyAnimation *anim1=new QPropertyAnimation(bird_1, "pos");
anim1->setDuration(2000); 
anim1->setStartValue(QPoint(0, 360)); 
anim1->setEndValue(QPoint(110, 180)); 
anim1->setEasingCurve(QEasingCurve::OutBounce);  
anim1->start();
3，使用QAnimationGroup实现多个动画同时进行效果
QAnimationGroup有两个子类QSequentialAnimationGroup（串行动画组）和QParallelAnimationGroup（并行动画组）
使用方法：
QSequentialAnimationGroup group;
//QParallelAnimationGroup group;
group.addAnimation(anim1); 
group.addAnimation(anim2); 
group.start();  
