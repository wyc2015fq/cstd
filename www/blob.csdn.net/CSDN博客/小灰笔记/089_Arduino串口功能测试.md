# 089_Arduino串口功能测试 - 小灰笔记 - CSDN博客





2019年04月16日 22:13:52[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3








         之前接触Arduino的时候，仅仅是简单用了一下串口的打印输出功能，也没有认真去看。今天把文档看了一遍，还是挺有收获的。

         我自己的测试代码如下，同时我还会梳理一下我在学习过程中遇到的一点小问题。

```cpp
#include "SoftwareSerial.h"

// 创建一个串口对象，10号管脚用于接收，11号管脚用于发送。这个函数还有一个可选的参数，默认为false，用于指示是否逻辑反转
SoftwareSerial my_serial(10, 11);
unsigned int counter;
int read_first_byte;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    while (!Serial)
    {
        /* 等待连接 */
    }

    Serial.println(" Serial init for arduino");
}

void loop()
{
    // put your main code here, to run repeatedly:
    Serial.begin(9600);
    Serial.println("------------------------Serial test task for arduino------------------------");
    Serial.print(counter);
    Serial.print("\t");
    Serial.print(counter, BIN);
    Serial.print("\t");
    Serial.print(counter, HEX);
    Serial.print("\n");
    Serial.println("test for format print function:");
    Serial.print(3.14159265358);
    Serial.print("\t");
    Serial.print(3.14159265358, 2);
    Serial.print("\t");
    Serial.print(3.14159265358, 5);
    Serial.print("\t\n");
    // test for read function
    if(Serial.available() > 0)
    {
        // read first byte
        read_first_byte = Serial.read();
        Serial.print("data come from serial interface!\t");
        Serial.print(read_first_byte);
    }
    // test for write function
    Serial.write("test for write function");
    Serial.end();
    Serial.print(3.14159265358, 8);
    Serial.println("should not display on screen!");
    delay(1000); // wait for a second
    counter++;
}
```

         代码分析与收获：
- 首先说一下print以及println

这两个功能都是打印功能，前者更加灵活一点，可以进行格式化的输出，而println的输出还带有一个加换行符的功能。在我的测试代码中，也测试了格式化输出的功能，可以输出二进制、八进制、十六进制等多种格式，还可以选择输出浮点数小数点的位数。函数都是支持重载功能的，因此可以看得出是使用C++而不是C来设计的。或许，后续的嵌入式开发我也可以尝试用一下C++。
- begin

这个功能可以设置波特率并启动，与之对应的一个功能是end。
- end

这个功能可以禁止串口功能，禁用之后的管脚可以用于GPIO的功能。
- available

这个功能可以用来查看是否有数据输入，返回值就是数据的字节数。
- read

这个是一个很值得注意的功能，我使用的时候遇到了一点编译障碍。按说，读取回来的返回值是一个字节的数据，但是必须使用int来接收数据。如果使用unsigned char的变量来接收的话会遇到编译问题。
- write

其实，这个功能在一定程度上类似于print，但是少了格式化输出的功能。



         还有一个值得注意的功能是第一行有效代码：SoftwareSerial my_serial(10, 11);

         本来，这个是用来设置管脚的。但是，我发现屏蔽之后也没有什么不行的效果。板子上有一个LED灯会在发送动作产生的时候点亮。应该还有一个接收，现在没有测试，也就没有看到这个点亮的现象。

         最后附加一个执行时候的监控效果：

![](https://img-blog.csdnimg.cn/20190416221248513.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)



