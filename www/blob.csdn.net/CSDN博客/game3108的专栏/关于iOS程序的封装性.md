# 关于iOS程序的封装性 - game3108的专栏 - CSDN博客
2015年04月13日 15:56:01[game3108](https://me.csdn.net/game3108)阅读数：535
最近在项目中对封装性有了新的要求，但是oc中的语法并非如同c与java一般有良好的继承封装性。尤其是对于方法的封装，更加困难。
在stackoverflow上找到了对于方法封装的一个办法，自己记录一下：
http://stackoverflow.com/questions/3725857/protected-methods-in-objective-c
You can simulate protected and private access to methods by doing the following:
- Declare your private methods in a class continuation (i.e. a unnamed category declared near the top of the class' .m file)
- Declare your protected methods in a Subclass header – Apple uses this pattern with respect to UIGestureRecognizer (see documentation and reference to UIGestureRecognizerSubclass.h)
These protections are not, as Sachin noted, enforced at runtime (as they are in Java, for example).
