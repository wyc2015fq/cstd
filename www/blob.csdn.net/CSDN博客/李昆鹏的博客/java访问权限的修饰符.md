
# java访问权限的修饰符 - 李昆鹏的博客 - CSDN博客


2018年03月06日 13:36:08[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：30



![](https://img-blog.csdn.net/20180306133224287)

Protected和 private（破染v特）是不能修饰类的，如下 代码
protected class Perm{
}
结果
![](https://img-blog.csdn.net/2018030613325466?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
private class Perm{
}
![](https://img-blog.csdn.net/20180306133324603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
四种访问权限修饰符的作用范围
在类上的权限修饰符我们一般都定义成public。
方法大多数是public的（使用一些设计模式如：单列模式 可能会使用protected 获得private）。
属性绝大多数都是private
访问权限修饰符对方法和属性的访问限制
|同类中
|同一个包
|没有父子类关系
|同一个包
|有父子类关系
|不同包
|有父子类关系
|不同包
|没有父子类关系
|
|private
|Y
|N
|N
|N
|N
|
|default
|Y
|Y
|Y
|N
|N
|
|Protected
|Y
|Y
|Y
|Y
|N
|
|public
|Y
|Y
|Y
|Y
|Y
|
//测试四个权限修饰符在同类中的访问权限，全部OK
public class Perm{
private voidprivateMethod(){
System.out.println("private修饰的方法可以被调用");
}
void defaultMethod(){
System.out.println("default修饰的方法可以被调用");
}
protected voidprotectedMethod(){
System.out.println("protected修饰的方法可以被调用");
}
public void publicMethod(){
System.out.println("public修饰的方法可以被调用");
}
//调用四个权限修饰符的方法
public static voidmain(String[] args){
Perm p = new Perm();
p.privateMethod();
p.defaultMethod();
p.protectedMethod();
p.publicMethod();
}
}
![](https://img-blog.csdn.net/20180306133351350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-----------------------------------------------------------------------------------------
package cn.tx;
//package(拍k哥)  测试四个权限修饰符在同包中没有父子类关系的访问权限
//private不能被访问编译错误
public class Perm1{
private voidprivateMethod(){
System.out.println("private修饰的方法可以被调用");
}
void defaultMethod(){
System.out.println("default修饰的方法可以被调用");
}
protected voidprotectedMethod(){
System.out.println("protected修饰的方法可以被调用");
}
public void publicMethod(){
System.out.println("public修饰的方法可以被调用");
}
}
------------------
package cn.tx;
//import(音跑特)同一个包中不需要引入
//package(拍k哥)  测试四个权限修饰符在同包中没有父子类关系的访问权限
//private不能被访问编译错误
public class TestPerm1{
//调用四个权限修饰符的方法
public static voidmain(String[] args){
Perm1 p = newPerm1();
p.privateMethod();
p.defaultMethod();
p.protectedMethod();
p.publicMethod();
}
}
![](https://img-blog.csdn.net/20180306133422746?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
当private方法被注掉后就可以成功编译了
![](https://img-blog.csdn.net/20180306133439274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180306133444311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注：在同一个包中有没有父子关系四个访问权限都是一样的，只有private不能访问
-------------------------------在不同包中有父子关系------------------------------------------------------------
package cn.tx;
//package(拍k哥)  测试四个权限修饰符在不同包中有父子类关系的访问权限
public class Perm2{
private void privateMethod(){
System.out.println("private修饰的方法可以被调用");
}
void defaultMethod(){
System.out.println("default修饰的方法可以被调用");
}
protected void protectedMethod(){
System.out.println("protected修饰的方法可以被调用");
}
public void publicMethod(){
System.out.println("public修饰的方法可以被调用");
}
}
-------------
package rl.tx;
//import(音跑特)同一个包中不需要引入
import cn.tx.Perm2;
//package(拍k哥)  测试四个权限修饰符在同包中有父子类关系的访问权限
//private和default不能被访问编译错误
public class SubPerm2extends Perm2{
//extends (一可四腾四)
//调用四个权限修饰符的方法
public static void main(String[] args){
SubPerm2 p = new SubPerm2();
//   p.privateMethod();
//   p.defaultMethod();
p.protectedMethod();
p.publicMethod();
}
}
![](https://img-blog.csdn.net/20180306133518128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
private和default不能被访问编译错误
----------------------------------------不同包没有父子类关系-------------------------------------------------------
package cn.tx;
//package(拍k哥)  测试四个权限修饰符在不同包中没有父子类关系的访问权限
public class Perm3{
private void privateMethod(){
System.out.println("private修饰的方法可以被调用");
}
void defaultMethod(){
System.out.println("default修饰的方法可以被调用");
}
protected void protectedMethod(){
System.out.println("protected修饰的方法可以被调用");
}
public void publicMethod(){
System.out.println("public修饰的方法可以被调用");
}
}
------------------
package rl.tx;
//import(音跑特)同一个包中不需要引入
import cn.tx.Perm3;
//package(拍k哥)  测试四个权限修饰符在同包中没有父子类关系的访问权限
//private 和 default 、 protected 不能被访问编译错误
public class SubPerm3 {
//extends (一可四腾四)
//调用四个权限修饰符的方法
public static void main(String[] args){
Perm3 p = new Perm3();
p.privateMethod();
p.defaultMethod();
p.protectedMethod();
p.publicMethod();
}
}
//private 和 default 、 protected 不能被访问编译错误
![](https://img-blog.csdn.net/20180306133541594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





