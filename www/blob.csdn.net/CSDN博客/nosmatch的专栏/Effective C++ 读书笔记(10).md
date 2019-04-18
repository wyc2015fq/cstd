# Effective C++ 读书笔记(10) - nosmatch的专栏 - CSDN博客
2013年07月10日 09:31:43[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：776
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款10：让操作符=返回一个reference to *this
> 
对一些存在的操作符重载的时候，最好是要兼容这些操作符本身就有的属性，例如对于操作符=，可以实现连等的操作，例如
int a = b = c = 10；这样可以认为10先赋值给c，然后c的值又赋值给b，b的值最后赋值给a，同样的对于类中操作符=的重载也要支持
这样的操作，那么可以将operator返回操作符的左值，为了效率问题，返回左值的一个引用，例如：
> 
```cpp
class People{
public:
	People& operator=(const People& rhs){
		.....
		return  *this;
	}
	...
private:
	...
};
```
此外对于类似的+=，-=等也要用同样的准则，不过如果不遵守这个准则，编译也能通过，但是兼容性不理想，因为在库中存在的
类中的operator=操作符都是遵循这个原则的，所以没有特殊的原因不要另辟蹊径！
请记住：
- 领操作符operator=返回一个reference to *this
