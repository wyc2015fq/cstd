
# java基础-泛型详解(2)：高级进阶 - wangzhiguo9261的博客 - CSDN博客


2018年02月07日 16:38:03[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：1450


前言：被温水煮惯了，梦想的东西总是不敢于尝试，失败了又怎样，最多从头来过。

**相关文章：**
1、[《夯实JAVA基本之一 —— 泛型详解(1):基本使用》](http://blog.csdn.net/harvic880925/article/details/49872903)
2、[《夯实JAVA基本之一——泛型详解(2)：高级进阶》](http://blog.csdn.net/harvic880925/article/details/49883589)
3、[《夯实JAVA基本之二 —— 反射（1）：基本类周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50072739)
4、[《夯实JAVA基本之二 —— 反射（2）：泛型相关周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50085595)
5、[《夯实JAVA基本之二 —— 反射（3）：类内部信息获取》](http://blog.csdn.net/harvic880925/article/details/50107951)

上一篇给大家初步讲解了泛型变量的各种应用环境，这篇将更深入的讲解一下有关类型绑定，通配符方面的知识。

## 一、类型绑定
### 1、引入
我们重新看上篇写的一个泛型：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
classPoint<T> {
privateT x;// 表示X坐标
privateT y;// 表示Y坐标
publicvoidsetX(T x) {
this.x = x;
}
publicvoidsetY(T y) {
this.y = y;
}
publicT getX() {
returnthis.x;
}
publicT getY() {
returnthis.y;
}
}
//使用
Point<Integer> p1 =newPoint<Integer>();
p1.setX(newInteger(100));
System.out.println(p1.getX());
首先，我们要知道一点，任何的泛型变量（比如这里的T）都是派生自Object，所以我们在填充泛型变量时，只能使用派生自Object的类，比如String,Integer,Double，等而不能使用原始的变量类型，比如int,double,float等。
然后，问题来了，那在泛型类Point<T>内部，利用泛型定义的变量T x能调用哪些函数呢？
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
privateT x;
当然只能调用Object所具有的函数，因为编译器根本不知道T具体是什么类型，只有在运行时，用户给什么类型，他才知道是什么类型。编译器唯一能确定的是，无论什么类型，都是派生自Object的，所以T肯定是Object的子类，所以T是可以调用Object的方法的。
那么问题又来了，如果我想写一个找到最小值的泛型类；由于不知道用户会传什么类型，所以要写一个接口,让用户实现这个接口来自已对比他所传递的类型的大小。
接口如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
publicinterfaceComparable<T>{
publicbooleancompareTo(T i);
}
但如果我们直接利用T的实例来调用compareTo（）函数的话，会报错，编译器截图如下：
![](https://img-blog.csdn.net/20151117095734997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这是因为，编译器根本无法得知T是继承自Comparable接口的函数。那怎么样才能让编译器知道，T是继承了Comparable接口的类型呢？
这就是类型绑定的作用了。

### 2、类型绑定：extends
**（1）、定义**
有时候，你会希望泛型类型只能是某一部分类型，比如操作数据的时候，你会希望是Number或其子类类型。这个想法其实就是给泛型参数添加一个界限。其定义形式为：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
<TextendsBoundingType>
此定义表示T应该是BoundingType的子类型（subtype）。T和BoundingType可以是类，也可以是接口。另外注意的是，此处的”extends“表示的子类型，不等同于继承。
一定要非常注意的是，这里的extends不是类继承里的那个extends！两个根本没有任何关联。在这里extends后的BoundingType可以是类，也可以是接口，意思是说，T是在BoundingType基础上创建的，具有BoundingType的功能。目测是JAVA的开发人员不想再引入一个关键字，所以用已有的extends来代替而已。
**（2）、实例：绑定接口**
同样，我们还使用上面对比大小的接口来做例子
首先，看加上extends限定后的min函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
publicinterfaceComparable<T> {
publicbooleancompareTo(T i);
}
//添加上extends Comparable之后，就可以Comparable里的函数了
publicstatic<TextendsComparable>  T min(T...a){
T smallest = a[0];
for(T item:a){
if(smallest.compareTo(item)){
smallest = item;
}
}
returnsmallest;
}
这段代码的意思就是根据传进去的T类型数组a，然后调用其中item的compareTo()函数，跟每一项做对比，最终找到最小值。
从这段代码也可以看出，类型绑定有两个作用：1、对填充的泛型加以限定 2、使用泛型变量T时，可以使用BoundingType内部的函数。
这里有一点非常要注意的是，在这句中smallest.compareTo(item)，smallest和item全部都是T类型的，也就是说，compareTo对比的是同一种类型。
然后我们实现一个派生自Comparable接口的类：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
publicclassStringCompareimplementsComparable<StringCompare> {
privateString mStr;
publicStringCompare(String string){
this.mStr = string;
}
@Override
publicbooleancompareTo(StringCompare str) {
if(mStr.length() > str.mStr.length()){
returntrue;
}
returnfalse;
}
}
在这段代码，大家可能会疑惑为什么把T也填充为StringCompare类型，记得我们上面说的吗：smallest.compareTo(item)，smallest和item是同一类型！！所以compareTo的参数必须是与调用者自身是同一类型，所以要把T填充为StringCompare；
在这段代码中compareTo的实现为，对比当前mstr的长度与传进来实例的mstr长度进行比较，如果超过，则返回true,否则返回false;
最后是使用min函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
StringCompare result = min(newStringCompare("123"),newStringCompare("234"),newStringCompare("59897"));
Log.d(TAG,"min:"+result.mStr);
结果如下：
![](https://img-blog.csdn.net/20151117100009027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里有extends接口，我们开篇说过，extends表示绑定，后面的BindingType即可以是接口，也可以是类，下面我们就再举个绑定类的例子。
**源码在文章底部给出**
**（3）、实例：绑定类**
我们假设，我们有很多种类的水果，需要写一个函数，打印出填充进去水果的名字：
为此，我们先建一个基类来设置和提取名字：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
classFruit {
privateString name;
publicString getName() {
returnname;
}
publicvoidsetName(String name) {
this.name = name;
}
}
然后写个泛型函数来提取名字：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
publicstatic<TextendsFruit> String getFruitName(T t){
returnt.getName();
}
这里泛型函数的用法就出来了，由于我们已知水果都会继承Fruit基类，所以我们利用<T extends Fruit>就可以限定填充的变量必须派生自Fruit的子类。一来，在T中，我们就可以利用Fruit类中方法和函数；二来，如果用户填充进去的类没有派生自Fruit，那编译器就会报错。
然后，我们新建两个类，派生自Fruit，并填充进去它们自己的名字:
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
classBananaextendsFruit{
publicBanana(){
setName("bababa");
}
}
classAppleextendsFruit{
publicApple(){
setName("apple");
}
}
最后调用：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
String name_1 = getFruitName(newBanana());
String name_2 = getFruitName(newApple());
Log.d(TAG,name_1);
Log.d(TAG,name_2);
结果如下：
![](https://img-blog.csdn.net/20151117100238378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**源码在文章底部给出**
**（4）、绑定多个限定**
上面我们讲了，有关绑定限定的用法，其实我们可以同时绑定多个绑定,用&连接，比如：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
publicstatic<TextendsFruit&Serializable> String getFruitName(T t){
returnt.getName();
}
再加深下难度，如果我们有多个泛型，每个泛型都带绑定，那应该是什么样子的呢：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
publicstatic<TextendsComparable & Serializable, UextendsRunnable> T foo(T a, U b){
…………
}
大家应该看得懂，稍微讲一下：这里有两个泛型变量T和U,将T与Comparable & Serializable绑定，将U与Runnable绑定。
好了，这部分就讲完了，下面讲讲有关通配符的用法。
## 二、通配符
通配符是一个非常令人头疼的一个功能，理解与掌握难度比较大，下面我尽力去讲明白它与泛型变量的区别与用法。
### 1、引入
重新来看我们上篇用的Point泛型定义：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
classPoint<T> {
privateT x;
privateT y;
publicPoint(){
}
publicPoint(T x,T y){
this.x = x;
this.y = y;
}
publicvoidsetX(T x) {
this.x = x;
}
publicvoidsetY(T y) {
this.y = y;
}
publicT getX() {
returnthis.x;
}
publicT getY() {
returnthis.y;
}
}
这段代码很简单，引入了一个泛型变量T，然后是有两个构造函数，最后分别是利用set和get方法来设置和获取x,y的值。这段代码没什么难度，不再细讲。
我们看看下面这段使用的代码：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
Point<Integer> integerPoint =newPoint<Integer>(3,3);
…………
Point<Float> floatPoint =newPoint<Float>(4.3f,4.3f);
…………
Point<Double> doublePoint =newPoint<Double>(4.3d,4.90d);
…………
Point<Long> longPoint =newPoint<Long>(12l,23l);
…………
在这段代码中，我们使用Point<T>生成了四个实例:integerPoint,floatPoint,doublePoint和longPoint；
在这里，我们生成四个实例，就得想四个名字。如果我们想生成十个不同类型的实例呢？那不得想十个名字。
光想名字就是个事，（其实我并不觉得想名字是个什么大事…… T _ T ，没办法，想不出更好的例子了…… ）
那有没有一种办法，生成一个变量，可以将不同类型的实例赋值给他呢？
### 2、无边界通配符：？
**（1）、概述**
先不讲无边界通配符是什么，同样拿上面的例子来看，如果我们这样实现：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
Point<?> point;
point =newPoint<Integer>(3,3);
point =newPoint<Float>(4.3f,4.3f);
point =newPoint<Double>(4.3d,4.90d);
point =newPoint<Long>(12l,23l);
在这里，我们首先，利用下面的代码生成一个point实例，注意到，在填充泛型时，用的是?
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
Point<?> point;
然后，各种类型的Point实例，都可以赋值给point了：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
point =newPoint<Integer>(3,3);
point =newPoint<Float>(4.3f,4.3f);
point =newPoint<Double>(4.3d,4.90d);
point =newPoint<Long>(12l,23l);
这里的?就是无边界通配符。通配符的意义就是它是一个未知的符号，可以是代表任意的类。
所以这里可能大家就明白了，这里不光能将泛型变量T填充为数值类型，其实任意Point实例都是可以传给point的：比如这里的Point<String>(),Point<Object>()都是可以的
![](https://img-blog.csdn.net/20151117100613126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**（2）、？与T的区别**
大家可能会有疑问，那无边界通配符？与泛型变量T有什么区别呢？
答案是：他们俩没有任何联系！！！！！
泛型变量T不能在代码用于创建变量，只能在类，接口，函数中声明以后，才能使用。
比如：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
publicclassBox<T> {
publicT get(){
…………
};
publicvoidput(T element){
…………
};
}
而无边界通配符？则只能用于填充泛型变量T，表示通配任何类型！！！！再重复一遍：？只能用于填充泛型变量T。它是用来填充T的！！！！只是填充方式的一种！！！
比如：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
//无边界通配符填充
Box<?> box;
//其它类型填充
Box<String> stringBox;
**（3）、通配符只能用于填充泛型变量T,不能用于定义变量**
大家一定要记得，通配符的使用位置只有：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
Box<?> box;
box =newBox<String>();
即填充泛型变量T的位置，不能出现在后面String的位置！！！！
下面的第三行，第四行，都是错误的。通配符不能用于定义变量。
![](https://img-blog.csdn.net/20151117100757714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再次强调，？只能出现在Box<?> box;中，其它位置都是不对的。

### 3、通配符？的extends绑定
**（1）、概述**
从上面我们可以知道通配符？可以代表任意类型，但跟泛型一样，如果不加以限定，在后期的使用中编译器可能不会报错。所以我们同样，要对？加以限定。
绑定的形式，同样是通过extends关键字，意义和使用方法都用泛型变量一致。
同样，以我们上面的Point<T>泛型类为例，因为Point在实例意义中，其中的值是数值才有意义，所以将泛型变量T填充为Object类型、String类型等都是不正确的。
所以我们要对Point<?> point加以限定：只有数值类型才能赋值给point；
我们把代码改成下面的方式：
![](https://img-blog.csdn.net/20151117100849151?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们给通配符加上限定： Point<? extends Number> point;
此时，最后两行，当将T填充为String和Object时，赋值给point就会报错！
这里虽然是指派生自Number的任意类型，但大家注意到了没： new Point<Number>();也是可以成功赋值的，这说明包括边界自身。
再重复一遍：无边界通配符只是泛型T的填充方式，给他加上限定，只是限定了赋值给它（比如这里的point）的实例类型。
如果想从根本上解决乱填充Point的问题，需要从Point泛型类定义时加上<T extends Number>:
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
classPoint<TextendsNumber> {
privateT x;// 表示X坐标
privateT y;// 表示Y坐标
…………
}
**（2）注意：利用<? extends Number>定义的变量，只可取其中的值，不可修改**
看下面的代码：
![](https://img-blog.csdn.net/20151117100946908?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
明显在point.setX(Integer(122));时报编译错误。但point.getX()却不报错。
这是为什么呢？
首先，point的类型是由Point<? extends Number>决定的，并不会因为point = new Point<Integer>(3,3);而改变类型。
即便point = new Point<Integer>(3,3);之后，point的类型依然是Point<? extends Number>，即派生自Number类的未知类型！！！这一点很好理解，如果在point = new Point<Integer>(3,3);之后，point就变成了Point<Integer>类型，那后面point = new Point<Long>(12l,23l);操作时，肯定会因为类型不匹配而报编译错误了，正因为，point的类型始终是Point<? extends Number>，因此能继续被各种类型实例赋值。
回到正题，现在说说为什么不能赋值
正因为point的类型为 Point<? extends Number> point，那也就是说，填充Point的泛型变量T的为<? extends Number>，这是一个什么类型？未知类型！！！怎么可能能用一个未知类型来设置内部值！这完全是不合理的。
但取值时，正由于泛型变量T被填充为<? extends Number>，所以编译器能确定的是T肯定是Number的子类，编译器就会用Number来填充T
也就是说，编译器，只要能确定通配符类型，就会允许，如果无法确定通配符的类型，就会报错。

### 4、通配符？的super绑定
**(1)、概述**
如果说 <? extends XXX>指填充为派生于XXX的任意子类的话，那么<? super XXX>则表示填充为任意XXX的父类！
我们先写三个类，Employee,Manager,CEO,分别代表工人，管理者，CEO
其中Manager派生于Employee,CEO派生于Manager,代码如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
classCEOextendsManager {
}
classManagerextendsEmployee {
}
classEmployee {
}
然后，如果我这样生成一个变量：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
List<?superManager> list;
它表示的意思是将泛型T填充为<? super Manager>，即任意Manager的父类；也就是说任意将List<T>中的泛型变量T填充为Manager父类的List变量，都可以赋值给list;
![](https://img-blog.csdn.net/20151117101352735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从上面的代码中可以看出new ArrayList<Employee>()，new ArrayList<Manager>()都是正确的，而new ArrayList<CEO>()却报错，当然是因为CEO类已经不再是Manager的父类了。所以会报编译错误。
这里还要注意一个地方，从代码中可以看出new ArrayList<Manager>()是可以成功赋值给 List<? super Manager> list的，可见，super关键字也是包括边界的。即边界类型（这里是Manager）组装的实例依然可以成功赋值。
**（2）、super通配符实例内容：能存不能取**
上面我们讲了，extends通配符，能取不能存，那super通配符情况又怎样呢？我们试试看：
![](https://img-blog.csdn.net/20151117101450199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
先看存的部分：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
List<?superManager> list;
list =newArrayList<Employee>();
//存
list.add(newEmployee());//编译错误
list.add(newManager());
list.add(newCEO());
首先，需要声明的是，与Point<? extends Number> point中point的类型是由Point<? extends Number>确定的，相同的是list的类型是也是由List<? super Manager> ；list的item的类型始终是<? super Manager>，即Manager类的任意父类，即可能是Employee或者Object.
大家可能疑惑的地方在于，为什么下面这两个是正确的！而list.add(new Employee()); 却是错误的！
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
list.add(newManager());
list.add(newCEO());
因为list里item的类型是<? super Manager>,即Manager的任意父类，我们假如是Employee，那下面这段代码大家能理解了吧：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
List<Employee> list =newArrayList<Employee>();
list.add(newManager());
list.add(newCEO());
在这里，正因为Manager和CEO都是Employee的子类，在传进去list.add()后，会被强制转换为Employee！
现在回过头来看这个：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
List<?superManager> list;
list =newArrayList<Employee>();
//存
list.add(newEmployee());//编译错误
list.add(newManager());
list.add(newCEO());
编译器无法确定<? super Manager>的具体类型，但唯一可以确定的是Manager()、CEO()肯定是<? super Manager>的子类，所以肯定是可以add进去的。但Employee不一定是<? super Manager>的子类，所以不能确定，不能确定的，肯定是不允许的，所以会报编译错误。
最后再来看看取：
![](https://img-blog.csdn.net/20151117102125881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在这段代码中，Object object = list.get(0);是不报错的，而Employee employee = list.get(0);是报错的；
我们知道list中item的类型为<? super Manager>，那编译器能肯定的是<? super Manager>肯定是Manger的父类；但不能确定，它是Object还是Employee类型。但无论是填充为Object还是Employee，它必然是Object的子类！
所以Object object = list.get(0);是不报错的。因为 list.get(0);肯定是Object的子类；
而编译器无法判断list.get(0)是不是Employee类型的，所以Employee employee = list.get(0);是报错的。
这里虽然看起来是能取的，但取出来一个Object类型，是毫无意义的。所以我们认为super通配符：能存不能取；

### 5、通配符？总结
总结 ? extends 和 the ? super 通配符的特征，我们可以得出以下结论：
◆ 如果你想从一个数据类型里获取数据，使用 ? extends 通配符（能取不能存）
◆ 如果你想把对象写入一个数据结构里，使用 ? super 通配符（能存不能取）
◆ 如果你既想存，又想取，那就别用通配符。

### 6、常见问题注意
**（1）、Point与Point<T>构造泛型实例的区别**
同样以Point泛型类为例：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
classPoint<T> {
privateT x;// 表示X坐标
privateT y;// 表示Y坐标
publicPoint(){
}
publicPoint(T x,T y){
this.x = x;
this.y = y;
}
publicvoidsetX(T x) {
this.x = x;
}
publicvoidsetY(T y) {
this.y = y;
}
publicT getX() {
returnthis.x;
}
publicT getY() {
returnthis.y;
}
}
我们来看看下面这种构造Point泛型实例有什么区别：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
//使用Point<?>
Point<?> point1 =newPoint(newInteger(23),newInteger(23));
Point<?> point2 =newPoint(newString(""),newString(""));
//直接使用Point
Point point3 =newPoint(newInteger(23),newInteger(23));
Point point4 =newPoint(newString(""),newString(""));
上面的四行代码中，point1,point2生成的是Point<?>的实例，填充的是无边界通配符。而point3和point4则非常奇怪，没有了泛型的<>标识，直接使用Point生成的实例，那它填充的是什么呢？
这四行代码在编译和运行时，都没有报错，而且输出结果也一样！
那么问题就来了:
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
Point<?> point1 =newPoint(newInteger(23),newInteger(23));
Point<?> point2 =newPoint(newString(""),newString(""));
在上面的代码中，使用了无界通配符，所以能够将各种Point实例赋值给Point<?> point1
而省略了泛型标识的构造方法，依然能将各种Point实例赋值给它：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
Point point3 =newPoint(newInteger(23),newInteger(23));
Point point4 =newPoint(newString(""),newString(""));
这说明：构造泛型实例时，如果省略了填充类型，则默认填充为无边界通配符！
所以下面这两个是对等的：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/49883589#)[copy](http://blog.csdn.net/harvic880925/article/details/49883589#)
Point point3 =newPoint(newInteger(23),newInteger(23));
Point<?> point3 =newPoint(newInteger(23),newInteger(23));
最后重复一遍：构造泛型实例时，如果省略了填充类型，则默认填充为无边界通配符！

好了，快累死了，这部分真是太难讲了，有关通配符捕获和编译器类型擦除的知识，就不讲了，在实际项目中基本用不到，有兴趣的同学可以自行去补充下。
下篇给大家讲下反射。

如果本文有帮到你，记得加关注哦
**本文涉及源码下载地址：****[http://download.csdn.net/detail/harvic880925/9275551](http://download.csdn.net/detail/harvic880925/9275551)**
**请大家尊重原创者版权，转载请标明出处：****[http://blog.csdn.net/harvic880925/article/details/49883589](http://blog.csdn.net/harvic880925/article/details/49883589)****谢谢**

**参考文章：**
[1、《 java 泛型编程（一）》](http://peiquan.blog.51cto.com/7518552/1302898)
[2、《Java泛型--泛型应用--泛型接口、泛型方法、泛型数组、泛型嵌套》](http://blog.csdn.net/waldmer/article/details/12773021)
[3、《Java泛型编程最全总结》](http://qiemengdao.iteye.com/blog/1525624)
[4、《java 通配符解惑》](http://peiquan.blog.51cto.com/7518552/1303768)
[5、《《Java编程思想》学习笔记8——泛型编程高级》](http://blog.csdn.net/chjttony/article/details/6801406)
[6、《步步理解 JAVA 泛型编程(三)》](http://unmi.cc/understand-java-generic-3/)
[7、《Java-泛型编程-类型擦除(Type Erasure)》](http://blog.csdn.net/fw0124/article/details/42295463)
[8、《Java泛型--泛型入门》](http://blog.csdn.net/waldmer/article/details/12704963)
[9、《Java泛型--通配符》](http://blog.csdn.net/waldmer/article/details/12745095)
[10、《在 Java 的泛型类型中使用通配符》](http://blog.csdn.net/hikvision_java_gyh/article/details/8986487)
[11、《Java 理论与实践: 使用通配符简化泛型使用》](http://www.ibm.com/developerworks/cn/java/j-jtp04298.html)
[12、《Java 泛型学习三 通配符》](http://www.oseye.net/user/kevin/blog/165)
[13、《Java泛型-- 通配符（转载）》](http://langgufu.iteye.com/blog/2190212)



