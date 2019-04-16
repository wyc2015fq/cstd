# 面向对象(三)——static关键字 - PeterBishop - CSDN博客





2018年09月29日 14:32:32[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：22








### 首先来看看main方法的写法

### public static void main(String[] args){

### }

### 可以很明显的看到main方法里有一个关键字叫static，这节课我们就来讲讲static关键字

### static，中文翻译为静态的

### 被static修饰的成员变量叫静态变(或者叫静态属性）

### 被static修饰的方法叫静态方法

### 那么静态到底是什么意思呢？

### 静态还有个说法叫全局，意味可以在任意位置访问

### 静态变量和方法为类本身所共有，可以直接通过类名.静态变量或者类名.静态方法(参数)访问

### 当然，类的对象也可以访问本类的静态方法，引用引用对象.静态方法

### 注意:

### 静态方法中不能访问非静态属性或非静态方法

###        无法通过对象访问静态属性

### Demo:

### public class Demo {    public static int a = 0;    public static void add(int a)    {        Demo.a += a;    }    public static void main(String[] args) {        Demo.add(1);        System.out.println(Demo.a);        Demo demo = new Demo();        demo.add(2);        System.out.println(Demo.a);    }}

### 运行结果:

### 1

### 3

### 解释:

###        静态变量/静态方法为整个类所共有，这个类的所有对象包括类本身共享同一个静态变量/静态方法



