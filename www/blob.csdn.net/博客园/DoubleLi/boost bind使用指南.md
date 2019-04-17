# boost bind使用指南 - DoubleLi - 博客园






bind - boost

头文件: boost/bind.hpp

bind 是一组重载的函数模板.
用来向一个函数(或函数对象)绑定某些参数. 
bind的返回值是一个函数对象.

它的源文件太长了. 看不下去. 这里只记下它的用法:

9.1 对于普通函数

假如有函数 fun() 如下: 
 void fun(int x, int y) {
  cout << x << ", " << y << endl;
 }
现在我们看看怎么用bind 向其绑定参数. 
对于像 fun 这样的普通函数. 若fun 有n个参数. 则 bind 需要 n+1 个参数: 原始函数的地址 以及 n个要绑定的参数.

第1种用法: 
向原始函数 fun 绑定所有的参数
 boost::bind(&fun, 3, 4)     // bind的实参表依次为: 要绑定的函数的地址, 绑定到fun的第一个参数值, 第二个参数值...
        // fun有多少个参数, 这里就要提供多少个.
表示将 3 和 4 作为参数绑定到 fun 函数. 
因为绑定了所有的参数. 现在我们调用bind所返回的函数对象:
 boost::bind(&fun, 3, 4)( );  //无参数. 
就会输出 3, 4

第2种用法: 
向原始函数 fun 绑定一部分参数
 boost::bind(&fun, 3, _1)    // bind的实参表依次还是: 要绑定的函数的地址, 要绑定到fun的第一个参数值, 然后注意
        // 因为我们不打算向fun绑定第2个参数(即我们希望在调用返回的Functor时再指定这个参数的值)
        // 所以这里使用 _1 来占位. 这里的 _1 代表该新函数对象被调用时. 实参表的第1个参数.
        // 同理下边还会用到 _2 _3 这样的占位符. 
这里只为fun绑定了第一个参数3. 所以在调用bind返回的函数对象时. 需要:
 boost::bind(&fun, 3, _1)(4);  //这个4 会代替 _1 占位符.
输出 3, 4
同理 boost::bind(&fun, _1, 3)(4); 
输出 4, 3

第3种用法:
不向 fun 绑定任何参数
 boost::bind(&fun, _1, _2)   // _1 _2 都是占位符. 上边已经说过了.
所以它就是 将新函数对象在调用时的实参表的第1个参数和第2个参数 绑定到fun函数.  
 boost::bind(&fun, _1, _2)(3, 4);    // 3将代替_1占位符, 4将代替_2占位符.
输出 3, 4
同理 boost::bind(&fun, _2, _1)(3, 4);   // 3将代替_1占位符, 4将代替_2占位符.
会输出 4, 3  
同理 boost::bind(&fun, _1, _1)(3);     // 3将代替_1占位符
会输出 3, 3

对于普通函数就这些. 对于函数对象. 如:
 struct Func {
  void operator()(int x) {
   cout << x << endl;
  }
 } f;
绑定的时候可能要指出返回值的类型:
 boost::bind<void>(f, 3)();  //指出返回值的类型 void


9.2 对于非静态成员函数

假如有:
 struct A {
  void func(int x, int y) {
   cout << x << "," << y << endl;
  }
 };

 A a;  
 A* pa = new A; //指针
 boost::shared_ptr<A> ptr_a(pa);  //智能指针.

现在要向像 A::func 这样的非静态成员函数绑定. 
若A::func有n个参数, 则 bind 要有 n+2 个参数: 指向成员函数fun的指针, 绑定到this的对象, n个参数.
如:  
 boost::bind(&A::func, a, 3, 4)();    //输出 3, 4
 boost::bind(&A::func, pa, 3, 4)();   //输出 3, 4
 boost::bind(&A::func, ptr_a, 3, 4)();//输出 3, 4
同样可以用 _1 这样的占位符. 如:
 boost::bind(&A::func, _1, 3, 4)(ptr_a);//输出 3, 4

可以看出. 不论传递给bind 的第2个参数是 对象. 对象指针. 还是智能指针. bind函数都能够正常工作.




9.3 bind嵌套

有个类如下. 记录人的信息:
 class Personal_info {
  string name_;
  int age_;
 public:
  int get_age();
  string name();
 };

 vector<Personal_info> vec; 
 ...
现在要对 vec 排序. 可以用 bind 函数做一个比较谓词
 std::sort(  
  vec.begin(),  
  vec.end(),  
  boost::bind( 
   std::less<int>(),    
   boost::bind(&personal_info::age,_1),     //_1 占位符是 sort 中调用比较函数时的第一个参数.
   boost::bind(&personal_info::age,_2)));   //_2 占位符是 sort 中调用比较函数时的第二个参数.


9.4 函数组合

假如有:
 vector<int> ints;
 ...
想用 std::count_if() 来求ints中有多少是 >5 且 <=10 的. 这在常规代码中通常就要写一个函数来实现这个谓词:
 if (i>5 && i<=10) ...
现在用bind则可以:
 std::count_if(  
  ints.begin(),  ints.end(),  
  boost::bind(    
   std::logical_and<bool>(),    
   boost::bind(std::greater<int>(),_1,5),    
   boost::bind(std::less_equal<int>(),_1,10)));



9.5 绑定到成员变量

有:
 map<int, string> my_map;
 my_map[0]="Boost";my_map[1]="Bind";
现在要输出所有元素的 second 成员. 也就是输出这些字符串. 其中的打印函数如下:
 void print_string(const string& s) {  
  std::cout << s << '/n';
 }
则可以:
 for_each(  
  my_map.begin(),  
  my_map.end(),  
  boost::bind(
   &print_string, 
   boost::bind(&std::map<int,std::string>::value_type::second,_1)
   )
  );

汗... 看不懂bind的源码. 也不知是如何实现这些功能的. 只能等<<boost源码剖析>>出来了.


 注意:
(以下补于08年6月3日)

boost::bind() 返回的函数对象会保存要绑定的实参. 而且总是拷贝一份以值的方式保存..
这主要是考虑到被绑定的实参的生命期.  
但这并不总是我们期望的. 例如有时我们希望它保存指针或引用:

有函数:
void f(int & x) { ++x; }
然后:
int n = 0;
bind(&f, n)();    //我们希望 n==1 . 但实际上没有这样...

要避免这种对象复制.  而要bind得到的函数对象保存实参的引用语义. 可以:
使用 boost::ref()  或 boost::cref() 如
bind(&f, ref(n))();        //OK,  执行后 n==1

如果是绑定一个对象到它的成员函数上. 如:
A a;
bind(&A::fun, a);       //则保存的是 a对象的拷贝.
要避免这种拷贝. 除了上面提到的 ref() 外, 也可以:
bind(&A::fun, &a);      //用指针.  反正用对象和用指针都可以. 而用指针可以避免对象拷贝的问题. 



注意: (以下补于6月10日)
bind () 的第一个参数——被绑定函数——是不被求值的. 如下例:

typedef void (*pf)(int);
std::vector<pf> v;  //v中有一些函数指针.
std::for_each(v.begin(), v.end(), bind(_1, 5));   //想实现 _1(5);  这样的调用. 但这样不行!

正确的做法是借助 boost::apply 模板(来自boost/bind/apply.hpp).  
apply也是一个函数对象. 它的作用如下:
apply<void> a;   //模板参数为函数对象的返回值类型.
a(x);       //相当于调用 x();
a(x, y);    //相当于调用  x(y);
a(x, y, z);  //相当于调用 x(y, z);
所以错误的bind应该写为:
std::for_each(v.begin(), v.end(), bind(apply<void>(), _1, 5));









