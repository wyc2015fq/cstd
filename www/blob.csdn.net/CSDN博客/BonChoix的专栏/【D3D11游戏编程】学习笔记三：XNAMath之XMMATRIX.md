# 【D3D11游戏编程】学习笔记三：XNAMath之XMMATRIX - BonChoix的专栏 - CSDN博客





2012年12月11日 05:35:14[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：10790
所属专栏：[D3D11游戏编程](https://blog.csdn.net/column/details/d3d11-bonchoix.html)









（注：【D3D11游戏编程】学习笔记系列由CSDN作者BonChoix所写，转载请注明出处：[http://blog.csdn.net/BonChoix](http://blog.csdn.net/BonChoix)，谢谢~）



       在熟悉了XMVECTOR的风格及规则之后，再来了XNA数学库中的矩阵就容易得多了。基本上可以跟向量相类比。

       一、 矩阵类型

       XNA数学库中针对矩阵的类型为XMMATRIX。这是一个4x4的矩阵，里面可以容纳16个32位浮点数。对于XMMATRIX的初始化，可以有多种方法，一种方法是传4个XMVECTOR参数，每个XMVECTOR对应矩阵的一行；第二种方法是直接传递16个浮点数，代表整个矩阵的内容；第三种方法是用一个FLOAT型的数组，代表整个矩阵的内容。当然，这三种方法都有点麻烦，不过在实际中，自己来手动初始化矩阵的地方也不多，了解下其初始化方法即可。其次，XMMATRIX重载了（）操作符，用来访问矩阵中的成员，用法跟正常矩阵访问完全一样，即传递两个参数(row,
 col)即可。 XMMATRIX也重载了*和*=操作符，用途很明显，用来进行矩阵的乘积。

       二、 类成员矩阵类型

       跟XMVECTOR一样，作为局部或全局变量使用时，直接使用XMMATRIX即可。当作为类中成员类型时，推荐使用的类型为XMFLOAT4X4，从名字上一目了然，即4x4矩阵，不像向量一样提供了多种维的类型，这里只提供了4x4的通用类型的矩阵，如果是用较小维的矩阵，多出来的不关心即可。XMFLOAT4X4也重载了（）操作符用来访问内部成员。

       三、 作为函数参数类型时的规则

       同样，XMMATRIX作为函数参数类型时，其使用也要服从一定的规则。这里规则更为简单，即统一要求使用CXMMATRIX。即在函数定义中，类型为XMMATRIX的形参必须用CXMMATRIX类型代替。

       四、 矩阵相关函数

```cpp
XMMATRIX XMMatrixIdentity();				//返回单位矩阵，即对角线全1，其他全0
BOOL	 XMMatrixIsIdentity(CXMMATRIX M);		//判断一个矩阵是否为单位矩阵
XMMATRIX XMMatrixMultiply(CXMMATRIX A, CXMMATRIX B);	//两矩阵相乘
XMMATRIX XMMatrixTranspose(CXMMATRIX M);		//矩阵转置
XMVECTOR XMMatrixDeterminant(CXMMATRIX M);		//求矩阵对应行列式的值det
XMMATRIX XMMatrixInverse(XMVECTOR *pDet, CXMMATRIX M);	//求矩阵M的逆，*pDet为该矩阵的det
```

       五、 3D中矩阵变换相关函数

```cpp
XMMATRIX XMMatrixScaling(FLOAT scaleX, FLOAT scaleY, FLOAT scaleZ);	//伸缩变化，指定三个方向上的伸缩比
XMMATRIX XMMatrixScalingFromVector(FXMVECTOR scale);			//通过向量指定三个方向伸缩比，求伸缩变换
XMMATRIX XMMatrixRotationX(FLOAT angle);				//围绕x轴的旋转变换
XMMATRIX XMMatrixRotationY(FLOAT angle);				//围绕y轴的旋转变换
XMMATRIX XMMatrixRotationZ(FLOAT angle);				//围绕z轴的旋转变换
XMMATRIX XMMatrixRotationAxis(FXMVECTOR axis, FLOAT angle);		//围绕任意轴axis的旋转变换
XMMATRIX XMMatrixTranslation(FLOAT x, FLOAT y, FLOAT z);		//平移变换，指定三个方向的平移量
XMMATRIX XMMatrixTranslationFromVector(FXMVECTOR offset);		//通过向量指定三个方向的平移量，求平移变换
XMVECTOR XMVector3Transform(FXMVECTOR v, CXMMATRIX m);			//向量、矩阵相乘
XMVECTOR XMVector3TransformCoord(FXMVECTOR v, CXMMATRIX m); 		//向量、矩阵相乘，但向量w=1,因此为点的变换
XMVECTOR XMVector3TransformNormal(FXMVECTOR v, CXMMATRIX m);		//向量、矩阵相乘，但向量w=0,因此为向量的变换
```



       关于矩阵的基本知识就这么些，相比XMVECTOR简单了很多。等用多了，习惯了相关的规则及命名后，XNA Math库还是相当好用的~不但可以用于学习d3d11的过程中，而且在以后自己构架框架时也可以考虑使用，毕竟它进行了很大的优化，一般人自己很写出比它性能更好的数学库了，当然为了学习3D数学而自自己动手写数学库例外。

       下面是我学习XNA数学库时的一点小总结，其实这太明显了，不过对于更快的记住函数命名、熟练地使用还是有一定作用的：

       1. 所有类型名全部大写：XMVECTOR、XMVECTORF32、XMFLOAT2、XMFLOAT3、XMFLOAT4、XMCOLOR、XMBYTE4、XMMATRIX、XMFLOAT4X4等等

       2. 所有函数名以XM开头（意指DirectX Math)

       3. 跟向量操作相关的函数名在XM后全部以Vector3开头：XMVector3Length、XMVector3Normalize、XMVector3Transform等等（Load型、Store型、Set型、Get型等不属于“操作”相关的范畴，不满足这个特性）

       4. 跟矩阵操作相关的函数名在XM后全部以Matrix开头：XMMatrixScaling、XMMatrixRotationX、XMMatrixTranspose等等

       5. XMVector3、XMMatrix前缀后，所有单词首字母大写，其他小写（这个规则本身就很常见）



       XNA Math部分完毕，最后附上该部分的示例代码，供学习XMMATRIX参考。

[XMMatrixGetSampleCode();](http://download.csdn.net/detail/bonchoix/4870753)



       熟悉了XNA数学库，就开始真正走进D3D11的精彩世界啦~



