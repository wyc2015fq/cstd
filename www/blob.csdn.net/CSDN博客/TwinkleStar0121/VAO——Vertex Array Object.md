# VAO——Vertex Array Object - TwinkleStar0121 - CSDN博客





2018年11月24日 14:34:58[TwinkleStar0121](https://me.csdn.net/jvandc)阅读数：30标签：[Vertex Array Object																[VAO																[VAO & VBO](https://so.csdn.net/so/search/s.do?q=VAO & VBO&t=blog)
个人分类：[OpenGL](https://blog.csdn.net/jvandc/article/category/7830718)








### 文章目录
- [VAO 性能](#VAO__2)
- [0. VAO](#0_VAO_4)
- [.1 VAO是什么？](#1_VAO_6)
- [.2 为什么使用VAO？](#2_VAO_10)
- [.3 怎么使用VAO？](#3_VAO_15)




### VAO 性能

#### 0. VAO

VAO 的全称是 Vertex Array Object，我们可以称之为“顶点数组对象”，它和VBO有什么关系吗？精确的来讲，它们之间并没有什么联系，可能在使用VAO的过程中，我们会发现在绑定VAO后，通常也会绑定VBO，然后填充Vertex Attributes——顶点属性信息，由此会让我们觉得VAO和VBO之间会有什么必然的联系，其实不然，没有VAO，我们仅仅使用VBO也可以完成图元的绘制。接下来我们搞清楚什么是VAO，且我们为什么要使用它？

##### .1 VAO是什么？

我们看看官方《Vertex Specification》对”Vertex Array Object“的描述：一个”顶点数组对象“（Vertex Array Object）是一个OpenGL 对象，它存储了所有需要提供给 ”Vertex data“的”状态“信息。OpenGL在绘制的时候，需要知道顶点信息——”位置“、”颜色“、”法线“等可以在哪里取得（Slot），怎么取得（offset， stride），在VAO出现以前，我们在调用Draw Command绘制的之前需要调用glVertexAttribPointer(…)函数设置这些信息，并且每一次调用Draw Command都需要重新设置顶点状态信息，而有了VAO之后，我们便可以将这些顶点状态信息存储起来，需要绘制时，我们只需要在Draw Command调用之前bind vertex array。

总结：VAO 是一个”顶点属性状态对象“。

##### .2 为什么使用VAO？

为什么使用VAO？

上文已经提到了一点，VAO可以保存状态，避免绘制的时候重新设置、绑定 ”OpenGL State“，使用VAO和不适用VAO的差别是非常大的，引文[1]做了比较完整的性能分析。性能提升的一个原因是使用VAO每一次绘制的时候OpenGL API调用的的次数要少（bind vertex array 就够了）；VAO的另一个优势是它是一个状态集合对象，它可以一次性将GPU 程序需要的信息推送过去，这和一次性推送大量渲染状态变化（amount of state changes）给GPU是一个道理。

##### .3 怎么使用VAO？](https://so.csdn.net/so/search/s.do?q=VAO&t=blog)](https://so.csdn.net/so/search/s.do?q=Vertex Array Object&t=blog)




