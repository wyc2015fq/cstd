# reinterpret_cast应用 - nosmatch的专栏 - CSDN博客
2012年05月09日 10:03:14[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：523标签：[class																[c																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[C++](https://blog.csdn.net/HDUTigerkin/article/category/1142667)
reinterpret_cast是C++里的[强制类型转换](http://baike.baidu.com/view/2886403.htm)符。
　　操作符修改了操作数类型,但仅仅是重新解释了给出的对象的比特模型而没有进行二进制转换。
　　例如：int *n= new int ;
　　double *d=reinterpret_cast<double*> (n);
　　在进行计算以后, d 包含无用值. 这是因为 reinterpret_cast 仅仅是复制 n 的比特位到 d, 没有进行必要的分析。
　　因此, 需要谨慎使用 reinterpret_cast.
　　== ===========================================
　　== [static_cast](http://baike.baidu.com/view/1745207.htm) .vs. reinterpret_cast
　　== ================================================
　　reinterpret_cast是为了映射到一个完全不同类型的意思，这个关键词在我们需要把类型映射回原有类型时用到它。我们映射到的类型仅仅是为了故弄玄虚和其他目的，这是所有映射中最危险的。(这句话是C++编程思想中的原话)
　　static_cast和reinterpret_cast的区别主要在于多重继承，比如
　　class A { public: int m_a; };
　　class B { public: int m_b; };
　　class C : public A, public B {};
　　那么对于以下代码：
　　C c;
　　printf("%p, %p, %p\r\n", &c, reinterpret_cast<B*>(&c), static_cast <B*>(&c));
　　前两个的输出值是相同的，最后一个则会在原基础上偏移4个字节，这是因为static_cast计算了父子类指针转换的偏移量，并将之转换到正确的地址（c里面有m_a,m_b，转换为B*指针后指到m_b处），而reinterpret_cast却不会做这一层转换。
　　因此, 你需要谨慎使用 reinterpret_cast.
