# 使用STL vector 作为XNAMath快速灵活的SIMD数据容器 - 逍遥剑客 - CSDN博客
2013年12月29日 18:47:29[xoyojank](https://me.csdn.net/xoyojank)阅读数：2649
原文链接: [http://www.qsoftz.com/mirza/?p=59](http://www.qsoftz.com/mirza/?p=59)
### 简介
这篇文章是关于如何将传统的STL vector作为XNAMath类型容器使用. 因为SSE/SSE2指令集要求数据必须对齐到16字节的边界, 所以vector的分配器必须替换成一个可以对齐的内存分配器(x86架构). 本文适用于想在代码中引入新鲜空气的x86 Windows开发者, 你将会看到如何把强大的XNAMath和灵活的STL vector结合到一起.
### 背景
之前我在Gamasutra上读过一篇关于SIMD的精华文章, 标题是"Designing Fast Cross-Platform SIMD Vector Libraries", 出自Gustavo Oliveira之手. 它开阔了我的眼界, 并促使我开始思考可以从哪里优化自己的代码. 虽然我想获得SIMD的速度, 但又想保持灵活性, 以便省力地移植我的代码. 灵活的内存管理不管对于游戏还是编辑器的常用代码都是一件好事. 像网格顶点这样的内存密集性操作, 如: 网格加载, 连接, 断开等, 使用STL vector这样的线性容器更加简单和方便.
STL vector并不是设计为最快的线性容器, 主要是函数调用有开销, 也就是没有内联操作符的原因.  但在知道问题出在哪里后, 这个问题很容易解决, 见下面的示例代码.
在做过一些研究后, 我发现XNAMath最符合我做SIMD移植的要求. 它只有5个头文件, 全是内联的SSE/SSE2指令, 并且有完善的文档和支持. 最棒的是它可以像在Direct3D中那样应用在OpenGL中. 把矩阵转置成列主序的消耗并不是很大, 毕竟头文件就在那里, 实在不行还可以自己改嘛.
值得一提的是, XNAMath可以在不支持SIMD的平台上使用传统的FPU指令.
缺点就是这是个预处理指令, 你必须书写和编译相同的SSE/SSE2和FPU指令的代码, 并且测试哪一个版本适用于目标平台. 当然, 除非你想支持非SSE处理器, 否则不会遇到这种状况.
一个好友打赌说Microsoft很快就会把XNAMath包装成一个动态库, 对其进行保护, 不再开源免费. 但我不这么认为, 如果封装成动态库就不能内联SSE指令了.
不管怎样, 我还是向他们的开源精神致敬, 时间会说明一切的. 
### 内存布局
STL诞生的时候, SIMD没有像现在这么流行. 但是, 聪明的STL设计者们让vector的分配器可以按需自定义. x86架构的内存分布是8字节对齐的, 而x64架构则是16字节对齐. 这就是说, 内存分配的地址都可以被8或者16整除的. SIMD则适用于16字节对齐的数据. 在系统内存中分配16字节对齐的数据可以加速数据载入SIMD寄存器和从SIMD寄存器读取. 加载没有对齐的数据到SIMD寄存器存在转换开销, 会比加载对齐数据慢大约两倍左右.
### Vector的对齐分配器
vector类使用默认的分配器进行new和delete的内存操作. 在x86平台上, new操作符分配的内存是8字节对齐的. 如果想自定义内存分配, 那就需要重写分配器以支持16字节的内存对齐. 这意味着内存分配器使用的new和delete操作符必须替换成_mm_malloc和_mm_free指令.
下面是vector对齐分配器的完整代码:
```cpp
template <typename T, size_t N=16>
class AAllocator
{
public:
        typedef       T              value_type;
        typedef       size_t                 size_type;
        typedef       ptrdiff_t      difference_type;
        typedef       T             *pointer;
        typedef const T             *const_pointer;
        typedef       T             &reference;
        typedef const T             &const_reference;
        inline AAllocator() throw(){}
        template <typename T2>
        inline  AAllocator(const AAllocator<T2, N> &) throw(){}
        inline ~AAllocator() throw(){}
        inline pointer address(reference r)
        { return &r; }
        inline const_pointer address(const_reference r) const
        { return &r; }
        inline pointer allocate(size_type n)
        { return (pointer)_mm_malloc(n*sizeof(value_type), N); }
        inline void deallocate(pointer p, size_type)
        { _mm_free(p); }
        inline void construct(pointer p, const value_type & wert)
        { new(p) value_type(wert); }
        inline void destroy(pointer p)
        { /* C4100 */ p; p->~value_type(); }
        inline size_type max_size() const throw()
        { return size_type(-1) / sizeof(value_type); }
        template <typename T2>
        struct rebind { typedef AAllocator<T2, N> other; };
}
```
可以看到allocate和deallocate方法已经使用_mm_malloc和_mm_free重写成16字节对齐的了, 其它方法像构造取地址等都是STL所需要的.
### XNAMath的数据类型
虽然XNAMath有大量的数据类型, 但我们只对XMFLOAT4, XMFLOAT4A和XMVECTOR感兴趣. XMFLOAT4和XMFLOAT4A都是包含4个单精度浮点数的4D向量, 不同之处在于XMFLOAT4A是16字节对齐的. 想像一下一个XMFLOAT4A可使以栈或者全局类型保存对齐的SIMD寄存器结果. XMFLOAT4A不能在vector类中使用, 因为对齐类型不能按值传递. 函数参数也不是16位对齐的, 它会产生一个编译错误'C2719'. 
非对齐类型XMFLOAT4是一个包含4个单精度浮点数的结构体, 可以用在堆内存对齐的vector类中. 
XMVECTOR只是一个编译器友好的__m128的类型定义, 用在SSE/SSE2指令集中.
### 示例
下面的简单示例中STL vector包含了2000万的顶点 (**). 任务是对所有向量求和 *R = Vi + Vj* 其中 *i,j* 分别是偶数和奇数, 表示从 *i=0* 和*j=1*开始的顶点索引(0是偶数).
下面是求和的公式:
*SUM(R) = V(i=k) + V(j=k+1) , k=0,2,4…n-2*
### 代码
首先要包含 <xnamath.h> 和<vector> 头文件.
示例说明了怎样定义一个简短的类型标识 `vec_type`.`vec_type` 就是一个使用了新的AAllocator的 STL vector, 而vector 作为XMFLOAT4的容器. 注意这里使用XMFLOAT4代替XMFLOAT4A, 因为x86 8字节对齐的天性决定了16字节对齐的参数不能按值传递, 之前有提到.
```cpp
typedef std::vector<XMFLOAT4, AAllocator<XMFLOAT4>> vec_type
```
因为容器的大小在初始化时就知道了, 所以可以在构造函数中给出:
```cpp
// the number of elements to work with
#define N_VECTORS 20000000
// issues a call to AAllocator's::allocate with n=N_VECTORS
vec_type foo(N_VECTORS)
```
或者之后容器可以重置大小:
```cpp
// issues a call to AAllocator's::allocate with n=1vec_type foo;
// issues a call to AAllocator's::allocate with n=N_VECTORS
foo.resize(N_VECTORS)
```
为了填充向量数据 (x,y,z) 我使用 rand() 函数随机从0到9取值. 'w'分量如下都设置为1:
```cpp
// initialize vectors x,y,z and w.
for(int i=0; i<N_VECTORS; ++i)
{
    foo[i].x = float(rand() % 10);
    foo[i].y = float(rand() % 10);
    foo[i].z = float(rand() % 10);
    foo[i].w = 1;
}
```
注意上面的元素索引使用容器的'[]'操作符. 这不是最快的方式, 因为操作符函数调用的开销是值得关注的, 特别是在循环内部时.
为了解决这个问题, 容器中的元素可以通过指针进行引用:
```cpp
// get pointer to the beginning of an array (1st XMFLOAT4 element)
 XMFLOAT4 *v  = &foo[0];
```
参考求和公式, 结果会被保存在SIMD寄存器中, 并在循环内部进行更新. XMVectorSet方法可以调用SSE/SSE2指令指定x,y,z,w分量初始化一个SIMD寄存器.
```cpp
// storing sum in the SIMD register (initialized)
XMVECTOR res = XMVectorSet(0, 0, 0, 0);
```
在循环内部, 首先使用XMLoadFloat4A方法把对齐的操作数a和b加载进SIMD寄存器. 不要以为参数是XMFLOAT4A类型的, 我们的数据在容器中是XMFLOAT4类型(XMFLOAT4在堆上对齐, 而XMFLOAT4A因为'C2719'错误不能使用). 调用XMLoadFloat4A使用XMFLOAT4类型参数十分安全并且准确. 注意非对齐的变种XMLoadFloat4使用非对齐的XMFLOAT4会产生SIMD转换开销 (*).
这里需要额外的寄存器保存a+b的结果r, 而r会被累加到res寄存器.
下面是循环的代码:
```cpp
for(vec_type::size_type i=0; i<foo.size(); i+=2)
{
      // store a operand into SIMD 'a' register
      XMVECTOR a   = XMLoadFloat4A(v++);
      // store b operand into SIMD 'b' register
      XMVECTOR b   = XMLoadFloat4A(v++);
      // sum a + b into SIMD 'r' register
      XMVECTOR r   = XMVectorAdd(a, b);
      // sum res + r into 'res' register
      res = XMVectorAdd(res, r);
}
```
在循环结束后, 用下面的方法从SIMD中获取结果:
```cpp
XMFLOAT4A  f;// store res into FPU register
XMStoreFloat4A(&f, res)
```
注意上面的结果保存在栈上的对齐数据类型XMFLOAT4A中. 使用相同的函数调用可以直接更新STL容器中的XMFLOAT4类型 (从SIMD寄存器中).
### 结果
##### 1st. SSE/SSE2 使用指针引用元素:
vectors SSE addition executed in: *0.054484* sec.
result: x:89802048.0 y:89821688.0 z:89811992.0 w:20000000.0
##### 2nd. SSE/SSE2 使用 []操作符引用元素:
vectors SSE addition executed in: *0.061230* sec.
result: x:89802048.0 y:89821688.0 z:89811992.0 w:20000000.0
##### 3rd. Non-SSE, FPU 使用指针引用元素:
vectors SSE addition executed in: *0.143803* sec.
result: x:89802048.0 y:89821688.0 z:89811992.0 w:20000000.0
大约比编译器 /O2 优化过的FPU 指令有着250% 的速度提升. 换句话说, 如果正确地使用SSE/SSE2进行编码, 可以提升到原来3倍的速度 (取决于 FPU). 使用指针代替'[]'操作符访问容器内的数据大约有10%的速度提升.
这里 你可以下载到MSVC 2008的示例工程 (依赖XNAMath, 可以安装DirectX SDK获取).
(*) 加载16字节对齐的数据到SIMD寄存器, 或者从SIMD读取数据到16字节对齐的寄存器中使用的是比较快的SIMD汇编指令 `MOVAPS, 而非对齐的数据寄存器使用的是比较慢的``MOVUPS`.
(**) 我参考我的硬件规格设置的vector大小, 运行于 P4 Dual Core (x86 Family 6 Model 15 Stepping 6 GenuineIntel ~2133 Mhz).
