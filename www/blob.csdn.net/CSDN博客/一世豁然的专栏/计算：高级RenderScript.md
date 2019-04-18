# 计算：高级RenderScript - 一世豁然的专栏 - CSDN博客





2017年05月11日 16:36:00[一世豁然](https://me.csdn.net/Explorer_day)阅读数：452








因为使用RenderScript的应用程序仍然在Android VM内部运行，所以您可以访问您熟悉的所有框架API，但在适当的时候可以使用RenderScript。 为了促进框架和RenderScript运行时之间的这种交互，还提供了一个代码中间层，以便于在两个级别的代码之间进行通信和内存管理。 本文档将详细介绍有关这些不同层次的代码以及如何在Android VM和RenderScript运行时之间共享内存。




一、RenderScript运行时层

您的RenderScript代码在紧凑且定义良好的运行时层中进行编译和执行。 RenderScript运行时API为强大的计算提供了支持，便携式和可自动扩展到处理器上可用的核心数量。





注意：NDK中的标准C函数必须保证在CPU上运行，因此RenderScript无法访问这些库，因为RenderScript旨在运行在不同类型的处理器上。





您可以在Android项目的src /目录中的.rs和.rsh文件中定义RenderScript代码。 该代码由作为Android构建的一部分运行的llvm编译器编译为中间字节码。 当您的应用程序在设备上运行时，字节码然后由设备上的另一个llvm编译器编译（即时）到机器代码。 机器代码针对设备进行了优化，并且还被缓存，因此后续使用启用RenderScript的应用程序不会重新编译字节码。





RenderScript运行时库的一些主要功能包括：


1、内存分配请求功能。




2、大量的数学函数集合，具有标量和向量类型的许多常用例程的重载版本。 可以使用加法，乘法，点积和交叉乘积等操作以及原子算术和比较功能。




3、原始数据类型和向量，矩阵例程以及日期和时间例程的转换程序。




4、用于支持RenderScript系统的数据类型和结构，例如用于定义二，三或四向量的向量类型。




5、日志函数。




有关可用功能的更多信息，请参阅RenderScript运行时API参考。








一、反射层

反射层是Android构建工具生成的一组类，可以从Android框架访问RenderScript运行时。 此层还提供了方法和构造函数，允许您为RenderScript代码中定义的指针分配和使用内存。 以下列表描述了反映的主要组件：


1、您创建的每个.rs文件都生成一个名为project_root / gen / package / name / ScriptC_renderscript_filename类的脚本类。 此文件是.rs文件的.java版本，您可以从Android框架调用该文件。 此类包含.rs文件所反映的以下项目：

 1）、非静态功能。

 2）、非静态全局RenderScript变量。 为每个变量生成访问器方法，因此您可以从Android框架读取和写入RenderScript变量。 如果全局变量在RenderScript运行时层初始化，那么这些值将用于初始化Android框架层中的相应值。 如果全局变量被标记为const，则不会生成set方法。

 3）、全局指针。




2、一个结构体被反映到它自己的名为project_root / gen / package / name / ScriptField_struct_name的类中，它扩展了Script.FieldBase。 该类表示一个struct的数组，它允许你为这个结构体的一个或多个实例分配内存。




一）、函数

函数反映在脚本类本身，位于project_root / gen / package / name / ScriptC_renderscript_filename中。 例如，如果您在RenderScript代码中定义了以下函数：




```java
void touch(float x, float y, float pressure, int id) {
    if (id >= 10) {
        return;
    }

    touchPos[id].x = x;
    touchPos[id].y = y;
    touchPressure[id] = pressure;
}
```


那么将生成以下Java代码：



```java
public void invoke_touch(float x, float y, float pressure, int id) {
    FieldPacker touch_fp = new FieldPacker(16);
    touch_fp.addF32(x);
    touch_fp.addF32(y);
    touch_fp.addF32(pressure);
    touch_fp.addI32(id);
    invoke(mExportFuncIdx_touch, touch_fp);
}
```


函数不能具有返回值，因为RenderScript系统被设计为异步的。 当您的Android框架代码调用到RenderScript时，调用将排队，并在可能的情况下执行。 该限制允许RenderScript系统在不间断的情况下运行，并提高效率。 如果允许函数具有返回值，则调用将阻塞，直到返回值为止。




如果您希望RenderScript代码将值发送回Android框架，请使用rsSendToClient（）函数。





二）、变量

受支持类型的变量反映在脚本类本身中，位于project_root / gen / package / name / ScriptC_renderscript_filename中。 为每个变量生成一组访问器方法。 例如，如果您在RenderScript代码中定义了以下变量：




```java
uint32_t unsignedInteger = 1;
```


那么将生成以下Java代码：



```java
private long mExportVar_unsignedInteger;
public void set_unsignedInteger(long v){
    mExportVar_unsignedInteger = v;
    setVar(mExportVarIdx_unsignedInteger, v);
}

public long get_unsignedInteger(){
    return mExportVar_unsignedInteger;
}
```



三）、结构

结构体被反映在自己的类中，位于<project_root> / gen / com / example / renderscript / ScriptField_struct_name中。 此类表示一个结构体的数组，并允许您为指定数量的结构体分配内存。 例如，如果您定义了以下结构体：



```java
typedef struct Point {
    float2 position;
    float size;
} Point_t;
```


那么在ScriptField_Point.java中生成以下代码：



```java
package com.example.android.rs.hellocompute;

import android.renderscript.*;
import android.content.res.Resources;

  /**
  * @hide
  */
public class ScriptField_Point extends android.renderscript.Script.FieldBase {

    static public class Item {
        public static final int sizeof = 12;

        Float2 position;
        float size;

        Item() {
            position = new Float2();
        }
    }

    private Item mItemArray[];
    private FieldPacker mIOBuffer;
    public static Element createElement(RenderScript rs) {
        Element.Builder eb = new Element.Builder(rs);
        eb.add(Element.F32_2(rs), "position");
        eb.add(Element.F32(rs), "size");
        return eb.create();
    }

    public  ScriptField_Point(RenderScript rs, int count) {
        mItemArray = null;
        mIOBuffer = null;
        mElement = createElement(rs);
        init(rs, count);
    }

    public  ScriptField_Point(RenderScript rs, int count, int usages) {
        mItemArray = null;
        mIOBuffer = null;
        mElement = createElement(rs);
        init(rs, count, usages);
    }

    private void copyToArray(Item i, int index) {
        if (mIOBuffer == null) mIOBuffer = new FieldPacker(Item.sizeof * getType().getX()/* count
        */);
        mIOBuffer.reset(index * Item.sizeof);
        mIOBuffer.addF32(i.position);
        mIOBuffer.addF32(i.size);
    }

    public void set(Item i, int index, boolean copyNow) {
        if (mItemArray == null) mItemArray = new Item[getType().getX() /* count */];
        mItemArray[index] = i;
        if (copyNow)  {
            copyToArray(i, index);
            mAllocation.setFromFieldPacker(index, mIOBuffer);
        }
    }

    public Item get(int index) {
        if (mItemArray == null) return null;
        return mItemArray[index];
    }

    public void set_position(int index, Float2 v, boolean copyNow) {
        if (mIOBuffer == null) mIOBuffer = new FieldPacker(Item.sizeof * getType().getX()/* count */);
        if (mItemArray == null) mItemArray = new Item[getType().getX() /* count */];
        if (mItemArray[index] == null) mItemArray[index] = new Item();
        mItemArray[index].position = v;
        if (copyNow) {
            mIOBuffer.reset(index * Item.sizeof);
            mIOBuffer.addF32(v);
            FieldPacker fp = new FieldPacker(8);
            fp.addF32(v);
            mAllocation.setFromFieldPacker(index, 0, fp);
        }
    }

    public void set_size(int index, float v, boolean copyNow) {
        if (mIOBuffer == null) mIOBuffer = new FieldPacker(Item.sizeof * getType().getX()/* count */);
        if (mItemArray == null) mItemArray = new Item[getType().getX() /* count */];
        if (mItemArray[index] == null) mItemArray[index] = new Item();
        mItemArray[index].size = v;
        if (copyNow)  {
            mIOBuffer.reset(index * Item.sizeof + 8);
            mIOBuffer.addF32(v);
            FieldPacker fp = new FieldPacker(4);
            fp.addF32(v);
            mAllocation.setFromFieldPacker(index, 1, fp);
        }
    }

    public Float2 get_position(int index) {
        if (mItemArray == null) return null;
        return mItemArray[index].position;
    }

    public float get_size(int index) {
        if (mItemArray == null) return 0;
        return mItemArray[index].size;
    }

    public void copyAll() {
        for (int ct = 0; ct < mItemArray.length; ct++) copyToArray(mItemArray[ct], ct);
        mAllocation.setFromFieldPacker(0, mIOBuffer);
    }

    public void resize(int newSize) {
        if (mItemArray != null)  {
            int oldSize = mItemArray.length;
            int copySize = Math.min(oldSize, newSize);
            if (newSize == oldSize) return;
            Item ni[] = new Item[newSize];
            System.arraycopy(mItemArray, 0, ni, 0, copySize);
            mItemArray = ni;
        }
        mAllocation.resize(newSize);
        if (mIOBuffer != null) mIOBuffer = new FieldPacker(Item.sizeof * getType().getX()/* count */);
    }
}
```


生成的代码提供给您，以便为RenderScript运行时请求的结构分配内存并与内存中的结构体进行交互。 每个struct的类定义了以下方法和构造函数：

1、重载的构造函数，允许您分配内存。 ScriptField_struct_name（RenderScript rs，int count）构造函数允许您使用count参数定义要为内存分配的结构数。 ScriptField_struct_name（RenderScript rs，int count，int usages）构造函数定义了一个额外的参数，可用于指定此内存分配的内存空间。 有四种内存空间的可能性：

 1）、USAGE_SCRIPT：在脚本内存空间中分配。 如果不指定内存空间，则这是默认的内存空间。

 2）、USAGE_GRAPHICS_TEXTURE：在GPU的纹理内存空间中分配。

 3）、USAGE_GRAPHICS_VERTEX：在GPU的顶点存储空间中分配。

 4）、USAGE_GRAPHICS_CONSTANTS：分配在各种程序对象使用的GPU的常量存储空间中。




您可以使用按位OR运算符指定多个存储空间。 这样做通知RenderScript运行时您打算访问指定的内存空间中的数据。 以下示例在脚本和顶点存储空间中为自定义数据类型分配内存：




```java
ScriptField_Point touchPoints = new ScriptField_Point(myRenderScript, 2,
        Allocation.USAGE_SCRIPT | Allocation.USAGE_GRAPHICS_VERTEX);
```


2、静态嵌套类“项”允许您以对象的形式创建一个结构体的实例。 如果使用Android代码中的struct更有意义，这个嵌套类是非常有用的。 完成操作对象后，可以通过调用set（Item i，int index，boolean copyNow）并将Item设置为数组中所需的位置来将对象推送到已分配的内存。 RenderScript运行时可以自动访问新写的内存。




3、获取和设置结构中每个字段的值的访问器方法。 这些访问器方法中的每一个都有一个索引参数，用于指定要读取或写入的数组中的结构。 每个setter方法也有一个copyNow参数，它指定是否立即将该内存同步到RenderScript运行时。 要同步任何尚未同步的内存，请调用copyAll（）。




4、createElement（）方法创建内存中的struct的描述。 此描述用于分配由一个或多个元素组成的内存。




5、resize（）的工作方式与C中的realloc（）非常类似，可以扩展先前分配的内存，维护先前创建的当前值。




6、copyAll（）将在框架级设置的内存与RenderScript运行时同步。 当您在成员上调用set访问器方法时，可以指定一个可选的copyNow布尔参数。 调用该方法时，指定true同步内存。 如果指定false，则可以调用copyAll（）一次，并且为所有尚未同步的属性同步内存。




四）、指针

全局指针被反映到脚本类本身，位于project_root / gen / package / name / ScriptC_renderscript_filename中。 您可以声明一个指向一个结构体或任何一个受支持的RenderScript类型的指针，但是一个结构体不能包含指针或嵌套数组。 例如，如果您定义了以下指向struct和int32_t的指针




```java
typedef struct Point {
    float2 position;
    float size;
} Point_t;

Point_t *touchPoints;
int32_t *intPointer;
```


那么将生成以下Java代码：



```java
private ScriptField_Point mExportVar_touchPoints;
public void bind_touchPoints(ScriptField_Point v) {
    mExportVar_touchPoints = v;
    if (v == null) bindAllocation(null, mExportVarIdx_touchPoints);
    else bindAllocation(v.getAllocation(), mExportVarIdx_touchPoints);
}

public ScriptField_Point get_touchPoints() {
    return mExportVar_touchPoints;
}

private Allocation mExportVar_intPointer;
public void bind_intPointer(Allocation v) {
    mExportVar_intPointer = v;
    if (v == null) bindAllocation(null, mExportVarIdx_intPointer);
    else bindAllocation(v, mExportVarIdx_intPointer);
}

public Allocation get_intPointer() {
    return mExportVar_intPointer;
}
```


生成一个get方法和一个名为bind_pointer_name（而不是一个set（）方法）的特殊方法。 bind_pointer_name方法允许您将Android VM中分配的内存绑定到RenderScript运行时（您不能在.rs文件中分配内存）。 有关详细信息，请参阅使用分配的内存。







二、内存分配API

使用RenderScript的应用程序仍然在Android VM中运行。 然而，实际的RenderScript代码本身运行，需要访问Android VM中分配的内存。 要完成此操作，您必须将在VM中分配的内存附加到RenderScript运行时。 此过程称为绑定，允许RenderScript运行时与其请求但不能显式分配的内存无缝协同工作。 最终结果基本上与C中调用malloc相同。另外一个好处是，Android VM可以执行垃圾收集以及与RenderScript运行时层共享内存。 绑定只对动态分配的内存是必需的。
 在编译时自动为您的RenderScript代码创建静态分配的内存。 有关内存分配如何发生的更多信息，请参见图1。





为了支持这种内存分配系统，有一组API允许Android VM分配内存并提供与malloc调用类似的功能。 这些类本质上描述了如何分配内存，并且还执行分配。 为了更好地了解这些类是如何工作的，可以将它们与一个简单的malloc调用进行比较看起来像这样：




```java
array = (int *)malloc(sizeof(int)*10);
```


malloc调用可以分为两部分：正在分配的内存的大小（sizeof（int））以及应该分配多少单位的内存（10）。 Android框架提供了这两个部分的类，以及一个代表malloc本身的类。




Element类表示malloc调用的（sizeof（int））部分，并封装一个单元的内存分配，例如单个浮点值或结构体。 Type类封装了Element和要分配的元素的数量（在我们的示例中为10）。 您可以将类型视为元素数组。 Allocation类根据给定的类型执行实际的内存分配，并表示实际分配的内存。





在大多数情况下，您不需要直接调用这些内存分配API。 反射层类生成自动使用这些API的代码，所有您需要做的分配内存都是调用在其中一个反射层类中声明的构造函数，然后将生成的内存分配绑定到RenderScript。 在某些情况下，您将直接使用这些类来自行分配内存，例如从资源加载位图，或者当您要为内存分配指向原始类型的内存时。 您可以在分配和绑定内存到RenderScript部分中看到如何执行此操作。 下表更详细地介绍了三个内存管理类：



|Android对象类型|描述|
|----|----|
|`[Element](https://developer.android.google.cn/reference/android/renderscript/Element.html)`|元素描述一个单元的内存分配，可以有两种形式：基本的或复杂的。一个基本元素包含任何有效的RenderScript数据类型的单个组件数据。 基本元素数据类型的示例包括单个浮点值，float4向量或单个RGB-565颜色。复杂元素包含一个基本元素列表，并从您在RenderScript代码中声明的结构体创建。 例如，分配可以包含在内存中按顺序排列的多个结构体。 每个结构体被认为是自己的元素，而不是该结构体内的每个数据类型。|
|`[Type](https://developer.android.google.cn/reference/android/renderscript/Type.html)`|类型是内存分配模板，由一个元素和一个或多个维度组成。 它描述了内存的布局（基本上是一个Elements的数组），但是并不为它所描述的数据分配内存。类型由五个维度组成：X，Y，Z，LOD（细节级别）和Faces（立方体贴图）。 您可以在可用内存限制内将X，Y，Z尺寸设置为任何正整数值。 单维尺寸分配的X维数大于零，而Y和Z的尺寸为零，表示不存在。 例如，x = 10，y = 1的分配被认为是二维的，x = 10，y = 0被认为是一维的。 LOD和Faces维度是指示存在或不存在的布尔值。|
|`[Allocation](https://developer.android.google.cn/reference/android/renderscript/Allocation.html)`|分配根据由类型表示的内存的描述为应用程序提供内存。 分配的内存可以同时存在于许多内存空间中。 如果在一个空间中修改了内存，则必须显式同步内存，以便在其存在的所有其他空格中进行更新。分配数据以两种主要方式之一进行上传：检查类型，并取消选中类型。 对于简单的数组，有copyFrom（）函数从Android系统获取一个数组并将其复制到本机层内存存储。 未经检查的变体允许Android系统复制结构数组，因为它不支持结构。 例如，如果存在n个浮点数组的分配，则可以复制float [n]数组或byte [n * 4]数组中包含的数据。|


三、使用内存

您在RenderScript中声明的非静态全局变量在编译时分配内存。 您可以直接在RenderScript代码中处理这些变量，而无需在Android框架级别为其分配内存。 Android框架层还可以使用在反射层类中生成的提供的访问器方法来访问这些变量。 如果这些变量在RenderScript运行时层初始化，那么这些值将用于初始化Android框架层中的相应值。 如果全局变量被标记为const，则不会生成set方法。





注意：如果您使用某些包含指针的RenderScript结构（如rs_program_fragment和rs_allocation），则必须首先获取相应的Android框架类的对象，然后调用该结构的set方法将内存绑定到RenderScript运行时。 您不能在RenderScript运行时层直接操作这些结构。 此限制不适用于包含指针的用户定义的结构，因为它们首先不能导出到反射层类。 如果您尝试声明包含指针的非静态全局结构体，则会生成编译器错误。





RenderScript还支持指针，但您必须在Android框架代码中明确分配内存。 当您在.rs文件中声明全局指针时，可以通过适当的反射层类分配内存，并将该内存绑定到本机RenderScript层。 您可以从Android框架层以及RenderScript层与内存进行交互，这样可以在最合适的层中修改变量。





一）、将动态内存分配并绑定到RenderScript

要分配动态内存，您需要调用Script.FieldBase类的构造函数，这是最常用的方法。 另一种方法是手动创建一个分配，这对于诸如原始类型的指针是必需的。 为了简单起见，您应该使用Script.FieldBase类构造函数。 获取内存分配后，调用指针的反射绑定方法将分配的内存绑定到RenderScript运行时。





下面的示例为原始类型的指针，intPointer和指向struct的指针touchPoints分配内存。 它还将内存绑定到RenderScript：




```java
private RenderScript myRenderScript;
private ScriptC_example script;
private Resources resources;

public void init(RenderScript rs, Resources res) {
    myRenderScript = rs;
    resources = res;

    //allocate memory for the struct pointer, calling the constructor
    ScriptField_Point touchPoints = new ScriptField_Point(myRenderScript, 2);

    //Create an element manually and allocate memory for the int pointer
    intPointer = Allocation.createSized(myRenderScript, Element.I32(myRenderScript), 2);

    //create an instance of the RenderScript, pointing it to the bytecode resource
    mScript = new ScriptC_example(myRenderScript, resources, R.raw.example);

    //bind the struct and int pointers to the RenderScript
    mScript.bind_touchPoints(touchPoints);
    script.bind_intPointer(intPointer);

   ...
}
```


二）、读取和写入到内存

您可以在RenderScript运行时和Android框架层读写静态和动态分配的内存。





静态分配的内存在RenderScript运行时级别具有单向通信限制。 当RenderScript代码更改变量的值时，为了效率，它不会传回到Android框架层。 在调用get方法时，始终返回从Android框架设置的最后一个值。 但是，当Android框架代码修改变量时，该更改可以自动传递到RenderScript运行或稍后进行同步。 如果需要将数据从RenderScript运行时发送到Android框架层，则可以使用rsSendToClient（）函数来克服此限制。





当使用动态分配的内存时，如果使用其关联的指针修改了内存分配，则RenderScript运行时层上的任何更改都将传播回到Android框架层。 在Android框架层修改对象会立即将该更改传播回RenderScript运行时层。





1、读写全局变量

读取和写入全局变量是一个简单的过程。 您可以使用Android框架级别的访问者方法或直接在RenderScript代码中进行设置。 请记住，您在RenderScript代码中所做的任何更改都不会传播回Android框架层。





例如，给定在名为rsfile.rs的文件中声明的以下结构体：




```java
typedef struct Point {
    int x;
    int y;
} Point_t;

Point_t point;
```


你可以直接在rsfile.rs中为这个结构赋值。 这些值不会传播回Android框架级别：



```java
point.x = 1;
point.y = 1;
```


您可以将这些值分配给Android框架层的结构。 这些值将传播回RenderScript运行时级别：



```java
ScriptC_rsfile mScript;

...

Item i = new ScriptField_Point.Item();
i.x = 1;
i.y = 1;
mScript.set_point(i);
```


您可以阅读RenderScript代码中的值，如下所示：



```java
rsDebug("Printing out a Point", point.x, point.y);
```


您可以使用以下代码读取Android框架图层中的值。 请记住，如果在Android框架级别设置了此代码，则此代码将返回一个值。 如果仅在RenderScript运行时级别设置值，则会得到空指针异常：



```java
Log.i("TAGNAME", "Printing out a Point: " + mScript.get_point().x + " " + mScript.get_point().y);
System.out.println(point.get_x() + " " + point.get_y());
```


2、读写全局指针

假设内存已经在Android框架级别分配并绑定到RenderScript运行时，您可以通过使用该指针的get和set方法从Android框架级别读写内存。 在RenderScript运行时层中，您可以读取和写入指针的内存，并将更改传播回Android框架层，而不像静态分配的内存。





例如，给定以下指向名为rsfile.rs的文件中的结构体的指针：




```java
typedef struct Point {
    int x;
    int y;
} Point_t;

Point_t *point;
```


假设您已经在Android框架层分配了内存，可以正常访问结构中的值。 您通过其指针变量对结构体进行的任何更改都将自动提供给Android框架层：



```java
point[index].x = 1;
point[index].y = 1;
```


您还可以在Android框架层读取和写入指针的值：



```java
ScriptField_Point p = new ScriptField_Point(mRS, 1);
    Item i = new ScriptField_Point.Item();
    i.x=100;
    i.y = 100;
    p.set(i, 0, true);
    mScript.bind_point(p);

    points.get_x(0);            //read x and y from index 0
    points.get_x(0);
```


一旦内存已被绑定，您不必每次更改值时将内存重新绑定到RenderScript运行时。



