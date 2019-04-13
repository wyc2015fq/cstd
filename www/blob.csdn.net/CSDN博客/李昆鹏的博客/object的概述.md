
# object的概述 - 李昆鹏的博客 - CSDN博客


2018年03月06日 21:34:16[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：62


Object概述
Object是所有类的根类，所有的类都是直接或者间接的去继承object类。
类object是类层次结构的根类。每个类都使用object作为超类。所有对象（包括数组）都实现这个类的方法。
超类相当于祖师爷类0.0，超类和父类要区分。
根类是最顶上的类。
class Student {  //隐藏了extends Object{
privateString name;
privateint age;
privateint gender;
publicStudent(){
//隐藏了super();
}
publicvoid setName(String name){
this.name= name;
}
publicString getName(){
returnname;
}
publicvoid setAge(int age){
this.age= age;
}
publicint getAge(){
returnage;
}
publicvoid setGender(int gender){
this.gender= gender;
}
publicint getGender(){
returngender;
}
}
class ObjectDmeo{
public static void main(String[] args){
Students = new Student();
}
}
**隐藏和显示继承****object****都不会影响编译，构造器隐藏或者显示****super****（）也不影响编译。**
class ObjectDmeo{
public static void main(String[] args){
Students = new Student();
//使用学生的对象来调用父类继承下来的toString方法
Stringstr = s.toString();
System.out.println(str);
}
}
![](https://img-blog.csdn.net/20180306213304753?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Java.lang.Object****类是默认被每一个类继承的**
**toString**
publicString**toString**()
返回该对象的字符串表示。通常，toString 方法会返回一个“以文本方式表示”此对象的字符串。结果应是一个简明但易于读懂的信息表达式。建议所有子类都重写此方法。
Object 类的 toString 方法返回一个字符串，该字符串由类名（对象是该类的一个实例）、at 标记符“@”和此对象哈希码的无符号十六进制表示组成。换句话说，该方法返回一个字符串，它的值等于：
getClass().getName()+ '@' + Integer.toHexString(hashCode())
**返回：**
该对象的字符串表示形式。
**-------------------------------------------------------------------**
**hashCode**
public int**hashCode**()
返回该对象的哈希码值。支持此方法是为了提高哈希表（例如java.util.Hashtable 提供的哈希表）的性能。
hashCode 的常规协定是：
·        在 Java 应用程序执行期间，在对同一对象多次调用 hashCode 方法时，必须一致地返回相同的整数，前提是将对象进行 equals 比较时所用的信息没有被修改。从某一应用程序的一次执行到同一应用程序的另一次执行，该整数无需保持一致。
·        如果根据 equals(Object) 方法，两个对象是相等的，那么对这两个对象中的每个对象调用hashCode 方法都必须生成相同的整数结果。
·        如果根据equals(java.lang.Object)方法，两个对象不相等，那么对这两个对象中的任一对象上调用hashCode 方法*不*要求一定生成不同的整数结果。但是，程序员应该意识到，为不相等的对象生成不同整数结果可以提高哈希表的性能。
实际上，由 Object 类定义的 hashCode 方法确实会针对不同的对象返回不同的整数。（这一般是通过将该对象的内部地址转换成一个整数来实现的，但是 JavaTM编程语言不需要这种实现技巧。）
**返回：**
此对象的一个哈希码值。
**另请参见：**
equals(java.lang.Object),Hashtable
---

获得该对象的哈希码
class ObjectDmeo{
public static void main(String[]args){
Student s = new Student();
Student s1 = new Student();
//使用学生的对象来调用父类继承下来的toString方法，默认情况下我们认为Object的toString打印的就是对象的地址
String str = s.toString();
System.out.println(str);
//获得该对象的哈希码
int hashCode = s.hashCode();
System.out.println("Student  s对象的哈希码：" +hashCode);
int hashCode1 = s1.hashCode();
System.out.println("Student  s1对象的哈希码：" +hashCode1);
}
}
![](https://img-blog.csdn.net/20180306213325239?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**每一个对象对应的哈希码是不一样的**
**使用学生的对象来调用父类继承下来的toString****方法，默认情况下我们认为Object的toString打印的就是对象的地址**
Java1.9 toString方法源码查看 打开src.zip àjava.base à javaàlang àObject.java
找到toString方法
![](https://img-blog.csdn.net/20180306213333613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**源码中有****native****本地编程方法是****c****语言实现的**
**getClass().getName()****返回一个类的对象**
**Integer.toHexString(hashCode())****包中类中一个将十进制转换成十六进制的方法**
`toHexString``(int i)`
以十六进制（基数 16）无符号整数形式返回一个整数参数的字符串表示形式。
--------------------对toString和hashCode方法进行重写----------------------------------------------------------------
class Student1 {  //隐藏了extends Object{
private String name;
private int age;
private int gender;
public Student1(){
//隐藏了super();
}
public void setName(String name){
this.name = name;
}
public String getName(){
return name;
}
public void setAge(int age){
this.age = age;
}
public int getAge(){
return age;
}
public void setGender(int gender){
this.gender = gender;
}
public int getGender(){
return gender;
}
/**
* 重写Object中的toString方法
*/
public String toString(){
return this.name + "    " + this.age + "    " + this.gender;
}
/**
*重写Object中的hashCode方法
*不要随便重写hashCode方法，因为每一个对象返回的哈希码是不一样的
*/
public int hashCode(){
return 1;
}
}
class ObjectDmeo1{
public static void main(String[] args){
Student1 s = new Student1();
s.setName("张三");
s.setAge(10);
s.setGender(1);
//获得重写后的toString
String str = s.toString();
System.out.println(str);
//获得重写后的hashCode
int hashCode = s.hashCode();
System.out.println(hashCode);
}
}
![](https://img-blog.csdn.net/20180306213346862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


