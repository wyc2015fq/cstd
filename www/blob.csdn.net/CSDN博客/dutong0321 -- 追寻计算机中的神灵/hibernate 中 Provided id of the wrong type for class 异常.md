# hibernate 中 Provided id of the wrong type for class 异常 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2017年11月20日 17:04:25[dutong0321](https://me.csdn.net/dutong0321)阅读数：1932
今天在工作中遇到了一个问题，在hibernate的使用过程中抛出了个异常：
`HTTP Status 500 - Provided id of the wrong type for class com.s_football.pojo.UserBean. Expected: class java.lang.Integer, got class java.lang.String; nested exception is org.hibernate.TypeMismatchException: Provided id of the wrong type for class com.s_football.pojo.UserBean. Expected: class java.lang.Integer, got class java.lang.String`
收集资料后发现：在HibernateTemplate中的get方法仅仅可以查找主键，不可以查找其他字段。其实是自己没有考虑好，在开始的时候自己还有过这样的疑问：get来查找是怎么对应字段，而且查找出来只是单个的，并不是个List。但是没有多想，现在用get来查找其他字段就抛出了了这个异常。 
其实看到这个异常就可以知道，肯定是类型错误，像我这个就是String类型转int类型了（在使用get方法中）。 
所以，检查一下类型匹配就可以解决了，包括是不是也用get方法查找其他字段了（其实也是类型匹配的问题）。
