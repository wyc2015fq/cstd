# Class类文件的结构

 

class文件是一组以8位字节为基础的二进制流，各个数据项目严格按照顺序紧凑地排列在class文件中，中间没有任何分隔符，这点和png、jpg等图片文件格式类似。当遇到需要占用8位字节以上空间的数据项时，则会按照一定的字节顺序分隔为若干个8位字节进行存储。

Java虚拟机规范规定class文件格式采用一种类似于C语言结构体的伪结构来存储数据，这种伪结构只有两种数据类型：**无符号数**和**表**。

> - 无符号数属于基本的数据类型，以u1、u2、u4、u8来分别代表1个字节、2个字节、4个字节和8个字节。 
>      无符号数可以用来描述数字、索引引用、数量值或按照utf-8编码构成的字符串值。
> - 表是由多个无符号数或其他表构成的复合数据结构，所有的表都以“_info”结尾。

这里需要重点说明下：

> Class文件不像XML等描述性语言存在某种辅助性语法说明，因此Class文件中没有任何分割符号，即Class文件格式所示例的数据项，无论是顺序还是数量，甚至数据存储的字节序(Big-Endian)等细节全部都是被严格限定的。 
>    哪个字节代表什么含义，长度是多少，先后顺序如何，都不允许改变。

但也正是因为是被限定死的，所以Class文件的解析就简单的变成了**数数字解析**

# Class文件结构

Class文件结构概要见[Class文件格式](##Class文件格式)

## 魔数

class文件的头4个字节。

## 主次版本号

魔数后4个字节，第5个和第6个字节表示次版本号（Minor Version），第7个和第8个字节表示主版本号（Major Version）。

## 常量池总数

主次版本号后2个字节，表明常量池中常量的个数。

## 常量池元素

常量池总数后开始进入具体的常量，每个常量的第1个字节表示常量类型，然后根据常量类型查询具体常量的结构，从而知道该常量所占字节大小。

## 访问标志

所有常量池元素访问完成后，其后面的2个字节表示访问标志，访问标志是不同访问标志值的`按位取与`的值。参考[访问标志及标志含义](https://blog.csdn.net/u013595419/article/details/79334516#t14)

## 类索引、父类索引、接口索引

在访问标志完成后，其后面的2个字节表示类索引。

类索引又指向一个类型为`CONSTANT_Class_info`的类描述符号常量（该常量在[常量池元素](https://blog.csdn.net/u013595419/article/details/79334516#t4)一节中可以查到），然后从该常量中找到`CONSTANT_UTF-8_info`。

整体过程如下所示。

![Fully Qualified Names and Canonical Names](https://img-blog.csdn.net/20180218135903687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzU5NTQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

父类索引和接口索引的过程与父索引大同小异。

## 字段表总数

类索引、父类索引、接口索引访问完成后的两个字节码中存放了字段表总数。

## 字段表集合

在字段表总数访问完成后以获取到后面多少字节存放的是字段。而每个字段又具有相同结构，如[字段结构表](https://blog.csdn.net/u013595419/article/details/t13)所示。

## 属性表总数

与字段表类似

## 属性表集合

与字段属性集合类似

到这里为止，Class文件的结构算是讲完了。

------

# 附件

## Class文件格式

| name                | type           | counts                  | description                                                  |
| ------------------- | -------------- | ----------------------- | ------------------------------------------------------------ |
| magic               | u4             | 1                       | 0xCAFEBABE                                                   |
| minor_version       | u2             | 1                       | the minor versions of the class file                         |
| major_version       | u2             | 1                       | the major versions of the class file                         |
| constant_pool_count | u2             | 1                       |                                                              |
| constant_pool       | cp_info        | constant_pool_count - 1 | Pool of constants for the class                              |
| access_flags        | u2             | 1                       | for example whether the class is abstract, static, etc.      |
| this_class          | u2             | 1                       | The name of the current class                                |
| super_class         | u2             | 1                       | The name of the super class                                  |
| interfaces_count    | u2             | 1                       |                                                              |
| interfaces          | u2             | interfaces_count        | Any interfaces in the class                                  |
| fields_count        | u2             | 1                       |                                                              |
| fields              | field_info     | fields_count            | Any fields in the class                                      |
| methods_count       | u2             | 1                       |                                                              |
| methods             | method_info    | methods_count           | Any methods in the class                                     |
| attribute_count     | u2             | 1                       |                                                              |
| attributes          | attribute_info | attributes_count        | Any attributes of the class (for example the name of the sourcefile, etc.) |

## 字段结构表

| 类型           | 名称             | 数量             |
| -------------- | ---------------- | ---------------- |
| u2             | access_flags     | 1                |
| u2             | name_index       | 1                |
| u2             | descriptor_index | 1                |
| u2             | attributes_count | 1                |
| attribute_info | attributes       | attributes_count |

## 访问标志及标志含义

| 标志名称       | 标志值 | 含义                                                         |
| -------------- | ------ | ------------------------------------------------------------ |
| ACC_PUBLIC     | 0x0001 | 是否为public类型                                             |
| ACC_FINAL      | 0x0010 | 是否被声明为final，只有类可设置                              |
| ACC_SUPER      | 0x0020 | 是否允许使用invokespecial字节码指令的新语言，invokespecial指令的语意在JDK1.0.2发生过改变，为了区别这条指令使用哪种语意，JDK1.0.2之后编译出来的类的这个标志都必须为真 |
| ACC_INTERFACE  | 0x0200 | 标识这是一个接口                                             |
| ACC_ABSTRACT   | 0x0400 | 是否为abstract类型，对于接口或者抽象类来说，此标志值为真，其他类值为假 |
| ACC_SYNTHETIC  | 0x1000 | 标识这个类并非由用户代码产生的                               |
| ACC_ANNOTATION | 0x2000 | 标识这是一个注解                                             |
| ACC_ENUM       | 0x4000 | 标识这是一个枚举                                             |

## 示例Java代码

```Java
package org.haha.clazz;

public class TestClass {
    private int m;
    public int inc() {
        return m + 1;
    }
}12345678
```

## Javac编译后的class文件

```shell
hexdump -C org/haha/clazz/TestClass.class1
00000000  ca fe ba be 00 00 00 34  00 13 0a 00 04 00 0f 09  |.......4........|
00000010  00 03 00 10 07 00 11 07  00 12 01 00 01 6d 01 00  |.............m..|
00000020  01 49 01 00 06 3c 69 6e  69 74 3e 01 00 03 28 29  |.I...<init>...()|
00000030  56 01 00 04 43 6f 64 65  01 00 0f 4c 69 6e 65 4e  |V...Code...LineN|
00000040  75 6d 62 65 72 54 61 62  6c 65 01 00 03 69 6e 63  |umberTable...inc|
00000050  01 00 03 28 29 49 01 00  0a 53 6f 75 72 63 65 46  |...()I...SourceF|
00000060  69 6c 65 01 00 0e 54 65  73 74 43 6c 61 73 73 2e  |ile...TestClass.|
00000070  6a 61 76 61 0c 00 07 00  08 0c 00 05 00 06 01 00  |java............|
00000080  18 6f 72 67 2f 68 61 68  61 2f 63 6c 61 7a 7a 2f  |.org/haha/clazz/|
00000090  54 65 73 74 43 6c 61 73  73 01 00 10 6a 61 76 61  |TestClass...java|
000000a0  2f 6c 61 6e 67 2f 4f 62  6a 65 63 74 00 21 00 03  |/lang/Object.!..|
000000b0  00 04 00 00 00 01 00 02  00 05 00 06 00 00 00 02  |................|
000000c0  00 01 00 07 00 08 00 01  00 09 00 00 00 1d 00 01  |................|
000000d0  00 01 00 00 00 05 2a b7  00 01 b1 00 00 00 01 00  |......*.........|
000000e0  0a 00 00 00 06 00 01 00  00 00 03 00 01 00 0b 00  |................|
000000f0  0c 00 01 00 09 00 00 00  1f 00 02 00 01 00 00 00  |................|
00000100  07 2a b4 00 02 04 60 ac  00 00 00 01 00 0a 00 00  |.*....`.........|
00000110  00 06 00 01 00 00 00 06  00 01 00 0d 00 00 00 02  |................|
00000120  00 0e                                             |..|
000001221234567891011121314151617181920
```

### hexdump的输出结果简单说明

最左边的一列是字节偏移量，中间的两大列是二进制数字的16进制表示（实际上就是class文件内容转成16进制表示而已），最右边的一列则是对中间两大列的ASCII表示。

#### 中间两大列的16进制表示

应该注意到在二进制里面是没有分段这个概念的，所有的内容都是`0`,`1`表示。也就是说上面的二进制文件在电脑看来大概是这样子的。

```plain
11001010111111101011101010111110000000000000000000000000001101001
```

hexdump在输出的时候会按照每4位切割转换成16进制输出（参考[2进制转16进制](https://baike.baidu.com/item/%E5%8D%81%E5%85%AD%E8%BF%9B%E5%88%B6%E8%BD%AC%E6%8D%A2)）。过程如下

```plain
1100 1010 1111 1110 1011 1010 1011 1110 0000 0000 0000 0000 0000 0000 0011 01001
c    a    f    e    b    a    b    e    0    0    0    0    0    0    3    41
```

然后再将两个相邻16进制（刚好占1Byte空间 `1Byte = 8bit`）数字写在一起，如下所示。

```AscllDoc
ca fe ba be 00 00 00 341
```

这里便发现hexdump的第一行的第二大列数字已经展示出来。

#### 偏移量的16进制表示

那么最左边的偏移量（**注意：偏移量仍然是16进制表示**）又是怎么回事呢？为什么不是+1，而是+16？

一开始就已经提到了在二进制数字里面是没有分段，换行之类的说法的，那么我们需要精确指出某个字节怎么办？

理所当然的想到了索引（或者称为下标，就像数组中可以通过下标取出数组中保存的内容一样），这个偏移量便是索引。在电脑的观点来看，中间两大列本质上是一行，而为了方便人来看，需要将其分成若干行，假如每一行放置16个16进制数字，那么为了和电脑中的观点对应起来，是不是应该对索引值加16了呢？

#### 最右边的ASCII表示

已经知道了内容的16进制表示，但是对于人而言，16进制的数字仍然是不可能直接读懂并且知道含义的，hexdump尽可能的帮助人去读懂中间两大列到底是什么，最简单的办法就是查[ASCII码表](https://ascii.cl/)。

但是ASCII码表只有128个字符表示，查不到的怎么办，hexdump就选择用`.`来标记该内容超出了ASCII码表的表示范围。最后在显示的时候，实际上会把`NUL`等不可打印字符同样用`.`显示出来。

> 比如上述 
>    16进制`ca fe ba be`超出ASCII表示范围，则显示`.`； 
>    16进制`00 00 00`表示`NUL`，显示`.` 
>    16进制`34`查找ASCII码表后，发现表示的是数字`4`，那么显示出来就可以了。