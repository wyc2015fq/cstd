# Python基本理念 - qq_30006593的博客 - CSDN博客





2017年09月28日 20:14:12[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：212








# 类

super函数：为了方便得初始化父类对象。 

之前 A(B) B.**init**(self)  

现在 A(B) super(A, self).**init**() 

如果需要改变父类为C，之前需要改两处，现在只需要改一次。
# 系统变量

[sys.path](https://www.zhihu.com/question/19930477)：决定从哪里寻找包




