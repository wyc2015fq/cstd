# ReflectASM，高性能的反射 - z69183787的专栏 - CSDN博客
2016年06月13日 14:59:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3960
什么是ReflectASM   
 ReflectASM是一个很小的java类库，主要是通过asm生产类来实现java反射，执行速度非常快，看了网上很多和反射的对比，觉得ReflectASM比较神奇，很想知道其原理，下面介绍下如何使用及原理。
如何使用
Java代码  ![收藏代码](http://xiaoqiangcn.iteye.com/images/icon_star.png)
- publicclass User {  
- privateint id;  
- private String name;  
- 
- publicint getId() {  
- return id;  
-     }  
- 
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- }  
- 
- 
- 
- publicclass ReflectAsmTest {  
- publicstaticvoid main(String[] args) {  
-         User user = new User();  
- //使用reflectasm生产User访问类
-         MethodAccess access = MethodAccess.get(User.class);  
- //invoke setName方法name值
-         access.invoke(user, "setName", "张三");  
- //invoke getName方法 获得值
-         String name = (String)access.invoke(user, "getName", null);  
-         System.out.println(name);  
-     }  
- }  
输出： 
张三 
原理
   上面代码的确实现反射的功能，代码主要的核心是 MethodAccess.get(User.class); 
看了下源码，这段代码主要是通过asm生产一个User的处理类 UserMethodAccess（这个类主要是实现了invoke方法）的ByteCode，然后获得该对象，通过上面的invoke操作user类。 
MethodAccess反编辑源码 
Java代码  ![收藏代码](http://xiaoqiangcn.iteye.com/images/icon_star.png)
- // Decompiled by Jad v1.5.8g. Copyright 2001 Pavel Kouznetsov.
- // Jad home page: http://www.kpdus.com/jad.html
- // Decompiler options: packimports(3) 
- 
- package com.johhny.ra;  
- 
- import com.esotericsoftware.reflectasm.MethodAccess;  
- 
- // Referenced classes of package com.johhny.ra:
- //            User
- 
- publicclass UserMethodAccess extends MethodAccess  
- {  
- 
- public UserMethodAccess()  
-     {  
-     }  
- 
- /**
-      * 这个方法是主要是实现了MethodAccess 的抽象方法，来实现反射的功能  
-      * @param obj  需要反射的对象
-      * @param i  class.getDeclaredMethods 对应方法的index
-      * @param 参数对象集合
-      * @return
-      */
- publictransient Object invoke(Object obj, int i, Object aobj[])  
-     {  
-         User user = (User)obj;  
- switch(i)  
-         {  
- case0: // '\0'
- return user.getName();  
- 
- case1: // '\001'
- return Integer.valueOf(user.getId());  
- 
- case2: // '\002'
-             user.setName((String)aobj[0]);  
- returnnull;  
- 
- case3: // '\003'
-             user.setId(((Integer)aobj[0]).intValue());  
- returnnull;  
-         }  
- thrownew IllegalArgumentException((new StringBuilder("Method not found: ")).append(i).toString());  
-     }  
- }  
： 
看了UserMethodAccess源码后明白 ReflectASM 为什么会比java放射快那么多，其实就是我们的bean调用里面的方法，速度当然很快 
注意：
    1. MethodAccess.get()方法比较耗时的,特别是类方法比较多的时候，如果生成的反射类用到的地方比较多或者会多次调用，建议缓存下来，如果使用次数很少建议还是使用反射来完成功能 
