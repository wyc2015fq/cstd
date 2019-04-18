# 用浏览器 JS 做 Path Tracing 路径跟踪渲染 - 知乎
# 



Path Tracing 路径跟踪渲染可以得到在物理上真实的光影效果，缺点是噪音往往略大，不过最近推出了一些深度网络降噪的方法，效果颇为不错。

这是很久以前写的 Demo，点击在浏览器中就可以直接运行（建议用电脑打开，在手机会很慢）：
[Starting...​blinkdl.github.io](https://link.zhihu.com/?target=https%3A//blinkdl.github.io/PathTracingJS/)
渲染10次后效果： 
![](https://pic4.zhimg.com/v2-664808c26db06a7301fe9a30d44cf437_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1600' height='900'></svg>)
渲染100次后效果： 
![](https://pic3.zhimg.com/v2-9343a35693e2c40410952f584be4a41a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1600' height='900'></svg>)
代码很简单，和 smallpt 差不多：
[BlinkDL/PathTracingJS​github.com![图标](https://pic2.zhimg.com/v2-14ff3f9bdcc35d88f5c4c0b21bacac1d_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/BlinkDL/PathTracingJS)
其中做了一点略微精细的处理降低噪音，比普通的例子收敛更快。

如果加入 WebGL 和 WebWorker 就可以快很多。有兴趣还可以研究怎么加 BDPT metropolis 等等。


