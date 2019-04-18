# Java 8特性探究(1)：通往lambda之路_语法篇 - z69183787的专栏 - CSDN博客
2015年11月18日 15:05:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：725
现在开始要灌输一些概念性的东西了，这能帮助你理解lambda更加透彻一点，如果你之前听说过，也可当是温习，所谓温故而知新......
在开始之前，可以同步下载jdk 8 和  IDE，IDE根据个人习惯了，不过eclipse官方版本还没出来，所以目前看的话，netbean7.4是首选的，毕竟前段子刚刚出的正式版本，以下是他们的下载地址。
- jdk 8：[https://jdk8.java.net/download.html](https://jdk8.java.net/download.html)
- Netbeans 7.4正式版[:https://netbeans.org/downloads/](https://netbeans.org/downloads/)(推荐，oracle官方发布)
- IDEA 12 EAP [http://confluence.jetbrains.net/display/IDEADEV/IDEA+12+EAP](http://confluence.jetbrains.net/display/IDEADEV/IDEA+12+EAP)
- Unofficial builds of Eclipse[:http://downloads.efxclipse.org/eclipse-java8/](http://my.oschina.net/benhaile/admin/:http:/downloads.efxclipse.org/eclipse-java8)
函数式接口
函数式接口（functional interface 也叫功能性接口，其实是同一个东西）。简单来说，函数式接口是只包含一个方法的接口。比如Java标准库中的java.lang.Runnable和 java.util.Comparator都是典型的函数式接口。java 8提供 @FunctionalInterface作为注解,这个注解是非必须的，只要接口符合函数式接口的标准（即只包含一个方法的接口），虚拟机会自动判断， 但 最好在接口上使用注解@FunctionalInterface进行声明，以免团队的其他人员错误地往接口中添加新的方法。
Java中的lambda无法单独出现，它需要一个函数式接口来盛放，lambda表达式方法体其实就是函数接口的实现，下面讲到语法会讲到
Lambda语法
包含三个部分
一个括号内用逗号分隔的形式参数，参数是函数式接口里面方法的参数
一个箭头符号：->
方法体，可以是表达式和代码块，方法体函数式接口里面方法的实现，如果是代码块，则必须用{}来包裹起来，且需要一个return 返回值，但有个例外，若函数式接口里面方法返回值是void，则无需{}
总体看起来像这样
- (parameters)-> expression 或者(parameters)->{ statements;}
看一个完整的例子，方便理解
- /**
-  * 测试lambda表达式
-  *
-  * @author benhail
-  */
- publicclassTestLambda{
- publicstaticvoid runThreadUseLambda(){
- //Runnable是一个函数接口，只包含了有个无参数的，返回void的run方法；
- //所以lambda表达式左边没有参数，右边也没有return，只是单纯的打印一句话
- newThread(()->System.out.println("lambda实现的线程")).start();
- }
- publicstaticvoid runThreadUseInnerClass(){
- //这种方式就不多讲了，以前旧版本比较常见的做法
- newThread(newRunnable(){
- @Override
- publicvoid run(){
- System.out.println("内部类实现的线程");
- }
- }).start();
- }
- publicstaticvoid main(String[] args){
- TestLambda.runThreadUseLambda();
- TestLambda.runThreadUseInnerClass();
- }
- }
可以看出，使用lambda表达式设计的代码会更加简洁，而且还可读。
方法引用
其实是lambda表达式的一个简化写法，所引用的方法其实是lambda表达式的方法体实现，语法也很简单，左边是容器（可以是类名，实例名），中间是"::"，右边是相应的方法名。如下所示：
- ObjectReference::methodName
一般方法的引用格式是
- 如果是静态方法，则是ClassName::methodName。如 Object ::equals
- 如果是实例方法，则是Instance::methodName。如Object obj=new Object();obj::equals;
- 构造函数.则是ClassName::new
再来看一个完整的例子，方便理解
- import java.awt.FlowLayout;
- import java.awt.event.ActionEvent;
- import javax.swing.JButton;
- import javax.swing.JFrame;
- /**
-  *
-  * @author benhail
-  */
- publicclassTestMethodReference{
- publicstaticvoid main(String[] args){
- JFrame frame =newJFrame();
-         frame.setLayout(newFlowLayout());
-         frame.setVisible(true);
- JButton button1 =newJButton("点我!");
- JButton button2 =newJButton("也点我!");
-         frame.getContentPane().add(button1);
-         frame.getContentPane().add(button2);
- //这里addActionListener方法的参数是ActionListener，是一个函数式接口
- //使用lambda表达式方式
-         button1.addActionListener(e ->{System.out.println("这里是Lambda实现方式");});
- //使用方法引用方式
-         button2.addActionListener(TestMethodReference::doSomething);
- }
- /**
-      * 这里是函数式接口ActionListener的实现方法
-      * @param e
-      */
- publicstaticvoid doSomething(ActionEvent e){
- System.out.println("这里是方法引用实现方式");
- }
- }
可以看出，doSomething方法就是lambda表达式的实现，这样的好处就是，如果你觉得lambda的方法体会很长，影响代码可读性，方法引用就是个解决办法
总结
以上就是lambda表达式语法的全部内容了，相信大家对lambda 表达式都有一定的理解了，但只是代码简洁了这个好处的话，并不能打动很多观众，java 8也不会这么令人期待，其实java 8引入lambda迫切需求是因为lambda 表达式能简化集合上数据的多线程或者多核的处理，提供更快的集合处理速度 ，这个后续会讲到，关于JEP126的这一特性，将分3部分，之所以分开，是因为这一特性可写的东西太多了，这部分让读者熟悉lambda表达式以及方法 引用的语法和概念，第二部分则是虚拟扩展方法（default method）的内容，最后一部分则是大数据集合的处理，解开lambda表达式的最强作用的神秘面纱。敬请期待。。。。
