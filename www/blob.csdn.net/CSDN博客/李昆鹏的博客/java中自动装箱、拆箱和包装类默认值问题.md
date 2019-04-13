
# java中自动装箱、拆箱和包装类默认值问题 - 李昆鹏的博客 - CSDN博客


2018年03月16日 18:15:49[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：170


**----------------java中自动装箱、拆箱和包装类默认值问题-------------**
**装箱：**
**基本数据类型转换成包装类就是装箱****(jdk1.5****以后的特性，自动完成****)****。**
**拆箱：**
**把包装类转换成基本数据类型就是拆箱****(jdk1.5****以后的特性，自动完成****)****。**
**int****和****Integer****之间的区别：**
**int****是基本数据类型，****Integer****是包装类。**
**包装类中提供了很多对整数的操作方法。**
**int****和****Integer****之间可以自动装箱和拆箱。**
**int****的默认值是****0****。**
**Intege****的默认值是****null**。
**包装类在使用之前要判断是否为空。**
**例子：**
**public****class**Test4 {
**public****static****void**main(String[]args) {
Personp=**new**Person();
System.**out**.println(p.getId());
System.**out**.println(p.getAge());
//对象在使用之前要判断是否是空
Integeri=p.getAge()+ 10 ;
}
}
**结果：**
0
null
Exception in thread"main" null
java.lang.NullPointerException
at cn.tx.system.Test4.main(Test4.java:19)


