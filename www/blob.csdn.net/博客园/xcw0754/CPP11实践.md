# CPP11实践 - xcw0754 - 博客园
# [CPP11实践](https://www.cnblogs.com/xcw0754/p/9943765.html)
#### - 001 以lambda作为返回值该如何声明?
标准库提供了`function`模板可以直接声明，如`std::function<void (int, int)>`。如下函数`foo`返回了一个函数指针，该指针接受两个参数，返回类型为int。
```
std::function<int (string, int)> foo()
{
    auto handler = [] () { return 0; };
    return handler;
}
int main()
{
    auto func = foo();
    cout << func("abc", 1) << endl;
    return 0;
}
```
#### - 002 lambda可以捕获shared_ptr吗?
要知道，捕获变量是在执行lambda时才会赋值。如下是运行报错的程序，lambda函数捕获了一个`shared_ptr`类型临时变量`tmp`，当foo执行完时，tmp所指向的堆内存块已经被释放了，在执行`fun()`时就会报错了，因为tmp不存在。用引用捕获也是行不通的。这种场景下还是用回普通指针更合适。
```
class A {
public:
    A() {cout<<"A()"<<endl;};
    ~A() {cout<<"~A()"<<endl;};
    A(A *a) {cout<<"A(A *a)"<<endl;};
    void test(cout<<"123"<<endl;})
};
std::function<void ()> foo()
{
    std::shared_ptr<A> tmp = std::make_shared<A>(new A);
    auto handler = [tmp] () {    // 注意这里捕获了tmp
        tmp->test();
    };
    return handler;
}
void main(){
    auto fun = foo();
    fun();
}
```

