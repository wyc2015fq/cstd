# 【D3D11游戏编程】学习笔记十五：混合（Blending） - BonChoix的专栏 - CSDN博客





2012年12月31日 06:28:52[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：10884
所属专栏：[D3D11游戏编程](https://blog.csdn.net/column/details/d3d11-bonchoix.html)









（注：【D3D11游戏编程】学习笔记系列由CSDN作者BonChoix所写，转载请注明出处：[http://blog.csdn.net/BonChoix](http://blog.csdn.net/BonChoix)，谢谢~）



       在D3D11中，“混合”发生在像素着色器阶段的下一阶段，即Output Merger Stage。整个场景在全部经历过像素着色器阶段后，对应于屏幕上每一点像素，可能有多个片段（Fragment）。如下图所示：

![](https://img-my.csdn.net/uploads/201212/30/1356881669_9814.png)

        该图中，场景中有三个点P1,P2,P3投影在屏幕上同一个点P。这样在像素着色器阶段后，针对P1,P2,P3将有三个片段与像素P对应。默认情况下，在渲染管线中，混合是被关闭的。这时为了确定像素P的最终显示颜色，主要依据是深度测试（这时暂不考虑模板测试等其他因素）。通过深度测试的片段将自身颜色替代后缓冲区中P点的当前颜色，未通过的片段被抛弃。

       当混合功能被打开时，决定最终颜色的方法有所不同。当一个片段通过深度测试后，并不是直接取代后缓冲区中P点的当前颜色，而是通过一定的比例因子与之进行插值（混合），并将结果作为P点的当前值。当然，未通过深度测试的片段依然被抛弃。这个是“混合”的一种最简单例子。除此之外，D3D11针对混合阶段有非常多的配置，从而实现各种特殊效果。



##        1. 混合方程

       学习混合的第一步，就是要了解混合方程，方程如下：

![](https://img-my.csdn.net/uploads/201212/31/1356884062_7616.png)

       该方程针对每个像素逐一进行。方程左边的C为混合结果，右边Csrc（我们称为源颜色）和Cdst（我们称为目标颜色）分别为即将要处理的片段的颜色和后缓冲区中该像素当前的颜色；Fsrc和Fdst分别是两个颜色对应的混合因子。注意方程中的![](https://img-my.csdn.net/uploads/201212/31/1356884632_9482.png)在这里为“分量相乘”（Componentwise multiplication），即针对颜色值中的R、G、B三种分量分别进行相乘。源颜色和目标颜色分别与相应的混合因子分量相乘后，将结果进行“op"（混合操作）操作，作为当前片段处理的最终颜色，并替换后缓冲区中该像素处的颜色。

       以上计算公式只适合于颜色值中RGB三个分量。此外，Alpha分量的计算公式完全一致：

![](https://img-my.csdn.net/uploads/201212/31/1356886220_9209.png)



##        2. 混合操作

> 
针对上述公式中的混合操作”op"，在D3D11中定义在如下枚举类型中：

```cpp
typedef enum D3D11_BLEND_OP {
  D3D11_BLEND_OP_ADD            = 1,
  D3D11_BLEND_OP_SUBTRACT       = 2,
  D3D11_BLEND_OP_REV_SUBTRACT   = 3,
  D3D11_BLEND_OP_MIN            = 4,
  D3D11_BLEND_OP_MAX            = 5 
} D3D11_BLEND_OP;
```

我们省略各个变量的前缀D3D11_BLEND_OP_。

ADD表示相加操作，即![](https://img-my.csdn.net/uploads/201212/31/1356885283_3291.png)；

SUBTRACT表示相关（目标-源），即![](https://img-my.csdn.net/uploads/201212/31/1356885341_4372.png)；

REV_SUBTRACT表示反射的相关（源-目标），即![](https://img-my.csdn.net/uploads/201212/31/1356885335_1278.png)；

MIN表示取源、目标颜色中较小值，即![](https://img-my.csdn.net/uploads/201212/31/1356885714_1083.png)；

MAX表示取源、目标颜色中较大值，即![](https://img-my.csdn.net/uploads/201212/31/1356885788_9083.png)。

注意：MIN和MAX操作与混合因子无关。

同样，所有这些操作也适合于计算Alpha值。


## 3. 混合因子

针对方程中的混合因子F，在D3D11中有如下几种：

D3D11_BLEND_ZERO：此外针对颜色混合，F为（0，0，0），针对alpha值，F为0。

D3D11_BLEND_ONE：针对颜色混合为（1，1，1），针对alpha值为1；

D3D11_BLEND_SRC_COLOR：针对颜色混合为（Rs，Gs, Bs），针对alpha值为As；

D3D11_BLEND_INV_SRC_COLOR：针对颜色混合为（1-Rs，1-Gs，1-Bs），针对alpha值为1-As；

D3D11_BLEND_SRC_ALPHA：针对颜色混合为（As,As,As），针对alpha值为As；

D3D11_BLEND_INV_SRC_ALPHA：针对颜色混合为（1-As,1-As,1-As），针对alpha值为1-As；

D3D11_BLEND_DEST_COLOR：针对颜色混合为（Rd,Gd,Bd），针对alpha值为Ad；

D3D11_BLEND_INV_DESC_COLOR：针对颜色混合为（1-Rd,1-Gd,1-Bd），针对alpha值为1-Ad；

D3D11_BLEND_DEST_ALPHA：针对颜色混合为（Ad,Ad,Ad），针对alpha值为Ad；

D3D11_BLEND_INV_DEST_ALPHA：针对颜色混合为(1-Rd,1-Rd,1-Rd），针对alpha值为1-Ad；

D3D11_BLEND_BLEND_FACTOR：此时的混合因子为程序员指定的颜色值（R,G,B,A)，针对颜色混合为(R,G,B)，针对alpha值为A。该颜色值通过函数ID3D11DeviceContext::OMSetBlendState来指定；

D3D11_BLEND_INV_BLEND_FACTOR：同上，为程序员指定颜色值，针对颜色混合为(1-R,1-G,1-B)，针对alpha值为1-A。




> 
## 4. 混合状态

D3D11中，设置混合状态前要先创建相应的混合状态接口ID3D11BlendState，创建函数如下：

```cpp
HRESULT CreateBlendState(
  [in]   const D3D11_BLEND_DESC *pBlendStateDesc,
  [out]  ID3D11BlendState **ppBlendState
);
```

第二个参数为要创建的接口的地址，第一个参数为一个用来描述混合状态参数的结构，定义如下：

```cpp
typedef struct D3D11_BLEND_DESC {
  BOOL                           AlphaToCoverageEnable;
  BOOL                           IndependentBlendEnable;
  D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
} D3D11_BLEND_DESC;
```



第一个参数设置是否打开AlphaToCoverage，AlphaToCoverage在后面会详细介绍，暂时先不用，设置为false；

第二个参数设置是否针对不同的RenderTarget使用不同的混合状态。在D3D11中，一共可以支持多达8个的渲染对象（RenderTarget），如果针对不同的对象想使用不同的混合方式，则设置为true。由于我们暂时用不到，因此设置为false；

第三个参数为针对8个RenderTarget分别指定的混合状态参数，当第二个参数为false时，这里我们只需要设置数组中第一个元素即可。

D3D11_RENDER_TARGET_BLEND_DESC结构定义如下：

```cpp
typedef struct D3D11_RENDER_TARGET_BLEND_DESC {
  BOOL           BlendEnable;
  D3D11_BLEND    SrcBlend;
  D3D11_BLEND    DestBlend;
  D3D11_BLEND_OP BlendOp;
  D3D11_BLEND    SrcBlendAlpha;
  D3D11_BLEND    DestBlendAlpha;
  D3D11_BLEND_OP BlendOpAlpha;
  UINT8          RenderTargetWriteMask;
} D3D11_RENDER_TARGET_BLEND_DESC;
```

第一个需要设置为true，以开启混合状态；

后面的参数即我们的混合方程中的参数：

SrcBlend、DestBlend分别为源、目标颜色混合因子；

BlendOp为源、目标颜色的混合操作；

SrcBlendAlpha、DestBlendAlpha为源、目标alpha值的混合因子；

BlendOpAlpha为源、目标alpha值的混合操作；

RenderTargetWriteMask为最终混合结果在写到缓冲区时的掩码，即用来指定哪些位写进去，哪些们不能写。针对该参数有如下枚举类型：

```cpp
typedef enum D3D11_COLOR_WRITE_ENABLE {
  D3D11_COLOR_WRITE_ENABLE_RED     = 1,
  D3D11_COLOR_WRITE_ENABLE_GREEN   = 2,
  D3D11_COLOR_WRITE_ENABLE_BLUE    = 4,
  D3D11_COLOR_WRITE_ENABLE_ALPHA   = 8,
  D3D11_COLOR_WRITE_ENABLE_ALL     = 
      ( D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN |  
        D3D11_COLOR_WRITE_ENABLE_BLUE | D3D11_COLOR_WRITE_ENABLE_ALPHA ) 
} D3D11_COLOR_WRITE_ENABLE;
```

其中，RED、GREEN、BLUE和ALPHA分别表示只允许写入R、G、B、A部分的值。默认情况下，我们把带个颜色值替换缓冲区中的值，因此该参数我们指定为D3D11_COLOR_WRITE_ENABLE_ALL。



创建好ID3D11BlendState接口后，通过以下函数来设置为指定的状态：

```cpp
void OMSetBlendState(
  [in]  ID3D11BlendState *pBlendState,
  [in]  const FLOAT* BlendFactor,
  [in]  UINT SampleMask
);
```

第一个参数即创建的接口；

第二个参数为程序员手动指定的混合因子，即刚介绍混合因子时，如果指定参数为D3D11_BLEND_BLEND_FACTOR或D3D11_BLEND_INV_BLEND_FACTOR，则使用第二个参数指定的颜色值为了混合因子；

第三个参数为采样点掩码。D3D11中的多重采样可以支持32个采样点，该参数用来决定”使用/丢弃"哪些采样点。该参数类型为UINT，32位，其中从最低位到最高位分别代表一个采样点。比如，如果第5位指定为0，则第5个采样点将被丢弃。当然，只有当开启至少5重采样时该设定才有效。如果当前设置为单个采样点，则只有最低位才对我们有用。默认情况下，该参数值为0xFFFFFFFF，即对所有采样点有效。



Voila，使用混合的所有步骤就这些~ 初次接触是不是觉得有点头晕？有太多的结构要填，参数名又非常长！ 其实一点也不难，参数名长有一个很大的好处，就是“自解释”，看到名字一下子就能明白其用意，而且还很容易记住。只要理解了混合的基本原理，这些过程其实是一气呵成的事。以下是一个开启混合的例子：

```cpp
//开启透明
D3D11_BLEND_DESC transDesc;
//先创建一个混合状态的描述
transDesc.AlphaToCoverageEnable = false;		//关闭AlphaToCoverage
transDesc.IndependentBlendEnable = false;		//不针对多个RenderTarget使用不同的混合状态
//因此只设置第一个数组元素即可
transDesc.RenderTarget[0].BlendEnable = true;
transDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
transDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
transDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
transDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
transDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
transDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
transDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//创建ID3D11BlendState接口
device->CreateBlendState(&transDesc,&TransparentBS);

//现在可以设置混合状态了
//先指定混合因子，一般不用它，除非在上面混合因子指定为使用blend factor
float factor[4] = {1.f,1.f,1.f,1.f};
//使用该状态
deviceContext->OMSetBlendState(TransparentBS,factor,0xffffffff);	//32个采样点都有效
```

渲染完后，一般要恢复默认状态，这时使用NULL参数即可，如下：

```cpp
deviceContext->OMSetBlendState(0,factor,0xffffffff);
```

## 5. 几种混合的例子

在D3D中通过合理地设置不同的混合状态，可以实现各种各样的效果。以下是几个常见的例子：

### 5.1 禁止颜色写入

有时候，在渲染过程中，我们只希望修改深度/模板缓冲区部分，而且希望保持后缓冲区中的原有颜色值，这时修，我们需要“禁止颜色写入”。

一种方法是把目标混合因子设为D3D11_BLEND_ONE，把源混合因子设为D3D11_BLEND_ZERO，这样混合方程中源部分相乘后结果为0，目标部分相乘后保持原样，相加结果仍为原来的颜色，这样即禁止了颜色的写入；还有另一种更为直观的方法是，直接把描述中D3D11_RENDER_TARGET_BLEND_DESC::RenderTargetWriteMask成员设为0，即任何一位都无法写入。

### 5.2 把颜色相加、相减

在处理片段时，如果我们希望把片段颜色与后缓冲区中当前颜色值相加。这时，可以通过把源、目标混合因子全部设为D3D11_BLEND_ONE。这样，在乘以相应的混合因子后，源、目标颜色保持不变。对于混合操作，如果是实现相加，可以设置为D3D11_BLEND_OP_ADD，如果为相减，可以设置为D3D11_BLEND_OP_SUBTRACT或D3D11_BLEND_OP_REV_SUBTRACT（取决于源颜色值减目标颜色值，还是相反）。

### 5.3 把颜色相乘

如果希望把片段颜色值与后缓冲区中对应的当前值相乘，可以设置目标混合因子为D3D11_BLEND_SRC_COLOR，而把源混合因为设为D3D11_BLEND_ZERO。这样，混合方程中源部分变为0，对于目标部分，由于混合因子是片段的颜色值，因为目标颜色乘以混合因子，实际上就是目标颜色与源颜色相乘了。至于混合操作，设置为相加即可，即D3D11_BLEND_OP_ADD。

### 5.4 透明效果

有时候，我们需要渲染透明的物体，比如玻璃。透过玻璃，我们可以看见其后面的物体。一般情况下，实现透明效果时，我们需要用到该物体的alpha值。比如对于alpha为0.4，意思是该物体60%透明，即最终我们观察到的颜色40%来自该物体，60%来自其后面的物体。要实现这种效果，我们可以为源颜色指明混合因子为D3D11_BLEND_SRC_ALPHA，即该片段对应的alpha值，目标因子设为D3D11_BLEND_INV_ALPHA。混合操作设置相加：D3D11_BLEND_OP_ADD。这时，比如透明物体的alpha值为0.25，即源混合因子为0.25，目标混合因子为0.75。



## 6. 使用“透明”效果时的注意事项

在场景中包含透明物体时需要额外注意的是，要先渲染不透明物体，然后把透明物体由远到近逐个渲染。

原因很简单，如果很渲染了透明物体，这样当渲染该透明物体后面的物体时，将无法通过深度测试而被丢弃，从而导致无法再看到透明物体后面的物体。

## 7. 示例程序

在本节的示例程序中，演示的是水面的透明效果。场景为一个很大的水池，透过水面，可以看到水池底部以及水中的物体。当然，这里的水面仅仅是静态的水面，没有任何波动效果。以下是程序截图：
![](https://img-my.csdn.net/uploads/201212/31/1356892996_7417.png)

操作说明：鼠标左键按下旋转屏幕，右键按下调整镜头远近。

以下是示例程序源代码：

[D3D11“透明"效果示例程序](http://download.csdn.net/detail/bonchoix/4945713)








