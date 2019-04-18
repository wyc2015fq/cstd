# 关于byte[]转String 和 String 转byte[]自己的理解 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年02月01日 00:52:34[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：724


**byte[]转String**

byte bytes[]= {"120","121","122","123"};
String str = new String(bytes);

System.out.println(str);

byte元素是由8位 2进制组成的，字符串内部实际上是char[]是由4位16进制 组成的。 2位的16进制的数就可以完全对应8位的二进制的数。所以

当String str = new String(bytes);  会将bytes[0]中的120对应的8位二进制数“01111000” 转为 两位 16进制数 即 “7”和"8"

然后调用System.out.println(str); 时 将ASCII表中 对应的 元素输出 即 "X" 。 其他索引位的元素皆如此。

**String 转byte[]**

String str = "数据类型转换";

str.getBytes("UTF-8");      //UTF-8 将一个汉字  转换为 3组 byte类型数据分别存入bytes数组的每个索引位中。

str.getBytes();      //我的项目 编码格式 为 GBK   ，GBK 将一个汉字转为 2组 byte 类型数组。

![](https://img-blog.csdn.net/20180201005152672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



