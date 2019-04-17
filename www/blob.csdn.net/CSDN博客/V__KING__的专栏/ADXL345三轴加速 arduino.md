# ADXL345三轴加速 arduino - V__KING__的专栏 - CSDN博客





2013年11月25日 20:24:06[v__king__](https://me.csdn.net/V__KING__)阅读数：5495








很早就听说Arduino，早就想玩了。最近搞了快Arduino板子，就是前面介绍的Arduino Leonardo，最近需要用到传感器，从三轴加速度ADXL345开始。

开发环境：

系统：XP

单板：Arduino Leonardo

平台：Arduino-1.5.2

目标：读三轴加速度ADXL345的原始数据，并通过串口显示

一、硬件介绍

ADXL345是一款小而薄的超低功耗3 轴加速度计，分辨率高(13位)，测量范围达±  16g 。数字输出数据为16 位二进制补码格式，可通过SPI(3线或4线)或I2C数字接口访问。ADXL345非常适合移动设备应用。它可以在倾斜检测应用中测量静态重力加速度，还可以测量运动或冲击导致的动态加速度。其高分辨率(3.9mg/LSB)，能够测量不到1.0° 的

倾斜角度变化。ADXL345模块原理图如下：

![](https://img-blog.csdn.net/20130525191535450)

这里只用到SCL、SDA、VCC_3.3V、GND分别连接到Arduino的对应接口上。Arduino Leonardo上直接标有SDA、SCL连上即可，其它Arduino根据自己的板子连接。

二、编写测试代码

其实在Arduino上写代码非常简单，很多库都帮你做好了，直接调用即可，十分方便。但Arduino的版本很多，特别是Arduino-1.0以后，有些库有所改变，需要看对应版本的参考手册arduino-1.5.2-windows\arduino-1.5.2\reference下面有详细说明，我也是结合网上找的程序参考这些修改。代码如下

文件adx1345.ino



**[plain]**[view plain](http://blog.csdn.net/u010216127/article/details/8974085#)[copy](http://blog.csdn.net/u010216127/article/details/8974085#)

- #include <Wire.h>  
- 
- #define Register_ID 0  
- #define Register_2D 0x2D  
- #define Register_X0 0x32  
- #define Register_X1 0x33  
- #define Register_Y0 0x34  
- #define Register_Y1 0x35  
- #define Register_Z0 0x36  
- #define Register_Z1 0x37  
- 
- int ADXAddress = 0xA7 >> 1;  // the default 7-bit slave address  
- int reading = 0;  
- int val=0;  
- int X0,X1,X_out;  
- int Y0,Y1,Y_out;  
- int Z1,Z0,Z_out;  
- double Xg,Yg,Zg;  
- 
- void setup()  
- {  
-   Wire.begin();                  
-   Serial.begin(9600);      
-   delay(100);  
-   // enable to measute g data  
-   Wire.beginTransmission(ADXAddress);  
-   Wire.write(Register_2D);  
-   Wire.write(8);                //measuring enable  
-   Wire.endTransmission();     // stop transmitting  
- 
- }  
- 
- void loop()  
- {  
-   //--------------X  
-   Wire.beginTransmission(ADXAddress); // transmit to device  
-   Wire.write(Register_X0);  
-   Wire.write(Register_X1);  
-   Wire.endTransmission();  
-   Wire.requestFrom(ADXAddress,2);   
-   if(Wire.available()<=2)     
-   {  
-     X0 = Wire.read();  
-     X1 = Wire.read();   
-     X1=X1<<8;  
-     X_out=X0+X1;     
-   }  
- 
-   //------------------Y  
-   Wire.beginTransmission(ADXAddress); // transmit to device  
-   Wire.write(Register_Y0);  
-   Wire.write(Register_Y1);  
-   Wire.endTransmission();  
-   Wire.requestFrom(ADXAddress,2);   
-   if(Wire.available()<=2)     
-   {  
-     Y0 = Wire.read();  
-     Y1 = Wire.read();   
-     Y1=Y1<<8;  
-     Y_out=Y0+Y1;  
-   }  
-   //------------------Z  
-   Wire.beginTransmission(ADXAddress); // transmit to device  
-   Wire.write(Register_Z0);  
-   Wire.write(Register_Z1);  
-   Wire.endTransmission();  
-   Wire.requestFrom(ADXAddress,2);   
-   if(Wire.available()<=2)     
-   {  
-     Z0 = Wire.read();  
-     Z1 = Wire.read();   
-     Z1=Z1<<8;  
-     Z_out=Z0+Z1;  
-   }  
-   /*  
-   //display the real value  
-   Xg=X_out/256.0;  
-   Yg=Y_out/256.0;  
-   Zg=Z_out/256.0;  
-   */  
-   //display the reading value  
-   Xg=X_out;  
-   Yg=Y_out;  
-   Zg=Z_out;  
-   Serial.print("X= ");  
-   Serial.print(Xg);  
-   Serial.print("       ");  
-   Serial.print("Y= ");  
-   Serial.print(Yg);  
-   Serial.print("       ");  
-   Serial.print("Z= ");  
-   Serial.print(Zg);  
-   Serial.println("  ");  
-   delay(200);  
- }  


需要对着手册操作，根据手册上介绍的寄存器设置、读写。寄存器FORMAT-OX31使用默认值0x0，设置加速度范围-2g~2g，根据手册

![](https://img-blog.csdn.net/20130525195857507)

我们设置的采样范围-2g~2g，根据手册知道我们的加速度传感器采样值最大范围 -282~282，其中存在偏差。10位分辨率，采样范围-2g~2g，则1024/(2g-(-2g))=256LSB/g,也就是说理想值是256对应2g的加速度，呆会测试时水平放置时，读到垂直方向的值应该是128，对应重力加速度1g.开始测试，看是不是这样

3.编译、测试

Arduino还是很方便操作的，选择好单板、参考，直接点上面的“对勾”就开始编译，编译没问题，点“->”箭头状的，开始上传程序，直至上传进度条完成。



接着打开Tools/Serial Monitor 显示如下：

![](https://img-blog.csdn.net/20130525202525300)这是水平放的，Z方向垂直加速度。不是很平

![](https://img-blog.csdn.net/20130525203143288)这是X方向，向X方向举起

![](https://img-blog.csdn.net/20130525203433345)这是向Y方向，其实模块后面有个坐标轴，朝坐标方向测试。

基本正常，但是这些数据还不能直接拿来用，还要进一步优化，以后会结合卡尔曼滤波等算法优化。



