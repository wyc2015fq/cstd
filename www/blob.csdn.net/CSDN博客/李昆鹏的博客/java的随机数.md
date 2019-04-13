
# java的随机数 - 李昆鹏的博客 - CSDN博客


2018年03月16日 16:06:46[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：31标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


**----------------------------java的随机数------------------------------**
**Random随机数产生类**
**在Math类中也有一个产生0-1之间的小数随机数，Math.Random();**
## java.util
## 类 Random
java.lang.Object**java.util.Random****所有已实现的接口：**
Serializable
**直接已知子类：**
SecureRandom
---

public class**Random**extendsObjectimplementsSerializable此类的实例用于生成伪随机数流。此类使用 48 位的种子，使用线性同余公式 (linear congruential form) 对其进行了修改（请参阅Donald Knuth 的*The Art of Computer Programming,Volume 3*，第 3.2.1 节）。
如果用相同的种子创建两个`Random`实例，则对每个实例进行相同的方法调用序列，它们将生成并返回相同的数字序列。为了保证此属性的实现，为类`Random`指定了特定的算法。为了 Java 代码的完全可移植性，Java 实现必须让类`Random`使用此处所示的所有算法。但是允许`Random`类的子类使用其他算法，只要其符合所有方法的常规协定即可。
`Random`类实现的算法使用一个`protected`实用工具方法，每次调用它最多可提供 32 个伪随机生成的位。
很多应用程序会发现`Math.random()`方法更易于使用。
**1.  Random的构造器**
|构造方法摘要
|
|Random|()
|创建一个新的随机数生成器。
|
|Random|(long seed)
|使用单个|long|种子创建一个新的随机数生成器。
|
|方法摘要
|
|protected  int
|next|(int bits)
|生成下一个伪随机数。
|
|boolean
|nextBoolean|()
|返回下一个伪随机数，它是取自此随机数生成器序列的均匀分布的|boolean|值。
|
|void
|nextBytes|(byte[] bytes)
|生成随机字节并将其置于用户提供的 byte 数组中。
|
|double
|nextDouble|()
|返回下一个伪随机数，它是取自此随机数生成器序列的、在|0.0|和|1.0|之间均匀分布的|double|值。
|
|float
|nextFloat|()
|返回下一个伪随机数，它是取自此随机数生成器序列的、在|0.0|和|1.0|之间均匀分布的|float|值。
|
|double
|nextGaussian|()
|返回下一个伪随机数，它是取自此随机数生成器序列的、呈高斯（“正态”）分布的|double|值，其平均值是|0.0|，标准差是|1.0|。
|
|int
|nextInt|()
|返回下一个伪随机数，它是此随机数生成器的序列中均匀分布的|int|值。
|
|int
|nextInt|(int n)
|返回一个伪随机数，它是取自此随机数生成器序列的、在 0（包括）和指定值（不包括）之间均匀分布的|int|值。
|
|long
|nextLong|()
|返回下一个伪随机数，它是取自此随机数生成器序列的均匀分布的|long|值。
|
|void
|setSeed|(long seed)
|使用单个|long|种子设置此随机数生成器的种子。
|
例子：
**public****class**Test2 {
**public****static****void**main(String[]args) {
*printRandomSeed*();
}
**public****static****void**printRandom() {
//创建随机数对象
Randomr=**new**Random();
//获得随机整数,会获得负数
**int**intVal=r.nextInt();
System.**out**.println(intVal);
**for**(**int**i= 0;i< 10;i++) {
intVal=r.nextInt(10);
System.**out**.println(intVal);
}
}
**public****static****void**printRandomSeed() {
//创建随机数对象,带种子的构造器，每一个种子对应一个随机数
Randomr=**new**Random(99);
//获得随机整数,会获得负数
**int**intVal=r.nextInt();
System.**out**.println(intVal);
**for**(**int**i= 0;i< 10;i++) {
//生成0-9之间的随机数
intVal=r.nextInt(10);
System.**out**.print(intVal+",");
}
}
}
结果：
-1192035722
8,9,1,0,2,4,6,0,9,8,

