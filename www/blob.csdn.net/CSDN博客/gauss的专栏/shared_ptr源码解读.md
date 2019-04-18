# shared_ptr源码解读 - gauss的专栏 - CSDN博客
2013年01月08日 12:53:51[gauss](https://me.csdn.net/mathlmx)阅读数：291
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)

1  介绍
众所周知，auto_ptr由于它的破坏性复制语义，无法满足标准容器对元素的要求，因而不能放在标准容器中；如果我们希望当容器析构时能自动把它容纳的指针元素所指的对象删除时，通常采用一些间接的方式来实现，显得比较繁琐。boost库中提供了一种新型的智能指针 shared_ptr，它解决了在多个指针间共享对象所有权的问题，同时也满足容器对元素的要求，因而可以安全地放入容器中。现在我们可以这样写：
vector< shared_ptr<Book> > books;
books.push_back( shared_ptr<Book>(new Book()) );
2  实现
shared_ptr是怎么实现的？让我们分析一下它的源码吧：
按照上面代码，在创建了Book对象之后，会创建一个临时的shared_ptr<Book>对象，shared_ptr的构造函数被调用： 
template<class Y>
explicit shared_ptr(Y * p)
  : px(p), pn(p, checked_deleter<Y>())
{
    detail::sp_enable_shared_from_this(p, p, pn);
}
显然这是个模板函数，推导出的Y应该为Book类型，px和pn为shared_ptr的成员变量，其声明如下： 
template<class T> class shared_ptr
{
private:
    T * px;                     // contained pointer
    detail::shared_count pn;        // reference counter
}
由于我们是用Book为模板参数实例化shared_ptr(shared_ptr<Book>)，px应该为 Book* 类型，px(p)把px初始化为指向创建的Book 对象。类型为shared_count的pn成员变量以两个参数初始化，一个是传入的Book对象指针，另一个是一个临时的  check_deleter<Book> 对象，shared_count的构造函数被调用：
template<class P, class D>
shared_count(P p, D d)
: pi_(0)
{
    try
    {
        pi_ = new sp_counted_base_impl<P, D>(p, d);
    }
    catch(...)
    {
        d(p); // delete p
        throw;
    }
}
应该推导出P为Book*类型，D为checked_deleter<Book>类型。在shared_count的构造函数体，创建了一个  sp_counted_base_impl<P,D> 的对象，以传入的 Book 对象指针和
 check_deleter<Book> 对象为构造函数参数。看起来 sp_counted_base_impl 像是一个实现类： 
template<class P, class D>
class sp_counted_base_impl: public sp_counted_base
{
private:
    P ptr; // copy constructor must not throw
    D del; // copy constructor must not throw
public:
    sp_counted_base_impl(P p, D d): ptr(p), del(d)
    {
    }
};
sp_counted_base的构造函数应该先被调用：
class sp_counted_base {
public:
    sp_counted_base(): use_count_(1), weak_count_(1)
    {
    }
private:
    long use_count_;        // #shared
    long weak_count_;       // #weak + (#shared != 0)
};
看来sp_counted_base是保存引用计数的地方，这里use_count_和weak_count_被初始化为1。接下来  sp_counted_base_impl 的构造函数保存了 Book 对象的指针和 check_deleter<Book> 对象。用类图把几个主要类之间的关系表示出来：
![](http://blog.csdn.net/images/blog_csdn_net/hansoft/110108/r_class.jpg)
当books.push_back被调用时，应该发生一次拷贝构造： 
template<class Y>
shared_ptr(shared_ptr<Y> const & r)
    : px(r.px),pn(r.pn) // never throws
{ }
pn为shared_count类型，它的拷贝构造函数又被调用：
shared_count(shared_count const & r)
    : pi_(r.pi_) // nothrow
{
    if(pi_!= 0) pi_->add_ref_copy();
}
可见新创建的shared_ptr对象和被拷贝的shared_ptr对象拥有了指向同一个 sp_counted_base 对象的指针，并通过调用  sp_count_base 的add_ref_copy函数把引用计数加一：
void add_ref_copy() { ++use_count_; }
用对象图把此时刻的对象关系表示出来：
当临时share_ptr对象被复制到容器中后，它就应该被销毁了，由于没有显式定义析构函数，编译器生成的析构函数会析构pn成员： 
~shared_count() // nothrow
{
    if(pi_ != 0) pi_->release();
}
shared_counted_base的release函数：
void release() // nothrow
{
    {
        long new_use_count = --use_count_;
       if(new_use_count != 0) return;
    }
    dispose();
    weak_release();
}
这里把sp_counted_base保存的引用计数减一，引用计数减为1而返回。于是临时创建的shared_ptr对象被销毁了，但是Book对象并没有被销毁。当vector中的shared_ptr对象也被析构时，引用计数会减为0，于是执行dispose函数：
virtual void dispose() = 0; // nothrow
纯虚函数？别忘了pi_实际指向的对象类型为 sp_counted_base_impl ，所以 sp_counted_base_impl 的  dispose 函数被调用：
virtual void dispose() // nothrow
{
    del(ptr);
}
根据前面分析，del是一个check_deleter<Book>对象： 
template<class T>
struct checked_deleter {
    typedef void result_type;
    typedef T * argument_type;
    void operator()(T * x) const
    {
        // boost:: disables ADL
        boost::checked_delete(x);
    }
};
好像是个function object，传入了Book对象的指针，通过调用 check_delete 函数从而删除了 shared_ptr 管理的Book对象。可以看到check_deleter没有任何成员变量，因而上面发生的几次拷贝构造应该是没有开销的。sp_counted_base和 sp_counted_base_impl好像用了什么模式？对了，就是template method，它使引用计数的算法独立于对象删除的实现细节而被重用。
shared_ptr是线程安全的吗？是的，它使用了一种轻量级的mutex做同步，为简单起见我没有列出相关的代码，实际是比较简单的。至于效率，好像多了很多操作，会不会比较慢？经验告诉我们：不要做事先优化。当真正发现性能瓶颈时再去做优化会更有的放矢。对于一些资源，以指针方式表示，但需要以特定的方式释放，比如文件句柄，shared_ptr有没有办法管理？答案是肯定的，你只需要提供一个函数或function object来实现那个特定的释放逻辑，然后在构造shared_ptr对象时传进去就可以了：
void file_closer(FILE* f) { fclose(f); }
shared_ptr<FILE> file(fopen("/a.txt", "r"), file_closer);
这里，shared_ptr的第二个模板构造函数就起作用了： 
template<class Y,
class D> shared_ptr(Y * p, D d)
    : px(p), pn(p, d)
{
    detail::sp_enable_shared_from_this(p, p, pn);
}
推导出D应该为void(*)(FILE*)函数指针类型，当在该文件句柄上的引用计数为0时，file_closer就会被调用，从而关闭文件。
3  结论
shared_ptr解决了在多个指针间共享对象所有权的问题，它可以安全的放入标准容器，并且是线程安全的。 
