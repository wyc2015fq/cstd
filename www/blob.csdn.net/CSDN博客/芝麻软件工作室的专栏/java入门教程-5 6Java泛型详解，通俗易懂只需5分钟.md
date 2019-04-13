
# java入门教程-5.6Java泛型详解，通俗易懂只需5分钟 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:28:49[seven-soft](https://me.csdn.net/softn)阅读数：280


我们知道，使用变量之前要定义，定义一个变量时必须要指明它的数据类型，什么样的数据类型赋给什么样的值。
假如我们现在要定义一个类来表示坐标，要求坐标的数据类型可以是整数、小数和字符串，例如：x = 10、y = 10
x = 12.88、y = 129.65
x = "东京180度"、y = "北纬210度"
针对不同的数据类型，除了借助方法重载，还可以借助自动装箱和向上转型。我们知道，基本数据类型可以自动装箱，被转换成对应的包装类；Object 是所有类的祖先类，任何一个类的实例都可以向上转型为 Object 类型，例如：int --> Integer --> Object
double -->Double --> Object
String --> Object
这样，只需要定义一个方法，就可以接收所有类型的数据。请看下面的代码：[复制](http://www.weixueyuan.net/view/6321.html#)[纯文本](http://www.weixueyuan.net/view/6321.html#)[新窗口](http://www.weixueyuan.net/view/6321.html#)
publicclassDemo{
publicstaticvoidmain(String[]args){
Pointp=newPoint();
p.setX(10);// int -> Integer -> Object
p.setY(20);
intx=(Integer)p.getX();// 必须向下转型
inty=(Integer)p.getY();
System.out.println("This point is："+x+", "+y);
p.setX(25.4);// double -> Integer -> Object
p.setY("东京180度");
doublem=(Double)p.getX();// 必须向下转型
doublen=(Double)p.getY();// 运行期间抛出异常
System.out.println("This point is："+m+", "+n);
}
}
classPoint{
Objectx=0;
Objecty=0;
publicObjectgetX(){
returnx;
}
publicvoidsetX(Objectx){
this.x=x;
}
publicObjectgetY(){
returny;
}
publicvoidsetY(Objecty){
this.y=y;
}
}

上面的代码中，生成坐标时不会有任何问题，但是取出坐标时，要向下转型，在[Java多态对象的类型转换](http://www.weixueyuan.net/view/6002.html)一文中我们讲到，向下转型存在着风险，而且编译期间不容易发现，只有在运行期间才会抛出异常，所以要尽量避免使用向下转型。运行上面的代码，第12行会抛出
 java.lang.ClassCastException 异常。
那么，有没有更好的办法，既可以不使用重载（有重复代码），又能把风险降到最低呢？
有，可以使用泛型类(Java
 Class)，它可以接受任意类型的数据。所谓“泛型”，就是“宽泛的数据类型”，任意的数据类型。
更改上面的代码，使用泛型类：publicclassDemo{
publicstaticvoidmain(String[]args){
// 实例化泛型类
Point<Integer, Integer>p1=newPoint<Integer,Integer>();
p1.setX(10);
p1.setY(20);
intx=p1.getX();
inty=p1.getY();
System.out.println("This point is："+x+", "+y);
Point<Double, String>p2=newPoint<Double,String>();
p2.setX(25.4);
p2.setY("东京180度");
doublem=p2.getX();
Stringn=p2.getY();
System.out.println("This point is："+m+", "+n);
}
}
// 定义泛型类
classPoint<T1,T2>{
T1x;
T2y;
publicT1getX(){
returnx;
}
publicvoidsetX(T1x){
this.x=x;
}
publicT2getY(){
returny;
}
publicvoidsetY(T2y){
this.y=y;
}
}

运行结果：
This point is：10, 20
This point is：25.4, 东京180度
与普通类的定义相比，上面的代码在类名后面多出了 <T1, T2>，T1, T2 是自定义的标识符，也是参数，用来传递数据的类型，而不是数据的值，我们称之为类型参数。在泛型中，不但数据的值可以通过参数传递，数据的类型也可以通过参数传递。T1,
 T2 只是数据类型的占位符，运行时会被替换为真正的数据类型。
传值参数（我们通常所说的参数）由小括号包围，如 (int x, double y)，类型参数（泛型参数）由尖括号包围，多个参数由逗号分隔，如 <T> 或 <T, E>。
类型参数需要在类名后面给出。一旦给出了类型参数，就可以在类中使用了。类型参数必须是一个合法的标识符，习惯上使用单个大写字母，通常情况下，K 表示键，V 表示值，E 表示异常或错误，T 表示一般意义上的数据类型。
泛型类在实例化时必须指出具体的类型，也就是向类型参数传值，格式为：
className variable<dataType1, dataType2> = new className<dataType1, dataType2>();
也可以省略等号右边的数据类型，但是会产生警告，即：
className variable<dataType1, dataType2> = new className();
因为在使用泛型类时指明了数据类型，赋给其他类型的值会抛出异常，既不需要向下转型，也没有潜在的风险，比本文一开始介绍的自动装箱和向上转型要更加实用。
注意：泛型是 Java 1.5 的新增特性，它以C++模板为参照，本质是参数化类型(Parameterized Type)的应用。
类型参数只能用来表示引用类型，不能用来表示基本类型，如  int、double、char 等。但是传递基本类型不会报错，因为它们会自动装箱成对应的包装类。
## 泛型方法
除了定义泛型类，还可以定义泛型方法，例如，定义一个打印坐标的泛型方法：publicclassDemo{
publicstaticvoidmain(String[]args){
// 实例化泛型类
Point<Integer, Integer>p1=newPoint<Integer,Integer>();
p1.setX(10);
p1.setY(20);
p1.printPoint(p1.getX(),p1.getY());
Point<Double, String>p2=newPoint<Double,String>();
p2.setX(25.4);
p2.setY("东京180度");
p2.printPoint(p2.getX(),p2.getY());
}
}
// 定义泛型类
classPoint<T1,T2>{
T1x;
T2y;
publicT1getX(){
returnx;
}
publicvoidsetX(T1x){
this.x=x;
}
publicT2getY(){
returny;
}
publicvoidsetY(T2y){
this.y=y;
}
// 定义泛型方法
public<T1,T2>voidprintPoint(T1x,T2y){
T1m=x;
T2n=y;
System.out.println("This point is："+m+", "+n);
}
}

运行结果：
This point is：10, 20
This point is：25.4, 东京180度
上面的代码中定义了一个泛型方法 printPoint()，既有普通参数，也有类型参数，类型参数需要放在修饰符后面、返回值类型前面。一旦定义了类型参数，就可以在参数列表、方法体和返回值类型中使用了。
与使用泛型类不同，使用泛型方法时不必指明参数类型，编译器会根据传递的参数自动查找出具体的类型。泛型方法除了定义不同，调用就像普通方法一样。
注意：泛型方法与泛型类没有必然的联系，泛型方法有自己的类型参数，在普通类中也可以定义泛型方法。泛型方法 printPoint() 中的类型参数 T1, T2 与泛型类 Point 中的 T1, T2 没有必然的联系，也可以使用其他的标识符代替：publicstatic<V1,V2>voidprintPoint(V1x,V2y){
V1m=x;
V2n=y;
System.out.println("This point is："+m+", "+n);
}

## 泛型接口
在Java中也可以定义泛型接口，这里不再赘述，仅仅给出示例代码：publicclassDemo{
publicstaticvoidmain(Stringarsg[]){
Info<String>obj=newInfoImp<String>("www.weixueyuan.net");
System.out.println("Length Of String: "+obj.getVar().length());
}
}
//定义泛型接口
interfaceInfo<T>{
publicTgetVar();
}
//实现接口
classInfoImp<T>implementsInfo<T>{
privateTvar;
// 定义泛型构造方法
publicInfoImp(Tvar){
this.setVar(var);
}
publicvoidsetVar(Tvar){
this.var=var;
}
publicTgetVar(){
returnthis.var;
}
}

运行结果：
Length Of String: 18
## 类型擦除
如果在使用泛型时没有指明数据类型，那么就会擦除泛型类型，请看下面的代码：publicclassDemo{
publicstaticvoidmain(String[]args){
Pointp=newPoint();// 类型擦除
p.setX(10);
p.setY(20.8);
intx=(Integer)p.getX();// 向下转型
doubley=(Double)p.getY();
System.out.println("This point is："+x+", "+y);
}
}
classPoint<T1,T2>{
T1x;
T2y;
publicT1getX(){
returnx;
}
publicvoidsetX(T1x){
this.x=x;
}
publicT2getY(){
returny;
}
publicvoidsetY(T2y){
this.y=y;
}
}

运行结果：
This point is：10, 20.8
因为在使用泛型时没有指明数据类型，为了不出现错误，编译器会将所有数据向上转型为 Object，所以在取出坐标使用时要向下转型，这与本文一开始不使用泛型没什么两样。
## 限制泛型的可用类型
在上面的代码中，类型参数可以接受任意的数据类型，只要它是被定义过的。但是，很多时候我们只需要一部分数据类型就够了，用户传递其他数据类型可能会引起错误。例如，编写一个泛型函数用于返回不同类型数组（Integer 数组、Double
 数组、Character 数组等）中的最大值：public<T>TgetMax(Tarray[]){
Tmax=null;
for(Telement:array){
max=element.doubleValue()>max.doubleValue()?element:max;
}
returnmax;
}

上面的代码会报错，doubleValue() 是 Number 类的方法，不是所有的类都有该方法，所以我们要限制类型参数 T，让它只能接受 Number 及其子类（Integer、Double、Character 等）。
通过 extends 关键字可以限制泛型的类型，改进上面的代码：public<TextendsNumber>TgetMax(Tarray[]){
Tmax=null;
for(Telement:array){
max=element.doubleValue()>max.doubleValue()?element:max;
}
returnmax;
}

<T extends Number> 表示 T 只接受 Number 及其子类，传入其他类型的数据会报错。这里的限定使用关键字 extends，后面可以是类也可以是接口。但这里的 extends 已经不是继承的含义了，应该理解为
 T 是继承自 Number 类的类型，或者 T 是实现了 XX 接口的类型。
注意：一般的应用开发中泛型使用较少，多用在框架或者库的设计中，这里不再深入讲解，主要让大家对泛型有所认识，为后面的教程做铺垫。

