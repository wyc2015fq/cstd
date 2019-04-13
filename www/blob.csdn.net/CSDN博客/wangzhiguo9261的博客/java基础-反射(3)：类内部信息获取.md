
# java基础-反射(3)：类内部信息获取 - wangzhiguo9261的博客 - CSDN博客


2018年02月07日 16:37:27[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：607个人分类：[java基础](https://blog.csdn.net/wangzhiguo9261/article/category/7367704)


前言：人总会疲惫，当感觉累的时候，再坚持一下就好，再坚持一下。

**相关文章：**
1、[《夯实JAVA基本之一 —— 泛型详解(1):基本使用》](http://blog.csdn.net/harvic880925/article/details/49872903)
2、[《夯实JAVA基本之一——泛型详解(2)：高级进阶》](http://blog.csdn.net/harvic880925/article/details/49883589)
3、[《夯实JAVA基本之二 —— 反射（1）：基本类周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50072739)
4、[《夯实JAVA基本之二 —— 反射（2）：泛型相关周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50085595)
5、[《夯实JAVA基本之二 —— 反射（3）：类内部信息获取》](http://blog.csdn.net/harvic880925/article/details/50107951)

上两篇文章中，给大家讲了，有关类周边信息的获取方法，这篇文章中我们将深入到类的内部，看看类的构造函数，内部变量，函数等信息的获取方法。相比而言，这篇更重要。

## 一、构造函数相关获取
### 1、获取构造函数
要获取类的构造函数，有下面几种方法：**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//获取public类型的构造函数
Constructor<?>[] getConstructors();
Constructor<T> getConstructor(Class<?>... parameterTypes);
//获取所有类型的构造函数
Constructor<?>[] getDeclaredConstructors();
Constructor<T> getDeclaredConstructor(Class<?>... parameterTypes)
这四种方法中，getConstructors与getConstructor获取的是声明为public的构造函数。无法得到声明为protected,private的构造函数。而加有Declared（声明）的两个函数getDeclaredConstructors，getDeclaredConstructor能获取所有声明的构造函数，无论它是public,proctected还是private类型，都能获取到。所以这两个函数较为常用。
下面我们举个例子来看下getDeclaredConstructors和getDeclaredConstructor的用法。
#### (1)、getDeclaredConstructors()
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Constructor<?>[] getDeclaredConstructors();
getDeclaredConstructors()函数得到的是一个Constructor的数组；
首先，构造一个类Person:
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicclassPerson {
privateintage;
privateString name;
publicPerson(){
}
privatePerson(intage, String name){
this.age = age;
this.name = name;
}
privatePerson(Integer age, String name){
this.age = age;
this.name = name;
}
publicintgetAge() {
returnage;
}
publicvoidsetAge(intage) {
this.age = age;
}
publicString getName() {
returnname;
}
publicvoidsetName(String name) {
this.name = name;
}
}
在Person类中，目前我们只关心构造函数，所以我们声明了三个构造函数，有一个无参数的构造函数，和两个有参数的构造函数。我们把无参的构造函数声明为public，另外两个都声明为private.在上面我们说过，在四个获取构造函数的方法中，只有具有Declared的两个getDeclaredXXX()的函数才能获取类中所有的函数。而另外两个只能获取声明为public的函数。下面我们就利用一个例子来获取Person类的构造函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//1、枚举
Class<?> clazz = Person.class;
Constructor<?>[] constructors = clazz.getDeclaredConstructors();
for(Constructor item:constructors){
Log.d(TAG,"枚举到的构造函数："+item.toString());
}
//2、根据类型，获取指定的构造的构造函数
Constructor<?> constructor = clazz.getDeclaredConstructor(Integer.class, String.class);
Log.d(TAG,"指定参数得到的构造函数："+constructor.toString());
结果如下：
![](https://img-blog.csdn.net/20151130154524973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们先看下这两个函数的意义，然后再来讲解上面的代码：
**（1）、getDeclaredConstructors()**
getDeclaredConstructors()将得到所有构造函数的列表，包括声明为private,protected和public的构造函数。
然后我们通过for…each循环，得到将每个构造函数打印出来。
**（2）、getDeclaredConstructor(Class<?>… parameterTypes)**
这个函数是通过指定构造函数的参数列表来得到指定构造函数的，有两点要非常注意：
parameterTypes是数据类型的Class对象，如果是原始类型，那么应该对应的是int.class,double.class(原始类型也是有class对象的)
parameterTypes一定要与要得到的构造函数中的参数声明顺序、类型及个数要完全匹配，如果多一个、少一个或类型不匹配也是找不到的。这一点是极其要注意的，getDeclaredConstructor不是模糊匹配而是精确匹配！
所以综合这两点，我们再回过头来看一下我们代码：**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Constructor<?> constructor = clazz.getDeclaredConstructor(Integer.class, String.class);
在这里，我们指定匹配构造函数时，指定参数列表，这个要匹配的构造函数的第一个参数是Integer类型，第二个参数是String类型。
大家可能会怀疑了，我们这里好像有两个构造函数能匹配：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
privatePerson(intage, String name){
this.age = age;
this.name = name;
}
privatePerson(Integer age, String name){
this.age = age;
this.name = name;
}
在这两个构造函数中，第一个构造函数的age类型是原始类型int，第二个构造函数的age参数类型是Integer.
那clazz.getDeclaredConstructor(Integer.class, String.class);要匹配哪个呢？
由于这里是严格匹配，所以这里必然匹配的是private Person(Integer age, String name)函数，如果我们要匹配private Person(int age, String name)要怎么办呢？那获取构造函数的方法就要这么写了：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Constructor<?> constructor = clazz.getDeclaredConstructor(int.class, String.class);
这里要非常提醒大家的是，不光派生自Object的类具有Class对象，原始的数据类型也是具有Class对象的。
### 2、Constructor系列方法之构造实例
利用Constructor的对象来构造实例时，主要是使用的是newInstance方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicT newInstance(Object... args);
可以看到newInstance有可以传入可变长参数，值得非常注意的是，传入的参数类型、顺序及个数都必须与当前的Constructor对象一一对应，不然就会报下面的错误：
![](https://img-blog.csdn.net/20151130154745297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面我们举个例子来看下这个函数的用法：
我们同样，这个例子还是基于Person类的：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicclassPerson {
privateintage;
privateString name;
publicPerson(){
}
privatePerson(intage, String name){
this.age = age;
this.name = name;
}
privatePerson(Integer age, String name){
this.age = age;
this.name = name;
}
publicintgetAge() {
returnage;
}
publicvoidsetAge(intage) {
this.age = age;
}
publicString getName() {
returnname;
}
publicvoidsetName(String name) {
this.name = name;
}
}
仍然要注意的是，这里的三个构造函数，一个无参的构造函数的访问符是public，另外两个全是private!
下面来看看，如何使用newInstance来构造Person的实例：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?> constructor = clazz.getDeclaredConstructor(Integer.class, String.class);
constructor.setAccessible(true);
//构造实例一
Person person1 = (Person) constructor.newInstance(newInteger(30),newString("harvic"));
Log.d(TAG,"构造的参数为："+ person1.getName() +"  "+ person1.getAge());
//构造实例二
Person person2 = (Person) constructor.newInstance(50,"qijian");
Log.d(TAG,"构造的参数为："+person2.getName() +"  "+ person2.getAge());
//构造实例三
Person person3 = (Person) constructor.newInstance();
person3.setAge(30);
person3.setName("qijian");
Log.d(TAG,"构造的参数为："+person3.getName() +"  "+ person3.getAge());
结果如下：
![](https://img-blog.csdn.net/20151130154830875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这段代码总共分为四部分：
**第一部分：获取Constructor对象**
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?> constructor = clazz.getDeclaredConstructor(Integer.class, String.class);
constructor.setAccessible(true);
首先，通过getDeclaredConstructor(Integer.class, String.class)获取指定的构造函数，要得到的这个构造函数，第一个参数是Integer类型，第二个参数是String类型。
所以匹配的是下面这个构造函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
privatePerson(Integer age, String name){
this.age = age;
this.name = name;
}
最后一句利用的是Constructor的函数setAccessible，它完整的声明是：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
voidsetAccessible(booleanflag)
它代表的含义是，是否将任何函数或字段设置为可访问的。如果设置为true，就不管这个函数或者字段是声明为private还是public，都是可以访问的，默认情况下是false，即只有public类型是可访问的。如果没有设置setAccessible(true)的话，在使用protected或者private构造函数创建实例时，会提示：（访问拒绝）
![](https://img-blog.csdn.net/20151130154939573?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在反射中，总共有三种类型具有setAccessible(boolean flag)函数：Constructor,Field,Method；分别对应构造函数，成员变量和成员函数。我们知道在一个类中，也就只有构造函数，成员变量和成员函数这三部分，而且这三部分都可能会被设置为private和protected.所以它们三个都会具有设置是否可访问的setAccessible(boolean flag)函数。（后面用到还会再讲）
**第二部分：使用标准的Integer,String构造实例**
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//构造实例一
Person person1 = (Person) constructor.newInstance(newInteger(30),newString("harvic"));
Log.d(TAG,"构造的参数为："+ person1.getName() +"  "+ person1.getAge());
在这个利用中，我们向newInstance（）函数中传递的两个参数是严格遵守constructor所对应的构造函数的，即第一个参数是Integer类型，第二个参数是String类型
然后将constructor.newInstance的返回值强转成Person的实例，最后通过person的函数person1.getName()和person1.getAge()获取它的内容。
**第三部分：使非标准，但可转换的参数来构造实例**
我们再看来第二个构造方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//构造实例二
Person person2 = (Person) constructor.newInstance(50,"qijian");
Log.d(TAG,"构造的参数为："+person2.getName() +"  "+ person2.getAge());
在这个构造方法中，我们传递的两个参数都不是标准的Integer和String类型。但第一个参数50是可以强转成Integer类型的，而第二个字符串，也是可以强转为String类型的。所以对于可以强转成为参数类型的数值，也是可以成功接受的！所以从结果也可以看出，我们可以成功得到person2中的值。
**第四部分：如果不按规则传递参数会怎样**
我们知道，我们得到的constructor对应的构造函数是：private Person(Integer age, String name)，所在我们在newInstance的时候，必须声明这个Constructor时的参数类型，顺序及个数一个个传进去参数的对应值，如果我们不传、少传或者多传，结果会怎样呢？
我们来看看第三个构造实例：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//构造实例三
Person person3 = (Person) constructor.newInstance();
person3.setAge(30);
person3.setName("qijian");
Log.d(TAG,"构造的参数为："+person3.getName() +"  "+ person3.getAge());
在这个实例中，我们没有按照constructor的参数顺序去填对应的值，而是压根没有填任何的值！如果我们constructor对应的构造函数是下面这个没有参数的构造函数的话，这么做是允许的。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicPerson(){
}
而我们这里的constructor对应的构造函数却是：private Person(Integer age, String name)，即便我们后面利用person3.setAge(30);person3.setName(“qijian”);重新设置进了各种参数，但这是没用的，因为在程序执行到constructor.newInstance();的时候就已经报错了。
错误信息为：（参数个数不对）
![](https://img-blog.csdn.net/20151201082436823?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 3、Constructor系列方法之获取参数类型
Constructor中获取参数类型的主要有两个方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//用于解析一般函数
Class<?>[] getParameterTypes();
//用于解析泛型对象
Type[] getGenericParameterTypes();
这里依然有两个函数，getParameterTypes()用于解析普通参数类型的构造函数，而 getGenericParameterTypes()则用于解析具有泛型类型参数的构造函数，它返回的是参数列表所对应的Type数组，有关Type类型的解析比如复杂，在第二篇文章中已经详细讲过，这里就不再赘述，有必要请参考文章[《夯实JAVA基本之二
 —— 反射（2）：泛型相关周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50085595)
这里为了减轻本文理解难度，采用getParameterTypes()举例子：
下面的程序将实现，分别打印出Person.class中所有构造函数的参数类型列表：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?>[] constructors = clazz.getDeclaredConstructors();
for(Constructor<?> c:constructors){
c.setAccessible(true);
Class<?>[] types = c.getParameterTypes();
StringBuilder builder =newStringBuilder("获取参数类型为：");
for(Class t:types){
builder.append(t.getName());
builder.append("   ");
}
Log.d(TAG,builder.toString());
}
执行结果如下：
![](https://img-blog.csdn.net/20151201082633322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们看看代码是如何实现的：
首先，获取所有构造函数列表：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?>[] constructors = clazz.getDeclaredConstructors();
然后枚举所有的构造函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
for(Constructor<?> c:constructors){
c.setAccessible(true);
Class<?>[] types = c.getParameterTypes();
…………
}
上面针对每个构造函数，首先利用c.setAccessible(true)将该构造函数设置为可访问的，也就是让编译器忽略该函数的访问权限。然后利用Class<?>[] types = c.getParameterTypes();得到这个函数的所有参数类型列表。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
StringBuilder builder =newStringBuilder("获取参数类型为：");
for(Class t:types){
builder.append(t.getName());
builder.append("   ");
}
Log.d(TAG,builder.toString());
然后利用StringBuilder将每个参数列表中的每个Class对象的完整名称拼接起来然后打印出来。
### 4、Constructor系列方法之获取构造函数的访问修饰符
在第一篇中，我们就已经讲过访问修饰符的概念及用法，这里不打算再细讲，仅通过一个例子来简单说明它的用法，有关详细的访问修饰符的定义及用法，请参考[《夯实JAVA基本之二 —— 反射（1）：基本类周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50072739)
示例函数：打印出每个构造函数的访问修饰符
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?>[] constructors = clazz.getDeclaredConstructors();
for(Constructor<?> c:constructors) {
c.setAccessible(true);
intmodifier = c.getModifiers();
Log.d(TAG,"一个访问修饰符为："+Modifier.toString(modifier));
}
结果如下：
![](https://img-blog.csdn.net/20151201082817747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这段代码难度不大，就不再讲了。

### 5、Constructor系列方法之getDeclaringClass()
getDeclaringClass()方法声明如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<T> getDeclaringClass();
该函数用于得到声明Constructor的类的Class对象；
我们用个例子来说明下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?> constructor = clazz.getDeclaredConstructor();
Class<?> declarClazz = constructor.getDeclaringClass();
Log.d(TAG,declarClazz.getName());
结果如下：
![](https://img-blog.csdn.net/20151201082939081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们来说明一下这个例子
首先，得到一个默认的无参数的构造函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?> constructor = clazz.getDeclaredConstructor();
然后得到声明这个constructor的类的Class对象，即Person.Class
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> declarClazz = constructor.getDeclaringClass();
Log.d(TAG,declarClazz.getName());
好了，有关Constructor所有常用函数都讲完了，下面我们看看类中其它参数的获取方法。
**源码在文章底部给出**

## 二、成员变量的获取与设置
### 1、获取Field对象
类中的信息，无外乎有三种，：构造函数，成员变量和成员函数。类中成员变量相关的信息都保存在Filed类型的对象里。
Class中对Field对象的获取有下面四个函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//仅能获取声明类型为public的成员变量
Field[] getFields();
Field getField(String name);
//可以获取全部的成员变量
Field[] getDeclaredFields();
Field getDeclaredField(String name)
可以看到与Constructor的获取方法类似，这里也有四个函数，其中getFields()和getField(String name)只能够获取到声明为public类型的成员变量，而getDeclaredFields()和getDeclaredField(String name)则可以得到所有的成员变量，无论是设置为public,protected还是private都是可以得到的。
getFields()和getDeclaredFields()是得到所有能够得到的成员变量的Field数组。
getField(String name)和getDeclaredField(String name)的意义是得到指定变量名的对应的Field对象。
下面，我们就利用getDeclaredXXX（）系列函数来做讲解。
#### (1)、getDeclaredFields()
我们依然是Person来做例子：
Person.class:
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicclassPerson {
privateintage;
privateString name;
publicPerson(){
}
…………
}
在person类中，我们有两个成员变量，一个是int age，另一个是String name，非常注意的是这两个变量全部都被声明为private!所以一方面，我们只有利用getDeclaredXXX()系列函数才能得到他们的Field。另一方面，如果要访问每个成员变量所对应Field对象的信息，就必须设置field.setAccessible(true);在Constructor中，我们已经提到在类中的三部分：构造函数、成员变量、成员函数，他们都可能会被设置为private或protected类型，所以为了能让用户访问private或protected类型的对象，所以在它们三个所对应的类型Constructor,Field,Method中都有实现setAccessible(boolean
 flag)函数。意义与都完全一样：表示是否将当前对象设置成为可访问的。默认是false,即如果当前对象的声明类型为private或protected就无法访问。
我们看例子：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Field[] fields = clazz.getDeclaredFields();
for(Field field:fields){
field.setAccessible(true);
Class<?> type = field.getType();
Log.d(TAG,"枚举到的field:"+type.getName()+"  "+field.getName());
}
结果为：
![](https://img-blog.csdn.net/20151201083216510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们来分析下这段代码
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Field[] fields = clazz.getDeclaredFields();
首先，利用clazz.getDeclaredFields()得到所有成员变量所对应的Field对象数组。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
for(Field field:fields){
field.setAccessible(true);
Class<?> type = field.getType();
Log.d(TAG,"枚举到的field:"+type.getName()+"  "+field.getName());
}
然后，对每一个field对象进行枚举，利用field.setAccessible(true);针对每一个field将其设置为可访问的，因为我们的所有成员变量都是private,所以必须设置field.setAccessible(true);否则，在下面利用 field.getType()和field.getName()获取对应Field信息的时候，就会报错。
这里额外用到两个函数：
field.getName()：用于得到当前成员变量的名称，比如Person类中的age,name
field.getType()：用于得到当前成员变量的类型。
#### （2）、getDeclaredField(String name)
此函数完整的声名如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Field getDeclaredField(String name)
参数中的name：用于指定要获取的成员变量的名称。
下面我们举个例子来说明下它的用法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Field field = clazz.getDeclaredField("age");
field.setAccessible(true);
Class<?> type = field.getType();
Log.d(TAG,"得到age对应的field:"+type.getName()+"  "+field.getName());
结果为：
![](https://img-blog.csdn.net/20151201083418070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在本例中，我们通过指定clazz.getDeclaredField(“age”)来获取变量为为age的成员变量，然后利用field.getType()获取对应Field对象的类型，利用field.getName()得到成员变量的名称。
我们这里知道了怎么得到成员变量的Field对象，下面就来看看Field中有哪些操作吧。

### 2、Field的get、set操作
#### （1）、get()与set()
当获取或设置指定类对象中某变量的值时，可以使用Field中的get,set方法，声明如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
voidset(Object object, Object value)
Object get(Object object)
set时，有两个Object对象，而get时参数是一个Object对象，返回值也是一个Object对象，搞得让人有点迷糊，我们先不看他们的意义，直接来看看使用方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?> constructor = clazz.getConstructor();
Person person = (Person)constructor.newInstance();
Field fName = clazz.getDeclaredField("name");
fName.setAccessible(true);
fName.set(person,"qijian");
String val = (String)fName.get(person);
Log.d(TAG,"fieldName:"+ val +"   personName:"+ person.getName());
结果如下：
![](https://img-blog.csdn.net/20151201083548198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在上面的例子中，我们先通过Constructor得到Person的一个实例：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?> constructor = clazz.getConstructor();
Person person = (Person)constructor.newInstance();
然后得到成员变量name所对应的Field对象，并将其设置为可访问的：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Field fName = clazz.getDeclaredField("name");
fName.setAccessible(true);
下面就是利用Field类的set,get函数的操作了：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
fName.set(person,"qijian");
String val = (String)fName.get(person);
大家注意，在set时，第一个参数是要设置的Person的实例，第二个参数是这个Field所对应的值。
在get时，传递的参数是当前要获取的Person实例，得到的是当前实例中对应成员变量的值。
我们再回过头来看看get，set函数
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//第一个参数object为要设置的类的对象，第二个值是此Field对象所对应的值
voidset(Object object, Object value)
//参数是要获取值的类的对象，返回值为在此类的实例中，对应成员变量的值
Object get(Object object)

#### (2)get系列函数与set系列函数
再回过头来看上面的get与set函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
voidset(Object object, Object value)
Object get(Object object)
在set中，要传入所Field的值必须是派生自Object类型的，也就规定这个Field的所对应的成员变量类型就必须是派生自Object的的类型，比如Integer,String等。
在get中，获取到的值的类型，也是Object类型的，同样，也就要求要通过这个get函数获取Field所对应的成员变量的值，此成员变量的类型就必须是派生自Object的类型！
那么问题就来了，那我们的成员变量的类型，并不一定是派生自Object的类型啊，也可能是原始的数据类型，比如int,double,char,byte等，那这些类型的成员变量的值要怎么设置与获取呢。
为了解决这个问题，Java的开发者们，额外开发了几对get与set函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//设置与获取int类型的值
voidsetInt(Object object,intvalue)
intgetInt(Object object)
//设置与获取double类型的值
voidsetDouble(Object object,doublevalue)
doublegetDouble(Object object)
//设置与获取float类型的值
voidsetFloat(Object object,floatvalue)
floatgetFloat(Object object)
//设置与获取bool类型的值
voidsetBoolean(Object object,booleanvalue)
booleangetBoolean(Object object)
//设置与获取short类型的值
voidsetShort(Object object,shortvalue)
shortgetShort(Object object)
//设置与获取long类型的值
voidsetLong(Object object,longvalue)
longgetLong(Object object)
//设置与获取byte类型的值
voidsetByte(Object object,bytevalue)
bytegetByte(Object object)
//设置与获取char类型的值
voidsetChar(Object object,charvalue)
chargetChar(Object object)
这些就是所有的原始类型的变量相关的设置与获取方法。
下面我们就利用我们的例子来看看：
同样还是我们的Person类，因为我们的Person中的成员变量age的类型是int：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicclassPerson {
privateintage;
privateString name;
…………
}
所以，我们来通过setInt与getInt函数对来设置与获取一下person变量中它的值。
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?> constructor = clazz.getConstructor();
Person person = (Person)constructor.newInstance();
Field fAge = clazz.getDeclaredField("age");
fAge.setAccessible(true);
fAge.setInt(person,20);
Log.d(TAG,"fieldAge:"+ fAge.getInt(person) +"   personAge:"+ person.getAge());
结果如下：
![](https://img-blog.csdn.net/20151201083905759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在这个例子中，我们先通过setInt将20设置进person实例的age变量中，然后分别通过fage.getInt与person自己的person.getAge()取出。
没什么难度，就不再细讲了。

#### （3）、给原始类型使用set,get函数
我们再尝试一下，如果我给声明为原始类型的变量使用set,get函数，能通过吗？
看看下面的代码：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Constructor<?> constructor = clazz.getConstructor();
Person person = (Person)constructor.newInstance();
Field fAge = clazz.getDeclaredField("age");
fAge.setAccessible(true);
fAge.set(person,30);
Integer ageval = (Integer)fAge.get(person);
Log.d(TAG,"fieldAge:"+ ageval.toString() +"   personAge:"+ person.getAge());
结果如下：
![](https://img-blog.csdn.net/20151201083955353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
首先，这段代码是可以通过的，而且也有输出的值，我们再来看看，我们都做了什么。
最关键的是这段：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
fAge.set(person,30);
Integer ageval = (Integer)fAge.get(person);
我们在给age变量设置值时通过的set(Object object, Object value)函数，我们知道age成员变量的类型是Int,不是Integer那这里是怎么设置成功的呢。
首先，这里的数字30，编译器会强转成new Integer(30),做为参数传给set函数中。
而在真正给age设置时，Integer类型也是可以强转成Int类型的！所以不会出问题。
在通过get取出时，也是一样的，正是由于int和Integer是可以相互转换的，所以直接利用get与set是不会出问题的。
所以结论出来了：如果我们直接利用set(),get()函数来设置与获取原始类型的变量，要保证所设置的Object类型与原始类型是可以相互转换的！如果不能，那将报错！
所以，为了保险起见，如果遇到原始类型的变量，我们还是使用专门针对原始类型而设置的setxxx,getxxx系列函数为好。
### 3、Field之isEnumConstant()
该函数的声明为：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
booleanisEnumConstant()
这个函数用于判断当前field是否为枚举常量；
我们先说明一下，什么是枚举常量，注意是常量！不是变量！
下面这个例子中，定义一个枚举COLOR，然后利用COLOR定义一个color变量：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicstaticenumCOLOR{WHITE,BLACK,YELLOW}
privateCOLOR color;
首先，枚举类COLOR中，WHITE,BLACK,YELLOW它们三个都是枚举常量！
而private COLOR color，这个color是COLOR类型的变量！
下面我们同样在Person的基础上举个例子：
我们先给Person添加一个枚举：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicclassPerson {
publicstaticenumCOLOR{WHITE,BLACK,YELLOW}
privateintage;
privateString name;
privateCOLOR color;
…………
}
在这里，我们添加了两部分，先添加了一个枚举类COLOR,另外添加一个COLOR类型的变量color。
我们看看下面的获取代码：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz2 = Person.COLOR.class;
Field field = clazz2.getDeclaredField("WHITE");
Log.d(TAG,"COLOR.WHITE是否是枚举常量："+field.isEnumConstant()+"");
Class<?> clazz = Person.class;
Field fColor = clazz.getDeclaredField("color");
fColor.setAccessible(true);
booleanisEnum = fColor.isEnumConstant();
Log.d(TAG,"color是否是枚举常量："+isEnum);
结果如下：
![](https://img-blog.csdn.net/20151201084239315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们从结果中可以看到，首先COLOR枚举类中的WHITE是一个枚举常量，而利用COLOR定义的color变量不是枚举常量
我们再来看看代码，先看看COLOR部分：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz2 = Person.COLOR.class;
Field field = clazz2.getDeclaredField("WHITE");
Log.d(TAG,"COLOR.WHITE是否是枚举常量："+field.isEnumConstant()+"");
首先，我们在这里可以看到枚举类型也是有对应的class变量的，我们能依然能通过 clazz2.getDeclaredField(“WHITE”);来获取其中的声明的值的Field对象；
然后，我们再看看操作color的代码：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Field fColor = clazz.getDeclaredField("color");
fColor.setAccessible(true);
booleanisEnum = fColor.isEnumConstant();
Log.d(TAG,"color是否是枚举常量："+isEnum);
在这段代码中，我们利用clazz.getDeclaredField(“color”)得到成员变量color所对应的Field对象fColor，然后利用fColor.isEnumConstant();来获取它是否是枚举常量。
### 4、Field之Modifiers与getDeclaringClass()
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
//获取该成员变量所对应的访问修饰符组所对应的Int数字
intgetModifiers()
//获取声明该变量的类
Class<?> getDeclaringClass()
getModifiers()：获取该Field所对应的成员变量的访问修饰符。有关访问修饰符的概念，我们已经在第一篇文章中讲过，不再赘述
getDeclaringClass()：获取声明该成员变量的类的Class对象。比如这里声明int age的类是Person，所以：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Field fAge = clazz.getDeclaredField("age");
Class<?> declareClazz = fAge.getDeclaringClass();
这个declareClazz的值就是Person.Class，这个函数的用法，我们在讲解Constructor的时候已经讲过了，不再讲了。
**源码在文章底部给出**

## 三、成员函数的获取与设置
### 1、获取Method对象
与成员变量对应的类型为Field类似，类中的方法对应的类型是Method.
获取指定类中的成员函数的方法有下面四种方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Method[] getMethods()
Method getMethod(String name, Class<?>... parameterTypes)
Method[] getDeclaredMethods()
Method getDeclaredMethod(String name, Class<?>... parameterTypes)
与上面所有的有Declared系列的函数组一样，getMethods()和getMethod()只能获取声明为public的函数。而getDeclaredMethods()、getDeclaredMethod()可以得到所有的函数，无论声明为public,protected还是private。
下面我们针对getDeclaredMethods()和getDeclaredMethod()来举个例子说明其用法：
#### (1)、getDeclaredMethods()
该函数能够获取类中所有声明的函数（不包括构造函数），举个例子来看他使用方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Method[] methods = clazz.getDeclaredMethods();
for(Method m:methods){
Log.d(TAG,"枚举到的方法："+m.toString());
}
结果如下：
![](https://img-blog.csdn.net/20151201084827772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上面的例子很容易理解，首先通过 clazz.getDeclaredMethods()得到Person类中，所有的成员函数，然后利用for…each循环将它们逐个打印出来

#### （2）、getDeclaredMethod(String name, Class<?>… parameterTypes)
该函数的完整声明如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Method getDeclaredMethod(String name, Class<?>... parameterTypes)
第一个参数name:表示要获取Method对象的成员函数名
第二个可变长参数parameterTypes：需要匹配的成员函数需要传入的参数的类型，这里需要非常注意，这里的getDeclaredMethod是精确匹配的，如果我们传入的函数名以及参数类型、顺序、个数与该函数声明不一致的话，就会报错
值得非常注意的是，当我们要找的方法所对应的参数类型是原始数据类型，比如Int,double等，它们所对应的Class对象为int.class,double.class即可；所以原始类型也是有Class对象的！
下面我们举个例子来看：
我们还是用Person类，我们知道Person类中有一个setName（String name）函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicclassPerson {
publicstaticenumCOLOR{WHITE,BLACK,YELLOW}
privateintage;
…………
publicvoidsetName(String name) {
this.name = name;
}
}
下面看我们如何用getDeclaredMethod来找到setName(String name)函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Method method = clazz.getDeclaredMethod("setName",String.class);
Log.d(TAG,"得到指定方法："+method.toString());
结果如下：
![](https://img-blog.csdn.net/20151201084949207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从代码中也可以看出getDeclaredMethod很容易使用，与我们讲解的用法一样，第一个参数传入函数名，第二个参数逐个传入该函数的参数类型。然后就可以得到该函数的Method对象了。

### 2、Method之Invoke
Invoke函数无疑是Method类中最重要的方法，它的功能是用于执行Method对象所对应的函数。完整的声明如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Object invoke(Object receiver, Object... args)
参数讲解：
Object receiver：指要执行该函数的类对象，比如我们的Person类的某一实例
Object… args:可变长参数，用于传入该函数声明中的参数所对应的值的列表；
返回值：Object：将函数的结果返回，Object对象就是执行此函数后返回的结果
下面我们看个例子来说明下它的用法：
我们在Person中单独再加一个函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicclassPerson {
…………
publicBoolean testInvoke(Integer age,String name){
Log.d("qijian","得到参数age:"+age +"   name:"+name);
returntrue;
}
}
我们单独添加了一个testInvoke函数，他需要传入两个参数分别是Integer类型和String类型。在函数中，我们将传入的函数打印出来，然后返回true;
下面我们来看看如何使用Method.Invoke函数来执行testInvoke()
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Person person =newPerson();
Method method = clazz.getDeclaredMethod("testInvoke", Integer.class,String.class);
method.setAccessible(true);
Boolean result = (Boolean)method.invoke(person,25,"I m harvic");
Log.d(TAG,"执行结果:"+result);
这段代码分为两段：
结果如下：
![](https://img-blog.csdn.net/20151201085119857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第一部分，得到Person实例与testInvoke所对应的Method对象：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = Person.class;
Person person =newPerson();
Method method = clazz.getDeclaredMethod("testInvoke", Integer.class,String.class);
然后，将Method对象设为可访问的（其实这里设为可访问的，并没有什么用，因为testInvoke本来就是public类型的）
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
method.setAccessible(true);
最后是执行invoke函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Boolean result = (Boolean)method.invoke(person,25,"I m harvic");
可见，invoke的第一个参数是传进去要执行哪个Person对象的testInvoke函数。然后后面逐个传进去testInvoke所需要的参数所对应的值。
最后invoke会将函数执行结果以Object类型返回。我们可以将它强转为我们需要的类型。
这里总结一下，从Field，Method中都可以看出，Field,Method的对象对应的不是哪个类实例中变量，而是类中变量的对象。
也就是说，Field,Method对象只有一个,是在编译时，保存在Class中的。而Person的实例却可以有很多个，如果要改变哪个Person实例中成员变量的值，就必须将该Person实例传入Field对象的参数中，让Field对象设置。
### 3、Method之获取参数类型
与其它类的获取参数类型一样，这里同样针对普通变量和泛型有两种获取参数类型的方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?>[] getParameterTypes()
Type[] getGenericParameterTypes()
getParameterType用于获取普通的参数类型
getGenericParameterTypes用于获取带有泛型参数的类型，有关泛型的获取比较复杂，我们单独用一篇来讲述有关泛型相关方法的获取，如要必须请大家参考[《夯实JAVA基本之二 —— 反射（2）：泛型相关周边信息获取》](http://blog.csdn.net/harvic880925/article/details/50085595)
这里我以getParameterType来举例子：(同样使用Person类中的testInvoke函数)
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz  = Person.class;
Method method = clazz.getDeclaredMethod("testInvoke", Integer.class,String.class);
Class<?>[] params = method.getParameterTypes();
for(Class c:params){
Log.d(TAG,"枚举到参数类型："+c.getName());
}
执行结果如下：
![](https://img-blog.csdn.net/20151201085405769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这段代码很好理解，首先利用clazz.getDeclaredMethod找到testInvoke所对应的Method对象。
然后利用method.getParameterTypes()得到testInvoke的参数列表。最后参数枚举打印出来

### 4、Method之获取返回值类型
获取指定成员函数返回值类型的方法，同样根据返回值是否是泛型，同样有两种方法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> getReturnType()
Type getGenericReturnType()
这两个函数都没什么难度，我们以getReturnType为例来看看它的用法：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz  = Person.class;
Method method = clazz.getDeclaredMethod("testInvoke", Integer.class,String.class);
Class type = method.getReturnType();
Log.d(TAG,"返回值类型为："+type.getName());
结果为：
![](https://img-blog.csdn.net/20151201085518476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这段代码也非常简单，首先得到Person类中成员函数testInvoke的Method对象；然后利用method.getReturnType()得到它的返回值类型。
好了，到这里反射相关的所有内容就讲完，一句话，涉及面太TM广了……有关泛型相关的部分是比较难的，大家可能需要多看几篇，下面我们举两个例子来看在实际代码中，反射的用法
**源码在文章底部给出**

## 四、实战实例讲解
### 1、实例一：Fragment.instantiate(Context context, String fname, @Nullable Bundle args)
在V4包中，Fragment有一个函数，可以根据指定Fragment的路径名来得到对应Fragment的实例。
它的完整声明如下：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicstaticFragment instantiate(Context context, String fname, Bundle args)
参数说明如下：
context:Context类型对象;
fname：对应Fragment类的完整的路径名，可以利用Class.getName()得到。
args：是在构造Fragment实例时需要的Bundle参数。
#### （1）、使用方法
我们先看下Fragment.instantiate的用法：
我们先引入v4包，然后新建一个Fragment：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicclassDemoFragmentextendsFragment {
publicvoidprintFragment(String name){
Log.d("qijian",name+"就知道你是个逗逼");
}
}
这个DemoFragment派生于V4包中的Fragment，然后有一个函数printFragment(String name)，可以传进去一个String字符串。
然后我们来看看怎么利用Fragment.instantiate来获取它的实例：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class<?> clazz = DemoFragment.class;
DemoFragment fragment = (DemoFragment) Fragment.instantiate(this,clazz.getName(),null);
fragment.printFragment("harvic");
这段代码总共三名，首先得到DemoFragment的Class对象clazz
然后利用Fragment.instantiate(this,clazz.getName(),null);获取它的实例。
这里需要注意的是，我们传进去的完整的路径名，通过clazz.getName()得到。
然后利用fragment.printFragment(“harvic”);执行DemoFragment中的printFragment函数。
执行结果如下：
![](https://img-blog.csdn.net/20151201085707020?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### （2）、原理
上面学会了使用Fragment的instantiate函数，下面就来看看这个函数内部是如何实现的吧：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicstaticFragment instantiate(Context context, String fname, Bundle args) {
try{
Class e = (Class)sClassMap.get(fname);
if(e ==null) {
e = context.getClassLoader().loadClass(fname);
sClassMap.put(fname, e);
}
Fragment f = (Fragment)e.newInstance();
if(args !=null) {
args.setClassLoader(f.getClass().getClassLoader());
f.mArguments = args;
}
returnf;
}catch(ClassNotFoundException var5) {
…………
}
}
这段代码总共分为两部分：
第一部分，根据fName得到对应Fragment的Class对象：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
Class e = (Class)sClassMap.get(fname);
if(e ==null) {
e = context.getClassLoader().loadClass(fname);
sClassMap.put(fname, e);
}
它这里首先通过一个sClassMap中查找一下，如果没有，则通过ClassLoader将其加载到内存中，并将其保存到sClassMap中。
第二部分：构造实例与设置参数：
这里首先，通过Class.newInstance()函数得到对应Fragment的实例，然后将我们传入的Bundle参数args加入到Fragment的mArguments参数对象中。
最后将Fragment的实例f返回。**源码在文章底部给出**
### 2、实例二：PopupWindow之setTouchModal(boolean touchModal)
如果我想要一个效果,点击PopupWindow外部区域,弹窗不消失,但是点击事件会向下面的activity传递,这时候就需要用到PopupWindow的setTouchModal(boolean touchModal)函数。
但setTouchModal(boolean touchModal)函数完整声明为：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
/**
* Set whether this window is touch modal or if outside touches will be sent to
* other windows behind it.
* @hide
*/
publicvoidsetTouchModal(booleantouchModal) {
mNotTouchModal = !touchModal;
}
在注解中，利用@hide将此函数隐藏了，我们通过PopupWindow对象是没有办法得到调用它的。所以我们只有通过反射来做。
下面我们来举个例子来调用这个函数：
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
publicstaticvoidsetPopupWindowTouchModalthrowsException(PopupWindow popupWindow,
booleantouchModal) {
if(null== popupWindow) {
return;
}
Method method = PopupWindow.class.getDeclaredMethod("setTouchModal",boolean.class);
method.setAccessible(true);
method.invoke(popupWindow, touchModal);
}
反射代码比较简单，我们先找到PopupWindow的setTouchModal方法所对应的Method对象，然后利用invoke(popupWindow, touchModal);执行setTouchModal函数而已。难度不大，不再细讲了。
到这里，有关反射所有内容就讲完了，这几篇文章无论是难度还是篇幅都是比较难的，大家能坚持看完也真是太不容易了。
**本文所涉及函数如下：**
Constructor相关
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
/**
* 获取构造函数
*/
//获取public类型的构造函数
Constructor<?>[] getConstructors();
Constructor<T> getConstructor(Class<?>... parameterTypes);
//获取所有类型的构造函数
Constructor<?>[] getDeclaredConstructors();
Constructor<T> getDeclaredConstructor(Class<?>... parameterTypes)
/*
* 获取对应类的实例
*/
publicT newInstance(Object... args);
/**
* 获取参数类型
*/
//用于解析一般函数
Class<?>[] getParameterTypes();
//用于解析泛型对象
Type[] getGenericParameterTypes();
/**
* 获取访问修饰符
*/
intgetModifiers()
/**
* 获取声明Constructor的类的Class对象；
*/
Class<T> getDeclaringClass()

Field相关
**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
/**
* 获取Field对象
*/
//仅能获取声明类型为public的成员变量
Field[] getFields();
Field getField(String name);
//可以获取全部的成员变量
Field[] getDeclaredFields();
Field getDeclaredField(String name)
/**
* get,set方法系列
*/
//当获取或设置指定类对象中某变量的值
voidset(Object object, Object value)
Object get(Object object)
//设置与获取int类型的值
voidsetInt(Object object,intvalue)
intgetInt(Object object)
//设置与获取double类型的值
voidsetDouble(Object object,doublevalue)
doublegetDouble(Object object)
//设置与获取float类型的值
voidsetFloat(Object object,floatvalue)
floatgetFloat(Object object)
//设置与获取bool类型的值
voidsetBoolean(Object object,booleanvalue)
booleangetBoolean(Object object)
//设置与获取short类型的值
voidsetShort(Object object,shortvalue)
shortgetShort(Object object)
//设置与获取long类型的值
voidsetLong(Object object,longvalue)
longgetLong(Object object)
//设置与获取byte类型的值
voidsetByte(Object object,bytevalue)
bytegetByte(Object object)
//设置与获取char类型的值
voidsetChar(Object object,charvalue)
chargetChar(Object object)
/**
* 这个函数用于判断当前field是否为枚举常量；
*/
booleanisEnumConstant()

Method相关**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50107951#)[copy](http://blog.csdn.net/harvic880925/article/details/50107951#)
/*
* 获取指定类中的成员函数Method对象
*/
//获取声明为public的成员函数
Method[] getMethods()
Method getMethod(String name, Class<?>... parameterTypes)
//获取所有的成员函数
Method[] getDeclaredMethods()
Method getDeclaredMethod(String name, Class<?>... parameterTypes)
/*
* 执行某个方法
*/
Object invoke(Object receiver, Object... args)
/**
* 获取参数类型
*/
Class<?>[] getParameterTypes()
Type[] getGenericParameterTypes()
/**
* 获取返回值类型
*/
Class<?> getReturnType()
Type getGenericReturnType()

**如果本文有帮到你，记得加关注哦****本文涉及源码下载地址：****[http://download.csdn.net/detail/harvic880925/9314371](http://download.csdn.net/detail/harvic880925/9314371)**
**请大家尊重原创者版权，转载请标明出处：****[http://blog.csdn.net/harvic880925/article/details/50107951](http://blog.csdn.net/harvic880925/article/details/50107951)****谢谢**

**参考文章：**
[1、《 一个例子让你了解Java反射机制》](http://blog.csdn.net/ljphhj/article/details/12858767)
[2、《Java反射中Method类invoke方法的用法》](http://blog.csdn.net/cuiran/article/details/5302074)
[3、《【译】2. Java反射——Class对象》](http://www.cnblogs.com/penghongwei/p/3300047.html)(系列译文非常棒)
[4、《反射得到参数化类型中的类型参数》](http://blog.csdn.net/jiang_bing/article/details/7794365)
[5、《提炼Java Reflection》](http://lsy.iteye.com/blog/220264)
[6、《实例讲解：java reflection 反射机制》](http://tech.it168.com/a2011/0706/1214/000001214015_all.shtml)
[7、《Class.forname() 和 ClassLoader.loadClass()的区别 (转)》](http://www.cnblogs.com/bingoidea/archive/2011/03/29/1998927.html)
[8、《接口 ParameterizedType》](http://shouce.jb51.net/java/jdk1.5/java/lang/reflect/ParameterizedType.html)
[9、《ParameterizedType.getOwnerType() 函数怎么用?》](http://segmentfault.com/q/1010000000655631)
[10、《详解Java泛型type体系整理》](http://developer.51cto.com/art/201103/250028.htm)
[11、《Java类型中ParameterizedType，GenericArrayType，TypeVariabl，WildcardType详解》](http://www.webkfa.com/one7/w595.html)
[12、《谈谈Java类型中ParameterizedType，GenericArrayType，TypeVariabl》](http://www.aspku.com/kaifa/java/69576.html)
[13、《泛型与反射的使用总结之反射篇》](http://www.blogjava.net/bolo/archive/2014/05/06/413323.html)
[14、《Field，Method，Constructor— java 7 Reflection（三）》](http://peiquan.blog.51cto.com/7518552/1300259)
[15、《 Java 反射机制》](http://wiki.jikexueyuan.com/project/java-reflection/java-var.html)
[16、《试用Java中的反射reflect之getDeclaredMethods和getMethods》](http://www.cnblogs.com/jianjianjiao/articles/1853409.html)
[17、《Java Reflection 相关及示例》](http://www.2cto.com/kf/201412/365080.html)
[18、《 Android PopupWindow的使用和分析》](http://blog.csdn.net/dzz_dzz/article/details/46006133)
[19、《彻底搞懂Java ClassLoader》](http://weli.iteye.com/blog/1682625)
[20、《JVM学习笔记（二）------Java代码编译和执行的整个过程》](http://blog.csdn.net/cutesource/article/details/5904542)
[21、《Java程序编译和运行的过程》](http://allenhk.iteye.com/blog/1866288)
[22、《Java类加载/ClassLoader》](http://blog.csdn.net/fw0124/article/details/44037659)

版权声明：本文为博主原创文章，未经博主允许不得转载。

