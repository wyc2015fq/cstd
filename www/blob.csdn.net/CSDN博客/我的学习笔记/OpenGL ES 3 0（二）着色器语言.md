# OpenGL ES 3.0（二）着色器语言 - 我的学习笔记 - CSDN博客





2018年08月11日 21:41:14[zouzhiheng](https://me.csdn.net/u011330638)阅读数：121








OpenGL ES 着色器语言：OpenGL ES Shading Language，下面简写为 ES SL 或 SL。

## 着色器版本指定
`#version 300 es`
如果没有指定，则默认为 1.00，这是 OpenGL ES 2.0 使用的版本，在 OpenGL ES 3.0 中，制定规范的作者决定匹配 OpenGL ES 和 ES SL 的版本，因此直接从 1.0 跳到了 3.0。

## 变量类型
|标量|float, int, uint, bool|基于标量的数据类型|
|----|----|----|
|浮点向量|float, vec2, vec3, vec4|分别表示有 1、2、3、4 个成员的浮点向量|
|整型向量|int, ivec2, ivec3, ivec4|分别表示有 1、2、3、4 个成员的整型向量|
|无符号整型向量|uint, uvec2, uvec3, uvec4|分别表示有 1、2、3、4 个成员的无符号整型向量|
|布尔向量|bool, bvec2, bvec3, bvec4|分别表示有 1、2、3、4 个成员的布尔向量|
|矩阵|mat2 (or mat2x2), mat2x3, mat2x4, mat3x2, mat3 (or mat3x3), mat3x4, mat4x2, mat4x3, mat4 (or mat4x4)|分别表示 2x2、2x3、…、4x4 矩阵|

## 构造函数

在 OpenGL ES SL 中，对于类型转换有很严格的限制，即变量只允许使用相同类型的其它变量进行赋值或其它操作。但 SL 中提供了一些构造函数可以用于类型转换：

```
float myFloat = 1.0;
float myFloat2 = 1; // 错误，不正确的类型转换
bool myBool = true;
int myInt = 0;
int myInt2 = 0.0; // 错误，不正切的类型转换
myFloat = float(myBool); // 正确，bool -> float
myFloat = float(myInt); // 正确，int -> float
myBool = bool(myInt); // 正确，int -> bool
```

向量类型的构造函数：

```
vec4 myVec4 = vec4(1.0);              // myVec4 = {1.0, 1.0, 1.0, 1.0}
vec3 myVec3 = vec3(1.0, 0.0, 0.5);    // myVec3 = {1.0, 0.0, 0.5}
vec3 temp = vec3(myVec3);             // temp = myVec3
vec2 myVec2 = vec2(myVec3);           // myVec2 = {myVec3.x, myVec3.y}
myVec4 = vec4(myVec2, temp);          // myVec4 = {myVec2.x, myVec2.y, temp.x, temp.y}
```

矩阵类型的构造函数则更加灵活，规则如下： 

    1) 如果只提供了一个标量（基本数据类型），则会用这个值填充矩阵的对角线，比如 mat4(1.0) 将会构造一个 4x4 的单位矩阵 

    2) 可以使用多个向量构造，比如 mat2 可以使用两个 vec2 构造 

    3) 可以使用多个标量构造
需要注意的是，和平时的写法不同，OpenGL 中的矩阵以列优先的形式排序：

```
mat3 myMat3 = mat3(1.0, 0.0, 0.0,  // 第一列
                   0.0, 1.0, 0.0,  // 第二列
                   0.0, 1.0, 1.0); // 第三列
```

## 向量和矩阵的成员

根据组成 vector 的成员数量，可以通过 {x, y, z, w}, {r, g, b, a}, 或 {s, t, p, q} 来访问它的成员，x、r、s 永远指向第一个成员，使用不同的名字仅仅只是为了方便，但不能混合使用，即不能用 “.xgr” 这样的形式。使用示例：

```
vec3 myVec3 = vec3(0.0, 1.0, 2.0); // myVec3 = {0.0, 1.0, 2.0}
vec3 temp;
temp = myVec3.xyz; // temp = {0.0, 1.0, 2.0}
temp = myVec3.xxx; // temp = {0.0, 0.0, 0.0}
temp = myVec3.zyx; // temp = {2.0, 1.0, 0.0}
```

也可以使用 “[]” 操作符，记住，矩阵是以列排序的：

```
mat4 myMat4 = mat4(1.0);     // 初始化对角线的值为 1.0 (单位矩阵)
vec4 col0 = myMat4[0];          // 从矩阵中获取第 0 列向量
float m1_1 = myMat4[1][1];    // 从矩阵中获取元素 [1][1]
float m2_2 = myMat4[2].z;      // 从矩阵中获取元素 [2][2]
```

## 常量

常量必须在声明的时候初始化：

```
const float zero = 0.0;
const float pi = 3.14159;
const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
const mat4 identity = mat4(1.0);
```

## Structures

结构体定义：

```
struct fogStruct
{
    vec4 color;
    float start;
    float end;
} fogVar;
```

结构体被定义的同时，它的构造函数也被定义：

```
fogVar = fogStruct(vec4(0.0, 1.0, 0.0, 0.0), // color
                               0.5,     // start
                               2.0);    // end
```

注意形参和实参必须一一对应。

访问结构体成员：

```
vec4 color = fogVar.color;
float start = fogVar.start;
float end = fogVar.end;
```

## 数组

数组声明及初始化：

```
float floatArray[4];
vec4 vecArray[2];

float a[4] = float[](1.0, 2.0, 3.0, 4.0);
float b[4] = float[4](1.0, 2.0, 3.0, 4.0);
vec2 c[2] = vec2[2](vec2(1.0), vec2(1.0));
```

## 函数

OpenGL ES SL 中定义函数的方法和 C 差不多，最重要的一点区别是，OpenGL ES SL 中提供了几个特殊的标识符，表明某一个参数是否会在函数内部被更改：
|Qualifier|Description|
|----|----|
|in|如果没有指定，则默认为 in 表明变量以传值的形式进入到函数中，不会被更改|
|inout|表明变量以引用的形式传递到函数中，可能会被更改|
|out|表明变量的值不会被传递到函数中，但会在函数返回值被更改|

例如：

```
vec4 myFunc(inout float myFloat,  // inout 参数
            out vec4 myVec4,     // out 参数
            mat4 myMat4);         // in 参数（默认）
```

值得注意的是，在 OpenGL ES SL 中，函数不能递归，这是因为有一些 GPU 调用函数的方式为 inline。

## 内置函数

OpenGL ES 中定义了许多内置函数可供开发者直接使用，比如下面这个计算漫射反光的函数：

```
vec4 diffuse(vec3 normal, vec3 light, vec4 baseColor) {
    return baseColor * dot(normal, light);
}
```

这个函数调用了一个内置函数 dot，dot 用于计算点积，即矩阵各个对应元素相乘，要求两个矩阵具有相同的大小。

下面是镜面反射（specular lighting ）的计算过程：

```
float nDotL = dot(normal, light);
float rDotV = dot(viewDir, (2.0 * normal) * nDotL - light);
float specular = specularColor * pow(rDotV, specularPower);
```

## 流程控制语句

使用方法和 C 一样，但在 OpenGL ES 2.0 中，对循环的使用有着非常严格的规则控制，只支持编译器可以展开的循环；而在 OpenGL 3.0 中，这些限制不再存在。但循环的使用依然会对性能有一些影响，因此，应该尽量限制流程控制语句的使用。

## Uniforms

和 attribute(2.0) 或 in(3.0) 数据一样，Uniforms 也是由外部程序输入到着色器代码中的，所不同的是，它被同一 program 中的顶点、片段着色器共享，且着色器不能更改。Uniform 变量存储在硬件上的“常量存储区”中，因为这个区域的尺寸是固定的，因此同一个 program 中的 uniform 数量是有限制的。可以使用 gl_MaxVertexUniformVectors 和 gl_MaxFragmentUniformVectors 确认可使用的最大 uniform 个数，或使用函数 glGetintegerv(参数为 GL_MAX_VERTEX_UNIFORM_VECTORS 或 GL_MAX_FRAGMENT_UNIFORM_VECTORS) 获取。

OpenGL ES 3.0 保证最少有 256 个 vertex uniform vector，224 个 fragment uniform vector 可用。

## Uniform Blocks

使用 uniform buffer object 通常有几个优点： 

    1) uniform 数据可以在多个 program 之间共享（注意仅能设置一次） 

    2) 允许存储数量更多的 uniform  

    3) 在 uniform buffer object 之间切换可能比一次单独地加载一个 uniform 效率更高
uniform buffer object 通过 uniform block 的形式使用：

```
#version 300 es
uniform TransformBlock
{
    mat4 matViewProj;
    mat3 matNormal;
    mat3 matTexGen;
};

layout(location = 0) in vec4 a_position;
void main()
{
    gl_Position = matViewProj * a_position;
}
```

## in、out

OpenGL ES SL 中另一个特殊类型是 vertex input (or attribute) 变量， 用 in 修饰，用于指定顶点着色器中每一个顶点的输入，通常用于存储位置、纹理坐标、颜色等信息。代码示例：

```
#version 300 es
layout(location=0) in vec4 aColor;
layout(location=1) in vec4 aPosition;
out vec4 vColor;
void main() {
    vColor = aColor;
    gl_Position = aPosition;
}
```

layout 限定符相当于 index，用于分辨顶点属性，如果没有指定，链接器会自动为其赋值。

和 uniform 变量一样，硬件对 attribute 变量的数量有限制，可以通过 gl_MaxVertexAttribs 或 glGetIntegerv(GL_MAX_VERTEX_ATTRIBS) 查询，OpenGL ES 3.0 保证最少能支持 16 个，因此如果希望编写在任何 OpenGL ES 3.0 的实现上都能运行的着色器，应该限制不超过 16 个 attribute。

顶点着色器的输出变量用 out 修饰，对应于片元着色器中使用 in 修饰对应的变量。这些数据会被输出到片元着色器中，并且在光栅化过程中，会对图元进行线性插值。注意不能使用 layout 限定符修饰，程序实现会自动选择它们的 location 值。同样的，顶点着色器的输出变量在硬件中也有数量限制，可以通过 gl_MaxVertexOutputVectors 或  glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS，返回总的成员数量，而不是 vector 的数量) 查询，OpenGL ES 3.0 最少能支持 16 个。而片元着色器 in 变量的数量限制可以通过 gl_MaxFragmentInputVectors 或 glGetIntegerv (GL_MAX_FRAGMENT_INPUT_COMPONENTS，返回总的成员数量，而不是 vector 的数量) 获取，OpenGL ES 3.0 最少能支持 15 个。

通常，片元着色器仅需要渲染到一个颜色缓冲区中，但如果存在多个渲染目标，则需要使用 layout 限定符指定，比如：

```
#version 300 es
precision mediump float;
layout(location = 0) out vec4 fragData0;
layout(location = 1) out vec4 fragData1;
layout(location = 2) out vec4 fragData2;
layout(location = 3) out vec4 fragData3;
void main()
{
    fragData0 = vec4 ( 1, 0, 0, 1 );
    fragData1 = vec4 ( 0, 1, 0, 1 );
    fragData2 = vec4 ( 0, 0, 1, 1 );
    fragData3 = vec4 ( 1, 1, 0, 1 );
}
```

## 插值限定符

顶点着色器、片元着色器的输入变量可以指定插值限定符，如果没有指定，则默认使用 smooth 差值方法渲染，也就是说，图元会在顶点着色器的输出变量的值的基础上进行线性插值，片元着色器接收到的是经过线性插值后的结果。可以在着色器代码中显式地指定插值方式：

```
// 顶点着色器
smooth out vec3 v_color;

// 片段着色器
smooth in vec3 v_color;
```

另一个插值方式为 flat shading，这种方式不会在图元上插值，而是使用一个激发顶点（provoking vertex），该顶点的值会被应用于图元中的所有片段：

```
// 顶点着色器
flat out vec3 v_color;

// 片段着色器
flat in vec3 v_color;
```

最后一个可以加入到插值器中的是 centroid 关键字，可以强制插值作用于图元的内部，否则，在图元边缘可能会出现伪像。使用：

```
// 顶点着色器
smooth centroid out vec3 v_color;

// 片段着色器
smooth centroid in vec3 v_color;
```

## 预处理器

和 C 类似，有：

```
#define
#undef
#if
#ifdef
#ifndef
#else
#elif
#endif
```

以下是一些内置的宏：

```
__LINE__            // 替换为着色中的当前行号
__VERSION__    // OpenGL ES 着色器语言版本 (例如 300)
```

另外有： 

```
#error // 将在着色器编译期间导致编译错误，并在信息日志中放置相应的消息
#pragma // 用于改为编译器指定 implementation-specificc 指令
```

还有一个重要的指令是 #extension，用于启用和设置扩展的行为：

```
// 设置某个扩展的行为
#extension extension_name : behavior
// 设置所有扩展的行为
#extension all : behavior
```

behavior 的可选值有：require、enable、warn、disable。

## 精度限定符

精度限定符可以指定着色器变量进行计算时的精度，可选值有：lowp、mediump、highp。低精度在一些 OpenGL ES 的实现中可能会带有速度及效率的提升，但精度的降低可能造成伪像（artifacts）现象。但 OpenGL ES 规范中没有规定必须支持多个精度，因此所有计算以最高精度执行并忽略精度限定符是有效的。

精度限定符可用于修饰 flocating-point 和 integer-based 变量，示例：

```
highp vec4 position;
varying lowp vec4 color;
mediump float specularExp;
```

可以在着色器顶部指定默认精度：

```
precision highp float;
precision mediump int;
```

如果没有指定，则 int 和 flocat 都使用 highp 作为默认精度。但在片元着色器中，必须指定一个默认的 flocat 精度。





