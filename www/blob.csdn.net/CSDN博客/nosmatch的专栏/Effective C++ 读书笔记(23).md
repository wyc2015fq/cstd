# Effective C++ 读书笔记(23) - nosmatch的专栏 - CSDN博客
2013年07月14日 15:59:04[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：512
条款23：宁以non-member、non-friend替代member函数
> 
在C++中封装性是其三大特性之一，本条款可以看作是最大限度的增加class的封装性，在C++中member函数包括friend
函都是由对类的成员变量的访问权限，而相对应的non-member函数和non-friend函数都是没有权限的，这样在封装性上肯定是后
者对其封装效果好，因此如果一个class的member函数可以用一个non-member函数完整的实现，则尽量的使用non-member函数和
non-friend函数，当然这个函数可以是另一个类的成员函数或者friend函数，不过常见的用法是和该类作为同一个namespace的成员
例如：
> 
```cpp
namespace WebBrowsStuff{
      class webBrowser{...};
      void clearBrowser(webBrowser& wb);
        ...
}
```
在该namespace空间中，clearBrowser类作为一个non-member函数来对class webBrowser来进行处理！其中namespace
是可以在不同的源码文件中存在的，这样如果我们对webBrowser处理的另外一个对不同客户使用的non-member函数我们可以
放在其他的源码文件中，我们只要能保证namespace的名字相同就行，这样我们就能很方便的对处理函数进行扩展！
请记住：
- 宁可拿non-member 和 non-friend函数来替换member函数，这样做可以增加封装性，包裹弹性和技能扩充性！
