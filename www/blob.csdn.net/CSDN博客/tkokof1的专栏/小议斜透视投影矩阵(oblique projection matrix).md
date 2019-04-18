# 小议斜透视投影矩阵(oblique projection matrix) - tkokof1的专栏 - CSDN博客

2019年04月07日 20:59:19[tkokof1](https://me.csdn.net/tkokof1)阅读数：38


> 
本文简介了斜透视投影矩阵(oblique projection matrix)相关的一些知识

Unity 的这篇[文档](https://docs.unity3d.com/Manual/ObliqueFrustum.html)提及了斜透视投影的一些内容,还列出了示例代码:

```
using UnityEngine;
using System.Collections;

public class ExampleScript : MonoBehaviour {
    void SetObliqueness(float horizObl, float vertObl) {
        Matrix4x4 mat  = Camera.main.projectionMatrix;
        mat[0, 2] = horizObl;
        mat[1, 2] = vertObl;
        Camera.main.projectionMatrix = mat;
    }
}
```

代码挺简单的,但是其中的原理文档中并未提及,本篇文章尝试简单讲解一下~

首先,我们要了解一下 **Camera.projectionMatrix** 这个矩阵的构成,简单起见,我们这里直接给出结论,有兴趣的朋友可以去看看完整的[推导过程](http://www.songho.ca/opengl/gl_projectionmatrix.html)(很好的一篇文章,目前似乎还没有译文,有时间自己来翻译一下)(**更新:自己简单翻译了一下,在[这里](https://blog.csdn.net/tkokof1/article/details/89152607)**):

Unity 中的 **Camera.projectionMatrix** 遵循 **OpenGL** 的规范约定,正常的透视投影情况下,该矩阵的构成如下:

$$\begin{bmatrix}\dfrac{2n}{r - l} & 0 & \dfrac{r + l}{r - l} & 0 \\0 & \dfrac{2n}{t - b} & \dfrac{t + b}{t - b} & 0 \\0 & 0 & -\dfrac{f + n}{f - n} & -\dfrac{2nf}{f - n} \\0 & 0 & -1 & 0\end{bmatrix}$$

其中
- $l$ 是左(垂直)裁剪面的坐标
- $r$ 是右(垂直)裁剪面的坐标
- $b$ 是下(水平)裁剪面的坐标
- $t$ 是上(水平)裁剪面的坐标
- $n$ 是近(深度)裁剪面的坐标
- $f$ 是远(深度)裁剪面的坐标

正常的透视投影情况下,我们有:

$$\begin{aligned}& r = -l \implies r + l = 0 \implies r - l = 2r & \hspace{20 mm} (1)\\& t = -b \implies t + b = 0 \implies t - b = 2t & \hspace{20 mm} (2)\\\end{aligned}$$

所以上面的矩阵可以简化为:

$$\begin{bmatrix}\dfrac{n}{r} & 0 & 0 & 0 \\0 & \dfrac{n}{t} & 0 & 0 \\0 & 0 & -\dfrac{f + n}{f - n} & -\dfrac{2nf}{f - n} \\0 & 0 & -1 & 0\end{bmatrix}$$

现在我们需要调整这个矩阵来达到斜透视投影的效果,怎么做呢?拿水平方向的斜透视举例,我们要做的其实就是 **偏移(shift)***左(垂直)裁剪面的坐标* 和 *右(垂直)裁剪面的坐标*,即偏移上面矩阵中的 $l$ 和 $r$, 假设我们偏移 $s$ 个坐标单位,则有:

$$\begin{aligned}& l' = l + s & \hspace{20 mm} (3) \\& r' = r + s & \hspace{20 mm} (4) \\\end{aligned}$$

考虑最开始的透视投影矩阵,由于我们变更了其中的 $l$ 和 $r$(变更为了 $l'$ 和 $r'$),所以新的(斜)透视投影矩阵变为:

$$\begin{bmatrix}\dfrac{2n}{r' - l'} & 0 & \dfrac{r' + l'}{r' - l'} & 0 \\0 & \dfrac{2n}{t - b} & \dfrac{t + b}{t - b} & 0 \\0 & 0 & -\dfrac{f + n}{f - n} & -\dfrac{2nf}{f - n} \\0 & 0 & -1 & 0\end{bmatrix}$$

将之前的 $(1),(2),(3),(4)$ 这四个等式代入计算,我们得到:

$$\begin{bmatrix}\dfrac{n}{r} & 0 & \dfrac{s}{r} & 0 \\0 & \dfrac{n}{t} & 0 & 0 \\0 & 0 & -\dfrac{f + n}{f - n} & -\dfrac{2nf}{f - n} \\0 & 0 & -1 & 0\end{bmatrix}$$

注意到相比之前简化的透视投影矩阵,只有一个矩阵元素发生了变化(**第一行第三列,即M[0, 2]**),从之前的 $0$ 变为了 $s/r$,而 $s/r$ 这个数值表示的则是(水平)倾斜度:
- $s/r = 0$ 即 $s = 0$,表示不进行偏移,即(水平)倾斜度为 $0$
- $s/r = 1$ 即 $s = r$,表示向右偏移整个右(垂直)裁剪面的坐标,即(水平)倾斜度为 $1$
- $s/r = -1$ 即 $s = -r = l$,表示向左偏移整个右(垂直)裁剪面的坐标,即(水平)倾斜度为 $-1$
- 其他的一些数值情况即代表不同的(水平)倾斜度

垂直方向的斜透视也同样可以依此分析,假设垂直方向的偏移量为 $s'$ 坐标单位,我们能够得到:

$$\begin{bmatrix}\dfrac{n}{r} & 0 & 0 & 0 \\0 & \dfrac{n}{t} & \dfrac{s'}{t} & 0 \\0 & 0 & -\dfrac{f + n}{f - n} & -\dfrac{2nf}{f - n} \\0 & 0 & -1 & 0\end{bmatrix}$$

可以看到,相比之前简化的透视投影矩阵,新的(斜)透视投影矩阵也仅有一个矩阵元素发生了变化(**第二行第三列,即M[1, 2]**),并且该元素的数值同样表示(垂直)倾斜度($s'/t$).

综上,如果我们给定了 **(水平)倾斜度** 和 **(垂直)倾斜度**,只要据此改变原透视投影矩阵的两个元素(设置 **第一行第三列,即M[0, 2]** 为(水平)倾斜度,设置 **第二行第三列,即M[1, 2]** 为(垂直)倾斜度)即可得到我们想要的斜透视投影矩阵~

讲到这里,如果再看一眼先前的示例代码的话,想必是一目了然了~

```
using UnityEngine;
using System.Collections;

public class ExampleScript : MonoBehaviour {
    void SetObliqueness(float horizObl, float vertObl) {
        Matrix4x4 mat  = Camera.main.projectionMatrix;
        mat[0, 2] = horizObl;
        mat[1, 2] = vertObl;
        Camera.main.projectionMatrix = mat;
    }
}
```

#### 相关资料
- Unity文档 [[1](https://docs.unity3d.com/Manual/ObliqueFrustum.html)] [[2](https://docs.unity3d.com/ScriptReference/Camera-projectionMatrix.html)] [[3](https://docs.unity3d.com/ScriptReference/GL.GetGPUProjectionMatrix.html)]
- [OpenGL Projection Matrix](http://www.songho.ca/opengl/gl_projectionmatrix.html)

