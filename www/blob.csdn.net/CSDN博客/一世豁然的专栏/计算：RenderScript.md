# 计算：RenderScript - 一世豁然的专栏 - CSDN博客





2017年05月11日 15:06:55[一世豁然](https://me.csdn.net/Explorer_day)阅读数：365标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









RenderScript是在Android上以高性能运行计算密集型任务的框架。 RenderScript主要面向数据并行计算，尽管串行工作负载也可以受益。 RenderScript运行时间可以跨设备上可用的处理器（如多核CPU和GPU）进行并行工作。 这允许您专注于表达算法，而不是调度工作。 RenderScript对于执行图像处理，计算摄影或计算机视觉的应用程序特别有用。




要开始使用RenderScript，您应该了解两个主要的概念：


1、该语言本身是用于编写高性能计算代码的C99派生语言。 以下编写RenderScript内核一节描述如何使用它来编写计算内核。




2、控制API用于管理RenderScript资源的生命周期并控制内核执行。 它有三种不同的语言：Android，NDK中的Java，C ++以及C99派生的内核语言本身。 使用Java代码和单源RenderScript中的RenderScript分别描述了第一个和第三个选项。







一、编写一个RenderScript内核

RenderScript内核通常驻留在<project_root> / src /目录中的.rs文件中; 每个.rs文件都被称为脚本。 每个脚本都包含自己的一组内核，函数和变量。 脚本可以包含：


1、一个pragma声明（#pragma version（1）），声明此脚本中使用的RenderScript内核语言的版本。 目前，1是唯一的有效值。




2、一个pragma声明（#pragma rs java_package_name（com.example.app）），声明从这个脚本反映的Java类的包名称。 请注意，.rs文件必须是应用程序包的一部分，而不是库项目中。




3、零个或多个可调用功能。 调用函数是一个单线程的RenderScript函数，您可以使用任意参数从Java代码调用。 这些通常用于较大的处理流水线中的初始设置或串行计算。




4、零个或多个脚本全局变量。 脚本全局相当于C中的全局变量。您可以从Java代码访问脚本全局变量，这些常用于将参数传递给RenderScript内核。




5、零个或多个计算内核。 计算内核是一个函数或函数集合，您可以指导RenderScript运行时并行跨数据集合执行。 有两种计算内核：映射内核（也称为foreach内核）和还原内核。




映射内核是一个并行函数，它对相同维度的分配集合进行操作。 默认情况下，它对于这些维度中的每个坐标执行一次。 通常（但不排他地）用于将输入分配的集合一次转换为输出分配一个元素。





●这是一个简单映射内核的例子：




```java
uchar4 RS_KERNEL invert(uchar4 in, uint32_t x, uint32_t y) {
  uchar4 out = in;
  out.r = 255 - in.r;
  out.g = 255 - in.g;
  out.b = 255 - in.b;
  return out;
}
```


在大多数方面，这与标准C函数相同。 应用于函数原型的RS_KERNEL属性指定该函数是RenderScript映射内核，而不是可调用函数。 in参数是根据传递给内核启动的输入分配自动填写的。 下面讨论参数x和y。 从内核返回的值将自动写入输出分配中的相应位置。 默认情况下，该内核在其整个输入分配中运行，每个元素在分配中执行一个内核函数。






映射内核可以具有一个或多个输入分配，单个输出分配或两者。 RenderScript运行时检查以确保所有输入和输出分配具有相同的维度，并且输入和输出分配的元素类型与内核的原型相匹配; 如果这些检查中的任何一个失败，RenderScript会引发异常。





注意：在Android 6.0（API级别23）之前，映射内核可能没有多个输入分配。





如果您需要比内核更多的输入或输出分配，则这些对象应绑定到rs_allocation脚本全局，并通过rsGetElementAt_type（）或rsSetElementAt_type（）从内核或可调用函数访问。





注意：RS_KERNEL是由RenderScript自动定义的一个宏，方便您使用：




```java
#define RS_KERNEL __attribute__((kernel))
```


还原内核是一系列功能，可以对相同维度的输入分配集合进行操作。 默认情况下，其累加器功能对于这些维度中的每个坐标执行一次。 通常（但不排他地）用于将输入分配的集合“减少”为单个值。






●这是一个简单的还原内核的例子，它将其输入的元素相加：




```java
#pragma rs reduce(addint) accumulator(addintAccum)

static void addintAccum(int *accum, int val) {
  *accum += val;
}
```


还原内核由一个或多个用户编写的功能组成。 #pragma rs reduce用于通过指定其名称（在本例中为addint）以及组成内核的函数（在本示例中为累加器函数addintAccum）的名称和角色来定义内核。 所有这些功能必须是静态的。 还原内核总是需要一个累加器功能; 它可能还有其他功能，这取决于您希望内核做什么。






还原内核累加器函数必须返回void并且必须至少有两个参数。 第一个参数（在本示例中为accum）是一个指向累加器数据项的指针，而第二个参数（在此示例中为val）将根据传递给内核启动的输入分配自动填写。 累加器数据项由RenderScript运行时创建; 默认情况下，它被初始化为零。 默认情况下，该内核在其整个输入分配中运行，每个元素在分配中执行一次累加器函数。 默认情况下，累加器数据项的最终值被视为减少的结果，并返回给Java。 RenderScript运行时检查以确保输入分配的Element类型与累加器函数的原型匹配;
 如果不匹配，则RenderScript会引发异常。





还原内核具有一个或多个输入分配，但没有输出分配。





在缩小内核深度这一节更详细地说明还原粒子。





Android 7.0（API级别24）及更高版本支持减少内核。





映射内核函数或还原内核累加器函数可以使用特殊参数x，y和z访问当前执行的坐标，这些参数必须是int或uint32_t类型。 这些参数是可选的。





映射内核函数或还原内核累加器函数也可以使用类型为rs_kernel_context的可选特殊参数上下文。 一系列运行时API需要用于查询当前执行的某些属性 - 例如rsGetDimX。 （上下文参数在Android 6.0（API级别23）及更高版本中可用）。





6、一个可选的init（）函数。 init（）函数是一个特殊类型的调用函数，当脚本首次实例化时，RenderScript将会运行。 这允许一些计算在脚本创建时自动发生。




5、零个或多个静态脚本全局变量和函数。 静态脚本全局相当于脚本全局，除了它不能从Java代码访问。 静态函数是可以从脚本中的任何内核或调用函数调用但不暴露于Java API的标准C函数。 如果不需要从Java代码调用脚本全局或函数，则强烈建议将其声明为静态。




一）、设置浮点精度

您可以在脚本中控制所需的浮点精度级别。 如果不需要完整的IEEE 754-2008标准（默认使用），这是有用的。 以下pragmas可以设置不同级别的浮点精度：


1、#pragma rs_fp_full（如果没有指定，则为默认值）：对于IEEE 754-2008标准所述的需要浮点精度的应用程序。




2、#pragma rs_fp_relaxed：对于不需要严格的IEEE 754-2008兼容性并且可以容忍更低精度的应用程序。 此模式可实现对零零脉冲和零向零的清零。




3、#pragma rs_fp_imprecise：对于没有严格精度要求的应用程序。 此模式可以使rs_fp_relaxed中的所有内容与以下内容一起使用：

 1）、导致-0.0的操作可以返回+0.0替代。

 2）、INF和NAN的操作未定义。




大多数应用程序可以使用rs_fp_relaxed而没有任何副作用。 这对于某些架构可能是非常有益的，因为只有轻松的精度才能获得额外的优化（如SIMD CPU指令）。








二、从Java访问RenderScript API

在开发使用RenderScript的Android应用程序时，您可以通过以下两种方式之一从Java访问其API：


1、
```
[android.renderscript](https://developer.android.google.cn/reference/android/renderscript/package-summary.html) - 此类包中的API可用于运行Android
 3.0（API级别11）及更高版本的设备。
```



```

```



```
2、
```
[android.support.v8.renderscript](https://developer.android.google.cn/reference/android/support/v8/renderscript/package-summary.html) - 该软件包中的API可以通过支持库来获得，支持库可以在运行Android
 2.3（API等级9）或更高版本的设备上使用。
```
```



```
```

```
```



```
```
以下是权衡：
```
```



```
```
1、如果您使用支持库API，则应用程序的RenderScript部分将与运行Android 2.3（API级别9）及更高版本的设备兼容，无论使用哪种RenderScript功能。
 这允许您的应用程序使用更多的设备，而不是使用原生（android.renderscript）API。
```
```



```
```

```
```


``2、某些RenderScript功能无法通过Support Library API进行。``


```
```

```
```


``3、如果您使用支持库API，您将比使用使用本机（android.renderscript）API获得（可能显着）更大的APK。``


```
```

```
```


``一）、使用RenderScript支持库API``


```
```
为了使用支持库RenderScript API，您必须配置您的开发环境才能访问它们。 使用以下API需要以下Android SDK工具：
```
```


``1、Android SDK Tools版本22.2或更高版本。``


```
```

```
```


``2、Android SDK Build-tools修订版18.1.0或更高版本。``


```
```

```
```



```
```
您可以在Android SDK管理器中检查并更新这些工具的安装版本。
```
```



```
```

```
```



```
```
使用支持库RenderScript API：
```
```


``1、确保您已安装所需的Android SDK版本和Build Tools版本。``


```
```

```
```


``2、更新Android构建过程的设置以包括RenderScript设置：``

`` 1）、在应用程序模块的应用程序文件夹中打开build.gradle文件。``

`` 2）、将以下RenderScript设置添加到文件中：``

````

```
android {
    compileSdkVersion 23
    buildToolsVersion "23.0.3"

    defaultConfig {
        minSdkVersion 9
        targetSdkVersion 19

        renderscriptTargetApi 18
        renderscriptSupportModeEnabled true

    }
}
```


上面列出的设置控制了Android构建过程中的特定行为：




```
```
1）、renderscriptTargetApi - 指定要生成的字节码版本。 我们建议您将此值设置为能够提供您正在使用的所有功能的最低API级别，并将renderscriptSupportModeEnabled设置为true。
 此设置的有效值是从11到最近发布的API级别的任何整数值。 如果您的应用程序清单中指定的最小SDK版本设置为不同的值，则该值将被忽略，并且构建文件中的目标值用于设置最低SDK版本。
```
```


``2）、renderscriptSupportModeEnabled - 指定生成的字节码如果运行的设备不支持目标版本，则应该恢复到兼容版本。``

3）、buildToolsVersion - 使用Android SDK构建工具的版本。 该值应设置为18.1.0或更高。 如果未指定此选项，则使用最高版本的构建工具版本。 您应始终设置此值，以确保具有不同配置的开发机器之间的构建一致性。




3、在使用RenderScript的应用程序类中，为支持库类添加导入：



```java
import android.support.v8.renderscript.*;
```





三、使用Java代码中的RenderScript

使用Java代码中的RenderScript依赖于android.renderscript或android.support.v8.renderscript包中的API类。 大多数应用程序遵循相同的基本使用模式：


1、初始化RenderScript上下文。 使用create（Context）创建的RenderScript上下文确保可以使用RenderScript，并提供一个对象来控制所有后续RenderScript对象的生命周期。 您应该将上下文创建视为潜在的长期运行操作，因为它可能会在不同的硬件上创建资源;
 如果可能的话，它不应该在应用程序的关键路径中。 通常，应用程序一次只能有一个RenderScript上下文。




2、至少创建一个要传递给脚本的分配。 分配是一种为固定数量的数据提供存储的RenderScript对象。 脚本中的内核将Allocation对象作为其输入和输出，并且可以使用rsGetElementAt_type（）和rsSetElementAt_type（）绑定为脚本全局变量时，使用内核访问Allocation对象。
 分配对象允许将数组从Java代码传递到RenderScript代码，反之亦然。 分配对象通常使用createTyped（）或createFromBitmap（）创建。




3、创建任何必要的脚本。 使用RenderScript时可以使用两种类型的脚本：

 1）、ScriptC：这些是用户定义的脚本，如上面写入RenderScript内核所述。 每个脚本都有一个由RenderScript编译器反映的Java类，以便从Java代码访问脚本; 该类的名称为ScriptC_filename。 例如，如果上面的映射内核位于invert.rs中，并且RenderScript上下文已经位于mRenderScript中，则用于实例化脚本的Java代码将是：



```java
ScriptC_invert invert = new ScriptC_invert(mRenderScript);
```




ScriptIntrinsic：这些内置的RenderScript内核用于常见操作，如高斯模糊，卷积和图像混合。 有关更多信息，请参阅ScriptIntrinsic的子类。





4、使用数据填充分配。 除了使用createFromBitmap（）创建的分配之外，首先创建一个分配的空的数据。 要填充分配，请使用分配中的“复制”方法之一。 “复制”方法是同步的。




5、设置任何必要的脚本全局变量。 您可以使用名为set_globalname的相同ScriptC_filename类中的方法设置全局变量。 例如，为了设置名为threshold的int变量，请使用Java方法set_threshold（int）; 并且为了设置名为lookup的rs_allocation变量，请使用Java方法set_lookup（Allocation）。
 集合方法是异步的。




6、启动适当的内核和调用功能。

启动给定内核的方法反映在相同的ScriptC_filename类中，方法名为forEach_mappingKernelName（）或reduce_reductionKernelName（）。 这些启动是异步的。 根据内核的参数，该方法需要一个或多个分配，所有这些分配必须具有相同的维度。
 默认情况下，内核在这些维度中的每个坐标上执行; 在这些坐标的一个子集上执行一个内核，将适当的Script.LaunchOptions作为forEach或reduce方法的最后一个参数传递。





使用反映在相同ScriptC_filename类中的invoke_functionName方法启动可调用函数。 这些启动是异步的。





7、从Allocation对象和javaFutureType对象中检索数据。 为了从Java代码分配中访问数据，您必须使用“分配”中的“复制”方法之一将该数据复制回Java。 为了获得还原内核的结果，必须使用javaFutureType.get（）方法。
 “copy”和get（）方法是同步的。




8、撕下RenderScript上下文。 您可以使用destroy（）或通过允许RenderScript上下文对象进行垃圾回收来销毁RenderScript上下文。 这导致任何进一步使用属于该上下文的任何对象来抛出异常。




一）、异步执行模型

反映的forEach，invoke，reduce和set方法是异步的 - 每个可以在完成请求的操作之前返回Java。 但是，单个操作按照它们的启动顺序进行序列化。





分配类提供“复制”方法来将数据复制到分配中或从分配中复制数据。 “复制”方法是同步的，并且相对于触摸相同分配的上述任何异步操作而被序列化。





反映的javaFutureType类提供了一个get（）方法来获得缩减的结果。 get（）是同步的，并且相对于缩减（这是异步的）被序列化。








四、单源RenderScript

Android 7.0（API第24级）引入了一种新的编程功能，名为Single-Source RenderScript，其中内核从定义的脚本启动，而不是从Java启动。 这种方法目前仅限于映射内核，在本节中简称为“内核”，以便简化。 此新功能还支持从脚本内部创建类型为rs_allocation的分配。
 即使需要多个内核启动，现在也可以在脚本中实现整个算法。 好处是双重的：更可读的代码，因为它保持一种语言的算法的实现; 并且可能更快的代码，因为在多个内核启动之间的Java和RenderScript之间的转换较少。





在单源RenderScript中，您可以按照“编写RenderScript内核”中的说明编写内核。 然后，您可以编写一个调用rsForEach（）来启动它们的调用函数。 该API将内核函数作为第一个参数，接着是输入和输出分配。 类似的API rsForEachWithOptions（）接受一个类型为rs_script_call_t的额外参数，该参数指定要从内核函数处理的输入和输出分配中的元素的子集。





要启动RenderScript计算，可以从Java调用invokable函数。 按照从Java代码使用RenderScript中的步骤。 在启动适当的内核的步骤中，使用invoke_function_name（）调用invokable函数，这将启动整个计算，包括启动内核。





通常需要分配来保存并将中间结果从一个内核启动传递到另一个内核。 您可以使用rsCreateAllocation（）创建它们。 该API的一个易于使用的形式是rsCreateAllocation_ <T> <W>（...），其中T是元素的数据类型，W是元素的向量宽度。
 API将X，Y和Z的大小作为参数。 对于1D或2D分配，可以省略尺寸为Y或Z的尺寸。 例如，rsCreateAllocation_uchar4（16384）创建了一个16384个元素的1D分配，每个元素的类型为uchar4。





分配由系统自动管理。 您不必明确释放它们。 但是，您可以调用rsClearObject（rs_allocation * alloc）来指示您不再需要对底层分配的handle分配，以便系统尽早释放资源。





编写RenderScript内核部分包含反转图像的示例内核。 下面的示例扩展了使用单源渲染脚本将多个效果应用于图像。 它包括另一个内核，灰度，将彩色图像变成黑白。 可调用函数process（）然后将这两个内核连续地应用于输入图像，并产生输出图像。 输入和输出的分配作为rs_allocation类型的参数传递。




```java
// File: singlesource.rs

#pragma version(1)
#pragma rs java_package_name(com.android.rssample)

static const float4 weight = {0.299f, 0.587f, 0.114f, 0.0f};

uchar4 RS_KERNEL invert(uchar4 in, uint32_t x, uint32_t y) {
  uchar4 out = in;
  out.r = 255 - in.r;
  out.g = 255 - in.g;
  out.b = 255 - in.b;
  return out;
}

uchar4 RS_KERNEL greyscale(uchar4 in) {
  const float4 inF = rsUnpackColor8888(in);
  const float4 outF = (float4){ dot(inF, weight) };
  return rsPackColorTo8888(outF);
}

void process(rs_allocation inputImage, rs_allocation outputImage) {
  const uint32_t imageWidth = rsAllocationGetDimX(inputImage);
  const uint32_t imageHeight = rsAllocationGetDimY(inputImage);
  rs_allocation tmp = rsCreateAllocation_uchar4(imageWidth, imageHeight);
  rsForEach(invert, inputImage, tmp);
  rsForEach(greyscale, tmp, outputImage);
}
```


您可以从Java调用process（）函数，如下所示：





```java
// File SingleSource.java

RenderScript RS = RenderScript.create(context);
ScriptC_singlesource script = new ScriptC_singlesource(RS);
Allocation inputAllocation = Allocation.createFromBitmapResource(
    RS, getResources(), R.drawable.image);
Allocation outputAllocation = Allocation.createTyped(
    RS, inputAllocation.getType(),
    Allocation.USAGE_SCRIPT | Allocation.USAGE_IO_OUTPUT);
script.invoke_process(inputAllocation, outputAllocation);
```


该示例显示了如何在RenderScript语言本身中完全实现涉及两个内核启动的算法。 没有单源RenderScript，您将不得不从Java代码启动内核，将内核启动与内核定义分开，并使其难以理解整个算法。 单源RenderScript代码不仅易于阅读，还可以消除跨内核启动的Java和脚本之间的转换。 一些迭代算法可能会启动数百次内核，从而使这种转换的开销相当可观。









五、缩小内核深度

减少是将数据集合到一个值中的过程。 这是并行编程中的一个有用的原语，应用如下：


1、计算所有数据的总和或乘积。




2、计算所有数据的逻辑运算（和，或xor）。




3、找到数据中的最小值或最大值。




4、搜索数据中的特定值或特定值的坐标。




在Android 7.0（API级别24）及更高版本中，RenderScript支持缩小内核以允许高效的用户编写的减少算法。 您可以在1,2或3维的输入上启动还原内核。





上面的例子显示了一个简单的addint还原内核。 这是一个更复杂的findMinAndMax还原内核，它可以在一维分配中找到最小和最大长值的位置：




```java
#define LONG_MAX (long)((1UL << 63) - 1)
#define LONG_MIN (long)(1UL << 63)

#pragma rs reduce(findMinAndMax) \
  initializer(fMMInit) accumulator(fMMAccumulator) \
  combiner(fMMCombiner) outconverter(fMMOutConverter)

// Either a value and the location where it was found, or INITVAL.
typedef struct {
  long val;
  int idx;     // -1 indicates INITVAL
} IndexedVal;

typedef struct {
  IndexedVal min, max;
} MinAndMax;

// In discussion below, this initial value { { LONG_MAX, -1 }, { LONG_MIN, -1 } }
// is called INITVAL.
static void fMMInit(MinAndMax *accum) {
  accum->min.val = LONG_MAX;
  accum->min.idx = -1;
  accum->max.val = LONG_MIN;
  accum->max.idx = -1;
}

//----------------------------------------------------------------------
// In describing the behavior of the accumulator and combiner functions,
// it is helpful to describe hypothetical functions
//   IndexedVal min(IndexedVal a, IndexedVal b)
//   IndexedVal max(IndexedVal a, IndexedVal b)
//   MinAndMax  minmax(MinAndMax a, MinAndMax b)
//   MinAndMax  minmax(MinAndMax accum, IndexedVal val)
//
// The effect of
//   IndexedVal min(IndexedVal a, IndexedVal b)
// is to return the IndexedVal from among the two arguments
// whose val is lesser, except that when an IndexedVal
// has a negative index, that IndexedVal is never less than
// any other IndexedVal; therefore, if exactly one of the
// two arguments has a negative index, the min is the other
// argument. Like ordinary arithmetic min and max, this function
// is commutative and associative; that is,
//
//   min(A, B) == min(B, A)               // commutative
//   min(A, min(B, C)) == min((A, B), C)  // associative
//
// The effect of
//   IndexedVal max(IndexedVal a, IndexedVal b)
// is analogous (greater . . . never greater than).
//
// Then there is
//
//   MinAndMax minmax(MinAndMax a, MinAndMax b) {
//     return MinAndMax(min(a.min, b.min), max(a.max, b.max));
//   }
//
// Like ordinary arithmetic min and max, the above function
// is commutative and associative; that is:
//
//   minmax(A, B) == minmax(B, A)                  // commutative
//   minmax(A, minmax(B, C)) == minmax((A, B), C)  // associative
//
// Finally define
//
//   MinAndMax minmax(MinAndMax accum, IndexedVal val) {
//     return minmax(accum, MinAndMax(val, val));
//   }
//----------------------------------------------------------------------

// This function can be explained as doing:
//   *accum = minmax(*accum, IndexedVal(in, x))
//
// This function simply computes minimum and maximum values as if
// INITVAL.min were greater than any other minimum value and
// INITVAL.max were less than any other maximum value.  Note that if
// *accum is INITVAL, then this function sets
//   *accum = IndexedVal(in, x)
//
// After this function is called, both accum->min.idx and accum->max.idx
// will have nonnegative values:
// - x is always nonnegative, so if this function ever sets one of the
//   idx fields, it will set it to a nonnegative value
// - if one of the idx fields is negative, then the corresponding
//   val field must be LONG_MAX or LONG_MIN, so the function will always
//   set both the val and idx fields
static void fMMAccumulator(MinAndMax *accum, long in, int x) {
  IndexedVal me;
  me.val = in;
  me.idx = x;

  if (me.val <= accum->min.val)
    accum->min = me;
  if (me.val >= accum->max.val)
    accum->max = me;
}

// This function can be explained as doing:
//   *accum = minmax(*accum, *val)
//
// This function simply computes minimum and maximum values as if
// INITVAL.min were greater than any other minimum value and
// INITVAL.max were less than any other maximum value.  Note that if
// one of the two accumulator data items is INITVAL, then this
// function sets *accum to the other one.
static void fMMCombiner(MinAndMax *accum,
                        const MinAndMax *val) {
  if ((accum->min.idx < 0) || (val->min.val < accum->min.val))
    accum->min = val->min;
  if ((accum->max.idx < 0) || (val->max.val > accum->max.val))
    accum->max = val->max;
}

static void fMMOutConverter(int2 *result,
                            const MinAndMax *val) {
  result->x = val->min.idx;
  result->y = val->max.idx;
}
```


为了运行还原内核，RenderScript运行时创建一个或多个称为累加器数据项的变量来保存还原过程的状态。 RenderScript运行时以选择最大化性能的方式选择累加器数据项的数量。 累加器数据项（accumType）的类型由内核的累加器函数确定 - 该函数的第一个参数是指向累加器数据项的指针。 默认情况下，每个累加器数据项被初始化为零（好像通过memset）; 但是，您可以编写一个初始化函数来执行不同的操作。




示例：在添加内核中，累加器数据项（类型int）用于将输入值相加。 没有初始化器功能，每个累加器数据项被初始化为零。





示例：在findMinAndMax内核中，累加器数据项（类型MinAndMax）用于跟踪到目前为止发现的最小值和最大值。 有一个初始化函数分别将它们设置为LONG_MAX和LONG_MIN; 并将这些值的位置设置为-1，表示这些值实际上并未存在于已处理的输入的（空）部分中。





RenderScript会在输入中为每个坐标调用累加器函数一次。 通常，您的功能应该根据输入以某种方式更新累加器数据项。





示例：在添加内核中，累加器函数将输入元素的值添加到累加器数据项。





示例：在findMinAndMax内核中，累加器功能检查输入元素的值是否小于或等于累加器数据项中记录的最小值和/或大于或等于记录在累加器中的最大值 数据项，并相应地更新累加器数据项。





对于输入中的每个坐标，累加器函数被调用一次后，RenderScript必须将累加器数据项组合成一个累加器数据项。 您可以编写组合器功能来执行此操作。 如果累加器函数具有单个输入并且没有特殊参数，则不需要编写组合器函数; RenderScript将使用累加器函数来组合累加器数据项。
 （如果这个默认行为不是你想要的，你可能还会写一个组合器功能。）





示例：在addint内核中没有组合器功能，因此将使用累加器函数。 这是正确的行为，因为如果我们将值的集合分成两部分，并且我们分别将这两个数值相加，则将这两个总和相加，与将整个集合相加一样。





示例：在findMinAndMax内核中，组合器功能检查“源”累加器数据项* val中记录的最小值是否小于“目的地”累加器数据项* accum中记录的最小值，并更新* accum因此。 它为最大值做类似的工作。 这将更新*累加到所有输入值已累积到*
 accum而不是一些进入*。





在所有累加器数据项被组合后，RenderScript确定还原的结果返回到Java。 你可以写一个outconverter函数来做到这一点。 如果希望将合并的累加器数据项的最终值作为减少的结果，则不需要编写outconverter函数。





示例：在添加内核中，没有转换器功能。 组合数据项的最终值是输入的所有元素的总和，这是我们要返回的值。





示例：在findMinAndMax内核中，outconverter函数初始化一个int2结果值，以保存由所有累加器数据项组合而产生的最小值和最大值的位置。





一）、编写一个还原内核

#pragma rs reduce通过指定其名称以及构成内核的函数的名称和角色来定义还原内核。 所有这些功能必须是静态的。 还原内核总是需要一个累加器函数; 您可以省略部分或全部其他功能，具体取决于您希望内核执行的操作。



```java
#pragma rs reduce(kernelName) \
  initializer(initializerName) \
  accumulator(accumulatorName) \
  combiner(combinerName) \
  outconverter(outconverterName)
```


#pragma中项目的含义如下：


1、reduce（kernelName）（mandatory）：指定正在定义一个还原内核。 反映的Java方法reduce_kernelName将启动内核。




2、initializer（initializerName）（可选）：指定此还原内核的初始化函数的名称。 启动内核时，RenderScript会为每个累加器数据项调用此函数一次。 该函数必须如下定义：


```java
static void initializerName(accumType *accum) { … }
```


accum是一个指向累加器数据项的指针，用于此函数初始化。





如果您不提供初始化函数，则RenderScript会将每个累加器数据项初始化为零（如同memset一样），表现为像初始化函数一样，如下所示：



```java
static void initializerName(accumType *accum) {
  memset(accum, 0, sizeof(*accum));
}
```


3、累加器（cumulativeName）（必需）：指定此还原内核的累加器函数的名称。 启动内核时，RenderScript会在输入中为每个坐标调用此功能一次，以某种方式根据输入更新累加器数据项。 该函数必须如下定义：


```java
static void accumulatorName(accumType *accum,
                            in1Type in1, …, inNType inN
                            [, specialArguments]) { … }
```


accum是指向该函数修改的累加器数据项的指针。 in1到inN是一个或多个参数，它们根据传递给内核启动的输入自动填充，每个输入一个参数。 累加器功能可以选择采用任何特殊参数。





组合器（combinerName）（可选）：指定此还原内核的组合器功能的名称。 在RenderScript对输入中的每个坐标调用累加器函数一次后，根据需要调用此函数多次，以将所有累加器数据项组合到单个累加器数据项中。 该函数必须如下定义：



```java
static void combinerName(accumType *accum, const accumType *other) { … }
```


accum是指向此函数进行修改的“目标”累加器数据项的指针。 另一个是一个指向“source”累加器数据项的指针，用于将此函数“合并”为* accum。





注意：* accum，* other或两者都可能被初始化，但从未传递给累加器函数; 也就是说，一个或两个从未根据任何输入数据进行更新。 例如，在findMinAndMax内核中，组合函数fMMCombiner显式地检查idx <0，因为它指示这样一个累加器数据项，其值为INITVAL。





如果您不提供组合器功能，则RenderScript会在其位置使用累加器函数，表现为像组合器函数如下所示：



```java
static void combinerName(accumType *accum, const accumType *other) {
  accumulatorName(accum, *other);
}
```


如果内核具有多个输入，如果输入数据类型与累加器数据类型不相同，或者累加器函数需要一个或多个特殊参数，则组合器函数是必需的。





4、outconverter（outconverterName）（可选）：指定此缩减内核的outconverter函数的名称。 在RenderScript结合所有累加器数据项之后，它调用此函数来确定还原到Java的结果。 该函数必须如下定义：


```java
static void outconverterName(resultType *result, const accumType *accum) { … }
```


结果是指向结果数据项的指针（由RenderScript运行时分配而不是由RenderScript运行时初始化），以便此函数使用缩减的结果进行初始化。 resultType是该数据项的类型，不需要与accumType相同。 accum是指向由组合器函数计算的最终累加器数据项的指针。





如果不提供outconverter功能，RenderScript将最终的累加器数据项复制到结果数据项，表现为如果有一个outconverter函数，如下所示：



```java
static void outconverterName(accumType *result, const accumType *accum) {
  *result = *accum;
}
```


如果您想要的结果类型不同于累加器数据类型，则outconverter函数是强制性的。





请注意，内核具有输入类型，累加器数据项类型和结果类型，不需要相同。 例如，在findMinAndMax内核中，输入类型long，累加器数据项类型MinAndMax和结果类型int2都不同。








你不能假设什么？


您不能依赖于给定内核启动的RenderScript创建的累加器数据项的数量。 不能保证具有相同输入的同一内核的两次启动将创建相同数量的累加器数据项。





您不能依赖于RenderScript调用初始化程序，累加器和组合器函数的顺序; 甚至可能并行调用其中的一些。 不能保证具有相同输入的同一内核的两次启动将遵循相同的顺序。 唯一的保证是只有初始化器功能才会看到未初始化的累加器数据项。 例如：


1、不能保证在累加器函数被调用之前，所有累加器数据项都将被初始化，尽管它只会在初始化的累加器数据项上被调用。




2、输入元素传递给累加器函数的顺序不能保证。




3、在组合器功能被调用之前，不能保证已经为所有输入元件调用了累加器函数。




一个后果是findMinAndMax内核不是确定性的：如果输入包含多个相同最小值或最大值的出现，那么您无法知道内核会发现哪个内容。








你必须保证什么？


因为RenderScript系统可以选择以许多不同的方式执行内核，所以您必须遵循某些规则，以确保您的内核按照所需的方式运行。 如果您不遵守这些规则，您可能会得到不正确的结果，非确定性行为或运行时错误。





下面的规则经常表示两个累加器数据项必须具有“相同的值”。 这是什么意思？ 这取决于你想要的内核做什么。 对于诸如addint的数学简化来说，“相同”通常意味着数学平等。 对于“pick any”搜索，例如findMinAndMax（“找到最小和最大输入值的位置”），其中可能出现多个相同的输入值，给定输入值的所有位置必须被视为“相同”。 您可以编写一个类似的内核来“找到最左边的最小和最大输入值的位置”，其中（例如）位置100的最小值优于位置200上相同的最小值; 对于该内核，“相同”意味着相同的位置，而不仅仅是相同的值，并且累加器和组合器函数将不同于findMinAndMax的函数。





1、初始化函数必须创建一个标识值。也就是说，如果I和A是由初始化函数初始化的累加器数据项，并且我从未被传递给累加器函数（但A可能已经被传递），那么：

 1)、combinerName（＆A，＆I）必须离开与A一样。

 2）、combinerName（＆I，＆A）I必须离开时与A相同。




示例：在添加内核中，累加器数据项被初始化为零。 该内核的组合器功能执行加法; 零是加法的标识值。





示例：在findMinAndMax内核中，累加器数据项被初始化为INITVAL。


 1）、fMMCombiner（＆A，＆I）离开A一样，因为I是INITVAL。

 2）、fMMCombiner（＆I，＆A）将I设置为A，因为I是INITVAL。

因此，INITVAL确实是一个身份值。





2、组合器功能必须是可交换的。 也就是说，如果A和B是初始化函数初始化的累加器数据项，并且可能已被传递给累加器函数为零次或更多次，那么combinerName（＆A，＆B）必须将A设置为与combinerName（＆B ，＆A）设置B一样的值.




示例：在添加内核中，组合器函数添加两个累加器数据项值; 附加的是交换。





示例：在findMinAndMax内核中，



```java
fMMCombiner(&A, &B)
```
是相同的跟：


```java
A = minmax(A, B)
```
而minmax是可交换的，所以fMMCombiner也是。




也就是说，如果A，B和C是由初始化器函数初始化的累加器数据项，并且可能已经被传递给累加器函数零次或更多次，则以下两个代码序列必须将A设置为相同的值：

```java
combinerName(&A, &B);
combinerName(&A, &C);
```


```java
combinerName(&B, &C);
combinerName(&A, &B);
```


示例：在添加内核中，组合器函数添加两个累加器数据项值：

```java
A = A + B
A = A + C
// Same as
//   A = (A + B) + C
```

```java
B = B + C
A = A + B
// Same as
//   A = A + (B + C)
//   B = B + C
```
添加是关联的，因此组合器功能也是。



示例：在findMinAndMax内核中，



```java
fMMCombiner(&A, &B)
```
是相同的跟：


```java
A = minmax(A, B)
```
所以这两个序列是：


```java
A = minmax(A, B)
A = minmax(A, C)
// Same as
//   A = minmax(minmax(A, B), C)
```

```java
B = minmax(B, C)
A = minmax(A, B)
// Same as
//   A = minmax(A, minmax(B, C))
//   B = minmax(B, C)
```
minmax是关联的，所以fMMCombiner也是。






3、累加器功能和组合器功能必须遵守基本的折叠规则。 也就是说，如果A和B是累加器数据项，则A由初始化器函数初始化，并且可能已经被传递给累加器函数零次或更多次，B未被初始化，args是输入参数和特殊的列表 对于累加器函数的特定调用的参数，则以下两个代码序列必须将A设置为相同的值：


```java
accumulatorName(&A, args);  // statement 1
```

```java
initializerName(&B);        // statement 2
accumulatorName(&B, args);  // statement 3
combinerName(&A, &B);       // statement 4
```


示例：在添加内核中，对于输入值V：

1、声明1与A + = V相同。

2、声明2与B = 0相同。

3、声明3与B + = V相同，此与B = V相同。

4、声明4与A + = B相同，此与A + = V相同。

语句1和4将A设置为相同的值，因此该内核遵循基本的折叠规则。




示例：在findMinAndMax内核中，对于坐标X处的输入值V：

1、声明1与A = minmax（A，IndexedVal（V，X））相同）。

2、声明2与B = INITVAL相同。

3、声明3相同。


```java
B = minmax(B, IndexedVal(V, X))
```
这是因为B是初始值，与以下相同：


```java
B = IndexedVal(V, X)
```


4、声明4与以下相同：


```java
A = minmax(A, B)
```
此下面的相同：


```java
A = minmax(A, IndexedVal(V, X))
```
语句1和4将A设置为相同的值，因此该内核遵循基本的折叠规则。



二）、从Java代码调用还原内核

对于在文件filename.rs中定义的名为kernelName的还原内核，ScriptC_filename类中反映了三种方法：



```java
// Method 1
public javaFutureType reduce_kernelName(Allocation ain1, …,
                                        Allocation ainN);

// Method 2
public javaFutureType reduce_kernelName(Allocation ain1, …,
                                        Allocation ainN,
                                        Script.LaunchOptions sc);

// Method 3
public javaFutureType reduce_kernelName(devecSiIn1Type[] in1, …,
                                        devecSiInNType[] inN);
```


以下是调用添加内核的一些示例：



```java
ScriptC_example script = new ScriptC_example(mRenderScript);

// 1D array
//   and obtain answer immediately
int input1[] = …;
int sum1 = script.reduce_addint(input1).get();  // Method 3

// 2D allocation
//   and do some additional work before obtaining answer
Type.Builder typeBuilder =
  new Type.Builder(RS, Element.I32(RS));
typeBuilder.setX(…);
typeBuilder.setY(…);
Allocation input2 = createTyped(RS, typeBuilder.create());
populateSomehow(input2);  // fill in input Allocation with data
script.result_int result2 = script.reduce_addint(input2);  // Method 1
doSomeAdditionalWork(); // might run at same time as reduction
int sum2 = result2.get();
```


方法1在内核的累加器函数中的每个输入参数都有一个输入分配参数。 RenderScript运行时检查以确保所有输入分配具有相同的维度，并且每个输入分配的Element类型与累加器函数原型的相应输入参数的Element类型相匹配。 如果任何这些检查失败，RenderScript会引发异常。 内核在这些维度中的每个坐标上执行。





方法2与方法1相同，只是方法2使用可用于将内核执行限制到坐标的子集的附加参数sc。





方法3与方法1相同，只是代替分配输入，它需要Java数组输入。 这是一种方便，可以节省您编写代码以显式创建分配并从Java数组复制数据。 然而，使用方法3而不是方法1不会增加代码的性能。 对于每个输入数组，方法3创建一个具有适当的Element类型和setAutoPadding（boolean）的临时一维分配，并将数组复制到分配，就好像通过适当的分配方式的copyFrom（）方法。 然后调用方法1，传递这些临时分配。





注意：如果您的应用程序将使用相同的数组或具有相同维度和元素类型的不同数组进行多个内核调用，则可以通过显式创建，填充和重用分配来提高性能，而不是使用方法3。





javaFutureType是反映缩小方法的返回类型，是ScriptC_filename类中反映的静态嵌套类。 它代表了还原内核运行的未来结果。 要获取运行的实际结果，请调用该类的get（）方法，该方法返回类型为javaResultType的值。 get（）是同步的。



```java
public class ScriptC_filename extends ScriptC {
  public static class javaFutureType {
    public javaResultType get() { … }
  }
}
```


javaResultType由outconverter函数的resultType决定。 除非resultType是无符号类型（标量，向量或数组），否则javaResultType是直接对应的Java类型。 如果resultType是一个无符号类型，并且有较大的Java签名类型，那么javaResultType是较大的Java签名类型; 否则，它是直接相应的Java类型。 例如：


1、如果resultType为int，int2或int [15]，则javaResultType为int，Int2或int []。 resultType的所有值都可以由javaResultType表示。

2、如果resultType是uint，uint2或uint [15]，那么javaResultType是long，Long2或long []。 resultType的所有值都可以由javaResultType表示。

3、如果resultType是ulong，ulong2或ulong [15]，那么javaResultType是long，Long2或long []。 resultType的某些值不能由javaResultType表示。




javaFutureType是对应于outconverter函数的resultType的未来结果类型。


1、如果resultType不是数组类型，那么javaFutureType是result_resultType。

2、如果resultType是具有类型为memberType的成员的长度为Count的数组，那么javaFutureType为resultArrayCount_memberType。




例如：


```java
public class ScriptC_filename extends ScriptC {
  // for kernels with int result
  public static class result_int {
    public int get() { … }
  }

  // for kernels with int[10] result
  public static class resultArray10_int {
    public int[] get() { … }
  }

  // for kernels with int2 result
  //   note that the Java type name "Int2" is not the same as the script type name "int2"
  public static class result_int2 {
    public Int2 get() { … }
  }

  // for kernels with int2[10] result
  //   note that the Java type name "Int2" is not the same as the script type name "int2"
  public static class resultArray10_int2 {
    public Int2[] get() { … }
  }

  // for kernels with uint result
  //   note that the Java type "long" is a wider signed type than the unsigned script type "uint"
  public static class result_uint {
    public long get() { … }
  }

  // for kernels with uint[10] result
  //   note that the Java type "long" is a wider signed type than the unsigned script type "uint"
  public static class resultArray10_uint {
    public long[] get() { … }
  }

  // for kernels with uint2 result
  //   note that the Java type "Long2" is a wider signed type than the unsigned script type "uint2"
  public static class result_uint2 {
    public Long2 get() { … }
  }

  // for kernels with uint2[10] result
  //   note that the Java type "Long2" is a wider signed type than the unsigned script type "uint2"
  public static class resultArray10_uint2 {
    public Long2[] get() { … }
  }
}
```


如果javaResultType是一个对象类型（包括数组类型），则对同一个实例上的每个对javaFutureType.get（）的调用将返回相同的对象。





如果javaResultType不能表示类型为resultType的所有值，并且还原内核产生不可映射的值，那么javaFutureType.get（）将抛出异常。







方法3和devecSiInXType


devecSiInXType是对应于累加器函数的相应参数的inXType的Java类型。 除非inXType是无符号类型或向量类型，否则devecSiInXType是直接相应的Java类型。 如果inXType是未签名的标量类型，则devecSiInXType是直接对应于相同大小的带符号标量类型的Java类型。 如果inXType是带符号向量类型，则devecSiInXType是直接对应于向量组件类型的Java类型。 如果inXType是无符号向量类型，则devecSiInXType是直接对应于与向量组件类型相同大小的带符号标量类型的Java类型。
 例如：


1、如果inXType为int，则devecSiInXType为int。

2、如果inXType是int2，那么devecSiInXType是int。 数组是一个扁平化的表示形式：它具有两倍的标量元素，分配具有2分量矢量元素。 这与分配方式的copyFrom（）方法相同。

3、如果inXType是uint，那么deviceSiInXType是int。 Java数组中的有符号值被解释为分配中相同位模式的无符号值。 这与分配方式的copyFrom（）方法相同。

4、如果inXType是uint2，那么deviceSiInXType是int。 这是处理int2和uint的方式的组合：数组是一个展平表示，Java数组有符号值被解释为RenderScript无符号元素值。




请注意，对于方法3，输入类型的处理方式与结果类型不同：


1、脚本的向量输入在Java侧平坦化，而脚本的向量结果不是。

2、脚本的无符号输入在Java端表示为相同大小的签名输入，而脚本的无符号结果在Java侧被表示为扩展的签名类型（除了ulong的情况）。




三）、更多示例减少内核


```java
#pragma rs reduce(dotProduct) \
  accumulator(dotProductAccum) combiner(dotProductSum)

// Note: No initializer function -- therefore,
// each accumulator data item is implicitly initialized to 0.0f.

static void dotProductAccum(float *accum, float in1, float in2) {
  *accum += in1*in2;
}

// combiner function
static void dotProductSum(float *accum, const float *val) {
  *accum += *val;
}
```


```java
// Find a zero Element in a 2D allocation; return (-1, -1) if none
#pragma rs reduce(fz2) \
  initializer(fz2Init) \
  accumulator(fz2Accum) combiner(fz2Combine)

static void fz2Init(int2 *accum) { accum->x = accum->y = -1; }

static void fz2Accum(int2 *accum,
                     int inVal,
                     int x /* special arg */,
                     int y /* special arg */) {
  if (inVal==0) {
    accum->x = x;
    accum->y = y;
  }
}

static void fz2Combine(int2 *accum, const int2 *accum2) {
  if (accum2->x >= 0) *accum = *accum2;
}
```


```java
// Note that this kernel returns an array to Java
#pragma rs reduce(histogram) \
  accumulator(hsgAccum) combiner(hsgCombine)

#define BUCKETS 256
typedef uint32_t Histogram[BUCKETS];

// Note: No initializer function --
// therefore, each bucket is implicitly initialized to 0.

static void hsgAccum(Histogram *h, uchar in) { ++(*h)[in]; }

static void hsgCombine(Histogram *accum,
                       const Histogram *addend) {
  for (int i = 0; i < BUCKETS; ++i)
    (*accum)[i] += (*addend)[i];
}

// Determines the mode (most frequently occurring value), and returns
// the value and the frequency.
//
// If multiple values have the same highest frequency, returns the lowest
// of those values.
//
// Shares functions with the histogram reduction kernel.
#pragma rs reduce(mode) \
  accumulator(hsgAccum) combiner(hsgCombine) \
  outconverter(modeOutConvert)

static void modeOutConvert(int2 *result, const Histogram *h) {
  uint32_t mode = 0;
  for (int i = 1; i < BUCKETS; ++i)
    if ((*h)[i] > (*h)[mode]) mode = i;
  result->x = mode;
  result->y = (*h)[mode];
}
```








