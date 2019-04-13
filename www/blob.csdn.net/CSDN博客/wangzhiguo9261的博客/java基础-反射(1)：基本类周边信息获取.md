
# java基础-反射(1)：基本类周边信息获取 - wangzhiguo9261的博客 - CSDN博客


2018年02月07日 16:37:44[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：1640


前言：坚持梦想，过程或是艰辛的，回忆是幸福的。与其最后豪言如果当时我怎样怎样，倒不如坚持脚下。

**相关文章：**
1、[《夯实JAVA基本之一 —— 泛型详解(1):基本使用》](http://blog.csdn.net/harvic880925/article/details/49872903)
2、[《夯实JAVA基本之一——泛型详解(2)：高级进阶》](http://blog.csdn.net/harvic880925/article/details/49883589)
3、[《夯实JAVA基本之二 —— 反射（1）：基本类周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50072739)
4、[《夯实JAVA基本之二 —— 反射（2）：泛型相关周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50085595)
5、[《夯实JAVA基本之二 —— 反射（3）：类内部信息获取》](http://blog.csdn.net/harvic880925/article/details/50107951)

今天开始给大家讲讲有关反射的知识，在应用程序开发时，如果纯做上层，搭搭框架啥的，那用到反射的机会不多，但如果你想做出来一个公共类或者公共模块给其它人用的时候，那用到反射的可能性就大大增加了。况且反射听起来也是蛮屌的名字，今天就我们彻底认识他下吧。

## 一、引入
在开始反射之前，我们先看看JVM是如何将我们写的类对应的java文件加载到内存中的。
### 1、类的生命周期
这部分我们先讲讲JVM的加载机制（可能会有点难度，我尽力讲的直白点）
我们写一个最简单的Main函数，来看看这个函数的是如何被执行的，代码如下：(Main.java)
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicclassMain {
publicstaticvoidmain(String[] args)  {
Animal animal =newAnimal();
animal.setName("cat");
}
publicstaticclassAnimal{
privateString name;
publicString getName() {
returnname;
}
publicvoidsetName(String name) {
this.name = name;
}
}
}
这段代码很简单，我们定义了一个Animal的类，在main()函数中，我们首先定义了一个Animal实例，然后调用了该实例的setName()方法。
大家都知道，在拿到一个java源文件后，如果要经过源码编译，要经过两个阶段：
**编译：**
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
javac Main.java
在执行后后在同一目录下生成Main.class和Animal类对应的文件Main$Animal.class(由于我们的Animal类是Main中的内部类，所以用$表示Main类中的内部类)
**运行**
然后使用java Main命令运行程序：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
java Main
在这一阶段，又分为三个小阶段：装载，链接，初始化
装载：类的装载是通过类加载器完成的，加载器将.class文件的二进制文件装入JVM的方法区，并且在堆区创建描述这个类的java.lang.Class对象。用来封装数据。 但是同一个类只会被类装载器装载一次，记住：只装载一次！
链接：链接就是把二进制数据组装为可以运行的状态。链接分为校验，准备，解析这3个阶段。校验一般用来确认此二进制文件是否适合当前的JVM（版本），准备就是为静态成员分配内存空间，并设置默认值。解析指的是转换常量池中的代码作为直接引用的过程，直到所有的符号引用都可以被运行程序使用（建立完整的对应关系）。
初始化：初始化就是对类中的变量进行初始化值；完成之后，类型也就完成了初始化，初始化之后类的对象就可以正常使用了，直到一个对象不再使用之后，将被垃圾回收。释放空间。
当没有任何引用指向Class对象时就会被卸载，结束类的生命周期。如果再次用到就再重新开始装载、链接和初始化的过程。
上面这一大段有关类生命周期有讲解，可能会有些难度，毕竟有关JVM的东东不是三言两语能讲透彻的，通过上面的这一段只想告诉大家一点：类只会被装载一次！！！！利用装载的类可以实例化出各种不同的对象！
### 2、获取类类型
#### 1、泛型隐藏填充类型默认填充为无界通配符？
在上面，我们讲了，类只会被装载一次，利用装载的类可以实例化出各种不同的对象。而反射就是通过获取装载的类来做出各种操作的。装载的类，我们称为类类型，利用装载的类产生的实例，我们称为类实例。下面我们就看看，如何利用代码获取类类型的：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//使用方法一
Class class1 = Animal.class;
System.out.println(class1.getName());
//使用方法二
Class<?> class2= Animal.class;
System.out.println(class2.getName());
运行结果如下：
![](https://img-blog.csdn.net/20151127171912215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从结果中可以看出class1和class2是完全一样的，那构造他们时的方法一和方法二有什么区别呢？
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//使用方法一
Class class1 = Animal.class;
//使用方法二
Class<?> class2= Animal.class;
可以看到这两个方法，右边全部都是Animal.class，而左边却有些不同。
方法一中，是直接生成了一个Class的实例。
而在方法二中，则生成的是一个Class的泛型，并且使用的是无界通配符来填充的。有关无界通配符的事，下面再说，这里先讲讲方法一中直接生成Class对象与方法二中生成的Class泛型的区别。
我们都知道，Class类是一个泛型。而泛型的正规写法就应该是
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<Animal> class2= Animal.class;
而方法一，只是把泛型的填充为省略了，在泛型中，如果把泛型的填充给省略掉，那就会默认填充为无界通配符？。所以方法一的真实写法是这样的：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<?> class1 = Animal.class;
所以这两种写法是意义是完全相同的。
如果我们不用通配符，也就只能这样写：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<Animal> class2= Animal.class;
有关泛型和通配符的用法请参看：[《夯实JAVA基本之一 —— 泛型详解(1)：基本使用》](http://blog.csdn.net/harvic880925/article/details/49872903)（上面这部分，在泛型详解中也讲过）
在本文中，为了不误导大家，我们采用完整的Class填充方式即Class<?>
#### 2、获取类类型的方法
上面我们通过Class<?> class1 = Animal.class，即直接使用类名的Class对象可以获取类类型，这只是其中一个方法，下面这四种方法都可以获得对应的类类型：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//方法一：
Person person =newPerson();
Class a = person.getClass()
//方法二：
Class b = Persion.class;
//方法三：
Class c = Class.forName(String ClassName);
//方法四：（不建议使用）
Class d = context.getClassLoader().loadClass(String ClassName);
方法一中是通过类实例的getClass()方法得到类类型。
方法二中，直接通过类的class对象得到
方法三和方法四中是通过类名得到，这两点要非常注意，这里的ClassName一定要从包名具体到类名，唯一定位到一个类才行，不然就会报ClassNotFound错误
在上面我们提到过，类只会被加载一次，所以a,b,c,d都是相等的，因为他们都是指向同一个对象，如果用等号操作符来判断的话：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
booleanresult = (clazz1 == clazz2 && clazz3 == clazz4 && clazz1 == clazz3);
result的值为true;
下面我们针对方法三和方法四举个粟子来看下：
先看一下完整的代码结构：
![](https://img-blog.csdn.net/20151127172408006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到我们有一个Activity：MyActivity,和一个类Animal;
我们在Activity上放一个btn，把所有代码放在btn的点击响应中，Activity的布局难度不大就不再贴出代码，下面仅针对类类型的代码讲解。
Animal类的定义与上方一样,唯一不同的是，Animal类已经不再是内部类了而是单独出来的一个类。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicclassAnimal {
privateString name;
publicString getName() {
returnname;
}
publicvoidsetName(String name) {
this.name = name;
}
}
然后在Activity上btn点击时：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//btn点击时调用demoFunc()函数
Button button = (Button)findViewById(R.id.btn);
button.setOnClickListener(newView.OnClickListener() {
@Override
publicvoidonClick(View v) {
try{
demoFunc();
}catch(Exception e){
System.out.print(e.getMessage());
}
}
});
demoFunc()函数代码如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicvoiddemoFunc()throwsException{
Class<?> class1 = Class.forName("com.example.myReflect.Animal");
Log.d(TAG,"通过Class.forName获得的类名："+class1.getName());
class1 = getClassLoader().loadClass("com.example.myReflect.Animal");
Log.d(TAG,"通过ClassLoader获得的类名："+class1.getName());
}
其中
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
privatestaticString TAG ="qijian";
结果如下：
![](https://img-blog.csdn.net/20151127172636939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从上面的用法中，可以看出，我们要使用Class.forName（）或者getClassLoader().loadClass（），其中的类名必须是从包名到类名的完整路径！
从这里看来Class.forName（）和getClassLoader().loadClass（）是相同的，其实他们是有区别的，平时，我们不建议使用getClassLoader().loadClass（）的方法来加载类类型。有关Class.forName（）和getClassLoader().loadClass（）的具体区别，会在本篇末尾讲述。

## 二、基本类类型周边信息获取
我们知道类分为基本类和泛型类，这篇我们只讲基本类类型的周边信息获取，有关泛型类的周边信息获取，我们会放到下一篇中。
这部分主要讲述类类型周边信息获取方法，包括类名，包名，超类和继承接口。
### 1、类名，包名获取
相关的有三个函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//获取完整的类名（包含包名）
publicString getName();
//仅获取类名
publicString getSimpleName();
//获取类类型所对应的package对象
publicPackage getPackage()
上面都有解释，我们用一下这几个函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<?> class1 = Animal.class;
Package package1 = class1.getPackage();
Log.d(TAG,"完整的类名："+class1.getName());
Log.d(TAG,"仅获取类名："+class1.getSimpleName());
Log.d(TAG,"包名："+package1.getName());
![](https://img-blog.csdn.net/20151127200115908?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从结果中很清晰的看到，class.getName()获取的是类名包含完整路径。调用Class.forName()就是用的这个值。class.getSimpleName()得到的是仅仅是一个类名。而class.getPackage()得到的是该类对应的Package对象。通过package.getName()能获得该类所对应的包名。
有关Package类的相关函数就不讲了，基本用不到。
### （2）、获取超类Class对象
获取superClass的类对象，涉及到两个函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//获取普通函数的父类Class对象
publicClass<?> getSuperclass();
//针对泛型父类而设计
publicType getGenericSuperclass();
getSuperclass()用来获取普通函数，而getGenericSuperclass()是用来获取泛型类型的父类而设计的，有关getGenericSuperclass()的知识我们后面会讲，这里先看看getSuperclass()的用法。
我们仍然利用前面讲到的Animal类，然后在其上派生一个AnimalImpl子类：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicclassAnimal {
privateString name;
publicString getName() {
returnname;
}
publicvoidsetName(String name) {
this.name = name;
}
}
publicclassAnimalImplextendsAnimal {
}
然后使用：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<?> class2 = Class.forName("com.example.myReflect.AnimalImpl");
Class<?> parentClass = class2.getSuperclass();
Log.d(TAG,"父类："+ parentClass.getName());
结果如下：
![](https://img-blog.csdn.net/20151127200249233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在这里，我们使用了Class.forName(“com.example.myReflect.AnimalImpl”);找到AnimalImpl的类类型对象
然后调用 class2.getSuperclass()找到它的父类Class对象。很明显，它的父类是Animal类
由于我们这里得到了父类的Class对象parentClass，所以可以对它使用Class的一切函数。
所以调用parentClass.getName()就可以获得父类的名称了。

### (3)、获取类所直接继承的接口的Class对象
这里要先声明一个观点：Class类，不同于定义类的class标识，Class类是一个泛型。类对象是由Class对象来表示，而接口对象同样也是用Class对象来表示！
所以同样是Class对象，它可能表示的类对象，也可能表示的是接口对象！
获取接口对象的函数如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//获取普通接口的方法
publicClass<?>[] getInterfaces();
//获取泛型接口的方法
publicType[] getGenericInterfaces();
与获取superClass对象一样，这里同样有两个函数来获取接口对象，有关getGenericInterfaces()获取泛型接口的方法，我们下篇再讲，这里先讲讲获取普通接口的方法getInterfaces()；
getInterfaces()将获取指定类直接继承的接口列表！注意一点：直接继承！！！如果不是直接继承，那将是获取不到的。
我们举个例子：
同样，以上面的Animal为例：
我们先声明一个接口，让Animal类来继承：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicinterfaceIAnimal {
voidsetName(String name);
String getName();
}
然后是Animal类继承接口：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicclassAnimalimplementsIAnimal{
privateString name;
@Override
publicString getName() {
returnname;
}
@Override
publicvoidsetName(String name) {
this.name = name;
}
}
为了测试不是直接继承的接口是无法获取的问题，我们再从Animal派生一个子类AnimalImpl:
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicclassAnimalImplextendsAnimal {
}
我们再整理一下思路，Animal类直接继承了IAnimal，而AnimalImpl仅仅派生自Animal，它的IAnimal接口不是直接继承的，而是从它的父类Aniaml那带过来的
然后我们分别看看Animal类和AnimalImpl类的的获取接口的结果，完整的代码如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//获取Animal类的接口列表
Class<?> class3 = Animal.class;
Class<?>[] interfaces = class3.getInterfaces();
for(Class interItem:interfaces){
Log.d(TAG,"Animal继承的接口："+ interItem.getName());
}
//获取AnimalImpl的接口列表
class3 = AnimalImpl.class;
interfaces = class3.getInterfaces();
if(interfaces.length >0) {
for(Class interItem : interfaces) {
Log.d(TAG,"AnimalImpl继承的接口："+ interItem.getName());
}
}else{
Log.d(TAG,"AnimalImpl无继承的接口");
}
结果如下：
![](https://img-blog.csdn.net/20151127200515556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们先看看Animal类的接口列表：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<?> class3 = Animal.class;
Class<?>[] interfaces = class3.getInterfaces();
for(Class interItem:interfaces){
Log.d(TAG,"Animal继承的接口："+ interItem.getName());
}
我们通过class3.getInterfaces()来获得Animal类直接继承的接口列表，然后通过for…each打印出来。
从结果可以看出，这里找到了Animal类所继承的接口值。
那我们再来看看AnimalImpl的接口获取情况又是怎样呢：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
class3 = AnimalImpl.class;
interfaces = class3.getInterfaces();
if(interfaces.length >0) {
for(Class interItem : interfaces) {
Log.d(TAG,"AnimalImpl继承的接口："+ interItem.getName());
}
}else{
Log.d(TAG,"AnimalImpl无继承的接口");
}
先通过class3 = AnimalImpl.class;获得AnimalImpl的类类型，然后通过class3.getInterfaces()获取AnimalImpl直接继承的接口列表，然后打印出来。
从结果也可以看出，这里获取到的接口列表为空！所以这也证明了getInterfaces()只能获取类直接继承的接口列表。
### （4）、综合提升：获取某个类类型的所有接口
现在我们提升一下，如果我想传进去一下类类型，然后要得到它所有继承的接口列表要怎么办？（不管它是不是直接继承来的都要列出来）
那只有靠递规了，我们需要递规它的父类直接继承的接口、父类的父类直接继承的接口以此类推，最终到Object类的时候就找到所有继承的接口了
在开始递规获取所有接口之前，我们先构造下代码。
由于我们要获取所有接口，为了效果更好些，我们在Animal和AnimalImpl基础上，多加几个继承的接口：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//给Animal添加 IAnimal,Serializable两个接口
publicclassAnimalimplementsIAnimal,Serializable{
privateString name;
@Override
publicString getName() {
returnname;
}
@Override
publicvoidsetName(String name) {
this.name = name;
}
}
//给AnimalImpl添加Serializable接口
publicclassAnimalImplextendsAnimalimplementsSerializable {
}
所以如果我们获取AnimalImpl类的接口列表，得到的应该是三个：自已直接继承的Serializable，从父类Animal那继承的IAnimal和Serializable
好了，言规正转，看获取类类型所有接口列表的方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
/**
* 获取所传类类型的所有继承的接口列表
* @param clazz
* @return
*/
publicClass<?>[] getAllInterface(Class<?> clazz){
//获取自身的所有接口
Class<?>[] interSelf = clazz.getInterfaces();
//递规调用getAllInterface获取超类的所有接口
Class<?> superClazz = clazz.getSuperclass();
Class<?>[] interParent =null;
if(null!= superClazz) {
interParent = getAllInterface(superClazz);
}
//返回值
if(interParent ==null&& interSelf !=null){
returninterSelf;
}elseif(interParent ==null&& interSelf ==null){
returnnull;
}elseif(interParent !=null&& interSelf ==null){
returninterParent;
}else{
finalintlength = interParent.length + interSelf.length;
Class<?>[] result =newClass[length];
System.arraycopy(interSelf,0,result,0,interSelf.length);
System.arraycopy(interParent,0,result,interSelf.length,interParent.length);
returnresult;
}
}

//调用
Class<?>[] clazzes = getAllInterface(AnimalImpl.class);
SpannableStringBuilder builder =newSpannableStringBuilder();
for(Class clazz:clazzes){
builder.append(clazz.getName());
builder.append("   ");
}
Log.d(TAG,"AnimalImpl继承的所有接口:"+ builder.toString());
先看看执行结果：
![](https://img-blog.csdn.net/20151127200718242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这段代码最关键的地方在于getAllInterface(Class<?> clazz)；我们来看看这个函数是如何递规得到所有接口的数组的。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicClass<?>[] getAllInterface(Class<?> clazz){
//获取自身的所有接口
Class<?>[] interSelf = clazz.getInterfaces();
//递规调用getAllInterface获取超类的所有接口
Class<?> superClazz = clazz.getSuperclass();
Class<?>[] interParent =null;
if(null!= superClazz) {
interParent = getAllInterface(superClazz);
}
//返回值
if(interParent ==null&& interSelf !=null){
returninterSelf;
}elseif(interParent ==null&& interSelf ==null){
returnnull;
}elseif(interParent !=null&& interSelf ==null){
returninterParent;
}else{
finalintlength = interParent.length + interSelf.length;
Class<?>[] result =newClass[length];
System.arraycopy(interSelf,0,result,0,interSelf.length);
System.arraycopy(interParent,0,result,interSelf.length,interParent.length);
returnresult;
}
}
这段代码分为两部分，第一部分是获得自己的接口列表和父类的列表：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//获取自身的所有接口
Class<?>[] interSelf = clazz.getInterfaces();
//递规调用getAllInterface获取超类的所有接口
Class<?> superClazz = clazz.getSuperclass();
Class<?>[] interParent =null;
if(null!= superClazz) {
interParent = getAllInterface(superClazz);
}
首先通过Class<?>[] interSelf = clazz.getInterfaces();获得自已直接继承的接口列表。这个很好理解，可能对于有些同学而言，难就难在获取父类列表的过程：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<?> superClazz = clazz.getSuperclass();
Class<?>[] interParent =null;
if(null!= superClazz) {
interParent = getAllInterface(superClazz);
}
在这段代码中，首先，通过Class<?> superClazz = clazz.getSuperclass();获取父类的Class类型，然后调用getAllInterface(superClazz)获得父类的所有接口列表。
有些同学不解了，getAllInterface(superClazz)这不是当前函数自己吗？是的，我们写getAllInterface(superClazz)是来干什么的，就是用来获取传进去的类的所有接口，所以我们把父类传进去，当然也能获得它父类的所有接口列表了。（有关递规的知识，可能是有些难度的，递规不是本文重点，不做详细介绍，有疑问的同学可以搜搜这方面文章补充下）
我们再重复一遍，我们的getAllInterface（Class<?> clazz）函数，会返回clazz对象的所有接口列表。现在我们得到了它自己直接继承的接口，也有它父类的所有接口列表。那么，把它们两个合并，就是所有的接口列表了。
所以下面是接口列表返回的代码：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
if(interParent ==null&& interSelf !=null){
returninterSelf;
}elseif(interParent ==null&& interSelf ==null){
returnnull;
}elseif(interParent !=null&& interSelf ==null){
returninterParent;
}else{
finalintlength = interParent.length + interSelf.length;
Class<?>[] result =newClass[length];
System.arraycopy(interSelf,0,result,0,interSelf.length);
System.arraycopy(interParent,0,result,interSelf.length,interParent.length);
returnresult;
}
首先，对interParent和interSelf判空，如果两个列表都是空，那直接返回空；如果有一个是空，另一个不是空，则返回那个不是空的列表，如果两个都不是空，则将他们合并，然后返回合并后的列表。
有点难度的地方，可能是当两个都不为空的时候，合并时的代码：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
finalintlength = interParent.length + interSelf.length;
Class<?>[] result =newClass[length];
System.arraycopy(interSelf,0,result,0,interSelf.length);
System.arraycopy(interParent,0,result,interSelf.length,interParent.length);
returnresult;
这里是先根据interParent和interSelf的长度，生成一个它俩总长的一个数组result；
然后通过System.arraycopy（）函数，将它们两个复制到result数组中。
System.arraycopy（）的定义及释义如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
/**
* 将指定个数的元素从源数组src复制到目标数组dst中
*
* @param src ：源数组
* @param srcPos：源数组开始复制的item的索引，从0开始
* @param dst：目标数组
* @param dstPos：目标数组开始接收复制item的位置索引，从0开始
* @param length：要复制的元素个数
*/
publicstaticnativevoidarraycopy(Object src,intsrcPos, Object dst,intdstPos,intlength);
最后就是使用getAllInterface（）函数啦：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<?>[] clazzes = getAllInterface(AnimalImpl.class);
SpannableStringBuilder builder =newSpannableStringBuilder();
for(Class clazz:clazzes){
builder.append(clazz.getName());
builder.append("   ");
}
Log.d(TAG,"AnimalImpl继承的所有接口:"+ builder.toString());
这段代码很好理解，先获取AnimalImpl的所有接口列表，然后使用SpannableStringBuilder将它们拼接成一个String字符串，然后打印出来。
到这里，基本类的周边信息获取就结束了，下面我们来看看泛型类的周边信息获取要怎么来做。
### （5）、获取类的访问修饰符
由于我们在定义类时，比如下面的内部类：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicstaticfinalclassInnerClass{
}
在类名，前面的那一坨public static final，就是类的访问修饰符，是定义这个类在的访问区域和访问限定的。这部分就讲讲如何获取类的这部分访问修饰符
我们先看一个例子（我们以上面的内部类InnerClass为例）：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<?> clazz = getClassLoader().loadClass(InnerClass.class.getName());
intmodifiers = clazz.getModifiers();
String retval = Modifier.toString(modifiers);
booleanisFinal = Modifier.isFinal(modifiers);
Log.d(TAG,"InnerClass的定义修饰符:"+ retval);
Log.d(TAG,"is Final:"+ isFinal);
结果如下：
![](https://img-blog.csdn.net/20151127201043738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
首先，在这部分代码中，我们又换了一种类加载方式，使用的是ClassLoader;
然后我们单独来看看这句：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
intmodifiers = clazz.getModifiers();
通过clazz.getModifiers()得到一个整型变量，由于访问修饰符有很多，所以这些修饰符被打包成一个int，对应的二进制中，每个修饰符是一个标志位，可以被置位或清零。
另外Java开发人员单独提供了一个类来提取这个整型变量中各标识位的函数，这个类就是Modifier
Modifier中主要有以下几个方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//根据整型变量来生成对应的修饰符字符串
String Modifier.toString(intmodifiers)
//以下这些方法来检查特定的修饰符是否存在
booleanModifier.isAbstract(intmodifiers)
booleanModifier.isFinal(intmodifiers)
booleanModifier.isInterface(intmodifiers)
booleanModifier.isNative(intmodifiers)
booleanModifier.isPrivate(intmodifiers)
booleanModifier.isProtected(intmodifiers)
booleanModifier.isPublic(intmodifiers)
booleanModifier.isStatic(intmodifiers)
booleanModifier.isStrict(intmodifiers)
booleanModifier.isSynchronized(intmodifiers)
booleanModifier.isTransient(intmodifiers)
booleanModifier.isVolatile(intmodifiers)
首先是toString函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
String Modifier.toString(intmodifiers)
这个函数的作用就是根据传进来的整型，根据其中的标识位来判断具有哪个修饰符，然后将所有修饰符拼接起来输出。比如我们的例子中输出的就是：public static final
其它的就是一些isXXXX(int moifiers)的判断指定标识位的函数了，没什么难度
在例子中，我们使用了Modifier.isFinal(int modifiers)来判断是不是具有final修饰符，返回结果为true;
从Modifier类的isXXX()系列函数中，可以看到它的修饰符确实很多，但这些修饰符，有些类是根本用不到的，比如isNative（）等，这是因为不光类会使用Modifier类来判断访问修饰符，接口，成员变量和成员函数所对应的类型也同样都是使用Modifier来判断访问修饰符的，这些我们后面在讲到的时候，都会说到！
### （6）、获取接口的访问修饰符
从上面获取类的访问修饰符时，我们讲过，接口，类，函数都是通过Modifier类判断访问修饰符的，又因为类和接口类型全部都是用Class对象来标识，所以接口和类的获取访问修饰符的方式完全相同，下面就举一个简单的例子：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//定义一个类部接口
publicstaticinterfaceInnerInteface{
}
//使用
Class<?> clazz2 = InnerInteface.class;
intmodifiers = clazz2.getModifiers();
String retval = Modifier.toString(modifiers);
booleanisInteface = Modifier.isInterface(modifiers);
Log.d(TAG,"InnerClass的定义修饰符:"+ retval);
Log.d(TAG,"isInteface:"+ isInteface);
这里首先要注意的一点是：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
Class<?> clazz2 = InnerInteface.class;
如果我们要直接获取一个接口的对象，同样，也是通过开头所讲的那四种获取Class对象的方式。
因为我们现在知道Class对象，不光代表类也可以代表接口。
下面有关Modifier的使用与第五部分获取类的修饰符是一样的，就不再细讲。
### （7）Class.forName(String className)与ClassLoader.loadClass(String ClassName)的区别
我们通过源码来看看他们的区别：
先看Class.forName：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicstaticClass<?> forName(String className)throwsClassNotFoundException {
returnforName(className,true, VMStack.getCallingClassLoader());
}
publicstaticClass<?> forName(String className,booleaninitializeBoolean,
ClassLoader classLoader)throwsClassNotFoundException {
Class<?> result;
try{
result = classForName(className, initializeBoolean,
classLoader);
}catch(ClassNotFoundException e) {
Throwable cause = e.getCause();
if(causeinstanceofExceptionInInitializerError) {
throw(ExceptionInInitializerError) cause;
}
throwe;
}
returnresult;
}
从源中可以看到Class.forName(String className),最终调用的是forName(String className, boolean initializeBoolean,ClassLoader classLoader)
其中：
className:类名
initializeBoolean：表示是否需要初始化；如果设为true，表示在加载以后，还会进入链接阶段
classLoader：ClassLoader加载器
我们知道源文件在编译后，在运行时，分为三个阶段，加载，链接和初始化。这里的initializeBoolean就是定义是否进行链接和初始化。而Class.forName默认是设置的为true,所以利用Class.forName（）得到的类类型，除了加载进来以外，还进行了链接和初始化操作。
下面再来看看ClassLoader.loadClass()**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
publicClass<?> loadClass(String className)throwsClassNotFoundException {
returnloadClass(className,false);
}
protectedClass<?> loadClass(String className,booleanresolve)throwsClassNotFoundException {
Class<?> clazz = findLoadedClass(className);
if(clazz ==null) {
try{
clazz = parent.loadClass(className,false);
}catch(ClassNotFoundException e) {
// Don't want to see this.
}
if(clazz ==null) {
clazz = findClass(className);
}
}
returnclazz;
}
loadClass（String className）最终是调用递规函数loadClass(String className, boolean resolve)来将类加载出来。
通过代码也可以看出来ClassLoader的loadClass（String className）只是将类加载出来，并没有链接与初始化的步骤。所以这就是它们的区别
最后，我们总结一下，Class.forName(String className)不仅会将类加载进来，而且会对其进行初始化，而ClassLoader.loadClass(String ClassName)则只是将类加载进来，而没有对类进行初始化。一般来讲，他们两个是通用的，但如果你加载类依赖初始化值的话，那ClassLoader.loadClass(String ClassName)将不再适用。
**举例来说:**
在JDBC编程中，常看到这样的用法，Class.forName(“com.mysql.jdbc.Driver”)，如果换成了getClass().getClassLoader().loadClass(“com.mysql.jdbc.Driver”)，就不行。
为什么呢？打开com.mysql.jdbc.Driver的源代码看看，
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
// Register ourselves with the DriverManager
static{
try{
java.sql.DriverManager.registerDriver(newDriver());
}catch(SQLException E) {
thrownewRuntimeException("Can't register driver!");
}
}
原来，Driver在static块中会注册自己到java.sql.DriverManager。而static块就是在Class的初始化中被执行。所以这个地方就只能用Class.forName(className)。
好了，这篇就到这了，内容不太多，但比较复杂。最后我们再总结一下这篇文章所涉及到的几个函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50072739#)[copy](http://blog.csdn.net/harvic880925/article/details/50072739#)
//获取类类型对象的几种方式：
Person person =newPerson();
Class a = person.getClass()//方法一：
Class b = Persion.class;//方法二：
Class c = Class.forName(String ClassName);//方法三：
Class d = context.getClassLoader().loadClass(String ClassName);//方法四：（不建议使用）
//获取包名类名
publicString getName();//获取完整的类名（包含包名）
publicString getSimpleName();//仅获取类名
publicPackage getPackage()//获取类类型所对应的package对象
//获取超类Class对象
publicClass<?> getSuperclass();//获取普通函数的父类Class对象
publicType getGenericSuperclass();//针对泛型父类而设计（下篇讲解）
//获取接口Class对象
publicClass<?>[] getInterfaces();//获取普通接口的方法
publicType[] getGenericInterfaces();//获取泛型接口的方法
//类访问修饰符
intmodifiers = clazz.getModifiers();//获取类访问修饰符对应的int变量
String Modifier.toString(intmodifiers)//根据整型变量来生成对应的修饰符字符串
booleanModifier.isAbstract(intmodifiers)//isXXX()系列函数用以检查特定的修饰符是否存在

**如果本文有帮到你，记得加关注哦**
**本文涉及源码下载地址：****[http://download.csdn.net/detail/harvic880925/9306441](http://download.csdn.net/detail/harvic880925/9306441)**
**请大家尊重原创者版权，转载请标明出处：****[http://blog.csdn.net/harvic880925/article/details/50072739](http://blog.csdn.net/harvic880925/article/details/50072739)****谢谢**

版权声明：本文为博主原创文章，未经博主允许不得转载。

