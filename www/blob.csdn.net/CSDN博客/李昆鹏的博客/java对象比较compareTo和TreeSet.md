
# java对象比较compareTo和TreeSet - 李昆鹏的博客 - CSDN博客


2018年03月25日 09:25:04[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：236


**-----------------------java对象比较和TreeSet-----------------------**
**----------------LinkedHashSet----------------**
**特点：**
**1.****元素唯一性**
**2.****有序性**
**3.****允许null****存在一个**
**4.****不是线程安全、不是同步的（效率高）**
**--------------TreeSet-------------------------**
**TreeSet 可以支持自定义排序，如果TreeSet所存储的对象的类没有实现Comparable接口就会报错ClassCastException。所以我们如果想要使用TreeSet来自定义的对象来排序必须实现Comparable接口。**
**特点：**
**1.****元素唯一性**
**2.****可自定义排序的（自动做自然排序）**
**3.****不允许****null****存在**
**4.****不是线程安全**
**排序：**
**类必须要实现一个接口****Comparable<T>****才能做排序。**
**例子：**
**创建一个****Sudent****类，重写父类的****compareTo****方法**
**package**cn.tx.set;
**public****class**Student**implements**Comparable<Student>{
**private****int**stuId;
**private**Stringname;
**private****int**age;
**public****int**getStuId() {
**return**stuId;
}
**public****void**setStuId(**int**stuId) {
**this**.stuId=stuId;
}
**public**String getName() {
**return**name;
}
**public****void**setName(Stringname) {
**this**.name=name;
}
**public****int**getAge(){
**return**age;
}
**public****void**setAge(**int**age) {
**this**.age=age;
}
**public**Student(**int**stuId, Stringname,**int**age) {
**super**();
**this**.stuId=stuId;
**this**.name=name;
**this**.age=age;
}
@Override
**public**String toString() {
**return**"Student[stuId="+stuId+", name="+name+",age="+age+"]";
}
/**
*实现两个学生的对象比较
*比较规则：
*如果先比较学号，按照学号的升序排列
*如果学号相等，就按照年龄比较，年龄小的在前面
*如果学号和年龄都相等，就按照名字的字符串来比较
*/
@Override
**public****int**compareTo(Studento) {
//先比较学号
**int**val=**this**.stuId-o.stuId;
**if**(val== 0)
//如果学号相等比较年龄
val=**this**.age-o.age;
**if**(val== 0) {
//如果学号和年龄都相等比较名字的字符串
**if**(**this**.name!=**null**&&o.name!=**null**)
val=**this**.name.compareTo(o.name);
}
**return**val;
}

}
**-------------------------------------------**
**测试****TreeSet****集合的自然排序**
**例子：**
**public****class**TreeSetDemo {
**public****static****void**main(String[]args) {
//创建一个TreeSet集合
Set<String>set=**new**TreeSet<String>() ;
set.add("b");
set.add("a");
set.add("h");
set.add("j");
set.add("aa");
System.**out**.println(set);
//创建一个Person泛型的TreeSet集合
Set<Student>set1=**new**TreeSet<Student>() ;
set1.add(**new**Student(2,"李昆鹏2", 22));
set1.add(**new**Student(3,"李昆鹏3", 23));
set1.add(**new**Student(1,"李昆鹏1", 21));
System.**out**.println(set1);
}
}
结果：
[a, aa, b, h, j]
[Student [stuId=1,name=李昆鹏1, age=21], Student [stuId=2, name=李昆鹏2, age=22], Student [stuId=3, name=李昆鹏3, age=23]]

**-----------------------****可变参数****-------------------------**
**可变参数的语法：**
**修饰符****返回值类型****方法名（数据类型****…****变量）****{**
**}**
**例子：**
**public static int sum(int… p){**
**return 0;**
**}**
**注意：参数****p****实际是一个数组，****p****都是同一种类型**
**public****class**DymanicParam {
**public****static****void**main(String[]args) {
**int**[]p={1,2,3,4,5};
System.**out**.println(*add*(p));;;;;;
}
**public****int**add(**int**a,**int**b) {
**return**a+b;
}
**public****int**add(**int**a,**int**b,**int**c) {
**return**add(a,b) +c;
}
/**
*可变参数的定义
*int...p参数是一个int类型的数组，相当于int[] p
*/
**public****static****int**add(**int**...p) {
**int**sum= 0;
**for**(**int**i= 0;i<p.length;i++) {
sum+=p[i];
}
**return**sum;
}
}
结果：
15


