
# java基础-泛型详解(1):基本使用 - wangzhiguo9261的博客 - CSDN博客


2018年02月07日 16:38:12[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：1218


前言：无论何时，相信自己。


**相关文章：**
1、[《夯实JAVA基本之一 —— 泛型详解(1):基本使用》](http://blog.csdn.net/harvic880925/article/details/49872903)
2、[《夯实JAVA基本之一——泛型详解(2)：高级进阶》](http://blog.csdn.net/harvic880925/article/details/49883589)
3、[《夯实JAVA基本之二 —— 反射（1）：基本类周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50072739)
4、[《夯实JAVA基本之二 —— 反射（2）：泛型相关周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50085595)
5、[《夯实JAVA基本之二 —— 反射（3）：类内部信息获取》](http://blog.csdn.net/harvic880925/article/details/50107951)

## 一、引入
### 1、泛型是什么
首先告诉大家ArrayList就是泛型。那ArrayList能完成哪些想不到的功能呢？先看看下面这段代码：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
ArrayList<String> strList =newArrayList<String>();
ArrayList<Integer> intList =newArrayList<Integer>();
ArrayList<Double> doubleList =newArrayList<Double>();
大家对ArrayList很熟悉，这里构造了三个List,分别盛装String、Integer和Double；这就是ArrayList的过人之处：即各种类型的变量都可以组装成对应的List，而不必针对每个类型分别实现一个构建ArrayList的类。这里可能看不懂，开篇总是困难的，下面看看如果没有泛型的话，我们要怎么做；
### 2、没有泛型会怎样
先看下面这段代码：
我们实现两个能够设置点坐标的类，分别设置Integer类型的点坐标和Float类型的点坐标：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
//设置Integer类型的点坐标
classIntegerPoint{
privateInteger x ;// 表示X坐标
privateInteger y ;// 表示Y坐标
publicvoidsetX(Integer x){
this.x = x ;
}
publicvoidsetY(Integer y){
this.y = y ;
}
publicInteger getX(){
returnthis.x ;
}
publicInteger getY(){
returnthis.y ;
}
}
//设置Float类型的点坐标
classFloatPoint{
privateFloat x ;// 表示X坐标
privateFloat y ;// 表示Y坐标
publicvoidsetX(Float x){
this.x = x ;
}
publicvoidsetY(Float y){
this.y = y ;
}
publicFloat getX(){
returnthis.x ;
}
publicFloat getY(){
returnthis.y ;
}
}
那现在有个问题：大家有没有发现，他们除了变量类型不一样，一个是Integer一个是Float以外，其它并没有什么区别！那我们能不能合并成一个呢？
答案是可以的，因为Integer和Float都是派生自Object的，我们用下面这段代码代替：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
classObjectPoint{
privateObject x ;
privateObject y ;
publicvoidsetX(Object x){
this.x = x ;
}
publicvoidsetY(Object y){
this.y = y ;
}
publicObject getX(){
returnthis.x ;
}
publicObject getY(){
returnthis.y ;
}
}
即全部都用Object来代替所有的子类；
在使用的时候是这样的：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
ObjectPoint integerPoint =newObjectPoint();
integerPoint.setX(newInteger(100));
Integer integerX=(Integer)integerPoint.getX();
在设置的时候，使用new Integer(100)来新建一个Integer
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
integerPoint.setX(newInteger(100));
然后在取值的时候，进行强制转换：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
Integer integerX=(Integer)integerPoint.getX();
由于我们设置的时候，是设置的Integer，所以在取值的时候，强制转换是不会出错的。
同理，FloatPoint的设置和取值也是类似的，代码如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
ObjectPoint floatPoint =newObjectPoint();
floatPoint.setX(newFloat(100.12f));
Float floatX = (Float)floatPoint.getX();
但问题来了：注意，注意，我们这里使用了强制转换，我们这里setX（）和getX（）写得很近，所以我们明确的知道我们传进去的是Float类型，那如果我们记错了呢？
比如我们改成下面这样，编译时会报错吗：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
ObjectPoint floatPoint =newObjectPoint();
floatPoint.setX(newFloat(100.12f));
String floatX = (String)floatPoint.getX();
不会！！！我们问题的关键在于这句：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
String floatX = (String)floatPoint.getX();
强制转换时，会不会出错。因为编译器也不知道你传进去的是什么，而floatPoint.getX()返回的类型是Object，所以编译时，将Object强转成String是成立的。必然不会报错。
而在运行时，则不然，在运行时，floatPoint实例中明明传进去的是Float类型的变量，非要把它强转成String类型，肯定会报类型转换错误的！
那有没有一种办法在编译阶段，即能合并成同一个，又能在编译时检查出来传进去类型不对呢？当然，这就是泛型。
下面我们将对泛型的写法和用法做一一讲解。
## 二、各种泛型定义及使用
### 1、泛型类定义及使用
我们先看看泛型的类是怎么定义的：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
//定义
classPoint<T>{// 此处可以随便写标识符号
privateT x ;
privateT y ;
publicvoidsetX(T x){//作为参数
this.x = x ;
}
publicvoidsetY(T y){
this.y = y ;
}
publicT getX(){//作为返回值
returnthis.x ;
}
publicT getY(){
returnthis.y ;
}
};
//IntegerPoint使用
Point<Integer> p =newPoint<Integer>() ;
p.setX(newInteger(100)) ;
System.out.println(p.getX());
//FloatPoint使用
Point<Float> p =newPoint<Float>() ;
p.setX(newFloat(100.12f)) ;
System.out.println(p.getX());
先看看运行结果：
![](https://img-blog.csdn.net/20151116222626759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从结果中可以看到，我们实现了开篇中IntegerPoint类和FloatPoint类的效果。下面来看看泛型是怎么定义及使用的吧。
**（1）、定义泛型：Point<T>**
首先，大家可以看到Point<T>，即在类名后面加一个尖括号，括号里是一个大写字母。这里写的是T，其实这个字母可以是任何大写字母，大家这里先记着，可以是任何大写字母，意义是相同的。
**（2）类中使用泛型**
这个T表示派生自Object类的任何类，比如String,Integer,Double等等。这里要注意的是，T一定是派生于Object类的。为方便起见，大家可以在这里把T当成String,即String在类中怎么用，那T在类中就可以怎么用！所以下面的：定义变量，作为返回值，作为参数传入的定义就很容易理解了。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
//定义变量
privateT x ;
//作为返回值
publicT getX(){
returnx ;
}
//作为参数
publicvoidsetX(T x){
this.x = x ;
}
**（3）使用泛型类**
下面是泛型类的用法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
//IntegerPoint使用
Point<Integer> p =newPoint<Integer>() ;
p.setX(newInteger(100)) ;
System.out.println(p.getX());
//FloatPoint使用
Point<Float> p =newPoint<Float>() ;
p.setX(newFloat(100.12f)) ;
System.out.println(p.getX());
首先，是构造一个实例：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
Point<String> p =newPoint<String>() ;
这里与普通构造类实例的不同之点在于，普通类构造函数是这样的：Point p = new Point() ;
而泛型类的构造则需要在类名后添加上<String>，即一对尖括号，中间写上要传入的类型。
因为我们构造时，是这样的：class Point<T>,所以在使用的时候也要在Point后加上类型来定义T代表的意义。
然后在getVar（）和setVar（）时就没有什么特殊的了，直接调用即可。
从上面的使用时，明显可以看出泛型的作用，在构造泛型类的实例的时候：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
//IntegerPoint使用
Point<Integer> p =newPoint<Integer>() ;
//FloatPoint使用
Point<Float> p =newPoint<Float>() ;
尖括号中，你传进去的是什么，T就代表什么类型。这就是泛型的最大作用，我们只需要考虑逻辑实现，就能拿给各种类来用。
前面我们提到ArrayList也是泛型，我们顺便它的实现：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicclassArrayList<E>{
…………
}
看到了吧，跟我们的Point实现是一样的，这也就是为什么ArrayList能够盛装各种类型的主要原因。
**(4）使用泛型实现的优势**
相比我们开篇时使用Object的方式，有两个优点：
（1）、不用强制转换
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
//使用Object作为返回值，要强制转换成指定类型
Float floatX = (Float)floatPoint.getX();
//使用泛型时，不用强制转换，直接出来就是String
System.out.println(p.getVar());
(2)、在settVar()时如果传入类型不对，编译时会报错
![](https://img-blog.csdn.net/20151116222950942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到，当我们构造时使用的是String,而在setVar时，传进去Integer类型时，就会报错。而不是像Object实现方式一样，在运行时才会报强制转换错误。

### 2、多泛型变量定义及字母规范
**（1）、多泛型变量定义**
上在我们只定义了一个泛型变量T，那如果我们需要传进去多个泛型要怎么办呢？
只需要在类似下面这样就可以了：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
classMorePoint<T,U>{
}
也就是在原来的T后面用逗号隔开，写上其它的任意大写字母即可。想加几个就加几个，比如我们想加五个泛型变量，那应该是这样的：**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
classMorePoint<T,U,A,B,C>{
}
举个粟子，我们在Point上再另加一个字段name，也用泛型来表示，那要怎么做？代码如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
classMorePoint<T,U> {
privateT x;
privateT y;
privateU name;
publicvoidsetX(T x) {
this.x = x;
}
publicT getX() {
returnthis.x;
}
…………
publicvoidsetName(U name){
this.name = name;
}
publicU getName() {
returnthis.name;
}
}
//使用
MorePoint<Integer,String> morePoint =newMorePoint<Integer, String>();
morePoint.setName("harvic");
Log.d(TAG,"morPont.getName:"+ morePoint.getName());
从上面的代码中，可以明显看出，就是在新添加的泛型变量U用法与T是一样的。
**（2）、字母规范**
在定义泛型类时，我们已经提到用于指定泛型的变量是一个大写字母：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
classPoint<T>{
…………
}
当然不是的！！！！任意一个大写字母都可以。他们的意义是完全相同的，但为了提高可读性，大家还是用有意义的字母比较好，一般来讲，在不同的情境下使用的字母意义如下：
E — Element，常用在java Collection里，如：List<E>,Iterator<E>,Set<E>
K,V — Key，Value，代表Map的键值对
N — Number，数字
T — Type，类型，如String，Integer等等
如果这些还不够用，那就自己随便取吧，反正26个英文字母呢。
再重复一遍，使用哪个字母是没有特定意义的！只是为了提高可读性！！！！
### 3、泛型接口定义及使用
在接口上定义泛型与在类中定义泛型是一样的，代码如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
interfaceInfo<T>{// 在接口上定义泛型
publicT getVar() ;// 定义抽象方法，抽象方法的返回值就是泛型类型
publicvoidsetVar(T x);
}
与泛型类的定义一样，也是在接口名后加尖括号；
**（1）、使用方法一：非泛型类**
但是在使用的时候，就出现问题了，我们先看看下面这个使用方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
classInfoImplimplementsInfo<String>{// 定义泛型接口的子类
privateString var ;// 定义属性
publicInfoImpl(String var){// 通过构造方法设置属性内容
this.setVar(var) ;
}
@Override
publicvoidsetVar(String var){
this.var = var ;
}
@Override
publicString getVar(){
returnthis.var ;
}
}
publicclassGenericsDemo24{
publicvoidmain(String arsg[]){
InfoImpl i =newInfoImpl("harvic");
System.out.println(i.getVar()) ;
}
};
首先，先看InfoImpl的定义：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
classInfoImplimplementsInfo<String>{
…………
}
要清楚的一点是InfoImpl不是一个泛型类！因为他类名后没有<T>！
然后在在这里我们将Info<String>中的泛型变量T定义填充为了String类型。所以在重写时setVar（）和getVar（）时，IDE会也我们直接生成String类型的重写函数。
最后在使用时,没什么难度，传进去String类型的字符串来构造InfoImpl实例，然后调用它的函数即可。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicclassGenericsDemo24{
publicvoidmain(String arsg[]){
InfoImpl i =newInfoImpl("harvic");
System.out.println(i.getVar()) ;
}
};
**（2）、使用方法二：泛型类**
在方法一中，我们在类中直接把Info<T>接口给填充好了，但我们的类，是可以构造成泛型类的，那我们利用泛型类来构造填充泛型接口会是怎样呢？
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
interfaceInfo<T>{// 在接口上定义泛型
publicT getVar() ;// 定义抽象方法，抽象方法的返回值就是泛型类型
publicvoidsetVar(T var);
}
classInfoImpl<T>implementsInfo<T>{// 定义泛型接口的子类
privateT var ;// 定义属性
publicInfoImpl(T var){// 通过构造方法设置属性内容
this.setVar(var) ;
}
publicvoidsetVar(T var){
this.var = var ;
}
publicT getVar(){
returnthis.var ;
}
}
publicclassGenericsDemo24{
publicstaticvoidmain(String arsg[]){
InfoImpl<String> i =newInfoImpl<String>("harvic");
System.out.println(i.getVar()) ;
}
};
最关键的是构造泛型类的过程：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
classInfoImpl<T>implementsInfo<T>{// 定义泛型接口的子类
privateT var ;// 定义属性
publicInfoImpl(T var){// 通过构造方法设置属性内容
this.setVar(var) ;
}
publicvoidsetVar(T var){
this.var = var ;
}
publicT getVar(){
returnthis.var ;
}
}
在这个类中，我们构造了一个泛型类InfoImpl<T>，然后把泛型变量T传给了Info<T>，这说明接口和泛型类使用的都是同一个泛型变量。
然后在使用时，就是构造一个泛型类的实例的过程，使用过程也不变。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicclassGenericsDemo24{
publicstaticvoidmain(String arsg[]){
Info<String> i =newInfoImpl<String>("harvic");
System.out.println(i.getVar()) ;
}
};
使用泛型类来继承泛型接口的作用就是让用户来定义接口所使用的变量类型，而不是像方法一那样，在类中写死。
那我们稍微加深点难度，构造一个多个泛型变量的类，并继承自Info接口：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
classInfoImpl<T,K,U>implementsInfo<U>{// 定义泛型接口的子类
privateU var ;
privateT x;
privateK y;
publicInfoImpl(U var){// 通过构造方法设置属性内容
this.setVar(var) ;
}
publicvoidsetVar(U var){
this.var = var ;
}
publicU getVar(){
returnthis.var ;
}
}
在这个例子中，我们在泛型类中定义三个泛型变量T,K,U并且把第三个泛型变量U用来填充接口Info。所以在这个例子中Info所使用的类型就是由U来决定的。
使用时是这样的：泛型类的基本用法，不再多讲,代码如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicclassGenericsDemo24{
publicvoidmain(String arsg[]){
InfoImpl<Integer,Double,String> i =newInfoImpl<Integer,Double,String>("harvic");
System.out.println(i.getVar()) ;
}
}

### 4、泛型函数定义及使用
上面我们讲解了类和接口的泛型使用，下面我们再说说，怎么单独在一个函数里使用泛型。比如我们在新建一个普通的类StaticFans，然后在其中定义了两个泛型函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicclassStaticFans {
//静态函数
publicstatic<T>voidStaticMethod(T a){
Log.d("harvic","StaticMethod: "+a.toString());
}
//普通函数
public<T>voidOtherMethod(T a){
Log.d("harvic","OtherMethod: "+a.toString());
}
}
上面分别是静态泛型函数和常规泛型函数的定义方法，与以往方法的唯一不同点就是在返回值前加上<T>来表示泛型变量。其它没什么区别。
使用方法如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
//静态方法
StaticFans.StaticMethod("adfdsa");//使用方法一
StaticFans.<String>StaticMethod("adfdsa");//使用方法二
//常规方法
StaticFans staticFans =newStaticFans();
staticFans.OtherMethod(newInteger(123));//使用方法一
staticFans.<Integer>OtherMethod(newInteger(123));//使用方法二
结果如下：
![](https://img-blog.csdn.net/20151117082843103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
首先，我们看静态泛型函数的使用方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
StaticFans.StaticMethod("adfdsa");//使用方法一
StaticFans.<String>StaticMethod("adfdsa");//使用方法二
从结果中我们可以看到，这两种方法的结果是完全一样的，但他们还有些区别的，区别如下：
方法一，可以像普通方法一样，直接传值，任何值都可以（但必须是派生自Object类的类型，比如String,Integer等），函数会在内部根据传进去的参数来识别当前T的类别。但尽量不要使用这种隐式的传递方式，代码不利于阅读和维护。因为从外观根本看不出来你调用的是一个泛型函数。
方法二，与方法一不同的地方在于，在调用方法前加了一个<String>来指定传给<T>的值，如果加了这个<String>来指定参数的值的话，那StaticMethod（）函数里所有用到的T类型也就是强制指定了是String类型。这是我们建议使用的方式。
同样，常规泛型函数的使用也有这两种方式：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
StaticFans staticFans =newStaticFans();
staticFans.OtherMethod(newInteger(123));//使用方法一
staticFans.<Integer>OtherMethod(newInteger(123));//使用方法二
可以看到，与平常一样，先创建类的实例，然后调用泛型函数。
方法一，隐式传递了T的类型，与上面一样，不建议这么做。
方法二，显示将T赋值为Integer类型，这样OtherMethod（T a）传递过来的参数如果不是Integer那么编译器就会报错。
**进阶：返回值中存在泛型**
上面我们的函数中，返回值都是void，但现实中不可能都是void，有时，我们需要将泛型变量返回，比如下面这个函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicstatic<T> List<T> parseArray(String response,Class<T> object){
List<T> modelList = JSON.parseArray(response, object);
returnmodelList;
}
函数返回值是List<T>类型。至于传入参数Class<T> object的意义，我们下面会讲。这里也就是想通过这个例子来告诉大家，泛型变量其实跟String,Integer，Double等等的类的使用上没有任何区别，T只是一个符号，可以代表String,Integer，Double……这些类的符号，在泛型函数使用时，直接把T看到String,Integer，Double……中的任一个来写代码就可以了。唯一不同的是，要在函数定义的中在返回值前加上<T>标识泛型；
### 5、其它用法:Class<T>类传递及泛型数组
**（1）、使用Class<T>传递泛型类Class对象**
有时，我们会遇到一个情况，比如，我们在使用JSON解析字符串的时候，代码一般是这样的
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicstaticList<SuccessModel> parseArray(String response){
List<SuccessModel> modelList = JSON.parseArray(response, SuccessModel.class);
returnmodelList;
}
其中SuccessModel是自定义的解析类，代码如下，其实大家不用管SuccessModel的定义，只考虑上面的那段代码就行了。写出来SuccessModel的代码，只是不想大家感到迷惑，其实，这里只是fastJson的基本用法而已。
这段代码的意义就是根据SuccessModel解析出List<SuccessModel>的数组。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicclassSuccessModel {
privatebooleansuccess;
publicbooleanisSuccess() {
returnsuccess;
}
publicvoidsetSuccess(booleansuccess) {
this.success = success;
}
}
那现在，我们把下面这句组装成一个泛型函数要怎么来做呢?
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicstaticList<SuccessModel> parseArray(String response){
List<SuccessModel> modelList = JSON.parseArray(response, SuccessModel.class);
returnmodelList;
}
首先，我们应该把SuccessModel单独抽出来做为泛型变量，但parseArray（）中用到的SuccessModel.class要怎么弄呢？
先来看代码：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicstatic<T> List<T> parseArray(String response,Class<T> object){
List<T> modelList = JSON.parseArray(response, object);
returnmodelList;
}
注意到，我们用的Class<T> object来传递类的class对象，即我们上面提到的SuccessModel.class。
这是因为Class<T>也是一泛型，它是传来用来装载类的class对象的，它的定义如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicfinalclassClass<T>implementsSerializable {
…………
}
通过Class<T>来加载泛型的Class对象的问题就讲完了，下面来看看泛型数组的使用方法吧。
**（2）、定义泛型数组**
在写程序时，大家可能会遇到类似String[] list = new String[8];的需求，这里可以定义String数组，当然我们也可以定义泛型数组，泛型数组的定义方法为 T[]，与String[]是一致的，下面看看用法：**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
//定义
publicstatic<T> T[] fun1(T...arg){// 接收可变参数
returnarg ;// 返回泛型数组
}
//使用
publicstaticvoidmain(String args[]){
Integer i[] = fun1(1,2,3,4,5,6) ;
Integer[] result = fun1(i) ;
}
我们先看看 定义时的代码：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49872903#)[copy](http://blog.csdn.net/harvic880925/article/details/49872903#)
publicstatic<T> T[] fun1(T...arg){// 接收可变参数
returnarg ;// 返回泛型数组
}
首先，定义了一个静态函数，然后定义返回值为T[]，参数为接收的T类型的可变长参数。如果有同学对T...arg的用法不了解，可以去找下JAVA 可变长参数方面的知识。
由于可变长参数在输入后，会保存在arg这个数组中，所以，我们直接把数组返回即可。
好了，这篇到这里就结束了，这篇中主要讲述了泛型在各方面的定义及用法，下篇，我们将讲述，有关泛型限定相关的知识。
**如果本文有帮到你，记得加关注哦**
**本文所涉及源码下载地址：****[http://download.csdn.net/detail/harvic880925/9275047](http://download.csdn.net/detail/harvic880925/9275047)**
**请大家尊重原创者版权，转载请标明出处哦：****[http://blog.csdn.net/harvic880925/article/details/49872903](http://blog.csdn.net/harvic880925/article/details/49872903)****谢谢。**



