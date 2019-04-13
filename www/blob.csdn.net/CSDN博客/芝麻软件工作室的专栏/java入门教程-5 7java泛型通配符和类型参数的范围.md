
# java入门教程-5.7java泛型通配符和类型参数的范围 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:33:52[seven-soft](https://me.csdn.net/softn)阅读数：270


本节先讲解如何限制类型参数的范围，再讲解通配符(?)。
## 类型参数的范围
在泛型中，如果不对类型参数加以限制，它就可以接受任意的数据类型，只要它是被定义过的。但是，很多时候我们只需要一部分数据类型就够了，用户传递其他数据类型可能会引起错误。例如，编写一个泛型函数用于返回不同类型数组（Integer 数组、Double
 数组等）中的最大值：public<T>TgetMax(Tarray[]){
Tmax=null;
for(Telement:array){
max=element.doubleValue()>max.doubleValue()?element:max;
}
returnmax;
}

上面的代码会报错，doubleValue() 是 Number 类及其子类的方法，不是所有的类都有该方法，所以我们要限制类型参数 T，让它只能接受 Number 及其子类（Integer、Double、Character 等）。
通过 extends 关键字可以限制泛型的类型的上限，改进上面的代码：public<TextendsNumber>TgetMax(Tarray[]){
Tmax=null;
for(Telement:array){
max=element.doubleValue()>max.doubleValue()?element:max;
}
returnmax;
}

<T extends Number> 表示 T 只接受 Number 及其子类，传入其他类型的数据会报错。这里的限定使用关键字 extends，后面可以是类也可以是接口。如果是类，只能有一个；但是接口可以有多个，并以“&”分隔，例如
 <T extends Interface1 & Interface2>。
这里的 extends 关键字已不再是继承的含义了，应该理解为 T 是继承自 Number 类的类型，或者 T 是实现了 XX 接口的类型。
## 通配符(?)
上一节的例子中提到要定义一个泛型类来表示坐标，坐标可以是整数、小数或字符串，请看下面的代码：classPoint<T1,T2>{
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

现在要求在类的外部定义一个 printPoint() 方法用于输出坐标，怎么办呢？
可以这样来定义方法：publicvoidprintPoint(Pointp){
System.out.println("This point is: "+p.getX()+", "+p.getY());
}

我们知道，如果在使用泛型时没有指名具体的数据类型，就会擦除泛型类型，并向上转型为 Object，这与不使用泛型没什么两样。上面的代码没有指明数据类型，相当于：publicvoidprintPoint(Point<Object, Object>p){
System.out.println("This point is: "+p.getX()+", "+p.getY());
}

为了避免类型擦除，可以使用通配符(?)：publicvoidprintPoint(Point<?, ?>p){
System.out.println("This point is: "+p.getX()+", "+p.getY());
}

通配符(?)可以表示任意的数据类型。将代码补充完整：publicclassDemo{
publicstaticvoidmain(String[]args){
Point<Integer, Integer>p1=newPoint<Integer,Integer>();
p1.setX(10);
p1.setY(20);
printPoint(p1);
Point<String, String>p2=newPoint<String,String>();
p2.setX("东京180度");
p2.setY("北纬210度");
printPoint(p2);
}
publicstaticvoidprintPoint(Point<?, ?>p){// 使用通配符
System.out.println("This point is: "+p.getX()+", "+p.getY());
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
This point is: 10, 20
This point is: 东京180度, 北纬210度
但是，数字坐标与字符串坐标又有区别：数字可以表示x轴或y轴的坐标，字符串可以表示地球经纬度。现在又要求定义两个方法分别处理不同的坐标，一个方法只能接受数字类型的坐标，另一个方法只能接受字符串类型的坐标，怎么办呢？
这个问题的关键是要限制类型参数的范围，请先看下面的代码：publicclassDemo{
publicstaticvoidmain(String[]args){
Point<Integer, Integer>p1=newPoint<Integer,Integer>();
p1.setX(10);
p1.setY(20);
printNumPoint(p1);
Point<String, String>p2=newPoint<String,String>();
p2.setX("东京180度");
p2.setY("北纬210度");
printStrPoint(p2);
}
// 借助通配符限制泛型的范围
publicstaticvoidprintNumPoint(Point<? extends Number, ? extends Number>p){
System.out.println("x: "+p.getX()+", y: "+p.getY());
}
publicstaticvoidprintStrPoint(Point<? extends String, ? extends String>p){
System.out.println("GPS: "+p.getX()+"，"+p.getY());
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
x: 10, y: 20
GPS: 东京180度，北纬210度
? extends Number 表示泛型的类型参数只能是 Number 及其子类，? extends String 也一样，这与定义泛型类或泛型方法时限制类型参数的范围类似。
不过，使用通配符(?)不但可以限制类型的上限，还可以限制下限。限制下限使用 super 关键字，例如 <? super Number> 表示只能接受 Number 及其父类。
注意：一般的项目中很少会去设计泛型，这里主要是让读者学会如何使用，为后面的教程做铺垫。

