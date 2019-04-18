# ANSI C中取得结构体字段偏移量的常用方法 - ljx0305的专栏 - CSDN博客
2009年02月23日 15:06:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1512
假设在ANSI C程序中定义了一个名为MyStruct的结构类型，其中有一个名为MyField的字段,如何取得它在结构体中的偏移?
typedef struct MyStruct_tag
{
// some fields
  ...
  long MyField;
// other fields
  ...
} MyStruct;
    最容易想到的方法应该与如下代码差不多:
size_t GetOffset()
{
  MyStruct s;
  return (size_t)( (char*)(&s.MyField) - (char*)(&s) );
}
    这段代码确实能完成任务，但为了取得偏移值，函数不得不定义了一个MyStruct结构体实例，可这有必要吗？仔细想想，结构体的内存布局是在什么时候由谁决定的？没错，是编译器在编译期确定的，它一旦被确定就不会改变了，而依赖于结构体内存布局的字段偏移也就随之确定并不再改变。既然在编译阶段编译器就洞悉了内幕，那么完全有理由要求它在编译期为程序提供这些信息。如何做呢？请看下面的代码：
#define MY_OFFSET (size_t)&(((MyStruct*)0)->MyField)
    上面定义的MY_OFFSET宏就是要的MyField的偏移。这样强制转换后的结构指针怎么可以用来访问结构体字段？其实这个表达式根本没有也不打算访问MyField字段。ANSI C标准允许任何值为0的常量被强制转换成任何一种类型的指针，并且转换结果是一个NULL指针，因此((MyStruct*)0)的结果就是一个类型为MyStruct*的NULL指针。如果利用这个NULL指针来访问MyStruct的成员当然是非法的，但&(((MyStruct*)0)->MyField)的意图并非想存取MyField字段内容，而仅仅是计算当结构体实例的首址为((MyStruct*)0)时MyField字段的地址。聪明的编译器根本就不生成访问MyField的代码，而仅仅是根据MyStruct的内存布局和结构体实例首址在编译期计算这个(常量)地址，这样就完全避免了通过NULL指针访问内存的问题。又因为首址的值为0，所以这个地址的值就是字段相对于结构体基址的偏移。
    如上做法避免了一定要实例化一个MyStruct对象，并且求值是在编译期进行，没有运行期负担。实际上这种利用编译器掌握的整个程序的信息以在编译期计算某些值的方法与现在C++编程中很流行的(静态)元编程技术类似，只不过C++程序员可以利用模板技术在编译期完成非常复杂的计算，而缺乏模板支持的ANSI C在这方面的能力则要弱许多。
    或许因为求结构体字段偏移很常用，ANSI C在标准头文件stddef.h中就专门定义了一个形如offsetof(s,m)的宏来求任意一个结构类型中某个字段的偏移，而且绝大多数C开发系统的实现都采用了上述的方法，例如：
// VC7.1
#ifdef  _WIN64
#define offsetof(s,m)   (size_t)( (ptrdiff_t)&(((s *)0)->m) )
#else
#define offsetof(s,m)   (size_t)&(((s *)0)->m)
#endif
//lcc-win32, last updated:Monday, 13-Dec-2004 04:05:23 EST
#define offsetof(s,m) (int)&(((s *)0)->m)
//Borland C++ 5.5.1 for WIN32
#define offsetof( s_name, m_name )  (_SIZE_T)&(((s_name _FAR *)0)->m_name)
//MinGW 3.1.0 (GCC 3.2.3)
#ifndef __cplusplus
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#else /* C++ */
/* The reference cast is necessary to thwart an operator& that might
   be applicable to MEMBER´s type.  See DR 273 for details.  */
#define offsetof(TYPE, MEMBER) (reinterpret_cast <size_t> /
    (&reinterpret_cast <char &>(static_cast <TYPE *> (0)->MEMBER)))
#endif /* C++ */
    可见这种简练而有效的方法已被C程序员接纳为一种惯用法(idiom)了。
引用:[http://blog.chinaunix.net/u2/62910/showart_492571.html](http://blog.chinaunix.net/u2/62910/showart_492571.html)
