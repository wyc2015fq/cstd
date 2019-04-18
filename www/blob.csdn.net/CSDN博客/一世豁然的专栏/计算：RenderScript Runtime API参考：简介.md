# 计算：RenderScript Runtime API参考：简介 - 一世豁然的专栏 - CSDN博客





2017年05月12日 09:31:18[一世豁然](https://me.csdn.net/Explorer_day)阅读数：407








一、概论

RenderScript是一个高性能的运行时，可以在本机级别提供计算操作。 RenderScript代码在运行时在设备上编译，以允许平台独立性。





本参考文档介绍了RenderScript运行时API，您可以使用它来编写C99中的RenderScript代码。 自动为你包含RenderScript计算头文件。





要使用RenderScript，您需要使用此处记录的RenderScript运行时API以及RenderScript的Android框架API。 有关Android框架API的文档，请参阅android.renderscript软件包参考。





有关如何使用RenderScript开发以及运行时和Android框架API如何交互的更多信息，请参阅RenderScript开发人员指南和RenderScript示例。








二、数值类型

1、标量

RenderScript支持以下标量数值类型：


RenderScript支持长度为2,3和4.的固定大小向量。使用通用类型名称后跟2,3或4.声明向量。 float4，int3，double2，ulong4。


||8 bits|16 bits|32 bits|64 bits|
|----|----|----|----|----|
|Integer:|char, [int8_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int8_t)|short, [int16_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int16_t)|[int32_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int32_t)|long, long long, [int64_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int64_t)|
|Unsigned integer:|uchar, [uint8_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint8_t)|ushort, [uint16_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint16_t)|uint, [uint32_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint32_t)|ulong, [uint64_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint64_t)|
|Floating point:||half|float|double|



2、矢量

RenderScript支持长度为2,3和4.的固定大小向量。使用通用类型名称后跟2,3或4.声明向量。 float4，int3，double2，ulong4。





要创建矢量文字，请使用矢量类型，后跟大括号之间的值，例如。 （float3）{1.0f，2.0f，3.0f}。





可以使用不同的命名样式来访问向量的条目。





可以通过使用点跟随变量名称来访问单个条目：


1、字母x，y，z和w，

2、字母r，g，b和a，

3、字母s或S，后跟零索引。




例如，使用int4 myVar; 以下是等效的：



```java
myVar.x == myVar.r == myVar.s0 == myVar.S0
myVar.y == myVar.g == myVar.s1 == myVar.S1
myVar.z == myVar.b == myVar.s2 == myVar.S2
myVar.w == myVar.a == myVar.s3 == myVar.S3
```


可以通过使用多个字母或索引的连接的标识符一次访问向量的多个条目。 结果向量的大小等于命名的条目数。




通过上面的例子，可以使用myVar.yz，myVar.gb，myVar.s12和myVar.S12访问中间的两个条目。





条目不必是连续的或按照递增的顺序。 条目甚至可以重复，只要我们不试图分配它。 您也不能混合命名样式。





以下是可以做什么或不能做的例子：



```java
float4 v4;
float3 v3;
float2 v2;
v2 = v4.xx; // Valid
v3 = v4.zxw; // Valid
v3 = v4.bba; // Valid
v3 = v4.s032; // Valid
v3.s120 = v4.S233; // Valid
v4.yz = v3.rg; // Valid
v4.yzx = v3.rg; // Invalid: mismatched sizes
v4.yzz = v3; // Invalid: z appears twice in an assignment
v3 = v3.xas0; // Invalid: can't mix xyzw with rgba nor s0...
v3 = v4.s034; // Invalid: the digit can only be 0, 1, 2, or 3
```



3、矩阵和四元数

RenderScript支持大小为2x2,3x3和4x4的固定尺寸的浮点矩阵。 这些类型命名为rs_matrix2x2，rs_matrix3x3和rs_matrix4x4。 有关操作列表，请参阅矩阵函数。





四元数也被rs四元数支持。 有关操作列表，请参见四元数函数。


[ushort4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ushort4)
Four 16 bit unsigned integers





三、对象类型

以下类型用于操作RenderScript对象，如分配，采样器，元素和脚本。 大多数这些对象是使用Java RenderScript API创建的。


[rs_type](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_type)
Handle to a Type





四、转换函数

以下功能将数字矢量类型转换为另一种，或从一种颜色表示转换为另一种。


[rsYuvToRGBA](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_convert.html#android_rs:rsYuvToRGBA)
Convert a YUV value to RGBA





五、数学常数和函数

下面的数学函数可以应用于标量和向量。 当应用于向量时，返回的值是应用于输入的每个条目的函数的向量。





例如：


```java
float3 a, b;
// The following call sets
// a.x to sin(b.x),
// a.y to sin(b.y), and
// a.z to sin(b.z).
a = sin(b);
```


参见vector（）函数，如distance（）和length（），将输入解释为n维空间中的单个向量。




32位浮点数学运算的精度受编译指示rs_fp_relaxed和rs_fp_full的影响。 在rs_fp_relaxed下，可能会将低于正常值的值刷新为零，并可以向零舍入。 相比之下，rs_fp_full需要对正常值进行正确处理，即小于1.17549435e-38f。 rs_fp_rull也需要一轮到最接近的连线。





通过使用常用数学函数的变体可以实现不同的精度/速度权衡。 名称以


1、native_：可能具有较低精度的定制硬件实现。 另外，亚正态值可以被刷新为零，可以使用向零舍入，并且可以不正确地处理NaN和无穷大输入。

2、half_：可以使用16位浮点进行内部计算。 另外，可以将次正规值刷新为零，并且可以使用向零舍入。

[M_SQRT2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_SQRT2)
sqrt(2), as a 32 bit float


[trunc](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:trunc)
截断一个浮点





六、矢量数学函数

这些函数将输入参数解释为n维空间中向量的表示。





32位浮点数学运算的精度受编译指示rs_fp_relaxed和rs_fp_full的影响。 有关详细信息，请参见数学常数和函数。





通过使用常用数学函数的变体可以实现不同的精度/速度权衡。 名称以


1、native_：可能具有较低精度的定制硬件实现。 另外，亚正态值可以被刷新为零，可以使用向零舍入，并且可以不正确地处理NaN和无穷大输入。

2、fast_：可以使用16位浮点进行内部计算。 另外，可以将次正规值刷新为零，并且可以使用向零舍入。

[normalize](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:normalize)
归一化矢量





七、矩阵函数

这些函数可以让您处理2×2,3×3和4×4级的方阵。 它们对于图形转换特别有用，并且与OpenGL兼容。





我们对行和列使用基于零的索引。 例如。 （3，3）找到rs_matrix4x4的最后一个元素。





RenderScript使用列主要矩阵和基于列的向量。 通过后向量来完成向量的变换，例如 （matrix * vector），由rsMatrixMultiply（）提供。





为了创建一次执行两个变换的变换矩阵，乘以两个源矩阵，第一个变换作为正确的参数。 例如。 创建一个转换矩阵，该矩阵应用s1后跟s2，调用rsMatrixLoadMultiply（＆combined，＆s2，＆s1）。 这来自s2 *（s1 * v），它是（s2 * s1）* v。





我们有两种风格的函数来创建转换矩阵：rsMatrixLoadTransformation和rsMatrixTransformation。 前一种风格只是将变换矩阵存储在第一个参数中。 后者修改了一个预先存在的转换矩阵，以便首先发生新的变换。 例如。 如果在已经进行缩放的矩阵上调用rsMatrixTranslate（），则应用于向量的矩阵将首先进行转换，然后进行缩放。


[rsMatrixTranspose](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixTranspose)
转置矩阵位置





八、四元函数

以下函数操纵四元数。


[rsQuaternionSlerp](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionSlerp)
两个四元数之间的球面线性插值





九、原子更新函数

要更新多个线程之间共享的值，请使用以下功能。 它们确保值被原子更新，即存储器读取，更新和存储器写入以正确的顺序完成。





这些功能比其非原子等价物要慢，因此只有在需要同步时才使用它们。





请注意，在RenderScript中，尽管您没有明确创建它们，您的代码可能会在单独的线程中运行。 RenderScript运行时将经常将多个线程中的一个内核的执行拆分。 更新全局变量应该用原子函数完成。 如果可能，修改您的算法以避免它们完全相同。


[rsAtomicXor](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicXor)
线程安全按位排序或





十、时间函数和类型

以下功能可用于说明当前时钟时间和当前系统正常运行时间。 不建议在内核中调用这些函数。


[rs_tm](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_time.html#android_rs:rs_tm)
日期和时间结构


[rsUptimeNanos](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_time.html#android_rs:rsUptimeNanos)
系统正常运行时间，以纳秒为单位





十一、分配数据访问功能

以下函数可用于获取和设置构成分配的单元格。


1、使用rs GetElementAt *和SetTlement函数访问各个单元格。

2、可以使用rsAllocationCopy *和rsAllocationV *函数复制多个单元格。

3、要通过采样器获取值，请使用rsSample。




rsGetElementAt和rsSetElement *函数有点不明确。 他们没有获取或设置元素，类似于数据类型; 他们得到或设置单元格。 将它们视为rsGetCellAt和rsSetCellAt。


[rsSetElementAt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsSetElementAt)
设置一个分配单元格





十二、对象特征功能

以下功能可用于查询分配，元素或采样器对象的特性。 这些对象是从Java创建的。 您无法从脚本创建它们。





1、分配

分配是用于向RenderScript内核传递数据的主要方法。





它们是可用于存储位图，纹理，任意数据点等的单元格的结构化集合。





单元格的这个集合可能具有许多维度（X，Y，Z，Array0，Array1，Array2，Array3），面（用于立方体）和细节级别（用于mipmap）。





有关创建分配的详细信息，请参阅android.renderscript.Allocation。





2、元素

术语“元素”在RenderScript中有点模糊，作为分配单元格的类型信息和该类型的实例化。 例如：


 1）、rs_element是类型规范的句柄，

 2）、在诸如rsGetElementAt（）的函数中，“元素”表示类型的实例化，即分配的单元格。




以下函数可以查询类型规格的特征。





元素可以指定C中所发现的简单数据类型，例如。 一个整数，float或boolean。 它还可以指定一个RenderScript对象的句柄。 有关基本类型的列表，请参阅rs_data_type。





元素可以指定基本类型的固定大小向量（大小为2,3或4）版本。 元素可以组合成复杂的元素，创建相当于C结构定义。





元素也可以有一种，这是用于解释像素数据的语义信息。 见rs_data_kind。





当创建公共元素的分配时，您可以简单地使用许多预定义元素之一，如F32_2。





要创建复杂的元素，请使用Element.Builder Java类。





3、取样

采样器对象定义如何将内存中的结构读取为分配。 参见android.renderscript.S。


[rsSamplerGetWrapT](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsSamplerGetWrapT)
采样器包裹T值





十三、内核调用函数和类型

rsForEach（）函数可用于调用脚本的根内核。





其他功能用于获取执行内核调用的特性，如维度和当前索引。 这些函数以rs_kernel_context为参数。


[rs_script_call_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rs_script_call_t)
单元格迭代信息


[rsGetLod](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetLod)
指定内核上下文的“细节级别”维度中的索引


十四、输入/输出功能

这些功能用于：


1、发送信息到Java客户端，和

2、发送处理后的分配或接收下一个分配进行处理。

[rsSendToClientBlocking](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_io.html#android_rs:rsSendToClientBlocking)
发送消息给客户端，阻塞





十五、调试功能

以下功能旨在在应用程序开发过程中使用。 它们不应用于运输应用程序。


[rsDebug](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_debug.html#android_rs:rsDebug)
记录消息和值





十六、图形功能和类型

RenderScript的图形子系统在API级别23被删除。







