# Map拷贝 关于对象深拷贝 浅拷贝的问题 - z69183787的专栏 - CSDN博客
2017年01月23日 17:06:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4445
问题：map拷贝时发现数据会变化。
**高能预警，你看到的下面的栗子是不正确的，后面有正确的一种办法，如果需要看的话的，请看到底，感谢各同学的提醒，已做更正，一定要看到最后**
     先看例子：
**public****class**CopyMap
 {
/**
       ***@author**张仲华
       ***@param**args
       * 2014-8-6
 上午9:29:33
       */
**public****static****void**main(String[]
 args) {
            Map<String,Integer> map =**new**HashMap<String,Integer>();
            map.put("key1",
 1);
            Map<String,Integer>mapFirst=
 map;
            System.*out*.println(mapFirst);
            map.put("key2",
 2);
            System.*out*.println(mapFirst);
      }
}
上面程序的期望输出值是，mapFrist的值均为1，
但是实际上输出结果为：
{key1=1}
{key2=2, key1=1}
这里是因为map发生了浅拷贝，mapFirst只是复制了map的引用，和map仍使用同一个内存区域，所以，在修改map的时候，mapFirst的值同样会发生变化。
PS：
所谓浅复制：则是只复制对象的引用，两个引用仍然指向同一个对象，在内存中占用同一块内存。被复制对象的所有变量都含有与原来的对象相同的值，而所有的对其他对象的引用仍然指向原来的对象。换言之，浅复制仅仅复制所考虑的对象，而不复制它所引用的对象。
          深复制：被复制对象的所有变量都含有与原来的对象相同的值，除去那些引用其他对象的变量。那些引用其他对象的变量将指向被复制过的新对象，而不再是原有的那些被引用的对象。换言之，深复制把要复制的对象所引用的对象都复制了一遍。
如何解决？
     使用深拷贝，拷贝整个对象，而非引用
     Map中有个方法叫做putAll方法，可以实现深拷贝，如下：
**public****class**CopyMap
 {
/**
       * **@author**张仲华
       * **@param**args
       * 2014-8-6
 上午9:29:33
       */
**public****static****void** main(String[]
 args) {
            Map<String,Integer> map = **new** HashMap<String,Integer>();
            map.put("key1",
 1);
            Map<String,Integer> mapFirst = **new** HashMap<String,Integer>();
            mapFirst.putAll(map);//深拷贝
            System.*out*.println(mapFirst);
            map.put("key2",
 2);
            System.*out*.println(mapFirst);
      }
}
如上，输出结果为：
{key1=1}
{key1=1}
参考：http://blog.csdn.net/lzkkevin/article/details/6667958
**注意！！！注意！！！！注意！！！ 上面并不是深拷贝，留下来的原因是提醒大家，这里是存在错误的。（很高兴你看到这里了）**
感谢下面这几位朋友的提醒。
![QQ20160318-0.png](http://www.dutycode.com/content/uploadfile/201603/thum-f6331458313173.png)![QQ20160318-1.png](http://www.dutycode.com/content/uploadfile/201603/thum-2e061458313176.png)
 文章更正如下：
如何实现Map的深拷贝呢？
有一种方法，是使用序列化的方式来实现对象的深拷贝，但是前提是，对象必须是实现了Serializable接口才可以，Map本身没有实现 Serializable 这个接口，所以这种方式不能序列化Map，也就是不能深拷贝Map。但是HashMap是可以的，因为它实现了 Serializable。下面的方式，基于HashMap来讲，非Map的拷贝。
具体实现如下：
|`01`|`public``class````CloneUtils {```|
|`02`||
|`03`|`    ``@SuppressWarnings``(``"unchecked"``)`|
|`04`|`    ``public``static``<T ``extends````Serializable> T clone(T obj){```|
|`05`|`        `|
|`06`|`        ````T clonedObj =````null``;`|
|`07`|`        ``try``{`|
|`08`|`            ````ByteArrayOutputStream baos =````new``ByteArrayOutputStream();`|
|`09`|`            ````ObjectOutputStream oos =````new``ObjectOutputStream(baos);`|
|`10`|`            ``oos.writeObject(obj);`|
|`11`|`            ``oos.close();`|
|`12`|`            `|
|`13`|`            ````ByteArrayInputStream bais =````new``ByteArrayInputStream(baos.toByteArray());`|
|`14`|`            ````ObjectInputStream ois =````new``ObjectInputStream(bais);`|
|`15`|`            ````clonedObj = (T) ois.readObject();```|
|`16`|`            ``ois.close();`|
|`17`|`            `|
|`18`|`        ``}``catch````(Exception e){```|
|`19`|`            ``e.printStackTrace();`|
|`20`|`        ``}`|
|`21`|`        `|
|`22`|`        ``return``clonedObj;`|
|`23`|`    ``}`|
|`24`|`}`|
如何使用呢，下面是个使用的例子，同时证明了Map的putAll方法并没有实现深拷贝，putAll仅对基本数据类型起到深拷贝的作用。
栗子：
|`01`|`public``static``void````main(String[] args) {```|
|`02`|`        `|
|`03`|`        ````List<Integer> list =````new``ArrayList<Integer>();`|
|`04`|`        ``list.add(``100``);`|
|`05`|`        ``list.add(``200``);`|
|`06`|`        `|
|`07`|`        ````HashMap<String,Object> map =````new``HashMap<String,Object>();`|
|`08`|`        ``//放基本类型数据`|
|`09`|`        ``map.put(``"basic"``, ``100``);`|
|`10`|`        ``//放对象`|
|`11`|`        ``map.put(``"list"````, list);```|
|`12`|`        `|
|`13`|`        ````HashMap<String,Object> mapNew =````new``HashMap<String,Object>();`|
|`14`|`        ``mapNew.putAll(map);`|
|`15`|`        `|
|`16`|`        ``System.out.println(``"----数据展示-----"``);`|
|`17`|`        ``System.out.println(map);`|
|`18`|`        ``System.out.println(mapNew);`|
|`19`|`        `|
|`20`|`        ``System.out.println(``"----更改基本类型数据-----"``);`|
|`21`|`        ``map.put(``"basic"``, ``200``);`|
|`22`|`        ``System.out.println(map);`|
|`23`|`        ``System.out.println(mapNew);`|
|`24`|`        `|
|`25`|`        ``System.out.println(``"----更改引用类型数据-----"``);`|
|`26`|`        ``list.add(``300``);`|
|`27`|`        ``System.out.println(map);`|
|`28`|`        ``System.out.println(mapNew);`|
|`29`|`        `|
|`30`|`        `|
|`31`|`        ``System.out.println(``"----使用序列化进行深拷贝-----"``);`|
|`32`|`        ````mapNew = CloneUtils.clone(map);```|
|`33`|`        ``list.add(``400``);`|
|`34`|`        ``System.out.println(map);`|
|`35`|`        ``System.out.println(mapNew);`|
|`36`|`        `|
|`37`|`    ``}`|
输出结果如下：
![QQ20160318-2.png](http://www.dutycode.com/content/uploadfile/201603/a5e01458313523.png)
最上面的两条是原始数据，使用了putAll方法拷贝了一个新的mapNew对象，
中间两条，是修改map对象的基本数据类型的时候，并没有影响到mapNew对象。
但是看倒数第二组，更改引用数据类型的时候，发现mapNew的值也变化了，所以putAll并没有对map产生深拷贝。
最后面是使用序列化的方式，发现，更改引用类型的数据的时候，mapNew对象并没有发生变化，所以产生了深拷贝。
上述的工具类，可以实现对象的深拷贝，不仅限于HashMap，前提是实现了Serlizeable接口。
还没有看putAll的源码实现，后面看下为什么不能实现深拷贝。
