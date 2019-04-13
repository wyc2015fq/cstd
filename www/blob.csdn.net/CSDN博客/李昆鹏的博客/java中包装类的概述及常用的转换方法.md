
# java中包装类的概述及常用的转换方法 - 李昆鹏的博客 - CSDN博客


2018年03月16日 17:25:26[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：79


**-------------------------java中包装类的概述---------------------------**
**包装类**
**把基本数据类型转换成类，我们使用基本数据类型做进制转换很麻烦，对于临界值也不好判断，我们的包装类提供了很多方法供我们使用，这样会方便很多。**
Integer类在对象中包装了一个基本类型int的值。
|构造方法摘要
|
|Integer|(int value)
|构造一个新分配的|Integer|对象，它表示指定的|int|值。
|
|Integer|(|String|s)
|构造一个新分配的|Integer|对象，它表示|String|参数所指示的|int|值。
|
|字段摘要
|
|static int
|MAX_VALUE
|值为 2|31|－1 的常量，它表示|int|类型能够表示的最大值。
|
|static int
|MIN_VALUE
|值为 －2|31|的常量，它表示|int|类型能够表示的最小值。
|
|static int
|SIZE
|用来以二进制补码形式表示|int|值的比特位数。
|
|static|Class|<|Integer|>
|TYPE
|表示基本类型|int|的|Class|实例。
|
|方法摘要
|
|static int
|bitCount|(int i)
|返回指定|int|值的二进制补码表示形式的 1 位的数量。
|
|byte
|byteValue|()
|以|byte|类型返回该|Integer|的值。
|
|int
|compareTo|(|Integer|anotherInteger)
|在数字上比较两个|Integer|对象。
|
|static|Integer
|decode|(|String|nm)
|将|String|解码为|Integer|。
|
|double
|doubleValue|()
|以|double|类型返回该|Integer|的值。
|
|boolean
|equals|(|Object|obj)
|比较此对象与指定对象。
|
|float
|floatValue|()
|以|float|类型返回该|Integer|的值。
|
|static|Integer
|getInteger|(|String|nm)
|确定具有指定名称的系统属性的整数值。
|
|static|Integer
|getInteger|(|String|nm, int val)
|确定具有指定名称的系统属性的整数值。
|
|static|Integer
|getInteger|(|String|nm,|Integer|val)
|返回具有指定名称的系统属性的整数值。
|
|int
|hashCode|()
|返回此|Integer|的哈希码。
|
|static int
|highestOneBit|(int i)
|返回具有至多单个 1 位的|int|值，在指定的|int|值中最高位（最左边）的 1 位的位置。
|
|int
|intValue|()
|以|int|类型返回该|Integer|的值。
|
|long
|longValue|()
|以|long|类型返回该|Integer|的值。
|
|static int
|lowestOneBit|(int i)
|返回具有至多单个 1 位的|int|值，在指定的|int|值中最低位（最右边）的 1 位的位置。
|
|static int
|numberOfLeadingZeros|(int i)
|在指定|int|值的二进制补码表示形式中最高位（最左边）的 1  位之前，返回零位的数量。
|
|static int
|numberOfTrailingZeros|(int i)
|返回指定的|int|值的二进制补码表示形式中最低（“最右边”）的为 1 的位后面的零位个数。
|
|static int
|parseInt|(|String|s)
|将字符串参数作为有符号的十进制整数进行解析。
|
|static int
|parseInt|(|String|s, int radix)
|使用第二个参数指定的基数，将字符串参数解析为有符号的整数。
|
|static int
|reverse|(int i)
|返回通过反转指定|int|值的二进制补码表示形式中位的顺序而获得的值。
|
|static int
|reverseBytes|(int i)
|返回通过反转指定|int|值的二进制补码表示形式中字节的顺序而获得的值。
|
|static int
|rotateLeft|(int i, int distance)
|返回根据指定的位数循环左移指定的|int|值的二进制补码表示形式而得到的值。
|
|static int
|rotateRight|(int i, int distance)
|返回根据指定的位数循环右移指定的|int|值的二进制补码表示形式而得到的值。
|
|short
|shortValue|()
|以|short|类型返回该|Integer|的值。
|
|static int
|signum|(int i)
|返回指定|int|值的符号函数。
|
|static|String
|toBinaryString|(int i)
|以二进制（基数 2）无符号整数形式返回一个整数参数的字符串表示形式。
|
|static|String
|toHexString|(int i)
|以十六进制（基数 16）无符号整数形式返回一个整数参数的字符串表示形式。
|
|static|String
|toOctalString|(int i)
|以八进制（基数 8）无符号整数形式返回一个整数参数的字符串表示形式。
|
|String
|toString|()
|返回一个表示该|Integer|值的|String|对象。
|
|static|String
|toString|(int i)
|返回一个表示指定整数的|String|对象。
|
|static|String
|toString|(int i, int radix)
|返回用第二个参数指定基数表示的第一个参数的字符串表示形式。
|
|static|Integer
|valueOf|(int i)
|返回一个表示指定的|int|值的|Integer|实例。
|
|static|Integer
|valueOf|(|String|s)
|返回保存指定的|String|的值的|Integer|对象。
|
|static|Integer
|valueOf|(|String|s, int radix)
|返回一个|Integer|对象，该对象中保存了用第二个参数提供的基数进行解析时从指定的|String|中提取的值。
|
例子：
**public****class**Test2 {
**public****static****void**main(String[]args) {
**byte**b= 1;
Byteb1=**new**Byte("15");
System.**out**.println(b1);
//输出int的最大值
System.**out**.println(Integer.**MAX_VALUE**);
//Integer的构造器
Integeri1=**new**Integer(12);
Integeri2=**new**Integer("56");
System.**out**.println(i1+"\t"+i2);
//int基本数据类型和Integer包装类转换
**int**i= 10;
//int--》Integer
IntegeriClass=**new**Integer(i);
//Integer --》int
i=iClass.intValue();
System.**out**.println(i);
//String-->Integer
//String一定要是数值类型，不能是空。
//此方法最为常用
Stringstr="90";
Integeri3=**new**Integer(str);
System.**out**.println(i3);
//可以把任何类型转换成Integer类型
Integeri4=Integer.*valueOf*(str);
//String--》int
**int**i5=Integer.*parseInt*(str);
//Integer--> String
Integeri6=**new**Integer(9);
Stringstri6=i6.toString();
Stringstri7=i6+"";
Stringstri8= String.*valueOf*(i6);
}
}

