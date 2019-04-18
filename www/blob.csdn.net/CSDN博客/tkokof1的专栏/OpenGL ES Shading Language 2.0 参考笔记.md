# OpenGL ES Shading Language 2.0 参考笔记 - tkokof1的专栏 - CSDN博客

2018年01月15日 16:53:54[tkokof1](https://me.csdn.net/tkokof1)阅读数：240


OpenGL ES Shading Language 2.0 参考笔记

*这里只是一份简略的笔记,仅为方便参考而已,更详细的信息可以参考[这里](https://www.khronos.org/registry/OpenGL/index_gl.php)*

## 类型(type)

### 标量类型(scalar type)
|类型名|说明|示例|注意点|
|----|----|----|----|
|void|无类型|void function(void);|无|
|bool|布尔类型|bool bValue = true;|不支持隐式类型转换,但是可以借助构造方式来进行类型转换,就像这样: bool bValue = bool(iValue);|
|int|整数类型|int iValue = 1;|不支持隐式类型转换,但是可以借助构造方式来进行类型转换,就像这样: int iValue = int(bValue);|
|float|浮点类型|float fValue = 1.0;|不支持隐式类型转换,但是可以借助构造方式来进行类型转换,就像这样: float fValue = float(iValue);|

### 向量类型(vector type)
|类型名|说明|示例|注意点|
|----|----|----|----|
|bvec2|2维(2分量)布尔向量|bvec2 bVec2 = bvec2(true, true);|1. 可以使用标量类型对向量的各个分量分别进行初始化2. 如果初始化参数是单个标量,则向量的各个分量都初始化为该标量3.如果使用高维向量初始化低维向量,则低维向量的各个分量分别使用高维向量中对应的分量进行初始化 例如 **bvec2 bVec2 = bvec2(bVec3);**等价于 **bvec2 bVec2 = bvec2(bVec3.x, bVec3.y);**4. 使用低维向量初始化高维向量是不允许的,但是可以使用低维向量与(或)标量类型组合的方式来初始化高维向量,但是组合对应的参数数量需要至少为高维向量的分量个数. 例如 **vec3 bVec3 = bvec3(bVec2, bValue);vec3 bVec3 = bvec3(bVec2.x, bVec2.y, bValue);**5. 可以使用向量的构造方式来进行不同向量类型间的转化,其中向量的各个分量会自动进行相应的类型转换 例如 **bvec2 bVec2 = bvec2(iVec2);**|
|bvec3|3维(3分量)布尔向量|bvec3 bVec3 = bvec3(true, true, true);|同上|
|bvec4|4维(4分量)布尔向量|bvec4 bVec4 = bvec4(true, true, true, true);|同上|
|ivec2|2维(2分量)整数向量|ivec2 iVec2 = ivec2(1, 1);|同上|
|ivec3|3维(3分量)整数向量|ivec3 iVec3 = ivec3(1, 1, 1);|同上|
|ivec4|4维(4分量)整数向量|ivec4 iVec4 = ivec4(1, 1, 1, 1);|同上|
|vec2|2维(2分量)浮点向量|vec2 fVec2 = vec2(1.0, 1.0);|同上|
|vec3|3维(3分量)浮点向量|vec3 fVec3 = vec3(1.0, 1.0, 1.0);|同上|
|vec4|4维(4分量)浮点向量|vec4 fVec4 = vec4(1.0, 1.0, 1.0, 1.0);|同上|

### 矩阵类型(matrix type)
|类型名|说明|示例|注意点|
|----|----|----|----|
|mat2|2x2浮点矩阵|mat2 aMat2 = mat2(1.0, 0.0,  // first column0.0, 1.0); // second columnmat2 bMat2 = mat2(1.0);mat2 cMat2 = mat2(aVec2, bVec2);mat2 dMat2 = mat2(aVec3, aFloat);|1. 可以使用标量类型初始化矩阵的各个元素,矩阵按列顺序分别进行对应元素的初始化2. 如果初始化参数是单个标量,则矩阵使用该标量初始化主对角线元素(**注意,不是所有元素**)3. 可以使用向量与(或)标量类型组合的方式来初始化矩阵,但是组合对应的参数数量需要至少为矩阵的元素个数4. 使用[col_index]可以访问矩阵的列向量,使用[col_index][row_index]可以访问矩阵的元素|
|mat3|3x3浮点矩阵|mat3 aMat3 = mat3(1.0, 0.0, 0.0,  // 1. column0.0, 1.0, 0.0,  // 2. column0.0, 0.0, 1.0); // 3. columnmat3 bMat3 = mat3(1.0);mat3 cMat3 = mat3(aVec3, bVec3, cVec3);mat3 dMat3 = mat3(aVec4, aVec3, bVec4, aFloat);|同上|
|mat4|4x4浮点矩阵|mat4 aMat4 = mat4(1.0, 0.0, 0.0, 0.0,  // 1. column0.0, 1.0, 0.0, 0.0,  // 2. column0.0, 0.0, 1.0, 0.0,  // 3. column0.0, 0.0, 0.0, 1.0); // 4. columnmat4 bMat4 = mat4(1.0);mat4 cMat4 = mat4(aVec4, bVec4, cVec4, dVec4);mat4 dMat4 = mat4(aVec4, aVec3, bVec4, cVec4, aFloat);|同上|

### 贴图类型(texture type)
|类型名|说明|示例|注意点|
|----|----|----|----|
|sampler2D|2维贴图|uniform sampler2D texture;|需要声明为 uniform([限定符参考](#qualifier_section))|
|samplerCube|立方体贴图|uniform samplerCube texture;|需要声明为 uniform([限定符参考](#qualifier_section))|

### 结构(struct)
- 结构可以组合使用上面所提到的标准类型
- 结构可以包含数组或者结构
- 可以使用构造方式初始化结构,但初始化参数类型必须与结构定义类型一一对应
- 结构中的元素可以使用点号(.)进行访问

**声明**

struct matStruct { 

    vec4 ambientColor;  

    vec4 diffuseColor; 

    vec4 specularColor; 

    float specularExponent; 

};

**构造**

newMaterial = matStruct(vec4(0.1, 0.1, 0.1, 1.0), 

                        vec4(1.0, 0.0, 0.0, 1.0), 

                        vec4(0.7, 0.7, 0.7, 1.0), 

                        50.0);

**访问**

vec4 ambientColor = newMaterial.ambientColor;  

vec4 diffuseColor = newMaterial.diffuseColor; 

vec4 specularColor = newMaterial.specularColor; 

float specularExponent = newMaterial.specularExponent;

### 数组(array)
- 数组可以包含标准类型及结构类型
- 数组声明时不能就地初始化
- 数组需要以逐个元素的方式进行初始化

int newIntArray[9]; 

vec3 newVec3Array[3];

newIntArray[0] = 5; 

newVec3Array[1] = vec3(1.0, 1.0, 1.0);

int newInt = newIntArray[0]; 

vec3 newVec3 = newVec3Array[1];

## 限定符(qualifier)

### 存储限定符(storage qualifier)
|限定符|说明|示例|注意点|
|----|----|----|----|
|const|常量限定符|const float PI = 3.1415926;|1. 编译时常量2. 只读函数参数|
|attribute|-|attribute vec4 a_position;|1. 一般用于限定 OpenGL ES 环境向 vertex shader 传送顶点的各个属性,譬如位置,朝向,顶点贴图坐标等等2. 限定的数据不可更改3. 限定的数据 vertex shader 可见, fragment shader 不可见4. 只可以限定浮点类型的数据(浮点标量,浮点向量,(浮点)矩阵),**不能限定浮点类型数组**5. 需要在运行shader前加载好相应的限定数据|
|uniform|-|uniform vec2 u_resolution;|1. 一般用于限定一次渲染流程中不会改变的属性,譬如投影矩阵,光源位置,材质颜色等等2. 限定的数据不可更改3. 限定的数据 vertex shader 和 fragment shader 都可见,需要保证定义唯一4. 需要在运行shader前加载好相应的限定数据|
|varying|-|varying vec2 v_texCoord;|1. 一般用于限定在 vertex shader 中进行计算并需要向 fragment shader 传递的数据2. 限定的数据会首先由 vertex shader 进行初始化计算并经过光栅化阶段的插值再传递给 fragment shader3. 限定的数据 vertex shader 和 fragment shader 都可见,需要保证定义唯一4. 只可以限定浮点类型的数据(浮点标量,浮点向量,(浮点)矩阵以及包含这些类型的数组)|

### 精度限定符(precision qualifier)
|限定符|说明|示例|注意点|
|----|----|----|----|
|highp|高精度限定符|uniform highp vec3 lightDirection;|1. 用于限定某个变量使用定义类型的最高精度2. 只可用于限定整型标量,整型向量,浮点标量,浮点向量及(浮点)矩阵3. 在 vertex shader 中精度限定符是可选的,默认使用 highp,即最高精度4. 在 fragment shader 中精度限定符是必选的,除非你定义了相应类型的默认精度(使用[precision限定符](#precision_section))5. 精度的具体标识范围由具体的 OpenGL ES 实现决定,一般来讲,使用低精度可以提高渲染效率但是也会降低渲染质量|
|mediump|中精度限定符|varying mediump vec2 textureCoordinate;|1. 用于限定某个变量使用定义类型的中等精度2. 只可用于限定整型标量,整型向量,浮点标量,浮点向量及(浮点)矩阵3. 在 vertex shader 中精度限定符是可选的,默认使用 highp,即最高精度4. 在 fragment shader 中精度限定符是必选的,除非你定义了相应类型的默认精度(使用[precision限定符](#precision_section))|
|lowp|低精度限定符|varying lowp vec4 colorVarying;|1. 用于限定某个变量使用定义类型的低等精度2. 只可用于限定整型标量,整型向量,浮点标量,浮点向量及(浮点)矩阵3. 在 vertex shader 中精度限定符是可选的,默认使用 highp,即最高精度4. 在 fragment shader 中精度限定符是必选的,除非你定义了相应类型的默认精度(使用[precision限定符](#precision_section))|
|precision|默认精度限定符|precision highp float;|1. 用于限定某种类型的默认精度2. 只可用于限定整型标量,整型向量,浮点标量,浮点向量及(浮点)矩阵3. 在 vertex shader 中默认精度是 highp,即最高精度4. 在 fragment shader 中精度限定符是必选的,除非你定义了相应类型的默认精度|

### 参数限定符(parameter qualifier)
|限定符|说明|示例|注意点|
|----|----|----|----|
|in|只读参数限定符|int newFunction(in bvec4 aBvec4,   // read-only out vec3 aVec3,    // write-onlyinout int aInt);   // read-write|函数参数的默认限定符即为 in ,即函数参数默认都是只读的|
|out|只写参数限定符|int newFunction(in bvec4 aBvec4,   // read-only out vec3 aVec3,    // write-onlyinout int aInt);   // read-write|1. 函数参数会以引用方式传递2. 函数参数只可写不可读|
|inout|读写参数限定符|int newFunction(in bvec4 aBvec4,   // read-only out vec3 aVec3,    // write-onlyinout int aInt);   // read-write|1. 函数参数会以引用方式传递2. 函数参数可读可写|

## 内建的变量与常量(built-in variables and constants)

### 顶点着色器特殊输出变量(special output variables of the vertex shader)
|变量名|说明|示例|注意点|
|----|----|----|----|
|gl_Position|顶点位置|highp vec4 gl_Position;|1. vertex shader 中必须为 gl_Position 赋值2. gl_Position 需要变换到裁剪空间中|
|gl_PointSize|点大小|mediump float gl_PointSize;|1. 数值大小为点半径的像素大小2. 点实际使用quad渲染3. 渲染图元为点(GL_POINTS)时才会生效|

### 片段着色器特殊输入变量(special input variables of the fragment shader)
|变量名|说明|示例|注意点|
|----|----|----|----|
|gl_FragCoord|片段坐标|mediump vec4 gl_FragCoord;|1. 只读变量2. 定义于窗口坐标空间中,默认左下角为窗口坐标原点|
|gl_FrontFacing|片段朝向|bool gl_FrontFacing;|1. 只读变量2. 标识片段是否属于正朝向图元(三角形图元)|
|gl_PointCoord|点坐标|mediump vec2 gl_PointCoord;|1. 只读变量2. 渲染图元为点(GL_POINTS)时才可访问3. 片元位于点图元的归一化位置,默认点图元左上角为归一化空间原点(gl_PointCoord 分量范围为(0, 1))|

### 片段着色器特殊输出变量(special output variables of the fragment shader)
|变量名|说明|示例|注意点|
|----|----|----|----|
|gl_FragColor|片段颜色|mediump vec4 gl_FragColor;|1. fragment shader 中必须为 gl_FragColor 赋值2. gl_FragColor 定义于 RGBA 颜色空间中|

### 顶点着色器内建常量(built-in constants of vertex shader)
|常量名|说明|示例|注意点|
|----|----|----|----|
|gl_MaxVertexAttribs|-|const mediump int gl_MaxVertexAttribs;|1. vertex shader 中 attribute 限定数据的最大个数(单位最大为vec4大小)2. 实际大小由 OpenGL ES 实现决定, 至少为 8|
|gl_MaxVertexUniformVectors|-|const mediump int gl_MaxVertexUniformVectors;|1. vertex shader 中 uniform 限定数据的最大个数(单位最大为vec4大小)2. 实际大小由 OpenGL ES 实现决定, 至少为 128|
|gl_MaxVaryingVectors|-|const mediump int gl_MaxVaryingVectors;|1. vertex shader 和 fragment shader 中 varying 限定数据的最大个数(单位最大为vec4大小)2. 实际大小由 OpenGL ES 实现决定, 至少为 8|
|gl_MaxVertexTextureImageUnits|-|const mediump int gl_MaxVertexTextureImageUnits;|1. vertex shader 中可以使用的最大贴图数量2. 实际大小由 OpenGL ES 实现决定, 至少为 0, 即在 vertex shader 中无法访问纹理|
|gl_MaxCombinedTextureImageUnits|-|const mediump int gl_MaxCombinedTextureImageUnits;|1. vertex shader 或者 fragment shader 中可以使用的最大贴图数量2. 实际大小由 OpenGL ES 实现决定, 至少为 8|

### 片段着色器内建常量(built-in constants of fragment shader)
|常量名|说明|示例|注意点|
|----|----|----|----|
|gl_MaxTextureImageUnits|-|const mediump int gl_MaxTextureImageUnits;|1. fragment shader 中可以使用的最大贴图数量2. 实际大小由 OpenGL ES 实现决定, 至少为 8|
|gl_MaxFragmentUniformVectors|-|const mediump int gl_MaxFragmentUniformVectors;|1. fragment shader 中 uniform 限定数据的最大个数(单位最大为vec4大小)2. 实际大小由 OpenGL ES 实现决定, 至少为 16|
|gl_MaxDrawBuffers|-|const mediump int gl_MaxDrawBuffers;|1. 最大的可用渲染缓存数量2. 当前大小固定为 1|

## 语法声明(statements)

基本与 C 无异,简单列几点:

### 循环(loop)

for(;;) {} 示例 for(int i = 0; i <= 99; i++) { aFunction(); }

while() {} 示例 while(i <= 99) { aFunction(); }

do {} while(); 示例 do { aFunction(); } while(i <= 99);

循环中可以使用 continue(终止单遍循环) 和 break(终止整个循环)

### 条件(condition)

if() {} 示例 if(i != 0) { aFunction(); }

if() {} else {} 示例 if(i != 0) { aFunction(); } else { bFunction(); }

### 退出(exit)

可以使用 return; 退出无返回的函数

可以使用 return aValue; 退出对应返回值类型的函数

在 fragment shader 中可以使用 discard; 中断 fragment shader 的运行,对应的片元也不会被渲染

### 起始(start)

main 函数为 vertex shader 和 fragment shader 的入口函数,没有参数,也没有返回类型

void main(void) {}

## 内建函数(built-in functions)

### 角度与三角函数(angle and trigonometry functions)
|函数名|说明|示例|注意点|
|----|----|----|----|
|radians|角度转弧度|float radians(float degrees)  vec2 radians(vec2 degrees)  vec3 radians(vec3 degrees)  vec4 radians(vec4 degrees)|参数类型可以为浮点标量或者浮点向量|
|degrees|弧度转角度|float degrees(float radians)  vec2 degrees(vec2 radians)  vec3 degrees(vec3 radians)  vec4 degrees(vec4 radians)|参数类型可以为浮点标量或者浮点向量|
|sin|正弦函数|float sin(float angle)  vec2 sin(vec2 angle)  vec3 sin(vec3 angle)  vec4 sin(vec4 angle)|1. 参数为弧度 2. 参数类型可以为浮点标量或者浮点向量|
|cos|余弦函数|float cos(float angle)  vec2 cos(vec2 angle)  vec3 cos(vec3 angle)  vec4 cos(vec4 angle)|1. 参数为弧度 2. 参数类型可以为浮点标量或者浮点向量|
|tan|正切函数|float tan(float angle)  vec2 tan(vec2 angle)  vec3 tan(vec3 angle)  vec4 tan(vec4 angle)|1. 参数为弧度 2. 参数类型可以为浮点标量或者浮点向量|
|asin|反正弦函数|float asin(float x)  vec2 asin(vec2 x)  vec3 asin(vec3 x)  vec4 asin(vec4 x)|1. 返回值为弧度 2. 参数类型可以为浮点标量或者浮点向量|
|acos|反余弦函数|float acos(float x)  vec2 acos(vec2 x)  vec3 acos(vec3 x)  vec4 acos(vec4 x)|1. 返回值为弧度 2. 参数类型可以为浮点标量或者浮点向量|
|atan|反正切函数|float atan(float y_over_x)  vec2 atan(vec2 y_over_x)  vec3 atan(vec3 y_over_x)  vec4 atan(vec4 y_over_x)float atan(float y, float x)  vec2 atan(vec2 y, vec2 x)  vec3 atan(vec3 y, vec3 x)vec4 atan(vec4 y, vec4 x)|1. 返回值为弧度 2. 参数类型可以为浮点标量或者浮点向量 3. 注意该函数具有两参数版本,等价于C语言中atan2函数|

### 指数函数(exponential functions)
|函数名|说明|示例|注意点|
|----|----|----|----|
|pow|-|float pow(float x, float y)  vec2 pow(vec2 x, vec2 y)  vec3 pow(vec3 x, vec3 y)  vec4 pow(vec4 x, vec4 y)|1. 返回 x^y 2. 参数类型可以为浮点标量或者浮点向量|
|exp|-|float exp(float x)  vec2 exp(vec2 x)  vec3 exp(vec3 x)  vec4 exp(vec4 x)|1. 返回 e^x 2. 参数类型可以为浮点标量或者浮点向量|
|log|-|float log(float x)  vec2 log(vec2 x)  vec3 log(vec3 x)  vec4 log(vec4 x)|1. 返回 loge(x),即以e为底的对数函数 2. 参数类型可以为浮点标量或者浮点向量|
|exp2|-|float exp2(float x)  vec2 exp2(vec2 x)  vec3 exp2(vec3 x)  vec4 exp2(vec4 x)|1. 返回 2^x 2. 参数类型可以为浮点标量或者浮点向量|
|log2|-|float log2(float x)  vec2 log2(vec2 x)  vec3 log2(vec3 x)  vec4 log2(vec4 x)|1. 返回 log2(x),即以2为底的对数函数 2. 参数类型可以为浮点标量或者浮点向量|
|sqrt|-|float sqrt(float x)  vec2 sqrt(vec2 x)  vec3 sqrt(vec3 x)  vec4 sqrt(vec4 x)|1. 返回 x 的平方根 2. 参数类型可以为浮点标量或者浮点向量|
|inversesqrt|-|float inversesqrt(float x)  vec2 inversesqrt(vec2 x)  vec3 inversesqrt(vec3 x)  vec4 inversesqrt(vec4 x)|1. 返回 x 的平方根的倒数 2. 参数类型可以为浮点标量或者浮点向量|

### 一般函数(common functions)
|函数名|说明|示例|注意点|
|----|----|----|----|
|abs|绝对值函数|float abs(float x)  vec2 abs(vec2 x)  vec3 abs(vec3 x)  vec4 abs(vec4 x)|1. x >= 0 返回 xx < 0  返回 -x 2. 参数类型可以为浮点标量或者浮点向量|
|sign|符号函数|float sign(float x)  vec2 sign(vec2 x)  vec3 sign(vec3 x)  vec4 sign(vec4 x)|1. x > 0 返回 1 x = 0 返回 0x < 0 返回 -12. 参数类型可以为浮点标量或者浮点向量|
|floor|下取整函数|float floor(float x)  vec2 floor(vec2 x)  vec3 floor(vec3 x)  vec4 floor(vec4 x)|1. 返回 <= x 的最大整数2. 参数类型可以为浮点标量或者浮点向量3. 函数仍然使用浮点类型返回整数结果|
|ceil|上取整函数|float ceil(float x)  vec2 ceil(vec2 x)  vec3 ceil(vec3 x)  vec4 ceil(vec4 x)|1. 返回 >= x 的最小整数2. 参数类型可以为浮点标量或者浮点向量3. 函数仍然使用浮点类型返回整数结果|
|fract|-|float fract(float x)  vec2 fract(vec2 x)  vec3 fract(vec3 x)  vec4 fract(vec4 x)|1. 返回 x 的小数部分,即返回 x - floor(x)2. 参数类型可以为浮点标量或者浮点向量|
|mod|取模函数|float mod(float x, float y)  vec2 mod(vec2 x, vec2 y)  vec3 mod(vec3 x, vec3 y)  vec4 mod(vec4 x, vec4 y)vec2 mod(vec2 x, float y)  vec3 mod(vec3 x, float y)  vec4 mod(vec4 x, float y)|1. 返回 x % y 或者说返回 x - y * floor(x / y)2. 参数类型可以为浮点标量或者浮点向量3. 存在第一参数为浮点向量第二参数为浮点标量的函数版本|
|min|最小值函数|float min(float x, float y)  vec2 min(vec2 x, vec2 y)  vec3 min(vec3 x, vec3 y)  vec4 min(vec4 x, vec4 y)vec2 min(vec2 x, float y)  vec3 min(vec3 x, float y)  vec4 min(vec4 x, float y)|1. 返回 x 和 y 的较小值2. 参数类型可以为浮点标量或者浮点向量3. 存在第一参数为浮点向量第二参数为浮点标量的函数版本|
|max|最大值函数|float max(float x, float y)  vec2 max(vec2 x, vec2 y)  vec3 max(vec3 x, vec3 y)  vec4 max(vec4 x, vec4 y)vec2 max(vec2 x, float y)  vec3 max(vec3 x, float y)  vec4 max(vec4 x, float y)|1. 返回 x 和 y 的较大值2. 参数类型可以为浮点标量或者浮点向量3. 存在第一参数为浮点向量第二参数为浮点标量的函数版本|
|clamp|-|float clamp(float x, float minVal, float maxVal)  vec2 clamp(vec2 x, vec2 minVal, vec2 maxVal)  vec3 clamp(vec3 x, vec3 minVal, vec3 maxVal)  vec4 clamp(vec4 x, vec4 minVal, vec4 maxVal)vec2 clamp(vec2 x, float minVal, float maxVal)  vec3 clamp(vec3 x, float minVal, float maxVal)  vec4 clamp(vec4 x, float minVal, float maxVal)|1. x >= minVal 并且 x <= maxVal 返回 xx < minVal                   返回 minValx > maxVal                   返回 maxVal2. 参数类型可以为浮点标量或者浮点向量3. 存在第一参数为浮点向量第二参数及第三参数为浮点标量的函数版本|
|mix|-|float mix(float x, float y, float a)  vec2 mix(vec2 x, vec2 y, vec2 a)  vec3 mix(vec3 x, vec3 y, vec3 a)  vec4 mix(vec4 x, vec4 y, vec4 a)vec2 mix(vec2 x, vec2 y, float a)  vec3 mix(vec3 x, vec3 y, float a)  vec4 mix(vec4 x, vec4 y, float a)|1. 按 a 对 x 和 y 进行线性插值,即返回 (1 - a) * x + a * y2. 参数类型可以为浮点标量或者浮点向量3. 存在第一参数及第二参数为浮点向量第三参数为浮点标量的函数版本|
|step|-|float step(float edge, float x)  vec2 step(vec2 edge, vec2 x)  vec3 step(vec3 edge, vec3 x)  vec4 step(vec4 edge, vec4 x)vec2 step(float edge, vec2 x)  vec3 step(float edge, vec3 x)  vec4 step(float edge, vec4 x)|1. x < edge 返回 0x >= edge 返回 12. 参数类型可以为浮点标量或者浮点向量3. 存在第一参数为浮点标量第二参数为浮点向量的函数版本|
|smoothstep|-|float smoothstep(float edge0, float edge1, float x)  vec2 smoothstep(vec2 edge0, vec2 edge1, vec2 x)  vec3 smoothstep(vec3 edge0, vec3 edge1, vec3 x)  vec4 smoothstep(vec4 edge0, vec4 edge1, vec4 x)vec2 smoothstep(float edge0, float edge1, vec2 x)  vec3 smoothstep(float edge0, float edge1, vec3 x)  vec4 smoothstep(float edge0, float edge1, vec4 x)|1. x < edge0 返回 0x > edge1 返回 1x >= edge0 并且 x <= edge1 返回 x * x * (3 - 2 * x) (**Hermite多项式**)2. 参数类型可以为浮点标量或者浮点向量3. 存在第一参数及第二参数为浮点标量第三参数为浮点向量的函数版本|

### 几何函数(geometric functions)
|函数名|说明|示例|注意点|
|----|----|----|----|
|length|长度函数|float length(float x)  float length(vec2 x)  float length(vec3 x)  float length(vec4 x)|1. 返回向量长度2. 参数类型可以为浮点标量或者浮点向量3. 如果参数为浮点标量,返回 abs(x)|
|distance|距离函数|float distance(float p0, float p1)  float distance(vec2 p0, vec2 p1)  float distance(vec3 p0, vec3 p1)  float distance(vec4 p0, vec4 p1)|1. 返回向量间距离,即返回 length(p0 - p1)2. 参数类型可以为浮点标量或者浮点向量3. 如果参数为浮点标量,返回 abs(p0 - p1)|
|dot|点积函数|float dot(float x, float y)  float dot(vec2 x, vec2 y)  float dot(vec3 x, vec3 y)  float dot(vec4 x, vec4 y)|1. 返回向量点乘结果(标量)2. 参数类型可以为浮点标量或者浮点向量3. 如果参数为浮点标量,返回 x * y|
|cross|差积函数|vec3 cross(vec3 x, vec3 y)|1. 返回向量差积结果(向量)2. 参数类型只能为3维浮点向量|
|normalize|归一化函数|float normalize(float x)  vec2 normalize(vec2 x)  vec3 normalize(vec3 x)  vec4 normalize(vec4 x)|1. 返回向量归一化结果,即返回 x / length(x)2. 参数类型可以为浮点标量或者浮点向量3. 如果参数为浮点标量,返回 1|
|faceforward|-|float faceforward(float N, float I, float Nref)  vec2 faceforward(vec2 N, vec2 I, vec2 Nref)  vec3 faceforward(vec3 N, vec3 I, vec3 Nref)  vec4 faceforward(vec4 N, vec4 I, vec4 Nref)|1. dot(I, Nref) < 0 返回 Ndot(I, Nref) >= 0 返回 -N2. 参数类型可以为浮点标量或者浮点向量|
|reflect|反射函数|float reflect(float I, float N)  vec2 reflect(vec2 I, vec2 N)  vec3 reflect(vec3 I, vec3 N)  vec4 reflect(vec4 I, vec4 N)|1. I 是入射向量(向量方向为从入射点到反射点),N 为反射面法向量(需要归一化)2. 返回 I - 2.0 * dot(N, I) * N|
|refract|折射函数|float refract(float I, float N, float eta)  vec2 refract(vec2 I, vec2 N, float eta)  vec3 refract(vec3 I, vec3 N, float eta)  vec4 refract(vec4 I, vec4 N, float eta)|1. I 是入射向量(向量方向为从入射点到反射点,需要归一化),N 为反射面法向量(需要归一化),eta 为相对折射率(入射介质的折射率 / 折射介质的折射率)2. 返回值的计算[如下所示](#refract_section)|

```
k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
if (k < 0.0)
    R = ReturnType(0) // 不发生折射
else
    R = eta * I - (eta * dot(N, I) + sqrt(k)) * N;|
```

### 矩阵函数(matrix functions)
|函数名|说明|示例|注意点|
|----|----|----|----|
|matrixCompMult|-|mat2 matrixCompMult(mat2 x, mat2 y)  mat3 matrixCompMult(mat3 x, mat3 y)  mat4 matrixCompMult(mat4 x, mat4 y)|结果矩阵按如下公式计算 z[i][j] = x[i][j] * y[i][j],**注意与矩阵乘法的区别**|

### 向量关系函数(vector relational functions)
|函数名|说明|示例|注意点|
|----|----|----|----|
|lessThan|小于函数|bvec2 lessThan(vec2 x, vec2 y)  bvec3 lessThan(vec3 x, vec3 y)    bvec4 lessThan(vec4 x, vec4 y)  bvec2 lessThan(ivec2 x, ivec2 y)  bvec3 lessThan(ivec3 x, ivec3 y)  bvec4 lessThan(ivec4 x, ivec4 y)|注意各个函数版本的参数|
|lessThanEqual|小于等于函数|bvec2 lessThanEqual(vec2 x, vec2 y)  bvec3 lessThanEqual(vec3 x, vec3 y)  bvec4 lessThanEqual(vec4 x, vec4 y)  bvec2 lessThanEqual(ivec2 x, ivec2 y)  bvec3 lessThanEqual(ivec3 x, ivec3 y)bvec4 lessThanEqual(ivec4 x, ivec4 y)|注意各个函数版本的参数|
|greaterThan|大于函数|bvec2 greaterThan(vec2 x, vec2 y)  bvec3 greaterThan(vec3 x, vec3 y)  bvec4 greaterThan(vec4 x, vec4 y)  bvec2 greaterThan(ivec2 x, ivec2 y)  bvec3 greaterThan(ivec3 x, ivec3 y)  bvec4 greaterThan(ivec4 x, ivec4 y)|注意各个函数版本的参数|
|greaterThanEqual|大于等于函数|bvec2 greaterThanEqual(vec2 x, vec2 y)  bvec3 greaterThanEqual(vec3 x, vec3 y)  bvec4 greaterThanEqual(vec4 x, vec4 y)  bvec2 greaterThanEqual(ivec2 x, ivec2 y)  bvec3 greaterThanEqual(ivec3 x, ivec3 y)bvec4 greaterThanEqual(ivec4 x, ivec4 y)|注意各个函数版本的参数|
|equal|等于函数|bvec2 equal(vec2 x, vec2 y)  bvec3 equal(vec3 x, vec3 y)  bvec4 equal(vec4 x, vec4 y)  bvec2 equal(ivec2 x, ivec2 y)  bvec3 equal(ivec3 x, ivec3 y)  bvec4 equal(ivec4 x, ivec4 y)|注意各个函数版本的参数|
|notEqual|不等于函数|bvec2 notEqual(vec2 x, vec2 y)  bvec3 notEqual(vec3 x, vec3 y)  bvec4 notEqual(vec4 x, vec4 y)  bvec2 notEqual(ivec2 x, ivec2 y)  bvec3 notEqual(ivec3 x, ivec3 y)  bvec4 notEqual(ivec4 x, ivec4 y)|注意各个函数版本的参数|
|any|-|bool any(bvec2 x)  bool any(bvec3 x)  bool any(bvec4 x)|参数的任一分量为true,则返回true,否则返回false|
|all|-|bool all(bvec2 x)  bool all(bvec3 x)  bool all(bvec4 x)|参数的所有分量为true,则返回true,否则返回false|
|not|-|bvec2 not(bvec2 x)  bvec3 not(bvec3 x)  bvec4 not(bvec4 x)|参数的所有分量取反|

### 贴图采样函数(texture lookup functions)
|函数名|说明|示例|注意点|
|----|----|----|----|
|texture2D|-|vec4 texture2D(sampler2D sampler, vec2 coord)  vec4 texture2D(sampler2D sampler, vec2 coord, float bias)|bias参数用于调整贴图lod的选取采样(在选取的 mipmap level 上做偏移)|
|textureCube|-|vec4 textureCube(samplerCube sampler, vec3 coord)  vec4 textureCube(samplerCube sampler, vec3 coord, float bias)|bias参数用于调整贴图lod的选取采样(在选取的 mipmap level 上做偏移)|

