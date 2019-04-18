# 另一种(Yet Another)三角形线性插值方法 - tkokof1的专栏 - CSDN博客

2019年04月17日 23:35:23[tkokof1](https://me.csdn.net/tkokof1)阅读数：66标签：[数学																[三角形																[插值																[三角形插值																[重心坐标](https://so.csdn.net/so/search/s.do?q=重心坐标&t=blog)](https://so.csdn.net/so/search/s.do?q=三角形插值&t=blog)](https://so.csdn.net/so/search/s.do?q=插值&t=blog)](https://so.csdn.net/so/search/s.do?q=三角形&t=blog)](https://so.csdn.net/so/search/s.do?q=数学&t=blog)
个人分类：[数学																[算法																[随性](https://blog.csdn.net/tkokof1/article/category/642708)](https://blog.csdn.net/tkokof1/article/category/642709)](https://blog.csdn.net/tkokof1/article/category/8122393)


> 
本文简述了一种三角形线性插值的方法(本文仅讨论二维情况)

#### 相关问题

给定一个三角形的顶点坐标(**P0, P1 和 P2**)以及对应的数值(**V0, V1 和 V2**),插值求解三角形内一点(坐标给定为**P**)的数值(**V**).

问题说的有些抽象,给张图会清晰明了一些,图中的 **V** 即是我们想要插值求解的数值.

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190417232732771.png)

#### 插值方法

如何求解呢?一般我们是采用比例面积的方法,见下图(图中 **a0, a1 和 a2** 分别代表三个分割三角形的面积):

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019041723274673.png)

图中 **a0** 占三角形整体面积的比例代表 **V2**(**P2**对应的数值)占 **V** 的比例, **a1** 占三角形整体面积的比例代表 **V1**(**P1**对应的数值)占 **V** 的比例, 而 **a2** 占三角形整体面积的比例则代表 **V0**(**P0**对应的数值)占 **V** 的比例,假设三角形的整体面积为 **a**, 则我们有:

$$    \begin{aligned}    V & = \dfrac{a_0}{a} * V_2 + \dfrac{a_1}{a} * V_1 + \dfrac{a_2}{a} * V_0 \\ & = (a_0 * V_2 + a_1 * V_1 + a_2 * V_0) / a    \end{aligned}$$

接下来的问题就是如何根据三角形的顶点求解三角形的面积了,这里我们直接给出结论,有兴趣的朋友可以从[这里](https://en.wikipedia.org/wiki/Cross_product)开始了解,方法就是使用**叉积**:

假设三角形的三个顶点分别为 **P0, P1 和 P2**, 则三角形面积 **a** 的计算公式为:

$$    a = \dfrac{1}{2} * | (P_1 - P_0) \times (P_2 - P_0) |$$

如果 **P0** 的坐标为 **<x0, y0>**, **P1** 的坐标为 **<x1, y1>**, **P2** 的坐标为 **<x2, y2>**,则上面的公式可表达为:

$$    \begin{aligned}    a & = \dfrac{1}{2} * | (P_1 - P_0) \times (P_2 - P_0) | \\      & = \dfrac{1}{2} * |(<x_1, y_1> - <x_0, y_0>) \times (<x_2, y_2> - <x_0, y_0>) | \\      & = \dfrac{1}{2} * |(<x_1 - x_0, y_1 - y_0>) \times (<x_2 - x_0, y_2 - y_0>) | \\      & = \dfrac{1}{2} * |(x_1 - x_0) * (y_2 - y_0) - (x_2 - x_0) * (y_1 - y_0) |    \end{aligned}$$

这里需要说明的一点是,二维向量实际上是没有叉积定义的,但是我们可以将二维坐标点看做是三维坐标点(第三维取 0 即可)来进行求解,更多细节还是请参看[这里](https://en.wikipedia.org/wiki/Cross_product).

讲到这里,我们便可以进行实现了,参考代码如下:

```
public struct Value2<T>
{
    public float x;
    public float y;
    public T v;

    public Vector2 Vector
    {
        get
        {
            return new Vector2(x, y);
        }
    }

    public Value2(float x, float y, T v)
    {
        this.x = x;
        this.y = y;
        this.v = v;
    }
}
```

```
public static float Cross(Vector2 v0, Vector2 v1)
{
    return v0.x * v1.y - v1.x * v0.y;
}

public static float TriangleArea(Vector2 v0, Vector2 v1)
{
    return 0.5f * Math.Abs(Cross(v0, v1));
}

public static float TriangleLerp(Value2f val0, Value2f val1, Value2f val2, Vector2 p)
{
    var v01 = val1.Vector - val0.Vector;
    var v02 = val2.Vector - val0.Vector;
    var v0p = p - val0.Vector;

    var a = TriangleArea(v01, v02);
    Debug.Assert(a > 0, "[MathUtil]Error to do triangle Lerp, seems vertexes collinear ...");
    
    var a0 = TriangleArea(v01, v0p);
    var a1 = TriangleArea(v0p, v02);
    var a2 = a - a0 - a1;

    return (val2.v * a0 + val1.v * a1 + val0.v * a2) / a;
}
```

#### 另一种(Yet Another)插值方法

实际上我还尝试了一种类似于[双线性插值](https://en.wikipedia.org/wiki/Bilinear_interpolation)的求解方法,发现也是可行的,参考下图:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190417232814775.png)

其中的虚线线段平行于向量 **P2 - P1**(虚线取用其他线段也是可行的,只是计算上不方便),只要我们求解出 **P1’** 的对应数值 **V1’**, **P2’** 的对应数值 **v2’**,以及子线段(**P1’ 至 P**)占总线段(**P1’ 至 P2’**)的比例 **t**,则 **P** 点对应的数值 **V** 便可以用简单的线性插值来求解了:

$$    V = (1 - t) * V_1' + t * V_2'$$

我们可以采用解析法来求解上面所需的 **V1’**, **V2’** 和 **t**, 参考下图:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190417232825459.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

我们设红色向量部分(**P1’ - P**)等于 **t1 * (P2 - P1)**,黄色向量部分(**P1’ - P0**)等于 **t2 * (P1 - P0)**,由于相似三角形对应边成比例的关系,蓝色向量部分(**P2’ - P0**)的比例系数也为 **t2**,类似的,向量 **P2’ - P1’** 相对与向量 **P2 - P1** 的比例系数同样也为 **t2**.

我们知道:

$$    \begin{aligned}    & P_1' - P_0 = (P - P_0) + (P_1' - P) \implies \\    & t_2 * (P_1 - P_0) = (P - P_0) + t_1 * (P_2 - P_1)    \end{aligned}$$

并且 **P0** 的坐标为 **<x0, y0>**, **P1** 的坐标为 **<x1, y1>**, **P2** 的坐标为 **<x2, y2>**, **P** 的坐标为 **<x, y>**

则有:

$$\left\{\begin{aligned}t_2 * (x_1 - x_0) & = x - x_0 + t_1 * (x_2 - x_1) \\t_2 * (y_1 - y_0) & = y - y_0 + t_1 * (y_2 - y_1)\end{aligned}\right.$$

求解可得:

$$\left\{\begin{aligned}t_1 & = \dfrac{(x_1 - x_0) * (y - y_0) - (x - x_0) * (y_1 - y_0)}{(x_2 - x_1) * (y_1 - y_0) - (x_1 - x_0) * (y_2 - y_1)} = \dfrac{(P_1 - P_0) \times (P - P_0)}{(P_2 - P_1) \times (P_1 - P_0)} \\t_2 & = \dfrac{(x - x_0) + t_1 * (x_2 - x_1)}{x_1 - x_0} or \dfrac{(y - y_0) + t_1 * (y_2 - y_1)}{y_1 - y_0} \end{aligned}\right.$$

可以看到 **t1** 的计算公式是一个叉积比例的形式,其实这个形式除了使用先前的解析方法,也可以运用几何方法来进行求解,只是对思维的要求比较高,有兴趣的朋友可以自己尝试一下(提示:叉积->面积).

有了 **t1** 和 **t2**,我们就可以计算之前的 **V1’**, **V2’** 和 **t** 了:

$$12\begin{aligned}V_1' & = (1 - t_2) * V_0 + t_2 * V_1 \\V_2' & = (1 - t_2) * V_0 + t_2 * V_2 \\t &= | \dfrac{t_1 * (P_2 - P_1)}{t_2 * (P_2 - P_1)} | = |\dfrac{t_1}{t_2}|\end{aligned}$$

相关实现代码如下:

```
public static float TriangleLerpV2(Value2f val0, Value2f val1, Value2f val2, Vector2 p)
{
    var v01 = val1.Vector - val0.Vector;
    var v12 = val2.Vector - val1.Vector;
    var v0p = p - val0.Vector;
    
    var c1 = Cross(v01, v0p);
    var c2 = Cross(v12, v01);
    Debug.Assert(c2 != 0, "[MathUtil]Error to do triangle Lerp, seems vertexes collinear ...");
    var t1 = c1 / c2;
    var t2 = v01.x != 0 ? (v0p.x + t1 * v12.x) / v01.x : (v0p.y + t1 * v12.y) / v01.y;
    if (t2 == 0)
    {
        return val0.v;
    }
    else
    {
        var t3 = Math.Abs(t1 / t2);
        var lerp0 = (1 - t2) * val0.v + t2 * val1.v;
        var lerp1 = (1 - t2) * val0.v + t2 * val2.v;
        return (1 - t3) * lerp0 + t3 * lerp1;
    }
}
```

#### 方法对比

简单的测试对比发现,第二种插值方法较第一种**快 10% 左右**~

#### 更多资料
- [重心座标插值](https://blog.csdn.net/silangquan/article/details/21990713)
- [三角形内部线性插值方法](http://www.cnblogs.com/wangchengfeng/p/3453194.html)


