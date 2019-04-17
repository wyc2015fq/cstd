# Why C++ Supports both Class and Typename for Type Parameters - 长歌行 - CSDN博客





2013年03月12日 13:49:02[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1641
个人分类：[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)









[http://blogs.msdn.com/b/slippman/archive/2004/08/11/212768.aspx](http://blogs.msdn.com/b/slippman/archive/2004/08/11/212768.aspx)


Recently, someone asked me why we support both class and typename within C++ to indicate a type parameter since the keywords do not hold any platform significance – for example, class is
 not meant to suggest a native type nor is typename meant to suggest a *CLI* type.
 Rather, both equivalently indicate that the name following represents a parameterized type placeholder that will be replaced by a user-specfied actual type.

The reason for the two keywords is historical. In the original template specification, Stroustrup reused the existing class keyword to specify a type parameter rather than introduce
 a new keyword that might of course break existing programs. It wasn't that a new keyword wasn't considered -- just that it wasn't considered necessary given its potential disruption. And up until the ISO-C++ standard, this was the only way to declare a type
 parameter.

Reuses of existing keywords seems to always sow confusion. What we found is that beginners were whether the use of the class constrained or limited the type arguments a user
 could specify to be class types rather than, say, a built-in or pointer type. So, there was some feeling that not having introduced a new keyword was a mistake.

During standardization, certain constructs were discovered within a template definition that resolved to expressions although they were meant to indicate declarations. For example,

template <class T>

class Demonstration {

public:

void method() {

    T::A *aObj; // oops …

     // …

};

While the statement containing aObj is intended by the programmer to be interpreted as the declaration of a pointer to a nested type A within the type parameter T, the language
 grammar interprets it as an arithmetic expression multiplying the static member A of type T with aObj and throwing away the result. Isn't that annoying! (This sort of dilemna is not possible within generics – there is no way to safely verify that any T contains
 an A so that the runtime can safely construct an instance of the generic type.)

The committee decided that a new keyword was just the ticket to get the compiler off its unfortunate obsession with expressions. The new keyword was the self-describing typename.
 When applied to a statement, such as,

typename T::A* a6; // *declare pointer to *T*’s *A

it instructs the compiler to treat the subsequent statement as a declaration. Since the keyword was on the payroll, heck, why not fix the confusion caused by the original decision
 to reuse the class keyword. Of course, given the extensive body of existing code and books and articles and talks and postings using the class keyword, they chose to also retain support for that use of the keyword as well. So that's why you have both.



