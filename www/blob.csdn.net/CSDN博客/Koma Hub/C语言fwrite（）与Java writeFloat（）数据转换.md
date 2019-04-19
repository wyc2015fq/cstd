# C语言fwrite（）与Java writeFloat（）数据转换 - Koma Hub - CSDN博客
2018年01月19日 16:02:21[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：540
C语言fwrite写的二进制float和Java的DataOuputStream的writeFloat写的二进制float之间怎么转换？
Java.io.DataOutputStream 类允许应用程序写入基本Java数据类型在一个可移植的方式输出流。然后，应用程序可以使用数据输入流中读取的数据回入。
类声明
以下是java.io.DataOutputStream类的声明：
```
public class DataOutputStream
   extends FilterOutputStream
      implements DataOutput
```
java.io.DataOuputStream.writeFloat(float v) 方法写入一个浮点值的基础流。
那么C语言fwrite出来的float与java writeFloat出来的float怎么转化？
```cpp
void swap_float_4(float  *tnf4)
/ **************************************************************************
swap_float_4 swap a float
*************************************************************************** /
{
        int *tni4=(int *)tnf4;
        *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
                        ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}
```
用法：swap_float_4(writeFloat_float);
调用函数后就变成了与fwrite格式一致的浮点型。
