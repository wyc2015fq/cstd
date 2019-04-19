# c++ vector clear 和 析构函数_用户5706034127_新浪博客
||分类：[cPlusPlus](http://blog.sina.com.cn/s/articlelist_5706034127_1_1.html)|
转载：http://blog.csdn.net/luomoshusheng/article/details/48226517
总结一下：
- 
vector中存储了对象的指针，调用clear后，并不会调用这些指针所指对象析构函数，因此要在clear之前调用delete；
- 
如果vector存储的是对象，调用clear后，自建类型的对象(int之类的)直接删除，若是外部类型，则调用析构函数。
```
class Test  
{  
public:  
    Test() { cout<<"Test cons"<<endl;}  
    ~Test() { cout << "Test des" << endl; }  
};  
int main()  
{  
    vector vec;  
    Test* p1=new Test();  
    Test* p2=new Test();  
    Test* p3=new Test();  
    vec.push_back(*p1);  
    vec.push_back(*p2);  
    vec.push_back(*p3);  
    //对象如何进行释放，要调用已定义析构函数  
    vec.clear();  
    return 0;  
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
运行结果： 
![运行结果](http://img.blog.csdn.net/20150905165615038)
*对于这个结果也许你会觉得奇怪*，为什么push_back3次，却调用了6次析构函数呢？这个就涉及到vector的构造与内存管理，《STL源码剖析里面》讲得很仔细。 
vecor是这样分配内存的： 
const size_type len=old_size !=0?2*old_size
:1; 
**也就是说，vector的容量永远是大于或者等于size。而当内存不够的时候，要重新分配空间，于是，allocate(len)；再uninititalize_copy();之后再destroy(begin(),end());因此，这其中的内存变化是从1—>2—>4,所以当调用clear时，析构了6次。**
下面是从网上摘录的，实际为《STL源码剖析》内容：
```
// 清除全部元素。注意并未释放空间，以备可能未来还会新加入元素。  
 void clear() { erase(begin(), end()); }  
//调用vector::erase的两迭代器范围版本  
 iterator erase(iterator first, iterator last) {  
      iterator i = copy(last, finish, first);  
      //finish在vector中定义表示目前使用空间的尾，相当于end（），clear调用时last=finish  
      destroy(i, finish); //全局函数，结构的基本函数  
      finish = finish - (last - first);  
      return first;  
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
以上关键就是调用了destroy函数。destory函数在中定义，为了便于分析整个的构造与释放，将construct函数的内容也进行了摘录。这其中要注意的是traits技术。
```
// destroy()单指针版本  
template <<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">class
```
 T>  
inlinevoid destroy(T* pointer) {  
    pointer->~T();   // 唤起 dtor ~T()  
}  
// destroy()两迭代器版本  //利用 __type_traits<> 求取最适当措施。  template <<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">class ForwardIterator>  
inlinevoid destroy(ForwardIterator first, ForwardIterator last) {  
  __destroy(first, last, value_type(first));  
}  
//判断元素的数值型别（value type）有 non-trivial destructor(自定义析构函数)  template <<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">class ForwardIterator, class T>  
inlinevoid __destroy(ForwardIterator first, ForwardIterator last, T*) {  
  typedeftypename __type_traits::has_trivial_destructor trivial_destructor;  
  __destroy_aux(first, last, trivial_destructor());  
}  
// 如果元素的数值型别（value type）有 non-trivial destructor(自定义析构函数)  template <<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">class ForwardIterator>  
inlinevoid  
__destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {  
 for ( ; first < last; ++first)    //遍历元素进行析构  
    destroy(&*first);                  //!!!!!关键句!!!!!!!!!  
}  
//如果元素的数值型别（value type）有trivial destructor  template <<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">class ForwardIterator>   
inlinevoid __destroy_aux(ForwardIterator, ForwardIterator, __true_type) {}  
//什么都不做，STL是用了一种保守的方式，只有内建的元素类型（int，float等）进行判定trivial destructor的时候才是__true_type其他一切用户自定义类型都是__false_type  // destroy()两迭代器版本，针对char*与wchar_t*的特化版本  inlinevoid destroy(char*, char*) {}  
inlinevoid destroy(wchar_t*, wchar_t*) {}  
//仅仅是对placement new 的一层封装  template <<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">class T1, class T2>  
inlinevoid construct(T1* p, const T2& value) {  
  new (p) T1(value);    // placement new; 唤起 ctor T1(value);  
}  
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
看到这里基本对上述的问题已经有答案了。 
由于对象的指针不是内建对象， 
for ( ; first < last; ++first)
//遍历元素进行析构 
destroy(&*first); 
*iterator是元素类型，&*iterator是元素地址，也就是一个指针。之后调用&*iterator->~T();所以可知当vector中所存储的元素为对象的时候，调用clear（）操作的时候系统会自动调用析构函数。但是当存储元素是指针的时候，指针指向的对象就没法析构了。因此需要释放指针所指对象的话，需要在clear操作之前调用delete。
```
for(i= 0; i <　vItem.size();i++)
      delete vItem[i];
```
