# 【D3D11游戏编程】学习笔记二：XNAMath之XMVECTOR - BonChoix的专栏 - CSDN博客





2012年12月11日 02:55:01[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：13092
所属专栏：[D3D11游戏编程](https://blog.csdn.net/column/details/d3d11-bonchoix.html)









（注：【D3D11游戏编程】学习笔记系列由CSDN作者BonChoix所写，转载请注明出处：[http://blog.csdn.net/BonChoix](http://blog.csdn.net/BonChoix)，谢谢~）



       一、XNA Math简介

       在D3D10及之前的版本中，3D数学库是伴随在D3DX库中的。在D3D11版中，3D数学库被单独隔离出来，为XNA Math库，功能和之前基本一样，但是建立在SIMD指令上，以更好地利用Windows及XBox360上特殊的硬件寄存器（128位，可以同时操作4个32位数）。

       二、向量类型

       在XNA数学库中，核心的向量类型为XMVECTOR，它正好映射为SIMD硬件上的128位寄存器，以利用一个指令同时操作4个32位的数。当SEE2指令集可用时，可以定义为：

```cpp
typedef __m128 XMVECTOR;
```



在操作向量时，XMVECTOR除了用于4维向量外，还可以用于操作2维、三维向量，以于多出的几维默认为0，不关心即可。

       此外要注意的是，XMVECTOR在内存中是16位对齐的，当作为局部或全局变量使用时，由硬件自动实现对齐。对于类中的成员变量，则推荐使用XMFLOAT2(2D), XMFLOAT3(3D), XMFLOAT4(4D)来代替XMVECTOR，定义分别如下：

```cpp
typedef struct _XMFLOAT2
{
    FLOAT x;
    FLOAT y;
} XMFLOAT2;

typedef struct _XMFLOAT3
{
    FLOAT x;
    FLOAT y;
    FLOAT z;
} XMFLOAT3;

typedef struct _XMFLOAT4
{
    FLOAT x;
    FLOAT y;
    FLOAT z;
    FLOAT w;
} XMFLOAT4;
```



       但是，如果直接使用XMFLOAT2、XMFLOAT3等这些类型进行计算，是不会利用到SIMD指令的加速效果的，因此在计算前要把这些向量转换为XMVECTOR，然后再进行各种向量运算。这几种类型与XMVECTOR之间的相互转换函数主要分为Store和Load两种，Store型用来把一个XMVECTOR存储到指定的XMFLOATx中，Load型用来从一个XMFLOATx读取内容到XMVECTOR中。总结下这段话内容，即：

1. 在局部、全局变量中使用XMVECTOR类型；

2. 在类中定义向量成员时，使用类型XMFLOAT2，XMFLOAT3，XMFLOAT4；

3. 对类中的向量进行运算时，用Load型函数把相应向量读取到XMVECTOR中，再进行运算；

4. 运算完后把相应的结果XMVECTOR通过Store型函数存储到相应的XMFLOATx向量中。

       三、 Load型和Store型函数

       Load型函数用来从一个XMFLOATx向量中读取内容到XMVECTOR中。如下几种：

```cpp
XMVECTOR XMLoadFloat2(CONST XMFLOAT2 *pSource);		//从XMFLOAT2中读取内容到一个XMVECTOR中
XMVECTOR XMLoadFloat3(CONST XMFLOAT3 *pSource);		//从XMFLOAT3中读取内容到一个XMVECTOR中
XMVECTOR XMLoadFloat4(CONST XMFLOAT4 *pSource);		//从XMFLOAT4中读取内容到一个XMVECTOR中
```

       此外除了XMFLOATx类型，还可以Load到其他类型的向量数据中，如：

```cpp
XMVECTOR XMLoadInt2(CONST UINT *pSource);		//从二维UINT向量中读取内容到一个XMVECTOR向量中
XMVECTOR XMLoadColor(CONST XMCOLOR *pSource);		//从XMCOLOR中读取内容到一个XMVECTOR中
XMVECTOR XMLoadByte4(CONST XMBYTE4 *pSource);		//从XMBYTE4中读取内容到一个XMVECTOR中
```

       Store型函数用来把一个XMVECTOR存储到一个XMFLOATx向量中。如下几种：

```cpp
void XMStoreFloat2(XMFLOAT2 *pDest, FXMVECTOR V);	//把XMVECTOR存储到一个XMFLOAT2向量中
void XMStoreFloat3(XMFLOAT3 *pDest, FXMVECTOR V);	//把XMVECTOR存储到一个XMFLOAT3向量中
void XMStoreFloat4(XMFLOAT4 *pDest, FXMVECTOR V);	//把XMVECTOR存储到一个XMFLOAT4向量中
```

       同理，除了XMFLOATx类型，还可以把XMVECTOR存储到其他类型向量中，如：

```cpp
void XMStoreInt3(UINT *pDest, FXMVECTOR V);		//把XMVECTOR存储到一个二维UINT向量中	
void XMStoreColor(XMCOLOR *pDest, FXMVECTOR V);		//把XMVECTOR存储到一个XMCOLOR向量中
void XMStoreByte4(XMBYTE4 *pDest, FXMVECTOR V);		//把XMVECTOR存储到一个XMBYTE4向量中
```



       由于XMVECTOR映射为一个特殊的SIMD寄存器，而不是一个struct，因此我们不能直接操作其x,y,z,w维数据，因此提供了以下几个Set和Get函数：

```cpp
FLOAT XMVectorGetX(FXMVECTOR V);	//得到XMVECTOR的X维数据
FLOAT XMVectorGetY(FXMVECTOR V);	//得到XMVECTOR的Y维数据
FLOAT XMVectorGetZ(FXMVECTOR V);	//得到XMVECTOR的Z维数据
FLOAT XMVectorGetW(FXMVECTOR V);	//得到XMVECTOR的W维数据

XMVECTOR XMVectorSetX(FXMVECTOR V, FLOAT x);	//设置一个XMVECTOR的X维数据，并返回新XMVECTOR
XMVECTOR XMVectorSetY(FXMVECTOR V, FLOAT y);	//设置一个XMVECTOR的Y维数据，并返回新XMVECTOR
XMVECTOR XMVectorSetZ(FXMVECTOR V, FLOAT z);	//设置一个XMVECTOR的Z维数据，并返回新XMVECTOR
XMVECTOR XMVectorSetW(FXMVECTOR V, FLOAT w);	//设置一个XMVECTOR的W维数据，并返回新XMVECTOR
```

       你可能注意到，在上面所有的函数中，有好多FXMVECTOR类型的参数，而不是XMVECTOR类型。别急，这正是我们下一步要说的。


       四、 XMVECTOR类型参数传递规定

       为了更好地利用SIMD，当XMVECTOR作为函数参数类型时，有特殊的规定。这些规则与平台相关，比如在32位和64位的Windows及XBox360下规则就不一样。为了在写代码时与平台无关，XNA Math专门针对函数参数类型定义了这两个类型：CXMVECTOR和FXMVECTOR。在不同的平台下这两个类型有着相应的不同的定义，对于程序员在写代码时则无需考虑这些，只要服从相应的规则即可。规则如下：

       在一个函数中，前三个使用到XMVECTOR类型的参数，必须为FXMVECTOR；之后所有其他的XMVECTOR类型参数要为CXMVECTOR。

```cpp
XMINLINE XMMATRIX XMMatrixTransformation(
		FXMVECTOR ScalingOrigin,					//第1个XMVECTOR类型参数
		FXMVECTOR ScalingOrientationQuaternion,				//第2个XMVECTOR类型参数
		FXMVECTOR Scaling,						//第3个XMVECTOR类型参数
		CXMVECTOR RotationOrigin,					//第4个开始，后面的全部为CXMVECTOR类型
		CXMVECTOR RotationQuaternion,
		CXMVECTOR Translation);
```

当然，各参数之间也可以夹杂其他非XMVECTOR类型的参数，但只要是前三个类型为XMVECTOR的参数使用FXMVECTOR，后面所有类型为XMVECTOR的参数使用CXMVECTOR即可。例如：

```cpp
XMINLINE XMMATRIX XMMatrixTransformation2D(
		FXMVECTOR ScalingOrigin,					//第1个XMVECTOR类型参数
		FLOAT	  ScalingOrientation,
		FXMVECTOR Scaling,						//第2个XMVECTOR类型参数
		FXMVECTOR RotationOrigin,					//第3个XMVECTOR类型参数
		FLOAT	  Rotation,
		CXMVECTOR Translation);						//第4个，必须使用CXMVECTOR
```

是不是感觉这规则有点奇怪？反正我感觉很奇怪，不过无所谓，其实也很简单，我们平时操作XMVECTOR类型进行运算时不用关心这个，仅仅是在定义函数时，如果参数类型为XMVECTOR，才需要考虑这个规则。     

        五、 常量向量

       当用到常量型的XMVECTOR时，应该使用XMVECTORF32类型，尤其是当使用初始化式时。例如：

```cpp
const XMVECTORF32 g_Zero = {0.f,0.f,0.f,0.f};
static const XMVECTORF32 g_tmp = {x,y,z,w};
```

       XMVECTORF32是16字节对齐的数据结构，可以转换为XMVECTOR类型。定义如下：

```cpp
typedef _DECLSPEC_ALIGN_16_ struct XMVECTORF32
{
	union
	{
		float v[4];
		XMVECTOR v;
	};
	
	//如果是在C++环境下，则还有其他成员函数
	//比如重载()操作符，转换为XMVECTOR类型
};
```

因此，如果想事先定义一些常量型的XMVECTOR，则可以用const XMVECTORF32类型来定义，在程序中用到XMVECTOR类型时，再利用类型转换（static_cast）转换为XMVECTOR类型即可。比如我们可能想事先定义好各种常见的颜色，像白色可以这样定义：const XMFLOAT32 White = {1.f, 1.f, 1.f, 1.f};

       六、 针对XMVECTOR类型重载的操作符

       在C++环境下，针对各种常见的向量操作，对XMVECTOR重载了相应的操作符。比如：向量加减+、-、+=、-=，向量与数字相乘：*、/、*=、/=，向量点积：*，*=等。

       七、 一些Setter函数

```cpp
XMVECTOR XMVectorZero();		//[0,0,0,0]
XMVECTOR XMVectorSplatOne();	//[1.f, 1.f, 1.f, 1.f]
XVMECTOR XMVectorSet(FLOAT x, FLOAT y, FLOAT z, FLOAT w);	//[x,y,z,w]
XMVECTOR XMVectorReplicate(FLOAT s);	//[s, s, s, s]
XMVECTOR XMVectorSplatX(FXMVECTOR v);	//[v.x, v.x, v.x, v.x]
XMVECTOR XMVectorSplatY(FXMVECTOR v);	//[v.y, v.y, v.y, v.y]
XMVECTOR XMVectorSplatZ(FXMVECTOR v);	//[v.y, v.y, v.y, v.y]
```

       八、 向量操作函数

```cpp
XMVECTOR XMVector3Length(FXMVECTOR v);	   //长度L，结果为[L,L,L,L],效率起见，全部为XMVECTOR类型，我们只取一个维即可。
XMVECTOR XMVector3LengthSq(FXMVECTOR v);   //长度L平方根，结果存放同上。其他任何返回scalar数的同理
XMVECTOR XMVector3Dot(FXMVECTOR v1, FXMVECTOR v2);	//点积dot，[dot, dot, dot, dot]
XMVECTOR XMVector3Cross(FXMVECTOR v1, FXMVECTOR v2);	//叉乘
XMVECTOR XMVector3Normalize(FXMVECTOR v);		//归一化
XMVECTOR XMVector3Orthogonal(FXMVECTOR v);		//返回一个垂直的向量
XMVECTOR XMVector3AngleBetweenVectors(FXMVECTOR v1, FXMVECTOR v2);  //返回两向量间角度angle，[angle, angle, angle, angle]
```



       学习XNA Math库的最好方法就是参考SDK，在安装好最新DirectX SDK后都会有个文档《DirectX Documentation for C++》，里面有XNA Math详细介绍，遇到不懂的函数可以直接在上面查找。以上只是对XMVECTOR的一些基本介绍，其他所有的XNA Math函数、类型都可以参考SDK。

       最后，附加作者Luna的源代码，通过示例来熟悉XMVECTOR的用法。

[XMVectorGetSampleCode();](http://download.csdn.net/detail/bonchoix/4870793)




