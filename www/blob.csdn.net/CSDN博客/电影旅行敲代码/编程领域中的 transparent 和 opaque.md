# 编程领域中的 "transparent" 和 "opaque" - 电影旅行敲代码 - CSDN博客
2016年06月17日 22:25:12[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：3842
# 引言
在学习计算机的过程中，经常会接触到 **“透明”** 和 **“非透明”** 的概念。
刚开始理解 **“透明”** 这个概念的时候，认为 **“透明”** 就是程序员可以看见其中的构造，但是老师却说透明是程序员***意识不到其中的存在***，所以对这个概念一直都有些困惑。
后面接触到 **“不透明”** 这个概念的时候，意味着程序员***看不到其中的机理***。既然 “transparent” 和 “opaque” 都有对看到其中构造的意思，那么两者有什么区别呢。
## transparent 透明
**“透明”** 意味着，程序员意识不到其中的存在，例如在程序员在使用IDE进行编码的时候，***IDE中的编译器对程序员来说就是透明的***，入门程序员根本不知道 **编译器**的存在。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160617221134030)
> 
**transparent** is being used to mean *hidden* in the sense of things taking place automatically behind scenes(i.e. without user of the code or the program having to interact). 
**Transparent** is used where something is present, but you can’t see it.
就像上面英文所述，”透明” 的事物一般是藏在表象后面的，用户（程序员）一半不会直接接触到该事物。
## Opaque 不透明
**不透明**意味着，程序员直接与该事物接触，但是不知道其中的机理，例如IDE对于程序员来说就是 **不透明** 的。程序员直接接触到IDE，但是却不知道IDE是如何实现的，有哪些模块组成。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160617222134448)
> 
**Opaque** is also being used to mean *hidden*, which is perhaps where the confusion comes in. 
**Opaque** is used where is present, but you can’t see *inside* it to inspect its inner workings.
“不透明” 的事物一般直接与程序员接触，但是程序员却不知道个中奥秘。
