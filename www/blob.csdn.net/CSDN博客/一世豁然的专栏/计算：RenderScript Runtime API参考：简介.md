# 计算：RenderScript Runtime API参考：简介 - 一世豁然的专栏 - CSDN博客





2017年05月12日 09:31:18[一世豁然](https://me.csdn.net/Explorer_day)阅读数：404








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


|Types| |
|----|----|
|[char2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:char2)|Two 8 bit signed integers|
|[char3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:char3)|Three 8 bit signed integers|
|[char4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:char4)|Four 8 bit signed integers|
|[double2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:double2)|Two 64 bit floats|
|[double3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:double3)|Three 64 bit floats|
|[double4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:double4)|Four 64 bit floats|
|[float2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:float2)|Two 32 bit floats|
|[float3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:float3)|Three 32 bit floats|
|[float4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:float4)|Four 32 bit floats|
|[half](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:half)|16 bit floating point value|
|[half2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:half2)|Two 16 bit floats|
|[half3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:half3)|Three 16 bit floats|
|[half4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:half4)|Four 16 bit floats|
|[int16_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int16_t)|16 bit signed integer|
|[int2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int2)|Two 32 bit signed integers|
|[int3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int3)|Three 32 bit signed integers|
|[int32_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int32_t)|32 bit signed integer|
|[int4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int4)|Four 32 bit signed integers|
|[int64_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int64_t)|64 bit signed integer|
|[int8_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:int8_t)|8 bit signed integer|
|[long2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:long2)|Two 64 bit signed integers|
|[long3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:long3)|Three 64 bit signed integers|
|[long4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:long4)|Four 64 bit signed integers|
|[rs_matrix2x2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:rs_matrix2x2)|2x2 matrix of 32 bit floats|
|[rs_matrix3x3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:rs_matrix3x3)|3x3 matrix of 32 bit floats|
|[rs_matrix4x4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:rs_matrix4x4)|4x4 matrix of 32 bit floats|
|[rs_quaternion](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:rs_quaternion)|Quaternion|
|[short2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:short2)|Two 16 bit signed integers|
|[short3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:short3)|Three 16 bit signed integers|
|[short4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:short4)|Four 16 bit signed integers|
|[size_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:size_t)|Unsigned size type|
|[ssize_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ssize_t)|Signed size type|
|[uchar](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uchar)|8 bit unsigned integer|
|[uchar2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uchar2)|Two 8 bit unsigned integers|
|[uchar3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uchar3)|Three 8 bit unsigned integers|
|[uchar4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uchar4)|Four 8 bit unsigned integers|
|[uint](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint)|32 bit unsigned integer|
|[uint16_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint16_t)|16 bit unsigned integer|
|[uint2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint2)|Two 32 bit unsigned integers|
|[uint3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint3)|Three 32 bit unsigned integers|
|[uint32_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint32_t)|32 bit unsigned integer|
|[uint4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint4)|Four 32 bit unsigned integers|
|[uint64_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint64_t)|64 bit unsigned integer|
|[uint8_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:uint8_t)|8 bit unsigned integer|
|[ulong](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ulong)|64 bit unsigned integer|
|[ulong2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ulong2)|Two 64 bit unsigned integers|
|[ulong3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ulong3)|Three 64 bit unsigned integers|
|[ulong4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ulong4)|Four 64 bit unsigned integers|
|[ushort](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ushort)|16 bit unsigned integer|
|[ushort2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ushort2)|Two 16 bit unsigned integers|
|[ushort3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ushort3)|Three 16 bit unsigned integers|
|[ushort4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_value_types.html#android_rs:ushort4)|Four 16 bit unsigned integers|






三、对象类型

以下类型用于操作RenderScript对象，如分配，采样器，元素和脚本。 大多数这些对象是使用Java RenderScript API创建的。


|Types| |
|----|----|
|[rs_allocation](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_allocation)|Handle to an allocation|
|[rs_allocation_cubemap_face](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_allocation_cubemap_face)|Enum for selecting cube map faces|
|[rs_allocation_usage_type](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_allocation_usage_type)|Bitfield to specify how an allocation is used|
|[rs_data_kind](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_data_kind)|Element data kind|
|[rs_data_type](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_data_type)|Element basic data type|
|[rs_element](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_element)|Handle to an element|
|[rs_sampler](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_sampler)|Handle to a Sampler|
|[rs_sampler_value](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_sampler_value)|Sampler wrap T value|
|[rs_script](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_script)|Handle to a Script|
|[rs_type](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_types.html#android_rs:rs_type)|Handle to a Type|






四、转换函数

以下功能将数字矢量类型转换为另一种，或从一种颜色表示转换为另一种。


|Functions| |
|----|----|
|[convert](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_convert.html#android_rs:convert)|Convert numerical vectors|
|[rsPackColorTo8888](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_convert.html#android_rs:rsPackColorTo8888)|Create a uchar4 RGBA from floats|
|[rsUnpackColor8888](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_convert.html#android_rs:rsUnpackColor8888)|Create a float4 RGBA from uchar4|
|[rsYuvToRGBA](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_convert.html#android_rs:rsYuvToRGBA)|Convert a YUV value to RGBA|






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

|常量| |
|----|----|
|[M_1_PI](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_1_PI)|1 / pi, as a 32 bit float|
|[M_2_PI](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_2_PI)|2 / pi, as a 32 bit float|
|[M_2_SQRTPI](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_2_SQRTPI)|2 / sqrt(pi), as a 32 bit float|
|[M_E](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_E)|e, as a 32 bit float|
|[M_LN10](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_LN10)|log_e(10), as a 32 bit float|
|[M_LN2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_LN2)|log_e(2), as a 32 bit float|
|[M_LOG10E](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_LOG10E)|log_10(e), as a 32 bit float|
|[M_LOG2E](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_LOG2E)|log_2(e), as a 32 bit float|
|[M_PI](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_PI)|pi, as a 32 bit float|
|[M_PI_2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_PI_2)|pi / 2, as a 32 bit float|
|[M_PI_4](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_PI_4)|pi / 4, as a 32 bit float|
|[M_SQRT1_2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_SQRT1_2)|1 / sqrt(2), as a 32 bit float|
|[M_SQRT2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:M_SQRT2)|sqrt(2), as a 32 bit float|



|函数| |
|----|----|
|[abs](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:abs)|Absolute value of an integer|
|[acos](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:acos)|Inverse cosine|
|[acosh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:acosh)|Inverse hyperbolic cosine|
|[acospi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:acospi)|Inverse cosine divided by pi|
|[asin](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:asin)|Inverse sine|
|[asinh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:asinh)|Inverse hyperbolic sine|
|[asinpi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:asinpi)|Inverse sine divided by pi|
|[atan](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:atan)|Inverse tangent|
|[atan2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:atan2)|Inverse tangent of a ratio|
|[atan2pi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:atan2pi)|Inverse tangent of a ratio, divided by pi|
|[atanh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:atanh)|Inverse hyperbolic tangent|
|[atanpi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:atanpi)|Inverse tangent divided by pi|
|[cbrt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:cbrt)|Cube root|
|[ceil](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:ceil)|Smallest integer not less than a value|
|[clamp](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:clamp)|Restrain a value to a range|
|[clz](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:clz)|Number of leading 0 bits|
|[copysign](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:copysign)|Copies the sign of a number to another|
|[cos](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:cos)|Cosine|
|[cosh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:cosh)|Hypebolic cosine|
|[cospi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:cospi)|Cosine of a number multiplied by pi|
|[degrees](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:degrees)|Converts radians into degrees|
|[erf](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:erf)|Mathematical error function|
|[erfc](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:erfc)|Mathematical complementary error function|
|[exp](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:exp)|e raised to a number|
|[exp10](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:exp10)|10 raised to a number|
|[exp2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:exp2)|2 raised to a number|
|[expm1](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:expm1)|e raised to a number minus one|
|[fabs](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:fabs)|Absolute value of a float|
|[fdim](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:fdim)|Positive difference between two values|
|[floor](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:floor)|Smallest integer not greater than a value|
|[fma](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:fma)|Multiply and add|
|[fmax](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:fmax)|Maximum of two floats|
|[fmin](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:fmin)|Minimum of two floats|
|[fmod](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:fmod)|Modulo|
|[fract](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:fract)|Positive fractional part|
|[frexp](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:frexp)|Binary mantissa and exponent|
|[half_recip](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:half_recip)|Reciprocal computed to 16 bit precision|
|[half_rsqrt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:half_rsqrt)|Reciprocal of a square root computed to 16 bit precision|
|[half_sqrt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:half_sqrt)|Square root computed to 16 bit precision|
|[hypot](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:hypot)|Hypotenuse|
|[ilogb](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:ilogb)|Base two exponent|
|[ldexp](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:ldexp)|Creates a floating point from mantissa and exponent|
|[lgamma](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:lgamma)|Natural logarithm of the gamma function|
|[log](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:log)|Natural logarithm|
|[log10](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:log10)|Base 10 logarithm|
|[log1p](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:log1p)|Natural logarithm of a value plus 1|
|[log2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:log2)|Base 2 logarithm|
|[logb](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:logb)|Base two exponent|
|[mad](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:mad)|Multiply and add|
|[max](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:max)|Maximum|
|[min](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:min)|Minimum|
|[mix](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:mix)|Mixes two values|
|[modf](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:modf)|Integral and fractional components|
|[nan](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:nan)|Not a Number|
|[native_acos](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_acos)|Approximate inverse cosine|
|[native_acosh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_acosh)|Approximate inverse hyperbolic cosine|
|[native_acospi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_acospi)|Approximate inverse cosine divided by pi|
|[native_asin](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_asin)|Approximate inverse sine|
|[native_asinh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_asinh)|Approximate inverse hyperbolic sine|
|[native_asinpi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_asinpi)|Approximate inverse sine divided by pi|
|[native_atan](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_atan)|Approximate inverse tangent|
|[native_atan2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_atan2)|Approximate inverse tangent of a ratio|
|[native_atan2pi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_atan2pi)|Approximate inverse tangent of a ratio, divided by pi|
|[native_atanh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_atanh)|Approximate inverse hyperbolic tangent|
|[native_atanpi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_atanpi)|Approximate inverse tangent divided by pi|
|[native_cbrt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_cbrt)|Approximate cube root|
|[native_cos](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_cos)|Approximate cosine|
|[native_cosh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_cosh)|Approximate hypebolic cosine|
|[native_cospi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_cospi)|Approximate cosine of a number multiplied by pi|
|[native_divide](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_divide)|Approximate division|
|[native_exp](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_exp)|Approximate e raised to a number|
|[native_exp10](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_exp10)|Approximate 10 raised to a number|
|[native_exp2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_exp2)|Approximate 2 raised to a number|
|[native_expm1](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_expm1)|近似e上升到数字减1|
|[native_hypot](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_hypot)|近似斜边|
|[native_log](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_log)|近似自然对数|
|[native_log10](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_log10)|近似基数10对数|
|[native_log1p](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_log1p)|值的近似自然对数加1|
|[native_log2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_log2)|近似基数2对数|
|[native_powr](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_powr)|近似积极的基数升至指数|
|[native_recip](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_recip)|近似倒数|
|[native_rootn](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_rootn)|近似第n个根|
|[native_rsqrt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_rsqrt)|平方根的近似倒数|
|[native_sin](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_sin)|近似正弦|
|[native_sincos](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_sincos)|大概正弦和余弦|
|[native_sinh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_sinh)|近似双曲正弦|
|[native_sinpi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_sinpi)|一个数字的近似正弦乘以pi|
|[native_sqrt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_sqrt)|近似平方根|
|[native_tan](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_tan)|近似切线|
|[native_tanh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_tanh)|近似双曲正切|
|[native_tanpi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:native_tanpi)|一个数字的近似正切乘以pi|
|[nextafter](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:nextafter)|下一个浮点数|
|[pow](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:pow)|基数提升到指数|
|[pown](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:pown)|基数提升为整数指数|
|[powr](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:powr)|正的基数上升到指数|
|[radians](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:radians)|将度数转换为弧度|
|[remainder](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:remainder)|剩余部门|
|[remquo](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:remquo)|一个除法的余数和商数|
|[rint](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:rint)|Round to even|
|[rootn](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:rootn)|第N根|
|[round](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:round)|从零开始|
|[rsRand](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:rsRand)|伪随机数|
|[rsqrt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:rsqrt)|平方根的倒数|
|[sign](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:sign)|一个价值的标志|
|[sin](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:sin)|正弦|
|[sincos](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:sincos)|正弦和余弦|
|[sinh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:sinh)|双曲正弦|
|[sinpi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:sinpi)|正弦数乘以pi|
|[sqrt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:sqrt)|平方根|
|[step](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:step)|如果小于0，则为0，否则为0|
|[tan](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:tan)|切线|
|[tanh](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:tanh)|双曲正切|
|[tanpi](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:tanpi)|一个数字的相切乘以pi|
|[tgamma](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:tgamma)|伽玛功能|
|[trunc](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_math.html#android_rs:trunc)|截断一个浮点|






六、矢量数学函数

这些函数将输入参数解释为n维空间中向量的表示。





32位浮点数学运算的精度受编译指示rs_fp_relaxed和rs_fp_full的影响。 有关详细信息，请参见数学常数和函数。





通过使用常用数学函数的变体可以实现不同的精度/速度权衡。 名称以


1、native_：可能具有较低精度的定制硬件实现。 另外，亚正态值可以被刷新为零，可以使用向零舍入，并且可以不正确地处理NaN和无穷大输入。

2、fast_：可以使用16位浮点进行内部计算。 另外，可以将次正规值刷新为零，并且可以使用向零舍入。

|Functions| |
|----|----|
|[cross](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:cross)|两个向量的交叉乘积|
|[distance](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:distance)|两点之间的距离|
|[dot](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:dot)|两个向量的点积|
|[fast_distance](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:fast_distance)|两点之间的近似距离|
|[fast_length](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:fast_length)|矢量的近似长度|
|[fast_normalize](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:fast_normalize)|近似归一化矢量|
|[length](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:length)|一个向量的长度|
|[native_distance](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:native_distance)|两点之间的近似距离|
|[native_length](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:native_length)|矢量的近似长度|
|[native_normalize](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:native_normalize)|大概归一化一个向量|
|[normalize](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_vector_math.html#android_rs:normalize)|归一化矢量|






七、矩阵函数

这些函数可以让您处理2×2,3×3和4×4级的方阵。 它们对于图形转换特别有用，并且与OpenGL兼容。





我们对行和列使用基于零的索引。 例如。 （3，3）找到rs_matrix4x4的最后一个元素。





RenderScript使用列主要矩阵和基于列的向量。 通过后向量来完成向量的变换，例如 （matrix * vector），由rsMatrixMultiply（）提供。





为了创建一次执行两个变换的变换矩阵，乘以两个源矩阵，第一个变换作为正确的参数。 例如。 创建一个转换矩阵，该矩阵应用s1后跟s2，调用rsMatrixLoadMultiply（＆combined，＆s2，＆s1）。 这来自s2 *（s1 * v），它是（s2 * s1）* v。





我们有两种风格的函数来创建转换矩阵：rsMatrixLoadTransformation和rsMatrixTransformation。 前一种风格只是将变换矩阵存储在第一个参数中。 后者修改了一个预先存在的转换矩阵，以便首先发生新的变换。 例如。 如果在已经进行缩放的矩阵上调用rsMatrixTranslate（），则应用于向量的矩阵将首先进行转换，然后进行缩放。


|函数| |
|----|----|
|[rsExtractFrustumPlanes](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsExtractFrustumPlanes)|计算平头锥体水平面|
|[rsIsSphereInFrustum](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsIsSphereInFrustum)|检查球体是否在平截头体水平面内|
|[rsMatrixGet](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixGet)|获得一个元素|
|[rsMatrixInverse](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixInverse)|将矩阵反转到位|
|[rsMatrixInverseTranspose](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixInverseTranspose)|反转并将矩阵转置到位|
|[rsMatrixLoad](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixLoad)|加载或复制矩阵|
|[rsMatrixLoadFrustum](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixLoadFrustum)|加载平截头体投影矩阵|
|[rsMatrixLoadIdentity](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixLoadIdentity)|加载标识矩阵|
|[rsMatrixLoadMultiply](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixLoadMultiply)|乘以两个矩阵|
|[rsMatrixLoadOrtho](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixLoadOrtho)|加载正投影矩阵|
|[rsMatrixLoadPerspective](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixLoadPerspective)|加载透视投影矩阵|
|[rsMatrixLoadRotate](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixLoadRotate)|加载旋转矩阵|
|[rsMatrixLoadScale](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixLoadScale)|加载缩放矩阵|
|[rsMatrixLoadTranslate](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixLoadTranslate)|加载一个翻译矩阵|
|[rsMatrixMultiply](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixMultiply)|通过矢量或另一个矩阵乘以一个矩阵|
|[rsMatrixRotate](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixRotate)|对转换矩阵应用旋转|
|[rsMatrixScale](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixScale)|应用缩放到转换矩阵|
|[rsMatrixSet](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixSet)|设置一个元素|
|[rsMatrixTranslate](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixTranslate)|将转换应用于转换矩阵|
|[rsMatrixTranspose](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_matrix.html#android_rs:rsMatrixTranspose)|转置矩阵位置|






八、四元函数

以下函数操纵四元数。


|函数| |
|----|----|
|[rsQuaternionAdd](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionAdd)|添加两个四元数|
|[rsQuaternionConjugate](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionConjugate)|共轭四元数|
|[rsQuaternionDot](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionDot)|两个四元数的点积|
|[rsQuaternionGetMatrixUnit](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionGetMatrixUnit)|从四元数得到旋转矩阵|
|[rsQuaternionLoadRotate](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionLoadRotate)|创建一个旋转四元数|
|[rsQuaternionLoadRotateUnit](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionLoadRotateUnit)|表示围绕任意单位向量的旋转的四元数|
|[rsQuaternionMultiply](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionMultiply)|将四元数乘以标量或其他四元数|
|[rsQuaternionNormalize](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionNormalize)|归一化四元数|
|[rsQuaternionSet](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionSet)|创建四元数|
|[rsQuaternionSlerp](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_quaternion.html#android_rs:rsQuaternionSlerp)|两个四元数之间的球面线性插值|






九、原子更新函数

要更新多个线程之间共享的值，请使用以下功能。 它们确保值被原子更新，即存储器读取，更新和存储器写入以正确的顺序完成。





这些功能比其非原子等价物要慢，因此只有在需要同步时才使用它们。





请注意，在RenderScript中，尽管您没有明确创建它们，您的代码可能会在单独的线程中运行。 RenderScript运行时将经常将多个线程中的一个内核的执行拆分。 更新全局变量应该用原子函数完成。 如果可能，修改您的算法以避免它们完全相同。


|函数| |
|----|----|
|[rsAtomicAdd](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicAdd)|线程安全添加|
|[rsAtomicAnd](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicAnd)|线程安全按位和|
|[rsAtomicCas](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicCas)|线程安全比较和设置|
|[rsAtomicDec](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicDec)|线程安全递减|
|[rsAtomicInc](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicInc)|线程安全增量|
|[rsAtomicMax](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicMax)|线程安全最大值|
|[rsAtomicMin](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicMin)|线程安全最小|
|[rsAtomicOr](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicOr)|线程安全按位或|
|[rsAtomicSub](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicSub)|线程安全减法|
|[rsAtomicXor](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_atomic.html#android_rs:rsAtomicXor)|线程安全按位排序或|






十、时间函数和类型

以下功能可用于说明当前时钟时间和当前系统正常运行时间。 不建议在内核中调用这些函数。


|类型| |
|----|----|
|[rs_time_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_time.html#android_rs:rs_time_t)|自1970.1.1以来的秒数|
|[rs_tm](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_time.html#android_rs:rs_tm)|日期和时间结构|



|函数| |
|----|----|
|[rsGetDt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_time.html#android_rs:rsGetDt)|自上次电话以来的经过时间|
|[rsLocaltime](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_time.html#android_rs:rsLocaltime)|转换到当地时间|
|[rsTime](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_time.html#android_rs:rsTime)|自1970.1.1以来的秒数|
|[rsUptimeMillis](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_time.html#android_rs:rsUptimeMillis)|系统正常运行时间（以毫秒为单位）|
|[rsUptimeNanos](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_time.html#android_rs:rsUptimeNanos)|系统正常运行时间，以纳秒为单位|






十一、分配数据访问功能

以下函数可用于获取和设置构成分配的单元格。


1、使用rs GetElementAt *和SetTlement函数访问各个单元格。

2、可以使用rsAllocationCopy *和rsAllocationV *函数复制多个单元格。

3、要通过采样器获取值，请使用rsSample。




rsGetElementAt和rsSetElement *函数有点不明确。 他们没有获取或设置元素，类似于数据类型; 他们得到或设置单元格。 将它们视为rsGetCellAt和rsSetCellAt。


|函数| |
|----|----|
|[rsAllocationCopy1DRange](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsAllocationCopy1DRange)|在分配之间复制连续的单元格|
|[rsAllocationCopy2DRange](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsAllocationCopy2DRange)|在分配之间复制单元格的矩形区域|
|[rsAllocationVLoadX](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsAllocationVLoadX)|从标量分配中获取向量|
|[rsAllocationVStoreX](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsAllocationVStoreX)|将向量存储到标量的分配中|
|[rsGetElementAt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsGetElementAt)|从分配中返回一个单元格|
|[rsGetElementAtYuv_uchar_U](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsGetElementAtYuv_uchar_U)|获取分配YUV的U分量|
|[rsGetElementAtYuv_uchar_V](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsGetElementAtYuv_uchar_V)|获取分配YUV的V分量|
|[rsGetElementAtYuv_uchar_Y](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsGetElementAtYuv_uchar_Y)|获取Y分配的Y分量|
|[rsSample](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsSample)|从纹理分配中抽取一个值|
|[rsSetElementAt](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_allocation_data.html#android_rs:rsSetElementAt)|设置一个分配单元格|






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


|函数| |
|----|----|
|[rsAllocationGetDimFaces](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsAllocationGetDimFaces)|Presence of more than one face|
|[rsAllocationGetDimLOD](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsAllocationGetDimLOD)|Presence of levels of detail（存在细节级别）|
|[rsAllocationGetDimX](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsAllocationGetDimX)|X维度的尺寸|
|[rsAllocationGetDimY](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsAllocationGetDimY)|Y维度的尺寸|
|[rsAllocationGetDimZ](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsAllocationGetDimZ)|Z维度的尺寸|
|[rsAllocationGetElement](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsAllocationGetElement)|获取描述分配单元格的对象|
|[rsClearObject](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsClearObject)|释放一个对象|
|[rsElementGetBytesSize](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetBytesSize)|元素的大小|
|[rsElementGetDataKind](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetDataKind)|元素的种类|
|[rsElementGetDataType](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetDataType)|元素的数据类型|
|[rsElementGetSubElement](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetSubElement)|复杂元素的子元素|
|[rsElementGetSubElementArraySize](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetSubElementArraySize)|复数元素的子元素的数组大小|
|[rsElementGetSubElementCount](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetSubElementCount)|子元素数量|
|[rsElementGetSubElementName](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetSubElementName)|子元素的名称|
|[rsElementGetSubElementNameLength](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetSubElementNameLength)|子元素名称的长度|
|[rsElementGetSubElementOffsetBytes](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetSubElementOffsetBytes)|实例子元素的偏移量|
|[rsElementGetVectorSize](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsElementGetVectorSize)|元素的矢量大小|
|[rsIsObject](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsIsObject)|检查一个空的句柄|
|[rsSamplerGetAnisotropy](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsSamplerGetAnisotropy)|取样器的各向异性|
|[rsSamplerGetMagnification](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsSamplerGetMagnification)|取样器放大值|
|[rsSamplerGetMinification](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsSamplerGetMinification)|取样器最小化值|
|[rsSamplerGetWrapS](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsSamplerGetWrapS)|采样器包裹S值|
|[rsSamplerGetWrapT](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_object_info.html#android_rs:rsSamplerGetWrapT)|采样器包裹T值|






十三、内核调用函数和类型

rsForEach（）函数可用于调用脚本的根内核。





其他功能用于获取执行内核调用的特性，如维度和当前索引。 这些函数以rs_kernel_context为参数。


|类型| |
|----|----|
|[rs_for_each_strategy_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rs_for_each_strategy_t)|建议单元格处理顺序|
|[rs_kernel_context](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rs_kernel_context)|处理内核调用上下文|
|[rs_script_call_t](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rs_script_call_t)|单元格迭代信息|



|函数| |
|----|----|
|[rsForEach](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsForEach)|调用脚本的根内核|
|[rsGetArray0](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetArray0)|指定内核上下文的Array0维度中的索引|
|[rsGetArray1](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetArray1)|指定内核上下文的Array1维度中的索引|
|[rsGetArray2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetArray2)|指定内核上下文的Array2维度中的索引|
|[rsGetArray3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetArray3)|指定内核上下文的Array3维度中的索引|
|[rsGetDimArray0](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetDimArray0)|指定内核上下文的Array0维的大小|
|[rsGetDimArray1](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetDimArray1)|指定内核上下文的Array1维的大小|
|[rsGetDimArray2](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetDimArray2)|指定内核上下文的Array2维度的大小|
|[rsGetDimArray3](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetDimArray3)|指定内核上下文的Array3维度的大小|
|[rsGetDimHasFaces](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetDimHasFaces)|针对指定的内核上下文存在多个面|
|[rsGetDimLod](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetDimLod)|指定内核上下文的详细级别数|
|[rsGetDimX](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetDimX)|指定内核上下文的X维的大小|
|[rsGetDimY](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetDimY)|指定内核上下文的Y维的大小|
|[rsGetDimZ](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetDimZ)|指定内核上下文的Z维的大小|
|[rsGetFace](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetFace)|面向指定内核上下文的坐标|
|[rsGetLod](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_for_each.html#android_rs:rsGetLod)|指定内核上下文的“细节级别”维度中的索引|



十四、输入/输出功能

这些功能用于：


1、发送信息到Java客户端，和

2、发送处理后的分配或接收下一个分配进行处理。

|函数| |
|----|----|
|[rsAllocationIoReceive](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_io.html#android_rs:rsAllocationIoReceive)|从队列中接收新内容|
|[rsAllocationIoSend](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_io.html#android_rs:rsAllocationIoSend)|发送新的内容到队列|
|[rsSendToClient](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_io.html#android_rs:rsSendToClient)|向客户端发送消息，不堵塞|
|[rsSendToClientBlocking](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_io.html#android_rs:rsSendToClientBlocking)|发送消息给客户端，阻塞|






十五、调试功能

以下功能旨在在应用程序开发过程中使用。 它们不应用于运输应用程序。


|函数| |
|----|----|
|[rsDebug](https://developer.android.google.cn/guide/topics/renderscript/reference/rs_debug.html#android_rs:rsDebug)|记录消息和值|






十六、图形功能和类型

RenderScript的图形子系统在API级别23被删除。







