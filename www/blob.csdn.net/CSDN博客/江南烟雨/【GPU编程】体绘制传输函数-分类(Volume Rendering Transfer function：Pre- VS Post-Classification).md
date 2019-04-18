# 【GPU编程】体绘制传输函数-分类(Volume Rendering Transfer function：Pre- VS Post-Classification) - 江南烟雨 - CSDN博客
2012年03月29日 17:15:32[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2993标签：[function																[编程																[textures																[图形																[video																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=textures&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)
在科学可视化中，我们所获得的体数据集经常是代表一些光学上的不同物理属性的单值。通常没有可行的方法可以从这样的数据中获得发射和吸收属性。因此用户必须采用某种映射方法给数据值分配光学属性值来决定数据中的不同结构的模样。这离的映射就被称作传输函数。寻找合适的传输函数的过程就叫做分类。
需要注意的是，这里的分类和模式识别中的特征分类有所不同。它只是将体数据中得单值转化为RGBA值。随着三维可视化技术的发展，传输函数的设计已经不仅仅局限于颜色映射表，还被用来分辨感兴趣的特征。
传输函数的设计往往是费时、单调的工作，需要对数据集所表示的空间结构有着深刻的理解。为了方便设计过程，能够直接看到自己行为的反馈结果是很重要的。因此，在实现时当正在渲染时也必须能够实时修改传输函数。
**一、原理概述**
虽然理论上对连续函数的分析和描述是可行的，但是几乎所有实际的体图形应用程序都采用离散传输函数，就像一个固定大小的查找表一样。通常采用得比较多的发射吸收模型。发射系数以RGB值来表示，阻光度（1-Ti）以介于0和1之间的一个单值表示。
离散体数据通常用包含采样点的是三维数组来表示。根据采样定理，通过与一个合适的滤波核求卷积，连续信号能够从这些采样点重建出来。传输函数可以在重建之前直接应用在离散采样点上（pre-interpolative，pre-classification），或者是应用在重建之后的信号上（post-interpolative，post-classification）。两种方法产生不同的视觉效果。在硬件上有两种不同的方法来实现赋值过程，依据查找表在图形管线中得不同位置而不同。颜色表查找的实施在很大程度上依赖当前的硬件结构。
**二、pre-interpolative传输函数**
pre-interpolative映射的意思是在数据进行插值之前把传输函数应用在离散采样点之前。信号重建基于发射系数和吸收系数。
![](https://img-my.csdn.net/uploads/201203/29/1333009686_7781.jpg)
**图一**
左图体现了这点。在这里传输函数被描述成一个一维函数。实际上，对RGBA各个分量的映射是分开的。X轴上的原始的采样值被映射为y轴上的发射和吸收系数值。就像图中显示的，不是刚好位于网格点上的采样点的发射和吸收系数值由相邻网格点的发射和吸收系数值的插值获得。
在图形管线中，pre-interpolative映射意味着颜色表的查找发生在光栅化阶段之前或者光栅化过程中，但是一定在纹理滤波之前。传输函数在插值之前被应用在每个体素上。其优点在于预分类表可以在几乎每种图形硬件上实现。
颜色表在预处理过程中应用，会导致在更改传输函数的时候难以获得交互性好的帧率。有两个原因：
（1）传输函数的更改会导致main memory中整个体数据的重建，还要将纹理图像重新导入到图形卡的video memory中。这不可避免地会导致带宽瓶颈，会极大地降低性能。
（2）直接将颜色和阻光度值存储在纹理中，需要采用RGBA内部格式，这就需要为每个体素分配4字节的空间，而颜色表的一个索引值只需要一个或两个字节的内存。
标准OpenGL提供了在从内存到图形卡的像素传输过程中应用颜色映射的方法。当一幅纹理图像被定义然后传输到图形卡，这种操作会发生。因为更改颜色表需要重新加载纹理，所以这不是一种应用传输函数的快速方法。除了比较差的性能，另外一个缺点就是必须在local video memory中分配大量数据空间。虽然在内存中只存储了转化之前的体数据单值，但是当写入到分配给纹理图像的video memory中时，像素传输操作把每个单值转化成RGBA值。结果，最坏情况下所需要的数据存储空间变为原来的4倍。
注：video   memory：就是显示卡上的显存，一般是32，64，128M这样，速度最快，显卡可直接访问 。
像素传输操作中得颜色映射OpenGL代码：
```cpp
// enable and set up pixel transfer
glPixelTransferi(GL MAP COLOR, GL TRUE);
glPixelMapfv(GL PIXEL MAP I TO R,m nTableSize, m pColorMapR);
glPixelMapfv(GL PIXEL MAP I TO G,m nTableSize, m pColorMapG);
glPixelMapfv(GL PIXEL MAP I TO B,m nTableSize, m pColorMapB);
glPixelMapfv(GL PIXEL MAP I TO A,m nTableSize, m pColorMapA);
// (re-)create texture image
glTexImage3D(...);
// disable pixel transfer
glPixelTransferi(GL MAP COLOR, GL FALSE);
```
为了解决这个问题，一些硬件制造商采用了这样一种机制：把纹理图像中得颜色索引和颜色表分开存放，这个概念叫做调色板纹理（paletted textures）.
纹理调色板能够减少图形卡上需要分配给纹理的存储空间。而且这个特性可以让我们通过修改颜色调色板而不是纹理对象本身来达到颜色效果。这里为每个体素存数的不是RGBA值，而是一个固定大小的颜色查找表的索引。这个颜色查找表和索引一同存储在local video memory中。插值操作发生在颜色查找之后，它使用从颜色查找表中获得的颜色值，这就是pre-interpolative传输函数。这样，所需要的local
 video memory大大减小了。考虑到需要分配给纹理调色板本身的空间，所需要的纹理内存被减小几乎为原来的1/4.
纹理调色板的访问受两个OpenGL扩展控制。第一个是EXT_paletted_texture。它能够支持普通操作。另外一个扩展是GL_EXT_shared_texture_palette。它能够允许多个纹理共同访问一个纹理调色板。这能够更加进一步减少存储空间。
OpenGL调色板纹理扩展GL_EXT_shared_texture_palette：
```cpp
#if defined GL EXT shared texture palette
glEnable(GL_SHARED_TEXTURE_PALETTE_EXT);
glColorTableEXT(
GL_SHARED_TEXTURE_PALETTE_EXT, // GLenum target
GL_RGBA, // GLenum internal format
m_nColorTableSize, // GLsizei size of the table
GL_RGBA, // GLenum external format
GL_UNSIGNED_BYTE, // GLenum data type
m_pColorTable); // const GLvoid *table
#endif // GL_EXT_shared_texture_palette
```
**三、post-interpolative传输函数**
Post-classification颠倒了操作顺序，传输函数在屏幕空间信号重建之后被应用。分类函数因此是被应用在连续信号上而不是离散采样点上。图一的右图体现了这点。如果一个采样点不是位于网格点上，原始数据值被插值。接着，以插值后的结果作为索引去颜色查找表中进行查找，获得该采样点的发射和吸收系数值。
实现post-interpolative传输函数需要这样一种机制：在屏幕空间纹理采样插值之后实现颜色表的查找。通过使用可编程片段处理器，实现颜色表的查找非常容易。
dependent textures可以有效地实现这一机制。体素的颜色索引以亮度值的形式存储在第一个多重纹理中（可能是2D纹理或者是3D纹理）。第二个多重纹理被定义成一维纹理，它和颜色表有相同的分辨率，其中存储着颜色表。在光栅化阶段，从第一个纹理中得到的颜色索引被用作第二个纹理的纹理坐标。所得到的RGBA值就是通过post-interpolative索引查找获得的颜色值。
代码示例：
采用3D纹理：
```cpp
half4 main (half3 texUV : TEXCOORD0,
uniform sampler3D volume texture,
uniform sampler1D transfer function) : COLOR
{
half index = tex3D(volume texture, texUV);
half4 result = tex1D(transfer function, index);
return result;
}
```
采用2D纹理：
```cpp
half4 main (half3 texUV : TEXCOORD0,
uniform sampler2D slice texture0,
uniform sampler2D slice texture1,
uniform sampler1D transfer function) : COLOR
{
half index0 = tex2D(slice texture0, texUV.xy);
half index1 = tex2D(slice texture1, texUV.xy);
half index = lerp(index0, index1, texUV.z);
half4 result = tex1D(transfer function, index);
return result;
}
```
**四、pre-classification 和post-classification 传输函数比较**
![](https://img-my.csdn.net/uploads/201203/29/1333012572_4969.jpg)
**图二**
图二左右两幅图采用了相同的采样率和传输函数，左边利用pre-classification，右边利用post-classification。可以明显看出效果的差异。二者产生不同效果的原因是分类是非线性操作。pre-classification经常用于医学图像分割的数据上。
![](https://img-my.csdn.net/uploads/201203/29/1333012592_3990.jpg)
**图三**
我们从图三来分析二者在采样和重建方面的差异。原始的连续信号首先被离散化。中间的图是Pre-interpolative，首先应用传输函数，然后再插值重建信号。它丢失了传输函数所引入的高频成分。有明显的失真。右图是post-classification，首先在屏幕空间重建信号，然后再利用传输函数。其效果更加接近于左下角的分析结果。原因是传输函数引入了额外的高频成分，比如锐利的边缘。但是原始信号离散化后的数据采样点只考虑了原始信号中的高频成分。
从图四可以清楚地看出二者之间的差异：
![](https://img-my.csdn.net/uploads/201211/14/1352882950_4104.jpg)
图四 ；两种不同的数据分类方式
由采样定理可知，一个连续信号f(x)可通过其采样点f(kt)完整地恢复。很显然，对采样点应用传递函数T后进行信号恢复会产生较大的误差。因此，采用后分类可以得到比预分类更好的绘制结果。
