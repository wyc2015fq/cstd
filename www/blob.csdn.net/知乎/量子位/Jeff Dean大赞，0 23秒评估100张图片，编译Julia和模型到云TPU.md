# Jeff Dean大赞，0.23秒评估100张图片，编译Julia和模型到云TPU - 知乎
# 



> 伊瓢 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

不久前，Julia Computing官方放出了一篇论文，展示将Julia代码和机器学习模型编译到谷歌云TPU的方法，可以实现在0.23秒内完成100张图片VGG19正向传递。

Jeff Dean看到这个方法后，开心的转了一作的Twitter，并且评价：真是又快又容易呀！
![](https://pic3.zhimg.com/v2-e4a0be16efbdc93c5580779b1207fa66_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='456'></svg>)
该方法应用的正是谷歌上个月给TPU开放权限的XLA（Accelerated Linear Algebra）编译器。

## **XLA嵌入**

首先，需要定义动态语义和静态嵌入，定义一个运行时结构来嵌入XLA值


```
1const AA{T, N} = AbstractArray{T, N}
 2struct XRTArray{T, Shp, N} <: AA{T, N}
 3storage::XRTAllocation
 4# XRTArrays are constructable by
 5# conversion from regular arrays
 6function XRTArray(
 7a::Array{T, N}) where {T, N}
 8new{T, size(A), N}(transfer(a))
 9end
10# XRTArrays are constructable from a
11# remote memory allocation if
12# (T, Dims, N) are specified
13function XRTArray{T, Dims, N}(
14a::XRTAllocation) where {T, Dims, N}
15new{T, Dims, N}(a)
16end
17end
```


**△** XRTArray的定义

假设我们有一个示例XLA操作’Foo’采用一个静态操作数（例如一个整数）和两个动态操作数。 我们将声明此嵌入如下：


```
struct HloFoo <: HloOp{:foo}
static_operand::Int
end

function (hlo::HloFoo)(dop1::XRTArray,
dop2::XRTArray)
execute(hlo, dynamic_op1, dynamic_op2)
end
```


手动构建XLA嵌入：


```
1# An HLO operand that generates a random
 2# uniform random number of the specificed
 3# shape and element type:
 4struct HloRng <: HloOp{:rng}
 5Type
 6Shape
 7end
 8
 9"""A function that adds random numbers to
10each entry of a 1000x1000 matrix"""
11@eval function add_rand_1000x1000(
12A::XRTArray{Float32, (1000, 1000), 2}
13random = $(HloRng(Float32,
14(1000, 1000)))()
15result = $(HloAdd())(random, A)
16return result
17end
18
```


## **将Julia映射到XLA**

现在，可以将Julia代码编译到XLA，不过Julia不是用HLO运行的，而是根据Julia库提供的功能编写的。

好在，Julia使用多个调度可以很容易地表达如何用HLO实现标准库抽象。下面是一个简单的例子：


```
1# Matrix-Matrix and Matrix-Vector product
 2function Base.:*(A::XRTMatrix,
 3B::Union{XRTMatrix, XRTArray})
 4ddots = DimNums((1,), (0,), (), ())
 5HloDot(ddots)(A, B)
 6end
 7Base.transpose(A::XRTArray) =
 8HloTranspose((1,0))(A)
 9# Scalar addition
10Base.:+(A::XRTArray{T, (), 0},
11B::XRTArray{T, (), 0})
12where {T<:XLAScalar} =
13GenericHloOp{:add}(T, ())(A, B)
```


## **结果比对**

论文中举了VGG19正向传递和反向传递的例子。

VGG19正向传递使用Metalhead软件包Mike Innes＆Contributors（2018）中的VGG19实现，它利用了Flux Innes＆Contributors（2017）框架将熟悉的机器学习层（卷积层，密集层）转换为线性代数运算。

但是，重要的是，Flux框架中的每一层都只是一个常规函数，而这些函数又调用常规线性代数运算。因此用Flux表达的机器学习模型只需要简单的Julia函数，适用于本文所述的方法。

VGG反向传递则使用基于Zygote.jl编译器的AD框架Innes（2018）。Zygote对Julia代码进行操作，其输出也是Julia函数，适合重新引入Zygote以获得更高阶导数，但也适合编译到TPU。
![](https://pic4.zhimg.com/v2-800893301142158f7ffbde48e6de1d03_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='683'></svg>)
这张表格展示编译到XLA后，Metalhead.jl VGG19正向传递和反后传递的指令计数细分。

## **传送门**

**Automatic Full Compilation of Julia Programs and ML Models to Cloud TPUs**

作者：Keno Fischer, Elliot Saba

[https://arxiv.org/abs/1810.09868](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1810.09868)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动


