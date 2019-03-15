[Unsharp Mask(USM)锐化算法的的原理及其实现。](https://www.cnblogs.com/Imageshop/archive/2013/05/19/3086388.html)

​      在开局之前，首先说明一点，Photoshop的USM锐化只是本文所指USM的一种比较特殊的例子而已。

​      通过增强图像的高频部分的内容，图像的视觉效果可以极大的得到改观。为达到这个目的，通常可以使用经典USM技术来实现。这个技术的流程可用下图来实现:

​                    ![img](https://images0.cnblogs.com/blog/349293/201305/18191527-d11db65ad6124e78949a28c301732eac.jpg)

​      用具体的公式表达即为：

​                    y(n,m)= x(n,m)+ λz(n,m)                                                       （1）

​      其中， x(n,m)为输入图像，y(n,m)为输出图像，而z(n,m)为校正信号，一般是通过对x进行高通滤波获取。λ是用于控制增强效果的的一个缩放因子。

​      在传统的USM算法中，z(n,m)一般可以通过下式获取：

​              z(n,m)=4x(n,m)-x(n-1,m) -x(n +1,m)-x(n, m-1) -x(n,m+1)             （2）

​      当然也可以用如下的模板：

​                      ![img](https://images0.cnblogs.com/blog/349293/201305/18192532-446cd9638d8041d7b138b88ad363164a.jpg)

​        贴部分参考代码：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```cpp
   Width = Bitmap.Width; Height = Bitmap.Height; Stride = Bitmap.Stride; 
   NewHeight = Height +  2; NewStride = (Width +  2) * BytePerPixel;　　　　　　　　　　
   ExpandPtr = (byte*)Win32Api.GlobalAlloc(Win32Const.GPTR, NewStride * NewHeight);　　// 为保证边缘部分处理方便，扩展边界部分
   Utility.GetExpandImage(Bitmap, 1, ExpandPtr);　　　　　　// 拷贝图像到缓冲区，以及填充边缘像素
   for (Y = 0; Y < Height; Y++)　　　　　　　　　　　　　　　　// 处理灰度图像
   {
       Pointer = Bitmap.Pointer + Y * Stride;
       ExpandP = ExpandPtr + (Y+1) * NewStride+1;
       for (X = 0; X < Width; X++)
       {
           HighPass = (ExpandP[X] << 2) - ExpandP[X - 1] - ExpandP[X + 1] - ExpandP[X - NewStride] - ExpandP[X + NewStride];   // z(n,m)
           Value = Pointer[X] + Amount * HighPass / 100;   // x(n,m)+ λz(n,m),式中的Amount即这里的λ     
           Pointer[X] = (byte)((((ushort)Value | ((short)(255 - Value) >> 15)) & ~Value >> 15));  // 防止数据溢出
        }
    }
    Win32Api.GlobalFree((IntPtr)ExpandPtr);
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在很多场合，这个方法也能获得较好的效果，比如下述的Lena图。

​     ![img](https://images0.cnblogs.com/blog/349293/201305/18194304-fd71e16937f34420bcf9f5871b04c645.jpg)    ![img](https://images0.cnblogs.com/blog/349293/201305/18195224-95d64390c96548629a5550fd8403abe2.jpg)    ![img](https://images0.cnblogs.com/blog/349293/201305/18195257-18a9444286bf427987475cab9e06f9cb.jpg)

​                   原图 　　　　　　　　　　　　　　　　　　Amount=25                                    Amount=100

​      不过这个方面也有以下的缺点： 1） 线性的高通滤波使得效果对噪音非常敏感，这会导致一些不希望的扭曲，特别在图像变换比较缓慢的地方的噪音，比如上图3中草帽的左侧平坦区域。2）对于图像的边缘（高对比度区域）会出现增强过头的现象，如上图草帽的边缘。  这两点会导致输出图像存在一些令人看上去不舒服的地方。
​     很多论文中提出了一些自适应通过动态的改变式（1）中的λ参数来控制调节结果。在图像的变换比较平坦的区域，λ取值小，在较大的对比度（边缘处）地方取适当的λ，而在中对比度处取较大的λ值，以使得这部分的得到最大的增强。 比如这篇文章[ Image Enhancement via Adaptive Unsharp Masking](http://www.ece.utah.edu/~mathews/publications/IP2000.pdf) 中就提出了一种逐步更新的方式。不过类似这样的文章都普遍存在一个问题，那就是可控参数过多，且这些从参数的取值需要过多的人工参与，我认为这样的算法，是不具有实用的价值的。

​     在Photoshop的锐化菜单中也有一项USM锐化，其实这个功能也是符合式1的定义的。通过其UI界面我们可以发现其有3个参数：半径、数量、阈值，其内部的算法过程可以用如下的简单代码表示：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```cpp
    Width = Bitmap.Width; Height = Bitmap.Height; Stride = Bitmap.Stride; BytePerPixel = Bitmap.BitCount / 8;
    FastBitmap Clone = Bitmap.Clone();　　　　// 备份图像数据
    BlurEffect.GaussianBlur(Clone, Radius);  // 对备份的数据进行高斯模糊处理
    for (Y = 0; Y < Height; Y++)
    {
        Pointer = Bitmap.Pointer + Y * Stride;
        PointerC = Clone.Pointer + Y * Stride;
        for (X = 0; X < Width; X++)
        {
            Value = Pointer[X] - PointerC[X];
            if (Utility.Abs (Value) > Threshold)
            {
                Value = Pointer[X] + Amount * Value / 100;
                Pointer[X] = (byte)((((ushort)Value | ((short)(255 - Value) >> 15)) & ~Value >> 15));
            }
        }
    }
    Clone.Dispose();
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

​      众所周知，高斯模糊时低通滤波，那么 Value = Pointer[X] - PointerC[X]（原值-低通）则相当于高通的结果，如果原值和低通的差异的绝对值大于指定的阈值，则对改点进行所谓的锐化。

​      这里对式（1）多引进了一个参数阈值，通过调节该值，来决定达到何种程度对比度的像素才需要增强。

​      实际的效果表明，这种方式的锐化要比传统的USM锐化能获得更好的调节效果。

​      以下为Threshold=0,Amount=50时不同半径的效果。

 ![img](https://images0.cnblogs.com/blog/349293/201305/18194304-fd71e16937f34420bcf9f5871b04c645.jpg)  ![img](https://images0.cnblogs.com/blog/349293/201305/18230125-39f62da5288b424da24f5bcd7b7915b5.jpg)  ![img](https://images0.cnblogs.com/blog/349293/201305/18230230-743d7bcc12be4e29bc4cc19ee7c161e8.jpg)  ![img](https://images0.cnblogs.com/blog/349293/201305/18230310-ee7004e3f8ae4ba0b792875e0f4677c8.jpg)  

 ![img](https://images0.cnblogs.com/blog/349293/201305/19000210-b6b60909042b4315b45a51a131d958a5.jpg)  ![img](https://images0.cnblogs.com/blog/349293/201305/19000218-fb0de7ddf9b545fa8d76e3d8ad4a18e1.jpg)  ![img](https://images0.cnblogs.com/blog/349293/201305/19000225-aeecadaf57fa4f499f0ce08c51c7632f.jpg)  ![img](https://images0.cnblogs.com/blog/349293/201305/19000232-0cb150df9c8d49bfa74b5b228d1c4eeb.jpg)

​                    原图                                                                Radius=5                                             Radius=50                                          Radius=200

​      随着半径的增大，图像的对比度逐渐变强，边缘越发明显，但是所有的调节后的图像都未出现明显的噪音增强，效果非常之理想，对于Lena图，处理后的图片中可以明显的看出在眼睛部位，眼白和眼球的对比更为清晰、明显，而且整幅图形从感觉上说原始图像较为朦胧，处理后的清晰不少。

​      Amount参数对效果的影响很明显可以获知，越大，对比度越高，但是由于其和结果的线性关系，这个参数的变化对结果的影响比其他两个参数更敏感。

​      实际上，上述高斯模糊也可以用中值模糊、方框模糊来代替，所得到的效果和高斯滤波非常相似。在Paint.net的效果-》相片-》尖锐化滤镜就是用的中值的方式实现的。

　   关于实现代码，基本上我在上面已经提及，其实最关键还是高斯模糊的实现。如果你觉得有难度，正如上文所说，也可以用均值来代替，而均值模糊编码则非常简单。 

 

​    同样，提供个编译好的文件给有兴趣研究该算法的朋友看看效果：

​    <http://files.cnblogs.com/Imageshop/USM.rar>

 

 

 ***************************作者： laviewpbt   时间： 2013.5.19    联系QQ:  33184777  转载请保留本行信息*************************