
# java文件夹遍历包括子文件夹的遍历和删除 - 李昆鹏的博客 - CSDN博客


2018年04月04日 00:03:31[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：419


------------------------------java文件夹遍历包括子文件夹的遍历和删除--------------------------
递归就是方法调用方法自身，递归一定有条件跳出。
如何使用递归的计算方式计算阶乘
5！= 5*4*3*2*1
..
N!= n*(n-1)
例子：
**public****class**FileDemo8 {
**public****static****void**main (String[]args)**throws**IOException {
**int**result=*fn*(5);
System.**out**.println(result);
//输出结果为120

}
/**
* n!= n * (n-1)
*递归计算阶乘
*
*/
**public****static****int**fn(**int**n) {
**if**(n>= 2 ) {
//对方法自身的调用
**return**n**fn*(n-1);
}
**return**1;
}
}
----------------------------------------
练习：利用递归打印文件夹下所有的子文件。
package cn.tx.file;
import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
public class FileDemo9 {
//判断文件夹在第几层
public  static int level = 1;
public  static void main (String[] args) throws IOException {
File  file = new File("E:/李昆鹏/java/javaseWord文件");
//打印文件夹下所有文件包括子文件夹中的文件
printFile(file);
//输出结果
/*  2018年3月24日
java对象比较和TreeSet.docx
java文件夹遍历.docx
java文件构造器.docx
java的Array工具类.docx
java的Map介绍和添加.docx
java的Map的子类.docx
java的set集合和子类HashSet.docx
java的扑克牌测试题.docx
java的集合工具类.docx
~$va文件夹遍历.docx
~$va文件构造器.docx
~$va的Map的子类.docx
2018年3月24日前
Eclipse中debug的使用.docx
Eclipse代码自动生成.docx
Eclipse工程导入和删除.docx
Eclipse快捷键使用.docx
Eclipse的使用和异常.docx
Eclipse视图介绍.docx
java中包装类概述.docx
java中自动装箱、拆箱和包装类默认值问题.docx
java字符串判断方法.docx
java字符串概述.docx
java字符串的作业题字符串分解.docx
java字符串的特殊功能.docx
java字符串获取.docx
java异常分类和处理.docx
java权限修饰符.docx
java的Collection常用方法.docx
java的List实现类.docx
java的List集合.docx
java的StringBuffer可变字符串.docx
java的StringBuffer可变字符串的追加.docx
java的System类常用方法.docx
java的日历类.docx
java的日期类Date.docx
java的随机数.docx
java编译期的异常处理.docx
java迭代器.docx
java集合.docx
java集合中泛型的使用.docx
object的方法.docx
object的概述.docx
BAT面试题.txt
java飞机小项目
java飞机小项目总结.docx
java飞机小项目第一步建主窗口.docx
java飞机小项目第七步面向对象重构飞机类的键盘控制代码.docx
java飞机小项目第三步线程内部类实现动画.docx
java飞机小项目第九步矩形检测原理.docx
java飞机小项目第二步图形和文本绘制.docx
java飞机小项目第五步面向对象思想重构飞机类设计.docx
java飞机小项目第八步炮弹类设计_任意角度飞行.docx
java飞机小项目第六步键盘控制游戏物体原理.docx
java飞机小项目第十一步飞机死亡计时功能.docx
java飞机小项目第十步爆炸类_图片数组轮播处理 - 副本.docx
java飞机小项目第四步创建游戏物体根类的实现.docx
飞机小项目图片
imges
backdrop.jpg
boon1.jpg
boon10.jpg
boon11.jpg
boon12.jpg
boon13.jpg
boon14.jpg
boon2.jpg
boon3.jpg
boon4.jpg
boon5.jpg
boon6.jpg
boon7.jpg
boon8.jpg
boon9.jpg
bullet.jpg
plane.png
timg.jpg
简历介绍.docx
*/
}
/**
*
* 遍历文件夹下的所有文件
*
*/
public  static void printFile(File file) {
//判断文件是否是文件夹
if(!file.isDirectory()){
//返回值是void的时候，方法中的return代表程序结束
return;
}
if(!file.exists()){
return;
}
level++;
//遍历文件夹内部的文件
File[]  files = file.listFiles();
for(Filef : files) {
//打印缩进
for(int i = 0; i < level; i++) {
System.out.print("\t");
}
//遍历文件
//System.out.println(f);
//遍历文件名
String  name = f.getName();
System.out.println(name);
//判断是否是文件夹
if(f.isDirectory()){
//调用递归打印文件夹中的文件夹
printFile(f);
}
}
level--;
}
}
-----------------------------------------------
练习： 如何删除一个文件夹下的所有文件（不包括目录）？
**public****class**FileDemo10 {
**public****static****void**main (String[]args)**throws**IOException {
Filefile=**new**File("aa");
//删除文件夹下所有文件包括子文件夹中的文件(不包括文件夹)
*deleteFile*(file);
}
/**
*
*遍历文件夹下的所有文件
*
*/
**public****static****void**deleteFile(Filefile) {
//判断文件是否是文件夹
**if**(!file.isDirectory()) {
//返回值是void的时候，方法中的return代表程序结束
**return**;
}
**if**(!file.exists()) {
**return**;
}
//遍历文件夹内部的文件
File[]files=file.listFiles();
**for**(Filef:files) {
//判断是否是文件
**if**(f.isFile()) {
//删除文件
f.delete();
}**else**{
//如果是文件夹就调用递归来对文件夹中的文件进行删除
*deleteFile*(f);
}
}
}
}
---------------------------------------------------
练习：如何删除一个文件夹下所有的文件（包括目录）

**public****class**FileDemo10 {
**public****static****void**main (String[]args)**throws**IOException {
Filefile=**new**File("aa");
//删除文件夹下所有文件包括子文件夹中的文件(包括文件夹)
*deleteFile*(file);
}
/**
*
*遍历文件夹下的所有文件
*
*/
**public****static****void**deleteFile(Filefile) {
//判断文件是否是文件夹
**if**(!file.isDirectory()) {
//返回值是void的时候，方法中的return代表程序结束
file.delete();
**return**;
}
**if**(!file.exists()) {
**return**;
}
//遍历文件夹内部的文件
File[]files=file.listFiles();
**for**(Filef:files) {
//判断是否是文件
**if**(!f.isFile()) {
//如果不是文件就调用递归删除
*deleteFile*(f);
}
//删除文件
f.delete();
}
}
}



