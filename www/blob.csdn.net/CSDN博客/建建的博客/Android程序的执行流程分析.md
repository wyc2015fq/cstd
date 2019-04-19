# Android程序的执行流程分析 - 建建的博客 - CSDN博客
2017年05月31日 14:40:27[纪建](https://me.csdn.net/u013898698)阅读数：203
10.3  Android程序的执行流程分析
经过前面对Android项目目录结构的介绍以及相关文件的讲解，我们对许多细节已经有所了解，只是Android程序是如何执行的呢？下面进行总结。
发布程序到手机上之后，当双击"抽屉"里该应用的图标时，系统会将这个事件包装成一个Intent，该Intent包含两个参数，代码如下：
- {action ："android.intent.action.MAIN"，  
-          category ："android.intent.category.LAUNCHER" }， 
这个意图被传递给Hello World这个应用之后在应用的功能清单文件中寻找与该意图匹配的意图过滤器，如果匹配成功，找到相匹配的意图过滤器所在的Activity元素，再根据<activity>元素的"name"属性来寻找其对应的Activity类。接着Android[操作系统](http://lib.csdn.net/base/operatingsystem)创建该Activity类的实例对象，对象创建完成之后，会执行该类的onCreate方法，此OnCreate方法是重写其父类Activity的OnCreate方法。此方法用来初始化Activity实例对象。以下是HelloWorld.[Java](http://lib.csdn.net/base/javase) 类中onCreate方法的代码。
- @Override  
-     public void onCreate（Bundle savedInstanceState） {  
-         super.onCreate（savedInstanceState）；  
-         setContentView（R.layout.main）；  
- }  
其中super.onCreate（savedInstanceState）的作用是调用其父类Activity的OnCreate方法来实现对界面的画图绘制工作。在实现自己定义的Activity子类的OnCreate方法时一定要记得调用该方法，以确保能够绘制界面。
SetContentView（R.layout.main）的作用是加载一个界面。该方法中传入的参数是"R.layout.main"，其含义为R.java类中静态内部类layout的静态常量main的值，而该值是一个指向"res"目录下的"layout"子目录下main.xml文件的标识符。因此代表着显示main.xml所定义的画面。
关于Activity类的执行流程及其生命周期会在后面的部分详细讲解。
Android程序执行的整个序列图如下图所示。
![](http://images.51cto.com/files/uploadimg/20110817/185430293.jpg)
